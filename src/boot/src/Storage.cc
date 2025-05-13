/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <boot/Logger.hh>
#include <boot/Memory.hh>
#include <boot/Storage.hh>
#include <wesos-builtin/Memory.hh>

using namespace wesos::boot::efi;

File::~File() {
  if (m_file_protocol != nullptr) {
    NOTE << u"Closing file: " << m_file_path << ENDL;
    m_file_protocol->m_close(m_file_protocol);

    const auto& gbs = SYSTEM_TABLE_GLOBAL->boot_services();

    const void* const_file_content = bit_cast<const void*>(m_file_content.into_ptr().unwrap());
    void* file_content = const_cast<void*>(const_file_content);

    gbs.m_free_pool(file_content);
  }
}

Storage::~Storage() {
  if (m_volume != nullptr) {
    NOTE << u"Closing file system manager" << ENDL;
    m_volume->m_close(m_volume);
  }
}

auto Storage::create(Handle image) -> Nullable<Storage> {
  const auto& gbs = SYSTEM_TABLE_GLOBAL->boot_services();

  NOTE << u"Creating file system manager" << ENDL;

  /** Locate the loaded image protocol */
  const auto loaded_image_protocol = [&]() -> NullableRefPtr<LoadedImageProtocol> {
    void* loaded_image = nullptr;

    /** We need this to determine the volume that the current app is running on */
    const OpStatus status = gbs.m_handle_protocol(image, &LOADED_IMAGE_PROTOCOL_GUID, &loaded_image);
    if (status.is_failure()) [[unlikely]] {
      FAIL << u"Failed to locate the Loaded Image Protocol: code " << status.get_status() << ENDL;
      return nullptr;
    }

    auto* protocol = static_cast<LoadedImageProtocol*>(loaded_image);
    NOTE << u"Loaded image protocol revision: " << protocol->revision() << ENDL;

    return protocol;
  }();

  if (loaded_image_protocol.is_null()) [[unlikely]] {
    FAIL << u"Failed to locate the Loaded Image Protocol" << ENDL;
    return nullptr;
  }

  //===============================================================================================

  /** Locate the simple file system protocol */
  const auto file_system_protocol_instance = [&]() -> NullableRefPtr<SimpleFileSystemProtocol> {
    void* fs_protocol = nullptr;
    auto* device_handle = loaded_image_protocol->device_handle();

    /** We need this to open the volume */
    const OpStatus status = gbs.m_handle_protocol(device_handle, &SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, &fs_protocol);
    if (status.is_failure()) [[unlikely]] {
      FAIL << u"Failed to locate Simple File System Protocol: code " << status.get_status() << ENDL;
      return nullptr;
    }

    auto* protocol = static_cast<SimpleFileSystemProtocol*>(fs_protocol);
    NOTE << u"Simple file system protocol revision: " << protocol->revision() << ENDL;

    return protocol;
  }();

  if (file_system_protocol_instance.is_null()) [[unlikely]] {
    FAIL << u"Failed to locate Simple File System Protocol" << ENDL;
    return nullptr;
  }

  //===============================================================================================

  /** Open the volume */
  const auto volume = [&]() -> NullableOwnPtr<FileProtocol> {
    FileProtocol* volume = nullptr;

    const OpStatus status = file_system_protocol_instance->open_volume(&volume);
    if (status.is_failure()) {
      FAIL << u"Failed to open volume: code " << status.get_status() << ENDL;
      return nullptr;
    }

    return volume;
  }();

  if (volume.is_null()) [[unlikely]] {
    FAIL << u"Failed to open volume" << ENDL;
    return nullptr;
  }

  //===============================================================================================

  auto instance = Storage(*file_system_protocol_instance, *volume);
  INFO << u"Created file system manager" << ENDL;

  return instance;
}

auto Storage::open_file(const u16* filepath) -> Nullable<File> {
  assert_always(m_volume != nullptr);

  const auto& gbs = SYSTEM_TABLE_GLOBAL->boot_services();

  /* Open the file */
  const auto file = [&]() -> NullableOwnPtr<FileProtocol> {
    FileProtocol* file = nullptr;

    const auto fixed_filemode = EFI_FILE_MODE_READ;

    const OpStatus status = m_volume->m_open(m_volume, &file, filepath, fixed_filemode, 0);
    if (status.is_failure()) [[unlikely]] {
      FAIL << u"Failed to open file: code " << status.get_status() << ENDL;
      return nullptr;
    }

    NOTE << u"Opened file: " << filepath << ENDL;

    return file;
  }();

  if (file.is_null()) [[unlikely]] {
    FAIL << u"Failed to open file" << ENDL;
    return nullptr;
  }

  //===============================================================================================

  const usize file_info_size = [&] {
    /* The FileInfo structure is apparently variable size and a
       function of the file name length. */

    const auto base_structure_size = sizeof(FileInfo);
    const auto file_name_length = strlen16(filepath) + 1;
    const auto file_name_size = file_name_length * sizeof(u16);
    const auto file_info_size = base_structure_size + file_name_size;

    return file_info_size;
  }();

  const auto file_info = [&]() -> NullableOwnPtr<FileInfo> {
    FileInfo* file_info = nullptr;
    void** file_info_vp = bit_cast<void**>(&file_info);

    const OpStatus status = gbs.m_allocate_pool(LoaderData, file_info_size, file_info_vp);
    if (status.is_failure()) [[unlikely]] {
      FAIL << u"Failed to allocate memory for file info: code " << status.get_status() << ENDL;
      return nullptr;
    }

    memset(file_info, 0, file_info_size);

    return file_info;
  }();

  if (file_info.is_null()) [[unlikely]] {
    FAIL << u"Failed to allocate memory for file info" << ENDL;
    file->m_close(file);
    return nullptr;
  }

  //===============================================================================================

  {
    auto file_info_size_tmp = file_info_size;

    const OpStatus status = file->m_get_info(file, &EFI_FILE_INFO_GUID, &file_info_size_tmp, file_info);
    if (status.is_failure()) {
      FAIL << u"Failed to get file info: code " << status.get_status() << ENDL;
      file->m_close(file);
      gbs.m_free_pool(file_info);
      return nullptr;
    }

    NOTE << u"File content size: " << file_info->file_size() << ENDL;
  }

  const auto file_size = file_info->file_size();
  gbs.m_free_pool(file_info);

  //===============================================================================================

  auto file_content = [&]() -> NullableOwnPtr<u8> {
    void* file_content = nullptr;

    const OpStatus status = gbs.m_allocate_pool(LoaderData, file_size, &file_content);
    if (status.is_failure()) {
      FAIL << u"Failed to allocate memory for file content: code " << status.get_status() << ENDL;
      file->m_close(file);
      return nullptr;
    }

    return static_cast<u8*>(file_content);
  }();

  if (file_content.is_null()) [[unlikely]] {
    FAIL << u"Failed to allocate memory for file content" << ENDL;
    file->m_close(file);
    return nullptr;
  }

  //===============================================================================================

  {
    auto bytes_read = file_size;

    const OpStatus status = file->m_read(file, &bytes_read, file_content);
    if (status.is_failure()) {
      FAIL << u"Failed to read file: code " << status.get_status() << ENDL;
      file->m_close(file);
      gbs.m_free_pool(file_content);
      return nullptr;
    }

    if (file_size != bytes_read) {
      FAIL << u"Failed to read the entire file: expected " << file_size << u" bytes, but read " << bytes_read
           << u" bytes" << ENDL;
      file->m_close(file);
      gbs.m_free_pool(file_content);
      return nullptr;
    }
  }

  NOTE << u"Read file: " << filepath << ENDL;

  //===============================================================================================

  const auto content = View<const u8>(file_content.unwrap(), file_size);

  // Ownership of the file content is transferred to the File object.
  // Sorry, no std::unique_ptr available in this environment.
  auto instance = File(*file, filepath, content);
  INFO << u"Created virtual file object for " << filepath << ENDL;

  return instance;
}

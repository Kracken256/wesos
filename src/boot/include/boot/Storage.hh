/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <boot/EFI.hh>
#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
  class File {
    friend class Storage;

    FileProtocol* m_file_protocol;
    const u16* m_file_path;
    View<const u8> m_file_content;

    File(FileProtocol& file_protocol, const u16* file_path, View<const u8> content)
        : m_file_protocol(&file_protocol), m_file_path(file_path), m_file_content(content) {}

  public:
    File(const File&) = delete;
    File(File&& o) : m_file_protocol(o.m_file_protocol), m_file_path(o.m_file_path) { o.m_file_protocol = nullptr; }
    auto operator=(const File&) -> File& = delete;
    auto operator=(File&& o) -> File& {
      if (this != &o) {
        m_file_protocol = o.m_file_protocol;
        m_file_path = o.m_file_path;
        o.m_file_protocol = nullptr;
        o.m_file_path = nullptr;
      }
      return *this;
    }
    ~File();

    [[nodiscard]] auto get_size() const -> usize { return m_file_content.size(); }
    [[nodiscard]] auto get_content() const -> View<const u8> { return m_file_content; }
  };

  class Storage {
    SimpleFileSystemProtocol* m_file_system_protocol;
    FileProtocol* m_volume;

    Storage(SimpleFileSystemProtocol& file_system_protocol, FileProtocol& volume)
        : m_file_system_protocol(&file_system_protocol), m_volume(&volume) {}

  public:
    Storage(const Storage&) = delete;
    Storage(Storage&& o) : m_file_system_protocol(o.m_file_system_protocol), m_volume(o.m_volume) {
      o.m_file_system_protocol = nullptr;
      o.m_volume = nullptr;
    }
    auto operator=(const Storage&) -> Storage& = delete;
    auto operator=(Storage&& o) -> Storage& {
      if (this != &o) {
        m_file_system_protocol = o.m_file_system_protocol;
        m_volume = o.m_volume;
        o.m_file_system_protocol = nullptr;
        o.m_volume = nullptr;
      }
      return *this;
    }
    ~Storage();

    /** The filepath parameter must outlive the returned File object */
    [[nodiscard]] auto open_file(const u16* filepath) -> Nullable<File>;

    [[nodiscard]] static auto create(Handle image) -> Nullable<Storage>;
  };
}  // namespace wesos::boot::efi

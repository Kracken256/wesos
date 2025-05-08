/*
     $$$$$$\                            $$\     $$$$$$\   $$$$$$\
    $$  __$$\                           $$ |   $$  __$$\ $$  __$$\
    $$ /  \__| $$$$$$\  $$$$$$$\   $$$$$$$ |   $$ /  $$ |$$ /  \__|
    \$$$$$$\   \____$$\ $$  __$$\ $$  __$$ |   $$ |  $$ |\$$$$$$\
     \____$$\  $$$$$$$ |$$ |  $$ |$$ /  $$ |   $$ |  $$ | \____$$\
    $$\   $$ |$$  __$$ |$$ |  $$ |$$ |  $$ |   $$ |  $$ |$$\   $$ |
    \$$$$$$  |\$$$$$$$ |$$ |  $$ |\$$$$$$$ |    $$$$$$  |\$$$$$$  |
     \______/  \_______|\__|  \__| \_______|    \______/  \______/

    Copyright (C) 2023 - Present Sand OS Project. All rights reserved.

    This file is part of the Sand Operating System. This content
    is protected by Copyright law. See the LICENSE file for more
    information.

    Created by:
        Wesley C. Jones

    Edited by:
        * Wesley C. Jones
*/

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
  using Handle = void *;
  using Status = u64;
  using Event = void *;

  static constexpr auto W_EFI_SUCCESS = 0x0000000000000000;
  static constexpr auto W_EFI_FAILURE = 0x8000000000000000;
  static constexpr auto W_EFI_BUFFER_TOO_SMALL = 0x8000000000000005;

  static constexpr auto W_EFI_FILE_MODE_READ = 0x0000000000000001;
  static constexpr auto W_EFI_FILE_MODE_WRITE = 0x0000000000000002;
  static constexpr auto W_EFI_FILE_MODE_CREATE = 0x8000000000000000;
  static constexpr auto W_EFI_FILE_READ_ONLY = 0x0000000000000001;
  static constexpr auto W_EFI_FILE_HIDDEN = 0x0000000000000002;
  static constexpr auto W_EFI_FILE_SYSTEM = 0x0000000000000004;
  static constexpr auto W_EFI_FILE_RESERVED = 0x0000000000000008;
  static constexpr auto W_EFI_FILE_DIRECTORY = 0x0000000000000010;
  static constexpr auto W_EFI_FILE_ARCHIVE = 0x0000000000000020;
  static constexpr auto W_EFI_FILE_VALID_ATTR = 0x0000000000000037;

  static constexpr auto W_EVT_NOTIFY_SIGNAL = 0x00000200;
  static constexpr auto W_TPL_CALLBACK = 8;
  static constexpr auto W_EVT_TIMER = 0x80000000;

  static constexpr auto W_EFI_BOOT_SERVICES_SIGNATURE = 0x56524553544f4f42;
  static constexpr auto W_EFI_RUNTIME_SERVICES_SIGNATURE = 0x56524553544e5552;

  struct InputKey {
    u16 m_scan_code;
    u16 m_unicode_char;
  };

  struct SimpleTextInputProtocol;
  using InputReadKey = Status (*)(SimpleTextInputProtocol *, InputKey *);
  using InputReset = Status (*)(SimpleTextInputProtocol *, bool);

  struct SimpleTextInputProtocol {
    InputReset m_reset;
    InputReadKey m_read_key_stroke;
    Event m_wait_for_key;
  };

  using PhysicalAddress = u64;
  using VirtualAddress = u64;
  using Tpl = usize;

  struct SimpleTextOutputProtocol;
  using TextReset = Status (*)(SimpleTextOutputProtocol *, bool);
  using TextString = Status (*)(SimpleTextOutputProtocol *, const u16 *);

  struct Guid {
    u32 m_data1;
    u16 m_data2;
    u16 m_data3;
    u8 m_data4[sizeof(u64)];
  };

#define W_EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID                                     \
  {                                                                                \
    0x387477c2, 0x69c7, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
  }

#define W_EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID                                        \
  {                                                                                \
    0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } \
  }

#define W_EFI_LOADED_IMAGE_PROTOCOL_GUID                                           \
  {                                                                                \
    0x5B1B31A1, 0x9562, 0x11d2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } \
  }

/// FIXME: COULD BE BUG in 0x39 swap with -> 0x8e
#define W_EFI_FILE_INFO_GUID                                                       \
  {                                                                                \
    0x09576e92, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
  }

#define W_EFI_DEVICE_PATH_PROTOCOL_GUID                                            \
  {                                                                                \
    0x09576e91, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
  }

#define W_EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID                                      \
  {                                                                                 \
    0x0964e5b22, 0x6459, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
  }

#define W_EFI_ACPI_TABLE_GUID                                                      \
  {                                                                                \
    0x8868e871, 0xe4f1, 0x11d3, { 0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81 } \
  }

  using TextTestString = Status (*)(SimpleTextOutputProtocol *, u16 *);
  using TextQueryMode = Status (*)(SimpleTextOutputProtocol *, usize, usize *, usize *);
  using TextSetMode = Status (*)(SimpleTextOutputProtocol *, usize);
  using TextSetAttribute = Status (*)(SimpleTextOutputProtocol *, usize);
  using TextClearScreen = Status (*)(SimpleTextOutputProtocol *);
  using TextSetCursorPosition = Status (*)(SimpleTextOutputProtocol *, usize, usize);
  using TextEnableCursor = Status (*)(SimpleTextOutputProtocol *, bool);
  using RaiseTpl = Tpl (*)(Tpl);
  using RestoreTpl = void (*)(Tpl);

  using AllocateType = enum { AllocateAnyPages, AllocateMaxAddress, AllocateAddress, MaxAllocateType };

  using MemoryType = enum {
    ReservedMemoryType,
    LoaderCode,
    LoaderData,
    BootServicesCode,
    BootServicesData,
    RuntimeServicesCode,
    RuntimeServicesData,
    ConventionalMemory,
    UnusableMemory,
    ACPIReclaimMemory,
    ACPIMemoryNVS,
    MemoryMappedIO,
    MemoryMappedIOPortSpace,
    PalCode,
    PersistentMemory,
    UnacceptedMemoryType,
    MaxMemoryType
  };

  using AllocatePages = Status (*)(AllocateType, MemoryType, usize, PhysicalAddress *);

  using SimpleTextOutputMode = struct {
    i32 m_max_mode;
    i32 m_mode;
    i32 m_attribute;
    i32 m_cursor_column;
    i32 m_cursor_row;
    bool m_cursor_visible;
  };

  using GraphicsPixelFormat = enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
  };

  using PixelBitmask = struct {
    u32 m_red_mask;
    u32 m_green_mask;
    u32 m_blue_mask;
    u32 m_reserved_mask;
  };

  using GraphicsOutputModeInformation = struct {
    u32 m_version;
    u32 m_horizontal_resolution;
    u32 m_vertical_resolution;
    GraphicsPixelFormat m_pixel_format;
    PixelBitmask m_pixel_information;
    u32 m_pixels_per_scan_line;
  };

  using GraphicsOutputBltPixel = struct {
    u8 m_blue;
    u8 m_green;
    u8 m_red;
    u8 m_reserved;
  };

  using Time = struct {
    u16 m_year;   // 1900 – 9999
    u8 m_month;   // 1 – 12
    u8 m_day;     // 1 – 31
    u8 m_hour;    // 0 – 23
    u8 m_minute;  // 0 – 59
    u8 m_second;  // 0 – 59
    u8 m_pad1;
    u32 m_nanosecond;  // 0 – 999,999,999
    i16 m_time_zone;   // -1440 to 1440 or 2047
    u8 m_daylight;
    u8 m_pad2;
  };

  using GraphicsOutputBltOperation = enum {
    BltVideoFill,
    BltVideoToBltBuffer,
    BltBufferToVideo,
    BltVideoToVideo,
    GraphicsOutputBltOperationMax
  };

  struct GraphicsOutputProtocol;

  using GraphicsOutputProtocolQueryMode = Status (*)(GraphicsOutputProtocol *, u32, usize *,
                                                     GraphicsOutputModeInformation **);

  using GraphicsOutputProtocolSetMode = Status (*)(GraphicsOutputProtocol *, u32);

  using GraphicsOutputProtocolBlt = Status (*)(GraphicsOutputProtocol *, GraphicsOutputBltPixel *,
                                               GraphicsOutputBltOperation, usize, usize, usize, usize, usize, usize,
                                               usize);

  using GraphicsOutputProtocolMode = struct {
    u32 m_max_mode;
    u32 m_mode;
    GraphicsOutputModeInformation *m_info;
    usize m_size_of_info;
    PhysicalAddress m_frame_buffer_base;
    usize m_frame_buffer_size;
  };

  struct GraphicsOutputProtocol {
    GraphicsOutputProtocolQueryMode m_query_mode;
    GraphicsOutputProtocolSetMode m_set_mode;
    GraphicsOutputProtocolBlt m_blt;
    GraphicsOutputProtocolMode *m_mode;
  };

  struct TableHeader {
    u64 m_signature;
    u32 m_revision;
    u32 m_header_size;
    u32 m_cr_c32;
    u32 m_reserved;
  };

  using FreePages = Status (*)(PhysicalAddress, usize);

  using MemoryDescriptor = struct {
    u32 m_type;
    PhysicalAddress m_physical_start;
    VirtualAddress m_virtual_start;
    u64 m_number_of_pages;
    u64 m_attribute;
  };

  using GetMemoryMap = Status (*)(usize *, MemoryDescriptor *, usize *, usize *, u32 *);

  using AllocatePool = Status (*)(MemoryType, usize, void **);

  using FreePool = Status (*)(void *);

  using EventNotify = void (*)(Event, void *);

  using CreateEvent = Status (*)(u32, Tpl, EventNotify, void *, Event *);

  using TimerDelay = enum { TimerCancel, TimerPeriodic, TimerRelative };

  using SetTimer = Status (*)(Event, TimerDelay, u64);

  using WaitForEvent = Status (*)(usize, Event *, usize *);

  using SignalEvent = Status (*)(Event);

  using CloseEvent = Status (*)(Event);

  using CheckEvent = Status (*)(Event);

  using InterfaceType = enum { EFI_NATIVE_INTERFACE };

  using InstallProtocolInterface = Status (*)(Handle *, Guid *, InterfaceType, void *);

  using ReinstallProtocolInterface = Status (*)(Handle, Guid *, void *, void *);

  using UninstallProtocolInterface = Status (*)(Handle, Guid *, void *);

  using HandleProtocol = Status (*)(Handle, Guid *, void **);

  using RegisterProtocolNotify = Status (*)(Guid *, Event, void **);

  using LocateSearchType = enum { AllHandles, ByRegisterNotify, ByProtocol };

  using LocateHandle = Status (*)(LocateSearchType, Guid *, void *, usize *, Handle *);

  struct DevicePathProtocol {
    u8 m_type;
    u8 m_sub_type;
    u8 m_length[2];
  };

  using LocateDevicePath = Status (*)(Guid *, DevicePathProtocol **, Handle *);

  using InstallConfigurationTable = Status (*)(Guid *, void *);

  using ImageLoad = Status (*)(bool, Handle, DevicePathProtocol *, void *, usize, Handle *);

  using ImageStart = Status (*)(Handle, usize *, u16 **);

  using Exit = Status (*)(Handle, Status, usize, u16 *);

  using ImageUnload = Status (*)(Handle);

  using ExitBootServices = Status (*)(Handle, usize);

  using GetNextMonotonicCount = Status (*)(u64 *);

  using Stall = Status (*)(usize);

  using SetWatchdogTimer = Status (*)(usize, u64, usize, u16 *);

  using ConnectController = Status (*)(Handle, Handle *, DevicePathProtocol *, bool);

  using DisconnectController = Status (*)(Handle, Handle, Handle);

  using OpenProtocol = Status (*)(Handle, Guid *, void **, Handle, Handle, u32);

  using CloseProtocol = Status (*)(Handle, Guid *, Handle, Handle);

  using OpenProtocolInformationEntry = struct {
    Handle m_agent_handle;
    Handle m_controller_handle;
    u32 m_attributes;
    u32 m_open_count;
  };

  using OpenProtocolInformation = Status (*)(Handle, Guid *, OpenProtocolInformationEntry **, usize *);

  using ProtocolsPerHandle = Status (*)(Handle, Guid ***, usize *);

  using LocateHandleBuffer = Status (*)(LocateSearchType, Guid *, void *, usize *, Handle **);

  using LocateProtocol = Status (*)(Guid *, void *, void **);

  using InstallMultipleProtocolInterfaces = Status (*)(Handle *, ...);

  using UninstallMultipleProtocolInterfaces = Status (*)(Handle, ...);

  using CalculateCrC32 = Status (*)(void *, usize, u32 *);

  using CopyMem = void (*)(void *, void *, usize);

  using SetMem = void (*)(void *, usize, u8);

  using CreateEventEx = Status (*)(u32, Tpl, EventNotify, const void *, const Guid *, Event *);

  struct FileProtocol;

  using FileOpen = Status (*)(FileProtocol *, FileProtocol **, u16 *, u64, u64);

  using FileClose = Status (*)(FileProtocol *);

  using FileDelete = Status (*)(FileProtocol *);

  using FileRead = Status (*)(FileProtocol *, usize *, void *);

  using FileWrite = Status (*)(FileProtocol *, usize *, void *);

  using FileGetPosition = Status (*)(FileProtocol *, u64 *);

  using FileSetPosition = Status (*)(FileProtocol *, u64);

  using FileGetInfo = Status (*)(FileProtocol *, Guid *, usize *, void *);

  using FileSetInfo = Status (*)(FileProtocol *, Guid *, usize, void *);

  using FileFlush = Status (*)(FileProtocol *);

  using FileIoToken = struct {
    Event m_event;
    Status m_status;
    usize m_buffer_size;
    void *m_buffer;
  };

  using FileOpenEx = Status (*)(FileProtocol *, FileProtocol **, u16 *, u64, u64, FileIoToken *);

  using FileReadEx = Status (*)(FileProtocol *, FileIoToken *);

  using FileWriteEx = Status (*)(FileProtocol *, FileIoToken *);

  using FileFlushEx = Status (*)(FileProtocol *, FileIoToken *);

  struct SystemTable;

  using LoadedImageProtocol = struct {
    u32 m_revision;
    Handle m_parent_handle;
    SystemTable *m_system_table;
    // Source location of the image
    Handle m_device_handle;
    DevicePathProtocol *m_file_path;
    void *m_reserved;
    // Image’s load options
    u32 m_load_options_size;
    void *m_load_options;
    // Location where image was loaded
    void *m_image_base;
    u64 m_image_size;
    MemoryType m_image_code_type;
    MemoryType m_image_data_type;
    ImageUnload m_unload;
  };

  using FileInfo = struct {
    u64 m_size;
    u64 m_file_size;
    u64 m_physical_size;
    Time m_create_time;
    Time m_last_access_time;
    Time m_modification_time;
    u64 m_attribute;
    u16 *m_file_name;
  };

  struct FileProtocol {
    u64 m_revision;
    FileOpen m_open;
    FileClose m_close;
    FileDelete m_delete;
    FileRead m_read;
    FileWrite m_write;
    FileGetPosition m_get_position;
    FileSetPosition m_set_position;
    FileGetInfo m_get_info;
    FileSetInfo m_set_info;
    FileFlush m_flush;
    FileOpenEx m_open_ex;
    FileReadEx m_read_ex;
    FileWriteEx m_write_ex;
    FileFlushEx m_flush_ex;
  };

  struct SimpleFileSystemProtocol;

  using SimpleFileSystemProtocolOpenVolume = Status (*)(SimpleFileSystemProtocol *, FileProtocol **);

  struct SimpleFileSystemProtocol {
    u64 m_revision;
    SimpleFileSystemProtocolOpenVolume m_open_volume;
  };

  using BootServices = struct {
    TableHeader m_hdr;
    RaiseTpl m_raise_tpl;
    RestoreTpl m_restore_tpl;
    AllocatePages m_allocate_pages;
    FreePages m_free_pages;
    GetMemoryMap m_get_memory_map;
    AllocatePool m_allocate_pool;
    FreePool m_free_pool;

    /** Event & Timer Services */
    CreateEvent m_create_event;
    SetTimer m_set_timer;
    WaitForEvent m_wait_for_event;
    SignalEvent m_signal_event;
    CloseEvent m_close_event;
    CheckEvent m_check_event;

    /** Protocol Handler Services */
    InstallProtocolInterface m_install_protocol_interface;
    ReinstallProtocolInterface m_reinstall_protocol_interface;
    UninstallProtocolInterface m_uninstall_protocol_interface;
    HandleProtocol m_handle_protocol;
    void *m_reserved;
    RegisterProtocolNotify m_register_protocol_notify;
    LocateHandle m_locate_handle;
    LocateDevicePath m_locate_device_path;
    InstallConfigurationTable m_install_configuration_table;

    /** Image Services */
    ImageLoad m_load_image;
    ImageStart m_start_image;
    Exit m_exit;
    ImageUnload m_unload_image;
    ExitBootServices m_exit_boot_services;

    /** Miscellaneous Services */
    GetNextMonotonicCount m_get_next_monotonic_count;
    Stall m_stall;
    SetWatchdogTimer m_set_watchdog_timer;

    /** DriverSupport Services */
    ConnectController m_connect_controller;
    DisconnectController m_disconnect_controller;

    /** Open and Close Protocol Services */
    OpenProtocol m_open_protocol;
    CloseProtocol m_close_protocol;
    OpenProtocolInformation m_open_protocol_information;

    /** Library Services  */
    ProtocolsPerHandle m_protocols_per_handle;
    LocateHandleBuffer m_locate_handle_buffer;
    LocateProtocol m_locate_protocol;
    InstallMultipleProtocolInterfaces m_install_multiple_protocol_interfaces;
    UninstallMultipleProtocolInterfaces m_uninstall_multiple_protocol_interfaces;

    /** 32-bit CRC Services  */
    CalculateCrC32 m_calculate_crc32;

    /** Miscellaneous Services  */
    CopyMem m_copy_mem;
    SetMem m_set_mem;
    CreateEventEx m_create_event_ex;
  };

  using TimeCapabilities = struct {
    u32 m_resolution;
    u32 m_accuracy;
    bool m_sets_to_zero;
  };

  using GetTime = Status (*)(Time *, TimeCapabilities *);

  using RuntimeServices = struct {
    TableHeader m_hdr;
    // //
    // // Time Services
    // //
    GetTime m_get_time;
    // EFI_GET_WAKEUP_TIME GetWakeupTime;
    // EFI_SET_WAKEUP_TIME SetWakeupTime;
    // //
    // // Virtual Memory Services
    // //
    // EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
    // EFI_CONVERT_POINTER ConvertPointer;
    // //
    // // Variable Services
    // //
    // EFI_GET_VARIABLE GetVariable;
    // EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
    // EFI_SET_VARIABLE SetVariable;
    // //
    // // Miscellaneous Services
    // //
    // EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
    // EFI_RESET_SYSTEM ResetSystem;
    // //
    // // UEFI 2.0 Capsule Services
    // //
    // EFI_UPDATE_CAPSULE UpdateCapsule;
    // EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
    // //
    // // Miscellaneous UEFI 2.0 Service
    // //
    // EFI_QUERY_VARIABLE_INFO QueryVariableInfo;
  };

  using ConfigurationTable = struct {
    Guid m_vendor_guid;
    void *m_vendor_table;
  };

  struct SystemTable {
    TableHeader m_hdr;
    u16 *m_firmware_vendor;
    u32 m_firmware_revision;
    Handle m_console_in_handle;
    SimpleTextInputProtocol *m_con_in;
    Handle m_console_out_handle;
    SimpleTextOutputProtocol *m_con_out;
    Handle m_standard_error_handle;
    SimpleTextOutputProtocol *m_std_err;
    RuntimeServices *m_runtime_services;
    BootServices *m_boot_services;
    usize m_number_of_table_entries;
    ConfigurationTable *m_configuration_table;
  };

  struct SimpleTextOutputProtocol {
    TextReset m_reset;
    TextString m_output_string;
    TextTestString m_test_string;
    TextQueryMode m_query_mode;
    TextSetMode m_set_mode;
    TextSetAttribute m_set_attribute;
    TextClearScreen m_clear_screen;
    TextSetCursorPosition m_set_cursor_position;
    TextEnableCursor m_enable_cursor;
    SimpleTextOutputMode *m_mode;
  };
}  // namespace wesos::boot::efi
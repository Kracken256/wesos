/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-private-field"

  using Handle = void *;
  using Event = void *;

  using Status = usize;

  static constexpr Status EFI_SUCCESS = 0x0000000000000000;
  static constexpr Status EFI_FAILURE = 0x8000000000000000;
  static constexpr Status EFI_BUFFER_TOO_SMALL = 0x8000000000000005;

  class OpStatus {
    usize m_status;

  public:
    constexpr OpStatus() : m_status(0) {}
    constexpr OpStatus(usize status) : m_status(status) {}

    [[nodiscard]] constexpr auto is_success() const -> bool { return m_status == EFI_SUCCESS; }
    [[nodiscard]] constexpr auto is_failure() const -> bool { return !is_success(); }
    [[nodiscard]] constexpr operator bool() const { return is_success(); }
    [[nodiscard]] constexpr auto get_status() const -> usize { return m_status; }
  } __attribute__((packed));

  static constexpr auto EFI_FILE_MODE_READ = 0x0000000000000001;
  static constexpr auto EFI_FILE_MODE_WRITE = 0x0000000000000002;
  static constexpr auto EFI_FILE_MODE_CREATE = 0x8000000000000000;
  static constexpr auto EFI_FILE_READ_ONLY = 0x0000000000000001;
  static constexpr auto EFI_FILE_HIDDEN = 0x0000000000000002;
  static constexpr auto EFI_FILE_SYSTEM = 0x0000000000000004;
  static constexpr auto EFI_FILE_RESERVED = 0x0000000000000008;
  static constexpr auto EFI_FILE_DIRECTORY = 0x0000000000000010;
  static constexpr auto EFI_FILE_ARCHIVE = 0x0000000000000020;
  static constexpr auto EFI_FILE_VALID_ATTR = 0x0000000000000037;

  static constexpr auto EVT_NOTIFY_SIGNAL = 0x00000200;
  static constexpr auto TPL_CALLBACK = 8;
  static constexpr auto EVT_TIMER = 0x80000000;

  static constexpr auto EFI_BOOT_SERVICES_SIGNATURE = 0x56524553544f4f42;
  static constexpr auto EFI_RUNTIME_SERVICES_SIGNATURE = 0x56524553544e5552;
  static constexpr auto EFI_SYSTEM_TABLE_SIGNATURE = 0x5453595320494249;

  //*******************************************************
  // Attributes
  //*******************************************************
  constexpr auto BLACK = 0x00;
  constexpr auto BLUE = 0x01;
  constexpr auto GREEN = 0x02;
  constexpr auto CYAN = 0x03;
  constexpr auto RED = 0x04;
  constexpr auto MAGENTA = 0x05;
  constexpr auto BROWN = 0x06;
  constexpr auto LIGHTGRAY = 0x07;
  constexpr auto BRIGHT = 0x08;
  constexpr auto DARKGRAY = 0x08;
  constexpr auto LIGHTBLUE = 0x09;
  constexpr auto LIGHTGREEN = 0x0A;
  constexpr auto LIGHTCYAN = 0x0B;
  constexpr auto LIGHTRED = 0x0C;
  constexpr auto LIGHTMAGENTA = 0x0D;
  constexpr auto YELLOW = 0x0E;
  constexpr auto WHITE = 0x0F;

  constexpr auto BACKGROUND_BLACK = 0x00;
  constexpr auto BACKGROUND_BLUE = 0x10;
  constexpr auto BACKGROUND_GREEN = 0x20;
  constexpr auto BACKGROUND_CYAN = 0x30;
  constexpr auto BACKGROUND_RED = 0x40;
  constexpr auto BACKGROUND_MAGENTA = 0x50;
  constexpr auto BACKGROUND_BROWN = 0x60;
  constexpr auto BACKGROUND_LIGHTGRAY = 0x70;

  class InputKey {
    u16 m_scan_code;
    u16 m_unicode_char;

  public:
    [[nodiscard]] constexpr auto scan_code() const -> u16 { return m_scan_code; }
    [[nodiscard]] constexpr auto unicode_char() const -> u16 { return m_unicode_char; }
  };

  class SimpleTextInputProtocol;
  using InputReadKey = Status (*)(SimpleTextInputProtocol *, InputKey *);
  using InputReset = Status (*)(SimpleTextInputProtocol *, bool);

  class SimpleTextInputProtocol {
    InputReset m_reset;
    InputReadKey m_read_key_stroke;
    Event m_wait_for_key;

  public:
    [[nodiscard]] constexpr auto reset() -> Status { return m_reset(this, false); }
    [[nodiscard]] constexpr auto read_key_stroke(InputKey *key) -> Status { return m_read_key_stroke(this, key); }
    [[nodiscard]] constexpr auto wait_for_key() -> Event { return m_wait_for_key; }
  };

  using PhysicalAddress = u64;
  using VirtualAddress = u64;
  using Tpl = usize;

  class SimpleTextOutputProtocol;
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

// FIXME: COULD BE BUG in 0x39 swap with -> 0x8e
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

  class SimpleTextOutputMode {
    i32 m_max_mode;
    i32 m_mode;
    i32 m_attribute;
    i32 m_cursor_column;
    i32 m_cursor_row;
    bool m_cursor_visible;

  public:
    [[nodiscard]] constexpr auto max_mode() const -> i32 { return m_max_mode; }
    [[nodiscard]] constexpr auto mode() const -> i32 { return m_mode; }
    [[nodiscard]] constexpr auto attribute() const -> i32 { return m_attribute; }
    [[nodiscard]] constexpr auto cursor_column() const -> i32 { return m_cursor_column; }
    [[nodiscard]] constexpr auto cursor_row() const -> i32 { return m_cursor_row; }
    [[nodiscard]] constexpr auto is_cursor_visible() const -> bool { return m_cursor_visible; }
  };

  using GraphicsPixelFormat = enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
  };

  class PixelBitmask {
    u32 m_red_mask;
    u32 m_green_mask;
    u32 m_blue_mask;
    u32 m_reserved_mask;

  public:
    [[nodiscard]] constexpr auto red_mask() const -> u32 { return m_red_mask; }
    [[nodiscard]] constexpr auto green_mask() const -> u32 { return m_green_mask; }
    [[nodiscard]] constexpr auto blue_mask() const -> u32 { return m_blue_mask; }
  };

  class GraphicsOutputModeInformation {
    u32 m_version;
    u32 m_horizontal_resolution;
    u32 m_vertical_resolution;
    GraphicsPixelFormat m_pixel_format;
    PixelBitmask m_pixel_information;
    u32 m_pixels_per_scan_line;

  public:
    [[nodiscard]] constexpr auto version() const -> u32 { return m_version; }
    [[nodiscard]] constexpr auto horizontal_resolution() const -> u32 { return m_horizontal_resolution; }
    [[nodiscard]] constexpr auto vertical_resolution() const -> u32 { return m_vertical_resolution; }
    [[nodiscard]] constexpr auto pixel_format() const -> GraphicsPixelFormat { return m_pixel_format; }
    [[nodiscard]] constexpr auto pixel_information() const -> PixelBitmask { return m_pixel_information; }
    [[nodiscard]] constexpr auto pixels_per_scan_line() const -> u32 { return m_pixels_per_scan_line; }
  };

  class GraphicsOutputBltPixel {
    u8 m_blue;
    u8 m_green;
    u8 m_red;
    u8 m_reserved;

  public:
    [[nodiscard]] constexpr auto blue() const -> u8 { return m_blue; }
    [[nodiscard]] constexpr auto green() const -> u8 { return m_green; }
    [[nodiscard]] constexpr auto red() const -> u8 { return m_red; }
  };

  class Time {
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

  public:
    [[nodiscard]] constexpr auto year() const -> u16 { return m_year; }
    [[nodiscard]] constexpr auto month() const -> u8 { return m_month; }
    [[nodiscard]] constexpr auto day() const -> u8 { return m_day; }
    [[nodiscard]] constexpr auto hour() const -> u8 { return m_hour; }
    [[nodiscard]] constexpr auto minute() const -> u8 { return m_minute; }
    [[nodiscard]] constexpr auto second() const -> u8 { return m_second; }
    [[nodiscard]] constexpr auto nanosecond() const -> u32 { return m_nanosecond; }
    [[nodiscard]] constexpr auto time_zone() const -> i16 { return m_time_zone; }
    [[nodiscard]] constexpr auto daylight() const -> u8 { return m_daylight; }
  };

  using GraphicsOutputBltOperation = enum {
    BltVideoFill,
    BltVideoToBltBuffer,
    BltBufferToVideo,
    BltVideoToVideo,
    GraphicsOutputBltOperationMax
  };

  class GraphicsOutputProtocol;

  using GraphicsOutputProtocolQueryMode = Status (*)(GraphicsOutputProtocol *, u32, usize *,
                                                     GraphicsOutputModeInformation **);

  using GraphicsOutputProtocolSetMode = Status (*)(GraphicsOutputProtocol *, u32);

  using GraphicsOutputProtocolBlt = Status (*)(GraphicsOutputProtocol *, GraphicsOutputBltPixel *,
                                               GraphicsOutputBltOperation, usize, usize, usize, usize, usize, usize,
                                               usize);

  class GraphicsOutputProtocolMode {
    u32 m_max_mode;
    u32 m_mode;
    GraphicsOutputModeInformation *m_info;
    usize m_size_of_info;
    PhysicalAddress m_frame_buffer_base;
    usize m_frame_buffer_size;

  public:
    [[nodiscard]] constexpr auto max_mode() const -> u32 { return m_max_mode; }
    [[nodiscard]] constexpr auto mode() const -> u32 { return m_mode; }
    [[nodiscard]] constexpr auto info() const -> GraphicsOutputModeInformation & { return *m_info; }
    [[nodiscard]] constexpr auto size_of_info() const -> usize { return m_size_of_info; }
    [[nodiscard]] constexpr auto frame_buffer_base() const -> PhysicalAddress { return m_frame_buffer_base; }
    [[nodiscard]] constexpr auto frame_buffer_size() const -> usize { return m_frame_buffer_size; }
  };

  class GraphicsOutputProtocol {
    GraphicsOutputProtocolQueryMode m_query_mode;
    GraphicsOutputProtocolSetMode m_set_mode;
    GraphicsOutputProtocolBlt m_blt;
    GraphicsOutputProtocolMode *m_mode;

  public:
    [[nodiscard]] constexpr auto query_mode(u32 mode_number, usize *size_of_info,
                                            GraphicsOutputModeInformation **info) -> Status {
      return m_query_mode(this, mode_number, size_of_info, info);
    }

    [[nodiscard]] constexpr auto set_mode(u32 mode_number) -> Status { return m_set_mode(this, mode_number); }

    [[nodiscard]] constexpr auto blt(GraphicsOutputBltPixel *blt_buffer, GraphicsOutputBltOperation operation,
                                     usize source_x, usize source_y, usize destination_x, usize destination_y,
                                     usize width, usize height, usize delta) -> Status {
      return m_blt(this, blt_buffer, operation, source_x, source_y, destination_x, destination_y, width, height, delta);
    }

    [[nodiscard]] constexpr auto mode() const -> GraphicsOutputProtocolMode * { return m_mode; }
  };

  class TableHeader {
    u64 m_signature;
    u32 m_revision;
    u32 m_header_size;
    u32 m_cr_c32;
    u32 m_reserved;

  public:
    [[nodiscard]] constexpr auto signature() const -> u64 { return m_signature; }
    [[nodiscard]] constexpr auto revision() const -> u32 { return m_revision; }
    [[nodiscard]] constexpr auto header_size() const -> u32 { return m_header_size; }
    [[nodiscard]] constexpr auto cr_c32() const -> u32 { return m_cr_c32; }
  };

  using FreePages = Status (*)(PhysicalAddress, usize);

  class MemoryDescriptor {
    u32 m_type;
    PhysicalAddress m_physical_start;
    VirtualAddress m_virtual_start;
    u64 m_number_of_pages;
    u64 m_attribute;

  public:
    [[nodiscard]] constexpr auto type() const -> u32 { return m_type; }
    [[nodiscard]] constexpr auto physical_start() const -> PhysicalAddress { return m_physical_start; }
    [[nodiscard]] constexpr auto virtual_start() const -> VirtualAddress { return m_virtual_start; }
    [[nodiscard]] constexpr auto number_of_pages() const -> u64 { return m_number_of_pages; }
    [[nodiscard]] constexpr auto attribute() const -> u64 { return m_attribute; }
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

  class DevicePathProtocol {
    u8 m_type;
    u8 m_sub_type;
    u8 m_length[2];

  public:
    [[nodiscard]] constexpr auto type() const -> u8 { return m_type; }
    [[nodiscard]] constexpr auto sub_type() const -> u8 { return m_sub_type; }
    [[nodiscard]] constexpr auto length() const -> u16 {
      const auto bits_per_byte = 8;
      return static_cast<u16>(m_length[0] | (m_length[1] << bits_per_byte));
    }
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

  class OpenProtocolInformationEntry {
    Handle m_agent_handle;
    Handle m_controller_handle;
    u32 m_attributes;
    u32 m_open_count;

  public:
    [[nodiscard]] constexpr auto agent_handle() const -> Handle { return m_agent_handle; }
    [[nodiscard]] constexpr auto controller_handle() const -> Handle { return m_controller_handle; }
    [[nodiscard]] constexpr auto attributes() const -> u32 { return m_attributes; }
    [[nodiscard]] constexpr auto open_count() const -> u32 { return m_open_count; }
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

  class FileProtocol;
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

  class FileIoToken {
    Event m_event;
    Status m_status;
    usize m_buffer_size;
    void *m_buffer;

  public:
    [[nodiscard]] constexpr auto event() const -> Event { return m_event; }
    [[nodiscard]] constexpr auto status() const -> Status { return m_status; }
    [[nodiscard]] constexpr auto buffer_size() const -> usize { return m_buffer_size; }
    [[nodiscard]] constexpr auto buffer() const -> void * { return m_buffer; }
  };

  using FileOpenEx = Status (*)(FileProtocol *, FileProtocol **, u16 *, u64, u64, FileIoToken *);
  using FileReadEx = Status (*)(FileProtocol *, FileIoToken *);
  using FileWriteEx = Status (*)(FileProtocol *, FileIoToken *);
  using FileFlushEx = Status (*)(FileProtocol *, FileIoToken *);
  class SystemTable;

  class LoadedImageProtocol {
    u32 m_revision;
    Handle m_parent_handle;
    SystemTable *m_system_table;
    Handle m_device_handle;
    DevicePathProtocol *m_file_path;
    void *m_reserved;
    u32 m_load_options_size;
    void *m_load_options;
    void *m_image_base;
    u64 m_image_size;
    MemoryType m_image_code_type;
    MemoryType m_image_data_type;
    ImageUnload m_unload;

  public:
    [[nodiscard]] constexpr auto revision() const -> u32 { return m_revision; }
    [[nodiscard]] constexpr auto parent_handle() const -> Handle { return m_parent_handle; }
    [[nodiscard]] constexpr auto system_table() const -> SystemTable * { return m_system_table; }
    [[nodiscard]] constexpr auto device_handle() const -> Handle { return m_device_handle; }
    [[nodiscard]] constexpr auto file_path() const -> DevicePathProtocol * { return m_file_path; }
    [[nodiscard]] constexpr auto load_options_size() const -> u32 { return m_load_options_size; }
    [[nodiscard]] constexpr auto load_options() const -> void * { return m_load_options; }
    [[nodiscard]] constexpr auto image_base() const -> void * { return m_image_base; }
    [[nodiscard]] constexpr auto image_size() const -> u64 { return m_image_size; }
    [[nodiscard]] constexpr auto image_code_type() const -> MemoryType { return m_image_code_type; }
    [[nodiscard]] constexpr auto image_data_type() const -> MemoryType { return m_image_data_type; }
    [[nodiscard]] constexpr auto unload() const -> ImageUnload { return m_unload; }
  };

  class FileInfo {
    u64 m_size;
    u64 m_file_size;
    u64 m_physical_size;
    Time m_create_time;
    Time m_last_access_time;
    Time m_modification_time;
    u64 m_attribute;
    u16 *m_file_name;

  public:
    [[nodiscard]] constexpr auto size() const -> u64 { return m_size; }
    [[nodiscard]] constexpr auto file_size() const -> u64 { return m_file_size; }
    [[nodiscard]] constexpr auto physical_size() const -> u64 { return m_physical_size; }
    [[nodiscard]] constexpr auto create_time() const -> Time { return m_create_time; }
    [[nodiscard]] constexpr auto last_access_time() const -> Time { return m_last_access_time; }
    [[nodiscard]] constexpr auto modification_time() const -> Time { return m_modification_time; }
    [[nodiscard]] constexpr auto attribute() const -> u64 { return m_attribute; }
    [[nodiscard]] constexpr auto file_name() const -> u16 * { return m_file_name; }
  };

  class FileProtocol {
  public:
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

  class SimpleFileSystemProtocol;

  using SimpleFileSystemProtocolOpenVolume = Status (*)(SimpleFileSystemProtocol *, FileProtocol **);

  class SimpleFileSystemProtocol {
    u64 m_revision;
    SimpleFileSystemProtocolOpenVolume m_open_volume;

  public:
    [[nodiscard]] constexpr auto open_volume(FileProtocol **root) -> Status { return m_open_volume(this, root); }
    [[nodiscard]] constexpr auto revision() const -> u64 { return m_revision; }
  };

  class BootServices {
  public:
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

  class TimeCapabilities {
    u32 m_resolution;
    u32 m_accuracy;
    bool m_sets_to_zero;

  public:
    [[nodiscard]] constexpr auto resolution() const -> u32 { return m_resolution; }
    [[nodiscard]] constexpr auto accuracy() const -> u32 { return m_accuracy; }
    [[nodiscard]] constexpr auto sets_to_zero() const -> bool { return m_sets_to_zero; }
  };

  using GetTime = Status (*)(Time *, TimeCapabilities *);

  class RuntimeServices {
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

  class ConfigurationTable {
    Guid m_vendor_guid;
    void *m_vendor_table;

  public:
    [[nodiscard]] constexpr auto vendor_guid() const -> Guid { return m_vendor_guid; }
    [[nodiscard]] constexpr auto vendor_table() const -> void * { return m_vendor_table; }
  };

  class SimpleTextOutputProtocol {
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

  public:
    [[nodiscard]] constexpr auto reset() -> Status { return m_reset(this, false); }
    [[nodiscard]] constexpr auto output_string(const u16 *string) -> Status { return m_output_string(this, string); }
    [[nodiscard]] constexpr auto test_string(u16 *string) -> Status { return m_test_string(this, string); }
    [[nodiscard]] constexpr auto query_mode(usize mode_number, usize *columns, usize *rows) -> Status {
      return m_query_mode(this, mode_number, columns, rows);
    }
    [[nodiscard]] constexpr auto set_mode(usize mode_number) -> Status { return m_set_mode(this, mode_number); }
    [[nodiscard]] constexpr auto set_attribute(usize attribute) -> Status { return m_set_attribute(this, attribute); }
    [[nodiscard]] constexpr auto clear_screen() -> Status { return m_clear_screen(this); }
    [[nodiscard]] constexpr auto set_cursor_position(usize column, usize row) -> Status {
      return m_set_cursor_position(this, column, row);
    }
    [[nodiscard]] constexpr auto enable_cursor(bool visible) -> Status { return m_enable_cursor(this, visible); }
  };

  class SystemTable {
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

  public:
    [[nodiscard]] constexpr auto validate() const -> bool {
      return m_hdr.signature() == EFI_SYSTEM_TABLE_SIGNATURE && m_hdr.header_size() >= sizeof(SystemTable);
    }

    [[nodiscard]] constexpr auto firmware_vendor() -> u16 * { return m_firmware_vendor; }
    [[nodiscard]] constexpr auto firmware_revision() -> u32 { return m_firmware_revision; }
    [[nodiscard]] constexpr auto console_in_handle() -> Handle { return m_console_in_handle; }
    [[nodiscard]] constexpr auto con_in() -> SimpleTextInputProtocol & { return *m_con_in; }
    [[nodiscard]] constexpr auto console_out_handle() -> Handle { return m_console_out_handle; }
    [[nodiscard]] constexpr auto con_out() -> SimpleTextOutputProtocol & { return *m_con_out; }
    [[nodiscard]] constexpr auto standard_error_handle() -> Handle { return m_standard_error_handle; }
    [[nodiscard]] constexpr auto std_err() -> SimpleTextOutputProtocol & { return *m_std_err; }
    [[nodiscard]] constexpr auto runtime_services() -> RuntimeServices & { return *m_runtime_services; }
    [[nodiscard]] constexpr auto boot_services() -> BootServices & { return *m_boot_services; }
    [[nodiscard]] constexpr auto number_of_table_entries() -> usize { return m_number_of_table_entries; }
    [[nodiscard]] constexpr auto configuration_table() -> ConfigurationTable & { return *m_configuration_table; }
  };

  static constexpr auto operator"" _u16(const char16_t *str, usize) -> const u16 * {
    return bit_cast<const u16 *>(str);
  }

  extern SystemTable *SYSTEM_TABLE_GLOBAL;

#pragma GCC diagnostic pop
}  // namespace wesos::boot::efi
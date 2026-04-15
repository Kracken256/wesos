#[repr(C, align(8))]
pub struct Multiboot2Header {
    magic: u32,
    architecture: u32,
    header_length: u32,
    checksum: u32,
    end_tag_type: u16,
    end_tag_flags: u16,
    end_tag_size: u32,
}

#[unsafe(link_section = ".multiboot2_header")]
#[unsafe(no_mangle)]
#[used]
pub static MULTIBOOT_HEADER: Multiboot2Header = Multiboot2Header {
    magic: 0xE85250D6,
    architecture: 0,
    header_length: 24,
    checksum: 0x17ADAF12,
    end_tag_type: 0,
    end_tag_flags: 0,
    end_tag_size: 8,
};

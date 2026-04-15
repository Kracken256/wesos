#![no_std]
#![no_main]

use core::alloc::{GlobalAlloc, Layout};
use core::panic::PanicInfo;

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
    architecture: 0, // i386 (protected mode)
    header_length: 24,
    checksum: 0x17ADAF12, // Exact calculation for 24-byte header
    end_tag_type: 0,
    end_tag_flags: 0,
    end_tag_size: 8,
};

// --- DUMMY ALLOCATOR ---
struct DummyAllocator;
unsafe impl GlobalAlloc for DummyAllocator {
    unsafe fn alloc(&self, _layout: Layout) -> *mut u8 {
        core::ptr::null_mut()
    }
    unsafe fn dealloc(&self, _ptr: *mut u8, _layout: Layout) {}
}
#[global_allocator]
static ALLOCATOR: DummyAllocator = DummyAllocator;

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {
    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

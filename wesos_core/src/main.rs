#![no_std]
#![no_main]

use core::panic::PanicInfo;
use multiboot2::{BootInformation, BootInformationHeader};
pub use print::_print;

mod heap;
mod multiboot;
mod print;
mod uart;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    println!("Kernel panic: {}", info);
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn _start(magic: u32, mbi_address: *const BootInformationHeader) -> ! {
    uart::init();

    if magic != 0x36d76289 {
        panic!(
            "Invalid magic number: expected 0x36d76289, got {:#x}",
            magic
        );
    }

    let boot_info =
        unsafe { BootInformation::load(mbi_address).expect("Failed to load Multiboot info") };
    println!("Multiboot information loaded successfully!");

    println!("Memory areas:");
    for area in boot_info
        .memory_map_tag()
        .expect("No memory map tag found")
        .memory_areas()
    {
        println!(
            "  Start: {:#010x}, Length: {:#010x}, Type: {:?}",
            area.start_address(),
            area.size(),
            area.typ()
        );
    }

    println!("Modules:");
    for module in boot_info.module_tags() {
        println!(
            "  Start: {:#010x}, End: {:#010x}, Cmdline: {}",
            module.start_address(),
            module.end_address(),
            module.cmdline().unwrap_or("<unknown>")
        );
    }

    println!(
        "Bootloader name: {}",
        boot_info
            .boot_loader_name_tag()
            .map(|tag| tag.name().unwrap_or("<unknown>"))
            .unwrap_or("<unknown>")
    );

    println!(
        "Command line: {}",
        boot_info
            .command_line_tag()
            .map(|tag| tag.cmdline().unwrap_or("<unknown>"))
            .unwrap_or("<unknown>")
    );

    heap::init(&boot_info);

    println!("Hello, WesOS!");

    loop {}
}

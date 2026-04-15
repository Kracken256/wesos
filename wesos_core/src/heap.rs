use buddy_system_allocator::LockedHeap;
use multiboot2::{BootInformation, MemoryAreaType};

use crate::println;

#[global_allocator]
static ALLOCATOR: LockedHeap<32> = LockedHeap::empty();

pub fn init(boot_info: &BootInformation) {
    println!("Initializing heap allocator...");

    let mmap = boot_info.memory_map_tag().unwrap();

    let mut allocator = ALLOCATOR.lock();
    for area in mmap
        .memory_areas()
        .iter()
        .filter(|a| a.typ() == MemoryAreaType::Available)
    {
        let start = area.start_address();
        let end = area.end_address();

        // Skip the first 1MB to protect VGA/BIOS/Kernel
        if start < 0x100000 {
            continue;
        }

        unsafe {
            allocator.add_to_heap(start as usize, end as usize);
            println!(
                "Added memory area to heap: {:#010x} - {:#010x} ({} bytes)",
                start,
                end,
                end - start
            );
        }
    }

    println!("Heap allocator initialized");
}

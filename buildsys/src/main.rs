use clap::{Parser, Subcommand, ValueEnum};
use std::fs;
use std::io::Write;
use std::path::PathBuf;
use std::process::Command;
use std::process::Stdio;

#[derive(Copy, Clone, PartialEq, Eq, PartialOrd, Ord, ValueEnum)]
enum BuildType {
    Dev,
    Release,
}

#[derive(Subcommand)]
enum Commands {
    /// Build the kernel and create a bootable ISO image
    Build {
        #[arg(value_enum, default_value_t = BuildType::Dev)]
        build_type: BuildType,
    },

    /// Run the ISO image in QEMU
    Run {
        /// Path to ISO image (optional, defaults to built ISO)
        #[arg(short, long)]
        iso: Option<PathBuf>,

        /// Extra QEMU flags (e.g. -nographic)
        #[arg(last = true)]
        qemu_args: Vec<String>,
    },
}

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

fn main() {
    let cli = Cli::parse();
    match cli.command {
        Commands::Build { build_type } => {
            build_and_create_iso(build_type);
        }

        Commands::Run { iso, qemu_args } => {
            run_qemu(iso, qemu_args);
        }
    }
}

fn build_and_create_iso(build_type: BuildType) {
    // 1. Build wesos_core (kernel)
    let profile = match build_type {
        BuildType::Dev => "debug",
        BuildType::Release => "release",
    };

    println!("Building wesos_core in {profile} mode...");

    let mut cmd = Command::new("cargo");

    cmd.arg("build")
        .arg("--manifest-path")
        .arg("wesos_core/Cargo.toml");

    if build_type == BuildType::Release {
        cmd.arg("--release");
    }

    let status = cmd.status().expect("Failed to run cargo build");

    if !status.success() {
        eprintln!("Kernel build failed");
        std::process::exit(1);
    }

    // 2. Create ISO image with GRUB2
    println!("Creating ISO image with GRUB2...");
    let out_dir = format!("target/{}/iso_root", profile);
    let boot_dir = format!("{}/boot/grub", out_dir);
    let kernel_src = format!("target/{}/wesos_core", profile);
    let kernel_dst = format!("{}/boot/wesos_core", out_dir);
    let grub_cfg = format!("{}/grub.cfg", boot_dir);
    let iso_path = format!("target/{}/wesos.iso", profile);

    // Clean and create directories
    let _ = fs::remove_dir_all(&out_dir);
    fs::create_dir_all(&boot_dir).expect("Failed to create GRUB directory");

    // Copy kernel
    fs::copy(&kernel_src, &kernel_dst).expect("Failed to copy kernel");

    // Write grub.cfg
    let mut cfg = fs::File::create(&grub_cfg).expect("Failed to create grub.cfg");
    writeln!(cfg, "set timeout=0").unwrap();
    writeln!(cfg, "set default=0").unwrap();
    writeln!(cfg, "menuentry 'wesos' {{").unwrap();
    writeln!(cfg, "    multiboot2 /boot/wesos_core").unwrap();
    writeln!(cfg, "    boot").unwrap();
    writeln!(cfg, "}}").unwrap();

    // Call grub-mkrescue
    let status = Command::new("grub-mkrescue")
        .arg("-o")
        .arg(&iso_path)
        .arg(&out_dir)
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status()
        .expect("Failed to run grub-mkrescue");
    if !status.success() {
        eprintln!("grub-mkrescue failed");
        std::process::exit(1);
    }
    println!("ISO image created at {}", iso_path);
}

fn run_qemu(iso: Option<PathBuf>, qemu_args: Vec<String>) {
    // 1. Determine ISO path and profile
    let iso_path = if let Some(user_iso) = iso {
        // If user provided a path, use it as-is (absolute or relative)
        if !user_iso.exists() {
            eprintln!("Provided ISO path does not exist: {}", user_iso.display());
            std::process::exit(1);
        }

        user_iso
    } else {
        // Try to find the most recent ISO in target/debug or target/release
        let debug_iso = PathBuf::from("target/debug/wesos.iso");
        let release_iso = PathBuf::from("target/release/wesos.iso");
        if debug_iso.exists() {
            debug_iso
        } else if release_iso.exists() {
            release_iso
        } else {
            eprintln!("No ISO image found in target/debug or target/release. Please build first.");
            std::process::exit(1);
        }
    };

    if !iso_path.exists() {
        eprintln!("ISO image not found: {}", iso_path.display());
        std::process::exit(1);
    }

    println!("Running QEMU with ISO: {}", iso_path.display());

    let mut cmd = Command::new("qemu-system-x86_64");
    cmd.arg("-cdrom").arg(&iso_path);

    for arg in qemu_args {
        cmd.arg(arg);
    }

    let status = cmd.status().unwrap_or_else(|e| {
        eprintln!(
            "Failed to run QEMU: {}\nIs qemu-system-x86_64 installed in your environment?",
            e
        );
        std::process::exit(1);
    });

    if !status.success() {
        eprintln!("QEMU exited with error");
        std::process::exit(1);
    }
}

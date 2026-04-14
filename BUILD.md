# Building wesos

This document describes how to build the `wesos_core` kernel and create a bootable ISO image using the `buildsys` tool.

## Building the Docker Container (wesos-dev)

Before running any build or run commands, you must build the Docker image:

```
docker build -t wesos-dev .
```

Run this command from the root of the repository (where the Dockerfile is located). This only needs to be done once, or whenever you change the Dockerfile.

## Building the Kernel and ISO

To build the kernel and create an ISO image:

```
docker run -it --rm -v $(pwd):/app wesos-dev buildsys build
```

- The ISO will be created in `target/debug/wesos.iso` (or `target/release/wesos.iso` for release builds).

## Requirements

- Docker (for containerized builds)
- The Docker image must include: build-essential, grub-pc-bin, grub-efi-amd64-bin, xorriso, mtools

## Troubleshooting

- If you see errors about missing tools, ensure your Docker image is up to date.
- For more details, see the source code in `buildsys/src/main.rs` and the Dockerfile for build environment setup.

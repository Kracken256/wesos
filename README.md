# WesOS

![](https://img.shields.io/badge/license-public%20domain-b3e32d.svg)
![](https://img.shields.io/badge/cmake_integration-true-purple.svg)

## Overview

A humble attempt to create an operating system to entertain my curiosity and 
gain experience in pragmatic systems programming. The project is in its 
earliest stages and is not yet functional. The goal is to create a simple 
operating system that can run on x86_64 architecture, with a focus on 
learning and experimentation.

## Table of Contents

- [WesOS](#wesos)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
    - [Download Operating System](#download-operating-system)
    - [Custom Build](#custom-build)
  - [Features](#features)
  - [Technology](#technology)
  - [Contributing](#contributing)
  - [License](#license)

## Installation

### Download Operating System
You can download the latest release of WesOS from the [releases page](https://github.com/Kracken256/wesos/releases).
The release is a bootable ISO image that can be used to boot the operating system in a virtual machine or on real hardware.

### Custom Build

To build the project from source, you will need to have the following dependencies installed:
- [Docker](https://www.docker.com/)

**A Linux system is required** to build the project. Either a native Linux system,
the Windows Subsystem for Linux (WSL), or a Linux virtual machine will work.

```bash
# Clone this repository
git clone https://github.com/Kracken256/wesos.git
cd wesos

# Build the Docker build container
docker build -f wesos-make.Dockerfile -t wesos-make:latest .

# To create a release ISO image, run the following command:
docker run -v /dev:/dev -v .:/app --rm --privileged -it wesos-make:latest release
# The release ISO image will be located at `./install/release/WesOS Live.iso` in
# the cloned repository.

# To create a debug ISO image, run the following command:
docker run -v /dev:/dev -v .:/app --rm --privileged -it wesos-make:latest debug
# The debug ISO image will be located at `./install/debug/WesOS Live.iso` in
# the cloned repository.

```

## Features

| Feature Name | Feature Description                 |
| ------------ | ----------------------------------- |
| ✅ UEFI Boot  | Boot from real hardware using UEFI. |

## Technology

| Tech Name  | URL                                  |
| ---------- | ------------------------------------ |
| CMake      | https://cmake.org/                   |
| GNU Make   | https://www.gnu.org/software/make/   |
| C++20      | https://en.cppreference.com/w/cpp/20 |
| LLVM       | https://llvm.org/                    |
| QEMU       | https://www.qemu.org/                |
| GNU Parted | https://www.gnu.org/software/parted/ |
| Docker     | https://www.docker.com/              |
| Doxygen    | https://www.doxygen.nl/              |
| Git        | https://git-scm.com/                 |

## Contributing

Contributions are welcome! Please submit a pull request or open an issue if you have suggestions.

## License

This project is licensed under the **Unlicense (public domain)** license. See the [LICENSE](LICENSE) file for more information.

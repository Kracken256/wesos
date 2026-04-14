# Running wesos in QEMU

This document describes how to run the built ISO image in QEMU using the `buildsys` tool, both with and without graphical display.

## Building the Docker Container (wesos-dev)

Before running any build or run commands, you must build the Docker image:

```
docker build -t wesos-dev .
```

Run this command from the root of the repository (where the Dockerfile is located). This only needs to be done once, or whenever you change the Dockerfile.

## Running the ISO in QEMU (with graphical display)

To run the ISO in QEMU with graphical output (Linux/X11):

```
docker run -it --rm \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $(pwd):/app \
    wesos-dev buildsys run --iso target/debug/wesos.iso
```

If you see an error like `Authorization required, but no authorization protocol specified`, run this on your host before starting Docker:

```
xhost +local:docker
```

To restrict access again:

```
xhost -local:docker
```

## Running QEMU in Headless Mode

If you do not need a graphical display, you can run QEMU in serial/console mode:

```
docker run -it --rm -v $(pwd):/app wesos-dev buildsys run --iso target/debug/wesos.iso -nographic
```

Or:

```
docker run -it --rm -v $(pwd):/app wesos-dev buildsys run --iso target/debug/wesos.iso -serial stdio -display none
```

## Custom QEMU Flags

You can pass any additional QEMU flags after `--`:

```
docker run -it --rm -v $(pwd):/app wesos-dev buildsys run --iso target/debug/wesos.iso -- -m 512M -smp 2
```

## Requirements

- Docker (for containerized runs)
- X11 server (for graphical QEMU display)
- On the host: `xhost` utility for X11 permissions

## Troubleshooting

- If QEMU fails with GTK/X11 errors, use the `xhost` command as above or run in headless mode.
- If you see `Failed to run QEMU: ... No such file or directory`, ensure `qemu-system-x86_64` is installed in your Docker image.

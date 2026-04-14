FROM rust:1.94

RUN apt update && apt install -y build-essential grub-pc-bin grub-efi-amd64-bin xorriso mtools qemu-system-x86

RUN mkdir -p /buildsys
COPY buildsys /buildsys
WORKDIR /buildsys
RUN cargo build --release --bin wesos_buildsys
RUN cp target/release/wesos_buildsys /usr/local/bin/buildsys

VOLUME /app
WORKDIR /app

CMD ["buildsys"]

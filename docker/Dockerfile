FROM ubuntu:24.04

RUN apt update && apt install -y \
    build-essential \
    gcc \
    g++ \
    nasm \
    make \
    grub-pc-bin \
    xorriso \
    mtools \
    qemu-system-x86 \
    gdb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /os

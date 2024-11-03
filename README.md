# atari-sw

Software for Atari 8-bit projects, see [atari-hw](https://github.com/dpicken/atari-hw) and [atari-fw](https://github.com/dpicken/atari-fw).

- !sbc-boot.atr: A menu-driven interface for the [SFF SD card adapter](https://github.com/dpicken/atari-hw/blob/main/doc/sbc-sd.md)
  <img src="/jpeg/sbc-menu.jpeg" width="480">

## Build software

Prepare the llvm-mos toolchain, e.g. on macOS:

```
mkdir ./third-party
curl --output-dir ./third-party -O -L https://github.com/llvm-mos/llvm-mos-sdk/releases/latest/download/llvm-mos-macos.tar.xz
xattr -d com.apple.quarantine ./third-party/llvm-mos-macos.tar.xz
tar -xf ./third-party/llvm-mos-macos.tar.xz -C ./third-party
```

Prepare the `dir2atr` tool:

```
git -C ./third-party clone https://github.com/HiassofT/AtariSIO.git
make -C ./third-party/AtariSIO posix
```

Clone and build:

```
git clone https://github.com/dpicken/fab.git
git clone https://github.com/dpicken/atari-fw.git
git clone https://github.com/dpicken/atari-sw.git
cd atari-sw
make
```

## Distribute !sbc-boot.atr to atari-fw

```
make sbc-boot-distribute
```

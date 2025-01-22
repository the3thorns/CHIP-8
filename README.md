# CHIP-8

## About

This repo serves as a simple implementation of the CHIP-8 emulator. The src directory has a few files that are worth to mention.

## Files

The `chip8.h/c` serve as an API for the emulator, later on, the `main.c` file takes the functions defined within these files and creates its own runtime.

## Graphical features

This implementation is meant to support graphics by using `raylib`. The `chip8.c` includes from `graphics.c` the specific functions that wrap the underlying `raylib` code.

This architecture gives the possibility to replace `raylib` with other graphics libary.

## ROMs

The compatibility between ROMs is not warranteed. It is possible that this emulator doesn't support specific features from other emulators. Discretion is advised

## Usage

The code of this repo can be compiled into a full working CHIP-8 emulator and used as a terminal tool.

- Shows help and usage guide:

```shell
ch8
```

or

```shell
ch8 --help
```

- Make the emulator execute a specific ROM (by default, graphical options are enabled):

```shell
ch8 <rom>
```

- Run a specific ROM and dump execution data (memory map, registers at the end, etc...):

```shell
ch8 <rom> -d <file-path>
```

- Disable graphical options during execution time:

```shell
ch8 <rom> -g
```

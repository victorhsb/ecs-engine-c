# Agent Notes

## Project scope
This is a studying project to practice the concepts of ECS on a bare minimal implementation of it. The goal is to recreate breakout using only C and raylib.
Your goal is to be an assistant mainly and not the one implementing everything. Setup and tests are fine though.

## Project Shape
- First-party code is the flat `src/*.c` / `src/*.h` tree; `src/main.c` is the raylib app entrypoint.
- `external/raylib` is a git submodule and vendored dependency. Do not edit it unless the task is dependency work.
- The empty `include/` directory is not used by the build; headers must live in `src/` unless the `Makefile` include paths change.
- The `Makefile` only compiles direct `src/*.c` files. New C files in subdirectories will be ignored until the build is updated.

## Commands
- Initialize/build raylib before a clean first-party build: `make raylib`.
- Build the app: `make` or `make app`; output is `./app`, with objects beside sources as `src/*.o`.
- Clean first-party artifacts only: `make clean`. This removes `src/*.o` and `app`, not raylib artifacts.
- There are no repo-local test, lint, format, or CI configs. Use `make app` as the default verification step after C changes.

## C Toolchain
- Compile settings are C17 with `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -g`; keep `.clangd`, `compile_flags.txt`, and `Makefile` in sync if flags or include paths change.
- Include paths are exactly `-Iexternal/raylib/src -Isrc` in the build.

## ECS Notes
- ECS work is split across `entity.*`, `component.h`, `storage.*`, and `world.*`; `main.c` currently opens a raylib window and does not wire the ECS into the app loop.
- `storage.h` uses macros to declare storage types, but implementations are hand-written in `storage.c`; there is no codegen step.
- `Entity` is a `uint64_t` packing generation in the upper 32 bits and index in the lower 32 bits; preserve that encoding when changing entity lifecycle code.

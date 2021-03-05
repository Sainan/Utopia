# Utopia Language Usage

## Preamble

Currently, no pre-compiled binaries are provided, so you will need to compile them yourself. In theory, this can be done using any compiler, but the recommended method is using clang via the provided PHP scripts, and installing everything you need to use that method is rather trivial.

### On ~~Linux~~ Debian

> `apt-get install clang php-cli`

You may also need `g++`/`gcc`.

### On Windows

- To get clang, head on over to the [LLVM downloads page](https://releases.llvm.org/download.html) and find the "pre-built binaries" section.
- To get PHP, head on over to the [PHP for Windows downloads page](https://windows.php.net/download/), select the zip appropriate for your system (thread safety doesn't matter), unzip it somewhere on your hard drive, add an entry to your `PATH` system variable, and ensure that `php -version` in a fresh console window works.

Additionally, clang will need you to have the Windows SDK and build tools so it can successfully compile. To get these, you can download the [Visual Studio Installer](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16), and within it you only need to select "MSVC" and "Windows 10 SDK".

## Interpreter

### Compiling

> `php compile-utopia.php interpreter`

### Usage

- Interpreting: `utopia-interpreter <uto-file>`
- Linting: `utopia-interpreter --lint <uto-file>`

## Compiler

### Usage

> `php compile-uto.php <uto-file>`

## Dynamic Library

### Compiling

> `php compile-utopia.php lib`

### Usage

Include `src/utopia.h` (for C & FFI) or `src/utopia.hpp` (for C++).

## Javascript Library (via WASM)

### Compiling

This will use [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) instead of Clang. Note that they have a _special_ way of setting environment variables.

> `php-compile-utopia.php wasmlib`


### Usage

Include `libutopia.js`.

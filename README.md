# Tiny Compiler

A compiler for the **Tiny** programming language, built with ANTLR4, C++17, and LLVM.

## Prerequisites

- CMake ≥ 3.20
- C++17 compiler (GCC 9+ / Clang 10+)
- ANTLR4 C++ runtime
- LLVM 15+ development libraries
- Java runtime (for ANTLR4 tool)

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install cmake g++ default-jre llvm-dev libantlr4-runtime-dev
# Download antlr4 jar:
curl -O https://www.antlr.org/download/antlr-4.13.2-complete.jar
alias antlr4='java -jar /path/to/antlr-4.13.2-complete.jar'
```

## Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Usage

```bash
# Compile a Tiny source file to LLVM IR
./tinyc examples/hello.tiny -o output.ll

# Run the IR directly
lli output.ll

# Or compile to native executable
clang output.ll runtime/libruntime.a -o program
./program
```

## Test

```bash
cd build && ctest --output-on-failure
# Or run end-to-end tests:
python3 tests/programs/run_tests.py
```

## Project Structure

See [PROJECT_STRUCTURE.md](docs/project-structure.md) for details.

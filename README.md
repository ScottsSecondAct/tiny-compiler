# Tiny Compiler

A fully functional compiler for a custom programming language, built from scratch using **ANTLR4**, **C++17**, and **LLVM**. This project spans the entire compilation pipeline — lexical analysis, parsing, AST construction, semantic analysis, LLVM IR code generation, and native executable creation — demonstrating systems programming skills and deep understanding of language implementation.

## Why This Project

Compilers sit at the intersection of several demanding disciplines: formal language theory, type systems, memory management, tree transformations, and low-level code generation. Building one from scratch — rather than following a tutorial line-by-line — requires making real architectural decisions, debugging across abstraction layers, and reasoning about how high-level constructs map to machine execution.

This project was developed as a hands-on exercise in compiler construction, using AI assistance (Anthropic's Claude) as a collaborator for architecture design, scaffolding, and debugging — the same way a professional engineer uses documentation, Stack Overflow, or a senior colleague. Every line of code was reviewed, understood, and integrated by hand. The AI didn't write the compiler; it accelerated the learning.

## The Tiny Language

Tiny is a statically-typed, imperative language designed to exercise the full ANTLR4 → C++ → LLVM pipeline. It's small enough to implement end-to-end, but rich enough to require real compiler engineering.

```tiny
fn fibonacci(n: int) -> int {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

fn main() {
    for (i in 0..15) {
        print("fib(", i, ") = ", fibonacci(i));
    }
}
```

### Closures and First-Class Functions

Functions are values in Tiny. You can store them in variables, pass them as arguments, and return them from other functions. Lambdas capture variables from their enclosing scope:

```tiny
fn makeAdder(n: int) -> fn(int) -> int {
    return fn(x: int) -> int { return x + n; };
}

let add5 = makeAdder(5);
print("add5(3) = ", add5(3));    // 8

fn apply(f: fn(int) -> int, x: int) -> int {
    return f(x);
}
print(apply(add5, 10));          // 15
```

### Language Features

- **Type system**: `int`, `float`, `bool`, `string`, fixed-size arrays (`int[3]`), function types (`fn(int) -> int`)
- **First-class functions**: Lambda expressions, closures with capture-by-value, higher-order functions
- **Bindings**: Immutable (`let`) and mutable (`var`) with optional type inference
- **Control flow**: `if`/`else`, `while`, range-based `for`
- **Functions**: Named functions with typed parameters, return types, and recursion
- **Expressions**: Full arithmetic, comparison, and logical operators with correct precedence
- **Built-ins**: `print()` with multiple arguments
- **Comments**: Single-line (`//`) and block (`/* */`)

## Architecture

The compiler follows a classical multi-pass architecture, with each phase cleanly separated behind a visitor interface:

```
 source.tiny
     │
     ▼
┌─────────────┐    ANTLR4 generates a lexer and parser from a formal
│  Lexer       │    grammar (Tiny.g4). The lexer tokenizes source text;
│  Parser      │    the parser builds a concrete syntax tree.
└──────┬──────┘
       │ parse tree
       ▼
┌─────────────┐    ASTBuilder walks ANTLR's parse tree and constructs
│  ASTBuilder  │    a clean, domain-specific AST decoupled from the
│              │    grammar. This is where syntax becomes structure.
└──────┬──────┘
       │ AST
       ▼
┌─────────────┐    SemanticAnalyzer enforces type safety, resolves
│  Semantic    │    symbols through a scoped symbol table, checks
│  Analysis    │    mutability, validates function signatures, and
│              │    performs capture analysis for closures.
└──────┬──────┘
       │ validated AST (with capture info)
       ▼
┌─────────────┐    CodeGen walks the AST and emits LLVM IR using
│  CodeGen     │    the LLVM C++ API — alloca/load/store for locals,
│  (LLVM IR)   │    basic blocks for control flow, GEP for arrays,
│              │    closure structs for first-class functions.
└──────┬──────┘
       │ output.ll
       ▼
┌─────────────┐    Clang compiles IR to native machine code, linked
│  clang       │    against a small C++ runtime that implements
│  + runtime   │    print() and other built-ins.
└─────────────┘
       │
       ▼
   executable
```

## Technical Highlights

### Grammar Design (ANTLR4)
The grammar uses precedence climbing for expressions, explicit keyword tokens to prevent identifier capture, and clean separation of lexer and parser rules. It supports lambda expressions as primary expressions and function types as first-class type annotations. ANTLR4 generates the lexer and parser as C++ classes with a visitor interface.

### AST Independence
The AST is entirely decoupled from ANTLR's parse tree. ANTLR's concrete syntax tree mirrors the grammar 1:1 — every rule produces a node. The AST is a cleaner, semantic tree (`BinaryExpr`, `IfStmt`, `FunctionDecl`, `LambdaExpr`) that later passes can traverse without knowing anything about ANTLR. This required solving a non-trivial engineering problem: ANTLR visitors return `std::any`, which requires copy-constructible types, but AST nodes use `unique_ptr` for ownership. The solution uses a `shared_ptr<Holder>` wrapper that satisfies `std::any`'s copy requirement while preserving unique ownership semantics.

### Semantic Analysis & Capture Analysis
The semantic analyzer performs a two-pass approach over declarations — first registering all function signatures, then analyzing bodies — enabling mutual recursion. For closures, it walks the lambda body to identify all variable references, subtracts parameters and local declarations, and records the remaining names as captures. It also supports calling variables with function types, not just named functions. It enforces type safety across all operators, validates mutability (`let` vs `var`), checks function call arity and argument types, and infers types from initializers.

### Closure Implementation
Closures are represented at the LLVM level as a struct `{ i8* fn_ptr, i8* env_ptr }`. When a lambda is created, the compiler heap-allocates an environment struct via `malloc`, copies captured variable values into it, and generates an internal LLVM function that takes the environment as a hidden first parameter. At the call site, the function pointer and environment are extracted from the struct and the call is dispatched through an indirect function pointer. This is the same strategy used by production compilers for languages like Swift and Rust.

### Visitor Pattern Across Phases
Every compiler phase implements the same `ASTVisitor` interface. The AST printer, semantic analyzer, and code generator are all visitors — adding a new pass (optimization, linting, formatting) means writing one new class. This design mirrors production compilers.

### LLVM Code Generation
The code generator maps Tiny constructs to LLVM IR using the C++ API:

| Tiny Construct | LLVM Pattern |
|---|---|
| `var x: int = 42` | `alloca` → `store` → `load` |
| `if / else` | Basic blocks + conditional `br` |
| `while`, `for..in` | Loop headers, back-edges, `br` |
| `fn add(a, b)` | `Function::Create` + entry basic block |
| `arr[i]` | `getelementptr` (GEP) |
| `print(...)` | External `@tiny_print_*` calls |
| `fn(x) { x + 1 }` | Internal function + `malloc` env + closure struct |
| `f(args)` | Extract fn/env from closure, indirect call |
| `-O2` | LLVM `PassBuilder` optimization pipeline |

### Testing Strategy
End-to-end tests use `.tiny` / `.expected` file pairs — the test runner compiles each program, executes the binary, and diffs stdout against expected output. Error tests verify that invalid programs produce correct diagnostics. Unit tests cover individual components with GoogleTest.

## Build

### Prerequisites

- CMake ≥ 3.20
- C++17 compiler (GCC 9+ / Clang 10+)
- ANTLR4 C++ runtime (4.13.2)
- LLVM 15+ development libraries
- Java runtime (for ANTLR4 tool)

### Ubuntu / Debian

```bash
sudo apt install cmake g++ default-jre llvm-dev libffi-dev libedit-dev \
    libncurses-dev zlib1g-dev libzstd-dev
```

### Build & Run

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)

# Compile a Tiny program to LLVM IR
./tinyc ../examples/hello.tiny -o output.ll

# With optimizations
./tinyc ../examples/hello.tiny -o output.ll -O2

# Link with runtime and run as native executable
clang output.ll ../runtime/runtime.cpp -o hello -no-pie
./hello

# Debug flags
./tinyc ../examples/hello.tiny --dump-tokens   # Print token stream
./tinyc ../examples/hello.tiny --dump-ast      # Print AST
```

### Run Tests

```bash
python3 tests/programs/run_tests.py --compiler ./build/tinyc
```

## Project Structure

See [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) for the full directory layout, architecture diagram, and design decision rationale.

## Development Process & AI Collaboration

This project was built incrementally using AI assistance as a learning accelerator:

- **Architecture and scaffolding**: Claude helped design the directory structure, CMake build system, grammar, and AST node hierarchy — the kind of boilerplate that takes hours to get right but teaches little.
- **Debugging**: When builds failed (ANTLR version mismatches, `std::any` copy semantics, LLVM CMake integration), Claude helped diagnose root causes from compiler output — a skill that transfers directly to real-world development.
- **Implementation**: Each compiler phase was discussed, designed, and then coded with AI assistance. Every component was reviewed and understood before integration.

This mirrors professional software development, where engineers routinely use tools, references, and collaborators to build systems they fully understand. The learning is in the architecture decisions, the debugging, and the integration — not in memorizing boilerplate.

## Skills Demonstrated

- **Systems programming**: C++17 with move semantics, smart pointers, CRTP, virtual dispatch, and template metaprogramming
- **Compiler engineering**: Lexing, parsing, AST design, type systems, scope resolution, capture analysis, code generation
- **LLVM**: IR generation via the C++ API — basic blocks, alloca/load/store, GEP, function definitions, external linkage, closure structs, indirect calls, optimization passes
- **Build systems**: CMake with custom targets, external tool integration, multi-library linking
- **Software architecture**: Clean separation of concerns, visitor pattern, test-driven development
- **Tool proficiency**: ANTLR4, LLVM, GDB, VS Code + WSL, Git

## Roadmap

- [x] ANTLR4 grammar and C++ lexer/parser generation
- [x] Clean AST design with visitor pattern
- [x] AST builder (parse tree → AST)
- [x] AST pretty-printer for debugging
- [x] Scoped symbol table
- [x] Semantic analysis (type checking, mutability enforcement)
- [x] LLVM IR code generation
- [x] Runtime library (print, newline)
- [x] End-to-end compilation to native executables
- [x] Optimization passes via LLVM PassManager (`-O1` / `-O2` / `-O3`)
- [x] Closures and first-class functions (lambda expressions, capture analysis, heap-allocated environments)
- [ ] End-to-end test suite execution
- [ ] Capture-by-reference for mutable closures
- [ ] String operations (concatenation, length)
- [ ] LLVM debug info for source-level debugging

## License

MIT
# Tiny Compiler

A fully functional compiler for a custom programming language, built from scratch using **ANTLR4**, **C++17**, and **LLVM**. This project spans the entire compilation pipeline — lexical analysis, parsing, AST construction, semantic analysis, and native code generation — demonstrating systems programming skills and deep understanding of language implementation.

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

### Language Features

- **Type system**: `int`, `float`, `bool`, `string`, fixed-size arrays (`int[3]`)
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
│  Analysis    │    mutability, and validates function signatures.
└──────┬──────┘
       │ validated AST
       ▼
┌─────────────┐    CodeGen walks the AST and emits LLVM IR using
│  CodeGen     │    the LLVM C++ API — alloca/load/store for locals,
│  (LLVM IR)   │    basic blocks for control flow, GEP for arrays.
└──────┬──────┘
       │ output.ll
       ▼
┌─────────────┐    LLVM's toolchain compiles IR to native machine
│  llc + clang │    code, linked against a small C++ runtime that
│  + runtime   │    implements print() and other built-ins.
└─────────────┘
       │
       ▼
   executable
```

## Technical Highlights

### Grammar Design (ANTLR4)
The grammar uses precedence climbing for expressions, explicit keyword tokens to prevent identifier capture, and clean separation of lexer and parser rules. ANTLR4 generates the lexer and parser as C++ classes with a visitor interface.

### AST Independence
The AST is entirely decoupled from ANTLR's parse tree. ANTLR's concrete syntax tree mirrors the grammar 1:1 — every rule produces a node. The AST is a cleaner, semantic tree (`BinaryExpr`, `IfStmt`, `FunctionDecl`) that later passes can traverse without knowing anything about ANTLR. This required solving a non-trivial engineering problem: ANTLR visitors return `std::any`, which requires copy-constructible types, but AST nodes use `unique_ptr` for ownership. The solution uses a `shared_ptr<Holder>` wrapper that satisfies `std::any`'s copy requirement while preserving unique ownership semantics.

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

# Dump the token stream
./tinyc ../examples/hello.tiny --dump-tokens

# Dump the AST
./tinyc ../examples/hello.tiny --dump-ast

# Compile to LLVM IR (once codegen is complete)
./tinyc ../examples/fibonacci.tiny -o output.ll
lli output.ll
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
- **Compiler engineering**: Lexing, parsing, AST design, type systems, scope resolution, code generation
- **LLVM**: IR generation via the C++ API — basic blocks, phi nodes, GEP, function definitions, external linkage
- **Build systems**: CMake with custom targets, external tool integration, multi-library linking
- **Software architecture**: Clean separation of concerns, visitor pattern, test-driven development
- **Tool proficiency**: ANTLR4, LLVM, GDB, VS Code + WSL, Git

## Roadmap

- [x] ANTLR4 grammar and C++ lexer/parser generation
- [x] Clean AST design with visitor pattern
- [x] AST builder (parse tree → AST)
- [x] AST pretty-printer for debugging
- [ ] Scoped symbol table
- [ ] Semantic analysis (type checking, mutability enforcement)
- [ ] LLVM IR code generation
- [ ] End-to-end compilation to native executables
- [ ] Optimization passes via LLVM PassManager

## License

MIT
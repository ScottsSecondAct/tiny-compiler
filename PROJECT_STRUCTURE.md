# Tiny Compiler — Project Structure

```
tiny-compiler/
│
├── CMakeLists.txt                          # Top-level build config (CMake 3.20+)
├── README.md                               # Build instructions & usage
├── ROADMAP.md                              # Phase-by-phase development guide
├── PROJECT_STRUCTURE.md                    # This file
├── LICENSE                                 # MIT license
├── .gitignore
├── .clang-format                           # C++ code style (LLVM-based)
│
├── .vscode/
│   ├── settings.json                       # CMake & ANTLR4 extension config
│   └── launch.json                         # Debug configurations
│
├── grammar/
│   └── Tiny.g4                             # ANTLR4 grammar definition
│
├── generated/                              # ANTLR4 output (gitignored, built by CMake)
│   ├── .gitkeep
│   ├── TinyLexer.h                         # Generated lexer
│   ├── TinyLexer.cpp
│   ├── TinyParser.h                        # Generated parser
│   ├── TinyParser.cpp
│   ├── TinyVisitor.h                       # Abstract visitor interface
│   ├── TinyBaseVisitor.h                   # Default no-op visitor to subclass
│   └── TinyBaseVisitor.cpp
│
├── include/tiny/                           # Public headers
│   ├── AST.h                               # ✅ AST node types, ASTVisitor, ASTBox
│   ├── ASTBuilder.h                        # ✅ ANTLR parse tree → AST converter
│   ├── ASTPrinter.h                        # ✅ Pretty-print AST for debugging
│   ├── SemanticAnalyzer.h                  # ✅ Type checking, symbol resolution
│   ├── SymbolTable.h                       # ✅ Scoped symbol table
│   ├── Diagnostics.h                       # ✅ Error/warning reporting
│   └── CodeGen.h                           # ✅ LLVM IR generation
│
├── src/                                    # Implementation files
│   ├── main.cpp                            # ✅ CLI entry point & full pipeline
│   ├── AST.cpp                             # ✅ binOpToString, etc.
│   ├── ASTBuilder.cpp                      # ✅ Parse tree → AST visitor
│   ├── ASTPrinter.cpp                      # ✅ AST → indented text dump
│   ├── SemanticAnalyzer.cpp                # ✅ Semantic passes
│   ├── SymbolTable.cpp                     # ✅ Scope push/pop, symbol lookup
│   ├── Diagnostics.cpp                     # ✅ Source locations, error formatting
│   └── CodeGen.cpp                         # ✅ AST → LLVM IR
│
├── runtime/                                # Linked runtime library for built-ins
│   ├── CMakeLists.txt                      # Builds libtiny_runtime.a
│   ├── runtime.h                           # ✅ C extern declarations
│   └── runtime.cpp                         # ✅ print, newline implementations
│
├── tests/
│   ├── CMakeLists.txt                      # Test build config
│   ├── unit/                               # Unit tests (GoogleTest)
│   │   ├── test_lexer.cpp                  # Token stream verification
│   │   ├── test_parser.cpp                 # Parse tree structure
│   │   ├── test_ast.cpp                    # AST builder round-trip
│   │   ├── test_semantic.cpp               # Type checking & errors
│   │   └── test_codegen.cpp                # IR output verification
│   │
│   └── programs/                           # End-to-end .tiny/.expected pairs
│       ├── run_tests.py                    # Test runner script
│       ├── basics/
│       │   ├── hello.tiny / .expected
│       │   ├── variables.tiny / .expected
│       │   └── arithmetic.tiny / .expected
│       ├── control_flow/
│       │   ├── if_else.tiny / .expected
│       │   ├── while_loop.tiny / .expected
│       │   └── for_range.tiny / .expected
│       ├── functions/
│       │   ├── factorial.tiny / .expected
│       │   ├── fibonacci.tiny / .expected
│       │   └── recursion.tiny / .expected
│       ├── arrays/
│       │   ├── indexing.tiny / .expected
│       │   └── dot_product.tiny / .expected
│       └── errors/
│           ├── type_mismatch.tiny / .expected
│           ├── undefined_var.tiny / .expected
│           └── immutable_assign.tiny / .expected
│
├── examples/                               # Showcase programs
│   ├── hello.tiny
│   ├── fizzbuzz.tiny
│   └── fibonacci.tiny
│
├── tools/
│   └── tiny-highlight.vim                  # Vim syntax highlighting
│
└── docs/
    ├── language-spec.md                    # Informal language reference
    ├── grammar-railroad.md                 # Railroad diagram links
    └── llvm-patterns.md                    # Tiny construct → LLVM IR cheat sheet
```

**Legend:** ✅ Implemented

## Architecture

```
                          ┌──────────────────────────────────────────┐
                          │              main.cpp                    │
                          │  CLI arg parsing, pipeline orchestration │
                          └────────┬─────────────┬──────────────────┘
                                   │             │
              ┌────────────────────▼──┐   ┌──────▼───────────────────┐
              │   ANTLR4 (generated/) │   │   --dump-tokens          │
              │   TinyLexer           │   │   Prints token stream    │
              │   TinyParser          │   │   and exits              │
              └────────────┬──────────┘   └──────────────────────────┘
                           │
                           │ parse tree
                           ▼
              ┌────────────────────────┐
              │   ASTBuilder           │   Phase 2 ✅
              │   (TinyBaseVisitor)    │   ANTLR parse tree → clean AST
              └────────────┬───────────┘
                           │
                           │ AST (unique_ptr<Program>)
                           ▼
              ┌────────────────────────┐
              │   ASTPrinter           │   --dump-ast
              │   (ASTVisitor)         │   Prints indented AST and exits
              └────────────┬───────────┘
                           │
                           ▼
              ┌────────────────────────┐
              │   SemanticAnalyzer     │   Phase 3 ✅
              │   SymbolTable          │   Type checking, scope resolution,
              │   Diagnostics          │   mutability enforcement
              └────────────┬───────────┘
                           │
                           │ validated AST
                           ▼
              ┌────────────────────────┐
              │   CodeGen              │   Phase 4 ✅
              │   (ASTVisitor)         │   AST → LLVM IR → .ll file
              └────────────┬───────────┘
                           │
                           │ output.ll
                           ▼
              ┌────────────────────────┐
              │   clang                │   Phase 5 ✅
              │   + runtime/runtime.cpp│   .ll → native executable
              └────────────────────────┘
```

## Key Design Decisions

### Separate AST from ANTLR parse tree
The ANTLR-generated parse tree mirrors the grammar 1:1 (every rule gets a node).
Our AST (`include/tiny/AST.h`) is a cleaner, domain-specific tree: `BinaryExpr`,
`IfStmt`, `FunctionDecl`, etc. This decouples later phases from the grammar and
makes visitors simpler to write.

### ASTBox wrapper for std::any
ANTLR visitors return `std::any`, but `std::any` requires copy-constructible
types. `unique_ptr` is move-only. `ASTBox` solves this by wrapping the
`unique_ptr` inside a `shared_ptr<Holder>`, allowing `std::any` to copy the
box while the underlying node stays uniquely owned. The `boxNode()` and
`unboxNode()` helpers keep this transparent.

### Visitor pattern across all phases
Every compiler phase implements `ASTVisitor`:
- `ASTPrinter` — debugging
- `SemanticAnalyzer` — validation
- `CodeGen` — LLVM IR emission

Adding a new pass (optimization, linting, formatting) = writing one new class.

### Runtime library
Built-in operations (`print`, string ops, bounds checks) are implemented in
C++ in `runtime/` and linked at compile time. The codegen declares them as
`extern` and calls them — simpler than emitting inline LLVM IR for each.

### End-to-end test pairs
Each test is a `.tiny` source + `.expected` output file. The runner compiles,
executes, and diffs. No framework needed for the integration layer.

## Build & Run

```bash
# Build the compiler
mkdir build && cd build
cmake .. && make -j$(nproc)

# Compile a Tiny program
./tinyc ../examples/hello.tiny -o output.ll

# Link with runtime and run
clang output.ll ../runtime/runtime.cpp -o hello -no-pie
./hello

# Debug flags
./tinyc ../examples/hello.tiny --dump-ast      # Print AST
./tinyc ../examples/hello.tiny --dump-tokens   # Print tokens

# Test
python3 ../tests/programs/run_tests.py --compiler ./tinyc
```
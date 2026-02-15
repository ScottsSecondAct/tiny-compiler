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
│   ├── SemanticAnalyzer.h                  # 🔲 Type checking, symbol resolution
│   ├── SymbolTable.h                       # 🔲 Scoped symbol table
│   ├── TypeSystem.h                        # 🔲 Type representations & rules
│   ├── CodeGen.h                           # 🔲 LLVM IR generation
│   └── Diagnostics.h                       # 🔲 Error/warning reporting
│
├── src/                                    # Implementation files
│   ├── main.cpp                            # ✅ CLI entry point & pipeline
│   ├── AST.cpp                             # ✅ binOpToString, etc.
│   ├── ASTBuilder.cpp                      # ✅ Parse tree → AST visitor
│   ├── ASTPrinter.cpp                      # ✅ AST → indented text dump
│   ├── SemanticAnalyzer.cpp                # 🔲 Semantic passes
│   ├── SymbolTable.cpp                     # 🔲 Scope push/pop, symbol lookup
│   ├── TypeSystem.cpp                      # 🔲 Type comparison, promotion
│   ├── CodeGen.cpp                         # 🔲 AST → LLVM IR
│   └── Diagnostics.cpp                     # 🔲 Source locations, error formatting
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

**Legend:** ✅ Implemented  🔲 Stub / TODO

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
              │   ASTBuilder           │   Phase 2
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
              │   SemanticAnalyzer     │   Phase 3 (TODO)
              │   SymbolTable          │   Type checking, scope resolution,
              │   TypeSystem           │   mutability enforcement
              └────────────┬───────────┘
                           │
                           │ validated AST
                           ▼
              ┌────────────────────────┐
              │   CodeGen              │   Phase 4 (TODO)
              │   (ASTVisitor)         │   AST → LLVM IR → .ll file
              └────────────┬───────────┘
                           │
                           │ output.ll
                           ▼
              ┌────────────────────────┐
              │   llc + clang          │   Phase 5
              │   + libtiny_runtime.a  │   .ll → native executable
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

### Visitor pattern for all phases
Every compiler phase implements `ASTVisitor`:
- `ASTPrinter` — debugging
- `SemanticAnalyzer` — validation (planned)
- `CodeGen` — LLVM IR emission (planned)

Adding a new pass = writing a new `ASTVisitor` subclass.

### Runtime library
Built-in operations (`print`, string ops, bounds checks) are implemented in
C++ in `runtime/` and compiled to a static library. The codegen declares them
as `extern` and calls them — simpler than emitting inline LLVM IR for each.

### End-to-end test pairs
Each test is a `.tiny` source + `.expected` output file. The runner compiles,
executes, and diffs. No framework needed for the integration layer.

## Build & Run

```bash
# Build
mkdir build && cd build
cmake .. && make -j$(nproc)

# Run
./tinyc ../examples/hello.tiny --dump-ast      # Print AST
./tinyc ../examples/hello.tiny --dump-tokens   # Print tokens
./tinyc ../examples/hello.tiny -o output.ll    # Compile (once codegen is done)

# Test
python3 ../tests/programs/run_tests.py --compiler ./tinyc
```
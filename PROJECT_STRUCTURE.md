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
│   ├── TinyLexer.h / .cpp                  # Generated lexer
│   ├── TinyParser.h / .cpp                 # Generated parser
│   ├── TinyVisitor.h                       # Abstract visitor interface
│   └── TinyBaseVisitor.h / .cpp            # Default no-op visitor to subclass
│
├── include/tiny/                           # Public headers
│   ├── AST.h                               # ✅ AST node types, ASTVisitor, ASTBox,
│   │                                       #    TypeKind::Function, LambdaExpr
│   ├── ASTBuilder.h                        # ✅ ANTLR parse tree → AST converter
│   ├── ASTPrinter.h                        # ✅ Pretty-print AST for debugging
│   ├── SemanticAnalyzer.h                  # ✅ Type checking, symbol resolution,
│   │                                       #    capture analysis for closures
│   ├── SymbolTable.h                       # ✅ Scoped symbol table
│   ├── Diagnostics.h                       # ✅ Error/warning reporting
│   └── CodeGen.h                           # ✅ LLVM IR generation, closure support,
│                                           #    optimization passes
│
├── src/                                    # Implementation files
│   ├── main.cpp                            # ✅ CLI entry point & full pipeline
│   ├── AST.cpp                             # ✅ binOpToString, etc.
│   ├── ASTBuilder.cpp                      # ✅ Parse tree → AST visitor (incl. lambdas)
│   ├── ASTPrinter.cpp                      # ✅ AST → indented text dump
│   ├── SemanticAnalyzer.cpp                # ✅ Semantic passes + capture analysis
│   ├── SymbolTable.cpp                     # ✅ Scope push/pop, symbol lookup
│   ├── Diagnostics.cpp                     # ✅ Source locations, error formatting
│   └── CodeGen.cpp                         # ✅ AST → LLVM IR + closures + opt passes
│
├── runtime/                                # Linked runtime library for built-ins
│   ├── CMakeLists.txt                      # Builds libtiny_runtime.a
│   ├── runtime.h                           # ✅ C extern declarations
│   └── runtime.cpp                         # ✅ print, newline implementations
│
├── tests/
│   ├── CMakeLists.txt                      # Test build config
│   ├── unit/                               # Unit tests (GoogleTest)
│   │   ├── test_lexer.cpp
│   │   ├── test_parser.cpp
│   │   ├── test_ast.cpp
│   │   ├── test_semantic.cpp
│   │   └── test_codegen.cpp
│   │
│   └── programs/                           # End-to-end .tiny/.expected pairs
│       ├── run_tests.py
│       ├── basics/
│       ├── control_flow/
│       ├── functions/
│       ├── arrays/
│       ├── closures/                       # Closure & first-class function tests
│       │   ├── simple_lambda.tiny / .expected
│       │   ├── higher_order.tiny / .expected
│       │   ├── capture.tiny / .expected
│       │   └── counter.tiny / .expected
│       └── errors/
│
├── examples/                               # Showcase programs
│   ├── hello.tiny
│   ├── fizzbuzz.tiny
│   ├── fibonacci.tiny
│   └── closures.tiny                       # Closure & first-class function demos
│
├── tools/
│   └── tiny-highlight.vim
│
└── docs/
    ├── language-spec.md
    ├── grammar-railroad.md
    └── llvm-patterns.md
```

**Legend:** ✅ Implemented

## Architecture

```
                          ┌──────────────────────────────────────────┐
                          │              main.cpp                    │
                          │  CLI arg parsing, pipeline orchestration │
                          │  Flags: --dump-ast --dump-tokens         │
                          │         -O0/-O1/-O2/-O3 -o output.ll    │
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
              │                        │   Handles lambdas, function types
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
              │   Diagnostics          │   mutability, capture analysis
              └────────────┬───────────┘
                           │
                           │ validated AST (with captures)
                           ▼
              ┌────────────────────────┐
              │   CodeGen              │   Phase 4 + 6 ✅
              │   (ASTVisitor)         │   AST → LLVM IR → .ll file
              │                        │   Closures: {fn_ptr, env_ptr}
              │                        │   Heap-allocated environments
              └────────────┬───────────┘
                           │
                           │ output.ll (optionally optimized)
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
`IfStmt`, `FunctionDecl`, `LambdaExpr`, etc. This decouples later phases from the
grammar and makes visitors simpler to write.

### ASTBox wrapper for std::any
ANTLR visitors return `std::any`, but `std::any` requires copy-constructible
types. `unique_ptr` is move-only. `ASTBox` solves this by wrapping the
`unique_ptr` inside a `shared_ptr<Holder>`, allowing `std::any` to copy the
box while the underlying node stays uniquely owned.

### Visitor pattern across all phases
Every compiler phase implements `ASTVisitor`:
- `ASTPrinter` — debugging
- `SemanticAnalyzer` — validation & capture analysis
- `CodeGen` — LLVM IR emission

Adding a new pass = writing one new class.

### Closure representation
Closures are a `{ i8* fn_ptr, i8* env_ptr }` struct. The function is an internal
LLVM function with an extra `i8* env` first parameter. The environment is a
heap-allocated struct (`malloc`) containing copies of captured variable values.
At the call site, the fn pointer and env pointer are extracted from the struct
and the call is dispatched indirectly. This is the same pattern used by Swift
and Rust compilers.

### Capture analysis in the semantic analyzer
The `findCaptures` method walks the lambda body to collect all `Identifier`
references, subtracts parameter names and locally-declared variables, then
checks each remaining name against the enclosing symbol table. Only non-function
symbols are captured (functions are resolved directly by name at the LLVM level).

### Runtime library
Built-in operations (`print`, string ops, bounds checks) are implemented in
C++ in `runtime/` and linked at compile time. The codegen declares them as
`extern` and calls them. `malloc` and `free` are also declared for closure
environments.

### End-to-end test pairs
Each test is a `.tiny` source + `.expected` output file. The runner compiles,
executes, and diffs. No framework needed for the integration layer.

## Build & Run

```bash
# Build the compiler
mkdir build && cd build
cmake .. && make -j$(nproc)

# Compile a Tiny program
./tinyc ../examples/closures.tiny -o output.ll

# With optimizations
./tinyc ../examples/closures.tiny -o output.ll -O2

# Link with runtime and run
clang output.ll ../runtime/runtime.cpp -o closures -no-pie
./closures

# Debug flags
./tinyc ../examples/closures.tiny --dump-ast
./tinyc ../examples/closures.tiny --dump-tokens

# Test
python3 ../tests/programs/run_tests.py --compiler ./tinyc
```
# Tiny Language — Compiler Project Roadmap

## Language Overview

**Tiny** is a statically-typed, imperative language designed to exercise the full
ANTLR4 → C++ → LLVM IR pipeline without drowning in complexity.

| Feature | LLVM Concept Exercised |
|---|---|
| `int`, `float`, `bool` | `i64`, `double`, `i1` types |
| `let` / `var` | `alloca` + `store` / `load`, immutability checks |
| Arithmetic & comparison | `add`, `fadd`, `icmp`, `fcmp` instructions |
| `if` / `else` | Basic blocks, conditional `br` |
| `while`, `for..in` | Loop basic blocks, back-edges |
| Functions | `define`, `call`, `ret`, parameter passing |
| Arrays `int[3]` | `[3 x i64]`, GEP (getelementptr) |
| `print()` | External runtime calls (`tiny_print_int`, etc.) |
| `string` | `i8*` global constants via `CreateGlobalStringPtr` |

## Build Pipeline

```
┌─────────────┐     ┌────────────┐     ┌──────────────┐     ┌──────────┐
│  source.tiny │────▶│ ANTLR4     │────▶│ AST Visitor  │────▶│ LLVM IR  │
│  (text)      │     │ Parse Tree │     │ (C++)        │     │ (.ll)    │
└─────────────┘     └────────────┘     └──────────────┘     └──────────┘
                                                                  │
                                                          clang + runtime
                                                                  │
                                                                  ▼
                                                          ┌──────────┐
                                                          │ Executable│
                                                          └──────────┘
```

## Phase-by-Phase Plan

### Phase 1 — Lexer & Parser (ANTLR4) ✅

```bash
# Generate C++ lexer/parser from the grammar
antlr4 -Dlanguage=Cpp -visitor -no-listener Tiny.g4

# This produces:
#   TinyLexer.h / .cpp
#   TinyParser.h / .cpp
#   TinyVisitor.h          ← base visitor interface
#   TinyBaseVisitor.h      ← default (no-op) visitor to subclass
```

**Milestone:** Parse `examples.tiny` and dump the parse tree. ✅

**What was built:**
- ANTLR4 grammar (`grammar/Tiny.g4`) covering all language features
- CMake integration for automatic grammar regeneration
- Token dumping via `--dump-tokens` flag
- Parse error detection and reporting

### Phase 2 — AST Design (C++) ✅

Clean AST separated from ANTLR's parse tree:

```
Program
├── FunctionDecl { name, params[], returnType, body }
├── VarDecl      { name, type, init, mutable }
├── Assignment   { target, index?, value }
├── IfStmt       { condition, thenBlock, elseBlock? }
├── WhileStmt    { condition, body }
├── ForStmt      { varName, start, end, body }
├── ReturnStmt   { value? }
├── PrintStmt    { args[] }
├── Block        { statements[] }
└── Expressions
    ├── BinaryExpr   { op, left, right }
    ├── UnaryExpr    { op, operand }
    ├── CallExpr     { callee, args[] }
    ├── IndexExpr    { array, index }
    ├── IntLit, FloatLit, BoolLit, StringLit
    ├── Identifier
    └── ArrayLiteral { elements[] }
```

**Milestone:** `ASTBuilder` visitor converts parse tree → AST, pretty-prints it. ✅

**What was built:**
- `AST.h` — Full node hierarchy with CRTP-based visitor dispatch
- `ASTBuilder` — ANTLR `TinyBaseVisitor` subclass that constructs AST nodes
- `ASTPrinter` — Debugging visitor that pretty-prints the tree with indentation
- `ASTBox` wrapper — Solved the `std::any` / `unique_ptr` incompatibility

**Key engineering challenge:** ANTLR visitors return `std::any`, which requires copy-constructible types. AST nodes use `unique_ptr` for ownership. The `ASTBox` solution wraps the `unique_ptr` inside a `shared_ptr<Holder>`, satisfying `std::any`'s copy requirement while preserving unique ownership semantics.

### Phase 3 — Semantic Analysis (C++) ✅

Walks the AST to enforce language rules:

1. **Symbol table** — scoped stack of maps: `name → {type, mutable?, location}`
2. **Type checking** — binary ops require matching types, assignment respects declared type
3. **Mutability enforcement** — `let` bindings cannot be reassigned
4. **Function signatures** — verify call arity and argument types
5. **Return type validation** — return values must match function signature
6. **Forward references** — two-pass declaration allows mutual recursion

**Milestone:** Report meaningful errors for type mismatches and undefined variables. ✅

**What was built:**
- `SymbolTable` — Scoped name resolution with push/pop for blocks, functions, and loops
- `SemanticAnalyzer` — `ASTVisitor` subclass that populates the symbol table and checks all rules
- `Diagnostics` — Error/warning collector with source line numbers

**Checks implemented:** undefined variables/functions, duplicate declarations, immutable reassignment, type mismatches in assignments/operators/returns, non-bool conditions, non-int array indices, function call arity and argument types, type inference from initializers.

### Phase 4 — LLVM IR Code Generation (C++) ✅

`CodeGen` is an `ASTVisitor` subclass that emits LLVM IR using the C++ API:

| Tiny Construct | LLVM IR Pattern |
|---|---|
| `let x: int = 42` | `alloca i64` → `store i64 42` |
| `x + y` | `load` both → `add i64` (or `fadd double`) |
| `if / else` | Three basic blocks + `br i1` |
| `while` | Header, body, exit blocks + back-edge |
| `for i in 0..n` | Desugars to while-style loop with alloca counter |
| `fn add(a, b)` | `define i64 @add(i64 %a, i64 %b)` + entry BB |
| `arr[i]` | `getelementptr` → `load` |
| `print(...)` | `call @tiny_print_int(i64 %val)` + `@tiny_print_newline()` |

**Milestone:** Generate `.ll` file, compile with `clang`, run native executable. ✅

**What was built:**
- `CodeGen` — Full `ASTVisitor` using `llvm::IRBuilder`, `llvm::Module`, `llvm::Function`
- Scoped variable tracking (`alloca` per local, `load`/`store` for access)
- Two-pass function handling (declare all first, then generate bodies)
- Runtime function declarations, array initialization via GEP, implicit `main()` wrapping
- LLVM module verification and `.ll` file output

### Phase 5 — Link & Run ✅

```bash
# Compile .tiny → .ll
./build/tinyc examples/hello.tiny -o output.ll

# Link with runtime and run
clang output.ll runtime/runtime.cpp -o hello -no-pie
./hello
```

**What was built:**
- C++ runtime library with `extern "C"` functions: `tiny_print_int`, `tiny_print_float`, `tiny_print_str`, `tiny_print_bool`, `tiny_print_newline`
- Full end-to-end pipeline: `.tiny` → parse → AST → semantic check → LLVM IR → native binary

## What's Next

### Possible Extensions

- **Optimization passes** → `llvm::PassManager` (constant folding, dead code elimination, inlining)
- **Closures / first-class functions** → LLVM function pointers
- **Structs** → `llvm::StructType`
- **Heap allocation** → `malloc` / GC
- **Pattern matching** → `switch` instruction
- **Modules / imports** → multiple LLVM modules + linking
- **String operations** → concatenation, length, slicing via runtime library
- **Error recovery** → continue parsing after errors to report multiple issues
- **Source maps** → LLVM debug info for GDB/LLDB stepping through `.tiny` files
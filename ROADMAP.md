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
| Lambdas / closures | Closure structs, `malloc`, indirect calls |
| First-class functions | Function types, higher-order functions |
| Optimization | LLVM `PassBuilder` pipeline (`-O1` / `-O2` / `-O3`) |

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

**Milestone:** Parse `examples.tiny` and dump the parse tree. ✅

**What was built:**
- ANTLR4 grammar (`grammar/Tiny.g4`) covering all language features including lambda expressions and function types
- CMake integration for automatic grammar regeneration
- Token dumping via `--dump-tokens` flag
- Parse error detection and reporting

### Phase 2 — AST Design (C++) ✅

**Milestone:** `ASTBuilder` visitor converts parse tree → AST, pretty-prints it. ✅

**What was built:**
- `AST.h` — Full node hierarchy with CRTP-based visitor dispatch, including `LambdaExpr` and `TypeKind::Function`
- `ASTBuilder` — ANTLR `TinyBaseVisitor` subclass that constructs AST nodes
- `ASTPrinter` — Debugging visitor that pretty-prints the tree with indentation
- `ASTBox` wrapper — Solved the `std::any` / `unique_ptr` incompatibility

**Key engineering challenge:** ANTLR visitors return `std::any`, which requires copy-constructible types. AST nodes use `unique_ptr` for ownership. The `ASTBox` solution wraps the `unique_ptr` inside a `shared_ptr<Holder>`, satisfying `std::any`'s copy requirement while preserving unique ownership semantics.

### Phase 3 — Semantic Analysis (C++) ✅

**Milestone:** Report meaningful errors for type mismatches and undefined variables. ✅

**What was built:**
- `SymbolTable` — Scoped name resolution with push/pop for blocks, functions, and loops
- `SemanticAnalyzer` — `ASTVisitor` subclass that populates the symbol table and checks all rules
- `Diagnostics` — Error/warning collector with source line numbers

**Checks implemented:** undefined variables/functions, duplicate declarations, immutable reassignment, type mismatches in assignments/operators/returns, non-bool conditions, non-int array indices, function call arity and argument types, type inference from initializers, calling variables with function types.

### Phase 4 — LLVM IR Code Generation (C++) ✅

**Milestone:** Generate `.ll` file, compile with `clang`, run native executable. ✅

**What was built:**
- `CodeGen` — Full `ASTVisitor` using `llvm::IRBuilder`, `llvm::Module`, `llvm::Function`
- Scoped variable tracking (`alloca` per local, `load`/`store` for access)
- Two-pass function handling (declare all first, then generate bodies)
- Runtime function declarations, array initialization via GEP, implicit `main()` wrapping
- LLVM module verification and `.ll` file output

### Phase 5 — Link & Run ✅

```bash
./build/tinyc examples/hello.tiny -o output.ll
clang output.ll runtime/runtime.cpp -o hello -no-pie
./hello
```

**What was built:**
- C++ runtime library with `extern "C"` functions: `tiny_print_int`, `tiny_print_float`, `tiny_print_str`, `tiny_print_bool`, `tiny_print_newline`
- Full end-to-end pipeline: `.tiny` → parse → AST → semantic check → LLVM IR → native binary

### Phase 5.5 — Optimization Passes ✅

```bash
./build/tinyc examples/fibonacci.tiny -o fib.ll -O2
```

**What was built:**
- LLVM `PassBuilder` integration with `-O0` / `-O1` / `-O2` / `-O3` flags
- Runs the full standard LLVM optimization pipeline (same passes as `clang -O2`): constant folding, dead code elimination, inlining, SROA, loop optimizations, GVN

### Phase 6 — Closures & First-Class Functions ✅

**Milestone:** Lambda expressions with capture, higher-order functions, function types as values. ✅

```tiny
fn makeAdder(n: int) -> fn(int) -> int {
    return fn(x: int) -> int { return x + n; };
}
let add5 = makeAdder(5);
print(add5(3));  // 8
```

**What was built across every compiler layer:**

**Grammar** — Added `lambdaExpr` rule (`fn(params) -> type { body }` as a primary expression) and function type syntax (`fn(int, int) -> int`) in `typeSpec`, plus `typeList` rule.

**AST** — Added `TypeKind::Function` with `paramTypes`/`returnType` on `TypeSpec`, `LambdaExpr` node (params, return type, body, `captures` vector), and `calleeExpr` on `CallExpr` for indirect calls.

**ASTBuilder** — `buildType` handles `fn(types) -> type`. `visitPrimary` recognizes lambda expressions. `visitPostfix` detects when a call target isn't a simple `Identifier` and routes to indirect call path.

**SemanticAnalyzer** — `visit(LambdaExpr)` type-checks the body and runs capture analysis. `findCaptures` walks the lambda body collecting all `Identifier` references, subtracts params and locals, stores the result in `node.captures`. `visit(CallExpr)` handles three cases: indirect expression calls, direct calls to function-typed variables, and traditional named function calls.

**CodeGen** — Closure representation is `{ i8* fn_ptr, i8* env_ptr }`. `visit(LambdaExpr)` creates an internal LLVM function with an extra `i8* env` first parameter, heap-allocates an environment struct via `malloc`, copies captured values into it, and returns the closure struct. `emitClosureCall` extracts the function pointer and environment from the struct and dispatches through an indirect call.

| LLVM concept | Where it's used |
|---|---|
| `StructType::create` | Closure struct `{ fnPtr, envPtr }` and per-lambda environment types |
| `malloc` / heap allocation | Environment structs for captured variables |
| `CreateBitCast` | Casting `i8*` ↔ typed environment pointers |
| `CreateExtractValue` | Extracting fn/env from the closure struct |
| `CreateInsertValue` | Building the closure struct |
| `CreateStructGEP` | Accessing captured variables in the environment |
| Indirect `CreateCall` | Calling through a function pointer |
| `InternalLinkage` | Lambda functions are module-private |

## What's Next

### Possible Extensions

- **Capture-by-reference** → store pointers to heap-allocated variables instead of copying values, enabling mutable closures
- **Structs** → `llvm::StructType`, field access via GEP
- **Heap allocation & GC** → `malloc` / reference counting / tracing GC
- **Pattern matching** → `switch` instruction
- **Modules / imports** → multiple LLVM modules + linking
- **String operations** → concatenation, length, slicing via runtime library
- **Error recovery** → continue parsing after errors to report multiple issues
- **Source maps** → LLVM debug info for GDB/LLDB stepping through `.tiny` files
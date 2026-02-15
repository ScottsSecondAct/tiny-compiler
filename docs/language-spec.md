# Tiny Language Specification

## Types

| Type | Description | LLVM |
|------|-------------|------|
| `int` | 64-bit signed integer | `i64` |
| `float` | 64-bit double precision | `double` |
| `bool` | Boolean | `i1` |
| `string` | String literal | `i8*` |
| `T[N]` | Fixed-size array | `[N x T]` |

## Variable Declarations

```tiny
let x: int = 42;      // immutable
var y: int = 0;        // mutable
let z = 3.14;          // type inferred as float
```

## Functions

```tiny
fn add(a: int, b: int) -> int {
    return a + b;
}

fn greet(name: string) {
    print("Hello, ", name);
}
```

## Control Flow

```tiny
if (condition) { ... } else { ... }
while (condition) { ... }
for (i in start..end) { ... }     // end is exclusive
```

## Operators

| Precedence | Operators | Associativity |
|------------|-----------|---------------|
| 1 (lowest) | `\|\|` | Left |
| 2 | `&&` | Left |
| 3 | `==` `!=` | Left |
| 4 | `<` `<=` `>` `>=` | Left |
| 5 | `+` `-` | Left |
| 6 | `*` `/` `%` | Left |
| 7 (highest) | `-` (unary) `!` | Right |

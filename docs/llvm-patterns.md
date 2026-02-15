# LLVM IR Patterns for Tiny

Quick reference: how each Tiny construct maps to LLVM IR.

## Integer Variable

```tiny
var x: int = 42;
```

```llvm
%x = alloca i64
store i64 42, i64* %x
```

## If / Else

```tiny
if (x > 0) { ... } else { ... }
```

```llvm
%cmp = icmp sgt i64 %x, 0
br i1 %cmp, label %then, label %else

then:
  ; ...
  br label %merge

else:
  ; ...
  br label %merge

merge:
  ; continues here
```

## While Loop

```llvm
br label %loop.header

loop.header:
  %cmp = icmp slt i64 %i, %n
  br i1 %cmp, label %loop.body, label %loop.exit

loop.body:
  ; ...
  br label %loop.header

loop.exit:
  ; continues here
```

## Function

```tiny
fn add(a: int, b: int) -> int {
    return a + b;
}
```

```llvm
define i64 @add(i64 %a, i64 %b) {
entry:
  %tmp = add i64 %a, %b
  ret i64 %tmp
}
```

## Array Access (GEP)

```tiny
let arr: int[3] = [1, 2, 3];
print(arr[1]);
```

```llvm
%arr = alloca [3 x i64]
; ... store elements ...
%ptr = getelementptr [3 x i64], [3 x i64]* %arr, i64 0, i64 1
%val = load i64, i64* %ptr
```

## Print (external call)

```llvm
declare void @tiny_print_int(i64)

call void @tiny_print_int(i64 %val)
call void @tiny_print_newline()
```

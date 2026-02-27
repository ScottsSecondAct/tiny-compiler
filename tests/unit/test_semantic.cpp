// test_semantic.cpp — Unit tests for SemanticAnalyzer
//
// Each test parses a source string, runs SemanticAnalyzer, and checks
// whether errors are produced and what messages they carry.

#include <gtest/gtest.h>
#include <string>

#include "antlr4-runtime.h"
#include "TinyLexer.h"
#include "TinyParser.h"

#include "tiny/AST.h"
#include "tiny/ASTBuilder.h"
#include "tiny/Diagnostics.h"
#include "tiny/SemanticAnalyzer.h"

using namespace tiny;

// ── Helper ────────────────────────────────────────────────────────────────────

struct SemanticResult {
    Diagnostics diags;
    bool ok() const          { return !diags.hasErrors(); }
    bool hasError() const    { return diags.hasErrors(); }
    int errorCount() const   { return diags.errorCount(); }

    bool hasErrorContaining(const std::string& sub) const {
        for (auto& d : diags.all()) {
            if (d.level == DiagLevel::Error &&
                d.message.find(sub) != std::string::npos)
                return true;
        }
        return false;
    }
};

static SemanticResult analyze(const std::string& src) {
    antlr4::ANTLRInputStream input(src);
    TinyLexer lexer(&input);
    lexer.removeErrorListeners();
    antlr4::CommonTokenStream tokens(&lexer);
    TinyParser parser(&tokens);
    parser.removeErrorListeners();
    auto* tree = parser.program();

    ASTBuilder builder;
    auto ast = builder.buildProgram(tree);

    SemanticResult r;
    SemanticAnalyzer sa(r.diags);
    sa.analyze(*ast);
    return r;
}

// ── Valid programs — no errors ─────────────────────────────────────────────────

TEST(SemanticTest, EmptyProgram_Valid) {
    EXPECT_TRUE(analyze("").ok());
}

TEST(SemanticTest, IntDecl_Valid) {
    EXPECT_TRUE(analyze("let x: int = 42;").ok());
}

TEST(SemanticTest, FloatDecl_Valid) {
    EXPECT_TRUE(analyze("let x: float = 3.14;").ok());
}

TEST(SemanticTest, BoolDecl_Valid) {
    EXPECT_TRUE(analyze("let b: bool = true;").ok());
}

TEST(SemanticTest, StringDecl_Valid) {
    EXPECT_TRUE(analyze("let s: string = \"hello\";").ok());
}

TEST(SemanticTest, VarDecl_TypeInferred_Valid) {
    EXPECT_TRUE(analyze("var x = 5;").ok());
}

TEST(SemanticTest, VarDecl_MutableAssignment_Valid) {
    EXPECT_TRUE(analyze("var x: int = 0; x = 10;").ok());
}

TEST(SemanticTest, BinaryArith_IntPlusInt_Valid) {
    EXPECT_TRUE(analyze("let z: int = 1 + 2;").ok());
}

TEST(SemanticTest, BinaryArith_FloatMinusFloat_Valid) {
    EXPECT_TRUE(analyze("let z: float = 5.0 - 2.0;").ok());
}

TEST(SemanticTest, StringConcat_Valid) {
    EXPECT_TRUE(analyze("let s: string = \"a\" + \"b\";").ok());
}

TEST(SemanticTest, FunctionDecl_Valid) {
    EXPECT_TRUE(analyze("fn add(a: int, b: int) -> int { return a + b; }").ok());
}

TEST(SemanticTest, FunctionCall_Valid) {
    EXPECT_TRUE(analyze(
        "fn square(n: int) -> int { return n * n; }\n"
        "let x: int = square(5);"
    ).ok());
}

TEST(SemanticTest, RecursiveFunction_Valid) {
    EXPECT_TRUE(analyze(
        "fn fib(n: int) -> int {"
        "  if (n <= 1) { return n; }"
        "  return fib(n - 1) + fib(n - 2);"
        "}"
    ).ok());
}

TEST(SemanticTest, MutualRecursion_Valid) {
    EXPECT_TRUE(analyze(
        "fn isEven(n: int) -> bool {"
        "  if (n == 0) { return true; }"
        "  return isOdd(n - 1);"
        "}"
        "fn isOdd(n: int) -> bool {"
        "  if (n == 0) { return false; }"
        "  return isEven(n - 1);"
        "}"
    ).ok());
}

TEST(SemanticTest, IfStmt_BoolCondition_Valid) {
    EXPECT_TRUE(analyze("if (true) { }").ok());
}

TEST(SemanticTest, IfElse_Valid) {
    EXPECT_TRUE(analyze("var x: int = 0; if (x == 0) { x = 1; } else { x = 2; }").ok());
}

TEST(SemanticTest, WhileStmt_BoolCondition_Valid) {
    EXPECT_TRUE(analyze("while (false) { }").ok());
}

TEST(SemanticTest, ForStmt_IntRange_Valid) {
    EXPECT_TRUE(analyze("for (i in 0..10) { }").ok());
}

TEST(SemanticTest, ForStmt_LoopVarUsableInBody_Valid) {
    EXPECT_TRUE(analyze("for (i in 0..5) { print(i); }").ok());
}

TEST(SemanticTest, ArrayDecl_Valid) {
    EXPECT_TRUE(analyze("let arr: int[3] = [1, 2, 3];").ok());
}

TEST(SemanticTest, ArrayIndex_Valid) {
    EXPECT_TRUE(analyze(
        "let arr: int[3] = [1, 2, 3];\n"
        "let x: int = arr[0];"
    ).ok());
}

TEST(SemanticTest, Lambda_Valid) {
    EXPECT_TRUE(analyze("let f = fn(x: int) -> int { return x; };").ok());
}

TEST(SemanticTest, Lambda_Closure_Valid) {
    EXPECT_TRUE(analyze(
        "var x: int = 10;"
        "let addX = fn(n: int) -> int { return n + x; };"
    ).ok());
}

TEST(SemanticTest, Print_MultiArg_Valid) {
    EXPECT_TRUE(analyze("print(1, \"hi\", true);").ok());
}

TEST(SemanticTest, NestedScopes_Valid) {
    EXPECT_TRUE(analyze(
        "let x: int = 1;"
        "{"
        "  let y: int = 2;"
        "  let z: int = x + y;"
        "}"
    ).ok());
}

TEST(SemanticTest, Shadowing_InnerScope_Valid) {
    EXPECT_TRUE(analyze(
        "let x: int = 1;"
        "{"
        "  let x: float = 2.0;"
        "}"
    ).ok());
}

TEST(SemanticTest, ComparisonExpr_Valid) {
    EXPECT_TRUE(analyze("let b: bool = 3 < 5;").ok());
}

TEST(SemanticTest, LogicalExpr_Valid) {
    EXPECT_TRUE(analyze("let b: bool = true && false;").ok());
}

TEST(SemanticTest, UnaryNeg_Valid) {
    EXPECT_TRUE(analyze("let x: int = -5;").ok());
}

TEST(SemanticTest, UnaryNot_Valid) {
    EXPECT_TRUE(analyze("let b: bool = !true;").ok());
}

TEST(SemanticTest, EmptyFunction_Void_Valid) {
    EXPECT_TRUE(analyze("fn f() { }").ok());
}

TEST(SemanticTest, DeepNestedScopes_Valid) {
    EXPECT_TRUE(analyze(
        "let x: int = 1;"
        "{"
        "  let y: int = 2;"
        "  {"
        "    let z: int = x + y;"
        "    print(z);"
        "  }"
        "}"
    ).ok());
}

// ── Undefined variable / function ─────────────────────────────────────────────

TEST(SemanticTest, UndefinedVariable_Error) {
    auto r = analyze("let x: int = y;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("undefined variable 'y'"));
}

TEST(SemanticTest, UndefinedFunction_Error) {
    auto r = analyze("foo();");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("undefined function 'foo'"));
}

TEST(SemanticTest, UseAfterScope_Error) {
    auto r = analyze("{ let inner: int = 5; } let x: int = inner;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("undefined variable 'inner'"));
}

TEST(SemanticTest, ForLoopVar_NotAccessibleAfterLoop_Error) {
    auto r = analyze("for (i in 0..5) { } print(i);");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("undefined variable 'i'"));
}

// ── Type mismatches ───────────────────────────────────────────────────────────

TEST(SemanticTest, TypeMismatch_IntExpected_FloatGiven_Error) {
    auto r = analyze("let x: int = 3.14;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("type mismatch"));
}

TEST(SemanticTest, TypeMismatch_FloatExpected_IntGiven_Error) {
    auto r = analyze("let x: float = 5;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("type mismatch"));
}

TEST(SemanticTest, TypeMismatch_BoolExpected_IntGiven_Error) {
    auto r = analyze("let x: bool = 1;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("type mismatch"));
}

TEST(SemanticTest, TypeMismatch_Assignment_Error) {
    auto r = analyze("var x: int = 0; x = 3.14;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("assignment"));
}

// ── Immutability ──────────────────────────────────────────────────────────────

TEST(SemanticTest, ImmutableAssignment_Error) {
    auto r = analyze("let x: int = 5; x = 10;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("immutable"));
}

// ── Function call errors ──────────────────────────────────────────────────────

TEST(SemanticTest, CallNonFunction_Error) {
    auto r = analyze("let x: int = 5; x(10);");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("not a function"));
}

TEST(SemanticTest, ArityMismatch_TooFewArgs_Error) {
    auto r = analyze(
        "fn add(a: int, b: int) -> int { return a + b; }"
        "add(1);"
    );
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("expects 2 argument(s), got 1"));
}

TEST(SemanticTest, ArityMismatch_TooManyArgs_Error) {
    auto r = analyze(
        "fn foo(a: int) -> int { return a; }"
        "foo(1, 2, 3);"
    );
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("expects 1 argument(s), got 3"));
}

TEST(SemanticTest, ArgTypeMismatch_Error) {
    auto r = analyze(
        "fn inc(n: int) -> int { return n + 1; }"
        "inc(3.14);"
    );
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("argument 1"));
}

// ── Return statement errors ───────────────────────────────────────────────────

TEST(SemanticTest, ReturnOutsideFunction_Error) {
    auto r = analyze("return 5;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("'return' outside of a function"));
}

TEST(SemanticTest, ReturnTypeMismatch_Error) {
    auto r = analyze("fn f() -> int { return true; }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("return type mismatch"));
}

TEST(SemanticTest, BareReturnInNonVoidFunction_Error) {
    auto r = analyze("fn f() -> int { return; }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("non-void function must return a value"));
}

// ── Duplicate declarations ────────────────────────────────────────────────────

TEST(SemanticTest, DuplicateVariable_SameScope_Error) {
    auto r = analyze("let x: int = 1; let x: int = 2;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("already declared"));
}

TEST(SemanticTest, DuplicateFunction_Error) {
    auto r = analyze("fn foo() {} fn foo() {}");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("already declared"));
}

TEST(SemanticTest, DuplicateParam_InFunctionDecl_Error) {
    auto r = analyze("fn f(x: int, x: int) -> int { return x; }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("duplicate parameter name 'x'"));
}

TEST(SemanticTest, DuplicateParam_InLambda_Error) {
    auto r = analyze("let f = fn(x: int, x: int) -> int { return x; };");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("duplicate parameter name 'x'"));
}

// ── Condition type errors ─────────────────────────────────────────────────────

TEST(SemanticTest, IfCondition_NonBool_Int_Error) {
    auto r = analyze("if (42) { }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("if condition must be 'bool'"));
}

TEST(SemanticTest, WhileCondition_NonBool_Int_Error) {
    auto r = analyze("while (42) { }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("while condition must be 'bool'"));
}

TEST(SemanticTest, ForRange_NonIntStart_Error) {
    auto r = analyze("for (i in 1.0..10) { }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("for range start must be 'int'"));
}

TEST(SemanticTest, ForRange_NonIntEnd_Error) {
    auto r = analyze("for (i in 0..10.0) { }");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("for range end must be 'int'"));
}

// ── Arithmetic type errors ────────────────────────────────────────────────────

TEST(SemanticTest, ArithOnBool_Error) {
    auto r = analyze("let x = true + 1;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("requires numeric type"));
}

TEST(SemanticTest, ArithMixedTypes_IntAndFloat_Error) {
    auto r = analyze("let x = 1 + 2.0;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("operand type mismatch"));
}

TEST(SemanticTest, LogicalAnd_OnInt_Error) {
    auto r = analyze("let b: bool = 1 && true;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("requires 'bool'"));
}

TEST(SemanticTest, LogicalOr_OnInt_Error) {
    auto r = analyze("let b: bool = true || 0;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("requires 'bool'"));
}

TEST(SemanticTest, UnaryNeg_OnBool_Error) {
    auto r = analyze("let x = -true;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("unary '-' requires numeric type"));
}

TEST(SemanticTest, UnaryNot_OnInt_Error) {
    auto r = analyze("let x = !42;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("unary '!' requires 'bool'"));
}

// ── Array errors ──────────────────────────────────────────────────────────────

TEST(SemanticTest, ArrayIndex_NonInt_Error) {
    auto r = analyze(
        "let arr: int[3] = [1, 2, 3];\n"
        "let x: int = arr[true];"
    );
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("array index must be 'int'"));
}

TEST(SemanticTest, IndexNonArray_Error) {
    auto r = analyze("let x: int = 5; let y: int = x[0];");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("cannot index into non-array type"));
}

TEST(SemanticTest, ArrayElementAssign_TypeMismatch_Error) {
    auto r = analyze("var arr: int[3] = [1, 2, 3]; arr[0] = 3.14;");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("array element assignment"));
}

// ── Lambda / closure errors ───────────────────────────────────────────────────

TEST(SemanticTest, Lambda_ReturnTypeMismatch_Error) {
    auto r = analyze("let f = fn() -> int { return true; };");
    EXPECT_TRUE(r.hasError());
    EXPECT_TRUE(r.hasErrorContaining("return type mismatch"));
}

// ── Multiple errors counted correctly ────────────────────────────────────────

TEST(SemanticTest, MultipleErrors_Counted) {
    auto r = analyze("let a: int = x; let b: int = y;");
    EXPECT_TRUE(r.hasError());
    EXPECT_GE(r.errorCount(), 2);
}

TEST(SemanticTest, SingleError_ExactCount) {
    auto r = analyze("let x: int = 3.14;");
    EXPECT_EQ(r.errorCount(), 1);
}

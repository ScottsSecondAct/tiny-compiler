// test_parser.cpp — Unit tests for TinyParser + ASTBuilder
//
// Parses source strings and checks:
//   - valid programs: 0 syntax errors, correct AST node types & field values
//   - invalid programs: syntax error count > 0

#include <gtest/gtest.h>
#include <string>

#include "antlr4-runtime.h"
#include "TinyLexer.h"
#include "TinyParser.h"

#include "tiny/AST.h"
#include "tiny/ASTBuilder.h"

using namespace tiny;

// ── Helper ────────────────────────────────────────────────────────────────────

struct ParseResult {
    std::unique_ptr<Program> ast;
    int syntaxErrors = 0;
};

static ParseResult parseStr(const std::string& src) {
    antlr4::ANTLRInputStream input(src);
    TinyLexer lexer(&input);
    lexer.removeErrorListeners();
    antlr4::CommonTokenStream tokens(&lexer);
    TinyParser parser(&tokens);
    parser.removeErrorListeners();
    auto* tree = parser.program();

    ParseResult r;
    r.syntaxErrors = (int)parser.getNumberOfSyntaxErrors();
    if (r.syntaxErrors == 0) {
        ASTBuilder builder;
        r.ast = builder.buildProgram(tree);
    }
    return r;
}

// ── Empty program ─────────────────────────────────────────────────────────────

TEST(ParserTest, EmptyProgram) {
    auto r = parseStr("");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_NE(r.ast, nullptr);
    EXPECT_TRUE(r.ast->declarations.empty());
}

// ── Variable declarations ─────────────────────────────────────────────────────

TEST(ParserTest, LetDecl_IntLiteral) {
    auto r = parseStr("let x: int = 42;");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 1u);

    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    EXPECT_EQ(vd->name, "x");
    EXPECT_EQ(vd->type.kind, TypeKind::Int);
    EXPECT_FALSE(vd->isMutable);

    auto* lit = dynamic_cast<IntLit*>(vd->init.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, 42);
}

TEST(ParserTest, VarDecl_FloatLiteral) {
    auto r = parseStr("var y: float = 3.14;");
    ASSERT_EQ(r.syntaxErrors, 0);

    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    EXPECT_EQ(vd->name, "y");
    EXPECT_EQ(vd->type.kind, TypeKind::Float);
    EXPECT_TRUE(vd->isMutable);

    auto* lit = dynamic_cast<FloatLit*>(vd->init.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_DOUBLE_EQ(lit->value, 3.14);
}

TEST(ParserTest, LetDecl_BoolTrue) {
    auto r = parseStr("let b: bool = true;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    auto* lit = dynamic_cast<BoolLit*>(vd->init.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_TRUE(lit->value);
}

TEST(ParserTest, LetDecl_BoolFalse) {
    auto r = parseStr("let b: bool = false;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* lit = dynamic_cast<BoolLit*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_FALSE(lit->value);
}

TEST(ParserTest, LetDecl_StringLiteral) {
    auto r = parseStr("let s: string = \"hello\";");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* lit = dynamic_cast<StringLit*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, "hello");
}

TEST(ParserTest, VarDecl_TypeInferred) {
    auto r = parseStr("var x = 5;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    EXPECT_EQ(vd->type.kind, TypeKind::Unknown);  // resolved by semantic analysis
    EXPECT_TRUE(vd->isMutable);
}

// ── Expressions ───────────────────────────────────────────────────────────────

TEST(ParserTest, BinaryExpr_Add) {
    auto r = parseStr("let z: int = 1 + 2;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Add);
}

TEST(ParserTest, BinaryExpr_Sub) {
    auto r = parseStr("let z: int = 10 - 3;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Sub);
}

TEST(ParserTest, BinaryExpr_Mul) {
    auto r = parseStr("let z: int = 4 * 5;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Mul);
}

TEST(ParserTest, BinaryExpr_Div) {
    auto r = parseStr("let z: int = 10 / 2;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Div);
}

TEST(ParserTest, BinaryExpr_Mod) {
    auto r = parseStr("let z: int = 10 % 3;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Mod);
}

TEST(ParserTest, BinaryExpr_Lt) {
    auto r = parseStr("let b: bool = 1 < 2;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Lt);
}

TEST(ParserTest, BinaryExpr_Eq) {
    auto r = parseStr("let b: bool = 1 == 1;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Eq);
}

TEST(ParserTest, BinaryExpr_Neq) {
    auto r = parseStr("let b: bool = 1 != 2;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Neq);
}

TEST(ParserTest, BinaryExpr_LogicalAnd) {
    auto r = parseStr("let b: bool = true && false;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::And);
}

TEST(ParserTest, BinaryExpr_LogicalOr) {
    auto r = parseStr("let b: bool = true || false;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Or);
}

TEST(ParserTest, UnaryExpr_Negation) {
    auto r = parseStr("let z: int = -5;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* un = dynamic_cast<UnaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(un, nullptr);
    EXPECT_EQ(un->op, UnOp::Neg);
}

TEST(ParserTest, UnaryExpr_Not) {
    auto r = parseStr("let b: bool = !true;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* un = dynamic_cast<UnaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(un, nullptr);
    EXPECT_EQ(un->op, UnOp::Not);
}

TEST(ParserTest, ParenthesizedExpr_Transparent) {
    auto r = parseStr("let z: int = (1 + 2);");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* bin = dynamic_cast<BinaryExpr*>(
        dynamic_cast<VarDecl*>(r.ast->declarations[0].get())->init.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, BinOp::Add);
}

TEST(ParserTest, Identifier_Expr) {
    auto r = parseStr("let a: int = 1; let b: int = a;");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 2u);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[1].get());
    ASSERT_NE(vd, nullptr);
    auto* id = dynamic_cast<Identifier*>(vd->init.get());
    ASSERT_NE(id, nullptr);
    EXPECT_EQ(id->name, "a");
}

// ── Function declarations ─────────────────────────────────────────────────────

TEST(ParserTest, FunctionDecl_NoParams_VoidReturn) {
    auto r = parseStr("fn greet() {}");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 1u);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "greet");
    EXPECT_TRUE(fn->params.empty());
    EXPECT_EQ(fn->returnType.kind, TypeKind::Void);
}

TEST(ParserTest, FunctionDecl_TwoParams_IntReturn) {
    auto r = parseStr("fn add(a: int, b: int) -> int { return a + b; }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "add");
    ASSERT_EQ(fn->params.size(), 2u);
    EXPECT_EQ(fn->params[0].name, "a");
    EXPECT_EQ(fn->params[0].type.kind, TypeKind::Int);
    EXPECT_EQ(fn->params[1].name, "b");
    EXPECT_EQ(fn->params[1].type.kind, TypeKind::Int);
    EXPECT_EQ(fn->returnType.kind, TypeKind::Int);
}

TEST(ParserTest, FunctionDecl_FloatReturn) {
    auto r = parseStr("fn pi() -> float { return 3.14; }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->returnType.kind, TypeKind::Float);
}

TEST(ParserTest, FunctionDecl_BoolReturn) {
    auto r = parseStr("fn yes() -> bool { return true; }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->returnType.kind, TypeKind::Bool);
}

TEST(ParserTest, FunctionDecl_FunctionTypeParam) {
    auto r = parseStr("fn apply(f: fn(int) -> int, x: int) -> int { return f(x); }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    ASSERT_EQ(fn->params.size(), 2u);
    EXPECT_EQ(fn->params[0].type.kind, TypeKind::Function);
}

// ── Control flow ──────────────────────────────────────────────────────────────

TEST(ParserTest, IfStmt_NoElse) {
    auto r = parseStr("if (true) { }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ifs = dynamic_cast<IfStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ifs, nullptr);
    EXPECT_EQ(ifs->elseBlock, nullptr);
}

TEST(ParserTest, IfElseStmt) {
    auto r = parseStr("if (true) { } else { }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ifs = dynamic_cast<IfStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ifs, nullptr);
    EXPECT_NE(ifs->elseBlock, nullptr);
}

TEST(ParserTest, IfElseIfChain) {
    auto r = parseStr("if (true) { } else if (false) { } else { }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ifs = dynamic_cast<IfStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ifs, nullptr);
    EXPECT_NE(ifs->elseBlock, nullptr);
    // else block should itself be an IfStmt
    auto* elseIf = dynamic_cast<IfStmt*>(ifs->elseBlock.get());
    ASSERT_NE(elseIf, nullptr);
}

TEST(ParserTest, WhileStmt) {
    auto r = parseStr("while (true) { }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ws = dynamic_cast<WhileStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ws, nullptr);
}

TEST(ParserTest, ForStmt_RangeLoop) {
    auto r = parseStr("for (i in 0..10) { }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fs = dynamic_cast<ForStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(fs, nullptr);
    EXPECT_EQ(fs->varName, "i");
}

// ── Print statement ───────────────────────────────────────────────────────────

TEST(ParserTest, PrintStmt_NoArgs) {
    auto r = parseStr("print();");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ps = dynamic_cast<PrintStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ps, nullptr);
    EXPECT_TRUE(ps->args.empty());
}

TEST(ParserTest, PrintStmt_OneArg) {
    auto r = parseStr("print(42);");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ps = dynamic_cast<PrintStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ps, nullptr);
    ASSERT_EQ(ps->args.size(), 1u);
}

TEST(ParserTest, PrintStmt_MultipleArgs) {
    auto r = parseStr("print(1, 2, 3);");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* ps = dynamic_cast<PrintStmt*>(r.ast->declarations[0].get());
    ASSERT_NE(ps, nullptr);
    EXPECT_EQ(ps->args.size(), 3u);
}

// ── Arrays ────────────────────────────────────────────────────────────────────

TEST(ParserTest, ArrayType_InVarDecl) {
    auto r = parseStr("let arr: int[5] = [1, 2, 3, 4, 5];");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    EXPECT_EQ(vd->type.kind, TypeKind::Array);
    EXPECT_EQ(vd->type.arraySize, 5);
    ASSERT_NE(vd->type.elementType, nullptr);
    EXPECT_EQ(vd->type.elementType->kind, TypeKind::Int);
}

TEST(ParserTest, ArrayLiteral_Elements) {
    auto r = parseStr("let arr: int[3] = [10, 20, 30];");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    auto* arr = dynamic_cast<ArrayLiteral*>(vd->init.get());
    ASSERT_NE(arr, nullptr);
    EXPECT_EQ(arr->elements.size(), 3u);
}

TEST(ParserTest, ArrayIndex_Expr) {
    auto r = parseStr("let arr: int[3] = [1, 2, 3]; let x: int = arr[0];");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd2 = dynamic_cast<VarDecl*>(r.ast->declarations[1].get());
    ASSERT_NE(vd2, nullptr);
    auto* idx = dynamic_cast<IndexExpr*>(vd2->init.get());
    ASSERT_NE(idx, nullptr);
}

// ── Lambda expressions ────────────────────────────────────────────────────────

TEST(ParserTest, LambdaExpr_NoParams) {
    auto r = parseStr("let f = fn() { };");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    auto* lam = dynamic_cast<LambdaExpr*>(vd->init.get());
    ASSERT_NE(lam, nullptr);
    EXPECT_TRUE(lam->params.empty());
    EXPECT_EQ(lam->returnType.kind, TypeKind::Void);
}

TEST(ParserTest, LambdaExpr_WithParams_IntReturn) {
    auto r = parseStr("let add = fn(a: int, b: int) -> int { return a + b; };");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    auto* lam = dynamic_cast<LambdaExpr*>(vd->init.get());
    ASSERT_NE(lam, nullptr);
    ASSERT_EQ(lam->params.size(), 2u);
    EXPECT_EQ(lam->params[0].name, "a");
    EXPECT_EQ(lam->params[1].name, "b");
    EXPECT_EQ(lam->returnType.kind, TypeKind::Int);
}

// ── Function calls ────────────────────────────────────────────────────────────

TEST(ParserTest, CallExpr_NoArgs) {
    auto r = parseStr("fn foo() {} foo();");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 2u);
    auto* es = dynamic_cast<ExprStmt*>(r.ast->declarations[1].get());
    ASSERT_NE(es, nullptr);
    auto* call = dynamic_cast<CallExpr*>(es->expr.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "foo");
    EXPECT_TRUE(call->args.empty());
}

TEST(ParserTest, CallExpr_WithArgs) {
    auto r = parseStr("fn add(a: int, b: int) -> int { return a + b; } add(1, 2);");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* es = dynamic_cast<ExprStmt*>(r.ast->declarations[1].get());
    auto* call = dynamic_cast<CallExpr*>(es->expr.get());
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "add");
    EXPECT_EQ(call->args.size(), 2u);
}

// ── Import declaration ────────────────────────────────────────────────────────

TEST(ParserTest, ImportDecl) {
    auto r = parseStr("import \"math.tiny\";");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 1u);
    auto* imp = dynamic_cast<ImportDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(imp, nullptr);
    EXPECT_EQ(imp->path, "math.tiny");
}

// ── Return statement ──────────────────────────────────────────────────────────

TEST(ParserTest, ReturnStmt_WithValue) {
    auto r = parseStr("fn f() -> int { return 42; }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    ASSERT_EQ(fn->body->statements.size(), 1u);
    auto* ret = dynamic_cast<ReturnStmt*>(fn->body->statements[0].get());
    ASSERT_NE(ret, nullptr);
    EXPECT_NE(ret->value, nullptr);
}

TEST(ParserTest, ReturnStmt_Bare) {
    auto r = parseStr("fn f() { return; }");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* fn = dynamic_cast<FunctionDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(fn, nullptr);
    auto* ret = dynamic_cast<ReturnStmt*>(fn->body->statements[0].get());
    ASSERT_NE(ret, nullptr);
    EXPECT_EQ(ret->value, nullptr);
}

// ── Assignment ────────────────────────────────────────────────────────────────

TEST(ParserTest, Assignment_Simple) {
    auto r = parseStr("var x: int = 0; x = 10;");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 2u);
    auto* assign = dynamic_cast<Assignment*>(r.ast->declarations[1].get());
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->target, "x");
    EXPECT_EQ(assign->index, nullptr);
}

TEST(ParserTest, Assignment_ArrayElement) {
    auto r = parseStr("var arr: int[3] = [1, 2, 3]; arr[0] = 99;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* assign = dynamic_cast<Assignment*>(r.ast->declarations[1].get());
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->target, "arr");
    EXPECT_NE(assign->index, nullptr);
}

// ── Source location tracking ──────────────────────────────────────────────────

TEST(ParserTest, SourceLoc_Line1) {
    auto r = parseStr("let x: int = 1;");
    ASSERT_EQ(r.syntaxErrors, 0);
    auto* vd = dynamic_cast<VarDecl*>(r.ast->declarations[0].get());
    ASSERT_NE(vd, nullptr);
    EXPECT_EQ(vd->loc.line, 1);
}

TEST(ParserTest, SourceLoc_SecondLine) {
    auto r = parseStr("let x: int = 1;\nlet y: int = 2;");
    ASSERT_EQ(r.syntaxErrors, 0);
    ASSERT_EQ(r.ast->declarations.size(), 2u);
    auto* vd2 = dynamic_cast<VarDecl*>(r.ast->declarations[1].get());
    ASSERT_NE(vd2, nullptr);
    EXPECT_EQ(vd2->loc.line, 2);
}

// ── Syntax error detection ────────────────────────────────────────────────────

TEST(ParserTest, SyntaxError_MissingSemicolon) {
    auto r = parseStr("let x: int = 5");
    EXPECT_GT(r.syntaxErrors, 0);
}

TEST(ParserTest, SyntaxError_MissingRBrace) {
    auto r = parseStr("fn f() {");
    EXPECT_GT(r.syntaxErrors, 0);
}

TEST(ParserTest, SyntaxError_MissingExpression) {
    auto r = parseStr("let x: int = ;");
    EXPECT_GT(r.syntaxErrors, 0);
}

TEST(ParserTest, SyntaxError_MissingColonInParam) {
    auto r = parseStr("fn f(x int) {}");
    EXPECT_GT(r.syntaxErrors, 0);
}

TEST(ParserTest, SyntaxError_MissingArrow) {
    auto r = parseStr("fn f() int { return 1; }");
    EXPECT_GT(r.syntaxErrors, 0);
}

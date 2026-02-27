// test_ast.cpp — Unit tests for TypeSpec, Diagnostics, SymbolTable, and ASTBox

#include <gtest/gtest.h>
#include <sstream>

#include "tiny/AST.h"
#include "tiny/Diagnostics.h"
#include "tiny/SymbolTable.h"

using namespace tiny;

// ── TypeSpec ─────────────────────────────────────────────────────────────────

TEST(TypeSpecTest, ScalarKinds) {
    EXPECT_EQ(TypeSpec::makeInt().kind,     TypeKind::Int);
    EXPECT_EQ(TypeSpec::makeFloat().kind,   TypeKind::Float);
    EXPECT_EQ(TypeSpec::makeBool().kind,    TypeKind::Bool);
    EXPECT_EQ(TypeSpec::makeString().kind,  TypeKind::String);
    EXPECT_EQ(TypeSpec::makeVoid().kind,    TypeKind::Void);
    EXPECT_EQ(TypeSpec::makeUnknown().kind, TypeKind::Unknown);
}

TEST(TypeSpecTest, ScalarEquality) {
    EXPECT_EQ(TypeSpec::makeInt(),    TypeSpec::makeInt());
    EXPECT_EQ(TypeSpec::makeFloat(),  TypeSpec::makeFloat());
    EXPECT_EQ(TypeSpec::makeBool(),   TypeSpec::makeBool());
    EXPECT_EQ(TypeSpec::makeString(), TypeSpec::makeString());
    EXPECT_EQ(TypeSpec::makeVoid(),   TypeSpec::makeVoid());
}

TEST(TypeSpecTest, ScalarInequality) {
    EXPECT_NE(TypeSpec::makeInt(),   TypeSpec::makeFloat());
    EXPECT_NE(TypeSpec::makeInt(),   TypeSpec::makeBool());
    EXPECT_NE(TypeSpec::makeFloat(), TypeSpec::makeString());
    EXPECT_NE(TypeSpec::makeVoid(),  TypeSpec::makeInt());
}

TEST(TypeSpecTest, ScalarToString) {
    EXPECT_EQ(TypeSpec::makeInt().toString(),     "int");
    EXPECT_EQ(TypeSpec::makeFloat().toString(),   "float");
    EXPECT_EQ(TypeSpec::makeBool().toString(),    "bool");
    EXPECT_EQ(TypeSpec::makeString().toString(),  "string");
    EXPECT_EQ(TypeSpec::makeVoid().toString(),    "void");
    EXPECT_EQ(TypeSpec::makeUnknown().toString(), "unknown");
}

TEST(TypeSpecTest, ArrayConstruction) {
    auto arr = TypeSpec::makeArray(TypeSpec::makeInt(), 5);
    EXPECT_EQ(arr.kind, TypeKind::Array);
    EXPECT_EQ(arr.arraySize, 5);
    ASSERT_NE(arr.elementType, nullptr);
    EXPECT_EQ(arr.elementType->kind, TypeKind::Int);
}

TEST(TypeSpecTest, ArrayToString) {
    EXPECT_EQ(TypeSpec::makeArray(TypeSpec::makeInt(), 3).toString(), "int[3]");
    EXPECT_EQ(TypeSpec::makeArray(TypeSpec::makeFloat(), 10).toString(), "float[10]");
}

TEST(TypeSpecTest, ArrayEquality) {
    auto a1 = TypeSpec::makeArray(TypeSpec::makeInt(), 3);
    auto a2 = TypeSpec::makeArray(TypeSpec::makeInt(), 3);
    auto a3 = TypeSpec::makeArray(TypeSpec::makeInt(), 5);
    auto a4 = TypeSpec::makeArray(TypeSpec::makeFloat(), 3);

    EXPECT_EQ(a1, a2);
    EXPECT_NE(a1, a3);  // different size
    EXPECT_NE(a1, a4);  // different element type
    EXPECT_NE(a1, TypeSpec::makeInt());  // array vs scalar
}

TEST(TypeSpecTest, FunctionConstruction) {
    auto f = TypeSpec::makeFunction(
        {TypeSpec::makeInt(), TypeSpec::makeFloat()},
        TypeSpec::makeBool());
    EXPECT_EQ(f.kind, TypeKind::Function);
    ASSERT_EQ(f.paramTypes.size(), 2u);
    EXPECT_EQ(f.paramTypes[0]->kind, TypeKind::Int);
    EXPECT_EQ(f.paramTypes[1]->kind, TypeKind::Float);
    ASSERT_NE(f.returnType, nullptr);
    EXPECT_EQ(f.returnType->kind, TypeKind::Bool);
}

TEST(TypeSpecTest, FunctionToString) {
    auto f = TypeSpec::makeFunction(
        {TypeSpec::makeInt(), TypeSpec::makeFloat()},
        TypeSpec::makeBool());
    EXPECT_EQ(f.toString(), "fn(int, float) -> bool");
}

TEST(TypeSpecTest, FunctionToString_NoParams) {
    auto f = TypeSpec::makeFunction({}, TypeSpec::makeInt());
    EXPECT_EQ(f.toString(), "fn() -> int");
}

TEST(TypeSpecTest, FunctionToString_VoidReturn) {
    auto f = TypeSpec::makeFunction({TypeSpec::makeInt()}, TypeSpec::makeVoid());
    // Void return is omitted from toString
    EXPECT_EQ(f.toString(), "fn(int)");
}

TEST(TypeSpecTest, FunctionEquality) {
    auto f1 = TypeSpec::makeFunction({TypeSpec::makeInt()}, TypeSpec::makeInt());
    auto f2 = TypeSpec::makeFunction({TypeSpec::makeInt()}, TypeSpec::makeInt());
    auto f3 = TypeSpec::makeFunction({TypeSpec::makeFloat()}, TypeSpec::makeInt());
    auto f4 = TypeSpec::makeFunction({TypeSpec::makeInt()}, TypeSpec::makeFloat());

    EXPECT_EQ(f1, f2);
    EXPECT_NE(f1, f3);  // different param type
    EXPECT_NE(f1, f4);  // different return type
}

TEST(TypeSpecTest, FunctionEquality_DifferentParamCount) {
    auto f1 = TypeSpec::makeFunction({TypeSpec::makeInt()}, TypeSpec::makeVoid());
    auto f2 = TypeSpec::makeFunction(
        {TypeSpec::makeInt(), TypeSpec::makeInt()}, TypeSpec::makeVoid());
    EXPECT_NE(f1, f2);
}

TEST(TypeSpecTest, FunctionEquality_NoParams) {
    auto f1 = TypeSpec::makeFunction({}, TypeSpec::makeVoid());
    auto f2 = TypeSpec::makeFunction({}, TypeSpec::makeVoid());
    EXPECT_EQ(f1, f2);
}

TEST(TypeSpecTest, NestedArrayToString) {
    auto inner = TypeSpec::makeArray(TypeSpec::makeInt(), 2);
    auto outer = TypeSpec::makeArray(inner, 3);
    EXPECT_EQ(outer.toString(), "int[2][3]");
}

// ── ASTBox / boxNode / unboxNode ─────────────────────────────────────────────

TEST(ASTBoxTest, BoxUnboxPreservesValue) {
    auto node = std::make_unique<IntLit>(42);
    IntLit* rawPtr = node.get();

    auto boxed = boxNode(std::move(node));
    EXPECT_TRUE(boxed.has_value());

    auto unboxed = unboxNode(boxed);
    ASSERT_NE(unboxed.get(), nullptr);
    EXPECT_EQ(unboxed.get(), rawPtr);
    EXPECT_EQ(static_cast<IntLit*>(unboxed.get())->value, 42);
}

TEST(ASTBoxTest, BoxUnboxRvaluePreservesValue) {
    auto node = std::make_unique<IntLit>(99);

    auto boxed = boxNode(std::move(node));
    auto unboxed = unboxNode(std::move(boxed));

    ASSERT_NE(unboxed.get(), nullptr);
    EXPECT_EQ(static_cast<IntLit*>(unboxed.get())->value, 99);
}

TEST(ASTBoxTest, NodeTypePreserved) {
    auto node = std::make_unique<FloatLit>(3.14);
    auto boxed = boxNode(std::move(node));
    auto unboxed = unboxNode(boxed);

    auto* floatNode = dynamic_cast<FloatLit*>(unboxed.get());
    ASSERT_NE(floatNode, nullptr);
    EXPECT_DOUBLE_EQ(floatNode->value, 3.14);
}

// ── Diagnostics ──────────────────────────────────────────────────────────────

TEST(DiagnosticsTest, InitiallyEmpty) {
    Diagnostics diags;
    EXPECT_FALSE(diags.hasErrors());
    EXPECT_EQ(diags.errorCount(), 0);
    EXPECT_EQ(diags.warningCount(), 0);
    EXPECT_TRUE(diags.all().empty());
}

TEST(DiagnosticsTest, RecordError) {
    Diagnostics diags;
    diags.error({1, 0}, "something went wrong");
    EXPECT_TRUE(diags.hasErrors());
    EXPECT_EQ(diags.errorCount(), 1);
    EXPECT_EQ(diags.warningCount(), 0);
    ASSERT_EQ(diags.all().size(), 1u);
    EXPECT_EQ(diags.all()[0].message, "something went wrong");
    EXPECT_EQ(diags.all()[0].level, DiagLevel::Error);
}

TEST(DiagnosticsTest, RecordWarning) {
    Diagnostics diags;
    diags.warning({2, 5}, "mild concern");
    EXPECT_FALSE(diags.hasErrors());
    EXPECT_EQ(diags.errorCount(), 0);
    EXPECT_EQ(diags.warningCount(), 1);
    ASSERT_EQ(diags.all().size(), 1u);
    EXPECT_EQ(diags.all()[0].level, DiagLevel::Warning);
}

TEST(DiagnosticsTest, MultipleErrors) {
    Diagnostics diags;
    diags.error({1, 0}, "first");
    diags.error({2, 0}, "second");
    diags.error({3, 0}, "third");
    EXPECT_EQ(diags.errorCount(), 3);
    EXPECT_EQ(diags.all().size(), 3u);
}

TEST(DiagnosticsTest, MixedErrorsAndWarnings) {
    Diagnostics diags;
    diags.error({1, 0}, "bad");
    diags.warning({1, 0}, "meh");
    diags.error({2, 0}, "worse");
    EXPECT_EQ(diags.errorCount(), 2);
    EXPECT_EQ(diags.warningCount(), 1);
    EXPECT_EQ(diags.all().size(), 3u);
}

TEST(DiagnosticsTest, DumpFormat_SingleError) {
    Diagnostics diags;
    diags.error({5, 0}, "undefined variable 'x'");

    std::ostringstream oss;
    diags.dump(oss);
    std::string out = oss.str();

    EXPECT_NE(out.find("Error"), std::string::npos);
    EXPECT_NE(out.find("undefined variable 'x'"), std::string::npos);
    EXPECT_NE(out.find("line 5"), std::string::npos);
    EXPECT_NE(out.find("1 error(s)"), std::string::npos);
}

TEST(DiagnosticsTest, DumpFormat_Warning) {
    Diagnostics diags;
    diags.warning({3, 0}, "unused variable");

    std::ostringstream oss;
    diags.dump(oss);
    std::string out = oss.str();

    EXPECT_NE(out.find("Warning"), std::string::npos);
    EXPECT_NE(out.find("unused variable"), std::string::npos);
    // Warnings don't trigger error count summary
    EXPECT_EQ(out.find("error(s)"), std::string::npos);
}

TEST(DiagnosticsTest, SourceLocPreserved) {
    Diagnostics diags;
    diags.error({7, 12}, "test");
    EXPECT_EQ(diags.all()[0].loc.line, 7);
    EXPECT_EQ(diags.all()[0].loc.col, 12);
}

// ── SymbolTable ───────────────────────────────────────────────────────────────

TEST(SymbolTableTest, DepthInitial) {
    SymbolTable st;
    EXPECT_EQ(st.depth(), -1);  // no scopes pushed
}

TEST(SymbolTableTest, PushIncreasesDepth) {
    SymbolTable st;
    st.pushScope();
    EXPECT_EQ(st.depth(), 0);
    st.pushScope();
    EXPECT_EQ(st.depth(), 1);
}

TEST(SymbolTableTest, PopDecreasesDepth) {
    SymbolTable st;
    st.pushScope();
    st.pushScope();
    st.popScope();
    EXPECT_EQ(st.depth(), 0);
}

TEST(SymbolTableTest, PopEmptyScopeThrows) {
    SymbolTable st;
    EXPECT_THROW(st.popScope(), std::runtime_error);
}

TEST(SymbolTableTest, DeclareWithoutScopeThrows) {
    SymbolTable st;
    Symbol sym;
    sym.name = "x";
    sym.type = TypeSpec::makeInt();
    EXPECT_THROW(st.declare(sym), std::runtime_error);
}

TEST(SymbolTableTest, DeclareAndLookup) {
    SymbolTable st;
    st.pushScope();
    Symbol sym;
    sym.name = "x";
    sym.type = TypeSpec::makeInt();
    sym.isMutable = true;

    EXPECT_TRUE(st.declare(sym));
    Symbol* found = st.lookup("x");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->name, "x");
    EXPECT_EQ(found->type.kind, TypeKind::Int);
    EXPECT_TRUE(found->isMutable);
}

TEST(SymbolTableTest, LookupNotFound) {
    SymbolTable st;
    st.pushScope();
    EXPECT_EQ(st.lookup("unknown"), nullptr);
}

TEST(SymbolTableTest, LookupNotFoundInEmptyTable) {
    SymbolTable st;
    EXPECT_EQ(st.lookup("x"), nullptr);
}

TEST(SymbolTableTest, DuplicateDeclReturnsFalse) {
    SymbolTable st;
    st.pushScope();
    Symbol sym;
    sym.name = "x";
    sym.type = TypeSpec::makeInt();

    EXPECT_TRUE(st.declare(sym));
    EXPECT_FALSE(st.declare(sym));  // second declaration fails
}

TEST(SymbolTableTest, InnerScopeCanShadowOuter) {
    SymbolTable st;
    st.pushScope();
    Symbol outer;
    outer.name = "x";
    outer.type = TypeSpec::makeInt();
    st.declare(outer);

    st.pushScope();
    Symbol inner;
    inner.name = "x";
    inner.type = TypeSpec::makeFloat();  // different type
    EXPECT_TRUE(st.declare(inner));  // shadowing is allowed

    // Inner scope lookup finds the float
    Symbol* found = st.lookup("x");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->type.kind, TypeKind::Float);

    st.popScope();

    // After pop, outer scope int is found
    Symbol* outerFound = st.lookup("x");
    ASSERT_NE(outerFound, nullptr);
    EXPECT_EQ(outerFound->type.kind, TypeKind::Int);
}

TEST(SymbolTableTest, LookupCurrent_OnlyInnerScope) {
    SymbolTable st;
    st.pushScope();
    Symbol sym;
    sym.name = "x";
    sym.type = TypeSpec::makeInt();
    st.declare(sym);

    st.pushScope();
    // x is in outer scope — lookupCurrent should not find it
    EXPECT_EQ(st.lookupCurrent("x"), nullptr);
    // But lookup (cross-scope) should find it
    EXPECT_NE(st.lookup("x"), nullptr);
}

TEST(SymbolTableTest, LookupCurrent_EmptyTable) {
    SymbolTable st;
    EXPECT_EQ(st.lookupCurrent("x"), nullptr);
}

TEST(SymbolTableTest, SymbolRemovedAfterPopScope) {
    SymbolTable st;
    st.pushScope();
    st.pushScope();
    Symbol sym;
    sym.name = "local";
    sym.type = TypeSpec::makeInt();
    st.declare(sym);
    EXPECT_NE(st.lookup("local"), nullptr);

    st.popScope();
    EXPECT_EQ(st.lookup("local"), nullptr);  // gone after pop
}

TEST(SymbolTableTest, MultipleSymbolsInSameScope) {
    SymbolTable st;
    st.pushScope();
    for (const char* name : {"a", "b", "c"}) {
        Symbol sym;
        sym.name = name;
        sym.type = TypeSpec::makeInt();
        EXPECT_TRUE(st.declare(sym));
    }
    EXPECT_NE(st.lookup("a"), nullptr);
    EXPECT_NE(st.lookup("b"), nullptr);
    EXPECT_NE(st.lookup("c"), nullptr);
}

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <any>
#include <functional>
#include <variant>

namespace tiny {

// ── Forward declarations ────────────────────────────────────────────────────

struct Program;

// Statements
struct VarDecl;
struct Assignment;
struct ReturnStmt;
struct PrintStmt;
struct IfStmt;
struct WhileStmt;
struct ForStmt;
struct ExprStmt;
struct Block;
struct FunctionDecl;

// Expressions
struct IntLit;
struct FloatLit;
struct BoolLit;
struct StringLit;
struct Identifier;
struct ArrayLiteral;
struct BinaryExpr;
struct UnaryExpr;
struct CallExpr;
struct IndexExpr;

// ── Source location (for error messages) ─────────────────────────────────────

struct SourceLoc {
    int line = 0;
    int col  = 0;
};

// ── Type representation ─────────────────────────────────────────────────────

enum class TypeKind {
    Int,
    Float,
    Bool,
    String,
    Array,
    Void,     // for functions with no return type
    Unknown,  // for type inference (before resolution)
};

struct TypeSpec {
    TypeKind kind = TypeKind::Unknown;
    int arraySize = 0;                          // only used when kind == Array
    std::shared_ptr<TypeSpec> elementType;       // only used when kind == Array

    static TypeSpec makeInt()    { return {TypeKind::Int}; }
    static TypeSpec makeFloat()  { return {TypeKind::Float}; }
    static TypeSpec makeBool()   { return {TypeKind::Bool}; }
    static TypeSpec makeString() { return {TypeKind::String}; }
    static TypeSpec makeVoid()   { return {TypeKind::Void}; }
    static TypeSpec makeUnknown(){ return {TypeKind::Unknown}; }

    static TypeSpec makeArray(TypeSpec elem, int size) {
        auto ts = TypeSpec{TypeKind::Array, size};
        ts.elementType = std::make_shared<TypeSpec>(std::move(elem));
        return ts;
    }

    bool operator==(const TypeSpec& other) const {
        if (kind != other.kind) return false;
        if (kind == TypeKind::Array) {
            return arraySize == other.arraySize
                && elementType && other.elementType
                && *elementType == *other.elementType;
        }
        return true;
    }

    bool operator!=(const TypeSpec& other) const { return !(*this == other); }

    std::string toString() const {
        switch (kind) {
            case TypeKind::Int:     return "int";
            case TypeKind::Float:   return "float";
            case TypeKind::Bool:    return "bool";
            case TypeKind::String:  return "string";
            case TypeKind::Void:    return "void";
            case TypeKind::Unknown: return "unknown";
            case TypeKind::Array:
                return elementType->toString() + "[" + std::to_string(arraySize) + "]";
        }
        return "?";
    }
};

// ── AST Visitor (for traversal / code generation) ───────────────────────────
//
// Each phase (printer, semantic analyzer, codegen) subclasses this.
// Returns std::any so each visitor can return whatever it needs.

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Expressions
    virtual std::any visit(IntLit& node)       = 0;
    virtual std::any visit(FloatLit& node)     = 0;
    virtual std::any visit(BoolLit& node)      = 0;
    virtual std::any visit(StringLit& node)    = 0;
    virtual std::any visit(Identifier& node)   = 0;
    virtual std::any visit(ArrayLiteral& node) = 0;
    virtual std::any visit(BinaryExpr& node)   = 0;
    virtual std::any visit(UnaryExpr& node)    = 0;
    virtual std::any visit(CallExpr& node)     = 0;
    virtual std::any visit(IndexExpr& node)    = 0;

    // Statements
    virtual std::any visit(VarDecl& node)      = 0;
    virtual std::any visit(Assignment& node)   = 0;
    virtual std::any visit(ReturnStmt& node)   = 0;
    virtual std::any visit(PrintStmt& node)    = 0;
    virtual std::any visit(IfStmt& node)       = 0;
    virtual std::any visit(WhileStmt& node)    = 0;
    virtual std::any visit(ForStmt& node)      = 0;
    virtual std::any visit(ExprStmt& node)     = 0;
    virtual std::any visit(Block& node)        = 0;
    virtual std::any visit(FunctionDecl& node) = 0;

    // Top-level
    virtual std::any visit(Program& node)      = 0;
};

// ── Base AST Node ───────────────────────────────────────────────────────────

struct ASTNode {
    SourceLoc loc;
    virtual ~ASTNode() = default;
    virtual std::any accept(ASTVisitor& visitor) = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

// ── Helper: typed node with accept() wired up ───────────────────────────────

template <typename Derived>
struct ASTNodeBase : ASTNode {
    std::any accept(ASTVisitor& visitor) override {
        return visitor.visit(static_cast<Derived&>(*this));
    }
};

// ── Expression Nodes ────────────────────────────────────────────────────────

// ── Move-friendly wrapper for unique_ptr inside std::any ────────────────────
// std::any requires CopyConstructible, but unique_ptr is move-only.
// This wrapper stores the unique_ptr inside a shared_ptr-to-wrapper so that
// std::any can copy the shared_ptr, while the underlying node stays unique.

struct ASTBox {
    struct Holder {
        std::unique_ptr<ASTNode> ptr;
        explicit Holder(std::unique_ptr<ASTNode> p) : ptr(std::move(p)) {}
    };
    std::shared_ptr<Holder> holder;

    ASTBox() = default;
    explicit ASTBox(std::unique_ptr<ASTNode> p)
        : holder(std::make_shared<Holder>(std::move(p))) {}

    std::unique_ptr<ASTNode> take() {
        return std::move(holder->ptr);
    }
};

inline std::any boxNode(std::unique_ptr<ASTNode> node) {
    return std::make_any<ASTBox>(ASTBox(std::move(node)));
}

inline std::unique_ptr<ASTNode> unboxNode(std::any& a) {
    auto& box = std::any_cast<ASTBox&>(a);
    return box.take();
}

inline std::unique_ptr<ASTNode> unboxNode(std::any&& a) {
    auto box = std::any_cast<ASTBox>(std::move(a));
    return box.take();
}

// We alias expression pointers for clarity
using ExprPtr = std::unique_ptr<ASTNode>;

struct IntLit : ASTNodeBase<IntLit> {
    int64_t value;
    explicit IntLit(int64_t v) : value(v) {}
};

struct FloatLit : ASTNodeBase<FloatLit> {
    double value;
    explicit FloatLit(double v) : value(v) {}
};

struct BoolLit : ASTNodeBase<BoolLit> {
    bool value;
    explicit BoolLit(bool v) : value(v) {}
};

struct StringLit : ASTNodeBase<StringLit> {
    std::string value;
    explicit StringLit(std::string v) : value(std::move(v)) {}
};

struct Identifier : ASTNodeBase<Identifier> {
    std::string name;
    explicit Identifier(std::string n) : name(std::move(n)) {}
};

struct ArrayLiteral : ASTNodeBase<ArrayLiteral> {
    std::vector<ExprPtr> elements;
};

enum class BinOp {
    Add, Sub, Mul, Div, Mod,          // arithmetic
    Eq, Neq, Lt, Lte, Gt, Gte,       // comparison
    And, Or,                           // logical
};

std::string binOpToString(BinOp op);

struct BinaryExpr : ASTNodeBase<BinaryExpr> {
    BinOp op;
    ExprPtr left;
    ExprPtr right;
};

enum class UnOp {
    Neg,  // -
    Not,  // !
};

struct UnaryExpr : ASTNodeBase<UnaryExpr> {
    UnOp op;
    ExprPtr operand;
};

struct CallExpr : ASTNodeBase<CallExpr> {
    std::string callee;
    std::vector<ExprPtr> args;
};

struct IndexExpr : ASTNodeBase<IndexExpr> {
    ExprPtr array;
    ExprPtr index;
};

// ── Statement Nodes ─────────────────────────────────────────────────────────

// We alias statement pointers for clarity
using StmtPtr = std::unique_ptr<ASTNode>;

struct VarDecl : ASTNodeBase<VarDecl> {
    std::string name;
    TypeSpec type;                     // may be Unknown if using inference
    ExprPtr init;
    bool isMutable;                    // var = true, let = false
};

struct Assignment : ASTNodeBase<Assignment> {
    std::string target;
    ExprPtr index;                     // nullptr for simple assignment, set for arr[i] = ...
    ExprPtr value;
};

struct ReturnStmt : ASTNodeBase<ReturnStmt> {
    ExprPtr value;                     // nullptr for bare `return;`
};

struct PrintStmt : ASTNodeBase<PrintStmt> {
    std::vector<ExprPtr> args;
};

struct IfStmt : ASTNodeBase<IfStmt> {
    ExprPtr condition;
    StmtPtr thenBlock;
    StmtPtr elseBlock;                 // nullptr if no else
};

struct WhileStmt : ASTNodeBase<WhileStmt> {
    ExprPtr condition;
    StmtPtr body;
};

struct ForStmt : ASTNodeBase<ForStmt> {
    std::string varName;
    ExprPtr start;
    ExprPtr end;
    StmtPtr body;
};

struct ExprStmt : ASTNodeBase<ExprStmt> {
    ExprPtr expr;
};

struct Block : ASTNodeBase<Block> {
    std::vector<StmtPtr> statements;
};

// ── Function Declaration ────────────────────────────────────────────────────

struct Param {
    std::string name;
    TypeSpec type;
};

struct FunctionDecl : ASTNodeBase<FunctionDecl> {
    std::string name;
    std::vector<Param> params;
    TypeSpec returnType;               // Void if not specified
    std::unique_ptr<Block> body;
};

// ── Program (root node) ─────────────────────────────────────────────────────

struct Program : ASTNodeBase<Program> {
    std::vector<ASTNodePtr> declarations;  // mix of FunctionDecl and statements
};

} // namespace tiny
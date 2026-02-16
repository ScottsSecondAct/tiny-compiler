#pragma once

#include "tiny/AST.h"
#include "tiny/Diagnostics.h"
#include "tiny/SymbolTable.h"
#include <string>

namespace tiny {

/// Walks the AST and performs semantic checks:
///   - Variable/function declaration and resolution
///   - Type checking for expressions and assignments
///   - Mutability enforcement (let vs var)
///   - Function call arity and argument type checking
///   - Return type validation
///
/// Usage:
///   Diagnostics diags;
///   SemanticAnalyzer analyzer(diags);
///   analyzer.analyze(*program);
///   if (diags.hasErrors()) { ... }
class SemanticAnalyzer : public ASTVisitor {
public:
    explicit SemanticAnalyzer(Diagnostics& diags);

    /// Main entry point
    void analyze(Program& program);

    // ── Expressions ─────────────────────────────────────────────────────
    std::any visit(IntLit& node) override;
    std::any visit(FloatLit& node) override;
    std::any visit(BoolLit& node) override;
    std::any visit(StringLit& node) override;
    std::any visit(Identifier& node) override;
    std::any visit(ArrayLiteral& node) override;
    std::any visit(BinaryExpr& node) override;
    std::any visit(UnaryExpr& node) override;
    std::any visit(CallExpr& node) override;
    std::any visit(IndexExpr& node) override;

    // ── Statements ──────────────────────────────────────────────────────
    std::any visit(VarDecl& node) override;
    std::any visit(Assignment& node) override;
    std::any visit(ReturnStmt& node) override;
    std::any visit(PrintStmt& node) override;
    std::any visit(IfStmt& node) override;
    std::any visit(WhileStmt& node) override;
    std::any visit(ForStmt& node) override;
    std::any visit(ExprStmt& node) override;
    std::any visit(Block& node) override;
    std::any visit(FunctionDecl& node) override;

    // ── Top-level ───────────────────────────────────────────────────────
    std::any visit(Program& node) override;

private:
    Diagnostics& diags_;
    SymbolTable symbols_;

    /// Tracks the return type of the function currently being analyzed
    /// (nullptr when at global scope)
    TypeSpec* currentReturnType_ = nullptr;

    // ── Helpers ─────────────────────────────────────────────────────────

    /// Extract a TypeSpec from the std::any returned by visiting an expression
    TypeSpec getType(std::any& result);

    /// Check that two types are compatible; report error if not
    bool checkTypesMatch(const TypeSpec& expected, const TypeSpec& actual,
                         const ASTNode& node, const std::string& context);

    /// Check if a type supports arithmetic operations
    bool isNumeric(const TypeSpec& type) const;

    /// Check if a type supports comparison operations
    bool isComparable(const TypeSpec& type) const;

    /// Register a built-in function in the symbol table
    void declareBuiltin(const std::string& name,
                        const std::vector<TypeSpec>& paramTypes,
                        const TypeSpec& returnType);
};

} // namespace tiny

#pragma once

#include "tiny/AST.h"
#include <functional>
#include <iostream>

namespace tiny {

/// Pretty-prints an AST with indentation for debugging.
class ASTPrinter : public ASTVisitor {
public:
    explicit ASTPrinter(std::ostream& out = std::cout) : out_(out) {}

    void print(ASTNode& node) { node.accept(*this); }

    // Expressions
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
    std::any visit(LambdaExpr& node) override;

    // Statements
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

    // Top-level
    std::any visit(Program& node) override;

private:
    std::ostream& out_;
    int indent_ = 0;

    void printIndent();
    void indented(const std::string& label, std::function<void()> body);
};

} // namespace tiny
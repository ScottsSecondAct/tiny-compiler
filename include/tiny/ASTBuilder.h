#pragma once

#include "tiny/AST.h"
#include "TinyBaseVisitor.h"

namespace tiny {

/// Walks ANTLR's parse tree and produces a clean AST.
///
/// Each visit method returns std::any wrapping the appropriate AST node type.
/// Use buildProgram() as the main entry point.
class ASTBuilder : public TinyBaseVisitor {
public:
    /// Main entry point: parse tree → AST
    std::unique_ptr<Program> buildProgram(TinyParser::ProgramContext* ctx);

private:
    // ── Helpers ─────────────────────────────────────────────────────────
    SourceLoc getLoc(antlr4::ParserRuleContext* ctx);
    TypeSpec  buildType(TinyParser::TypeSpecContext* ctx);

    // ── Top-level ───────────────────────────────────────────────────────
    std::any visitProgram(TinyParser::ProgramContext* ctx) override;
    std::any visitDeclaration(TinyParser::DeclarationContext* ctx) override;

    // ── Functions ───────────────────────────────────────────────────────
    std::any visitFunctionDecl(TinyParser::FunctionDeclContext* ctx) override;

    // ── Statements ──────────────────────────────────────────────────────
    std::any visitStatement(TinyParser::StatementContext* ctx) override;
    std::any visitVarDecl(TinyParser::VarDeclContext* ctx) override;
    std::any visitAssignment(TinyParser::AssignmentContext* ctx) override;
    std::any visitReturnStmt(TinyParser::ReturnStmtContext* ctx) override;
    std::any visitPrintStmt(TinyParser::PrintStmtContext* ctx) override;
    std::any visitIfStmt(TinyParser::IfStmtContext* ctx) override;
    std::any visitWhileStmt(TinyParser::WhileStmtContext* ctx) override;
    std::any visitForStmt(TinyParser::ForStmtContext* ctx) override;
    std::any visitExprStmt(TinyParser::ExprStmtContext* ctx) override;
    std::any visitBlock(TinyParser::BlockContext* ctx) override;

    // ── Expressions ─────────────────────────────────────────────────────
    std::any visitExpression(TinyParser::ExpressionContext* ctx) override;
    std::any visitLogicalOr(TinyParser::LogicalOrContext* ctx) override;
    std::any visitLogicalAnd(TinyParser::LogicalAndContext* ctx) override;
    std::any visitEquality(TinyParser::EqualityContext* ctx) override;
    std::any visitComparison(TinyParser::ComparisonContext* ctx) override;
    std::any visitAddition(TinyParser::AdditionContext* ctx) override;
    std::any visitMultiplication(TinyParser::MultiplicationContext* ctx) override;
    std::any visitUnary(TinyParser::UnaryContext* ctx) override;
    std::any visitPostfix(TinyParser::PostfixContext* ctx) override;
    std::any visitPrimary(TinyParser::PrimaryContext* ctx) override;
    std::any visitLambdaExpr(TinyParser::LambdaExprContext* ctx) override;
    std::any visitArrayLiteral(TinyParser::ArrayLiteralContext* ctx) override;
    std::any visitExpressionList(TinyParser::ExpressionListContext* ctx) override;
};

} // namespace tiny
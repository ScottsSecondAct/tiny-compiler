
// Generated from grammar/Tiny.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "TinyVisitor.h"


/**
 * This class provides an empty implementation of TinyVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  TinyBaseVisitor : public TinyVisitor {
public:

  virtual std::any visitProgram(TinyParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(TinyParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDecl(TinyParser::FunctionDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamList(TinyParser::ParamListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParam(TinyParser::ParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(TinyParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(TinyParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignment(TinyParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStmt(TinyParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrintStmt(TinyParser::PrintStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIfStmt(TinyParser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhileStmt(TinyParser::WhileStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForStmt(TinyParser::ForStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmt(TinyParser::ExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(TinyParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(TinyParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalOr(TinyParser::LogicalOrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalAnd(TinyParser::LogicalAndContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEquality(TinyParser::EqualityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparison(TinyParser::ComparisonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddition(TinyParser::AdditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplication(TinyParser::MultiplicationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnary(TinyParser::UnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPostfix(TinyParser::PostfixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimary(TinyParser::PrimaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayLiteral(TinyParser::ArrayLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionList(TinyParser::ExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeSpec(TinyParser::TypeSpecContext *ctx) override {
    return visitChildren(ctx);
  }


};


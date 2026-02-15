
// Generated from grammar/Tiny.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "TinyParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by TinyParser.
 */
class  TinyVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by TinyParser.
   */
    virtual std::any visitProgram(TinyParser::ProgramContext *context) = 0;

    virtual std::any visitDeclaration(TinyParser::DeclarationContext *context) = 0;

    virtual std::any visitFunctionDecl(TinyParser::FunctionDeclContext *context) = 0;

    virtual std::any visitParamList(TinyParser::ParamListContext *context) = 0;

    virtual std::any visitParam(TinyParser::ParamContext *context) = 0;

    virtual std::any visitStatement(TinyParser::StatementContext *context) = 0;

    virtual std::any visitVarDecl(TinyParser::VarDeclContext *context) = 0;

    virtual std::any visitAssignment(TinyParser::AssignmentContext *context) = 0;

    virtual std::any visitReturnStmt(TinyParser::ReturnStmtContext *context) = 0;

    virtual std::any visitPrintStmt(TinyParser::PrintStmtContext *context) = 0;

    virtual std::any visitIfStmt(TinyParser::IfStmtContext *context) = 0;

    virtual std::any visitWhileStmt(TinyParser::WhileStmtContext *context) = 0;

    virtual std::any visitForStmt(TinyParser::ForStmtContext *context) = 0;

    virtual std::any visitExprStmt(TinyParser::ExprStmtContext *context) = 0;

    virtual std::any visitBlock(TinyParser::BlockContext *context) = 0;

    virtual std::any visitExpression(TinyParser::ExpressionContext *context) = 0;

    virtual std::any visitLogicalOr(TinyParser::LogicalOrContext *context) = 0;

    virtual std::any visitLogicalAnd(TinyParser::LogicalAndContext *context) = 0;

    virtual std::any visitEquality(TinyParser::EqualityContext *context) = 0;

    virtual std::any visitComparison(TinyParser::ComparisonContext *context) = 0;

    virtual std::any visitAddition(TinyParser::AdditionContext *context) = 0;

    virtual std::any visitMultiplication(TinyParser::MultiplicationContext *context) = 0;

    virtual std::any visitUnary(TinyParser::UnaryContext *context) = 0;

    virtual std::any visitPostfix(TinyParser::PostfixContext *context) = 0;

    virtual std::any visitPrimary(TinyParser::PrimaryContext *context) = 0;

    virtual std::any visitArrayLiteral(TinyParser::ArrayLiteralContext *context) = 0;

    virtual std::any visitExpressionList(TinyParser::ExpressionListContext *context) = 0;

    virtual std::any visitTypeSpec(TinyParser::TypeSpecContext *context) = 0;


};


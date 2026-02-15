#include "tiny/ASTBuilder.h"
#include <stdexcept>

namespace tiny {

// ── Helpers ─────────────────────────────────────────────────────────────────

SourceLoc ASTBuilder::getLoc(antlr4::ParserRuleContext* ctx) {
    auto* tok = ctx->getStart();
    return {static_cast<int>(tok->getLine()),
            static_cast<int>(tok->getCharPositionInLine())};
}

TypeSpec ASTBuilder::buildType(TinyParser::TypeSpecContext* ctx) {
    if (!ctx) return TypeSpec::makeUnknown();

    if (ctx->typeSpec()) {
        auto elemType = buildType(ctx->typeSpec());
        int size = std::stoi(ctx->INT_LIT()->getText());
        return TypeSpec::makeArray(elemType, size);
    }

    std::string text = ctx->getText();
    if (text == "int")    return TypeSpec::makeInt();
    if (text == "float")  return TypeSpec::makeFloat();
    if (text == "bool")   return TypeSpec::makeBool();
    if (text == "string") return TypeSpec::makeString();

    throw std::runtime_error("Unknown type: " + text);
}

// ── Entry point ─────────────────────────────────────────────────────────────

std::unique_ptr<Program> ASTBuilder::buildProgram(TinyParser::ProgramContext* ctx) {
    auto result = unboxNode(visitProgram(ctx));
    return std::unique_ptr<Program>(static_cast<Program*>(result.release()));
}

// ── Top-level ───────────────────────────────────────────────────────────────

std::any ASTBuilder::visitProgram(TinyParser::ProgramContext* ctx) {
    auto program = std::make_unique<Program>();
    program->loc = getLoc(ctx);

    for (auto* declCtx : ctx->declaration()) {
        program->declarations.push_back(unboxNode(visitDeclaration(declCtx)));
    }

    return boxNode(std::move(program));
}

std::any ASTBuilder::visitDeclaration(TinyParser::DeclarationContext* ctx) {
    if (ctx->functionDecl()) return visitFunctionDecl(ctx->functionDecl());
    return visitStatement(ctx->statement());
}

// ── Functions ───────────────────────────────────────────────────────────────

std::any ASTBuilder::visitFunctionDecl(TinyParser::FunctionDeclContext* ctx) {
    auto func = std::make_unique<FunctionDecl>();
    func->loc = getLoc(ctx);
    func->name = ctx->IDENT()->getText();

    if (ctx->paramList()) {
        for (auto* paramCtx : ctx->paramList()->param()) {
            Param p;
            p.name = paramCtx->IDENT()->getText();
            p.type = buildType(paramCtx->typeSpec());
            func->params.push_back(std::move(p));
        }
    }

    func->returnType = ctx->typeSpec() ? buildType(ctx->typeSpec()) : TypeSpec::makeVoid();

    auto bodyNode = unboxNode(visitBlock(ctx->block()));
    func->body = std::unique_ptr<Block>(static_cast<Block*>(bodyNode.release()));

    return boxNode(std::move(func));
}

// ── Statements ──────────────────────────────────────────────────────────────

std::any ASTBuilder::visitStatement(TinyParser::StatementContext* ctx) {
    if (ctx->varDecl())     return visitVarDecl(ctx->varDecl());
    if (ctx->assignment())  return visitAssignment(ctx->assignment());
    if (ctx->returnStmt())  return visitReturnStmt(ctx->returnStmt());
    if (ctx->printStmt())   return visitPrintStmt(ctx->printStmt());
    if (ctx->ifStmt())      return visitIfStmt(ctx->ifStmt());
    if (ctx->whileStmt())   return visitWhileStmt(ctx->whileStmt());
    if (ctx->forStmt())     return visitForStmt(ctx->forStmt());
    if (ctx->exprStmt())    return visitExprStmt(ctx->exprStmt());
    if (ctx->block())       return visitBlock(ctx->block());

    throw std::runtime_error("Unknown statement at line " +
                             std::to_string(ctx->getStart()->getLine()));
}

std::any ASTBuilder::visitVarDecl(TinyParser::VarDeclContext* ctx) {
    auto decl = std::make_unique<VarDecl>();
    decl->loc = getLoc(ctx);
    decl->name = ctx->IDENT()->getText();
    decl->isMutable = (ctx->getStart()->getText() == "var");
    decl->type = ctx->typeSpec() ? buildType(ctx->typeSpec()) : TypeSpec::makeUnknown();
    decl->init = unboxNode(visitExpression(ctx->expression()));
    return boxNode(std::move(decl));
}

std::any ASTBuilder::visitAssignment(TinyParser::AssignmentContext* ctx) {
    auto assign = std::make_unique<Assignment>();
    assign->loc = getLoc(ctx);
    assign->target = ctx->IDENT()->getText();

    auto exprs = ctx->expression();
    if (exprs.size() == 2) {
        assign->index = unboxNode(visitExpression(exprs[0]));
        assign->value = unboxNode(visitExpression(exprs[1]));
    } else {
        assign->index = nullptr;
        assign->value = unboxNode(visitExpression(exprs[0]));
    }
    return boxNode(std::move(assign));
}

std::any ASTBuilder::visitReturnStmt(TinyParser::ReturnStmtContext* ctx) {
    auto ret = std::make_unique<ReturnStmt>();
    ret->loc = getLoc(ctx);
    if (ctx->expression()) ret->value = unboxNode(visitExpression(ctx->expression()));
    return boxNode(std::move(ret));
}

std::any ASTBuilder::visitPrintStmt(TinyParser::PrintStmtContext* ctx) {
    auto print = std::make_unique<PrintStmt>();
    print->loc = getLoc(ctx);
    if (ctx->expressionList()) {
        for (auto* exprCtx : ctx->expressionList()->expression()) {
            print->args.push_back(unboxNode(visitExpression(exprCtx)));
        }
    }
    return boxNode(std::move(print));
}

std::any ASTBuilder::visitIfStmt(TinyParser::IfStmtContext* ctx) {
    auto ifNode = std::make_unique<IfStmt>();
    ifNode->loc = getLoc(ctx);
    ifNode->condition = unboxNode(visitExpression(ctx->expression()));

    auto blocks = ctx->block();
    ifNode->thenBlock = unboxNode(visitBlock(blocks[0]));

    if (ctx->ifStmt()) {
        ifNode->elseBlock = unboxNode(visitIfStmt(ctx->ifStmt()));
    } else if (blocks.size() > 1) {
        ifNode->elseBlock = unboxNode(visitBlock(blocks[1]));
    }
    return boxNode(std::move(ifNode));
}

std::any ASTBuilder::visitWhileStmt(TinyParser::WhileStmtContext* ctx) {
    auto node = std::make_unique<WhileStmt>();
    node->loc = getLoc(ctx);
    node->condition = unboxNode(visitExpression(ctx->expression()));
    node->body = unboxNode(visitBlock(ctx->block()));
    return boxNode(std::move(node));
}

std::any ASTBuilder::visitForStmt(TinyParser::ForStmtContext* ctx) {
    auto node = std::make_unique<ForStmt>();
    node->loc = getLoc(ctx);
    node->varName = ctx->IDENT()->getText();
    auto exprs = ctx->expression();
    node->start = unboxNode(visitExpression(exprs[0]));
    node->end   = unboxNode(visitExpression(exprs[1]));
    node->body  = unboxNode(visitBlock(ctx->block()));
    return boxNode(std::move(node));
}

std::any ASTBuilder::visitExprStmt(TinyParser::ExprStmtContext* ctx) {
    auto stmt = std::make_unique<ExprStmt>();
    stmt->loc = getLoc(ctx);
    stmt->expr = unboxNode(visitExpression(ctx->expression()));
    return boxNode(std::move(stmt));
}

std::any ASTBuilder::visitBlock(TinyParser::BlockContext* ctx) {
    auto block = std::make_unique<Block>();
    block->loc = getLoc(ctx);
    for (auto* stmtCtx : ctx->statement()) {
        block->statements.push_back(unboxNode(visitStatement(stmtCtx)));
    }
    return boxNode(std::move(block));
}

// ── Expressions ─────────────────────────────────────────────────────────────

std::any ASTBuilder::visitExpression(TinyParser::ExpressionContext* ctx) {
    return visitLogicalOr(ctx->logicalOr());
}

std::any ASTBuilder::visitLogicalOr(TinyParser::LogicalOrContext* ctx) {
    auto terms = ctx->logicalAnd();
    auto result = unboxNode(visitLogicalAnd(terms[0]));
    for (size_t i = 1; i < terms.size(); i++) {
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = BinOp::Or;
        bin->left = std::move(result);
        bin->right = unboxNode(visitLogicalAnd(terms[i]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitLogicalAnd(TinyParser::LogicalAndContext* ctx) {
    auto terms = ctx->equality();
    auto result = unboxNode(visitEquality(terms[0]));
    for (size_t i = 1; i < terms.size(); i++) {
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = BinOp::And;
        bin->left = std::move(result);
        bin->right = unboxNode(visitEquality(terms[i]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitEquality(TinyParser::EqualityContext* ctx) {
    auto terms = ctx->comparison();
    auto result = unboxNode(visitComparison(terms[0]));
    auto children = ctx->children;
    size_t termIdx = 1;
    for (size_t i = 0; i < children.size(); i++) {
        auto* tn = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
        if (!tn) continue;
        std::string op = tn->getText();
        BinOp binOp;
        if (op == "==")      binOp = BinOp::Eq;
        else if (op == "!=") binOp = BinOp::Neq;
        else continue;
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = binOp;
        bin->left = std::move(result);
        bin->right = unboxNode(visitComparison(terms[termIdx++]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitComparison(TinyParser::ComparisonContext* ctx) {
    auto terms = ctx->addition();
    auto result = unboxNode(visitAddition(terms[0]));
    auto children = ctx->children;
    size_t termIdx = 1;
    for (size_t i = 0; i < children.size(); i++) {
        auto* tn = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
        if (!tn) continue;
        std::string op = tn->getText();
        BinOp binOp;
        if (op == "<")        binOp = BinOp::Lt;
        else if (op == "<=")  binOp = BinOp::Lte;
        else if (op == ">")   binOp = BinOp::Gt;
        else if (op == ">=")  binOp = BinOp::Gte;
        else continue;
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = binOp;
        bin->left = std::move(result);
        bin->right = unboxNode(visitAddition(terms[termIdx++]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitAddition(TinyParser::AdditionContext* ctx) {
    auto terms = ctx->multiplication();
    auto result = unboxNode(visitMultiplication(terms[0]));
    auto children = ctx->children;
    size_t termIdx = 1;
    for (size_t i = 0; i < children.size(); i++) {
        auto* tn = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
        if (!tn) continue;
        std::string op = tn->getText();
        BinOp binOp;
        if (op == "+")      binOp = BinOp::Add;
        else if (op == "-") binOp = BinOp::Sub;
        else continue;
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = binOp;
        bin->left = std::move(result);
        bin->right = unboxNode(visitMultiplication(terms[termIdx++]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitMultiplication(TinyParser::MultiplicationContext* ctx) {
    auto terms = ctx->unary();
    auto result = unboxNode(visitUnary(terms[0]));
    auto children = ctx->children;
    size_t termIdx = 1;
    for (size_t i = 0; i < children.size(); i++) {
        auto* tn = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
        if (!tn) continue;
        std::string op = tn->getText();
        BinOp binOp;
        if (op == "*")      binOp = BinOp::Mul;
        else if (op == "/") binOp = BinOp::Div;
        else if (op == "%") binOp = BinOp::Mod;
        else continue;
        auto bin = std::make_unique<BinaryExpr>();
        bin->loc = getLoc(ctx); bin->op = binOp;
        bin->left = std::move(result);
        bin->right = unboxNode(visitUnary(terms[termIdx++]));
        result = std::move(bin);
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitUnary(TinyParser::UnaryContext* ctx) {
    if (ctx->unary()) {
        auto node = std::make_unique<UnaryExpr>();
        node->loc = getLoc(ctx);
        std::string opText = ctx->children[0]->getText();
        node->op = (opText == "-") ? UnOp::Neg : UnOp::Not;
        node->operand = unboxNode(visitUnary(ctx->unary()));
        return boxNode(std::move(node));
    }
    return visitPostfix(ctx->postfix());
}

std::any ASTBuilder::visitPostfix(TinyParser::PostfixContext* ctx) {
    auto result = unboxNode(visitPrimary(ctx->primary()));

    auto children = ctx->children;
    for (size_t i = 1; i < children.size(); i++) {
        auto* terminal = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
        if (!terminal) continue;
        std::string text = terminal->getText();

        if (text == "(") {
            auto call = std::make_unique<CallExpr>();
            call->loc = getLoc(ctx);
            auto* ident = dynamic_cast<Identifier*>(result.get());
            if (ident) call->callee = ident->name;
            for (size_t j = i + 1; j < children.size(); j++) {
                auto* exprList = dynamic_cast<TinyParser::ExpressionListContext*>(children[j]);
                if (exprList) {
                    for (auto* ec : exprList->expression())
                        call->args.push_back(unboxNode(visitExpression(ec)));
                    break;
                }
                auto* cp = dynamic_cast<antlr4::tree::TerminalNode*>(children[j]);
                if (cp && cp->getText() == ")") break;
            }
            result = std::move(call);
            while (i + 1 < children.size()) {
                i++;
                auto* t = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
                if (t && t->getText() == ")") break;
            }
        } else if (text == "[") {
            auto idx = std::make_unique<IndexExpr>();
            idx->loc = getLoc(ctx);
            idx->array = std::move(result);
            for (size_t j = i + 1; j < children.size(); j++) {
                auto* ec = dynamic_cast<TinyParser::ExpressionContext*>(children[j]);
                if (ec) { idx->index = unboxNode(visitExpression(ec)); break; }
            }
            result = std::move(idx);
            while (i + 1 < children.size()) {
                i++;
                auto* t = dynamic_cast<antlr4::tree::TerminalNode*>(children[i]);
                if (t && t->getText() == "]") break;
            }
        }
    }
    return boxNode(std::move(result));
}

std::any ASTBuilder::visitPrimary(TinyParser::PrimaryContext* ctx) {
    if (ctx->INT_LIT()) {
        auto n = std::make_unique<IntLit>(std::stoll(ctx->INT_LIT()->getText()));
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->FLOAT_LIT()) {
        auto n = std::make_unique<FloatLit>(std::stod(ctx->FLOAT_LIT()->getText()));
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->STRING_LIT()) {
        std::string raw = ctx->STRING_LIT()->getText();
        auto n = std::make_unique<StringLit>(raw.substr(1, raw.size() - 2));
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->getText() == "true") {
        auto n = std::make_unique<BoolLit>(true);
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->getText() == "false") {
        auto n = std::make_unique<BoolLit>(false);
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->IDENT()) {
        auto n = std::make_unique<Identifier>(ctx->IDENT()->getText());
        n->loc = getLoc(ctx);
        return boxNode(std::move(n));
    }
    if (ctx->arrayLiteral()) return visitArrayLiteral(ctx->arrayLiteral());
    if (ctx->expression()) return visitExpression(ctx->expression());

    throw std::runtime_error("Unknown primary at line " +
                             std::to_string(ctx->getStart()->getLine()));
}

std::any ASTBuilder::visitArrayLiteral(TinyParser::ArrayLiteralContext* ctx) {
    auto arr = std::make_unique<ArrayLiteral>();
    arr->loc = getLoc(ctx);
    if (ctx->expressionList()) {
        for (auto* ec : ctx->expressionList()->expression())
            arr->elements.push_back(unboxNode(visitExpression(ec)));
    }
    return boxNode(std::move(arr));
}

std::any ASTBuilder::visitExpressionList(TinyParser::ExpressionListContext* ctx) {
    throw std::runtime_error("visitExpressionList should not be called directly");
}

} // namespace tiny
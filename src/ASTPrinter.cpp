#include "tiny/ASTPrinter.h"
#include <functional>

namespace tiny {

void ASTPrinter::printIndent() {
    for (int i = 0; i < indent_; i++) out_ << "  ";
}

void ASTPrinter::indented(const std::string& label, std::function<void()> body) {
    printIndent();
    out_ << label << "\n";
    indent_++;
    body();
    indent_--;
}

// ── Expressions ─────────────────────────────────────────────────────────────

std::any ASTPrinter::visit(IntLit& node) {
    printIndent();
    out_ << "IntLit(" << node.value << ")\n";
    return {};
}

std::any ASTPrinter::visit(FloatLit& node) {
    printIndent();
    out_ << "FloatLit(" << node.value << ")\n";
    return {};
}

std::any ASTPrinter::visit(BoolLit& node) {
    printIndent();
    out_ << "BoolLit(" << (node.value ? "true" : "false") << ")\n";
    return {};
}

std::any ASTPrinter::visit(StringLit& node) {
    printIndent();
    out_ << "StringLit(\"" << node.value << "\")\n";
    return {};
}

std::any ASTPrinter::visit(Identifier& node) {
    printIndent();
    out_ << "Identifier(" << node.name << ")\n";
    return {};
}

std::any ASTPrinter::visit(ArrayLiteral& node) {
    indented("ArrayLiteral", [&] {
        for (auto& elem : node.elements) {
            elem->accept(*this);
        }
    });
    return {};
}

std::any ASTPrinter::visit(BinaryExpr& node) {
    indented("BinaryExpr(" + binOpToString(node.op) + ")", [&] {
        node.left->accept(*this);
        node.right->accept(*this);
    });
    return {};
}

std::any ASTPrinter::visit(UnaryExpr& node) {
    std::string opStr = (node.op == UnOp::Neg) ? "-" : "!";
    indented("UnaryExpr(" + opStr + ")", [&] {
        node.operand->accept(*this);
    });
    return {};
}

std::any ASTPrinter::visit(CallExpr& node) {
    std::string label = node.callee.empty() ? "CallExpr(<expr>)" : "CallExpr(" + node.callee + ")";
    indented(label, [&] {
        if (node.calleeExpr) {
            printIndent(); out_ << "callee:\n";
            indent_++;
            node.calleeExpr->accept(*this);
            indent_--;
        }
        for (auto& arg : node.args) {
            arg->accept(*this);
        }
    });
    return {};
}

std::any ASTPrinter::visit(IndexExpr& node) {
    indented("IndexExpr", [&] {
        node.array->accept(*this);
        printIndent(); out_ << "index:\n";
        indent_++;
        node.index->accept(*this);
        indent_--;
    });
    return {};
}

std::any ASTPrinter::visit(LambdaExpr& node) {
    std::string sig = "LambdaExpr(fn(";
    for (size_t i = 0; i < node.params.size(); i++) {
        if (i > 0) sig += ", ";
        sig += node.params[i].name + ": " + node.params[i].type.toString();
    }
    sig += ") -> " + node.returnType.toString() + ")";
    if (!node.captures.empty()) {
        sig += " captures[";
        for (size_t i = 0; i < node.captures.size(); i++) {
            if (i > 0) sig += ", ";
            sig += node.captures[i];
        }
        sig += "]";
    }
    indented(sig, [&] {
        node.body->accept(*this);
    });
    return {};
}

// ── Statements ──────────────────────────────────────────────────────────────

std::any ASTPrinter::visit(VarDecl& node) {
    std::string mutStr = node.isMutable ? "var" : "let";
    indented("VarDecl(" + mutStr + " " + node.name + ": " + node.type.toString() + ")", [&] {
        node.init->accept(*this);
    });
    return {};
}

std::any ASTPrinter::visit(Assignment& node) {
    indented("Assignment(" + node.target + ")", [&] {
        if (node.index) {
            printIndent(); out_ << "index:\n";
            indent_++;
            node.index->accept(*this);
            indent_--;
        }
        printIndent(); out_ << "value:\n";
        indent_++;
        node.value->accept(*this);
        indent_--;
    });
    return {};
}

std::any ASTPrinter::visit(ReturnStmt& node) {
    if (node.value) {
        indented("ReturnStmt", [&] {
            node.value->accept(*this);
        });
    } else {
        printIndent();
        out_ << "ReturnStmt(void)\n";
    }
    return {};
}

std::any ASTPrinter::visit(PrintStmt& node) {
    indented("PrintStmt", [&] {
        for (auto& arg : node.args) {
            arg->accept(*this);
        }
    });
    return {};
}

std::any ASTPrinter::visit(IfStmt& node) {
    indented("IfStmt", [&] {
        printIndent(); out_ << "condition:\n";
        indent_++;
        node.condition->accept(*this);
        indent_--;

        printIndent(); out_ << "then:\n";
        indent_++;
        node.thenBlock->accept(*this);
        indent_--;

        if (node.elseBlock) {
            printIndent(); out_ << "else:\n";
            indent_++;
            node.elseBlock->accept(*this);
            indent_--;
        }
    });
    return {};
}

std::any ASTPrinter::visit(WhileStmt& node) {
    indented("WhileStmt", [&] {
        printIndent(); out_ << "condition:\n";
        indent_++;
        node.condition->accept(*this);
        indent_--;

        printIndent(); out_ << "body:\n";
        indent_++;
        node.body->accept(*this);
        indent_--;
    });
    return {};
}

std::any ASTPrinter::visit(ForStmt& node) {
    indented("ForStmt(" + node.varName + ")", [&] {
        printIndent(); out_ << "start:\n";
        indent_++;
        node.start->accept(*this);
        indent_--;

        printIndent(); out_ << "end:\n";
        indent_++;
        node.end->accept(*this);
        indent_--;

        printIndent(); out_ << "body:\n";
        indent_++;
        node.body->accept(*this);
        indent_--;
    });
    return {};
}

std::any ASTPrinter::visit(ExprStmt& node) {
    indented("ExprStmt", [&] {
        node.expr->accept(*this);
    });
    return {};
}

std::any ASTPrinter::visit(Block& node) {
    indented("Block", [&] {
        for (auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    });
    return {};
}

std::any ASTPrinter::visit(FunctionDecl& node) {
    std::string sig = "FunctionDecl(" + node.name + "(";
    for (size_t i = 0; i < node.params.size(); i++) {
        if (i > 0) sig += ", ";
        sig += node.params[i].name + ": " + node.params[i].type.toString();
    }
    sig += ") -> " + node.returnType.toString() + ")";

    indented(sig, [&] {
        node.body->accept(*this);
    });
    return {};
}

// ── Top-level ───────────────────────────────────────────────────────────────

std::any ASTPrinter::visit(Program& node) {
    indented("Program", [&] {
        for (auto& decl : node.declarations) {
            decl->accept(*this);
        }
    });
    return {};
}

} // namespace tiny
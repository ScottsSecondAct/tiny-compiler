#include "tiny/SemanticAnalyzer.h"
#include <any>

namespace tiny {

// ── Construction & Entry Point ──────────────────────────────────────────────

SemanticAnalyzer::SemanticAnalyzer(Diagnostics& diags) : diags_(diags) {}

void SemanticAnalyzer::analyze(Program& program) {
    program.accept(*this);
}

// ── Helpers ─────────────────────────────────────────────────────────────────

TypeSpec SemanticAnalyzer::getType(std::any& result) {
    if (!result.has_value()) return TypeSpec::makeUnknown();
    try {
        return std::any_cast<TypeSpec>(result);
    } catch (const std::bad_any_cast&) {
        return TypeSpec::makeUnknown();
    }
}

bool SemanticAnalyzer::checkTypesMatch(const TypeSpec& expected, const TypeSpec& actual,
                                        const ASTNode& node, const std::string& context) {
    if (expected.kind == TypeKind::Unknown || actual.kind == TypeKind::Unknown) {
        return true; // can't check yet — allow it
    }
    if (expected != actual) {
        diags_.error(node, context + ": cannot use '" + actual.toString()
                     + "' where '" + expected.toString() + "' is expected");
        return false;
    }
    return true;
}

bool SemanticAnalyzer::isNumeric(const TypeSpec& type) const {
    return type.kind == TypeKind::Int || type.kind == TypeKind::Float;
}

bool SemanticAnalyzer::isComparable(const TypeSpec& type) const {
    return type.kind == TypeKind::Int || type.kind == TypeKind::Float
        || type.kind == TypeKind::String;
}

void SemanticAnalyzer::declareBuiltin(const std::string& name,
                                       const std::vector<TypeSpec>& paramTypes,
                                       const TypeSpec& returnType) {
    Symbol sym;
    sym.name = name;
    sym.isFunction = true;
    sym.paramTypes = paramTypes;
    sym.returnType = returnType;
    sym.type = returnType;
    symbols_.declare(sym);
}

// ── Top-level ───────────────────────────────────────────────────────────────

std::any SemanticAnalyzer::visit(Program& node) {
    // Global scope
    symbols_.pushScope();

    // First pass: register all function declarations so they can call each other
    for (auto& decl : node.declarations) {
        auto* func = dynamic_cast<FunctionDecl*>(decl.get());
        if (func) {
            Symbol sym;
            sym.name = func->name;
            sym.isFunction = true;
            sym.declaredAt = func->loc;
            sym.returnType = func->returnType;
            sym.type = func->returnType;
            for (auto& p : func->params) {
                sym.paramTypes.push_back(p.type);
            }
            if (!symbols_.declare(sym)) {
                diags_.error(*func, "function '" + func->name + "' is already declared");
            }
        }
    }

    // Second pass: analyze everything
    for (auto& decl : node.declarations) {
        decl->accept(*this);
    }

    symbols_.popScope();
    return {};
}

// ── Functions ───────────────────────────────────────────────────────────────

std::any SemanticAnalyzer::visit(FunctionDecl& node) {
    // Push function scope
    symbols_.pushScope();

    // Declare parameters
    for (auto& param : node.params) {
        Symbol sym;
        sym.name = param.name;
        sym.type = param.type;
        sym.isMutable = false;  // parameters are immutable by default
        sym.declaredAt = node.loc;
        if (!symbols_.declare(sym)) {
            diags_.error(node, "duplicate parameter name '" + param.name + "'");
        }
    }

    // Track the expected return type for return statement checking
    TypeSpec* prevReturnType = currentReturnType_;
    currentReturnType_ = &node.returnType;

    // Analyze the function body
    node.body->accept(*this);

    currentReturnType_ = prevReturnType;
    symbols_.popScope();
    return {};
}

// ── Statements ──────────────────────────────────────────────────────────────

std::any SemanticAnalyzer::visit(VarDecl& node) {
    // Analyze the initializer first
    auto initResult = node.init->accept(*this);
    TypeSpec initType = getType(initResult);

    // If explicit type given, check it matches the initializer
    if (node.type.kind != TypeKind::Unknown) {
        checkTypesMatch(node.type, initType, node, "type mismatch");
    } else {
        // Type inference: adopt the initializer's type
        if (initType.kind != TypeKind::Unknown) {
            node.type = initType;
        } else {
            diags_.error(node, "cannot infer type for '" + node.name
                         + "'; please add a type annotation");
        }
    }

    // Declare in current scope
    Symbol sym;
    sym.name = node.name;
    sym.type = node.type;
    sym.isMutable = node.isMutable;
    sym.declaredAt = node.loc;
    if (!symbols_.declare(sym)) {
        diags_.error(node, "variable '" + node.name + "' is already declared in this scope");
    }

    return {};
}

std::any SemanticAnalyzer::visit(Assignment& node) {
    // Look up the target
    Symbol* sym = symbols_.lookup(node.target);
    if (!sym) {
        diags_.error(node, "undefined variable '" + node.target + "'");
        return {};
    }

    if (sym->isFunction) {
        diags_.error(node, "cannot assign to function '" + node.target + "'");
        return {};
    }

    if (!sym->isMutable) {
        diags_.error(node, "cannot assign to immutable variable '" + node.target + "'");
        return {};
    }

    // If array element assignment: check index and element type
    if (node.index) {
        auto indexResult = node.index->accept(*this);
        TypeSpec indexType = getType(indexResult);
        if (indexType.kind != TypeKind::Unknown && indexType.kind != TypeKind::Int) {
            diags_.error(*node.index, "array index must be 'int', got '"
                         + indexType.toString() + "'");
        }

        auto valueResult = node.value->accept(*this);
        TypeSpec valueType = getType(valueResult);

        if (sym->type.kind == TypeKind::Array && sym->type.elementType) {
            checkTypesMatch(*sym->type.elementType, valueType, node, "array element assignment");
        }
    } else {
        // Simple assignment
        auto valueResult = node.value->accept(*this);
        TypeSpec valueType = getType(valueResult);
        checkTypesMatch(sym->type, valueType, node, "assignment");
    }

    return {};
}

std::any SemanticAnalyzer::visit(ReturnStmt& node) {
    if (!currentReturnType_) {
        diags_.error(node, "'return' outside of a function");
        return {};
    }

    if (node.value) {
        auto valueResult = node.value->accept(*this);
        TypeSpec valueType = getType(valueResult);
        checkTypesMatch(*currentReturnType_, valueType, node, "return type mismatch");
    } else {
        // Bare return — only valid for void functions
        if (currentReturnType_->kind != TypeKind::Void) {
            diags_.error(node, "non-void function must return a value");
        }
    }

    return {};
}

std::any SemanticAnalyzer::visit(PrintStmt& node) {
    // print() accepts any types — just analyze arguments
    for (auto& arg : node.args) {
        arg->accept(*this);
    }
    return {};
}

std::any SemanticAnalyzer::visit(IfStmt& node) {
    auto condResult = node.condition->accept(*this);
    TypeSpec condType = getType(condResult);
    if (condType.kind != TypeKind::Unknown && condType.kind != TypeKind::Bool) {
        diags_.error(*node.condition, "if condition must be 'bool', got '"
                     + condType.toString() + "'");
    }

    node.thenBlock->accept(*this);
    if (node.elseBlock) {
        node.elseBlock->accept(*this);
    }

    return {};
}

std::any SemanticAnalyzer::visit(WhileStmt& node) {
    auto condResult = node.condition->accept(*this);
    TypeSpec condType = getType(condResult);
    if (condType.kind != TypeKind::Unknown && condType.kind != TypeKind::Bool) {
        diags_.error(*node.condition, "while condition must be 'bool', got '"
                     + condType.toString() + "'");
    }

    node.body->accept(*this);
    return {};
}

std::any SemanticAnalyzer::visit(ForStmt& node) {
    // Analyze range bounds
    auto startResult = node.start->accept(*this);
    TypeSpec startType = getType(startResult);
    if (startType.kind != TypeKind::Unknown && startType.kind != TypeKind::Int) {
        diags_.error(*node.start, "for range start must be 'int', got '"
                     + startType.toString() + "'");
    }

    auto endResult = node.end->accept(*this);
    TypeSpec endType = getType(endResult);
    if (endType.kind != TypeKind::Unknown && endType.kind != TypeKind::Int) {
        diags_.error(*node.end, "for range end must be 'int', got '"
                     + endType.toString() + "'");
    }

    // Push a new scope for the loop variable
    symbols_.pushScope();

    Symbol loopVar;
    loopVar.name = node.varName;
    loopVar.type = TypeSpec::makeInt();
    loopVar.isMutable = false;  // loop variable is immutable
    loopVar.declaredAt = node.loc;
    symbols_.declare(loopVar);

    node.body->accept(*this);

    symbols_.popScope();
    return {};
}

std::any SemanticAnalyzer::visit(ExprStmt& node) {
    node.expr->accept(*this);
    return {};
}

std::any SemanticAnalyzer::visit(Block& node) {
    symbols_.pushScope();
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
    symbols_.popScope();
    return {};
}

// ── Expressions ─────────────────────────────────────────────────────────────
// Each expression visitor returns std::any wrapping a TypeSpec.

std::any SemanticAnalyzer::visit(IntLit& /*node*/) {
    return TypeSpec::makeInt();
}

std::any SemanticAnalyzer::visit(FloatLit& /*node*/) {
    return TypeSpec::makeFloat();
}

std::any SemanticAnalyzer::visit(BoolLit& /*node*/) {
    return TypeSpec::makeBool();
}

std::any SemanticAnalyzer::visit(StringLit& /*node*/) {
    return TypeSpec::makeString();
}

std::any SemanticAnalyzer::visit(Identifier& node) {
    Symbol* sym = symbols_.lookup(node.name);
    if (!sym) {
        diags_.error(node, "undefined variable '" + node.name + "'");
        return TypeSpec::makeUnknown();
    }
    return sym->type;
}

std::any SemanticAnalyzer::visit(ArrayLiteral& node) {
    if (node.elements.empty()) {
        return TypeSpec::makeUnknown();
    }

    // Determine element type from the first element
    auto firstResult = node.elements[0]->accept(*this);
    TypeSpec elemType = getType(firstResult);

    // Check all elements match
    for (size_t i = 1; i < node.elements.size(); i++) {
        auto result = node.elements[i]->accept(*this);
        TypeSpec t = getType(result);
        checkTypesMatch(elemType, t, *node.elements[i], "array element type mismatch");
    }

    return TypeSpec::makeArray(elemType, static_cast<int>(node.elements.size()));
}

std::any SemanticAnalyzer::visit(BinaryExpr& node) {
    auto leftResult = node.left->accept(*this);
    TypeSpec leftType = getType(leftResult);

    auto rightResult = node.right->accept(*this);
    TypeSpec rightType = getType(rightResult);

    // If either side is unknown, we can't check — propagate unknown
    if (leftType.kind == TypeKind::Unknown || rightType.kind == TypeKind::Unknown) {
        return TypeSpec::makeUnknown();
    }

    switch (node.op) {
        // Arithmetic: requires matching numeric types, result is that type
        case BinOp::Add:
            // Special case: string + string = string concatenation
            if (leftType.kind == TypeKind::String && rightType.kind == TypeKind::String) {
                return TypeSpec::makeString();
            }
            [[fallthrough]];
        case BinOp::Sub:
        case BinOp::Mul:
        case BinOp::Div:
        case BinOp::Mod:
            if (!isNumeric(leftType)) {
                diags_.error(*node.left, "operator '" + binOpToString(node.op)
                             + "' requires numeric type, got '" + leftType.toString() + "'");
                return TypeSpec::makeUnknown();
            }
            if (!isNumeric(rightType)) {
                diags_.error(*node.right, "operator '" + binOpToString(node.op)
                             + "' requires numeric type, got '" + rightType.toString() + "'");
                return TypeSpec::makeUnknown();
            }
            checkTypesMatch(leftType, rightType, node, "operand type mismatch");
            return leftType;

        // Comparison: requires matching comparable types, result is bool
        case BinOp::Eq:
        case BinOp::Neq:
            checkTypesMatch(leftType, rightType, node, "comparison type mismatch");
            return TypeSpec::makeBool();

        case BinOp::Lt:
        case BinOp::Lte:
        case BinOp::Gt:
        case BinOp::Gte:
            if (!isComparable(leftType)) {
                diags_.error(*node.left, "operator '" + binOpToString(node.op)
                             + "' requires comparable type, got '" + leftType.toString() + "'");
                return TypeSpec::makeUnknown();
            }
            checkTypesMatch(leftType, rightType, node, "comparison type mismatch");
            return TypeSpec::makeBool();

        // Logical: requires bool operands, result is bool
        case BinOp::And:
        case BinOp::Or:
            if (leftType.kind != TypeKind::Bool) {
                diags_.error(*node.left, "operator '" + binOpToString(node.op)
                             + "' requires 'bool', got '" + leftType.toString() + "'");
            }
            if (rightType.kind != TypeKind::Bool) {
                diags_.error(*node.right, "operator '" + binOpToString(node.op)
                             + "' requires 'bool', got '" + rightType.toString() + "'");
            }
            return TypeSpec::makeBool();
    }

    return TypeSpec::makeUnknown();
}

std::any SemanticAnalyzer::visit(UnaryExpr& node) {
    auto operandResult = node.operand->accept(*this);
    TypeSpec operandType = getType(operandResult);

    if (operandType.kind == TypeKind::Unknown) {
        return TypeSpec::makeUnknown();
    }

    switch (node.op) {
        case UnOp::Neg:
            if (!isNumeric(operandType)) {
                diags_.error(node, "unary '-' requires numeric type, got '"
                             + operandType.toString() + "'");
                return TypeSpec::makeUnknown();
            }
            return operandType;

        case UnOp::Not:
            if (operandType.kind != TypeKind::Bool) {
                diags_.error(node, "unary '!' requires 'bool', got '"
                             + operandType.toString() + "'");
                return TypeSpec::makeUnknown();
            }
            return TypeSpec::makeBool();
    }

    return TypeSpec::makeUnknown();
}

std::any SemanticAnalyzer::visit(CallExpr& node) {
    Symbol* sym = symbols_.lookup(node.callee);
    if (!sym) {
        diags_.error(node, "undefined function '" + node.callee + "'");
        return TypeSpec::makeUnknown();
    }

    if (!sym->isFunction) {
        diags_.error(node, "'" + node.callee + "' is not a function");
        return TypeSpec::makeUnknown();
    }

    // Check argument count
    if (node.args.size() != sym->paramTypes.size()) {
        diags_.error(node, "function '" + node.callee + "' expects "
                     + std::to_string(sym->paramTypes.size()) + " argument(s), got "
                     + std::to_string(node.args.size()));
        return sym->returnType;
    }

    // Check argument types
    for (size_t i = 0; i < node.args.size(); i++) {
        auto argResult = node.args[i]->accept(*this);
        TypeSpec argType = getType(argResult);
        checkTypesMatch(sym->paramTypes[i], argType, *node.args[i],
                        "argument " + std::to_string(i + 1) + " of '" + node.callee + "'");
    }

    return sym->returnType;
}

std::any SemanticAnalyzer::visit(IndexExpr& node) {
    auto arrayResult = node.array->accept(*this);
    TypeSpec arrayType = getType(arrayResult);

    auto indexResult = node.index->accept(*this);
    TypeSpec indexType = getType(indexResult);

    // Index must be int
    if (indexType.kind != TypeKind::Unknown && indexType.kind != TypeKind::Int) {
        diags_.error(*node.index, "array index must be 'int', got '"
                     + indexType.toString() + "'");
    }

    // Array must actually be an array type
    if (arrayType.kind == TypeKind::Unknown) {
        return TypeSpec::makeUnknown();
    }

    if (arrayType.kind != TypeKind::Array) {
        diags_.error(*node.array, "cannot index into non-array type '"
                     + arrayType.toString() + "'");
        return TypeSpec::makeUnknown();
    }

    // Result type is the element type
    if (arrayType.elementType) {
        return *arrayType.elementType;
    }
    return TypeSpec::makeUnknown();
}

} // namespace tiny

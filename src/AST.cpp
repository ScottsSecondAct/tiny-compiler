#include "tiny/AST.h"

namespace tiny {

std::string binOpToString(BinOp op) {
    switch (op) {
        case BinOp::Add: return "+";
        case BinOp::Sub: return "-";
        case BinOp::Mul: return "*";
        case BinOp::Div: return "/";
        case BinOp::Mod: return "%";
        case BinOp::Eq:  return "==";
        case BinOp::Neq: return "!=";
        case BinOp::Lt:  return "<";
        case BinOp::Lte: return "<=";
        case BinOp::Gt:  return ">";
        case BinOp::Gte: return ">=";
        case BinOp::And: return "&&";
        case BinOp::Or:  return "||";
    }
    return "?";
}

} // namespace tiny

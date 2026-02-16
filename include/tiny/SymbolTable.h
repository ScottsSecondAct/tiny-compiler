#pragma once

#include "tiny/AST.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace tiny {

/// Metadata about a declared name (variable or function).
struct Symbol {
    std::string name;
    TypeSpec type;
    bool isMutable = false;
    bool isFunction = false;
    SourceLoc declaredAt;

    // Function-specific
    std::vector<TypeSpec> paramTypes;
    TypeSpec returnType;
};

/// A stack of scopes mapping names to symbols.
///
/// Each block, function, or for-loop pushes a new scope.
/// Lookup walks from the innermost scope outward.
class SymbolTable {
public:
    /// Push a new scope (entering a block, function, or loop)
    void pushScope();

    /// Pop the current scope (leaving a block, function, or loop)
    void popScope();

    /// Declare a symbol in the current (innermost) scope.
    /// Returns false if the name is already declared in the current scope.
    bool declare(const Symbol& sym);

    /// Look up a name, searching from innermost scope outward.
    /// Returns nullptr if not found.
    Symbol* lookup(const std::string& name);

    /// Look up only in the current (innermost) scope.
    /// Returns nullptr if not found.
    Symbol* lookupCurrent(const std::string& name);

    /// Current scope depth (0 = global)
    int depth() const { return static_cast<int>(scopes_.size()) - 1; }

private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
};

} // namespace tiny

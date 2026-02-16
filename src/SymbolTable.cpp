#include "tiny/SymbolTable.h"
#include <stdexcept>

namespace tiny {

void SymbolTable::pushScope() {
    scopes_.emplace_back();
}

void SymbolTable::popScope() {
    if (scopes_.empty()) {
        throw std::runtime_error("SymbolTable::popScope: no scope to pop");
    }
    scopes_.pop_back();
}

bool SymbolTable::declare(const Symbol& sym) {
    if (scopes_.empty()) {
        throw std::runtime_error("SymbolTable::declare: no scope available");
    }

    auto& current = scopes_.back();
    // Check if already declared in the current scope
    if (current.find(sym.name) != current.end()) {
        return false;
    }
    current[sym.name] = sym;
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    // Walk from innermost scope to outermost
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

Symbol* SymbolTable::lookupCurrent(const std::string& name) {
    if (scopes_.empty()) return nullptr;
    auto& current = scopes_.back();
    auto found = current.find(name);
    if (found != current.end()) {
        return &found->second;
    }
    return nullptr;
}

} // namespace tiny
#pragma once

#include "tiny/AST.h"
#include <iostream>
#include <string>
#include <vector>

namespace tiny {

enum class DiagLevel {
    Error,
    Warning,
};

struct Diagnostic {
    DiagLevel level;
    SourceLoc loc;
    std::string message;
};

/// Collects and reports compiler diagnostics with source locations.
class Diagnostics {
public:
    void error(const SourceLoc& loc, const std::string& message);
    void warning(const SourceLoc& loc, const std::string& message);

    /// Convenience: error using an AST node's location
    void error(const ASTNode& node, const std::string& message);
    void warning(const ASTNode& node, const std::string& message);

    bool hasErrors() const { return errorCount_ > 0; }
    int errorCount() const { return errorCount_; }
    int warningCount() const { return warningCount_; }

    const std::vector<Diagnostic>& all() const { return diagnostics_; }

    /// Print all diagnostics to a stream
    void dump(std::ostream& out = std::cerr) const;

private:
    std::vector<Diagnostic> diagnostics_;
    int errorCount_ = 0;
    int warningCount_ = 0;
};

} // namespace tiny
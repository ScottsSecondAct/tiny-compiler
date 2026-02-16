#include "tiny/Diagnostics.h"

namespace tiny {

void Diagnostics::error(const SourceLoc& loc, const std::string& message) {
    diagnostics_.push_back({DiagLevel::Error, loc, message});
    errorCount_++;
}

void Diagnostics::warning(const SourceLoc& loc, const std::string& message) {
    diagnostics_.push_back({DiagLevel::Warning, loc, message});
    warningCount_++;
}

void Diagnostics::error(const ASTNode& node, const std::string& message) {
    error(node.loc, message);
}

void Diagnostics::warning(const ASTNode& node, const std::string& message) {
    warning(node.loc, message);
}

void Diagnostics::dump(std::ostream& out) const {
    for (auto& diag : diagnostics_) {
        if (diag.level == DiagLevel::Error) {
            out << "Error";
        } else {
            out << "Warning";
        }
        out << ": " << diag.message
            << " (line " << diag.loc.line << ")\n";
    }

    if (errorCount_ > 0) {
        out << "\n" << errorCount_ << " error(s)";
        if (warningCount_ > 0) {
            out << ", " << warningCount_ << " warning(s)";
        }
        out << "\n";
    }
}

} // namespace tiny
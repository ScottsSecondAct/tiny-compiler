#pragma once
#include "tiny/AST.h"
#include "tiny/Diagnostics.h"
#include <set>
#include <string>

namespace tiny {

/// Resolves import declarations by loading the imported files, extracting
/// their FunctionDecl nodes, and prepending them to the main program.
/// This merges all modules at the AST level before semantic analysis runs.
class ModuleLoader {
public:
    explicit ModuleLoader(Diagnostics& diags);

    /// Expand all ImportDecl nodes in program by prepending imported FunctionDecls.
    void resolve(Program& program, const std::string& mainFilePath);

private:
    Diagnostics& diags_;
    std::set<std::string> loading_;   // in-progress chain (circular detection)
    std::set<std::string> loaded_;    // already done (deduplication)

    std::string resolvePath(const std::string& importPath, const std::string& baseDir);

    bool loadFile(const std::string& absPath, const SourceLoc& importLoc,
                  std::vector<ASTNodePtr>& out);
};

} // namespace tiny

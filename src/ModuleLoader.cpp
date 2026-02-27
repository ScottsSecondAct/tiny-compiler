#include "tiny/ModuleLoader.h"
#include "tiny/ASTBuilder.h"

#include "antlr4-runtime.h"
#include "TinyLexer.h"
#include "TinyParser.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace tiny {

ModuleLoader::ModuleLoader(Diagnostics& diags) : diags_(diags) {}

std::string ModuleLoader::resolvePath(const std::string& importPath,
                                       const std::string& baseDir) {
    return (fs::path(baseDir) / importPath).lexically_normal().string();
}

bool ModuleLoader::loadFile(const std::string& absPath,
                             const SourceLoc& importLoc,
                             std::vector<ASTNodePtr>& out) {
    std::string basename = fs::path(absPath).filename().string();

    // Deduplication: already fully loaded, skip
    if (loaded_.count(absPath)) {
        return true;
    }

    // Circular import detection
    if (loading_.count(absPath)) {
        diags_.error(importLoc, "circular import detected: '" + basename + "'");
        return false;
    }

    // Open the file
    std::ifstream inStream(absPath);
    if (!inStream.is_open()) {
        diags_.error(importLoc, "cannot open module '" + basename + "'");
        return false;
    }

    // Mark as currently loading
    loading_.insert(absPath);

    // Parse the file
    antlr4::ANTLRInputStream input(inStream);
    TinyLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    TinyParser parser(&tokens);
    auto* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() > 0) {
        diags_.error(importLoc, "syntax error in module '" + basename + "'");
        loading_.erase(absPath);
        return false;
    }

    ASTBuilder builder;
    auto program = builder.buildProgram(tree);

    // Compute base directory of this file for resolving its own imports
    std::string newBaseDir = fs::path(absPath).parent_path().string();

    // Recursively resolve imports declared in this file first
    for (auto& decl : program->declarations) {
        if (auto* imp = dynamic_cast<ImportDecl*>(decl.get())) {
            std::string childPath = resolvePath(imp->path, newBaseDir);
            if (!loadFile(childPath, imp->loc, out)) {
                loading_.erase(absPath);
                return false;
            }
        }
    }

    // Extract FunctionDecl nodes
    for (auto& decl : program->declarations) {
        if (dynamic_cast<ImportDecl*>(decl.get())) {
            continue;  // already handled above
        }
        if (dynamic_cast<FunctionDecl*>(decl.get())) {
            out.push_back(std::move(decl));
        } else {
            diags_.error(importLoc,
                "imported module '" + basename + "' contains top-level statements");
            loading_.erase(absPath);
            return false;
        }
    }

    loading_.erase(absPath);
    loaded_.insert(absPath);
    return true;
}

void ModuleLoader::resolve(Program& program, const std::string& mainFilePath) {
    std::string baseDir = fs::path(mainFilePath).parent_path().string();

    std::vector<ASTNodePtr> extra;

    for (auto& decl : program.declarations) {
        if (auto* imp = dynamic_cast<ImportDecl*>(decl.get())) {
            std::string absPath = resolvePath(imp->path, baseDir);
            loadFile(absPath, imp->loc, extra);
        }
    }

    if (diags_.hasErrors()) {
        return;
    }

    // Prepend collected function declarations before the main program's declarations
    for (auto it = extra.rbegin(); it != extra.rend(); ++it) {
        program.declarations.insert(program.declarations.begin(), std::move(*it));
    }
}

} // namespace tiny

#include <iostream>
#include <fstream>
#include <string>

#include "antlr4-runtime.h"
#include "TinyLexer.h"
#include "TinyParser.h"
#include "tiny/ASTBuilder.h"
#include "tiny/ASTPrinter.h"
// #include "tiny/SemanticAnalyzer.h"
// #include "tiny/CodeGen.h"

void printUsage(const char* argv0) {
    std::cerr << "Usage: " << argv0
              << " <source.tiny> [-o output.ll] [--dump-ast] [--dump-tokens]\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = "output.ll";
    bool dumpAST = false;
    bool dumpTokens = false;

    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "--dump-ast") {
            dumpAST = true;
        } else if (arg == "--dump-tokens") {
            dumpTokens = true;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    std::ifstream inStream(inputFile);
    if (!inStream.is_open()) {
        std::cerr << "Error: cannot open file '" << inputFile << "'\n";
        return 1;
    }

    // ── Phase 1: Lex & Parse ────────────────────────────────────────────
    antlr4::ANTLRInputStream input(inStream);
    TinyLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    if (dumpTokens) {
        tokens.fill();
        for (auto* token : tokens.getTokens()) {
            std::cout << token->toString() << "\n";
        }
        return 0;
    }

    TinyParser parser(&tokens);
    auto* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Parsing failed with "
                  << parser.getNumberOfSyntaxErrors() << " error(s).\n";
        return 1;
    }

    // ── Phase 2: Build AST ──────────────────────────────────────────────
    tiny::ASTBuilder builder;
    auto ast = builder.buildProgram(tree);

    if (dumpAST) {
        tiny::ASTPrinter printer(std::cout);
        printer.print(*ast);
        return 0;
    }

    // ── Phase 3: Semantic Analysis ──────────────────────────────────────
    // tiny::SemanticAnalyzer analyzer;
    // analyzer.analyze(*ast);

    // ── Phase 4: Code Generation ────────────────────────────────────────
    // tiny::CodeGen codegen;
    // codegen.generate(*ast, outputFile);

    std::cout << "Parsed and built AST successfully.\n";
    std::cout << "Use --dump-ast to see the tree.\n";
    return 0;
}
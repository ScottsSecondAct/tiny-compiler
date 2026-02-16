#pragma once

#include "tiny/AST.h"
#include "tiny/Diagnostics.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <map>
#include <string>

namespace tiny {

/// Walks the validated AST and emits LLVM IR.
///
/// Usage:
///   CodeGen codegen(diags);
///   codegen.generate(program, "output.ll");
class CodeGen : public ASTVisitor {
public:
    explicit CodeGen(Diagnostics& diags);

    /// Main entry point: generate IR for the program and write to file
    bool generate(Program& program, const std::string& outputFile);

    // ── Expressions (return llvm::Value*) ───────────────────────────────
    std::any visit(IntLit& node) override;
    std::any visit(FloatLit& node) override;
    std::any visit(BoolLit& node) override;
    std::any visit(StringLit& node) override;
    std::any visit(Identifier& node) override;
    std::any visit(ArrayLiteral& node) override;
    std::any visit(BinaryExpr& node) override;
    std::any visit(UnaryExpr& node) override;
    std::any visit(CallExpr& node) override;
    std::any visit(IndexExpr& node) override;

    // ── Statements (return nullptr) ─────────────────────────────────────
    std::any visit(VarDecl& node) override;
    std::any visit(Assignment& node) override;
    std::any visit(ReturnStmt& node) override;
    std::any visit(PrintStmt& node) override;
    std::any visit(IfStmt& node) override;
    std::any visit(WhileStmt& node) override;
    std::any visit(ForStmt& node) override;
    std::any visit(ExprStmt& node) override;
    std::any visit(Block& node) override;
    std::any visit(FunctionDecl& node) override;

    // ── Top-level ───────────────────────────────────────────────────────
    std::any visit(Program& node) override;

private:
    Diagnostics& diags_;

    llvm::LLVMContext context_;
    llvm::IRBuilder<> builder_;
    std::unique_ptr<llvm::Module> module_;

    /// Variable name → alloca instruction (for loads/stores)
    struct VarInfo {
        llvm::AllocaInst* alloca;
        TypeSpec type;
    };
    std::vector<std::map<std::string, VarInfo>> scopeStack_;

    /// Function name → LLVM function (for call resolution)
    std::map<std::string, llvm::Function*> functions_;

    // ── Helpers ─────────────────────────────────────────────────────────

    /// Convert a Tiny TypeSpec to an LLVM type
    llvm::Type* toLLVMType(const TypeSpec& type);

    /// Create an alloca at function entry (standard pattern for local vars)
    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* func,
                                              const std::string& name,
                                              llvm::Type* type);

    /// Extract llvm::Value* from std::any
    llvm::Value* getValue(std::any& result);

    /// Scope management
    void pushScope();
    void popScope();
    void declareVar(const std::string& name, llvm::AllocaInst* alloca, const TypeSpec& type);
    VarInfo* lookupVar(const std::string& name);

    /// Declare the external runtime functions (tiny_print_int, etc.)
    void declareRuntimeFunctions();

    /// Emit a print call for one argument based on its type
    void emitPrintArg(llvm::Value* val, const TypeSpec& type);

    /// Get the TypeSpec for an expression by re-evaluating (used for print dispatch)
    TypeSpec inferExprType(ASTNode& node);
};

} // namespace tiny

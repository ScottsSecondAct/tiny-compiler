#include "tiny/CodeGen.h"

#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Passes/PassBuilder.h>

#include <any>
#include <cassert>

namespace tiny {

// ── Construction ────────────────────────────────────────────────────────────

CodeGen::CodeGen(Diagnostics& diags)
    : diags_(diags), builder_(context_) {
    module_ = std::make_unique<llvm::Module>("tiny_module", context_);
}

// ── Helpers ─────────────────────────────────────────────────────────────────

llvm::Type* CodeGen::toLLVMType(const TypeSpec& type) {
    switch (type.kind) {
        case TypeKind::Int:    return llvm::Type::getInt64Ty(context_);
        case TypeKind::Float:  return llvm::Type::getDoubleTy(context_);
        case TypeKind::Bool:   return llvm::Type::getInt1Ty(context_);
        case TypeKind::String: return llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_));
        case TypeKind::Void:   return llvm::Type::getVoidTy(context_);
        case TypeKind::Array:
            if (type.elementType) {
                auto* elemTy = toLLVMType(*type.elementType);
                return llvm::ArrayType::get(elemTy, type.arraySize);
            }
            return llvm::Type::getVoidTy(context_);
        case TypeKind::Unknown:
            return llvm::Type::getVoidTy(context_);
    }
    return llvm::Type::getVoidTy(context_);
}

llvm::AllocaInst* CodeGen::createEntryBlockAlloca(llvm::Function* func,
                                                    const std::string& name,
                                                    llvm::Type* type) {
    llvm::IRBuilder<> tmpBuilder(&func->getEntryBlock(),
                                  func->getEntryBlock().begin());
    return tmpBuilder.CreateAlloca(type, nullptr, name);
}

llvm::Value* CodeGen::getValue(std::any& result) {
    if (!result.has_value()) return nullptr;
    try {
        return std::any_cast<llvm::Value*>(result);
    } catch (const std::bad_any_cast&) {
        return nullptr;
    }
}

void CodeGen::pushScope() {
    scopeStack_.emplace_back();
}

void CodeGen::popScope() {
    scopeStack_.pop_back();
}

void CodeGen::declareVar(const std::string& name, llvm::AllocaInst* alloca,
                          const TypeSpec& type) {
    scopeStack_.back()[name] = {alloca, type};
}

CodeGen::VarInfo* CodeGen::lookupVar(const std::string& name) {
    for (auto it = scopeStack_.rbegin(); it != scopeStack_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

// ── Runtime function declarations ───────────────────────────────────────────

void CodeGen::declareRuntimeFunctions() {
    auto* voidTy  = llvm::Type::getVoidTy(context_);
    auto* i64Ty   = llvm::Type::getInt64Ty(context_);
    auto* dblTy   = llvm::Type::getDoubleTy(context_);
    auto* i8PtrTy = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context_));
    auto* i8Ty    = llvm::Type::getInt8Ty(context_);

    // void tiny_print_int(i64)
    module_->getOrInsertFunction("tiny_print_int",
        llvm::FunctionType::get(voidTy, {i64Ty}, false));

    // void tiny_print_float(double)
    module_->getOrInsertFunction("tiny_print_float",
        llvm::FunctionType::get(voidTy, {dblTy}, false));

    // void tiny_print_str(i8*)
    module_->getOrInsertFunction("tiny_print_str",
        llvm::FunctionType::get(voidTy, {i8PtrTy}, false));

    // void tiny_print_bool(i8)
    module_->getOrInsertFunction("tiny_print_bool",
        llvm::FunctionType::get(voidTy, {i8Ty}, false));

    // void tiny_print_newline()
    module_->getOrInsertFunction("tiny_print_newline",
        llvm::FunctionType::get(voidTy, {}, false));
}

void CodeGen::emitPrintArg(llvm::Value* val, const TypeSpec& type) {
    llvm::Function* printFn = nullptr;

    switch (type.kind) {
        case TypeKind::Int:
            printFn = module_->getFunction("tiny_print_int");
            break;
        case TypeKind::Float:
            printFn = module_->getFunction("tiny_print_float");
            break;
        case TypeKind::Bool: {
            printFn = module_->getFunction("tiny_print_bool");
            // Bool is i1 in LLVM, but tiny_print_bool expects i8
            val = builder_.CreateZExt(val, llvm::Type::getInt8Ty(context_));
            break;
        }
        case TypeKind::String:
            printFn = module_->getFunction("tiny_print_str");
            break;
        default:
            // Fallback: try print as int
            printFn = module_->getFunction("tiny_print_int");
            break;
    }

    if (printFn) {
        builder_.CreateCall(printFn, {val});
    }
}

TypeSpec CodeGen::inferExprType(ASTNode& node) {
    // Simple type inference by checking what kind of node it is
    if (dynamic_cast<IntLit*>(&node))    return TypeSpec::makeInt();
    if (dynamic_cast<FloatLit*>(&node))  return TypeSpec::makeFloat();
    if (dynamic_cast<BoolLit*>(&node))   return TypeSpec::makeBool();
    if (dynamic_cast<StringLit*>(&node)) return TypeSpec::makeString();

    if (auto* id = dynamic_cast<Identifier*>(&node)) {
        auto* var = lookupVar(id->name);
        if (var) return var->type;
    }

    if (auto* bin = dynamic_cast<BinaryExpr*>(&node)) {
        switch (bin->op) {
            case BinOp::Eq: case BinOp::Neq:
            case BinOp::Lt: case BinOp::Lte:
            case BinOp::Gt: case BinOp::Gte:
            case BinOp::And: case BinOp::Or:
                return TypeSpec::makeBool();
            default:
                return inferExprType(*bin->left);
        }
    }

    if (auto* un = dynamic_cast<UnaryExpr*>(&node)) {
        if (un->op == UnOp::Not) return TypeSpec::makeBool();
        return inferExprType(*un->operand);
    }

    if (auto* call = dynamic_cast<CallExpr*>(&node)) {
        auto* fn = module_->getFunction(call->callee);
        if (fn) {
            auto* retTy = fn->getReturnType();
            if (retTy->isIntegerTy(64)) return TypeSpec::makeInt();
            if (retTy->isDoubleTy()) return TypeSpec::makeFloat();
            if (retTy->isIntegerTy(1)) return TypeSpec::makeBool();
            if (retTy->isPointerTy()) return TypeSpec::makeString();
        }
    }

    if (auto* idx = dynamic_cast<IndexExpr*>(&node)) {
        // Array element type — check the array's type
        if (auto* arrId = dynamic_cast<Identifier*>(idx->array.get())) {
            auto* var = lookupVar(arrId->name);
            if (var && var->type.kind == TypeKind::Array && var->type.elementType) {
                return *var->type.elementType;
            }
        }
    }

    return TypeSpec::makeInt(); // fallback
}

// ── Main entry point ────────────────────────────────────────────────────────

bool CodeGen::generate(Program& program, const std::string& outputFile,
                        OptLevel optLevel) {
    program.accept(*this);

    // Verify the module
    std::string verifyError;
    llvm::raw_string_ostream verifyStream(verifyError);
    if (llvm::verifyModule(*module_, &verifyStream)) {
        diags_.error(program, "LLVM verification failed: " + verifyError);
        return false;
    }

    // Run optimization passes if requested
    if (optLevel != OptLevel::O0) {
        runOptimizations(optLevel);
    }

    // Write .ll file
    std::error_code ec;
    llvm::raw_fd_ostream outFile(outputFile, ec, llvm::sys::fs::OF_Text);
    if (ec) {
        diags_.error(program, "Cannot open output file '" + outputFile + "': " + ec.message());
        return false;
    }

    module_->print(outFile, nullptr);
    return true;
}

// ── Top-level ───────────────────────────────────────────────────────────────

std::any CodeGen::visit(Program& node) {
    declareRuntimeFunctions();

    pushScope(); // global scope

    // First pass: declare all functions so they can call each other
    for (auto& decl : node.declarations) {
        if (auto* func = dynamic_cast<FunctionDecl*>(decl.get())) {
            // Build parameter types
            std::vector<llvm::Type*> paramTypes;
            for (auto& p : func->params) {
                paramTypes.push_back(toLLVMType(p.type));
            }

            auto* retTy = toLLVMType(func->returnType);
            auto* funcTy = llvm::FunctionType::get(retTy, paramTypes, false);
            auto* llvmFunc = llvm::Function::Create(
                funcTy, llvm::Function::ExternalLinkage, func->name, module_.get());

            functions_[func->name] = llvmFunc;
        }
    }

    // Collect top-level statements (non-function declarations)
    std::vector<ASTNode*> topLevelStmts;
    for (auto& decl : node.declarations) {
        if (!dynamic_cast<FunctionDecl*>(decl.get())) {
            topLevelStmts.push_back(decl.get());
        }
    }

    // If there are top-level statements, wrap them in a main() function
    // (unless the user already defined main)
    bool hasUserMain = functions_.find("main") != functions_.end();

    if (!topLevelStmts.empty() && !hasUserMain) {
        // Create implicit main
        auto* mainTy = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context_), {}, false);
        auto* mainFunc = llvm::Function::Create(
            mainTy, llvm::Function::ExternalLinkage, "main", module_.get());
        functions_["main"] = mainFunc;

        auto* entry = llvm::BasicBlock::Create(context_, "entry", mainFunc);
        builder_.SetInsertPoint(entry);

        pushScope(); // main's scope

        for (auto* stmt : topLevelStmts) {
            stmt->accept(*this);
        }

        // If the block doesn't already have a terminator, add return 0
        if (!builder_.GetInsertBlock()->getTerminator()) {
            builder_.CreateRet(llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(context_), 0));
        }

        popScope();
    }

    // Second pass: generate function bodies
    for (auto& decl : node.declarations) {
        if (auto* func = dynamic_cast<FunctionDecl*>(decl.get())) {
            func->accept(*this);
        }
    }

    // If user defined main() but it returns void or int, ensure it returns int
    // for proper C linkage
    if (hasUserMain && !topLevelStmts.empty()) {
        // Top-level statements need to go somewhere — put them before user's main
        // For now, top-level statements only work when there's no user-defined main
    }

    popScope(); // global scope

    return {};
}

// ── Functions ───────────────────────────────────────────────────────────────

std::any CodeGen::visit(FunctionDecl& node) {
    auto* func = functions_[node.name];
    if (!func) return {};

    auto* entry = llvm::BasicBlock::Create(context_, "entry", func);
    builder_.SetInsertPoint(entry);

    pushScope();

    // Create allocas for parameters and store the argument values
    unsigned idx = 0;
    for (auto& arg : func->args()) {
        auto& param = node.params[idx];
        auto* alloca = createEntryBlockAlloca(func, param.name, arg.getType());
        builder_.CreateStore(&arg, alloca);
        declareVar(param.name, alloca, param.type);
        arg.setName(param.name);
        idx++;
    }

    // Generate the function body
    for (auto& stmt : node.body->statements) {
        stmt->accept(*this);
        // If we've inserted a terminator (return), stop
        if (builder_.GetInsertBlock()->getTerminator()) break;
    }

    // If no terminator yet, add an implicit return
    if (!builder_.GetInsertBlock()->getTerminator()) {
        if (node.returnType.kind == TypeKind::Void) {
            builder_.CreateRetVoid();
        } else if (node.returnType.kind == TypeKind::Int) {
            builder_.CreateRet(llvm::ConstantInt::get(
                llvm::Type::getInt64Ty(context_), 0));
        } else if (node.returnType.kind == TypeKind::Float) {
            builder_.CreateRet(llvm::ConstantFP::get(
                llvm::Type::getDoubleTy(context_), 0.0));
        } else if (node.returnType.kind == TypeKind::Bool) {
            builder_.CreateRet(llvm::ConstantInt::get(
                llvm::Type::getInt1Ty(context_), 0));
        } else {
            builder_.CreateRetVoid();
        }
    }

    popScope();
    return {};
}

// ── Statements ──────────────────────────────────────────────────────────────

std::any CodeGen::visit(VarDecl& node) {
    auto* func = builder_.GetInsertBlock()->getParent();
    auto* llvmType = toLLVMType(node.type);
    auto* alloca = createEntryBlockAlloca(func, node.name, llvmType);

    // Special case: array literal initialization
    if (auto* arrLit = dynamic_cast<ArrayLiteral*>(node.init.get())) {
        auto* zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 0);
        for (size_t i = 0; i < arrLit->elements.size(); i++) {
            auto elemResult = arrLit->elements[i]->accept(*this);
            auto* elemVal = getValue(elemResult);
            if (elemVal) {
                auto* idx = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), i);
                auto* elemPtr = builder_.CreateGEP(llvmType, alloca, {zero, idx}, "arrinit");
                builder_.CreateStore(elemVal, elemPtr);
            }
        }
    } else {
        // Scalar initialization
        auto initResult = node.init->accept(*this);
        auto* initVal = getValue(initResult);
        if (initVal) {
            builder_.CreateStore(initVal, alloca);
        }
    }

    declareVar(node.name, alloca, node.type);
    return {};
}

std::any CodeGen::visit(Assignment& node) {
    auto* var = lookupVar(node.target);
    if (!var) return {};

    if (node.index) {
        // Array element assignment: arr[i] = value
        auto indexResult = node.index->accept(*this);
        auto* indexVal = getValue(indexResult);

        auto valueResult = node.value->accept(*this);
        auto* valueVal = getValue(valueResult);

        auto* zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 0);
        auto* elemPtr = builder_.CreateGEP(
            toLLVMType(var->type), var->alloca, {zero, indexVal}, "elemptr");
        builder_.CreateStore(valueVal, elemPtr);
    } else {
        // Simple assignment
        auto valueResult = node.value->accept(*this);
        auto* valueVal = getValue(valueResult);
        if (valueVal) {
            builder_.CreateStore(valueVal, var->alloca);
        }
    }

    return {};
}

std::any CodeGen::visit(ReturnStmt& node) {
    if (node.value) {
        auto valResult = node.value->accept(*this);
        auto* val = getValue(valResult);
        if (val) {
            builder_.CreateRet(val);
        }
    } else {
        builder_.CreateRetVoid();
    }
    return {};
}

std::any CodeGen::visit(PrintStmt& node) {
    for (auto& arg : node.args) {
        TypeSpec argType = inferExprType(*arg);
        auto result = arg->accept(*this);
        auto* val = getValue(result);
        if (val) {
            emitPrintArg(val, argType);
        }
    }

    // Print newline after all arguments
    auto* newlineFn = module_->getFunction("tiny_print_newline");
    if (newlineFn) {
        builder_.CreateCall(newlineFn, {});
    }

    return {};
}

std::any CodeGen::visit(IfStmt& node) {
    auto condResult = node.condition->accept(*this);
    auto* condVal = getValue(condResult);
    if (!condVal) return {};

    auto* func = builder_.GetInsertBlock()->getParent();

    auto* thenBB  = llvm::BasicBlock::Create(context_, "then", func);
    auto* elseBB  = llvm::BasicBlock::Create(context_, "else", func);
    auto* mergeBB = llvm::BasicBlock::Create(context_, "merge", func);

    builder_.CreateCondBr(condVal, thenBB, node.elseBlock ? elseBB : mergeBB);

    // Then block
    builder_.SetInsertPoint(thenBB);
    node.thenBlock->accept(*this);
    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(mergeBB);
    }

    // Else block
    if (node.elseBlock) {
        builder_.SetInsertPoint(elseBB);
        node.elseBlock->accept(*this);
        if (!builder_.GetInsertBlock()->getTerminator()) {
            builder_.CreateBr(mergeBB);
        }
    } else {
        // Empty else just falls through to merge
        builder_.SetInsertPoint(elseBB);
        builder_.CreateBr(mergeBB);
    }

    builder_.SetInsertPoint(mergeBB);
    return {};
}

std::any CodeGen::visit(WhileStmt& node) {
    auto* func = builder_.GetInsertBlock()->getParent();

    auto* headerBB = llvm::BasicBlock::Create(context_, "while.header", func);
    auto* bodyBB   = llvm::BasicBlock::Create(context_, "while.body", func);
    auto* exitBB   = llvm::BasicBlock::Create(context_, "while.exit", func);

    builder_.CreateBr(headerBB);

    // Header: evaluate condition
    builder_.SetInsertPoint(headerBB);
    auto condResult = node.condition->accept(*this);
    auto* condVal = getValue(condResult);
    builder_.CreateCondBr(condVal, bodyBB, exitBB);

    // Body
    builder_.SetInsertPoint(bodyBB);
    node.body->accept(*this);
    if (!builder_.GetInsertBlock()->getTerminator()) {
        builder_.CreateBr(headerBB); // back-edge
    }

    // Exit
    builder_.SetInsertPoint(exitBB);
    return {};
}

std::any CodeGen::visit(ForStmt& node) {
    auto* func = builder_.GetInsertBlock()->getParent();

    // Evaluate range bounds
    auto startResult = node.start->accept(*this);
    auto* startVal = getValue(startResult);
    auto endResult = node.end->accept(*this);
    auto* endVal = getValue(endResult);

    // Create loop variable
    auto* i64Ty = llvm::Type::getInt64Ty(context_);
    auto* loopVar = createEntryBlockAlloca(func, node.varName, i64Ty);
    builder_.CreateStore(startVal, loopVar);

    pushScope();
    declareVar(node.varName, loopVar, TypeSpec::makeInt());

    auto* headerBB = llvm::BasicBlock::Create(context_, "for.header", func);
    auto* bodyBB   = llvm::BasicBlock::Create(context_, "for.body", func);
    auto* exitBB   = llvm::BasicBlock::Create(context_, "for.exit", func);

    builder_.CreateBr(headerBB);

    // Header: check i < end
    builder_.SetInsertPoint(headerBB);
    auto* currentVal = builder_.CreateLoad(i64Ty, loopVar, node.varName);
    auto* cond = builder_.CreateICmpSLT(currentVal, endVal, "for.cond");
    builder_.CreateCondBr(cond, bodyBB, exitBB);

    // Body
    builder_.SetInsertPoint(bodyBB);
    node.body->accept(*this);

    // Increment
    if (!builder_.GetInsertBlock()->getTerminator()) {
        auto* current = builder_.CreateLoad(i64Ty, loopVar, node.varName);
        auto* next = builder_.CreateAdd(current,
            llvm::ConstantInt::get(i64Ty, 1), "for.inc");
        builder_.CreateStore(next, loopVar);
        builder_.CreateBr(headerBB);
    }

    // Exit
    builder_.SetInsertPoint(exitBB);
    popScope();
    return {};
}

std::any CodeGen::visit(ExprStmt& node) {
    node.expr->accept(*this);
    return {};
}

std::any CodeGen::visit(Block& node) {
    pushScope();
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
        if (builder_.GetInsertBlock()->getTerminator()) break;
    }
    popScope();
    return {};
}

// ── Expressions ─────────────────────────────────────────────────────────────

std::any CodeGen::visit(IntLit& node) {
    return static_cast<llvm::Value*>(
        llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), node.value, true));
}

std::any CodeGen::visit(FloatLit& node) {
    return static_cast<llvm::Value*>(
        llvm::ConstantFP::get(llvm::Type::getDoubleTy(context_), node.value));
}

std::any CodeGen::visit(BoolLit& node) {
    return static_cast<llvm::Value*>(
        llvm::ConstantInt::get(llvm::Type::getInt1Ty(context_), node.value ? 1 : 0));
}

std::any CodeGen::visit(StringLit& node) {
    return static_cast<llvm::Value*>(
        builder_.CreateGlobalStringPtr(node.value, "str"));
}

std::any CodeGen::visit(Identifier& node) {
    auto* var = lookupVar(node.name);
    if (!var) {
        diags_.error(node, "codegen: undefined variable '" + node.name + "'");
        return static_cast<llvm::Value*>(nullptr);
    }

    auto* loaded = builder_.CreateLoad(toLLVMType(var->type), var->alloca, node.name);
    return static_cast<llvm::Value*>(loaded);
}

std::any CodeGen::visit(ArrayLiteral& node) {
    // Array literals are handled during VarDecl — they store each element
    // into the alloca. Returning nullptr here; the parent VarDecl handles it.
    // For now, generate each element and store them.

    // We can't easily return an aggregate value, so this is handled specially
    // in VarDecl. If we get here standalone, just return null.
    return static_cast<llvm::Value*>(nullptr);
}

std::any CodeGen::visit(BinaryExpr& node) {
    auto leftResult = node.left->accept(*this);
    auto* lhs = getValue(leftResult);
    auto rightResult = node.right->accept(*this);
    auto* rhs = getValue(rightResult);

    if (!lhs || !rhs) return static_cast<llvm::Value*>(nullptr);

    // Determine if we're doing int or float ops
    bool isFloat = lhs->getType()->isDoubleTy();

    switch (node.op) {
        // Arithmetic
        case BinOp::Add:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFAdd(lhs, rhs, "fadd"));
            return static_cast<llvm::Value*>(builder_.CreateAdd(lhs, rhs, "add"));
        case BinOp::Sub:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFSub(lhs, rhs, "fsub"));
            return static_cast<llvm::Value*>(builder_.CreateSub(lhs, rhs, "sub"));
        case BinOp::Mul:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFMul(lhs, rhs, "fmul"));
            return static_cast<llvm::Value*>(builder_.CreateMul(lhs, rhs, "mul"));
        case BinOp::Div:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFDiv(lhs, rhs, "fdiv"));
            return static_cast<llvm::Value*>(builder_.CreateSDiv(lhs, rhs, "div"));
        case BinOp::Mod:
            return static_cast<llvm::Value*>(builder_.CreateSRem(lhs, rhs, "mod"));

        // Comparison
        case BinOp::Eq:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpOEQ(lhs, rhs, "feq"));
            return static_cast<llvm::Value*>(builder_.CreateICmpEQ(lhs, rhs, "eq"));
        case BinOp::Neq:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpONE(lhs, rhs, "fne"));
            return static_cast<llvm::Value*>(builder_.CreateICmpNE(lhs, rhs, "ne"));
        case BinOp::Lt:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpOLT(lhs, rhs, "flt"));
            return static_cast<llvm::Value*>(builder_.CreateICmpSLT(lhs, rhs, "lt"));
        case BinOp::Lte:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpOLE(lhs, rhs, "fle"));
            return static_cast<llvm::Value*>(builder_.CreateICmpSLE(lhs, rhs, "le"));
        case BinOp::Gt:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpOGT(lhs, rhs, "fgt"));
            return static_cast<llvm::Value*>(builder_.CreateICmpSGT(lhs, rhs, "gt"));
        case BinOp::Gte:
            if (isFloat) return static_cast<llvm::Value*>(builder_.CreateFCmpOGE(lhs, rhs, "fge"));
            return static_cast<llvm::Value*>(builder_.CreateICmpSGE(lhs, rhs, "ge"));

        // Logical
        case BinOp::And:
            return static_cast<llvm::Value*>(builder_.CreateAnd(lhs, rhs, "and"));
        case BinOp::Or:
            return static_cast<llvm::Value*>(builder_.CreateOr(lhs, rhs, "or"));
    }

    return static_cast<llvm::Value*>(nullptr);
}

std::any CodeGen::visit(UnaryExpr& node) {
    auto operandResult = node.operand->accept(*this);
    auto* operand = getValue(operandResult);
    if (!operand) return static_cast<llvm::Value*>(nullptr);

    switch (node.op) {
        case UnOp::Neg:
            if (operand->getType()->isDoubleTy()) {
                return static_cast<llvm::Value*>(builder_.CreateFNeg(operand, "fneg"));
            }
            return static_cast<llvm::Value*>(
                builder_.CreateNeg(operand, "neg"));
        case UnOp::Not:
            return static_cast<llvm::Value*>(
                builder_.CreateNot(operand, "not"));
    }

    return static_cast<llvm::Value*>(nullptr);
}

std::any CodeGen::visit(CallExpr& node) {
    auto* callee = module_->getFunction(node.callee);
    if (!callee) {
        // Also check our functions_ map
        auto it = functions_.find(node.callee);
        if (it != functions_.end()) {
            callee = it->second;
        }
    }

    if (!callee) {
        diags_.error(node, "codegen: undefined function '" + node.callee + "'");
        return static_cast<llvm::Value*>(nullptr);
    }

    std::vector<llvm::Value*> args;
    for (auto& arg : node.args) {
        auto result = arg->accept(*this);
        auto* val = getValue(result);
        if (val) args.push_back(val);
    }

    if (callee->getReturnType()->isVoidTy()) {
        builder_.CreateCall(callee, args);
        return static_cast<llvm::Value*>(nullptr);
    }

    return static_cast<llvm::Value*>(
        builder_.CreateCall(callee, args, "call"));
}

std::any CodeGen::visit(IndexExpr& node) {
    // Get the array's alloca
    auto* arrId = dynamic_cast<Identifier*>(node.array.get());
    if (!arrId) {
        diags_.error(node, "codegen: array indexing requires a variable");
        return static_cast<llvm::Value*>(nullptr);
    }

    auto* var = lookupVar(arrId->name);
    if (!var) {
        diags_.error(node, "codegen: undefined variable '" + arrId->name + "'");
        return static_cast<llvm::Value*>(nullptr);
    }

    auto indexResult = node.index->accept(*this);
    auto* indexVal = getValue(indexResult);

    auto* zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 0);
    auto* elemPtr = builder_.CreateGEP(
        toLLVMType(var->type), var->alloca, {zero, indexVal}, "elemptr");

    llvm::Type* elemLLVMTy = toLLVMType(*var->type.elementType);
    return static_cast<llvm::Value*>(
        builder_.CreateLoad(elemLLVMTy, elemPtr, "elem"));
}

// ── Optimization Passes ─────────────────────────────────────────────────────

void CodeGen::runOptimizations(OptLevel level) {
    // Create the analysis managers
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    // Create the pass builder and register analyses
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    // Map our OptLevel to LLVM's
    llvm::OptimizationLevel llvmLevel;
    switch (level) {
        case OptLevel::O1: llvmLevel = llvm::OptimizationLevel::O1; break;
        case OptLevel::O2: llvmLevel = llvm::OptimizationLevel::O2; break;
        case OptLevel::O3: llvmLevel = llvm::OptimizationLevel::O3; break;
        default:           llvmLevel = llvm::OptimizationLevel::O0; break;
    }

    // Build and run the standard optimization pipeline
    auto MPM = PB.buildPerModuleDefaultPipeline(llvmLevel);
    MPM.run(*module_, MAM);
}

} // namespace tiny
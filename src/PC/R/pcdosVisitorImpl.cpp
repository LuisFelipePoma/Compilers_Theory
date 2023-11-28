#include "pcdosVisitorImpl.h"

#include <any>
#include <llvm-17/llvm/IR/Constant.h>
#include <llvm-17/llvm/Support/raw_ostream.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <typeinfo>

void pcdosVisitorImpl::IRFunctionSysDecl(const char *nameFunction, std::vector<llvm::Type *> argTy, bool isVar)
{
	std::vector<llvm::Type *> argTypes = argTy; // Argument type is char*
												// Declare C standard library printf

	llvm::FunctionType *funcType = llvm::FunctionType::get(
		int32Type, // Return type is int
		argTypes,  // Argument types
		isVar	   // Is vararg?
	);

	llvm::Function *func = llvm::Function::Create(
		funcType,
		llvm::Function::ExternalLinkage,
		nameFunction, // Function name
		module.get()  // Get the raw pointer
	);
}

std::any pcdosVisitorImpl::visitProg(pcdosParser::ProgContext *ctx)
{
	int8Type = llvm::Type::getInt8Ty(*context);
	int32Type = llvm::Type::getInt32Ty(*context);
	charPtrType = llvm::PointerType::get(int8Type, 0);

	// Creates the sys call functions
	IRFunctionSysDecl("puts", {charPtrType}, false);

	// Creates the main Function
	std::vector<llvm::Type *> Doubles(0,
									  llvm::Type::getDoubleTy(*context));
	llvm::FunctionType *FT = llvm::FunctionType::get(
		llvm::Type::getVoidTy(*context), Doubles, false);

	F = llvm::Function::Create(
		FT, llvm::Function::ExternalLinkage, "_anon_", module.get());

	// Creates the entry to the function
	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*context, "entry", F);
	builder->SetInsertPoint(BB);

	// visit the body
	visitChildren(ctx);

	// Close the main Function
	builder->CreateRet(nullptr);

	return std::any(nullptr);
}

std::any pcdosVisitorImpl::visitPrintExpr(pcdosParser::PrintExprContext *ctx)
{
	llvm::Value *value = std::any_cast<llvm::Value *>(visit(ctx->expr()));
	value->print(llvm::errs(), false);

	llvm::FunctionCallee putsFunc = module->getOrInsertFunction("puts", llvm::Type::getInt32Ty(*context), llvm::Type::getInt8PtrTy(*context));
	llvm::CallInst *result = builder->CreateCall(putsFunc, {builder->CreateGlobalStringPtr(value->getName())}, "callSystem");

	return std::any();
}

std::any pcdosVisitorImpl::visitAssign(pcdosParser::AssignContext *ctx)
{
	std::string id = std::any_cast<std::string>(ctx->ID()->getText());

	auto value = visit(ctx->expr());

	if (value.type() == typeid(llvm::Value *))
	{
		llvm::AllocaInst *allocaInst = CreateEntryBlockAlloca(id);
		llvm::Value *valueToStore = std::any_cast<llvm::Value *>(value);
		builder->CreateStore(valueToStore, allocaInst);
		memory[id] = valueToStore;
	}

	return std::any(nullptr);
}

std::any pcdosVisitorImpl::visitStatdef(pcdosParser::StatdefContext *ctx)
{
	return visitChildren(ctx);
}

std::any
pcdosVisitorImpl::visitStatextern(pcdosParser::StatexternContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitBlank(pcdosParser::BlankContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitCall(pcdosParser::CallContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitNumber(pcdosParser::NumberContext *ctx)
{
	auto numVal = std::stod(ctx->NUMBER()->getText());
	llvm::Value *val = llvm::ConstantFP::get(*context, llvm::APFloat(numVal));
	return std::any(val);
}

std::any pcdosVisitorImpl::visitMulDiv(pcdosParser::MulDivContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitAddSub(pcdosParser::AddSubContext *ctx)
{
	llvm::Value *L = std::any_cast<llvm::Value *>(visit(ctx->expr(0)));
	llvm::Value *R = std::any_cast<llvm::Value *>(visit(ctx->expr(1)));
	if (ctx->op->getType() == pcdosParser::ADD)
	{
		return std::any(builder->CreateFAdd(L, R, "addTemp"));
	}
	else
	{
		return std::any(builder->CreateFSub(L, R, "subTemp"));
	}
}

std::any pcdosVisitorImpl::visitParens(pcdosParser::ParensContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitId(pcdosParser::IdContext *ctx)
{
	std::string idName = ctx->ID()->getText();
	// Verify if exists
	if (memory.find(idName) != memory.end())
		return std::any(memory[idName]);
	return std::any(idName);
}

std::any pcdosVisitorImpl::visitProto(pcdosParser::ProtoContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitDef(pcdosParser::DefContext *ctx)
{
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitExtern(pcdosParser::ExternContext *ctx)
{
	return visitChildren(ctx);
}
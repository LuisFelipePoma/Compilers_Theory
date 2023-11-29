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
#include <string>
#include <typeinfo>
#include <vector>

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

void pcdosVisitorImpl::goToMain()
{
	// Back to the def
	auto mainFunc = module->getFunction("_main_");
	// Get the entry block of the main function
	llvm::BasicBlock &entry = mainFunc->getEntryBlock();

	// Set the builder's insertion point to the entry block of main
	builder->SetInsertPoint(&entry);
}

llvm::Value *pcdosVisitorImpl::CreateFloatV(double number)
{
	llvm::Value *val = llvm::ConstantFP::get(*context, llvm::APFloat(number));

	return val;
}

//------------------------------------------------------------------------------------------------------------------------

std::any pcdosVisitorImpl::visitProg(pcdosParser::ProgContext *ctx)
{
	int8Type = llvm::Type::getInt8Ty(*context);
	int32Type = llvm::Type::getInt32Ty(*context);
	charPtrType = llvm::PointerType::get(int8Type, 0);
	typeDouble = llvm::Type::getDoubleTy(*context);

	// Creates the sys call functions
	IRFunctionSysDecl("puts", {charPtrType}, false);

	// Creates the main Function
	std::vector<llvm::Type *> Doubles(0,
									  llvm::Type::getDoubleTy(*context));
	llvm::FunctionType *FT = llvm::FunctionType::get(
		llvm::Type::getVoidTy(*context), Doubles, false);

	F = llvm::Function::Create(
		FT, llvm::Function::ExternalLinkage, "_main_", module.get());

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

// ID '(' expr* ')'         # Call
std::any pcdosVisitorImpl::visitCall(pcdosParser::CallContext *ctx)
{
	std::string nameFn = ctx->ID()->getText();

	// verify if exists
	if (memory.find(nameFn) == memory.end())
	{
		// throw error
		std::cout << "Function not found Cabezon\n";
		return std::any(CreateFloatV(1));
	}

	llvm::Function *F = module->getFunction(nameFn);
	int sizeArgs = F->arg_size();

	if (sizeArgs != ctx->expr().size())
	{
		std::cout << "Bad Params Cabezon\n";
		return std::any(CreateFloatV(1));
	}

	std::vector<llvm::Value *> valuesV;
	for (auto V : ctx->expr())
	{
		valuesV.push_back(std::any_cast<llvm::Value *>(visit(V)));
	}

	llvm::CallInst *result = builder->CreateCall(F, valuesV, "callFunction");

	return std::any(CreateFloatV(0));
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

// : ID '(' ID* ')'           # Proto
std::any pcdosVisitorImpl::visitProto(pcdosParser::ProtoContext *ctx)
{
	std::vector<std::string> args;

	for (auto id : ctx->ID())
	{
		args.push_back(id->getText());
	}
	return std::any(args);
}

// : 'def' prototype expr     # Def
std::any pcdosVisitorImpl::visitDef(pcdosParser::DefContext *ctx)
{
	std::vector<std::string> args = std::any_cast<std::vector<std::string>>(visit(ctx->prototype()));
	std::string function = args[0];

	// verify if exists
	if (argsFn.find(function) != argsFn.end())
	{
		// throw error
		return std::any(nullptr);
	}
	args.erase(args.begin());

	// Save in memory
	memory.insert({function, args});

	// Create Function
	std::vector<llvm::Type *> Doubles(args.size(), llvm::Type::getDoubleTy(*context));

	// Function Args
	llvm::FunctionType *functionTypes = llvm::FunctionType::get(
		llvm::Type::getVoidTy(*context),
		Doubles,
		false);

	// Function Declaration
	llvm::Function *F = llvm::Function::Create(
		functionTypes, llvm::Function::ExternalLinkage, function, module.get());

	// Entry
	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*context, "entry", F);
	builder->SetInsertPoint(BB);

	// Body
	//  Set name for arguments
	unsigned Idx = 0;
	// std::map<std::string, std::any> values;
	for (auto &Arg : F->args())
	{
		std::string argName = args.at(Idx++);
		Arg.setName(argName);
		memory.insert({argName, dynamic_cast<llvm::Value *>(&Arg)});
	}

	// Visit expr
	visit(ctx->stat());

	// Clean the memory from the args
	for (auto arg : args)
	{
		memory.erase(arg);
	}

	// create return
	builder->CreateRet(nullptr);

	// back to MAIN
	goToMain();

	return std::any(nullptr);
}

std::any pcdosVisitorImpl::visitExtern(pcdosParser::ExternContext *ctx)
{
	return visitChildren(ctx);
}
#include <llvm-17/llvm/ADT/APInt.h>
#include <llvm-17/llvm/IR/Value.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>
#include <llvm/Support/TargetSelect.h>

static auto TheContext = std::make_unique<llvm::LLVMContext>();
static auto TheModule = std::make_unique<llvm::Module>("Mi cool jit", *TheContext);
static auto TheBuilder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

void createFunction(const char *name, int sizeArgs, llvm::Type *, llvm::Value *);

int main()
{
	// Types
	auto typeDouble = llvm::Type::getDoubleTy(*TheContext);
	auto typeInt = llvm::Type::getInt8Ty(*TheContext);
	auto typeVoid = llvm::Type::getVoidTy(*TheContext);

	// Values
	llvm::Value *valInt = llvm::ConstantInt::get(*TheContext, llvm::APInt(8, 1));
	llvm::Value *valDouble = llvm::ConstantFP::get(*TheContext, llvm::APFloat(1.2));

	// init
	createFunction("_main_void_", 0, typeVoid, nullptr);
	createFunction("_int_", 0, typeInt, valInt);
	createFunction("_double_", 0, typeDouble, valDouble);
	createFunction("_doubleArgs_", 2, typeDouble, valDouble);

	// NO andre no!
	TheModule->print(llvm::errs(), nullptr);

	return 0;
}

void createFunction(const char *name, int sizeArgs, llvm::Type *types, llvm::Value *value)
{
	// Types
	std::vector<llvm::Type *> Doubles(sizeArgs, llvm::Type::getDoubleTy(*TheContext));

	// Function Args
	llvm::FunctionType *functionTypes = llvm::FunctionType::get(
		types,
		Doubles,
		false);

	// Function Declaration
	llvm::Function *F = llvm::Function::Create(
		functionTypes, llvm::Function::ExternalLinkage, name, TheModule.get());

	// Entry
	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
	TheBuilder->SetInsertPoint(BB);

	// Body
	//  Set name for arguments
	unsigned Idx = 0;
	std::vector<llvm::Value *> values;
	for (auto &Arg : F->args())
	{
		Arg.setName("arg" + std::to_string(Idx++));
		values.push_back(&Arg);
	}

	if (sizeArgs >= 2)
	{
		llvm::IRBuilder<> builder(*TheContext);
		llvm::Value *sum = builder.CreateAdd(values[0], values[1], "sum");

		TheBuilder->CreateRet(sum);

		return;
	}

	// Return value
	TheBuilder->CreateRet(value);
}

// void callFunction(const char *nameFunction)
// {
// 	llvm::Function *executeCommandFunction = TheModule->getFunction(nameFunction);
// 	llvm::CallInst *result = TheBuilder->CreateCall(executeCommandFunction, {}, "calltmp");
// }
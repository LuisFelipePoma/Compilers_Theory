#include <cstddef>
#include <llvm-15/llvm/IR/BasicBlock.h>
#include <llvm-15/llvm/IR/DerivedTypes.h>
#include <llvm-15/llvm/IR/IRBuilder.h>
#include <llvm-15/llvm/IR/Intrinsics.h>
#include <llvm-15/llvm/IR/LLVMContext.h>
#include <llvm-15/llvm/IR/Module.h>
#include <llvm-15/llvm/IR/Type.h>
#include <memory>
#include <vector>

using namespace llvm;

int main()
{
	auto TheContext = std::make_unique<llvm::LLVMContext>();
	auto TheModule = std::make_unique<Module>("Mi cool jit", *TheContext);
	auto TheBuilder = std::make_unique<IRBuilder<>>(*TheContext);

	Type *Tys[] = {Type::getInt8PtrTy(*TheContext), Type::getInt32Ty(*TheContext)};
	Function *memset_func = Intrinsic::getDeclaration(TheModule.get(), Intrinsic::memset, Tys);

	auto getchar_func = TheModule->getOrInsertFunction("getchar", IntegerType::getInt32Ty(*TheContext));

	std::vector<Type *> Doubles(0, Type::getDoubleTy(*TheContext));
	FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
	Function *Func = Function::Create(FT, Function::ExternalLinkage, "func", TheModule.get());

	BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", Func);
	TheBuilder->SetInsertPoint(BB);

	TheModule->print(errs(), nullptr);

	return 0;
}
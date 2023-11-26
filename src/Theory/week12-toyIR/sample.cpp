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
#include <iostream>
#include <llvm/Support/TargetSelect.h>

int main()
{
	// InitializeNativeTarget();
	// InitializeNativeTargetAsmPrinter();
	auto TheContext = std::make_unique<llvm::LLVMContext>();
	auto TheModule = std::make_unique<llvm::Module>("Mi cool jit", *TheContext);
	auto TheBuilder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

	std::cout << "visitPrintExpr\n";
	std::vector<double> Args;
	std::vector<llvm::Type *> Doubles(Args.size(),
									  llvm::Type::getDoubleTy(*TheContext));
	llvm::FunctionType *FT = llvm::FunctionType::get(
		llvm::Type::getDoubleTy(*TheContext), Doubles, false);

	llvm::Function *F = llvm::Function::Create(
		FT, llvm::Function::ExternalLinkage, "_anon_", TheModule.get());

	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
	TheBuilder->SetInsertPoint(BB);

	auto numVal = 10.0;
	llvm::Value *val = llvm::ConstantFP::get(*TheContext, llvm::APFloat(numVal));
	
	TheBuilder->CreateRet(val);

	TheModule->print(llvm::errs(), nullptr);
	return 0;
}
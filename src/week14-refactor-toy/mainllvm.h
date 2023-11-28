#ifndef __MAIN_H__
#define __MAIN_H__

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <string>

class MainLLVM
{
private:
	// variables
	std::unique_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;
	std::unique_ptr<llvm::IRBuilder<>> builder;

	// functions
	void moduleInit();
	void saveModuleToFile(const std::string &filename);

public:
	MainLLVM();
	void exec(const std::string &program);
};

#endif
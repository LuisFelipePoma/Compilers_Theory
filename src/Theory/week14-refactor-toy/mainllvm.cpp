#include "mainllvm.h"
#include <llvm-15/llvm/IR/LLVMContext.h>
#include <llvm-15/llvm/IR/Module.h>
#include <llvm-15/llvm/Support/raw_ostream.h>
#include <memory>
#include <string>
#include <system_error>

// private
void MainLLVM::moduleInit()
{
	context = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>("MyExample", *context);
	builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

void MainLLVM::saveModuleToFile(const std::string &fileName)
{
	std::error_code errorCode;
	llvm::raw_fd_stream outLL(fileName, errorCode);
	module->print(outLL, nullptr);
}

// public
MainLLVM::MainLLVM()
{
	moduleInit();
}

void MainLLVM::exec(const std::string &program)
{
	saveModuleToFile("./out.ll");
}

//------------------------ MAIN --------------------------------------//
int main(int argc, char const *argv[])
{
	std::string program = R"(
		42
	)";

	MainLLVM vm;
	vm.exec(program);

	return 0;
}
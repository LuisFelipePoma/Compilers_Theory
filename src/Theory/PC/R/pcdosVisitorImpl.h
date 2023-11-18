// #pragma once

#ifndef __PCDOS_VISITOR_IMPL
#define __PCDOS_VISITOR_IMPL

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include <algorithm>
#include <any>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <system_error>
// #include "pcdosBaseVisitor.h"

class pcdosVisitorImpl
{
public:
		pcdosVisitorImpl()
				: context(std::make_unique<llvm::LLVMContext>()),
					module(std::make_unique<llvm::Module>("pcdos", *context)),
					builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}

	void test()
	{
		std::error_code error;
		llvm::raw_fd_stream outLL("test.ll", error);
		module->print(outLL, nullptr);
	}

private:
	std::map<std::string, std::any> memory;
	std::unique_ptr<llvm::LLVMContext> context;
	std::unique_ptr<llvm::Module> module;
	std::unique_ptr<llvm::IRBuilder<>> builder;

public:
};

#endif
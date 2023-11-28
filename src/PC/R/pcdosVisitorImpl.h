#ifndef __PCDOS_VISITOR_IMPL__
#define __PCDOS_VISITOR_IMPL__

#include "libs/pcdosBaseVisitor.h"
#include "libs/pcdosParser.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

#include <algorithm>
#include <any>
#include <iostream>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <memory>
#include <string>
#include <system_error>

/* added the Impl at the end of the class to avoid it being .gitignored sorry */
class pcdosVisitorImpl : pcdosBaseVisitor
{
public:
	pcdosVisitorImpl()
		: context(std::make_unique<llvm::LLVMContext>()),
		  module(std::make_unique<llvm::Module>("LaPC2", *context)),
		  builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}

	virtual std::any visitProg(pcdosParser::ProgContext *ctx) override;
	virtual std::any visitPrintExpr(pcdosParser::PrintExprContext *ctx) override;
	virtual std::any visitAssign(pcdosParser::AssignContext *ctx) override;
	virtual std::any visitStatdef(pcdosParser::StatdefContext *ctx) override;
	virtual std::any
	visitStatextern(pcdosParser::StatexternContext *ctx) override;
	virtual std::any visitBlank(pcdosParser::BlankContext *ctx) override;
	virtual std::any visitCall(pcdosParser::CallContext *ctx) override;
	virtual std::any visitNumber(pcdosParser::NumberContext *ctx) override;
	virtual std::any visitMulDiv(pcdosParser::MulDivContext *ctx) override;
	virtual std::any visitAddSub(pcdosParser::AddSubContext *ctx) override;
	virtual std::any visitParens(pcdosParser::ParensContext *ctx) override;
	virtual std::any visitId(pcdosParser::IdContext *ctx) override;
	virtual std::any visitProto(pcdosParser::ProtoContext *ctx) override;
	virtual std::any visitDef(pcdosParser::DefContext *ctx) override;
	virtual std::any visitExtern(pcdosParser::ExternContext *ctx) override;
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

	// aux
	llvm::Function *F;

	// aux methods
	llvm::AllocaInst *CreateEntryBlockAlloca(llvm::StringRef varName)
	{
		llvm::IRBuilder<> TmpB(&F->getEntryBlock(), F->getEntryBlock().begin());
		return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*context), nullptr,
								 varName);
	}
};

#endif

// vim: set ts=2:sw=2:et:sts=2:
#include "pcdosVisitorImpl.h"
#include "libs/pcdosParser.h"
#include <any>
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
#include <memory>
#include <string>

std::any pcdosVisitorImpl::visitProg(pcdosParser::ProgContext *ctx)
{
	std::cout << "visitProg\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitPrintExpr(pcdosParser::PrintExprContext *ctx)
{
	std::cout << "visitPrintExpr\n";
	std::vector<double> Args;
	std::vector<llvm::Type *> Doubles(Args.size(),
									  llvm::Type::getDoubleTy(*context));
	llvm::FunctionType *FT = llvm::FunctionType::get(
		llvm::Type::getDoubleTy(*context), Doubles, false);

	F = llvm::Function::Create(
		FT, llvm::Function::ExternalLinkage, "_anon_", module.get());

	llvm::BasicBlock *BB = llvm::BasicBlock::Create(*context, "entry", F);
	builder->SetInsertPoint(BB);
	llvm::Value *val = std::any_cast<llvm::Value *>(visit(ctx->expr()));
	builder->CreateRet(val);

	return std::any();
	// return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitAssign(pcdosParser::AssignContext *ctx)
{
	std::cout << "visitAssign\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitStatdef(pcdosParser::StatdefContext *ctx)
{
	std::cout << "visitStatdef\n";
	return visitChildren(ctx);
}

std::any
pcdosVisitorImpl::visitStatextern(pcdosParser::StatexternContext *ctx)
{
	std::cout << "visitStatextern\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitBlank(pcdosParser::BlankContext *ctx)
{
	std::cout << "visitBlank\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitCall(pcdosParser::CallContext *ctx)
{
	std::cout << "visitCall\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitNumber(pcdosParser::NumberContext *ctx)
{
	std::cout << "visitNumber\n";
	auto numVal = std::stod(ctx->NUMBER()->getText());
	llvm::Value *val = llvm::ConstantFP::get(*context, llvm::APFloat(numVal));
	return std::any(val);
	// return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitMulDiv(pcdosParser::MulDivContext *ctx)
{
	std::cout << "visitMulDiv\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitAddSub(pcdosParser::AddSubContext *ctx)
{
	std::cout << "visitAddSub\n";
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
	// return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitParens(pcdosParser::ParensContext *ctx)
{
	std::cout << "visitParens\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitId(pcdosParser::IdContext *ctx)
{
	std::cout << "visitId\n";
	std::string idName = ctx->ID()->getText();
	llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(idName);
	memory[idName] = Alloca;
	return std::any(Alloca);
	// return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitProto(pcdosParser::ProtoContext *ctx)
{
	std::cout << "visitProto\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitDef(pcdosParser::DefContext *ctx)
{
	std::cout << "visitDef\n";
	return visitChildren(ctx);
}

std::any pcdosVisitorImpl::visitExtern(pcdosParser::ExternContext *ctx)
{
	std::cout << "visitExtern\n";
	return visitChildren(ctx);
}

// vim: set ts=2:sw=2:et:sts=2:
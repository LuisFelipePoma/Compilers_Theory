// Libraries for LLVM-15
#include <cctype>
#include <cstdio>
#include <cstdlib>

#include "llvm-15/llvm/ADT/APFloat.h"
#include "llvm-15/llvm/IR/BasicBlock.h"
#include "llvm-15/llvm/IR/Constants.h"
#include "llvm-15/llvm/IR/DerivedTypes.h"
#include "llvm-15/llvm/IR/Function.h"
#include "llvm-15/llvm/IR/IRBuilder.h"
#include "llvm-15/llvm/IR/LLVMContext.h"
#include "llvm-15/llvm/IR/Type.h"
#include "llvm-15/llvm/IR/Value.h"
#include "llvm-15/llvm/IR/Verifier.h"

// Libraries for Antlr4
#include "libs/ExprBaseVisitor.h"
#include "libs/ExprParser.h"

// Libraries for the program
#include <any>
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

class EvalVisitorImpl : public ExprBaseVisitor
{
	std::map<std::string, Value *> memory;

	std::unique_ptr<LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
	std::unique_ptr<Module> TheModule = std::make_unique<Module>("Mi cool pc2", *TheContext);
	std::unique_ptr<IRBuilder<>> Builder = std::make_unique<IRBuilder<>>(*TheContext);

	Value *LogErrorV(const char *Str)
	{
		LogError(Str);
		return nullptr;
	}

	Value *LogError(const char *Str)
	{
		fprintf(stderr, "Error: %s\n", Str);
		return nullptr;
	}

public:
	// program: statlist EOF;
	std::any visitProgram(ExprParser::ProgramContext *ctx)
	{
		// Initialize the LLVM module and generate code for each statement
		TheModule = std::make_unique<Module>("Mi cool pc2", *TheContext);
		std::cout << "Entrando..\n";
		visitChildren(ctx); // Assuming statlist() returns a single StatlistContext object
		std::cout << "Saliendo..\n";
		// Print the generated LLVM module
		TheModule->print(errs(), nullptr);

		return nullptr;
	}

	// initFunc: DEF ID '(' ID (',' ID)* ')' '{' stat+ '}' # BodyFunction;
	std::any visitBodyFunction(ExprParser::BodyFunctionContext *ctx)
	{
		std::cout << "initFunc..\n";

		// Get function information
		std::string funcName = ctx->ID(0)->getText();
		std::string argName = ctx->ID(1)->getText();

		// Create a function type with one argument of type double
		std::vector<Type *> Doubles(1, Type::getDoubleTy(*TheContext));
		FunctionType *FT = FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);

		// Create a new function with the specified name and type in the module
		Function *F = Function::Create(FT, Function::ExternalLinkage, funcName, TheModule.get());

		// Set names for all arguments
		F->arg_begin()->setName(argName);

		// Create a new basic block to start insertion into
		BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", F);
		Builder->SetInsertPoint(BB);

		// Assuming the function body is a single expression
		Value *RetVal = std::any_cast<Value *>(visit(ctx->stat(0)));

		// Add a return statement
		Builder->CreateRet(RetVal);

		// Verify the function
		verifyFunction(*F);

		return F;
	}

	// expr op=('+' | '-') expr	# SumExpr
	std::any visitSumExpr(ExprParser::SumExprContext *ctx)
	{
		std::cout << "visitSumExpr..\n";

		Value *L = std::any_cast<Value *>(visit(ctx->expr(0)));
		Value *R = std::any_cast<Value *>(visit(ctx->expr(1)));

		switch (ctx->op->getType())
		{
		case ExprParser::ADD:
			return Builder->CreateFAdd(L, R, "addtmp");
		case ExprParser::SUB:
			return Builder->CreateFSub(L, R, "subtmp");
		default:
			return LogErrorV("Unknown operator");
		}
	}

	// | expr op=('>' | '<') expr	# CompExpr
	std::any visitCompExpr(ExprParser::CompExprContext *ctx)
	{
		std::cout << "visitCompExpr..\n";

		Value *L = std::any_cast<Value *>(visit(ctx->expr(0)));
		Value *R = std::any_cast<Value *>(visit(ctx->expr(1)));

		switch (ctx->op->getType())
		{
		case ExprParser::COMP:
			L = Builder->CreateFCmpULT(L, R, "cmptmp");
			return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
		default:
			return LogErrorV("invalid binary operator");
		}
	}

	// ID '=' expr # Assign
	std::any visitAssign(ExprParser::AssignContext *ctx)
	{
		std::cout << "visitAssign..\n";

		// Obtener información sobre la asignación
		auto varName = ctx->ID()->getText();
		Value *val = std::any_cast<Value *>(visit(ctx->expr()));

		// Verificar si la variable ya está en el mapa de memoria
		memory[varName] = val;

		return nullptr;
	}

	// func: ID '(' expr ')' # FuncBody;
	std::any visitFuncBody(ExprParser::FuncBodyContext *ctx)
	{
		std::cout << "visitFuncBody..\n";

		if (!ctx->ID() || !ctx->expr())
			return LogErrorV("Invalid function body context");

		std::string funcName = ctx->ID()->getText();
		std::any exprResult = visit(ctx->expr());
		if (!exprResult.has_value())
			return LogErrorV("Invalid expression result");

		Value *argValue = std::any_cast<Value *>(exprResult);

		Function *CalleeF = TheModule->getFunction(funcName);
		if (!CalleeF)
			return LogErrorV("Unknown function referenced");

		if (CalleeF->arg_size() != 1)
			return LogErrorV("Function should take one argument");

		std::vector<Value *> ArgsV{argValue};
		return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
	}

	// 	| id					# IdExpr;
	std::any visitIdExpr(ExprParser::IdExprContext *ctx)
	{
		std::cout << "visitIdExpr..\n";

		if (!ctx->id())
			return LogErrorV("Invalid ID context");

		auto id = ctx->id()->getText();
		if (memory.find(id) == memory.end())
			return LogErrorV("Unknown variable referenced");

		Value *varValue = memory[id];
		if (!varValue)
			return LogErrorV("Invalid variable value");

		return std::any(id);
	}
};

#include "grammar/Parser.h"

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//
static void InitializeModule();
static void HandleDefinition();
static void HandleTopLevelExpression();
static void HandleExtern();
static void MainLoop();

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
	// Install standard binary operators.
	// 1 is lowest precedence.
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40; // highest.

	// Prime the first token.
	fprintf(stderr, "ready> ");
	getNextToken();

	// Make the module, which holds all the code.
	InitializeModule();

	// Run the main "interpreter loop" now.
	MainLoop();

	// Print out all of the generated code.
	TheModule->print(llvm::errs(), nullptr);

	return 0;
}

// FUNCTIONS

static void InitializeModule()
{
	// Open a new context and module.
	TheContext = std::make_unique<llvm::LLVMContext>();
	TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);

	// Create a new builder for the module.
	Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

static void HandleDefinition()
{
	if (auto FnAST = ParseDefinition())
	{
		if (auto *FnIR = FnAST->codegen())
		{
			fprintf(stderr, "Read function definition:");
			FnIR->print(llvm::errs());
			fprintf(stderr, "\n");
		}
	}
	else
	{
		// Skip token for error recovery.
		getNextToken();
	}
}

static void HandleExtern()
{
	if (auto ProtoAST = ParseExtern())
	{
		if (auto *FnIR = ProtoAST->codegen())
		{
			fprintf(stderr, "Read extern: ");
			FnIR->print(llvm::errs());
			fprintf(stderr, "\n");
		}
	}
	else
	{
		// Skip token for error recovery.
		getNextToken();
	}
}

static void HandleTopLevelExpression()
{
	// Evaluate a top-level expression into an anonymous function.
	if (auto FnAST = ParseTopLevelExpr())
	{
		if (auto *FnIR = FnAST->codegen())
		{
			fprintf(stderr, "Read top-level expression:");
			FnIR->print(llvm::errs());
			fprintf(stderr, "\n");

			// Remove the anonymous expression.
			FnIR->eraseFromParent();
		}
	}
	else
	{
		// Skip token for error recovery.
		getNextToken();
	}
}

static void MainLoop()
{
	while (true)
	{
		fprintf(stderr, "ready> ");
		switch (CurTok)
		{
		case tok_eof:
			return;
		case ';': // ignore top-level semicolons.
			getNextToken();
			break;
		case tok_def:
			HandleDefinition();
			break;
		case tok_extern:
			HandleExtern();
			break;
		default:
			HandleTopLevelExpression();
			break;
		}
	}
}
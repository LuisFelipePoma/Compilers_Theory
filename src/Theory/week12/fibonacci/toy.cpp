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

#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

enum Token
{
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,

	tok_identifier = -4,
	tok_number = -5,
};

static std::string IdentifierStr;
static double NumVal;

// ------------------------> Lexer Analyzer
static int gettok()
{
	static int LastChar = ' ';

	while (isspace(LastChar))
		LastChar = getchar();

	if (isalpha(LastChar))
	{
		IdentifierStr = LastChar;
		while (isalnum((LastChar = getchar())))
		{
			IdentifierStr += LastChar;
		}
		if (IdentifierStr == "def")
			return tok_def;
		if (IdentifierStr == "extern")
			return tok_extern;

		return tok_identifier;
	}

	if (isdigit(LastChar) || LastChar == '.')
	{
		std::string NumStr;
		do
		{
			NumStr += LastChar;
			LastChar = getchar();
		} while (isdigit(LastChar) || LastChar == '.');
		NumVal = strtod(NumStr.c_str(), 0);

		return tok_number;
	}

	if (LastChar == '#')
	{
		do
		{
			LastChar = getchar();
		} while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return gettok();
	}

	if (LastChar == EOF)
		return tok_eof;

	int ThisChar = LastChar;
	LastChar = getchar();

	return ThisChar;
}

namespace
{

	class ExprAST
	{
	public:
		virtual ~ExprAST() = default;
		virtual Value *codegen() = 0;
	};

	class NumberExprAST : public ExprAST
	{
		double Val;

	public:
		NumberExprAST(double Val) : Val(Val) {}
		Value *codegen() override;
	};

	class VariableExprAST : public ExprAST
	{
		std::string Name;

	public:
		VariableExprAST(const std::string &Name) : Name(Name) {}
		Value *codegen() override;
	};

	class BinaryExprAST : public ExprAST
	{
		char Op;
		std::unique_ptr<ExprAST> LHS, RHS;

	public:
		BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
					  std::unique_ptr<ExprAST> RHS)
			: Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
		Value *codegen() override;
	};

	class CallExprAST : public ExprAST
	{
		std::string Callee;
		std::vector<std::unique_ptr<ExprAST>> Args;

	public:
		CallExprAST(const std::string &Callee,
					std::vector<std::unique_ptr<ExprAST>> Args)
			: Callee(Callee), Args(std::move(Args)) {}
		Value *codegen() override;
	};

	class PrototypeAST
	{
		std::string Name;
		std::vector<std::string> Args;

	public:
		PrototypeAST(const std::string &Name, std::vector<std::string> Args)
			: Name(Name), Args(std::move(Args)) {}

		Function *codegen();
		const std::string &getName() const { return Name; }
	};

	class FunctionAST
	{
		std::unique_ptr<PrototypeAST> Proto;
		std::unique_ptr<ExprAST> Body;

	public:
		FunctionAST(std::unique_ptr<PrototypeAST> Proto,
					std::unique_ptr<ExprAST> Body)
			: Proto(std::move(Proto)), Body(std::move(Body)) {}
		Function *codegen();
	};

} // namespace

// ------------------------> Syntax Analyzer
static int CurTok;
static int getNextToken() { return CurTok = gettok(); }

static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence()
{
	if (!isascii(CurTok))
		return -1;

	int TokPrec = BinopPrecedence[CurTok];
	if (TokPrec <= 0)
		return -1;

	return TokPrec;
}

std::unique_ptr<ExprAST> LogError(const char *Str)
{
	fprintf(stderr, "Error: %s\n", Str);
	return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
	LogError(Str);
	return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
	auto Result = std::make_unique<NumberExprAST>(NumVal);
	getNextToken();
	return std::move(Result);
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr()
{
	getNextToken();
	auto V = ParseExpression();
	if (!V)
		return nullptr;
	if (CurTok != ')')
		return LogError("expected ')'");

	getNextToken();
	return V;
}

// identifierExpr
//     ::= identifier
//     ::= identifier '(' (E | expression (',' expression)*) ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
	std::string IdName = IdentifierStr;

	getNextToken();

	if (CurTok != '(')
		return std::make_unique<VariableExprAST>(IdName);

	getNextToken();
	std::vector<std::unique_ptr<ExprAST>> Args;
	if (CurTok != ')')
	{
		while (true)
		{
			if (auto Arg = ParseExpression())
				Args.push_back(std::move(Arg));
			else
				return nullptr;

			if (CurTok == ')')
				break;

			if (CurTok != ',')
				return LogError("Expected ')' ru ',' in argument list");
			getNextToken();
		}
	}
	getNextToken();
	return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// primary
//     ::= identifierexpr
//     ::= numberexpr
//     ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary()
{
	switch (CurTok)
	{
	case tok_identifier:
		return ParseIdentifierExpr();
	case tok_number:
		return ParseNumberExpr();
	case '(':
		return ParseParenExpr();
	default:
		return LogError("unknown token when expecting an expression");
	}
}

// binoprhs ::= (op primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
											  std::unique_ptr<ExprAST> LHS)
{
	while (true)
	{
		int TokPrec = GetTokPrecedence();

		if (TokPrec < ExprPrec)
			return LHS;

		int BinOp = CurTok;
		getNextToken();

		auto RHS = ParsePrimary();
		if (!RHS)
			return nullptr;

		int NextPrec = GetTokPrecedence();
		if (TokPrec < NextPrec)
		{
			RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
			if (!RHS)
				return nullptr;
		}

		LHS =
			std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
	}
}

// expression ::= primary binoprhs
static std::unique_ptr<ExprAST> ParseExpression()
{
	auto LHS = ParsePrimary();
	if (!LHS)
		return nullptr;

	return ParseBinOpRHS(0, std::move(LHS));
}

// protopype ::= id '(' (E|id (',' id)*) ')'
static std::unique_ptr<PrototypeAST> ParsePrototype()
{
	if (CurTok != tok_identifier)
		return LogErrorP("Expected function name in prototype");

	std::string FnName = IdentifierStr;
	getNextToken();

	if (CurTok != '(')
		return LogErrorP("Expected '(' in prototype");

	std::vector<std::string> ArgNames;
	while (getNextToken() == tok_identifier)
		ArgNames.push_back(IdentifierStr);

	if (CurTok != ')')
		return LogErrorP("Expected ')' in prototype");

	getNextToken();

	return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition()
{
	getNextToken();
	auto Proto = ParsePrototype();
	if (!Proto)
		return nullptr;

	if (auto E = ParseExpression())
		return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));

	return nullptr;
}

static std::unique_ptr<FunctionAST> ParseTopLevelExpression()
{
	if (auto E = ParseExpression())
	{
		auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
													std::vector<std::string>());
		return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
	}
	return nullptr;
}

// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern()
{
	getNextToken();
	return ParsePrototype();
}

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string, Value *> NamedValues;

Value *LogErrorV(const char *Str)
{
	LogError(Str);
	return nullptr;
}

Value *NumberExprAST::codegen()
{
	return ConstantFP::get(*TheContext, APFloat(Val));
}

Value *VariableExprAST::codegen()
{
	llvm::Value *V = NamedValues[Name];
	if (!V)
		LogErrorV("Unknown Variable");
	return V;
}

Value *BinaryExprAST::codegen()
{
	Value *L = LHS->codegen();
	Value *R = RHS->codegen();
	if (!L || !R)
		return nullptr;

	switch (Op)
	{
	case '+':
		return Builder->CreateFAdd(L, R, "addtmp");
	case '-':
		return Builder->CreateFSub(L, R, "subtmp");
	case '*':
		return Builder->CreateFMul(L, R, "multmp");
	case '<':
		L = Builder->CreateFCmpULT(L, R, "cmptmp");
		return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
	default:
		return LogErrorV("invalid binary operator");
	}
}

Value *CallExprAST::codegen()
{
	Function *CalleF = TheModule->getFunction(Callee);
	if (!CalleF)
		return LogErrorV("Unknown function referenced");
	if (CalleF->arg_size() != Args.size())
		return LogErrorV("Incorrect # of arguments passed");

	std::vector<Value *> ArgsV;
	for (unsigned i = 0, e = Args.size(); i != e; ++i)
	{
		ArgsV.push_back(Args[i]->codegen());
		if (!ArgsV.back())
			return nullptr;
	}
	return Builder->CreateCall(CalleF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen()
{
	std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
	FunctionType *FT =
		FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
	Function *F =
		Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

	unsigned Idx = 0;
	for (auto &Arg : F->args())
		Arg.setName(Args[Idx++]);

	return F;
}

Function *FunctionAST::codegen()
{
	Function *TheFunction = TheModule->getFunction(Proto->getName());
	if (!TheFunction)
		TheFunction = Proto->codegen();
	if (!TheFunction)
		return nullptr;
	BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
	Builder->SetInsertPoint(BB);
	NamedValues.clear();
	for (auto &Arg : TheFunction->args())
		NamedValues[std::string(Arg.getName())] = &Arg;
	if (Value *RetVal = Body->codegen())
	{
		Builder->CreateRet(RetVal);
		verifyFunction(*TheFunction);
		return TheFunction;
	}
	TheFunction->eraseFromParent();
	return nullptr;
}

static void InitializeModule()
{
	TheContext = std::make_unique<LLVMContext>();
	TheModule = std::make_unique<Module>("my cool jit", *TheContext);
	Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

// initial symbols
static void HandleDefinition()
{
	if (ParseDefinition())
	{
		fprintf(stderr, "Parsed a function definition.\n");
	}
	else
	{
		getNextToken();
	}
}

static void HandleExtern()
{
	if (ParseExtern())
	{
		fprintf(stderr, "Parsed a top-level extern.\n");
	}
	else
	{
		getNextToken();
	}
}

static void HandleTopLevelExpression()
{
	if (ParseTopLevelExpression())
	{
		fprintf(stderr, "Parced top-level expression.\n");
	}
	else
	{
		getNextToken();
	}
}

// top ::= definition | external | expression | ';'
static void MainLoop()
{
	while (true)
	{
		fprintf(stderr, "ready> ");
		switch (CurTok)
		{
		case tok_eof:
			return;
		case ';':
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

int main()
{
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40;

	fprintf(stderr, "ready> ");
	getNextToken();

	MainLoop();

	return 0;
}

/*
 * para compilar
 *
 * arch:
 *   clang++ -lLLVM toy.cpp
 *
 * ubuntu/debian:
 *   clang++ toy.cpp $(llvm-config-15 --cxxflags) -lLLVM-15
 *
 * */
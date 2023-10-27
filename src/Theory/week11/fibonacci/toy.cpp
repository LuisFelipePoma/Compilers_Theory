#include <memory>
#include <string>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
enum Token
{
	tok_eof = -1,
	tok_def = -2,
	tok_extern = -3,

	tok_identifier = -4,
	tok_number = -5
};

static std::string IdentifierStr;
static double NumVal;

static int gettok()
{
	IdentifierStr = "";
	NumVal = 0.0;
	static int LastChar = ' ';
	while (isspace(LastChar))
		LastChar = getchar();
	if (isalpha(LastChar))
	{
		IdentifierStr += LastChar;
		while (isalnum(LastChar = getchar()))
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

// Analizador Sintactico

namespace
{
	class ExprAST
	{
	public:
		virtual ~ExprAST() = default;
	};

	class NumberExpreAST : public ExprAST
	{
		double Val;

	public:
		NumberExpreAST(double Val) : Val(Val) {}
	};

	class VariableExpreAST : public ExprAST
	{
		std::string Name;

	public:
		VariableExpreAST(const std::string &Name) : Name(Name) {}
	};

	class BinaryExprAST : public ExprAST
	{
		char Op;
		std::unique_ptr<ExprAST> LHS, RHS; // LHS: Left Hand Side, RHS: Right Hand Side
										   // ExprAST * LHS , *RHS -> Traditional way
	public:
		BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
	};

	class Call
	{
		std::string Callee;
		std::vector<std::unique_ptr<ExprAST>> Args;

	public:
		Call(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
			: Callee(Callee), Args(std::move(Args)) {}
	};

	class PrototypeAST
	{
		std::string Name;
		std::vector<std::string> Args;

	public:
		PrototypeAST(const std::string &Name, std::vector<std::string> Args) : Name(Name), Args(std::move(Args)) {}
		const std::string &getName() const { return Name; }
	};

	class FuncionAST
	{
		std::unique_ptr<PrototypeAST> Proto;
		std::unique_ptr<ExprAST> Body;

	public:
		FuncionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
			: Proto(std::move(Proto)), Body(std::move(Body)) {}
	};
}

static int CurTok;
static int getNextToken() { return CurTok = gettok(); };

static std::map<char, int> BinopPrecedence;

static int GetTokPrecendence()
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

static std::unique_ptr<ExprAST> ParseNumber()
{
	auto Result = std::make_unique<NumberExpreAST>(NumVal);
	getNextToken();
	return std::move(Result);
}

// numberexpr ::= ()
static std::unique_ptr<ExprAST> ParseParenExpr()
{
	getNextToken();
	auto V = ParseExpression();
	if (!V)
		return nullptr;
	if (CurTok != ')')
		return LogError("Expected ')'");
	getNextToken();
	return V;
}

// FUNCION MAIN
main(int argc, char **argv)
{
	int tok;
	while ((tok = gettok()) != tok_eof)
	{
		switch (tok)
		{
		case tok_number:
			std::cout << "Number: " << NumVal << '\n';
			break;
		case tok_def:
		case tok_extern:
		case tok_identifier:
			std::cout << "String: " << IdentifierStr << '\n';
			break;
		}
	}
}
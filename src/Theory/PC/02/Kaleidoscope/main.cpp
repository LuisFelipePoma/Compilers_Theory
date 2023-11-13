#include <fstream>
#include <cstdio>
#include <string>

#include "antlr4-runtime.h"
#include "EvalVisitorImpl.h"
#include "libs/ExprLexer.h"
#include "libs/ExprParser.h"
#include "main.h"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return -1;
	std::ifstream is;
	is.open(argv[1]);
	antlr4::ANTLRInputStream input(is);
	ExprLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	ExprParser parser(&tokens);
	auto tree = parser.program();
	EvalVisitorImpl eval;
	eval.visitProgram(tree);
	return 0;
}
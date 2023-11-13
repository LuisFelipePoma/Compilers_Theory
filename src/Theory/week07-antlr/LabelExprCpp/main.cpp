#include <fstream>
#include <cstdio>
#include <string>

#include "antlr4-runtime.h"
#include "EvalVisitorImpl.h"
#include "libs/LabeledExprLexer.h"
#include "libs/LabeledExprParser.h"

int main(int argc, char **argv)
{
	if (argc <= 1)
		return -1;
	std::ifstream is;
	is.open(argv[1]);
	antlr4::ANTLRInputStream input(is);
	LabeledExprLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	LabeledExprParser parser(&tokens);
	auto tree = parser.prog();
	EvalVisitorImpl eval;
	eval.visitProg(tree);
	return 0;
}
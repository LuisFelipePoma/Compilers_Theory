#include "libs/pcdosLexer.h"
#include "libs/pcdosParser.h"
#include "pcdosVisitorImpl.h"

#include <fstream>

int main(int argc, char **argv)
{
	if (argc <= 1)
		return -1;
	std::ifstream is;
	is.open(argv[1]);
	antlr4::ANTLRInputStream input(is);
	pcdosLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	pcdosParser parser(&tokens);
	auto tree = parser.prog();
	pcdosVisitorImpl eval;
	eval.visitProg(tree);
	eval.test();
	// std::cout << tree->toStringTree(true) << std::endl;
}
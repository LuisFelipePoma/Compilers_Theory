%{
	#include <stdio.h>
	#include <stdlib.h>
%}
%option noyywrap

COMP (<=?)|(>=?)|(!=)|(==)
ID [a-zA-Z_][a-zA-Z0-9_]*
NUM [0-9]+

%%

{COMP}			{ printf("Lexem: %s\n", yytext); }
if				{ printf("Lexem: %s\n", yytext); }
else			{ printf("Lexem: %s\n", yytext); }
{ID}			{ printf("Lexem: %s\n", yytext); }
{NUM}			{ printf("Lexem: %s\n", yytext); }
[ \n\r\t]		{}
.				{}

%%

int main(int argc, char** argv){
	++argv, --argc;
	if (argc > 0){
		yyin = fopen(argv[0], "r");
	}else{
		yyin = stdin;
	}

	int token;
	while((token = yylex())){
		//
	}
}

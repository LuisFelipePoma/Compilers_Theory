%{
	#include <stdio.h>
	#include <stdlib.h>

	#define COMP 256
	#define ID 257
	#define IF 258
	#define ELSE 259
	#define NUM 260
%}
%option noyywrap

COMP (<=?)|(>=?)|(!=)|(==)
ID [a-zA-Z_][a-zA-Z0-9_]*
NUM [0-9]+
%%

{COMP}		{return COMP;}
if			{return IF;}
else		{return ELSE;}
{ID}		{return ID;}
{NUM}		{return NUM;}
[ \n\r\t]	{}
.			{}
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
		printf("Lexem (%d): %s\n", token, yytext);
	}
}

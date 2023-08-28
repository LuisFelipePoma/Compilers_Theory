#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM 265
#define YYEOF 399
double yylval;

int yylex(void){
	int c = getchar();
	while (c == ' ' || c == '\t')
		c = getchar();
	if (c == '.' || isdigit(c)){
		ungetc(c,stdin);
		if(scanf("%lf",&yylval) != 1)
			abort();
		return NUM;
	} else if(c == EOF)
		return YYEOF;
	else return c;
}
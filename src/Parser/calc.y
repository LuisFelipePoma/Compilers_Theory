%{
#include <stdio.h>
#include <math.h>

int yylex(void);
void yyerror(char const *);
	
%}

%define api.value.type {double}
%token NUM

%%

s :
	%empty 
	| s line
	;
line:
	'\n'
	| expr '\n'		{printf("%.10g\n", $1);}
	;

expr:
	NUM
	| expr expr '+'		{$$ = $1 + $2;}
	| expr expr '-'		{$$ = $1 - $2;}
	| expr expr '*'		{$$ = $1 * $2;}
	| expr expr '/'		{$$ = $1 / $2;}
	| expr expr '^'		{$$ = pow($1, $2);}
	| expr 'n'			{$$ = -$1;}
	;

%%

#include <ctype.h>
#include <stdlib.h>

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

int main(void){
	return yyparse();
}

void yyerror(char const *s){
	fprintf(stderr, "%s\n",s);
}
%{
#include <stdio.h>
%}
%option noyywrap
%%

"int"       { printf("<Token: %s, PALABRA_CLAVE_INT>\n",yytext); }
"if"        { printf("<Token: %s, PALABRA_CLAVE_IF>\n",yytext); }
"else"      { printf("<Token: %s, PALABRA_CLAVE_ELSE>\n",yytext); }
"for"       { printf("<Token: %s, PALABRA_CLAVE_FOR>\n",yytext); }
"while"     { printf("<Token: %s, PALABRA_CLAVE_WHILE>\n",yytext); }
"return"    { printf("<Token: %s, PALABRA_CLAVE_RETURN>\n",yytext); }
"public"    { printf("<Token: %s, PALABRA_CLAVE_PUBLIC>\n",yytext); }
"private"   { printf("<Token: %s, PALABRA_CLAVE_PRIVATE>\n",yytext); }
"class"     { printf("<Token: %s, PALABRA_CLAVE_CLASS>\n",yytext); }
"static"    { printf("<Token: %s, PALABRA_CLAVE_STATIC>\n",yytext); }


[a-zA-Z_][a-zA-Z0-9_]*   { printf("<Token: %s, IDENTIFICADOR>\n", yytext); }
[0-9]+                   { printf("<Token: %s, ENTERO>\n", yytext); }
[ \t\n]                  ;
[=]                      { printf("<Token: %s, ASSIGN>\n", yytext); }
.                        { printf("<Token: %s, DESCONOCIDO>\n", yytext); }

%%

int main(int argc, char** argv) {
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
    return 0;
}

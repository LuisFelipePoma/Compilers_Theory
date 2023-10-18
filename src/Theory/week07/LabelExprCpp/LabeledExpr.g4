grammar LabeledExpr;

/* Syntax analizer */

prog: stat+ EOF;

stat
	: expr NEWLINE          # printExpr
	| ID '=' expr NEWLINE   # assign
	| NEWLINE               # blank
	;

expr
	: expr op=('*'|'/') expr   # MulDiv
	| expr op=('+'|'-') expr   # AddSub
	| INT                   # Int
	| ID                    # Id
	| '(' expr ')'          # Parens
	;

/* Lexical analizer */

MUL: '*' ;
DIV: '/' ;
ADD: '+' ;
SUB: '-' ;

ID     : [a-zA-Z]+ ;
INT    : [0-9]+ ;
NEWLINE: '\n' ;
WS     : [ \t]+ -> skip ;
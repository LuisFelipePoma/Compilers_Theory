grammar pcdos;

/* Syntax analizer */

prog: stat+ ;

stat
	: expr NEWLINE          # printExpr
	| ID '=' expr NEWLINE   # assign
	| definition NEWLINE    # statdef
	| external NEWLINE      # statextern
	| NEWLINE               # blank
	;

expr
	: expr op=('*'|'/') expr   # MulDiv
	| expr op=('+'|'-') expr   # AddSub
	| NUMBER                   # Number
	| ID '(' expr* ')'         # Call
	| ID                       # Id
	| '(' expr ')'             # Parens
	;

prototype
	: ID '(' ID* ')'           # Proto
	;

definition
	: 'def' prototype expr     # Def
	;

external
	: 'extern' prototype       # Extern
	;

/* Lexical analizer */

MUL: '*' ;
DIV: '/' ;
ADD: '+' ;
SUB: '-' ;

DEF    : 'def' ;
EXTERN : 'extern' ;
ID     : [a-zA-Z]+ ;
NUMBER : [+-]?([0-9]*[.])?[0-9]+ ;
NEWLINE: '\r'? '\n' ;
COMMENT: [#].*? NEWLINE -> skip ;
WS     : [ \t]+ -> skip ;
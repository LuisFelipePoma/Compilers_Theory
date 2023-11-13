grammar Expr;

program: statlist EOF;

statlist: stat* # StatList;

stat: initFunc # InitFunction
	| if # InitIf
	| expr # InitExpr
	| ID '=' expr # Assign
	;

initFunc: DEF ID '(' ID ')' '{' stat+ '}' # BodyFunction;

expr:
	expr op=('+' | '-') expr	# SumExpr
	| expr op=('>' | '<') expr	# CompExpr
	| func					# FuncExpr
	| id					# IdExpr;

func: ID '(' expr ')' # FuncBody;

id: ID | INT
;

if: IF stat 'then' stat* ELSE? stat* # IfStmt;

DEF: 'def';
MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';
COMP: '>' | '<';

IF: 'if';
ELSE: 'else';

INT: [0-9]+;
ID: [a-zA-Z_][a-zA-Z_0-9]*;

WS: [ \n\r\t\f]+ -> skip;
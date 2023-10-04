grammar LabeledExpr;

/* Syntax analizer*/

prog: stat+ EOF;

stat
    : expr NEWLINE              # printExpr
    | ID '=' expr NEWLINE       # assing
    | NEWLINE                   # blank
    ;
expr
    : expr 'sqrt'               # Sqrt
    | expr op=('tan'|'sen')    # TanSen 
    | expr expr 'pow'           # Pow
    | expr expr 'mod'           # Mod
    | expr expr op=('*'|'/')    # MulDiv 
    | expr expr op=('+'|'-')    # AddSub 
    | INT                       # Int
    | ID                        # Id
    | '(' expr ')'              # Parens
    ;


/* Lexical analizer*/

MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';
POW: 'pow';
SQRT: 'sqrt';
MOD: 'mod';
TAN: 'tan';
SEN: 'sen';

ID      : [a-zA-Z]+;
INT     : [0-9]+;
NEWLINE : '\r'? '\n';
WS      : [ \t]+ -> skip;

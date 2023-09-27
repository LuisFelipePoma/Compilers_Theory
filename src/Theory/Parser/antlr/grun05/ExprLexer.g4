lexer grammar ExprLexer;

EQ   : '=' ;
PLUS : '+' ;
MINU : '-' ;
MULT : '*' ;
DIVI : '/' ;
EXP  : '^' ;

NUM : [-+]?([0-9]*[.][0-9]+|[0-9]+[.]?)([eE][-+]?[0-9]+)?;
ID: [a-zA-Z_][a-zA-Z_0-9]* ;
WS: [ \t\n\r\f]+ -> skip ;
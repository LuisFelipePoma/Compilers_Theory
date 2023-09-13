grammar expr2;
prog: e EOF ;
e: t eprime ;
eprime:
    '+' t eprime
    |
    ;
t: f tprime ;
tprime:
    '*' f tprime
    |
    ;
f:
    '(' e ')'
    | ID
    ;
NEWLINE : [\r\n]+ -> skip;
ID      : [a-zA-Z_][a-zA-Z_0-9]* ;
grammar Expr;		
prog:	expr EOF ;
expr:	expr ('*'|'/') expr
    |	expr ('+'|'-') expr
    |	INT
    |	'(' expr ')'
    ;
NEWLINE : [\r\n]+ -> skip;
INT     : [0-9]+ ;


// grammar Expr;		
// prog:	expr EOF ;
// expr:	expr ('*'|'/') expr
//     |	expr ('+'|'-') expr
//     |	INT
//     |	'(' expr ')'
//     ;
// NEWLINE : [\r\n]+ -> skip;
// INT     : [0-9]+ ;

// parser grammar ExprParser;

// e: t prime EOF;
// eprime:
//     '+' t eprime
//     |
//     ;
// t: 
//     '*' f tprime
//     |
//     ;
// f: 
//     '('e')'
//     | ID
//     ;
    
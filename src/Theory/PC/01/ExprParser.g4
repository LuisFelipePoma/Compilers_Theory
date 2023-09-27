parser grammar ExprParser;
options { tokenVocab=ExprLexer; }

program
    : lststat EOF
    ;
lststat
    : lststat stat
    | fn
    | 
    ;
stat
    : ID '=' expr ';'
    | expr ';'
    | expr '+''+' ';'
    ;
    
fn:  'func' ID '(' ID (',' ID)* ')' '{' (stat*|fn) '}' ;
expr
    :expr '+' expr
    | NUM
    | ID
    ;
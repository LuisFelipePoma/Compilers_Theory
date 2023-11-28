parser grammar ExprParser;
options { tokenVocab=ExprLexer; }

program
    : lststat EOF
    ;
lststat
    : lststat stat
    |
    ;
stat
    : ID '=' expr
    | expr
    ;
expr
    : expr expr '+'
    | NUM
    ;
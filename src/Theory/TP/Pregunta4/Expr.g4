grammar Expr;

program

  : stat EOF
  | def EOF
  ;

stat: ID '=' expr 
  | expr 
  ;

def : 'func' ID '(' ID TYPE ')' TYPE '{' stat* '}' ;

expr:
  ID (MINUS|TIMES) expr
  |ID
  | INT
  | func
  | RETURN expr
  ;

func : ID '(' expr (',' expr)* ')' 
   | 'if' condition '{' stat* '}' 'else' '{' stat* '}'  
   ;

condition: ID CONDITION INT; 

TYPE: 'int';

MINUS: '-';
TIMES: '*';
CONDITION: '=='| '<='|'>=' | '<'| '>' ;
RETURN: 'return';

INT : [0-9]+ ;
ID: [a-zA-Z_][a-zA-Z_0-9]* ;
WS: [ \t\n\r\f]+ -> skip ;
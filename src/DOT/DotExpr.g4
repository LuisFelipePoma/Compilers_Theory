grammar DotExpr;

graph
    : STRICT? (GRAPH|DIGRAPH) ID? '{' stmt_list '}' EOF
    ;

stmt_list
    :  (stmt A? stmt_list)?
    ;

stmt
    : subgraph
    | node_stmt
    | edge_stmt
    | attr_stmt
    | ID '=' ID
    ;
attr_stmt
    : (GRAPH|NODE|EDGE) attr_list
    ;

attr_list
    : '[' a_list? ']' attr_list?
    ;

a_list
    : ID '=' ID A? a_list?
    ;

edge_stmt
    : (node_id|subgraph) edgeRHS attr_list?
    ;

edgeRHS
    : EDGEOP (node_id|subgraph) edgeRHS?
    ;

node_stmt 
    : node_id attr_list?
    ;

node_id
    : ID port?
    ;

port
    : ':' ID (':' compass_pt)+
    | ':' compass_pt
    ;
    
subgraph	
    : (SUBGRAPH ID?)? '{' stmt_list '}'
    ;
compass_pt
    : COMPASS_PT
    ;

A: ';' | ',' ;
STRICT     : 'strict';
GRAPH      : 'graph';
DIGRAPH    : 'digraph';
SUBGRAPH   : 'subgraph';
NODE       : 'node';
EDGE       : 'edge';
COMPASS_PT : 'n' | 'ne' | 'e' | 'se' | 's' | 'sw' | 'w' | 'nw' | 'c' | '_';

ID         : [a-zA-Z_]+[a-zA-Z_0-9]* ;
EDGEOP     : '->' | '--' ; 
WS         : [ \t\n\r\f"]+ -> skip ;
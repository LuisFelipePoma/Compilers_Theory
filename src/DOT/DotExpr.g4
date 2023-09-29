grammar DotExpr;

graph
    : STRICT? (GRAPH|DIGRAPH) ID? '{' stmt_list '}' EOF # StmtList
    ;

stmt_list
    :  (stmt A?) stmt_list?                     
    ;

stmt
    : subgraph                                  # SubGraph
    | node_stmt                                 # Node
    | edge_stmt                                 # Edge
    | attr_stmt                                 # Attr
    | ID '=' ID                                 # Assign
    ;
attr_stmt
    : (GRAPH|NODE|EDGE) attr_list+
    ;

attr_list
    : '[' a_list+ ']'
    ;

a_list
    : ID '=' ID A? 
    ;

edge_stmt
    : (node_id|subgraph) edgeRHS attr_list?     # EdgeStmt   
    ;

edgeRHS
    : EDGEOP (node_id | subgraph) edgeRHS?                 # EdgeRhs
    ;

node_stmt 
    : node_id attr_list?                        # NodeStmt
    ;

node_id
    : ID port?                                  # Id
    ;

port
    : ':' ID (':' compass_pt)+
    | ':' compass_pt
    ;
    
subgraph	
    : (SUBGRAPH ID?)? '{' stmt_list '}'         # SubGraphBody
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

ID         : ["]+[a-zA-Z0-9:|]*["]+ | [a-zA-Z:|]+[a-zA-Z_0-9:|]* | [-]? [0-9]+;
EDGEOP     : '->' | '--' ; 
WS         : [ \t\n\r\f"]+ -> skip ;
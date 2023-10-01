grammar DotExpr;

graph
    : STRICT? ( GRAPH | DIGRAPH ) id_? '{' stmt_list '}' EOF  # GraphBody
    ;

stmt_list
    : (stmt sep?)+  # StmtList
    ;
stmt
    : node_stmt   # Node 
    | edge_stmt   # Edge
    | attr_stmt   # Attr
    | id_ '=' id_ # Assign
    | subgraph    # SubGraph
    ;
attr_stmt
    : (GRAPH | NODE | EDGE ) attr_list
    ;
attr_list
    : ('[' a_list? ']')+
    ;
a_list
    : (id_ '=' id_  sep? )+ #AList
    ;
edge_stmt
    : ((node_id | subgraph) edgeRHS+) attr_list? # EdgeStmt
    ;
edgeRHS
    : (edgeop ( node_id | subgraph)) # EdgeRhs
    ;
edgeop
    : '->' | '--'
    ;
sep
    : ',' | ';'
    ;
node_stmt
    : node_id attr_list? # NodeStmt
    ;
node_id
    : id_ port? # Id
    ;
port
    : ':' id_ ( ':' id_ )?
    ;
subgraph
    : (SUBGRAPH id_?)? '{' stmt_list '}' # SubGraphBody
    ;
id_
    : ID | STRING | HTML_STRING | NUMBER
    ;

STRICT
   : [Ss] [Tt] [Rr] [Ii] [Cc] [Tt]
   ;
GRAPH
   : [Gg] [Rr] [Aa] [Pp] [Hh]
   ;
DIGRAPH
   : [Dd] [Ii] [Gg] [Rr] [Aa] [Pp] [Hh]
   ;
NODE
   : [Nn] [Oo] [Dd] [Ee]
   ;
EDGE
   : [Ee] [Dd] [Gg] [Ee]
   ;
SUBGRAPH
   : [Ss] [Uu] [Bb] [Gg] [Rr] [Aa] [Pp] [Hh]
   ;
   
NUMBER: '-'? ( '.' DIGIT+ | DIGIT+ ( '.' DIGIT* )? )
        ;
        
fragment DIGIT
   : [0-9]
   ;
STRING: '"' ( '\\"' | . )*? '"';
ID: LETTER ( LETTER | DIGIT )*;

fragment LETTER
   : [a-zA-Z\u0080-\u00FF_]
   ;
HTML_STRING
   : '<' ( TAG | ~ [<>] )* '>'
   ;
fragment TAG
   : '<' .*? '>'
   ; 
   
COMMENT
   : '#' ~[\r\n]* -> skip
   ;

WS
   : [ \t\n\r]+ -> skip
   ;
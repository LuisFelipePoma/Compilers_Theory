import java.util.ArrayList;
import java.util.List;

public class Visitor extends DotExprBaseVisitor<String> {
    SymbolTable symbolTable = new SymbolTable(); // Symbol table
    List<String> attr_list = new ArrayList<String>(); // Attributes
    List<String> edges_list = new ArrayList<String>(); // Edges
    String previous_node; // Previous node

    /* id_ '=' id_ #Assign */
    @Override
    public String visitAssign(DotExprParser.AssignContext ctx) {
        String id = ctx.id_(0).getText();
        String value = ctx.id_(1).getText();
        addAttrToken(id, "Assign", value);
        return value;
    }

    /* (id_ '=' id_ sep? )+ #AList */
    @Override
    public String visitAList(DotExprParser.AListContext ctx) {

        Integer size = ctx.id_().size();
        for (int i = 0; i < size; i += 2) {
            String id = ctx.id_(i).getText();
            String value = ctx.id_(i + 1).getText();
            attr_list.add(id);
            attr_list.add(value);
        }
        return "size";
    }

    /* ((node_id | subgraph) edgeRHS ) attr_list? # EdgeStmt */
    @Override
    public String visitEdgeStmt(DotExprParser.EdgeStmtContext ctx) {

        previous_node = visit(ctx.getChild(0));
        edges_list.add(previous_node);

        Integer size = ctx.children.size();

        for (int i = 1; i < size; i++) {
            String type = ctx.getChild(i).getClass().getSimpleName();
            switch (type) {
                case "EdgeRhsContext":
                    String id = visit(ctx.getChild(i));
                    previous_node = id;
                    edges_list.add(id);
                    break;
                case "Attr_listContext":
                    visit(ctx.getChild(i));
                    break;
                case "EdgeopContext":
                    break;
            }
        }
        addAttrEdge();
        return null;
    }

    //? For subgraph implementation
    /* (edgeop ( node_id | subgraph)) # EdgeRhs */
    @Override
    public String visitEdgeRhs(DotExprParser.EdgeRhsContext ctx) {
        // String type = ctx.getChild(1).getClass().getSimpleName();
        // if (type.contains("SubGraphBodyContext")) {
        //     visit(ctx.getChild(1));
        //     return null;
        // }
        return visit(ctx.getChild(1));
    }
    //? For subgraph implementation

    /* node_id attr_list? # NodeStmt */
    @Override
    public String visitNodeStmt(DotExprParser.NodeStmtContext ctx) {
        String node_id = visit(ctx.node_id());
        if (ctx.children.size() == 2) {
            visit(ctx.getChild(1));
            addAttrToken(node_id, "Attr", null);
        }
        addAttrToken(node_id, "Type", "Node");
        return node_id;
    }

    /* id_ port? # Id */
    @Override
    public String visitId(DotExprParser.IdContext ctx) {
        String id = ctx.id_().getText();
        addAttrToken(id, "Type", "Node");
        return id;
    }

    //------------------------------------------- FUNCTIONS -------------------------------------------------- //

    // Function to get the symbol table
    public SymbolTable getSymbolTable() {
        return symbolTable;
    }

    // Function to add a edge to the symbol table
    public void addAttrEdge() {
        for (Integer i = 0; i < edges_list.size() - 1; i++) {
            symbolTable.addEdge(edges_list.get(i), edges_list.get(i + 1), attr_list);
        }
        attr_list.clear();
        edges_list.clear();
    }

    // Function to add a token to the symbol table
    public void addAttrToken(String token, String type, String attr) {
        List<String> tokenAttr = new ArrayList<>();
        tokenAttr.add(type);
        tokenAttr.add(attr);
        if (type.equals("Attr")) {
            symbolTable.addToken(token, attr_list);
            attr_list.clear();
            edges_list.clear();
        } else {
            symbolTable.addToken(token, tokenAttr);
        }
    }
}
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Visitor extends DotExprBaseVisitor<String> {
    Map<String, String> symbolTable = new HashMap<String, String>();
    Map<String, List<String>> list = new HashMap<String, List<String>>();
    List<String> SUBGRAPH = new ArrayList<String>();
    Boolean isSubgraph = false;
    Boolean isAttr = false;
    String node;
    String tmp;

    /* STRICT? (GRAPH|DIGRAPH) id_? '{' stmt_list '}' EOF # GraphBody */
    @Override
    public String visitGraphBody(DotExprParser.GraphBodyContext ctx) {
        // Get the type of the graph
        // if(ctx.children == null) System.out.println("ctx");;

        // Get the name of the graph
        // Boolean name = ctx.id_().isEmpty();

        visit(ctx.stmt_list());
        writeFile();
        return "true";
    }

    /* (stmt sep?)+ # StmtList */
    @Override
    public String visitStmtList(DotExprParser.StmtListContext ctx) {
        ctx.stmt().forEach(tree -> visit(tree));
        return "stmt";
    }

    /* id_ '=' id_ #Assign */
    @Override
    public String visitAssign(DotExprParser.AssignContext ctx) {
        String id = ctx.id_(0).getText();
        String value = ctx.id_(1).getText();
        symbolTable.put(id, value);
        return value;
    }

    // /* (id_ ( '=' id_ )? sep? )+ #AList */
    // @Override
    // public String visitAList(DotExprParser.AListContext ctx){
    //     String id = ctx.id_(0).getText();
    //     String value = ctx.id_(1).getText();
    //     if(isAttr)
    //     symbolTable.put(id, value);
    //     return value;
    // }

    /* ((node_id | subgraph) edgeRHS )+ attr_list? # EdgeStmt */
    @Override
    public String visitEdgeStmt(DotExprParser.EdgeStmtContext ctx) {

        node = visit(ctx.getChild(0));
        if (!isSubgraph)
            tmp = node;

        Integer size = ctx.children.size();

        for (int i = 1; i < size; i++) {
            String type = ctx.getChild(i).getClass().getSimpleName();
            switch (type) {
                case "EdgeRhsContext":
                    String id = visit(ctx.getChild(i));
                    if (id == null) {
                        final Integer pos = i;
                        node = visit(ctx.getChild(pos + 1));
                        SUBGRAPH.forEach(nodes -> insertNode(nodes, node));
                        SUBGRAPH.clear();
                        i++;
                    } else {
                        insertNode(node, id);
                        node = id;
                    }
                    break;
                case "EdgeopContext":
                    break;
            }
        }

        voidNode(node);
        return null;
    }

    /* (edgeop ( node_id | subgraph)) # EdgeRhs */
    @Override
    public String visitEdgeRhs(DotExprParser.EdgeRhsContext ctx) {
        String type = ctx.getChild(1).getClass().getSimpleName();
        if (type.contains("SubGraphBodyContext")) {
            isSubgraph = true;
            visit(ctx.getChild(1));
            SUBGRAPH.forEach(nodes -> insertNode(tmp, nodes));
            isSubgraph = false;
            return null;
        }
        return visit(ctx.getChild(1));
    }

    /* (SUBGRAPH id_?)? '{' stmt_list '}' # SubGraphBody */
    @Override
    public String visitSubGraphBody(DotExprParser.SubGraphBodyContext ctx) {
        visit(ctx.stmt_list());
        if (ctx.children.size() != 3)
            ; // TODO add id to table
        return null;
    }

    /* node_id attr_list? # NodeStmt */
    @Override
    public String visitNodeStmt(DotExprParser.NodeStmtContext ctx) {
        String node_id = visit(ctx.node_id());
        voidNode(node_id);
        return node_id;
    }

    /* id_ port? # Id */
    @Override
    public String visitId(DotExprParser.IdContext ctx) {
        String id = ctx.id_().getText();
        if (isSubgraph) {
            SUBGRAPH.add(id);
        }
        symbolTable.put(id, "node");
        return id;
    }

    // ! FUNCTIONS
    public void insertNode(String node, String element) {
        // Add all the edges
        if (list.containsKey(node)) {
            // If the key is already present, get the list of values
            List<String> values = list.get(node);
            values.removeIf((String arg0) -> arg0 == "-");
            // Add node and value to the symbolTable
            insertTableSymbolic(node, element);
            // Add the new value to the list
            values.add(element);
        } else {
            // If the key is not present, create a new list with the value
            List<String> values = new ArrayList<>();
            // Add node and value to the symbolTable
            insertTableSymbolic(node, element);
            // Add the new value to the list
            values.add(element);
            list.put(node, values);
        }
    }

    public void insertTableSymbolic(String node, String element) {
        if (element != "-")
            symbolTable.put(node + " -> " + element, "edge");
    }

    public void writeFile() {
        try (PrintWriter writer = new PrintWriter("output.txt")) {
            for (Map.Entry<String, List<String>> entry : list.entrySet()) {
                String key = entry.getKey();
                List<String> values = entry.getValue();
                writer.print(key + ": ");
                for (String value : values) {
                    writer.print(value + " ");
                }
                writer.println();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public void voidNode(String node) {
        if (!list.containsKey(node))
            insertNode(node, "-");
    }

    public Map<String, String> getSymbolTable() {
        return symbolTable;
    }
}
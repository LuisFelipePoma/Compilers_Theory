import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Visitor extends DotExprBaseVisitor<String> {
    Map<String, String> memory = new HashMap<String, String>();
    Map<String, List<String>> list = new HashMap<String, List<String>>();
    String node;

    /* ID '=' ID #Assign */
    @Override
    public String visitAssign(DotExprParser.AssignContext ctx) {
        String id = ctx.ID(0).getText();
        String value = ctx.ID(1).getText();
        memory.put(id, value);
        // System.out.println(value);
        return "0";
    }

    /* (node_id|subgraph) edgeRHS+ attr_list? #EdgeStmt */
    @Override
    public String visitEdgeStmt(DotExprParser.EdgeStmtContext ctx) {
        // Create a list of edges
        // List<String> arr = new ArrayList<>();
        // Get the node
        node = ctx.node_id().getText();
        // Get the iterator
        String element = visit(ctx.edgeRHS());
        // // Add all the edges

        if (list.containsKey(node)) {
            // If the key is already present, get the list of values
            List<String> values = list.get(node);
            // Add the new value to the list
            values.add(element);
        } else {
            // If the key is not present, create a new list with the value
            List<String> values = new ArrayList<>();
            values.add(element);
            list.put(node, values);
        }
        // while (iter.hasNext()) {
        // String element = visit(iter.next());
        // arr.add(element);
        // node = element;
        // }

        // Add the attributes
        // list.put(node, element);

        // // Write to file
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
        return node;
    }

    /* EDGEOP (node_id|subgraph) edgeRHS? # EdgeRhs */
    @Override
    public String visitEdgeRhs(DotExprParser.EdgeRhsContext ctx) {
        // ctx.children.get(0);
        System.out.println(ctx.getChild(1).getClass());

        return visit(ctx.getChild(1));
    }

    /* (SUBGRAPH ID?)? '{' stmt_list '}' # SubGraphBody */
    // @Override
    // public String visitSubGraphBody(DotExprParser.SubGraphBodyContext ctx) {
    //     System.out.println("POVVVV");
    //     return visit(ctx.stmt_list());
    // }

    /* (stmt A?) stmt_list? # StmtList */
    // @Override
    // public String visitStmtList(DotExprParser.StmtListContext ctx) {
    //     return visit(ctx.stmt());
    // }

    /* node_id attr_list? # NodeStmt */
    // @Override
    // public String visitNodeStmt(DotExprParser.NodeStmtContext ctx) {
    //     return visit(ctx.node_id());
    // }

    /* ID port? # Id */
    @Override
    public String visitId(DotExprParser.IdContext ctx) {
        return ctx.ID().getText();
    }
}
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

    /* STRICT? (GRAPH|DIGRAPH) ID? '{' stmt_list '}' EOF # StmtList */
    @Override
    public String visitStmtList(DotExprParser.StmtListContext ctx) {
        visit(ctx.stmt_list());
        writeFile();
        return "true";
    }

    /* ID '=' ID #Assign */
    @Override
    public String visitAssign(DotExprParser.AssignContext ctx) {
        String id = ctx.ID(0).getText();
        String value = ctx.ID(1).getText();
        memory.put(id, value);
        return value;
    }

    /* (node_id|subgraph) edgeRHS+ attr_list? #EdgeStmt */
    @Override
    public String visitEdgeStmt(DotExprParser.EdgeStmtContext ctx) {
        // Create a list of edges
        // Get the node
        node = ctx.node_id().getText();
        // Get the iterator
        String element = visit(ctx.edgeRHS());
        // Add all the edges
        insertNode(node, element);
        return node;
    }

    /* EDGEOP (node_id|subgraph) edgeRHS? # EdgeRhs */
    @Override
    public String visitEdgeRhs(DotExprParser.EdgeRhsContext ctx) {
        String n = ctx.getChild(1).getText();
        String e = "";
        if (ctx.children.size() == 3)
            e = visit(ctx.edgeRHS());
        System.out.print(n);
        System.out.println(e);
        insertNode(n, e);
        return visit(ctx.getChild(1));
    }

    /* ID port? # Id */
    @Override
    public String visitId(DotExprParser.IdContext ctx) {
        String node_id = ctx.ID().getText();
        if (!list.containsKey(node_id))
            insertNode(node_id, "");
        return node_id;
    }

    // ! FUNCTIONS
    public void insertNode(String node, String element) {
        // Add all the edges
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
    }

    public void writeFile() {
        try (PrintWriter writer = new PrintWriter("output.txt")) {
            for (Map.Entry<String, List<String>> entry : list.entrySet()) {
                String key = entry.getKey();
                List<String> values = entry.getValue();
                writer.print(key + ": ");
                for (String value : values) {
                    writer.print(value);
                }
                writer.println();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
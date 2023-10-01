import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import java.io.FileInputStream;
import java.io.InputStream;

public class ListAdj {
    public static void main(String[] args) throws Exception {
        String inputFile = null;
        if (args.length > 0)
            inputFile = args[0];
        InputStream is = System.in;
        if (inputFile != null)
            is = new FileInputStream(inputFile);
        CharStream input = CharStreams.fromStream(is);
        DotExprLexer lexer = new DotExprLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        DotExprParser parser = new DotExprParser(tokens);
        ParseTree tree = parser.graph();
        System.out.println(tree.toStringTree(parser));

        // Create a visitor
        Visitor eval = new Visitor();
        eval.visit(tree);

        // Get the symbol table
        SymbolTable symbolTable = eval.getSymbolTable();
        // Print the symbol table
        symbolTable.printSymbolTable();
        // Convert the symbol table to adjacency list and write it to a file
        symbolTable.convertToAdjacencyListAndWriteToFile("output.txt");
    }
}

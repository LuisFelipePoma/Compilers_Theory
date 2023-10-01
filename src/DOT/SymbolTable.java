import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class SymbolTable {
    private Map<String, Token> tokens;
    private List<Edge> edges;

    public SymbolTable() {
        this.tokens = new HashMap<>();
        this.edges = new ArrayList<>();
    }

    // Function to add a token to the symbol table
    public void addToken(String name, List<String> attrs) {
        Token token = new Token(name);
        for (Integer i = 0; i < attrs.size(); i += 2) {
            token.addAttribute(attrs.get(i), attrs.get(i + 1));
        }
        tokens.put(token.getName(), token);
    }

    // Function to add a edge to the symbol table
    public void addEdge(String source, String destiny, List<String> attrs) {
        Edge edge = new Edge(source, destiny);

        for (Integer i = 0; i < attrs.size(); i += 2) {
            edge.addAttribute(attrs.get(i), attrs.get(i + 1));
        }
        edges.add(edge);
    }

    // Function to get a token by its name
    public Map<String, Token> getTokens() {
        return tokens;
    }

    // Function to get a node by its name
    public List<Edge> getEdges() {
        return edges;
    }

    // Class to represent a token
    public class Token {
        private String name;
        private Map<String, String> attributes;

        public Token(String name) {
            this.name = name;
            this.attributes = new HashMap<>();
        }

        // Function to add a attribute to a token
        public void addAttribute(String key, String value) {
            attributes.put(key, value);
        }

        // Function to get the name of a token
        public String getName() {
            return name;
        }

    }

    public class Edge {
        private String source;
        private String destiny;
        private Map<String, String> visualAttributes;

        public Edge(String source, String destiny) {
            this.source = source;
            this.destiny = destiny;
            this.visualAttributes = new HashMap<>();
        }

        // Function to add a attribute to an edge
        public void addAttribute(String key, String value) {
            visualAttributes.put(key, value);
        }

        // Function to get the name of the source node
        public String getSourceNode() {
            return source;
        }

        // Function to get the name of the target node
        public String getTargetNode() {
            return destiny;
        }

        // Function to get the weight of an edge
        public String getWeigth() {
            String query = "label";
            String matchingValue = visualAttributes.get(query);
            if (matchingValue != null) {
                return matchingValue.replaceAll("\"", "");
            }
            return "_";
        }

        // Function to get the value of a visual attribute matching a regular expression
        public static String getValueMatchingRegex(Map<String, String> map, String regex) {
            Pattern pattern = Pattern.compile(regex);

            for (Map.Entry<String, String> entry : map.entrySet()) {
                Matcher matcher = pattern.matcher(entry.getKey());
                if (matcher.matches()) {
                    return entry.getValue();
                }
            }

            return null; 
        }
    }

    // Function to print symbol table
    public void printSymbolTable() {
        System.out.println("Tabla de Símbolos:");

        // Print token headers
        System.out.format("%-10s%-15s%n", "Token", "Attributes");
        for (Token token : tokens.values()) {
            System.out.format("%-10s", token.getName());
            StringBuilder attributesStr = new StringBuilder();
            for (Map.Entry<String, String> entry : token.attributes.entrySet()) {
                attributesStr.append(entry.getKey()).append(": ").append(entry.getValue()).append(", ");
            }
            // Delete the final comma and space
            if (attributesStr.length() > 2) {
                attributesStr.setLength(attributesStr.length() - 2);
            }
            System.out.format("%-15s%n", attributesStr.toString());
        }

        // Separate tokens and edges
        System.out.println("-------------------------");

        // Print edge headers
        System.out.format("%-10s%-10s%-15s%n", "Origin", "Target", "Attributes");
        for (Edge edge : edges) {
            System.out.format("%-10s%-10s", edge.getSourceNode(), edge.getTargetNode());
            StringBuilder attributesStr = new StringBuilder();
            for (Map.Entry<String, String> entry : edge.visualAttributes.entrySet()) {
                attributesStr.append(entry.getKey()).append(": ").append(entry.getValue()).append(", ");
            }
            // Delete the final comma and space
            if (attributesStr.length() > 2) {
                attributesStr.setLength(attributesStr.length() - 2);
            }
            System.out.format("%-15s%n", attributesStr.toString());
        }
    }

    // Function to convert the symbol table to an adjacency list and write it to a file
    public void convertToAdjacencyListAndWriteToFile(String filename) {
        Map<String, List<String>> adjacencyList = new HashMap<>();
        List<String> isolatedNodes = new ArrayList<>();

        // Build the adjacency list
        for (Edge edge : edges) {
            String sourceNode = edge.getSourceNode();
            String targetNode = edge.getTargetNode();
            String weight = edge.getWeigth();

            // Agregar la arista al nodo de origen
            adjacencyList.computeIfAbsent(sourceNode, k -> new ArrayList<>()).add(targetNode);
            adjacencyList.computeIfAbsent(sourceNode, k -> new ArrayList<>()).add(weight);

        }

        // Find isolated nodes
        for (Token token : tokens.values()) {
            String nodeName = token.getName();
            if (!adjacencyList.containsKey(nodeName)) {
                isolatedNodes.add(nodeName);
            }
        }

        // Write the adjacency list to a file
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            writer.write("Adjacency list:\n");
            for (Map.Entry<String, List<String>> entry : adjacencyList.entrySet()) {
                writer.write(entry.getKey() + " -> ");
                for (Integer i = 0; i < entry.getValue().size() - 1; i++) {
                    writer.write('(' + entry.getValue().get(i) + ',' + entry.getValue().get(i + 1) + ')' + ' ');
                    i++;
                }
                writer.newLine();
            }
            for (String isolatedNode : isolatedNodes) {
                writer.write(isolatedNode + " -> " + '_');
                writer.newLine();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}

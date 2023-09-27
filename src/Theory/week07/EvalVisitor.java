import java.util.HashMap;
import java.util.Map;

public class EvalVisitor extends LabeledExprBaseVisitor<Integer>{
    Map<String, Integer> memory = new HashMap<String, Integer>();
    
    @Override
    public Integer visitAssing(LabeledExprBaseVisitor.AssignContext ctx){
        String id = ctx.ID().getText();
        int value = visit(ctx.expre());
        memory.put(id, value);
        return value;
    }
}

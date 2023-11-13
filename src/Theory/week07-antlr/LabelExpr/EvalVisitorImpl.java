import java.util.HashMap;
import java.util.Map;


public class EvalVisitorImpl extends LabeledExprBaseVisitor<Integer> {
    Map<String, Integer> memory = new HashMap<String, Integer>();

    /* ID '=' expr NEWLINE */
    @Override
    public Integer visitAssing(LabeledExprParser.AssingContext ctx) {
        String id = ctx.ID().getText();
        int value = visit(ctx.expr());
        memory.put(id, value);
        return value;
    }

    /* expr NEWLINE */
    @Override
    public Integer visitPrintExpr(LabeledExprParser.PrintExprContext ctx) {
        Integer value = visit(ctx.expr());
        System.out.println(value);
        return 0;
    }

    /* INT */
    @Override
    public Integer visitInt(LabeledExprParser.IntContext ctx){
        return Integer.valueOf(ctx.INT().getText());
    }
    
    /* ID */
    public Integer visitId(LabeledExprParser.IdContext ctx){
        String id = ctx.ID().getText();
        if (memory.containsKey(id)) return memory.get(id);
        return 0;
    }

    /* expr ('*'|'/') expr */
    @Override
    public Integer visitMulDiv(LabeledExprParser.MulDivContext ctx){
        int left = visit(ctx.expr(0));
        int right = visit(ctx.expr(1));
        if(ctx.op.getType() == LabeledExprParser.MUL){
            return left*right;
        } else{
            return left / right;
        }
    }

    /* expr ('+'|'-') expr */
    @Override
    public Integer visitAddSub(LabeledExprParser.AddSubContext ctx){
        int left = visit(ctx.expr(0));
        int right = visit(ctx.expr(1));
        if(ctx.op.getType() == LabeledExprParser.ADD){
            return left+right;
        } else{
            return left - right;
        }
    }

    /* '(' expr ')' */
    @Override
    public Integer visitParens(LabeledExprParser.ParensContext ctx){
        return visit(ctx.expr());
    }
    
    
}

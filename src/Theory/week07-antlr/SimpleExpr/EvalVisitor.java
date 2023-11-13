package LabeledExpr;
import java.util.HashMap;
import java.util.Map;

import LabeledExprBaseVisitor;
import LabeledExprParser;
import LabeledExprParser.AddSubContext;
import LabeledExprParser.IdContext;
import LabeledExprParser.IntContext;
import LabeledExprParser.MulDivContext;
import LabeledExprParser.ParensContext;
import LabeledExprParser.PrintExprContext;

public class EvalVisitor extends LabeledExprBaseVisitor<Integer> {
    Map<String, Integer> memory = new HashMap<String, Integer>();

    /* ID '=' expr NEWLINE */
    @Override
    public Integer visitAssing(LabeledExprBaseVisitor.AssignContext ctx) {
        String id = ctx.ID().getText();
        int value = visit(ctx.expre());
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
        return Integer.valueOf(ctx.Int().getText());
    }
    
    /* ID */
    public Integer visitId(LabeledExprParser.IdContext ctx){
        String id = ctx.ID().getText();
        if (memory.containsKey(id)) return memory.get(id);
        return 0;
    }

    /* expr ('*'|'/') expr */
    @Override
    public Integer visitMultDiv(LabeledExprParser.MulDivContext ctx){
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
        if(ctx.op.getType() == LabeledExprParser.ADDL){
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

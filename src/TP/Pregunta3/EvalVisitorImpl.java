import java.util.HashMap;
import java.util.Map;

public class EvalVisitorImpl extends LabeledExprBaseVisitor<Double> {
    Map<String, Double> memory = new HashMap<String, Double>();

    /* ID '=' expr NEWLINE */
    @Override
    public Double visitAssing(LabeledExprParser.AssingContext ctx) {
        String id = ctx.ID().getText();
        Double value = visit(ctx.expr());
        memory.put(id, value);
        return value;
    }

    /* expr NEWLINE */
    @Override
    public Double visitPrintExpr(LabeledExprParser.PrintExprContext ctx) {
        Double value = visit(ctx.expr());
        System.out.println(value);
        return 0.0;
    }

    /* INT */
    @Override
    public Double visitInt(LabeledExprParser.IntContext ctx) {
        return Double.valueOf(ctx.INT().getText());
    }

    /* ID */
    public Double visitId(LabeledExprParser.IdContext ctx) {
        String id = ctx.ID().getText();
        if (memory.containsKey(id))
            return memory.get(id);
        return 0.0;
    }

    /* expr 'sqrt' # Sqrt */
    @Override
    public Double visitSqrt(LabeledExprParser.SqrtContext ctx) {
        Double left = visit(ctx.expr());
        return Math.sqrt(left);
    }

    /* expr op=('tan'|'sen') # TanSen */
    @Override
    public Double visitTanSen(LabeledExprParser.TanSenContext ctx) {
        Double left = visit(ctx.expr());
        if (ctx.op.getType() == LabeledExprParser.TAN) {
            return Math.tan(left);
        } else {
            return Math.sin(left);
        }
    }

    /* expr expr 'pow' # Pow */
    @Override
    public Double visitPow(LabeledExprParser.PowContext ctx) {
        Double left = visit(ctx.expr(0));
        Double right = visit(ctx.expr(1));
        Double ans = Math.pow(left, right);
        return ans;
    }

    /* expr expr 'mod' # Mod */
    @Override
    public Double visitMod(LabeledExprParser.ModContext ctx) {
        Double left = visit(ctx.expr(0));
        Double right = visit(ctx.expr(1));
        Double ans = (double) Math.floorMod(left.intValue(), right.intValue());
        return ans;
    }

    /* expr expr op=('*'|'/') # MulDiv */
    @Override
    public Double visitMulDiv(LabeledExprParser.MulDivContext ctx) {
        Double left = visit(ctx.expr(0));
        Double right = visit(ctx.expr(1));
        if (ctx.op.getType() == LabeledExprParser.MUL) {
            return left * right;
        } else {
            return left / right;
        }
    }

    /* expr op=('+'|'-') expr # AddSub */
    @Override
    public Double visitAddSub(LabeledExprParser.AddSubContext ctx) {
        Double left = visit(ctx.expr(0));
        Double right = visit(ctx.expr(1));
        if (ctx.op.getType() == LabeledExprParser.ADD) {
            return left + right;
        } else {
            return left - right;
        }
    }

    /* '(' expr ')' */
    @Override
    public Double visitParens(LabeledExprParser.ParensContext ctx) {
        return visit(ctx.expr());
    }
}

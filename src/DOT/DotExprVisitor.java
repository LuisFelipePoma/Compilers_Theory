// Generated from DotExpr.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link DotExprParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface DotExprVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link DotExprParser#graph}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitGraph(DotExprParser.GraphContext ctx);
	/**
	 * Visit a parse tree produced by the {@code StmtList}
	 * labeled alternative in {@link DotExprParser#stmt_list}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStmtList(DotExprParser.StmtListContext ctx);
	/**
	 * Visit a parse tree produced by the {@code SubGraph}
	 * labeled alternative in {@link DotExprParser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSubGraph(DotExprParser.SubGraphContext ctx);
	/**
	 * Visit a parse tree produced by the {@code Node}
	 * labeled alternative in {@link DotExprParser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNode(DotExprParser.NodeContext ctx);
	/**
	 * Visit a parse tree produced by the {@code Edge}
	 * labeled alternative in {@link DotExprParser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEdge(DotExprParser.EdgeContext ctx);
	/**
	 * Visit a parse tree produced by the {@code Attr}
	 * labeled alternative in {@link DotExprParser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAttr(DotExprParser.AttrContext ctx);
	/**
	 * Visit a parse tree produced by the {@code Assign}
	 * labeled alternative in {@link DotExprParser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAssign(DotExprParser.AssignContext ctx);
	/**
	 * Visit a parse tree produced by {@link DotExprParser#attr_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAttr_stmt(DotExprParser.Attr_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link DotExprParser#attr_list}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAttr_list(DotExprParser.Attr_listContext ctx);
	/**
	 * Visit a parse tree produced by {@link DotExprParser#a_list}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitA_list(DotExprParser.A_listContext ctx);
	/**
	 * Visit a parse tree produced by the {@code EdgeStmt}
	 * labeled alternative in {@link DotExprParser#edge_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEdgeStmt(DotExprParser.EdgeStmtContext ctx);
	/**
	 * Visit a parse tree produced by the {@code EdgeRhs}
	 * labeled alternative in {@link DotExprParser#edgeRHS}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEdgeRhs(DotExprParser.EdgeRhsContext ctx);
	/**
	 * Visit a parse tree produced by the {@code NodeStmt}
	 * labeled alternative in {@link DotExprParser#node_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNodeStmt(DotExprParser.NodeStmtContext ctx);
	/**
	 * Visit a parse tree produced by the {@code Id}
	 * labeled alternative in {@link DotExprParser#node_id}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitId(DotExprParser.IdContext ctx);
	/**
	 * Visit a parse tree produced by {@link DotExprParser#port}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitPort(DotExprParser.PortContext ctx);
	/**
	 * Visit a parse tree produced by the {@code SubGraphBody}
	 * labeled alternative in {@link DotExprParser#subgraph}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSubGraphBody(DotExprParser.SubGraphBodyContext ctx);
	/**
	 * Visit a parse tree produced by {@link DotExprParser#compass_pt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCompass_pt(DotExprParser.Compass_ptContext ctx);
}
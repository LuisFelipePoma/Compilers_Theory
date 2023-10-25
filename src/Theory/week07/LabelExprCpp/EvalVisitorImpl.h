#include "libs/LabeledExprBaseVisitor.h"
#include "libs/LabeledExprParser.h"
#include <any>
#include <iostream>
#include <map>
#include <string>

/* added the Impl at the end of the class to avoid it being .gitignored sorry */
class EvalVisitorImpl : LabeledExprBaseVisitor
{
	std::map<std::string, std::any> memory;

public:
	/* ID '=' expr NEWLINE */
	std::any visitAssign(LabeledExprParser::AssingContext *ctx)
	{
		std::string id = ctx->ID()->getText();
		auto value = visit(ctx->expr());
		memory[id] = value;
		return std::any(value);
	}

	/* expr NEWLINE */
	std::any visitPrintExpr(LabeledExprParser::PrintExprContext *ctx)
	{
		int value = std::any_cast<int>(visit(ctx->expr()));
		std::cout << value << '\n';
		return std::any(value);
	}

	/* INT */
	std::any visitInt(LabeledExprParser::IntContext *ctx)
	{
		return std::any(std::stoi(ctx->INT()->getText()));
	}

	/* ID */
	std::any visitId(LabeledExprParser::IdContext *ctx)
	{
		std::string id = ctx->ID()->getText();
		if (memory.count(id))
			return memory[id];
		return std::any(id);
	}

	/* expr op=('*'|'/') expr */
	std::any visitMulDiv(LabeledExprParser::MulDivContext *ctx)
	{
		auto left = std::any_cast<int>(visit(ctx->expr(0)));
		auto right = std::any_cast<int>(visit(ctx->expr(1)));
		int resp;
		if (ctx->op->getType() == LabeledExprParser::MUL)
		{
			resp = left * right;
		}
		else
		{
			resp = left / right;
		}
		return std::any(resp);
	}

	/* expr op=('+'|'-') expr */
	std::any visitAddSub(LabeledExprParser::AddSubContext *ctx)
	{
		auto left = std::any_cast<int>(visit(ctx->expr(0)));
		auto right = std::any_cast<int>(visit(ctx->expr(1)));
		int resp;
		if (ctx->op->getType() == LabeledExprParser::ADD)
		{
			resp = left + right;
		}
		else
		{
			resp = left - right;
		}
		return std::any(resp);
	}

	/* '(' expr ')' */
	std::any visitParens(LabeledExprParser::ParensContext *ctx)
	{
		return visit(ctx->expr());
	}

	std::any visitProg(LabeledExprParser::ProgContext *ctx)
	{
		// for (size_t i = 0; i > ctx->children.capacity(); i++)
		// {
		return visitChildren(ctx);
		// }
	}
};
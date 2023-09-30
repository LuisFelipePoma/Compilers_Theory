#include "libs/LabeledExprBaseVisitor.h"
#include "libs/LabeledExprParser.h"
#include <any>
#include <iostream>
#include <map>
#include <string>

using namespace std;

class EvalVisitorImpl : public LabeledExprBaseVisitor
{
    std::map<string, any> memory;

public:
    /* ID '=' expr NEWLINE */
    any visitAssing(LabeledExprParser::AssingContext *ctx)
    {
        string id = ctx->ID()->getText();
        auto value = visitChildren(ctx->expr());
        memory[id] = value;
        return value;
    }
    /* expr NEWLINE */
    any visitPrintExpr(LabeledExprParser::PrintExprContext *ctx)
    {
        auto value = visitChildren(ctx->expr());
        cout << any_cast<int>(value) << '\n';
        return any();
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
        return std::any();
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
        return visitChildren(ctx->expr());
    }

    std::any visitProg(LabeledExprParser::ProgContext *ctx)
    {
        return visitChildren(ctx);
    }
};
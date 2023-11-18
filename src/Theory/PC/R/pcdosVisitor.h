
// Generated from pcdos.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "pcdosParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by pcdosParser.
 */
class  pcdosVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by pcdosParser.
   */
    virtual std::any visitProg(pcdosParser::ProgContext *context) = 0;

    virtual std::any visitPrintExpr(pcdosParser::PrintExprContext *context) = 0;

    virtual std::any visitAssign(pcdosParser::AssignContext *context) = 0;

    virtual std::any visitStatdef(pcdosParser::StatdefContext *context) = 0;

    virtual std::any visitStatextern(pcdosParser::StatexternContext *context) = 0;

    virtual std::any visitBlank(pcdosParser::BlankContext *context) = 0;

    virtual std::any visitCall(pcdosParser::CallContext *context) = 0;

    virtual std::any visitNumber(pcdosParser::NumberContext *context) = 0;

    virtual std::any visitMulDiv(pcdosParser::MulDivContext *context) = 0;

    virtual std::any visitAddSub(pcdosParser::AddSubContext *context) = 0;

    virtual std::any visitParens(pcdosParser::ParensContext *context) = 0;

    virtual std::any visitId(pcdosParser::IdContext *context) = 0;

    virtual std::any visitProto(pcdosParser::ProtoContext *context) = 0;

    virtual std::any visitDef(pcdosParser::DefContext *context) = 0;

    virtual std::any visitExtern(pcdosParser::ExternContext *context) = 0;


};



// Generated from pcdos.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "pcdosVisitor.h"


/**
 * This class provides an empty implementation of pcdosVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  pcdosBaseVisitor : public pcdosVisitor {
public:

  virtual std::any visitProg(pcdosParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrintExpr(pcdosParser::PrintExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign(pcdosParser::AssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatdef(pcdosParser::StatdefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatextern(pcdosParser::StatexternContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlank(pcdosParser::BlankContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCall(pcdosParser::CallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumber(pcdosParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMulDiv(pcdosParser::MulDivContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddSub(pcdosParser::AddSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParens(pcdosParser::ParensContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitId(pcdosParser::IdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProto(pcdosParser::ProtoContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDef(pcdosParser::DefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExtern(pcdosParser::ExternContext *ctx) override {
    return visitChildren(ctx);
  }


};


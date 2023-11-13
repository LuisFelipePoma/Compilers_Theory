#include "llvm/ADT/APInt.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

static Function *CreateFactorialFunction(Module *M, LLVMContext &Context) {
  // Create the factorial function and insert it into module M
  FunctionType *FactFTy = FunctionType::get(Type::getInt32Ty(Context),
                                           {Type::getInt32Ty(Context)}, false);
  Function *FactF =
      Function::Create(FactFTy, Function::ExternalLinkage, "factorial", M);

  // Add a basic block to the function.
  BasicBlock *BB = BasicBlock::Create(Context, "EntryBlock", FactF);

  // Get pointers to the constants.
  Value *One = ConstantInt::get(Type::getInt32Ty(Context), 1);

  // Get pointer to the integer argument of the factorial function...
  Argument *ArgN = &*FactF->arg_begin(); // Get the arg.
  ArgN->setName("AnArg");            // Give it a nice symbolic name for fun.

  // Create the true_block.
  BasicBlock *RetBB = BasicBlock::Create(Context, "return", FactF);
  // Create an exit block.
  BasicBlock* RecurseBB = BasicBlock::Create(Context, "recurse", FactF);

  // Create the "if (arg <= 1) goto exitbb"
  Value *CondInst = new ICmpInst(*BB, ICmpInst::ICMP_SLE, ArgN, One, "cond");
  BranchInst::Create(RetBB, RecurseBB, CondInst, BB);

  // Create: ret int 1
  ReturnInst::Create(Context, One, RetBB);

  // create fact(n-1)
  Value *Sub = BinaryOperator::CreateSub(ArgN, One, "arg", RecurseBB);
  CallInst *CallFactN1 = CallInst::Create(FactF, Sub, "factn1", RecurseBB);
  CallFactN1->setTailCall();

  // n * fact(n-1)
  Value *Factorial = BinaryOperator::CreateMul(ArgN, CallFactN1,
                                         "factresult", RecurseBB);

  // Create the return instruction and add it to the basic block
  ReturnInst::Create(Context, Factorial, RecurseBB);

  return FactF;
}

int main(int argc, char **argv) {
  int n = argc > 1 ? atoi(argv[1]) : 5; // Default value is 5

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  LLVMContext Context;

  // Create some module to put our function into it.
  std::unique_ptr<Module> Owner(new Module("factorial", Context));
  Module *M = Owner.get();

  // We are about to create the "factorial" function:
  Function *FactF = CreateFactorialFunction(M, Context);

  // Now we going to create JIT
  std::string errStr;
  ExecutionEngine *EE =
    EngineBuilder(std::move(Owner))
    .setErrorStr(&errStr)
    .create();

  if (!EE) {
    errs() << argv[0] << ": Failed to construct ExecutionEngine: " << errStr
           << "\n";
    return 1;
  }

  errs() << "verifying... ";
  if (verifyModule(*M)) {
    errs() << argv[0] << ": Error constructing function!\n";
    return 1;
  }

  errs() << "OK\n";
  errs() << "We just constructed this LLVM module:\n\n---------\n" << *M;
  errs() << "---------\nstarting factorial(" << n << ") with JIT...\n";

  // Call the factorial function with argument n:
  std::vector<GenericValue> Args(1);
  Args[0].IntVal = APInt(32, n);
  GenericValue GV = EE->runFunction(FactF, Args);

  // Import result of execution
  outs() << "Result: " << GV.IntVal << "\n";

  return 0;
}

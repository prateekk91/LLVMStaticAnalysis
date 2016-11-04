#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include <map>
#include <string>
# include<vector>
# define IF_TRUE "if.then"
# define COND_TRUE "cond.true"
# define IF_FALSE "if.else"
# define COND_FALSE "cond.false"
# define RETURN 1

using namespace llvm;

namespace {
  struct BranchBias : public FunctionPass {
    BranchBias() : FunctionPass(ID) {}
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnFunction(Function &F) {
      
      std::vector<Instruction*> moduleInstructions;
      Module *mod = F.getParent();
      LLVMContext &Context = mod->getContext();
      Constant *c = mod->getOrInsertFunction("_Z14incrementTotalPc", Type::getVoidTy(Context), Type::getInt8PtrTy(Context), NULL);
	  Function *incrementTotal = cast<Function>(c);
      Constant *c2 = mod->getOrInsertFunction("_Z14incrementTakenPc", Type::getVoidTy(Context), Type::getInt8PtrTy(Context), NULL);
	  Function *incrementTaken = cast<Function>(c2);
	  Constant *c3 = mod->getOrInsertFunction("_Z5printv", Type::getVoidTy(Context), NULL);
	  Function *print = cast<Function>(c3);
	
	//If the function is main, insert a call to print before return instruction
	 if(strcmp(F.getName().str().c_str(), "main") == 0)
  	 {
  	 	Instruction *finalInstruction;
  	 	 for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
  	 	 {
  	 	 	if(I->getOpcode() == RETURN)
  	 	 	{
  	 	 		finalInstruction = &*I;
  	 	 		break;
  	 	 	}
  	 	 }
  	 	 IRBuilder<> builder(finalInstruction);
		 builder.SetInsertPoint(finalInstruction);
		 builder.CreateCall(print); 
 	 }
 	 
 	   
    for (Function::iterator fit = F.begin(), e = F.end(); fit != e; ++fit)
  	{
  		const char *label = fit->getName().str().c_str();
  		 
  		 //If label is true, add call to increment total and taken,
  		 //otherwise add call to increment total.
  		 //At runtime, only one will be executed, based on condition evaluation.
  		if (std::strstr(label, IF_TRUE) || std::strstr(label, COND_TRUE))
  		{
  			IRBuilder<> builder(fit);
  			builder.SetInsertPoint(fit->getFirstNonPHIOrDbg());
  			std::vector<Value *> args;
  			Value* a = builder.CreateGlobalStringPtr(F.getName().str().c_str());
	  	 	args.push_back(a);
	  	 	builder.CreateCall(incrementTotal,args);
	  	 	builder.CreateCall(incrementTaken,args);
  		}
  		if (std::strstr(label, IF_FALSE) || std::strstr(label, COND_FALSE))
  		{
  			IRBuilder<> builder(fit);
  			builder.SetInsertPoint(fit->getFirstNonPHIOrDbg());
  			std::vector<Value *> args;
	  	 	Value* a = builder.CreateGlobalStringPtr(F.getName().str().c_str());
	  	 	args.push_back(a);
	  	 	builder.CreateCall(incrementTotal,args);
  		}
  	 }
  	 	 
      return false;
    }
  };
}
char BranchBias::ID = 0;
static RegisterPass<BranchBias> X("branchBias", "Branch Bias");

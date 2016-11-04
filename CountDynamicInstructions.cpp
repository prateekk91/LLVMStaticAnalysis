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
# define PHI 48
# define INT_SIZE 32

using namespace llvm;

namespace {
  struct CountDynamicInstructions : public BasicBlockPass {
    CountDynamicInstructions() : BasicBlockPass(ID) {}
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnBasicBlock(BasicBlock &BB) {
      
      std::vector<Instruction*> moduleInstructions;
      Function *fun = BB.getParent();
      Module *mod = fun->getParent();
      
      for (BasicBlock::iterator bbit = BB.begin(), bbie = BB.end(); bbit != bbie; ++bbit) 
      { 
		   Instruction *i = bbit;
		   moduleInstructions.push_back(i);
  	 }
  	 
  	 IRBuilder<> builder(&BB);
  	 LLVMContext &Context = mod->getContext();
  	 
  	 Constant *c = mod->getOrInsertFunction("_Z5counti", Type::getVoidTy(Context), IntegerType::get(Context, INT_SIZE), NULL);
	 Constant *c2 = mod->getOrInsertFunction("_Z5printv", Type::getVoidTy(Context), NULL);
	 Function *count = cast<Function>(c);
     Function *print = cast<Function>(c2);
     
     for (int i=0; i<moduleInstructions.size();i++)
  	 {
  	 	int opcode = moduleInstructions[i]->getOpcode();
  	 	//PHI instruction can only be the first instruction of basic block
  	 	//For phi, insert a count instruction after the instruction
  	 	//For all other instruction, insert it before the instruction
  	 	if (opcode == PHI)
  	 		builder.SetInsertPoint(moduleInstructions[i]->getNextNode());
  	 	else
  	 		builder.SetInsertPoint(moduleInstructions[i]);
  	 	ConstantInt *a = builder.getInt32(opcode);
  	 	builder.CreateCall(count,a);
  	 	
  	 	//If the function happens to be main, and the next instruction is return, we have reached the end of program
  	 	//Here we insert a call to print function, that prints whatever we calculated
  	 	if(strcmp(fun->getName().str().c_str(), "main") == 0 && opcode == 1)
	  	 {
	  	 	builder.SetInsertPoint(moduleInstructions[i]);
	  	 	builder.CreateCall(print);
	  	 }
  	 }
  	 
      return false;
    }
  };
}
char CountDynamicInstructions::ID = 0;
static RegisterPass<CountDynamicInstructions> X("countDynamic", "Dynamic Pass");

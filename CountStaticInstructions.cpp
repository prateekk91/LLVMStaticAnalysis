#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include <map>
#include <string>
# include<iostream>
# include<stdio.h>
# include<iomanip>
using namespace llvm;
using namespace std;
static std::map<std::string,int> instructionCount;
static int totalInstructions;

namespace {
  struct CountStaticInstructions : public FunctionPass {
    CountStaticInstructions() : FunctionPass(ID) {}
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnFunction(Function &F) {
      for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) 
      {
      	  totalInstructions++;
		  instructionCount[I->getOpcodeName()]++;
	  }
	  
      
      return false;
    }
    
    virtual bool doFinalization(Module &M)
    {
    	//Raw stream does not support setw and indentation
    	//Hence printing with cerr on strerr.
    	std::map<std::string, int>::iterator it = instructionCount.begin();
    	while(it != instructionCount.end())
   		{
		    cerr << setw(30) << left << it->first<< setw(10) << right << it->second<<"\n";
		    it++;
   		}
   		cerr << setw(30) << left << "Total: " << setw(10) << right << totalInstructions << "\n";
   		return false;
    }
  };
}
char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> X("countStatic", "Static Pass");

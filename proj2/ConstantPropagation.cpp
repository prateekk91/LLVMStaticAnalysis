#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
# include "llvm/Support/Casting.h"
# include "CPFlowFunction.h"
# include "CPLatticePoint.h"
# include "WorkList.h"
#include <map>
# include<set>
#include <string>
# include<iostream>
# include<stdio.h>
# include<iomanip>

using namespace std;
static std::map<std::string,int> instructionCount;

namespace {
  struct ConstantPropagation : public FunctionPass {
    ConstantPropagation() : FunctionPass(ID) {}
//    map <Instruction*, set <pair <int, int> > lattice;
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnFunction(Function &F) {
    	map<Value*, ConstantInt*> cmap;
  	  	map <Instruction*, LatticePoint*> result;
  	  	set <Instruction*> instructionsToDelete;
		WorkList worklist;
		result = worklist.analyze(F, NULL, WorkList::ConstantPropagation);
		for (map<Instruction*, LatticePoint*>::iterator it = result.begin(); it!=result.end();it++)
		{
			map<Value*, ConstantInt*> ans = (dyn_cast<CPLatticePoint>(it->second))->latticePoint;
			for(map<Value*, ConstantInt*>::iterator it2 = ans.begin(); it2!=ans.end();it2++)
			{
				it2->first->replaceAllUsesWith(it2->second);
				if (isa<Instruction>(it2->first))
					instructionsToDelete.insert(dyn_cast<Instruction>(it2->first));
			}
		}
		
		for (set<Instruction*>::iterator it = instructionsToDelete.begin(); it != instructionsToDelete.end() ; it++)
		{
			errs() << "Removing instruction:" << *(*it) << "\n";
			(*it)->eraseFromParent();
		}
      	return false;
    }
  };
}
char ConstantPropagation::ID = 0;
static RegisterPass<ConstantPropagation> X("constantProp", "Constant Propagation Pass");

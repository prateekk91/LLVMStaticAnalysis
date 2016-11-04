# include "CPFlowFunction.h"
# include "CPLatticePoint.h"
# include "CSEFlowFunction.h"
# include "CSELatticePoint.h"
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
  struct CommonSubexpressionElimination : public FunctionPass {
    CommonSubexpressionElimination() : FunctionPass(ID) {}
//    map <Instruction*, set <pair <int, int> > lattice;
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnFunction(Function &F) {
    	map <Instruction*, LatticePoint*> result;
		WorkList worklist;
		result = worklist.analyze(F, NULL, WorkList::CommonSubexpressionElimination);
		set <Instruction*> instructionsToDelete;
		for (map<Instruction*, LatticePoint*>::iterator it = result.begin(); it!=result.end();it++)
		{
			map<Value*, Instruction*> ans = (dyn_cast<CSELatticePoint>(it->second))->latticePoint;
			for(map<Value*, Instruction*>::iterator it2 = ans.begin(); it2!=ans.end();it2++)
			{
				if (it2->first != it2->second)
				{
					it2->first->replaceAllUsesWith(it2->second);
					if (isa<Instruction>(it->first))
					{
						Instruction *I = dyn_cast<Instruction>(it2->first);
						instructionsToDelete.insert(I);
					}
				}
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
char CommonSubexpressionElimination::ID = 0;
static RegisterPass<CommonSubexpressionElimination> X("csePass", "Common Subexpression Elimination Pass");

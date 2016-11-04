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
  struct RangeAnalysis : public FunctionPass {
    RangeAnalysis() : FunctionPass(ID) {}
//    map <Instruction*, set <pair <int, int> > lattice;
	static char ID; // Pass identification, replacement for typeid
  
    virtual bool runOnFunction(Function &F) {
    	map<Value*, ConstantInt*> cmap;
    	map <Instruction*, LatticePoint*> result;
    	
  	  	WorkList worklist;
		result = worklist.analyze(F, NULL, WorkList::RangeAnalysis);
//		for (map<Instruction*, LatticePoint*>::iterator it = result.begin(); it!=result.end();it++)
//		{
//			errs() << *(it->first) << "\t::\t";
//			(dyn_cast<RangeLatticePoint>(it->second)) -> printLattice();
//		}

		for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
		{
			Instruction *instruction = &(*I);
			RangeLatticePoint *actualResult = dyn_cast<RangeLatticePoint>(result[instruction]);
			map<Value*, ConstantRange*> infoMap = actualResult->latticePoint;
			if (isa<GetElementPtrInst>(instruction)) 
			{
		    	GetElementPtrInst* gep = cast<GetElementPtrInst>(instruction);
		    	PointerType* pointerType = dyn_cast<PointerType>(gep->getPointerOperandType());
		   		Type* elementType = pointerType->getElementType();
		    
		    	if (isa<ArrayType>(elementType)) 
		    	{
				  	ArrayType* arrayType = cast<ArrayType>(elementType);
				  	int arraySize = arrayType->getNumElements();
					Value* accessedIndex = gep->getOperand(2);
					ConstantRange* allowedRange = new ConstantRange(APInt(32, 0), APInt(32, arraySize));
				    ConstantRange* accessedIndexRange = NULL;
				  	if (infoMap.count(accessedIndex) > 0) 
				  	{
				    	accessedIndexRange = infoMap[accessedIndex];
				    }
				  	else if (isa<ConstantInt>(accessedIndex))
				  	{
						ConstantInt* accessedIndexValue = dyn_cast<ConstantInt>(accessedIndex);
						accessedIndexRange = new ConstantRange(accessedIndexValue->getValue());
					}
				  	if (accessedIndexRange != NULL)
				  	{
				  		if (!allowedRange->contains(*accessedIndexRange)) 
						{
							errs() << "Warning: Array index out of bounds exception possible at instruction: " << *I << "\n";
							errs() << "Array scope: " << *allowedRange << "\n";
							errs() << "Array index accessed is in range: " << *accessedIndexRange << "\n\n";
						}
				  	}
		    	}
			}
		}
      	return false;
    }
  };
}
char RangeAnalysis::ID = 0;
static RegisterPass<RangeAnalysis> X("rangeAnalysis", "Range Analysis Pass");

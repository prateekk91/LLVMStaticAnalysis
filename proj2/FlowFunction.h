# ifndef _FlowFunction_
# define _FlowFunction_
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
# include "llvm/Support/Casting.h"
# include "LatticePoint.h"
#include <map>
# include<set>
#include <string>
# include<iostream>
# include<stdio.h>
# include<iomanip>
using namespace llvm;
using namespace std;
class FlowFunction
{
	public:
	virtual LatticePoint* applyBinaryFlowFunction(BinaryOperator&, LatticePoint* info_in) = 0;
	virtual LatticePoint* applyPHIFlowFunction(PHINode&, LatticePoint* info_in) = 0;
	virtual LatticePoint* applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in) = 0;
	virtual map<BasicBlock*, LatticePoint*> applyTerminatorFlowFunction(TerminatorInst&, LatticePoint* info_in, vector<BasicBlock*> &successors) = 0;
};
#endif

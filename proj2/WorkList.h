# ifndef _WORKLIST_
# define _WORKLIST_
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
# include "llvm/Support/Casting.h"
#include <map>
# include<set>
#include <string>
# include<iostream>
# include<stdio.h>
# include<iomanip>
# include<queue>
# include "CPLatticePoint.h"
# include "CPFlowFunction.h"
# include "CSEFlowFunction.h"
# include "CSELatticePoint.h"
# include "RangeLatticePoint.h"
# include "RangeFlowFunction.h"
using namespace std;
class WorkList
{
	public:
	enum AType {ConstantPropagation, CommonSubexpressionElimination, RangeAnalysis};
	
	private:
	set<BasicBlock*> workList;
	map <BasicBlock*, vector<BasicBlock*> > predecessorMap;
	map <BasicBlock*, vector<BasicBlock*> > successorMap;
	map <Instruction*, BasicBlock*> terminatorMap;
	map < pair <BasicBlock*, BasicBlock*>, LatticePoint*> edgeMap;
	AType analysisType;
	FlowFunction* getFlowFunction();
	void populateMaps(Function &F);
	LatticePoint* getInitialInfo();
	map<BasicBlock*, LatticePoint*> applyFlowFunctions(BasicBlock* node, LatticePoint* info_in);
	map<Instruction*, LatticePoint*> result;
	map<BasicBlock*, LatticePoint*> basicBlockBeginning;
	map<BasicBlock*, LatticePoint*> basicBlockEnding;
	map<Instruction*, LatticePoint*> instructionBeginning;
	map<Instruction*, LatticePoint*> instructionEnding;
	LatticePoint* getInfoIn(BasicBlock* node, vector<BasicBlock*> predecessors, map< pair<BasicBlock*, BasicBlock*>, LatticePoint*> edgeMap);
	void printComputationResult();
	public:
	map<Instruction*, LatticePoint*> analyze(Function &F, LatticePoint *inputValues, WorkList::AType analysisType);
};
#endif

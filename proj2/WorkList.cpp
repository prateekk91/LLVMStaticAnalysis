# include "WorkList.h"

LatticePoint* WorkList::getInitialInfo()
{
	LatticePoint *latticePoint = NULL;
	
	if (analysisType == ConstantPropagation)
	{
		latticePoint = new CPLatticePoint(true, false, map<Value*, ConstantInt*>());
	}
	else if (analysisType == CommonSubexpressionElimination)
	{
		latticePoint = new CSELatticePoint(true, false, map<Value*, Instruction*>());
	}
	else if (analysisType == RangeAnalysis)
	{
		latticePoint = new RangeLatticePoint(true, false, map<Value*, ConstantRange*>());
	}
	return latticePoint;
}

void WorkList::populateMaps(Function &func)
{
	for (Function::iterator i = func.begin(), e = func.end(); i != e; ++i)
	{
		workList.insert(i);
		TerminatorInst* terminator = i->getTerminator();
		terminatorMap[terminator] = i;
		for (unsigned int j = 0, end = terminator->getNumSuccessors(); j < end; j++) 
		{
			BasicBlock *successor = terminator->getSuccessor(j);
	        successorMap[i].push_back(successor);
	        edgeMap[ make_pair(i, successor) ] = getInitialInfo();
	        edgeMap[ make_pair(successor, i)] = getInitialInfo();
	        predecessorMap[successor].push_back(i);
        }
    }
}

FlowFunction* WorkList::getFlowFunction()
{
	switch (analysisType)
	{
		case WorkList::ConstantPropagation:
			return new CPFlowFunction();
			break;
		case WorkList::CommonSubexpressionElimination:
			return new CSEFlowFunction();
			break;
		case WorkList::RangeAnalysis:
			return new RangeFlowFunction();
			break;
		default: break;
	}
	return NULL;
}

map<BasicBlock*, LatticePoint*> WorkList::applyFlowFunctions(BasicBlock *node, LatticePoint *infoIn)
{
	map<BasicBlock*, LatticePoint *>info_out;
	LatticePoint *info = NULL;
	if (infoIn == NULL)
		infoIn = getInitialInfo();
	FlowFunction *fp;
	fp = getFlowFunction();
	for (BasicBlock::iterator i = node->begin(), e = node->end(); i != e; ++i)
	{
		Instruction *I = *(&i);
		info = info == NULL ? infoIn : info;
		
		//Get information at the beginning of basic block
		if (i == node->begin())
		{
			basicBlockBeginning[node] = info;
		}
		
		//Get information at the beginning of instruction
		instructionBeginning[I] = info;
		
		if (isa<BinaryOperator>(I))
			info = fp->applyBinaryFlowFunction(*(dyn_cast<BinaryOperator>(I)), info);
		else if (isa<PHINode>(I))
			info = fp->applyPHIFlowFunction(*(dyn_cast<PHINode>(I)), info);
		else if (I == node->getTerminator())
		{
			info_out = fp->applyTerminatorFlowFunction(*(dyn_cast<TerminatorInst>(I)), info, successorMap[node]);
			
			//Get information at the end of each Basic Block
			basicBlockEnding[node] = info;
		}
		result[I] = info;
			
		//Get information computed at the end of each instruction
		instructionEnding[I] = info;
	}
	return info_out;
}

LatticePoint* WorkList::getInfoIn(BasicBlock* node, vector<BasicBlock*> predecessors, map< pair<BasicBlock*, BasicBlock*>, LatticePoint*> edgeMap)
{
	LatticePoint* infoIn = getInitialInfo();
	for (unsigned int i=0;i<predecessors.size();i++)
	{
		if (infoIn->equals(getInitialInfo()))
		{
			infoIn = edgeMap.find(make_pair(predecessors[i], node))->second;
		}
		else
		{
			infoIn = infoIn->join(edgeMap.find(make_pair(predecessors[i], node))->second);
		}
	}
	return infoIn;
}

map<Instruction*, LatticePoint*> WorkList::analyze(Function &F, LatticePoint *inputValues, WorkList::AType analysisType)
{
	this->analysisType = analysisType;
	populateMaps(F);
	
	while(!workList.empty())
	{
		BasicBlock* node = *(workList.begin());
		workList.erase(workList.begin());
//		errs() << node->getName() << "\n";
		
		vector<BasicBlock*> predecessors = predecessorMap[node];
		LatticePoint* infoIn = getInfoIn(node, predecessors, edgeMap);
//		infoIn->printLattice();
		map<BasicBlock*, LatticePoint*> info_out = applyFlowFunctions(node, infoIn);
		map<BasicBlock*, LatticePoint*>::iterator it = info_out.begin();
		while (it != info_out.end())
		{
			BasicBlock* successor = it->first;
			LatticePoint* previousInfo = edgeMap[make_pair(node, successor)];
			LatticePoint* newInfo = it->second;
			if (! (newInfo->equals(previousInfo)))
			{
				edgeMap[make_pair(node, it->first)] = it->second;
				workList.insert(successor);
			}
			it++;
		}
	}
	printComputationResult();
	return result;
}

void WorkList::printComputationResult()
{
	errs() << "Information for each Basic Block:\n";
	
	for (map<BasicBlock*, LatticePoint*>::iterator it = basicBlockBeginning.begin(); it != basicBlockBeginning.end(); it++)
	{
		if (it->first)
			errs() << it->first->getName() << ":\n";
		errs() << "Beginning::\t";
		if (it->second)
			it->second->printLattice();
		errs() << "Ending::\t";
		if(basicBlockEnding[it->first])
			basicBlockEnding[it->first]->printLattice();
		errs() << "\n";
	}
	
	errs() << "\n\nInformation for each instruction:\n";
	for (map<Instruction*, LatticePoint*>::iterator it = instructionBeginning.begin(); it != instructionBeginning.end(); it++)
	{
		if (it->first)
			errs() << *(it->first) << ":\n";
		errs() << "Beginning::\t";
		if (it->second)
			it->second->printLattice();
		errs() << "Ending::\t";
		if (instructionEnding[it->first])
			instructionEnding[it->first]->printLattice();
		errs() << "\n";
	}
}

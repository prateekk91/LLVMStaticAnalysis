# include "CSEFlowFunction.h"

LatticePoint* CSEFlowFunction::applyBinaryFlowFunction(BinaryOperator &BO, LatticePoint* info_in)
{
	CSELatticePoint* out;
	CSELatticePoint* actual_in = dyn_cast<CSELatticePoint>(info_in);
	if (actual_in == NULL)
	{
//		errs() << "It is null.. It shouldn't be null\n";
	}
	map<Value*, Instruction*> infoMap = actual_in->latticePoint;
	bool found = false;
	for (map<Value*, Instruction*>::iterator it = infoMap.begin(); it!=infoMap.end();it++)
	{
		Instruction *info = it->second;
		if (info->isIdenticalTo(&BO))
		{
			infoMap[&BO] = info;
			found = true;
		}
	}
	if (!found)
	{
		infoMap[&BO] = &BO;
	}
	out = new CSELatticePoint(false, false, infoMap);
	return out;
}

LatticePoint* CSEFlowFunction::applyPHIFlowFunction(PHINode& phiNode, LatticePoint* info_in)
{
	return info_in;
}

LatticePoint* CSEFlowFunction::applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in)
{
	return info_in;
}

map<BasicBlock*, LatticePoint*> CSEFlowFunction::applyTerminatorFlowFunction(TerminatorInst&, LatticePoint* info_in, vector<BasicBlock*> &successors)
{
	map<BasicBlock*, LatticePoint*> info_out;
	for (unsigned int i=0;i<successors.size();i++)
		info_out[successors[i]] = info_in;
	return info_out;
}




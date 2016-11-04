# ifndef _CSEFLOWFUNCTION_H_
# define _CSEFLOWFUNCTION_H_
# include "FlowFunction.h"
# include "CSELatticePoint.h"
class CSEFlowFunction : public FlowFunction
{
	public:
	virtual LatticePoint* applyBinaryFlowFunction(BinaryOperator&, LatticePoint* info_in);
	virtual LatticePoint* applyPHIFlowFunction(PHINode&, LatticePoint* info_in);
	virtual LatticePoint* applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in);
	virtual map<BasicBlock*, LatticePoint*> applyTerminatorFlowFunction(TerminatorInst&, LatticePoint* info_in, vector<BasicBlock*> &successors);
};
# endif

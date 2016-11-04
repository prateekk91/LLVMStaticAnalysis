# ifndef _CPFlowFunction_
# define _CPFlowFunction_
# include "FlowFunction.h"
# include "CPLatticePoint.h"
using namespace std;
class CPFlowFunction : public FlowFunction
{
	public:
	virtual LatticePoint* applyBinaryFlowFunction(BinaryOperator&, LatticePoint* info_in);
	virtual LatticePoint* applyPHIFlowFunction(PHINode&, LatticePoint* info_in);
	virtual LatticePoint* applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in);
	virtual map<BasicBlock*, LatticePoint*> applyTerminatorFlowFunction(TerminatorInst&, LatticePoint* info_in, vector<BasicBlock*> &successors);
	virtual ~CPFlowFunction();
};
#endif

# ifndef _RangeFlowFunction_
# define _RangeFlowFunction_
# include "FlowFunction.h"
# include "RangeLatticePoint.h"
using namespace std;
class RangeFlowFunction : public FlowFunction
{
	public:
	virtual LatticePoint* applyBinaryFlowFunction(BinaryOperator&, LatticePoint* info_in);
	virtual LatticePoint* applyPHIFlowFunction(PHINode&, LatticePoint* info_in);
	virtual LatticePoint* applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in);
	virtual map<BasicBlock*, LatticePoint*> applyTerminatorFlowFunction(TerminatorInst&, LatticePoint* info_in, vector<BasicBlock*> &successors);
	virtual ~RangeFlowFunction();
};
#endif

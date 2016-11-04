# include "RangeFlowFunction.h"
# define MAX_ALLOWED_ITERATIONS 1000
ConstantRange* getUpdatedRange(int opCode, ConstantRange range1, ConstantRange range2)
{
	ConstantRange* updatedRange = new ConstantRange(32, false);
	switch(opCode)
	{
		case Instruction::Add:
			*updatedRange = range1.add(range2);
			break;
		case Instruction::Sub:
			*updatedRange = range1.sub(range2);
			break;
		case Instruction::UDiv:
			*updatedRange = range1.udiv(range2);
			break;
		case Instruction::Mul:
			*updatedRange = range1.multiply(range2);
			break;
	}
	return updatedRange;
}

static map<Value*, int> countChangedInstances;

LatticePoint* RangeFlowFunction::applyBinaryFlowFunction(BinaryOperator &BO, LatticePoint* info_in)
{
	RangeLatticePoint* out;
	RangeLatticePoint* actual_in = dyn_cast<RangeLatticePoint>(info_in);
	if (actual_in == NULL)
	{
		//errs() << "Its null"; Shouldn't be null
		return NULL;
	}
	map<Value*, ConstantRange*> infoMap = actual_in->latticePoint;
	
	Value *val1 = BO.getOperand(0);
	Value *val2 = BO.getOperand(1);
  	
  	ConstantRange* range1 = NULL;
 	ConstantRange* range2 = NULL;
	
	if (isa<ConstantInt>(val1)) 
  	{
		range1 = new ConstantRange( dyn_cast<ConstantInt>(val1) -> getValue());
  	} 
  	else if (infoMap.count(val1) > 0) 
  	{
  		range1 = infoMap[val1];
  	} 
  	if (isa<ConstantInt>(val2)) 
	{
		range2 = new ConstantRange( dyn_cast<ConstantInt>(val2) -> getValue());
	}
	else if (infoMap.count(val2) > 0) 
	{
		range2 = infoMap[val2];
	}
	
	if (countChangedInstances.count(&BO) <= 0)
			countChangedInstances[&BO] = 0;
	else
	{
		if (infoMap.count(&BO) && (*infoMap[&BO] != *(getUpdatedRange(BO.getOpcode(), *range1, *range2))))
			countChangedInstances[&BO]++;
	}
	if (range1 && range2)
	{
		infoMap[&BO] = getUpdatedRange(BO.getOpcode(), *range1, *range2);
	}
	
	//If the range of a variable is monotonically increasing for a maximum allowed iterations, set its range to top.
	if (countChangedInstances[&BO] > MAX_ALLOWED_ITERATIONS)
			infoMap[&BO] = new ConstantRange(32, true);
	
	out = new RangeLatticePoint(false, false, infoMap);
	return out;
}

LatticePoint* RangeFlowFunction::applyPHIFlowFunction(PHINode& phiNode, LatticePoint* info_in)
{
	RangeLatticePoint* actual_in = dyn_cast<RangeLatticePoint>(info_in);
	map<Value*, ConstantRange*> infoMap = actual_in->latticePoint;
	ConstantRange *updatedRange = new ConstantRange(32, false);
	for (int i=0;i<phiNode.getNumIncomingValues(); i++)
	{
		Value *val = phiNode.getIncomingValue(i);
		if (infoMap.count(val) > 0)
			*updatedRange = updatedRange->unionWith(*infoMap[val]);
		else if (isa<ConstantInt>(val))
			*updatedRange = updatedRange->unionWith(*(new ConstantRange(dyn_cast<ConstantInt>(val)->getValue())));
	}
	if (!updatedRange->isEmptySet())
		infoMap[&phiNode] = updatedRange;
//	errs() << "in phi: " << phiNode.getName() << " " << *updatedRange << "\n";
	return new RangeLatticePoint(false, false, infoMap);
}

LatticePoint* RangeFlowFunction::applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in)
{
	return info_in;
}

map<BasicBlock*, LatticePoint*> RangeFlowFunction::applyTerminatorFlowFunction(TerminatorInst& terminator, LatticePoint* info_in, vector<BasicBlock*> &successors)
{
	map<BasicBlock*, LatticePoint*> info_out;
	RangeLatticePoint *actualIn = dyn_cast<RangeLatticePoint>(info_in);
	map<Value*, ConstantRange*> infoMap = actualIn->latticePoint;
	
	if (isa<BranchInst>(terminator))
	{
		BranchInst* branch = dyn_cast<BranchInst>(&terminator);
		if (!branch->isUnconditional())
		{
			Value *val = branch->getCondition();
			if (isa<ICmpInst>(val))
			{
				ICmpInst* condition = dyn_cast<ICmpInst>(val);
				
				BasicBlock *branch_false = dyn_cast<BasicBlock>(branch->getOperand(1));
				BasicBlock *branch_true = dyn_cast<BasicBlock>(branch->getOperand(2));
				Value *comparedVal = condition->getOperand(0);
				Value *comparedAgainst = condition->getOperand(1);
				ConstantInt *constantComparedAgainst;
				ConstantRange *originalRange = NULL;
				ConstantRange *newRangeTrue = new ConstantRange(32, true);
				ConstantRange *newRangeFalse = new ConstantRange(32, true);
				if (infoMap.count(comparedVal) > 0)
					originalRange = infoMap[comparedVal];
				map<Value*, ConstantRange*> infoMapTrue = infoMap;
				map<Value*, ConstantRange*> infoMapFalse = infoMap;
				if (originalRange && isa<ConstantInt>(comparedAgainst))
				{
					constantComparedAgainst = dyn_cast<ConstantInt>(comparedAgainst);
					int predicate;
					if (condition->isSigned())
						predicate = condition->getSignedPredicate();
					else
						predicate = condition->getUnsignedPredicate();
					
					*newRangeTrue = ConstantRange::makeICmpRegion(predicate, ConstantRange(constantComparedAgainst->getValue()));
					*newRangeFalse = (ConstantRange(32, true).difference(*newRangeTrue));
					*newRangeTrue = newRangeTrue->intersectWith(*originalRange);
					//*newRangeFalse = newRangeFalse->intersectWith(*originalRange);	//Not required
					
					if (infoMapTrue.count(comparedVal) > 0)
						infoMapTrue[comparedVal] = newRangeTrue;
					if (infoMapFalse.count(comparedVal) > 0)
						infoMapFalse[comparedVal] = newRangeFalse;
				}
				
//				errs() << *newRangeTrue << " " << *newRangeFalse << "\n";
				info_out[branch_true] = new RangeLatticePoint(false, false, infoMapTrue);
				info_out[branch_false] = new RangeLatticePoint(false, false, infoMapFalse);
				
//				errs() << "Branch_false:" << branch_false->getName() << " Branch_true:" << branch_true->getName() << " leftSide: " << left->getName() << "rightSide:" << right->getName() << "\n";
				
				return info_out;
			}
		}
	}
	for (unsigned int i=0;i<successors.size();i++)
			info_out[successors[i]] = info_in;
			return info_out;
}

RangeFlowFunction::~RangeFlowFunction(){}

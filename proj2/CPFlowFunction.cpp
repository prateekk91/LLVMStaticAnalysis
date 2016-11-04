# include "CPFlowFunction.h"
using namespace std;
  
ConstantInt* constantFold(unsigned int Opcode ,ConstantInt *C1, ConstantInt *C2)
{
    LLVMContext &context = C1->getContext();
    const APInt &constant1 = C1->getValue();
    const APInt &constant2 = C2->getValue();
    ConstantInt* result = ConstantInt::get(context, APInt::getNullValue(32));
    switch (Opcode) 
    {
      case Instruction::Add:
        result = ConstantInt::get(context, constant1 + constant2);
        break;
      case Instruction::Sub:
        result = ConstantInt::get(context, constant1 - constant2);
        break;
      case Instruction::Mul:
        result = ConstantInt::get(context, constant1 * constant2);
        break;
      case Instruction::UDiv:
        result = ConstantInt::get(context, constant1.udiv(constant2));
        break;
      case Instruction::SDiv:
      	if (constant2 == APInt(32,0,false))
      	{
      		errs() << "-----------------------Possible divide by 0. Not optimising divide instruction----------------------------\n\n";
      		result = NULL;
      	}
      	else
	        result = ConstantInt::get(context, constant1.sdiv(constant2));
        break;
    }
    return result;
}
  
LatticePoint* CPFlowFunction::applyBinaryFlowFunction(BinaryOperator &BO, LatticePoint* info_in)
{
	CPLatticePoint* out;
	CPLatticePoint* actual_in = dyn_cast<CPLatticePoint>(info_in);
	if (actual_in == NULL)
	{
		//errs() << "Its null"; Shouldn't be null
		return NULL;
	}
	map<Value*, ConstantInt*> infoMap = actual_in->latticePoint;
	Value* val1 = BO.getOperand(0);
  	Value* val2 = BO.getOperand(1);
  	ConstantInt* C1 = NULL;
 	ConstantInt* C2 = NULL;
	
	if (isa<ConstantInt>(val1)) 
  	{
		C1 = dyn_cast<ConstantInt>(val1);
  	} 
  	else if (infoMap.count(val1) > 0) 
  	{
		C1 = infoMap[val1];
  	} 
  	if (isa<ConstantInt>(val2)) 
	{
		C2 = dyn_cast<ConstantInt>(val2);
	}
	else if (infoMap.count(val2) > 0) 
	{
		C2 = infoMap[val2];
	}
	if (C1 && C2)
	{
		ConstantInt *result = constantFold(BO.getOpcode(), C1, C2);
		if (result)
		{
			infoMap[&BO] = result;
		}
	}
	out = new CPLatticePoint(false, false, infoMap);
	return out;
}

LatticePoint* CPFlowFunction::applyPHIFlowFunction(PHINode& phiNode, LatticePoint* info_in)
{
	CPLatticePoint* actual_in = dyn_cast<CPLatticePoint>(info_in);
	map<Value*, ConstantInt*> infoMap = actual_in->latticePoint;
	ConstantInt *c_out = NULL;
	for (unsigned int i=0;i<phiNode.getNumIncomingValues(); i++)
	{
		Value *val = phiNode.getIncomingValue(i);
		ConstantInt *c = NULL;
		if (infoMap.count(val) > 0)
			c = infoMap[val];
		else if (isa<ConstantInt>(val))
			c = dyn_cast<ConstantInt>(val);
		if (c && (c_out == NULL))
			c_out = c;
		else if  ((c != NULL) && (c_out != NULL)  && (c != c_out))
			return new CPLatticePoint(false, false, infoMap);
		
	}
	if (c_out)
		infoMap[&phiNode] = c_out;
	return new CPLatticePoint(false, false, infoMap);
}

LatticePoint* CPFlowFunction::applyCompareFlowFunction(ICmpInst&, LatticePoint* info_in)
{
	return info_in;
}

map<BasicBlock*, LatticePoint*> CPFlowFunction::applyTerminatorFlowFunction(TerminatorInst& terminator, LatticePoint* info_in, vector<BasicBlock*> &successors)
{
	map<BasicBlock*, LatticePoint*> info_out;
	for (unsigned int i=0;i<successors.size();i++)
		info_out[successors[i]] = info_in;
	return info_out;
}

CPFlowFunction::~CPFlowFunction(){}

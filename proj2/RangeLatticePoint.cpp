# include "RangeLatticePoint.h"

LatticePoint* RangeLatticePoint::join(LatticePoint *in)
{
	RangeLatticePoint* latticeToReturn;
	RangeLatticePoint *actual_in = dyn_cast<RangeLatticePoint>(in);
	if (actual_in->isItBottom() || isItBottom())
		return new RangeLatticePoint(false, true, map<Value*, ConstantRange*>());
	else if (actual_in->isItTop() || isItTop())
		return new RangeLatticePoint(true, false, map<Value*, ConstantRange*>());
	else latticeToReturn = new RangeLatticePoint(false, false, map<Value*, ConstantRange*>());
	map <Value*, ConstantRange*> incoming = actual_in->latticePoint;
	for (map<Value*, ConstantRange*>::iterator it = latticePoint.begin(); it != latticePoint.end(); it++)
	{
		if (incoming.count(it->first) > 0)
		{
			ConstantRange *range1 = it->second;
			ConstantRange *range2 = incoming[it->first];
			ConstantRange *result = new ConstantRange(32,true);
			*result = range1->unionWith(*range2);
			latticeToReturn->latticePoint[it->first] = result;
		}
		else
			latticeToReturn->latticePoint[it->first] = it->second;
	}
	for (map<Value*, ConstantRange*>::iterator it = incoming.begin(); it != incoming.end(); it++)
	{
		if (latticePoint.count(it->first) <= 0)
			latticeToReturn->latticePoint[it->first] = it->second;
	}
	errs() << "In join\n";
	return latticeToReturn;
}

bool RangeLatticePoint::equals(LatticePoint *in)
{
	RangeLatticePoint* actual_in = dyn_cast<RangeLatticePoint>(in);
	if (actual_in -> isItBottom() && isItBottom())
		return true;
	if (actual_in->isItTop() && isItTop())
		return true;
	map<Value*, ConstantRange*> lattice_in = actual_in->latticePoint;
	if (latticePoint.size() != lattice_in.size())
		return false;
	for (std::map<Value *, ConstantRange *>::iterator it=latticePoint.begin(); it!=latticePoint.end(); ++it)
	{
		ConstantRange* c1 = it->second;
		if (lattice_in.count(it->first) <= 0)
		{
			return false;
		}
		else
		{
			ConstantRange* c2 = lattice_in[it->first];
			if (*c1 != *c2)
			{
				return false;
		  	}
		}
   }
   return true;
}

void RangeLatticePoint::printLattice()
{
	for (map<Value*, ConstantRange*>::iterator it = latticePoint.begin(); it!=latticePoint.end(); it++)
	{
		errs() << it->first->getName() << " " << *(it->second)  << " , ";
	}
	errs() << "\n";
}

RangeLatticePoint::~RangeLatticePoint(){}

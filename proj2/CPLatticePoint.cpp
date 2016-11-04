#include "CPLatticePoint.h"

LatticePoint* CPLatticePoint::join(LatticePoint* in)
{
	CPLatticePoint* latticeToReturn;
	CPLatticePoint *actual_in = dyn_cast<CPLatticePoint>(in);
	if (actual_in->isItBottom() || isItBottom())
		latticeToReturn = new CPLatticePoint(false, true, map<Value*, ConstantInt*>());
	else if (actual_in->isItTop() || isItTop())
		latticeToReturn = new CPLatticePoint(true, false, map<Value*, ConstantInt*>());
	else latticeToReturn = new CPLatticePoint(false, false, map<Value*, ConstantInt*>());
	map <Value*, ConstantInt*> incoming = actual_in->latticePoint;
	map<Value *, ConstantInt*>::iterator it = latticePoint.begin();
	while(it != latticePoint.end())
	{
		if (incoming.count(it->first) > 0 && incoming.find(it->first)->second == it->second)
			latticeToReturn->latticePoint[it->first] = it->second;
		it++;
	}
	return latticeToReturn;
}

bool CPLatticePoint::equals(LatticePoint *in)
{
	CPLatticePoint* actual_in = dyn_cast<CPLatticePoint>(in);
	if (actual_in -> isItBottom() && isItBottom())
		return true;
	if (actual_in->isItTop() && isItTop())
		return true;
	map<Value*, ConstantInt*> lattice_in = actual_in->latticePoint;
	if (latticePoint.size() != lattice_in.size())
		return false;
	for (std::map<Value *, ConstantInt *>::iterator it=latticePoint.begin(); it!=latticePoint.end(); ++it)
	{
		ConstantInt* c1 = it->second;
		if (lattice_in.count(it->first) <= 0)
		{
		  return false;
		}
		else
		{
		  ConstantInt* c2 = lattice_in[it->first];
		  if (c1 != c2)
		  {
		    return false;
		  }
		}
   }
   return true;
}

void CPLatticePoint::printLattice()
{
	for (map<Value*, ConstantInt*>::iterator it = latticePoint.begin(); it!=latticePoint.end(); it++)
	{
		errs() << it->first->getName() << " " << it->second->getValue() << ", ";
	}
	errs() << "\n";
}

CPLatticePoint::~CPLatticePoint(){}
	

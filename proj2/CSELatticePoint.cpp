#include "CSELatticePoint.h"

LatticePoint* CSELatticePoint::join(LatticePoint* in)
{
	CSELatticePoint* latticeToReturn;
	CSELatticePoint *actual_in = dyn_cast<CSELatticePoint>(in);
	if (actual_in->isItBottom() || isItBottom())
		latticeToReturn = new CSELatticePoint(false, true, map<Value*, Instruction*>());
	else if (actual_in->isItTop() || isItTop())
		latticeToReturn = new CSELatticePoint(true, false, map<Value*, Instruction*>());
	else latticeToReturn = new CSELatticePoint(false, false, map<Value*, Instruction*>());
	map <Value*, Instruction*> incoming = actual_in->latticePoint;
	map<Value *, Instruction*>::iterator it = latticePoint.begin();
	while(it != latticePoint.end())
	{
		if (incoming.count(it->first) > 0 && incoming.find(it->first)->second == it->second)
			latticeToReturn->latticePoint[it->first] = it->second;
		it++;
	}
	return latticeToReturn;
}

bool CSELatticePoint::equals(LatticePoint *in)
{
	CSELatticePoint* actual_in = dyn_cast<CSELatticePoint>(in);
	if (actual_in -> isItBottom() && isItBottom())
		return true;
	if (actual_in->isItTop() && isItTop())
		return true;
	map<Value*, Instruction*> lattice_in = actual_in->latticePoint;
	if (latticePoint.size() != lattice_in.size())
		return false;
	for (std::map<Value *, Instruction*>::iterator it=latticePoint.begin(); it!=latticePoint.end(); ++it)
	{
		Instruction* c1 = it->second;
		if (lattice_in.count(it->first) <= 0)
		{
		  return false;
		}
		else
		{
		 Instruction* c2 = lattice_in[it->first];
		  if (c1 != c2)
		  {
		    return false;
		  }
		}
   }
   return true;
}

void CSELatticePoint::printLattice()
{
	for (map<Value*, Instruction*>::iterator it = latticePoint.begin(); it!=latticePoint.end(); it++)
	{
		errs() << it->first->getName() << " " << *(it->second) << ", ";
	}
	errs() << "\n";
}

CSELatticePoint::~CSELatticePoint(){}
	

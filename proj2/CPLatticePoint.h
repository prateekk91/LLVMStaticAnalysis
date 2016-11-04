# ifndef _CPLatticePoint_
# define _CPLatticePoint_
# include "LatticePoint.h"
using namespace llvm;
class CPLatticePoint : public LatticePoint
{
//	using LatticePoint::join;
	public:
	std::map <Value*, ConstantInt*> latticePoint;
	virtual LatticePoint* join(LatticePoint* in);
	CPLatticePoint(bool isTop, bool isBottom, std::map<Value*, ConstantInt*> latticePoint) : LatticePoint(isTop, isBottom, LatticePoint::CPLattice)
	{
		this->latticePoint = latticePoint;
	};
	virtual ~CPLatticePoint();
	static bool classof(const LatticePoint *latticePoint) 
	{
    	return latticePoint->getKind() == CPLattice;
  	}
  	virtual bool equals(LatticePoint*);
  	virtual void printLattice();
};
#endif

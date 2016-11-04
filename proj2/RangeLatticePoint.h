# ifndef _RangeLatticePoint_H
# define _RangeLatticePoint_H
# include "LatticePoint.h"
# include "llvm/Support/ConstantRange.h"
class RangeLatticePoint : public LatticePoint
{
	public:
	std::map <Value*, ConstantRange*> latticePoint;
	virtual LatticePoint* join(LatticePoint* in);
	RangeLatticePoint(bool isTop, bool isBottom, std::map<Value*, ConstantRange*> latticePoint) : LatticePoint(isTop, isBottom, LatticePoint::RangeLattice)
	{
		this->latticePoint = latticePoint;
	};
	virtual ~RangeLatticePoint();
	static bool classof(const LatticePoint *latticePoint) 
	{
    	return latticePoint->getKind() == RangeLattice;
  	}
  	virtual bool equals(LatticePoint*);
  	virtual void printLattice();
};
#endif

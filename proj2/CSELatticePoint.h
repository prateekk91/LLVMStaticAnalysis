# ifndef __CSELATTICEPOINT_H_
# define __CSELATTICEPOINT_H_
# include "LatticePoint.h"
class CSELatticePoint : public LatticePoint
{
	public:
	std::map <Value*, Instruction*> latticePoint;
	virtual LatticePoint* join(LatticePoint* in);
	CSELatticePoint(bool isTop, bool isBottom, std::map<Value*, Instruction*> latticePoint) : LatticePoint(isTop, isBottom, LatticePoint::CSELattice)
	{
		this->latticePoint = latticePoint;
	};
	virtual ~CSELatticePoint();
	static bool classof(const LatticePoint *latticePoint) 
	{
    	return latticePoint->getKind() == CSELattice;
  	}
  	virtual bool equals(LatticePoint*);
  	virtual void printLattice();
};
#endif

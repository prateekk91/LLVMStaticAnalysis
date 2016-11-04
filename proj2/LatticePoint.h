#ifndef _LATTICEPOINT_
#define _LATTICEPOINT_
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
# include "llvm/Support/Casting.h"
#include <map>
#include <cstddef>
using namespace std;
using namespace llvm;
class LatticePoint
{
	public:
		enum LatticeKind 
		{
			CPLattice,
			CSELattice,
			RangeLattice
		};
	private:
	  const LatticeKind Kind;
	public:
	  LatticeKind getKind() const { return Kind; }

	private:
		bool isTop;
		bool isBottom;
	public:
		LatticePoint(bool isTop, bool isBottom, LatticeKind kind) : Kind (kind)
		{
			this->isBottom = isBottom;
			this->isTop = isTop;
		}
		bool isItBottom()
		{
			return isBottom;
		}
		bool isItTop()
		{
			return isTop;
		}
		 virtual LatticePoint* join(LatticePoint*) = 0;
		 virtual bool equals(LatticePoint*) = 0;
		 virtual void printLattice() = 0;
		 ~LatticePoint();
};
#endif

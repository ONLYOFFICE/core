#pragma once

namespace XLS
{;

struct FillInfo
{
	FillInfo(){}

	FillInfo(const unsigned char fls_set, const unsigned char icvFore_set, const unsigned char icvBack_set)
		: fls(fls_set), icvFore(icvFore_set), icvBack(icvBack_set)
	{	
	}

	unsigned char fls;
	unsigned char icvFore;
	unsigned char icvBack;

	bool operator < (const FillInfo& right) const
	{
		return  fls < right.fls || fls == right.fls && 
				(icvFore < right.icvFore || icvFore == right.icvFore &&
				(icvBack < right.icvBack));
	}

};

struct BorderInfo
{
	unsigned char dgLeft;
	unsigned char dgRight;
	unsigned char dgTop;
	unsigned char dgBottom;
	unsigned char dgDiag;
	unsigned char grbitDiag;
	unsigned char icvLeft;
	unsigned char icvRight;
	unsigned char icvTop;
	unsigned char icvBottom;
	unsigned char icvDiag;

	bool operator < (const BorderInfo& right) const
	{
		return  dgLeft < right.dgLeft || dgLeft == right.dgLeft && 
				(dgRight < right.dgRight || dgRight == right.dgRight &&
				(dgTop < right.dgTop || dgTop == right.dgTop &&
				(dgBottom < right.dgBottom || dgBottom == right.dgBottom &&
				(dgDiag < right.dgDiag || dgDiag == right.dgDiag &&
				(grbitDiag < right.grbitDiag || grbitDiag == right.grbitDiag &&
				(icvLeft < right.icvLeft || icvLeft == right.icvLeft &&
				(icvRight < right.icvRight || icvRight == right.icvRight &&
				(icvTop < right.icvTop || icvTop == right.icvTop &&
				(icvBottom < right.icvBottom || icvBottom == right.icvBottom &&
				(icvDiag < right.icvDiag))))))))));
	}

};


} // namespace XLS


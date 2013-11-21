#pragma once

namespace XLS
{;

struct FillInfo
{
	FillInfo(){}

	FillInfo(const BYTE fls_set, const BYTE icvFore_set, const BYTE icvBack_set)
		: fls(fls_set), icvFore(icvFore_set), icvBack(icvBack_set)
	{	
	}

	BYTE fls;
	BYTE icvFore;
	BYTE icvBack;

	bool operator < (const FillInfo& right) const
	{
		return  fls < right.fls || fls == right.fls && 
				(icvFore < right.icvFore || icvFore == right.icvFore &&
				(icvBack < right.icvBack));
	}

};

struct BorderInfo
{
	BYTE dgLeft;
	BYTE dgRight;
	BYTE dgTop;
	BYTE dgBottom;
	BYTE dgDiag;
	BYTE grbitDiag;
	BYTE icvLeft;
	BYTE icvRight;
	BYTE icvTop;
	BYTE icvBottom;
	BYTE icvDiag;

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


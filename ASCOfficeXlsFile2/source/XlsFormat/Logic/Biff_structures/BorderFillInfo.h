#pragma once

#include <sstream>

namespace XLS
{;

struct FillInfoExt
{
	FillInfoExt()	{enabled = false; xclrType = icv = 0; nTintShade = 0; xclrValue = 0;}
	bool			enabled;

	unsigned char	xclrType;
	unsigned char	icv;
	short			nTintShade;
	unsigned int	xclrValue;
};

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

	bool operator == (const FillInfo & rVal) const;
	bool operator != (const FillInfo & rVal) const;

	friend std::size_t hash_value(FillInfo const & val);

	int serialize(std::wostream & stream);

//additional (сравнение по базовому, расширения тока пишем)

	FillInfoExt foreFillInfo_;
	FillInfoExt backFillInfo_;


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

	BorderInfo(){}

	BorderInfo(	const unsigned char dgLeft_set, const unsigned char dgRight_set, const unsigned char dgTop_set, 
			const unsigned char dgBottom_set, const unsigned char dgDiag_set, const unsigned char grbitDiag_set, 
			const unsigned char icvLeft_set, const unsigned char icvRight_set, const unsigned char icvTop_set, 
			const unsigned char icvBottom_set, const unsigned char icvDiag_set	)

		:	dgLeft(dgLeft_set), dgRight(dgRight_set), dgTop(dgTop_set), dgBottom(dgBottom_set), 
			dgDiag(dgDiag_set), grbitDiag(grbitDiag_set),
			icvLeft(icvLeft_set), icvRight(icvRight_set), icvTop(icvTop_set), icvBottom(icvBottom_set),
			icvDiag(icvDiag_set)
	{	
	}
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

	bool operator == (const BorderInfo & rVal) const;
	bool operator != (const BorderInfo & rVal) const;
	
	friend std::size_t hash_value(BorderInfo const & val);

	int serialize(std::wostream & stream);
};



} // namespace XLS


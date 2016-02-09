#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;
class DXFN;

class DXFALC : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFALC)
public:
	BiffStructurePtr clone();
	
	static const ElementType	type = typeDXFALC;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int serialize(std::wostream & stream);

	unsigned char	alc;
	bool			fWrap;
	unsigned char	alcv;
	bool			fJustLast;
	unsigned char	trot;
	unsigned char	cIndent;
	bool			fShrinkToFit;
	bool			fMergeCell;
	unsigned char	iReadingOrder;

	_UINT32			iIndent;
//------------------------------------------------
	DXFN *parent;
};

} // namespace XLS


#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of BopPop record in BIFF8
class BopPop: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BopPop)
	BASE_OBJECT_DEFINE_CLASS_NAME(BopPop)
public:
	BopPop();
	~BopPop();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBopPop;

	int serialize(std::wostream & _stream);

//-----------------------------
	unsigned char	pst;
	unsigned char	fAutoSplit;
	_UINT16			split;
	_INT16			iSplitPos;
	_INT16			pcSplitPercent;
	_INT16			pcPie2Size;
	_INT16			pcGap;
	BIFF_DOUBLE		numSplitValue;
	
	bool			fHasShadow;


	BaseObjectPtr	m_Custom;
};

} // namespace XLS


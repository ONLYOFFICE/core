#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/AFDOper.h>

namespace XLS
{


// Logical representation of AutoFilter record in BIFF8
class AutoFilter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AutoFilter)
	BASE_OBJECT_DEFINE_CLASS_NAME(AutoFilter)
public:
	AutoFilter();
	~AutoFilter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAutoFilter;

	_UINT16 iEntry;
	
	unsigned char	wJoin;
	
	bool fSimple1;
	bool fSimple2;
	
	bool fTopN;
	bool fPercent;
	
	unsigned char	fTop;
	_UINT16 wTopN;

	AFDOper doper1;
	AFDOper doper2;

	bool m_bAutoFilter12;
	
	std::wstring str1;
	std::wstring str2;

};

} // namespace XLS


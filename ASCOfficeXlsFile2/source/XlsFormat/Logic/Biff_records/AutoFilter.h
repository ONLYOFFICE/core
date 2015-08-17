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

	BIFF_WORD iEntry;
	BIFF_BYTE wJoin;
	BIFF_BYTE fSimple1;
	BIFF_BYTE fSimple2;
	BIFF_BYTE fTopN;
	BIFF_BYTE fTop;
	BIFF_BYTE fPercent;
	BIFF_WORD wTopN;

	AFDOper doper1;
	AFDOper doper2;

	bool m_bAutoFilter12;
	XLUnicodeStringNoCch str1;
	XLUnicodeStringNoCch str2;

//-----------------------------



		//if (!m_bAutoFilter12)
		//{
		//	if (fSimple1)
		//	{
		//		//BO_ATTRIB_MARKUP_ATTRIB(str1)
		//		//BO_ATTRIB_MARKUP_COMPLEX(doper1)
		//	}
		//	if (fSimple2)
		//	{
		//		//BO_ATTRIB_MARKUP_ATTRIB(str2)
		//		//BO_ATTRIB_MARKUP_COMPLEX(doper2)
		//	}			
		//}


};

} // namespace XLS


#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ChartParsedFormula.h>

namespace XLS
{;


// Logical representation of BRAI record in BIFF8
class BRAI: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BRAI)
	BASE_OBJECT_DEFINE_CLASS_NAME(BRAI)
public:
	BRAI();
	~BRAI();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE id;
	BIFF_BYTE rt;
	BIFF_BOOL fUnlinkedIfmt;
	IFmt ifmt;
	ChartParsedFormula formula;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(id)
		BO_ATTRIB_MARKUP_ATTRIB(rt)
		BO_ATTRIB_MARKUP_ATTRIB(fUnlinkedIfmt)
		BO_ATTRIB_MARKUP_ATTRIB(ifmt)
		BO_ATTRIB_MARKUP_COMPLEX(formula)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS


#pragma once

#include "BiffRecord.h"

#include <Logic/Biff_structures/ChartParsedFormula.h>
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{


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

	static const ElementType	type = typeBRAI;

//-----------------------------
	unsigned char	id;
	unsigned char	rt;
	bool			fUnlinkedIfmt;
	unsigned short	ifmt;
	
	ChartParsedFormula	formula; //набор диапазонов и ячеек
};

} // namespace XLS


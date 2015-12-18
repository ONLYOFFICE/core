#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Pie record in BIFF8
class Pie: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pie)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pie)
public:
	Pie();
	~Pie();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePie;

	int	serialize(std::wostream & _stream);

//-----------------------------
	BIFF_WORD anStart;
	BIFF_WORD pcDonut;
	BIFF_BOOL fHasShadow;
	BIFF_BOOL fShowLdrLines;
};

} // namespace XLS


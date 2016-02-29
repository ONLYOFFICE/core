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
	_UINT16 anStart;
	_UINT16 pcDonut;
	bool fHasShadow;
	bool fShowLdrLines;
};

} // namespace XLS


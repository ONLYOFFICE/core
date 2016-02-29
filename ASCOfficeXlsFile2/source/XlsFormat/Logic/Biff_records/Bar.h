#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Bar record in BIFF8
class Bar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Bar)
	BASE_OBJECT_DEFINE_CLASS_NAME(Bar)
public:
	Bar();
	~Bar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBar;

	int		serialize(std::wostream & _stream);

	_INT16			pcOverlap;
	_UINT16			pcGap;

	bool			fTranspose;
	bool			fStacked;
	bool			f100;
	bool			fHasShadow;
//------------------------------------
	BaseObjectPtr		m_chart3D;
};

} // namespace XLS


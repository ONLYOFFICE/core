#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DefaultRowHeight record in BIFF8
class DefaultRowHeight: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DefaultRowHeight)
	BASE_OBJECT_DEFINE_CLASS_NAME(DefaultRowHeight)
public:
	DefaultRowHeight();
	~DefaultRowHeight();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDefaultRowHeight;

//-----------------------------
	bool fUnsynced;
	bool fDyZero;
	bool fExAsc;
	bool fExDsc;
	
	_INT16 miyRw; // measured in twips (1/20 of of a printer's point)

};

} // namespace XLS


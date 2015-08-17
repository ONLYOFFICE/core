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
	BIFF_BOOL fUnsynced;
	BIFF_BOOL fDyZero;
	BIFF_BOOL fExAsc;
	BIFF_BOOL fExDsc;
	BIFF_SHORT miyRw; // measured in twips (1/20 of of a printer's point)
	BIFF_SHORT miyRwHidden;

};

} // namespace XLS


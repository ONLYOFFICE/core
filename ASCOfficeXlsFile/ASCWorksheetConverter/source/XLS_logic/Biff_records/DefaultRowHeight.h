#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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
private:
	BIFF_BOOL fUnsynced;
	BIFF_BOOL fDyZero;
	BIFF_BOOL fExAsc;
	BIFF_BOOL fExDsc;
	BIFF_SHORT miyRw; // measured in twips (1/20 of of a printer's point)
	BIFF_SHORT miyRwHidden;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fUnsynced)
		BO_ATTRIB_MARKUP_ATTRIB(fDyZero)
		BO_ATTRIB_MARKUP_ATTRIB(fExAsc)
		BO_ATTRIB_MARKUP_ATTRIB(fExDsc)
		if(!fDyZero)
		{
			BO_ATTRIB_MARKUP_ATTRIB(miyRw)
		}
		else
		{
			BO_ATTRIB_MARKUP_ATTRIB(miyRwHidden)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS


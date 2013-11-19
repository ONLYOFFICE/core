#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Row record in BIFF8
class Row: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Row)
	BASE_OBJECT_DEFINE_CLASS_NAME(Row)
public:
	Row();
	~Row();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Rw rw;
	BackwardOnlyParam<WORD> colMic;
	BackwardOnlyParam<WORD> colMac;
	BIFF_WORD miyRw;

	BIFF_BYTE iOutLevel;
	BIFF_BOOL fCollapsed;
	BIFF_BOOL fDyZero;
	BIFF_BOOL fUnsynced;
	BIFF_BOOL fGhostDirty;
	
	BIFF_WORD ixfe_val;
	BIFF_BOOL fExAsc;
	BIFF_BOOL fExDes;
	BIFF_BOOL fPhonetic;
	BackwardOnlyParam<DWORD> in_block_position;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(rw)
		BO_ATTRIB_MARKUP_ATTRIB(colMic)
		BO_ATTRIB_MARKUP_ATTRIB(colMac)
		BO_ATTRIB_MARKUP_ATTRIB(miyRw)

		BO_ATTRIB_MARKUP_ATTRIB(iOutLevel)
		BO_ATTRIB_MARKUP_ATTRIB(fCollapsed)
		BO_ATTRIB_MARKUP_ATTRIB(fDyZero)
		BO_ATTRIB_MARKUP_ATTRIB(fUnsynced)
		BO_ATTRIB_MARKUP_ATTRIB(fGhostDirty)

		BO_ATTRIB_MARKUP_ATTRIB(ixfe_val)
		BO_ATTRIB_MARKUP_ATTRIB(fExAsc)
		BO_ATTRIB_MARKUP_ATTRIB(fExDes)
		BO_ATTRIB_MARKUP_ATTRIB(fPhonetic)
		BO_ATTRIB_MARKUP_ATTRIB(in_block_position)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS


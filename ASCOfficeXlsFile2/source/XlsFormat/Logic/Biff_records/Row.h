#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeRow;

//-----------------------------
	Rw rw;
	BackwardOnlyParam<unsigned short> colMic;
	BackwardOnlyParam<unsigned short> colMac;
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
	BackwardOnlyParam<unsigned int> in_block_position;
};

} // namespace XLS


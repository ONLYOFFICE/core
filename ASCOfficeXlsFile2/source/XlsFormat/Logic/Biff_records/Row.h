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

	int serialize(std::wostream & stream);

	static const ElementType	type = typeRow;

	GlobalWorkbookInfoPtr global_info_;

//-----------------------------
	Rw			rw;
	BackwardOnlyParam<unsigned short> colMic;
	BackwardOnlyParam<unsigned short> colMac;
	
	_UINT16		miyRw;

	unsigned char	iOutLevel;
	bool			fCollapsed;
	bool			fDyZero;
	bool			fUnsynced;
	bool			fGhostDirty;
	
	_UINT16			ixfe_val;
	bool			fExAsc;
	bool			fExDes;
	bool			fPhonetic;
	
	BackwardOnlyParam<_UINT32> in_block_position;
};

} // namespace XLS


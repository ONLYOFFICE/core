#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FilePointer.h>

namespace XLS
{


// Logical representation of Index record in BIFF8
class Index: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Index)
	BASE_OBJECT_DEFINE_CLASS_NAME(Index)
public:
	Index();
	~Index();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeIndex;

//-----------------------------
	BackwardOnlyParam<_UINT32>	rwMic;
	BackwardOnlyParam<_UINT32>	rwMac;
	ForwardOnlyParam<_UINT32>	ibXF;
	BackwardOnlyParam<_UINT32>	num_pointers;

	BiffStructurePtrVector rgibRw;

};

} // namespace XLS


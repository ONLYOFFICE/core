#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FilePointer.h>

namespace XLS
{;


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
	BackwardOnlyParam<unsigned int> rwMic;
	BackwardOnlyParam<unsigned int> rwMac;
	ForwardOnlyParam<unsigned int> ibXF;
	BackwardOnlyParam<unsigned int> num_pointers;
	BiffStructurePtrVector rgibRw;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(rwMic)
		//BO_ATTRIB_MARKUP_ATTRIB(rwMac)
// "ibXF" and "FilePointer" are not used in XSLT
// 		//BO_ATTRIB_MARKUP_ATTRIB(ibXF)
		//BO_ATTRIB_MARKUP_ATTRIB(num_pointers)
// 		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgibRw, FilePointer)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


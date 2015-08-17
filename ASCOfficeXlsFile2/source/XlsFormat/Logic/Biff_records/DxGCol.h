#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DxGCol record in BIFF8
class DxGCol: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DxGCol)
	BASE_OBJECT_DEFINE_CLASS_NAME(DxGCol)
public:
	DxGCol();
	~DxGCol();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDxGCol;

//-----------------------------
	BIFF_WORD dxgCol;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(dxgCol)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


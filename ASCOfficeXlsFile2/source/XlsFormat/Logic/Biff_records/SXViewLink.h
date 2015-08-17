#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SXViewLink record in BIFF8
class SXViewLink: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXViewLink)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXViewLink)
public:
	SXViewLink();
	~SXViewLink();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSXViewLink;
//-----------------------------
//	BIFF_WORD userName;


};

} // namespace XLS


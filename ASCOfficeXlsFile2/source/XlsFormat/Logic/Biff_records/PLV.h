#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of PLV record in BIFF8
class PLV: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PLV)
	BASE_OBJECT_DEFINE_CLASS_NAME(PLV)
public:
	PLV();
	~PLV();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePLV;

//-----------------------------
	BIFF_WORD wScalePLV;
	
	bool fPageLayoutView;
	bool fRulerVisible;
	bool fWhitespaceHidden;

};

} // namespace XLS


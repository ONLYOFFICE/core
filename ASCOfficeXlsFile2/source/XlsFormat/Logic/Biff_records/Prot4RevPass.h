#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Prot4RevPass record in BIFF8
class Prot4RevPass: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Prot4RevPass)
	BASE_OBJECT_DEFINE_CLASS_NAME(Prot4RevPass)
public:
	Prot4RevPass();
	~Prot4RevPass();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeProt4RevPass;

//-----------------------------
	BIFF_BSTR protPwdRev;

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(protPwdRev)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


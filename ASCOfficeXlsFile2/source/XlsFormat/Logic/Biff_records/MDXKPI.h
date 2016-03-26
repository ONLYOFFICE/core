#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of MDXKPI record in BIFF8
class MDXKPI: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDXKPI)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXKPI)
public:
	MDXKPI();
	~MDXKPI();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	_INT32			istrConnName;
	unsigned char	tfnSrc;
	unsigned char	kpiprop;
	_INT32			istrKPIName;
	_INT32			istrMbrKPI;
};

} // namespace XLS


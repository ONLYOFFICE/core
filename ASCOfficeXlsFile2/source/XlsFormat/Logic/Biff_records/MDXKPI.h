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
	BIFF_LONG istrConnName;
	BIFF_BYTE tfnSrc;
	BIFF_BYTE kpiprop;
	BIFF_LONG istrKPIName;
	BIFF_LONG istrMbrKPI;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(istrConnName)
		//BO_ATTRIB_MARKUP_ATTRIB(tfnSrc)
		//BO_ATTRIB_MARKUP_ATTRIB(kpiprop)
		//BO_ATTRIB_MARKUP_ATTRIB(istrKPIName)
		//BO_ATTRIB_MARKUP_ATTRIB(istrMbrKPI)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


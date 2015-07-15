#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ColInfo record in BIFF8
class ColInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ColInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(ColInfo)
public:
	ColInfo();
	~ColInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeColInfo;

//-----------------------------
	Col256U colFirst;
	Col256U colLast;
	BIFF_WORD coldx;
	IXFCell ixfe;

	BIFF_BOOL fHidden;
	BIFF_BOOL fUserSet;
	BIFF_BOOL fBestFit;
	BIFF_BOOL fPhonetic;
	BIFF_BYTE iOutLevel;
	BIFF_BOOL fCollapsed;


public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(colFirst)
		//BO_ATTRIB_MARKUP_ATTRIB(colLast)
		//BO_ATTRIB_MARKUP_ATTRIB(coldx)
		//BO_ATTRIB_MARKUP_ATTRIB(ixfe)

		//BO_ATTRIB_MARKUP_ATTRIB(fHidden)
		//BO_ATTRIB_MARKUP_ATTRIB(fUserSet)
		//BO_ATTRIB_MARKUP_ATTRIB(fBestFit)
		//BO_ATTRIB_MARKUP_ATTRIB(fPhonetic)
		//BO_ATTRIB_MARKUP_ATTRIB(iOutLevel)
		//BO_ATTRIB_MARKUP_ATTRIB(fCollapsed)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


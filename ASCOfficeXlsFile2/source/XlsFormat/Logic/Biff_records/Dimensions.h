#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Dimensions record in BIFF8
class Dimensions: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Dimensions)
	BASE_OBJECT_DEFINE_CLASS_NAME(Dimensions)
public:
	Dimensions();
	~Dimensions();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDimensions;

//-----------------------------
	RwLongU rwMic;
	BIFF_DWORD rwMac;
	ColU colMic;
	BIFF_WORD colMac;
	BIFF_BSTR ref_;
public:
	//BO_ATTRIB_MARKUP_BEGIN
/*
		//BO_ATTRIB_MARKUP_ATTRIB_NAME(rwMic.rw, L"rwMic");
		//BO_ATTRIB_MARKUP_ATTRIB(rwMac);
		//BO_ATTRIB_MARKUP_ATTRIB_NAME(colMic.col, L"colMic");
		//BO_ATTRIB_MARKUP_ATTRIB(colMac);
*/
		//BO_ATTRIB_MARKUP_ATTRIB_NAME(ref_, L"ref");
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS


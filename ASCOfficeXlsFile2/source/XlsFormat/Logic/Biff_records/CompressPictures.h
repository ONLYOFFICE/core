#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CompressPictures record in BIFF8
class CompressPictures: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CompressPictures)
	BASE_OBJECT_DEFINE_CLASS_NAME(CompressPictures)
public:
	CompressPictures();
	~CompressPictures();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<unsigned int> fAutoCompressPictures;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fAutoCompressPictures)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS


#pragma once

#include "Biff_records/BiffRecord.h"

namespace XLS
{;

class CFStreamCacheReader;

// Logical representation of a record in BIFF8
class AnyObject: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ANY_TYPE)
	BASE_OBJECT_DEFINE_CLASS_NAME(ANY_TYPE)
public:
	AnyObject();
	~AnyObject();

	BaseObjectPtr clone();
	virtual void writeFields(CFRecord& record);
	virtual void readFields(CFRecord& record);

	std::string tag_name;
	boost::shared_ptr<char> raw_data;
	size_t raw_data_size;

public:
	BO_ATTRIB_MARKUP_BEGIN
		//xml_tag->Puttext(STR::bin2str(raw_data.get(), raw_data_size).c_str());
		
		//CDllLog *log = new CDllLog(L"c:\\Temp\\for_test\\OutFiles\\log.txt", 1);

		//log->write((char*)(tag_name.c_str()), (char*)STR::int2str(raw_data_size, 10).c_str());
		//delete log;		

		Log::info("<" + tag_name + ">" + STR::bin2str(raw_data.get(), raw_data_size) + "</" + tag_name + ">");
	BO_ATTRIB_MARKUP_END
};

} // namespace XLS

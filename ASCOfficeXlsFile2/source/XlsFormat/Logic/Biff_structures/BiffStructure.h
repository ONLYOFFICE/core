#pragma once

#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "../XlsElementsType.h"

#include <Logging/Log.h>

#include "../../Binary/CFRecord.h"



namespace XLS
{

class BiffStructure_NoVtbl
{
public:
	void load(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))
	void store(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))

};

class BiffStructure;
typedef boost::shared_ptr<BiffStructure>	BiffStructurePtr;
typedef std::vector<BiffStructurePtr>		BiffStructurePtrVector;

class BiffStructure : protected BiffStructure_NoVtbl
{
public:

	virtual BiffStructurePtr clone() = 0;

	virtual void load(CFRecord& record) = 0;
	virtual void store(CFRecord& record) = 0;
	
	virtual ElementType get_type() = 0;

	virtual int serialize(std::wostream & _stream)
	{
        std::stringstream s;
        s << std::string("This element - ") << getClassName() << std::string("- not serialize");
		Log::warning(s.str());
		return 0;
	}

    virtual const std::string & getClassName() const = 0;   // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	public: \
        const std::string & getClassName() const { static std::string  str(#class_name); return str;};\
		virtual XLS::ElementType get_type() { return type; }
	


bool DiffBiff(BiffStructure_NoVtbl & val);
bool DiffBiff(BiffStructure & val);


template<class T>
CFRecord& operator>>(CFRecord& record, T& val)
{
	if(DiffBiff(val))
	{
		record.loadAnyData(val);
	}
	else
	{
		val.load(record);
	}
	return record;
}


template<class T>
CFRecord& operator<<(CFRecord& record, T& val)
{
	if(DiffBiff(val))
	{
		record.storeAnyData(val);
	}
	else
	{
		val.store(record);
	}
	return record;
}



} // namespace XLS


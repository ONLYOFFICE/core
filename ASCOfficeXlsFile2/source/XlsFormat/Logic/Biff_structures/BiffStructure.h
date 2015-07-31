#pragma once

#include <vector>
#include <sstream>
#include <boost/shared_ptr.hpp>

#include "../XlsElementsType.h"

#include <Auxiliary/BetterVariantT.h>
#include <Logging/Log.h>


namespace XLS
{;

class CFRecord;


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
	
	static const ElementType	type = typeBiffStructure;
	virtual ElementType			get_type() const { return type; }

	virtual int serialize(std::wostream & _stream)
	{
		std::wstringstream s;
		s << std::wstring(L"This element - ") << getClassName() << std::wstring(L"- not serialize");
		Log::warning(s.str());
		return 0;
	}

	virtual const std::wstring & getClassName() const = 0;   // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	public: \
		const std::wstring & getClassName() const { static std::wstring  str(L# class_name); return str; };
	


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


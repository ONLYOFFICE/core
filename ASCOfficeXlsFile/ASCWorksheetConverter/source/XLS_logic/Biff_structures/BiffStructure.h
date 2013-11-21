#pragma once

#include <Auxiliary/BetterVariantT.h>
//#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class BiffStructure_NoVtbl
{
public:
	void load(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))
	void store(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))

// protected:
// 	static MSXML2::IXMLDOMElementPtr createElement(MSXML2::IXMLDOMElementPtr parent, const _bstr_t tag_name);
};

class BiffStructure;
typedef boost::shared_ptr<BiffStructure> BiffStructurePtr;

class BiffStructure : protected BiffStructure_NoVtbl
{
public:
	virtual BiffStructurePtr clone() = 0;

	// Makes XML data
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	// Reads XML data
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent, const size_t position, const bool is_mandatory);

	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);


	virtual void load(CFRecord& record) = 0;
	virtual void store(CFRecord& record) = 0;

	virtual const _bstr_t& getClassName() const = 0;   // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter
protected:
	const AUX::BetterVariantT getStructAttribute(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name);
	MSXML2::IXMLDOMElementPtr getStructNode(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& tag_name);
};

#define BASE_OBJECT_DEFINE_CLASS_NAME(class_name) public: const _bstr_t& getClassName() const { static _bstr_t str(L# class_name); return str; }

typedef std::vector<BiffStructurePtr> BiffStructurePtrVector;


bool DiffBiff(BiffStructure_NoVtbl& val);
bool DiffBiff(BiffStructure& val);


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


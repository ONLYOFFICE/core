#pragma once

#include "BiffStructure.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;

class BiffAttribute : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(BiffAttribute)
public:
	BiffAttribute() {};
	BiffAttribute(const _bstr_t name_init) : attrib_name(name_init) {};
	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name) = 0;
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name) = 0;

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent)
	{
		toXML(parent, attrib_name);
	};
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag)
	{
		return fromXML(xml_tag, attrib_name);
	};

public:
	void setName(const _bstr_t name_init)
	{
		attrib_name = name_init;
	}

private:
	nullable<_bstr_t> attrib_name;
};




template<class Type>
class BiffAttributeSimple : public BiffAttribute
{
public:
	BiffAttributeSimple<Type>() {};
	BiffAttributeSimple<Type>(const Type& val_init) : val(val_init) {};
	BiffAttributeSimple<Type>(const Type& val_init, const _bstr_t& attrib_name) : val(val_init), BiffAttribute(attrib_name) {};


	BiffStructurePtr clone() 
	{
		return BiffStructurePtr(new BiffAttributeSimple<Type>(*this));
	};

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
	{
		parent->setAttribute(attrib_name, static_cast<Type>(val));
	};
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag, const _bstr_t& attrib_name)
	{
		val = getStructAttribute(xml_tag, attrib_name);
		return true;
	};
	virtual void load(CFRecord& record)
	{
		record >> val;
	};
	virtual void store(CFRecord& record)
	{
		record << val;
	};

	operator const Type () const { return static_cast<Type>(val); };
	operator const _variant_t () const { return static_cast<Type>(val);  };
	BiffAttributeSimple<Type> operator= (const BiffAttributeSimple<Type>& other)
	{
		val = other.val;
		return *this;
	};

	BiffAttributeSimple<Type> operator= (const Type& other_val)
	{
		 val = other_val;
		 return *this;
	};

protected:
	nullable<Type> val;
};



template<class Type>
class ForwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
	ForwardOnlyParam() {};
	ForwardOnlyParam(const Type& val_init) : BiffAttributeSimple(val_init) {};

	ForwardOnlyParam<Type> operator= (const ForwardOnlyParam<Type>& other)
	{
		return BiffAttributeSimple<Type>::operator=(other);
	};

	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag)
	{
		// do nothing
		return true;
	};
};


template<class Type>
class BackwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
	BackwardOnlyParam() {};
	BackwardOnlyParam(const Type& val_init) : BiffAttributeSimple(val_init) {};

	BackwardOnlyParam<Type> operator= (const BackwardOnlyParam<Type>& other)
	{
		return BiffAttributeSimple<Type>::operator=(other);
	};

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent)
	{
		// do nothing
	};
};



} // namespace XLS

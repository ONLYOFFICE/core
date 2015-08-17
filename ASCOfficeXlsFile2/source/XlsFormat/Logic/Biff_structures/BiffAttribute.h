#pragma once

#include "BiffStructure.h"

namespace XLS
{

class BiffAttribute : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BiffAttribute)
public:
    BiffAttribute() {}
    BiffAttribute(const std::wstring  name_init) : attrib_name(name_init) {}

	void setName(const std::wstring  name_init)
	{
		attrib_name = name_init;
	}
	static const ElementType	type = typeBiffAttribute;

	_CP_OPT(std::wstring) attrib_name;
};




template<class Type>
class BiffAttributeSimple : public BiffAttribute
{
public:
    BiffAttributeSimple() {}
    BiffAttributeSimple<Type>(const Type& val_init) : val(val_init) {}
    BiffAttributeSimple<Type>(const Type& val_init, const std::wstring & attrib_name) : val(val_init), BiffAttribute(attrib_name) {}

    _CP_OPT(Type) & value() {return val;}

	BiffStructurePtr clone() 
	{
        return BiffStructurePtr(new BiffAttributeSimple<Type>(*this));
    }
	virtual void load(CFRecord& record)
	{
		record >> val;
    }
	virtual void store(CFRecord& record)
	{
		record << val;
    }

    operator const Type () const { return (val ? static_cast<Type>(*val) : (Type)0); }
	
    BiffAttributeSimple<Type> operator= (const BiffAttributeSimple<Type>& other)
	{
		val = other.val;
		return *this;
    }

    BiffAttributeSimple<Type> operator= (const Type& other_val)
	{
		 val = other_val;
		 return *this;
    }

    //const bool operator== (const Type & F2){return (val == F2);}

    bool operator==(const Type & F2){return (val == F2);}

protected:
    boost::optional<Type> val;
};

//BiffAttributeSimple<std::wstring>::operator ==(const Type &F2)

template<class Type>
class ForwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
    ForwardOnlyParam() {}
    ForwardOnlyParam(const Type& val_init)
    {
        BiffAttributeSimple<Type>::val = val_init;
    }

    ForwardOnlyParam<Type> operator= (const ForwardOnlyParam<Type>& other)
	{
       BiffAttributeSimple<Type>::val = other.val;
    }

};


template<class Type>
class BackwardOnlyParam : public BiffAttributeSimple<Type>
{
public:
    BackwardOnlyParam() {}
    BackwardOnlyParam(const Type& val_init)
    {
         BiffAttributeSimple<Type>::val = val_init;
    }

    BackwardOnlyParam<Type> operator= (const BackwardOnlyParam<Type>& other)
	{
        BiffAttributeSimple<Type>::val = other.val;
    }

};



} // namespace XLS

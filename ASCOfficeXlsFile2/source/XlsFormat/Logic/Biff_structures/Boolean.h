#pragma once

#include "BiffAttribute.h"

namespace XLS
{

template<class Type>
struct Boolean : public BiffAttributeSimple<Type>
{
    Boolean<Type>() {}
    Boolean<Type>(const Type& val) : BiffAttributeSimple<Type>(val) {}
	BiffStructurePtr clone()
	{
		return BiffStructurePtr(new Boolean<Type>(*this));
    }
	operator const bool()
	{
        return static_cast<Type>(1) == BiffAttributeSimple<Type>::val.get();
    }
	operator const Type()
	{
        return BiffAttributeSimple<Type>::val.get();
    }
    operator const std::wstring() const	{ return toString();}
	const std::wstring toString() const
	{
        return static_cast<Type>(1) == BiffAttributeSimple<Type>::val ? L"true" : L"false";
    }
	Boolean<Type> operator=(const Boolean<Type>& other)
	{
        BiffAttributeSimple<Type>::val = other.val;
		return *this;
    }

};

} // namespace XLS


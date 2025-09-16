#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <string>
#include <vector>
#include <list>

namespace HWP
{
typedef char16_t HWP_CHAR;
typedef std::wstring HWP_STRING;
typedef char HWP_BYTE;

#define LIST std::list
#define VECTOR std::vector

#define TO_HWP_STRING(val) std::to_wstring(val)

#define CLEAR_ARRAY(type, array) \
	for (type* pValue : array) \
		if (nullptr != pValue) \
			delete pValue

#define SWITCH(type, value) switch(static_cast<type>(value))
#define CASE(value) case value : return value
#define DEFAULT(value) case value: default: return value

#define MAKE_STR(value) #value
#define STR(value) MAKE_STR(#value)
#define MAKE_WSTR(value) L##value
#define WSTR(value) MAKE_WSTR(#value)

#define IF_STRING_IN_ENUM(checked_value, value, enum_type)\
	if (WSTR(checked_value) == value)\
		return enum_type::checked_value
#define ELSE_IF_STRING_IN_ENUM(checked_value, value, enum_type)\
	else if (WSTR(checked_value) == value)\
		return enum_type::checked_value
#define ELSE_STRING_IN_ENUM(checked_value, enum_type)\
	else\
		return enum_type::checked_value

#define RETURN_VECTOR_CONST_PTR(type, array_values) \
	std::vector<const type*> arTempVector(array_values.size()); \
	for (unsigned int unIndex = 0; unIndex < array_values.size(); ++unIndex) \
		arTempVector[unIndex] = dynamic_cast<const type*>(array_values[unIndex]); \
	return arTempVector

#define TO_LOWER(value) std::transform(value.begin(), value.end(), value.begin(), tolower)
#define TO_UPPER(value) std::transform(value.begin(), value.end(), value.begin(), toupper)

class IRef
{
	unsigned long m_ulRef;
public:
	IRef()
	{
		m_ulRef = 1;
	}

	virtual ~IRef()
	{
	}

	virtual unsigned long AddRef()
	{
		++m_ulRef;
		return m_ulRef;
	}

	virtual unsigned long Release()
	{
		unsigned long ulRet = --m_ulRef;
		if (0 == m_ulRef)
			delete this;
		return ulRet;
	}
};
}
#endif // COMMON_H

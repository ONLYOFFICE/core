#ifndef COMMON_H
#define COMMON_H

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

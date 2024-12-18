#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <list>

namespace HWP
{
typedef char16_t CHAR;
typedef std::wstring STRING;
typedef char BYTE;

#define LIST std::list
#define VECTOR std::vector

#define TO_STRING(val) std::to_wstring(val)

#define CLEAR_ARRAY(type, array) \
	for (type* pValue : array) \
		if (nullptr != pValue) \
			delete pValue

#define SWITCH(type, value) switch(static_cast<type>(value))
#define CASE(value) case value : return value
#define DEFAULT(value) case value: default: return value
}
#endif // COMMON_H

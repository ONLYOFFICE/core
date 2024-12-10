#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <list>

namespace HWP
{
#define CHAR char16_t
#define STRING std::string
#define BYTE char
#define LIST std::list
#define VECTOR std::vector

#define CLEAR_ARRAY(type, array) \
	for (type* pValue : array) \
		if (nullptr != pValue) \
			delete pValue

#define SWITCH(type, value) switch(static_cast<type>(value))
#define CASE(value) case value : return value
#define DEFAULT(value) case value: default: return value
}
#endif // COMMON_H

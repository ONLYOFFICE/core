#ifndef _AVS_IAMGEMETADATA_UTILS_H_INCLUDED_
#define _AVS_IAMGEMETADATA_UTILS_H_INCLUDED_
#pragma once 

#include <string>
#include <sstream>

namespace AVS
{
	namespace Utils
	{
		/*!
		*	\brief	Перевод строки wchar_t * в char * в соответствии с заданной кодовой страницей
		*	
		*	\param[in]	wstr		исходная строка
		*	\param[in]	codepage	Кодовая страница, по умолчанию используется CP_ACP — текущая кодовая страница Windows
		*
		*/
		extern std::string __WtoS(const wchar_t* wstr, int codepage = 0);
		extern std::wstring __StoW(const char * str, int codepage = 0);

		template <typename Target, typename Source>
		Target lexical_cast(Source arg)
		{
			std::wstringstream interpreter;
			Target result;
			if (!(interpreter << arg) || !(interpreter >> result) || !(interpreter >> std::ws).eof())
				throw std::bad_cast();
			return result;
		}
	}
}
#endif
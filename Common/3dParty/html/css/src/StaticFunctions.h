#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../../DesktopEditor/common/File.h"
#include <algorithm>
#include <cwctype>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

namespace NSCSS
{
	namespace NS_STATIC_FUNCTIONS
	{
		std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding);
		std::string GetContentAsUTF8(const std::wstring& sFileName);

		double ReadDouble(const std::wstring& wsValue);
		std::vector<double> ReadDoubleValues(const std::wstring& wsValue);

		std::vector<std::wstring> ParseCSSPropertie(const std::wstring& wsInput);
		std::vector<std::wstring> GetWordsW(const std::wstring& wsLine, bool bWithSigns = false, const std::wstring& wsDelimiters = L" \n\r\t\f\v:;,!");
		std::vector<unsigned short int> GetWeightSelector(const std::wstring& sSelector);
		std::map<std::wstring, std::wstring> GetRules(const std::wstring& wsStyles);

		void RemoveSpaces(std::wstring& wsString);
		
		double CalculatePersentage(const std::wstring& wsValue, double dRelativeValue);
	}

	#define SWITCH(str)  switch(SWITCH_CASE::str_hash_for_switch(str))
	#define CASE(str)    static_assert(SWITCH_CASE::str_is_correct(str) && (SWITCH_CASE::str_len(str) <= SWITCH_CASE::MAX_LEN),\
	"CASE string contains wrong characters, or its length is greater than 9");\
	case SWITCH_CASE::str_hash(str, SWITCH_CASE::str_len(str))
	#define DEFAULT  default

	namespace SWITCH_CASE
	{
		typedef unsigned long long ullong;

		const unsigned int MAX_LEN = 32;
		const ullong N_HASH = static_cast<ullong>(-1);

		constexpr ullong raise_128_to(const wchar_t power)
		{
			return (1ULL << 7) * power;
		}

		constexpr bool str_is_correct(const wchar_t* const str)
		{
			return (static_cast<wchar_t>(*str) > 0) ? str_is_correct(str + 1) : (*str ? false : true);
		}

		constexpr unsigned int str_len(const wchar_t* const str)
		{
			return *str ? (1 + str_len(str + 1)) : 0;
		}

		constexpr ullong str_hash(const wchar_t* const str, const wchar_t current_len)
		{
			return *str ? (raise_128_to(current_len - 1) * static_cast<wchar_t>(*str) + str_hash(str + 1, current_len - 1)) : 0;
		}

		inline ullong str_hash_for_switch(const wchar_t* const str)
		{
			return (str_is_correct(str) && (str_len(str) <= MAX_LEN)) ? str_hash(str, str_len(str)) : N_HASH;
		}

		inline ullong str_hash_for_switch(const std::wstring& str)
		{
			return (str_is_correct(str.c_str()) && (str.length() <= MAX_LEN)) ? str_hash(str.c_str(), str.length()) : N_HASH;
		}
	}
}

#endif // STATICFUNCTIONS_H

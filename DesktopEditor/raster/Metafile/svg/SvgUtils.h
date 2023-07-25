#ifndef SVGUTILS_H
#define SVGUTILS_H

#include <algorithm>
#include <string>
#include <vector>
#include <regex>
#include <map>

namespace SVG
{
	#define ADD_COLOR( COLOR, R, G, B ) m_Table.insert(std::pair<std::wstring, unsigned int>( L##COLOR, ( R << 0 ) | ( G << 8 ) | ( B << 16 ) ))

	enum Metrics
	{
		EM,
		EX,
		PX,
		PT,
		PC,
		CM,
		MM,
		INCH,

		PCT,	//	percent

		UNDEFINED
	};

	class StrUtils
	{
	public:
		static inline std::vector<double> ReadDoubleValues(std::wstring::const_iterator oBegin, std::wstring::const_iterator oEnd)
		{
			std::vector<double> arValues;

			std::wregex oPattern(LR"([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)");

			std::wsregex_iterator oIter(oBegin, oEnd, oPattern);
			std::wsregex_iterator oEndIter;

			for (; oIter != oEndIter; ++oIter)
				arValues.push_back(std::stod(oIter->str()));

			return arValues;
		}

		static inline std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
		{
			return ReadDoubleValues(wsValue.begin(), wsValue.end());
		}

		static inline long LongValue(const std::wstring& value)
		{
			size_t len = value.length(); const wchar_t* buf = value.c_str();
			if (0 == len) return 0;
			for (size_t i = 0; i < len; ++i)
			{
				if (iswdigit(buf[i]) || (buf[i] == L'-') /* || (value[i] == L'.') || (value[i] == L',') */)
					continue;

				return std::stol(value.substr(0, i));
			}

			return std::stol(value);
		}
		static inline double DoubleValue(const std::wstring& value)
		{
			size_t len = value.length(); const wchar_t* buf = value.c_str();
			if (0 == len) return 0;
			for (size_t i = 0; i < len; ++i)
			{
				if (iswdigit(buf[i]) || (buf[i] == L'.') || (buf[i] == L',') || (buf[i] == L'-') || (buf[i] == 'e'))
					continue;

				return std::stol(value.substr(0, i));
			}

			bool bNeedZero = false;
			bool bNeedMZero = false;
			if(value.front() == L'-')
			{
				if(value.length() > 1)
					if(value[1] == L'.')
						bNeedMZero = true;
			}
			else if(value.front() == L'.')
				bNeedZero = true;
			return std::stod(bNeedZero ? L'0' + value : (bNeedMZero ? L"-0" + value.substr(1) : value));
		}
		static inline double DoubleValuePct(const std::wstring& sVal)
		{
			bool bNeedZero = false;
			bool bNeedMZero = false;
			if(!sVal.empty())
			{
				if(sVal.front() == L'-')
				{
					if(sVal.length() > 1)
						if(sVal[1] == L'.')
							bNeedMZero = true;
				}
				else if(sVal.front() == L'.')
					bNeedZero = true;
				if ('%' == sVal.back())
					return std::stod(sVal.substr(0, sVal.length() - 1)) * 0.01;
			}
			else
				return 0;
			return std::stod(bNeedZero ? L'0' + sVal : (bNeedMZero ? L"-0" + sVal.substr(1) : sVal));
		}
		static bool DoubleValues(const std::wstring& SourceW, std::vector<double>& Values)
		{
			std::wstring number	= L"";
			size_t length = SourceW.length();
			const wchar_t* Source = SourceW.c_str();

			for (size_t i = 0; i < length; ++i)
			{
				if ('-' == Source[i])
				{
					if (i > 1)
					{
						if ('e' == Source[i - 1])
						{
							number += Source[i];
							continue;
						}
					}
					if (!number.empty())
						Values.push_back(std::stod(number.front() == L'.' ? L'0' + number : number));
					number = L"";
				}

				if (iswdigit(Source[i]) || (Source[i] == '.') || (Source[i] == '-') || (Source[i] == 'e'))
				{
					number += Source[i];
					continue;
				}

				if (!number.empty())
				{
					bool bNeedZero = false;
					bool bNeedMZero = false;
					if(number.front() == L'-')
					{
						if(number.length() > 1)
							if(number[1] == L'.')
								bNeedMZero = true;
					}
					else if(number.front() == L'.')
						bNeedZero = true;
					Values.push_back(std::stod(bNeedZero ? L'0' + number : (bNeedMZero ? L"-0" + number.substr(1) : number)));
				}
				number = L"";
			}

			if (!number.empty())
				Values.push_back(std::stod(number.front() == L'.' ? L'0' + number : number));

			return (0 != Values.size());
		}

		static inline std::wstring RemoveSymbol(std::wstring& src, wchar_t symbol)
		{
			std::wstring result;
			const wchar_t* buf = src.c_str();
			size_t len = src.length();
			for (size_t i = 0; i < len; i++)
			{
				wchar_t currentChar = buf[i];
				if (currentChar != symbol)
					result += currentChar;
			}
			src = result;
			return result;
		}

		static inline std::wstring RemoveSpaces(std::wstring& src)
		{
			return RemoveSymbol(src, ' ');
		}

		static inline Metrics GetMetrics(const std::wstring& value )		//	only for long numbers in value
		{
			const wchar_t* buf = value.c_str();
			size_t len = value.length();
			for (size_t i = 0; i < len; ++i)
			{
				if (isdigit(buf[i]) || (buf[i] == '.') || (buf[i] == ','))
					continue;

				std::wstring metrics = value.substr(i, len - i);

				if (metrics.empty())
					return PX;
				if (metrics == L"px")
					return PX;
				if (metrics == L"mm")
					return MM;
				if (metrics == L"cm")
					return CM;
				if (metrics == L"%")
					return PCT;
				if (metrics == L"in")
					return INCH;
				if (metrics == L"em")  //	default	-	16px
					return EM;
				if (metrics == L"pt")
					return PT;

				return UNDEFINED;
			}

			return UNDEFINED;
		}

		static inline void ConvertValue(double &dValue, Metrics eMetrics)
		{
			switch (eMetrics)
			{
			case EM: break;
			case EX: break;
			case PX: dValue *= 96 / 25.4;
			case PT: break;
			case PC: break;
			case CM: break;
			case MM: break;
			case INCH: break;

			case PCT: break;

			case UNDEFINED: break;
			}
		}

		static inline std::wstring UrlRefValue(const std::wstring& sUrlRef)
		{
			if (sUrlRef.length() > 3)
			{
				std::wstring str = sUrlRef;
				str = StrUtils::RemoveSpaces(str);
				const wchar_t* buf = str.c_str();

				if ((buf[0] == 'u') || (buf[1] == 'r') || (buf[2] == 'l'))
				{
					std::wstring::size_type Oct = str.find(L"#");
					if (std::wstring::npos != Oct)
					{
						Oct += 1; // #
						if (')' == str[str.length() - 1])
							return str.substr(Oct, str.length() - Oct - 1);

						return str.substr(Oct, str.length() - Oct);
					}
				}
			}

			return L"";
		}
		static inline bool UrlRefValue2(std::wstring& sUrlRef)
		{
			std::wstring sRet = StrUtils::UrlRefValue(sUrlRef);

			if (!sRet.empty())
			{
				sUrlRef = sRet;
				return true;
			}

			return false;
		}

		static inline int Decode16(wchar_t c)
		{
			if (c >= '0' && c <= '9')
				return c - '0';
			if (c >= 'A' && c <= 'F')
				return 10 + c - 'A';
			if (c >= 'a' && c <= 'f')
				return 10 + c - 'a';
			return 0;
		}

		static std::wstring UrlDecode(const std::wstring& Source)
		{
			std::wstring strDecoded;

			const wchar_t* buf = Source.c_str();
			size_t len = Source.length();

			for ( size_t i = 0; i < len; ++i )
			{
				if ('%' == buf[i] && isxdigit(buf[i + 1]) && isxdigit(buf[i + 2]))
				{
					int byHex1 = Decode16(buf[i + 1]);
					int byHex2 = Decode16(buf[i + 2]);

					strDecoded += (wchar_t)(byHex1 * 16 + byHex2);

					i += 2;
				}
				else
					strDecoded += buf[i];
			}

			return strDecoded;
		}

		static std::wstring TrimExtraEnding(const std::wstring& wsString)
		{
			std::wstring::const_iterator itBeginPos = std::find_if(wsString.begin(), wsString.end(), [](const wchar_t& wChar){ return !iswspace(wChar);});
			std::wstring::const_reverse_iterator itEndPos =  std::find_if(wsString.rbegin(), wsString.rend(), [](const wchar_t& wChar){ return !iswspace(wChar);});

			if (wsString.end() == itBeginPos)
				return std::wstring();

			return std::wstring(itBeginPos, itEndPos.base()) + ((wsString.end() != itEndPos.base()) ? L" " : L"");
		}
	};
}

#endif // SVGUTILS_H

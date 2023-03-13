#ifndef SVGUTILS_H
#define SVGUTILS_H

#include <algorithm>
#include <string>
#include <vector>
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
		static inline std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
		{
			std::vector<double> arValues;

			std::wstring::const_iterator oFirstPos = wsValue.begin();
			std::wstring::const_iterator oSecondPos = oFirstPos;

			while (true)
			{
				oFirstPos  = std::find_if(oSecondPos, wsValue.end(), [](const wchar_t& wChar){ return iswdigit(wChar) || L'-' == wChar; });

				if (wsValue.end() == oFirstPos)
					break;

				oSecondPos = std::find_if(oFirstPos + 1,  wsValue.end(), [](const wchar_t& wChar){ return iswspace(wChar) || L',' == wChar; });

				arValues.push_back(std::stod(std::wstring(oFirstPos, oSecondPos)));
			}

			return arValues;
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

			return std::wstring(itBeginPos, itEndPos.base());
		}
	};


	class ColorTable
	{
	public:
		ColorTable ()
		{
			InitClrTable ();
		}

		inline int Find(const std::wstring& sKey)
		{
			std::map<std::wstring, unsigned int>::iterator iter = m_Table.find(sKey);
			if (iter == m_Table.end())
				return -1;

			unsigned int clr = iter->second;
			return (int)clr;
			//return ((clr & 0xFF) << 16) | ((clr & 0xFF00)) | ((clr & 0xFF0000) >> 16);
		}

	private:
		void InitClrTable()
		{
			if (m_Table.size())
				return;

			ADD_COLOR("aliceblue", 240, 248, 255);
			ADD_COLOR("antiquewhite", 250, 235, 215);
			ADD_COLOR("aqua",  0, 255, 255);
			ADD_COLOR("aquamarine", 127, 255, 212);
			ADD_COLOR("azure", 240, 255, 255);
			ADD_COLOR("beige", 245, 245, 220);
			ADD_COLOR("bisque", 255, 228, 196);
			ADD_COLOR("black",  0, 0, 0);
			ADD_COLOR("blanchedalmond", 255, 235, 205);
			ADD_COLOR("blue",  0, 0, 255);
			ADD_COLOR("blueviolet", 138, 43, 226);
			ADD_COLOR("brown", 165, 42, 42);
			ADD_COLOR("burlywood", 222, 184, 135);
			ADD_COLOR("cadetblue",  95, 158, 160);
			ADD_COLOR("chartreuse", 127, 255, 0);
			ADD_COLOR("chocolate", 210, 105, 30);
			ADD_COLOR("coral", 255, 127, 80);
			ADD_COLOR("cornflowerblue", 100, 149, 237);
			ADD_COLOR("cornsilk", 255, 248, 220);
			ADD_COLOR("crimson", 220, 20, 60);
			ADD_COLOR("cyan",  0, 255, 255);
			ADD_COLOR("darkblue",  0, 0, 139);
			ADD_COLOR("darkcyan",  0, 139, 139);
			ADD_COLOR("darkgoldenrod", 184, 134, 11);
			ADD_COLOR("darkgray", 169, 169, 169);
			ADD_COLOR("darkgreen",  0, 100, 0);
			ADD_COLOR("darkgrey", 169, 169, 169);
			ADD_COLOR("darkkhaki", 189, 183, 107);
			ADD_COLOR("darkmagenta", 139, 0, 139);
			ADD_COLOR("darkolivegreen",  85, 107, 47);
			ADD_COLOR("darkorange", 255, 140, 0);
			ADD_COLOR("darkorchid", 153, 50, 204);
			ADD_COLOR("darkred", 139, 0, 0);
			ADD_COLOR("darksalmon", 233, 150, 122);
			ADD_COLOR("darkseagreen", 143, 188, 143);
			ADD_COLOR("darkslateblue",  72, 61, 139);
			ADD_COLOR("darkslategray",  47, 79, 79);
			ADD_COLOR("darkslategrey",  47, 79, 79);
			ADD_COLOR("darkturquoise",  0, 206, 209);
			ADD_COLOR("darkviolet", 148, 0, 211);
			ADD_COLOR("deeppink", 255, 20, 147);
			ADD_COLOR("deepskyblue",  0, 191, 255);
			ADD_COLOR("dimgray", 105, 105, 105);
			ADD_COLOR("dimgrey", 105, 105, 105);
			ADD_COLOR("dodgerblue",  30, 144, 255);
			ADD_COLOR("firebrick", 178, 34, 34);
			ADD_COLOR("floralwhite", 255, 250, 240);
			ADD_COLOR("forestgreen",  34, 139, 34);
			ADD_COLOR("fuchsia", 255, 0, 255);
			ADD_COLOR("gainsboro", 220, 220, 220);
			ADD_COLOR("ghostwhite", 248, 248, 255);
			ADD_COLOR("gold", 255, 215, 0);
			ADD_COLOR("goldenrod", 218, 165, 32);
			ADD_COLOR("gray", 128, 128, 128);
			ADD_COLOR("grey", 128, 128, 128);
			ADD_COLOR("green",  0, 128, 0);
			ADD_COLOR("greenyellow", 173, 255, 47);
			ADD_COLOR("honeydew", 240, 255, 240);
			ADD_COLOR("hotpink", 255, 105, 180);
			ADD_COLOR("indianred", 205, 92, 92);
			ADD_COLOR("indigo",  75, 0, 130);
			ADD_COLOR("ivory", 255, 255, 240);
			ADD_COLOR("khaki", 240, 230, 140);
			ADD_COLOR("lavender", 230, 230, 250);
			ADD_COLOR("lavenderblush", 255, 240, 245);
			ADD_COLOR("lawngreen", 124, 252, 0);
			ADD_COLOR("lemonchiffon", 255, 250, 205);
			ADD_COLOR("lightblue", 173, 216, 230);
			ADD_COLOR("lightcoral", 240, 128, 128);
			ADD_COLOR("lightcyan", 224, 255, 255);
			ADD_COLOR("lightgoldenrodyellow", 250, 250, 210);
			ADD_COLOR("lightgray", 211, 211, 211);
			ADD_COLOR("lightgreen", 144, 238, 144);
			ADD_COLOR("lightgrey", 211, 211, 211);
			ADD_COLOR("lightpink", 255, 182, 193);
			ADD_COLOR("lightsalmon", 255, 160, 122);
			ADD_COLOR("lightseagreen",  32, 178, 170);
			ADD_COLOR("lightskyblue", 135, 206, 250);
			ADD_COLOR("lightslategray", 119, 136, 153);
			ADD_COLOR("lightslategrey", 119, 136, 153);
			ADD_COLOR("lightsteelblue", 176, 196, 222);
			ADD_COLOR("lightyellow", 255, 255, 224);
			ADD_COLOR("lime",  0, 255, 0);
			ADD_COLOR("limegreen",  50, 205, 50);
			ADD_COLOR("linen", 250, 240, 230);
			ADD_COLOR("magenta", 255, 0, 255);
			ADD_COLOR("maroon", 128, 0, 0);
			ADD_COLOR("mediumaquamarine", 102, 205, 170);
			ADD_COLOR("mediumblue",  0, 0, 205);
			ADD_COLOR("mediumorchid", 186, 85, 211);
			ADD_COLOR("mediumpurple", 147, 112, 219);
			ADD_COLOR("mediumseagreen",  60, 179, 113);
			ADD_COLOR("mediumslateblue", 123, 104, 238);
			ADD_COLOR("mediumspringgreen",  0, 250, 154);
			ADD_COLOR("mediumturquoise",  72, 209, 204);
			ADD_COLOR("mediumvioletred", 199, 21, 133);
			ADD_COLOR("midnightblue",  25, 25, 112);
			ADD_COLOR("mintcream", 245, 255, 250);
			ADD_COLOR("mistyrose", 255, 228, 225);
			ADD_COLOR("moccasin", 255, 228, 181);
			ADD_COLOR("navajowhite", 255, 222, 173);
			ADD_COLOR("navy",  0, 0, 128);
			ADD_COLOR("oldlace", 253, 245, 230);
			ADD_COLOR("olive", 128, 128, 0);
			ADD_COLOR("olivedrab", 107, 142, 35);
			ADD_COLOR("orange", 255, 165, 0);
			ADD_COLOR("orangered", 255, 69, 0);
			ADD_COLOR("orchid", 218, 112, 214);
			ADD_COLOR("palegoldenrod", 238, 232, 170);
			ADD_COLOR("palegreen", 152, 251, 152);
			ADD_COLOR("paleturquoise", 175, 238, 238);
			ADD_COLOR("palevioletred", 219, 112, 147);
			ADD_COLOR("papayawhip", 255, 239, 213);
			ADD_COLOR("peachpuff", 255, 218, 185);
			ADD_COLOR("peru", 205, 133, 63);
			ADD_COLOR("pink", 255, 192, 203);
			ADD_COLOR("plum", 221, 160, 221);
			ADD_COLOR("powderblue", 176, 224, 230);
			ADD_COLOR("purple", 128, 0, 128);
			ADD_COLOR("red", 255, 0, 0);
			ADD_COLOR("rosybrown", 188, 143, 143);
			ADD_COLOR("royalblue",  65, 105, 225);
			ADD_COLOR("paddlebrown", 139, 69, 19);
			ADD_COLOR("palmon", 250, 128, 114);
			ADD_COLOR("pandybrown", 244, 164, 96);
			ADD_COLOR("peagreen",  46, 139, 87);
			ADD_COLOR("peashell", 255, 245, 238);
			ADD_COLOR("pienna", 160, 82, 45);
			ADD_COLOR("pilver", 192, 192, 192);
			ADD_COLOR("pkyblue", 135, 206, 235);
			ADD_COLOR("plateblue", 106, 90, 205);
			ADD_COLOR("plategray", 112, 128, 144);
			ADD_COLOR("plategrey", 112, 128, 144);
			ADD_COLOR("pnow", 255, 250, 250);
			ADD_COLOR("ppringgreen",  0, 255, 127);
			ADD_COLOR("pteelblue",  70, 130, 180);
			ADD_COLOR("tan", 210, 180, 140);
			ADD_COLOR("teal",  0, 128, 128);
			ADD_COLOR("thistle", 216, 191, 216);
			ADD_COLOR("tomato", 255, 99, 71);
			ADD_COLOR("turquoise",  64, 224, 208);
			ADD_COLOR("violet", 238, 130, 238);
			ADD_COLOR("wheat", 245, 222, 179);
			ADD_COLOR("white", 255, 255, 255);
			ADD_COLOR("whitesmoke", 245, 245, 245);
			ADD_COLOR("yellow", 255, 255, 0);
			ADD_COLOR("yellowgreen", 154, 205, 50);
		}
	private:

		std::map<std::wstring, unsigned int> m_Table;
	};
	class ColorParser
	{
	public:
		long ColorFromString(const std::wstring& sColor) const
		{
			if (sColor.length() < 4)
				return 0;
			// HEX VALUE
			const wchar_t* buf = sColor.c_str();
			size_t len = sColor.length();

			std::wstring::size_type index = sColor.find(L"#");

			if (std::wstring::npos != index)
			{
				for (std::wstring::size_type i = index; i < len; ++i)
				{
					if (' ' == buf[i])
					{
						return ColorFromHexString(sColor.substr(index, i - index));
					}
				}

				return ColorFromHexString(sColor.substr(index, len - index));
			}

			if (L"none" == sColor)
				return -2;

			// 'rgb(x,x,x)'
			if (3 <= len && (buf[0] == 'r') && (buf[1] == 'g') && (buf[2] == 'b'))
				return ColorFromRgbString(sColor);

			// COLOR TABLE
			return m_oTable.Find(sColor);
		}

		long ColorFromRgbString(const std::wstring& Rgb) const
		{
			bool PCT = false;
			double Color [ 4 ]	=	{ 0.0, 0.0, 0.0, 255 };

			bool Begin = false;
			size_t IndS = 0;
			size_t IndCol =	0;

			const wchar_t* buf = Rgb.c_str();
			size_t len = Rgb.length();
			for ( size_t i = 0; i < len; ++i )
			{
				if ( isdigit ( buf [ i ] ) )
				{
					if ( false == Begin )
					{
						Begin	=	true;
						IndS	=	i;
					}
				}
				else
				{
					if ( Begin )
					{
						if ( i - IndS >= 1 )
						{
							Color [ IndCol++ ]	=	std::stod ( Rgb.substr ( IndS, i - IndS ) );
						}
					}

					Begin	=	false;
				}

				if ( '%' == buf[i] )
				{
					PCT	=	true;
				}
			}

			if ( PCT )
			{
				Color [ 0 ] *= 255.0 * 0.01;
				Color [ 1 ] *= 255.0 * 0.01;
				Color [ 2 ] *= 255.0 * 0.01;
			}

			if (0 == Color[3])
				return -2;

			return ( ( (int) (Color [ 0 ]) ) | ( (int) (Color [ 1 ]) << 8 ) | (int) (Color [ 2 ]) << 16 );
		}

		long ColorFromHexString(const std::wstring& Hex) const
		{
			std::wstring value	=	Hex.substr (1);
			const wchar_t* buf = value.c_str();
			if ( 3 == value.length () )	//	for ex ~ #fb0 expands to #ffbb00
			{
				std::wstring tmp;
				tmp += buf[0];
				tmp += buf[0];
				tmp += buf[1];
				tmp += buf[1];
				tmp += buf[2];
				tmp += buf[2];
				value = tmp;
			}

			buf = value.c_str();
			size_t len = value.length();
			for ( size_t i = 0; i < len; ++i )
			{
				if ( isdigit ( buf [ i ] ) || (( buf [ i ] >= L'a' ) && ( buf [ i ] <= L'f' )) || (( buf [ i ] >= L'A' ) && ( buf [ i ] <= L'F' )) )
					continue;

				if (0 == i)
					return 0;
				long InvCol = std::stoi ( value.substr ( 0, i ), NULL, 16 );
				return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
			}

			long InvCol = std::stoi( value, NULL, 16 );
			return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
		}

	private:
		static ColorTable	m_oTable;
	};
}

#endif // SVGUTILS_H

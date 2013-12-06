#pragma once

#include <map>
#include <string>
#include <algorithm>
#include <math.h>

#include "Logic\Color.h"
#include "Logic\ColorsTable.h"

#include "Constants.h"
#include "OfficeArt\Common.h"
#include "OfficeArt\Enumerations.h"

#define DPI_DEFAULT		72.0

namespace DOCX
{
	class CPointF
	{
	public:

		CPointF () : m_X (0), m_Y (0), m_TX (0), m_TY (0), m_fCorrection (65536.0)
		{

		}

		CPointF (unsigned int dX, unsigned int dY) : m_X (dX), m_Y (dY), m_TX (0), m_TY (0), m_fCorrection (65536.0)
		{

		}

		CPointF (const std::string& value) : m_X (0), m_Y (0), m_TX (0), m_TY (0), m_fCorrection (65536.0)
		{
			if (0 == value.length())
				return;

			std::string strText	=	value;

			strText.erase(std::remove(strText.begin(), strText.end(), L' '), strText.end());

			double fT			=	1.0;		//	

			//	MM

			size_t from			=	strText.find("mm");
			if (string::npos != from)
			{
				strText.erase(std::remove(strText.begin(), strText.end(), L'm'), strText.end());

				m_fCorrection	=	36000;
				fT				=	(DPI_DEFAULT * 20.0) / 25.399931;
			}

			// PT

			from				=	strText.find("pt");
			if (string::npos != from)
			{
				strText.erase(std::remove(strText.begin(), strText.end(), L'p'), strText.end());
				strText.erase(std::remove(strText.begin(), strText.end(), L't'), strText.end());

				m_fCorrection	=	12700.0;
				fT				=	20.0;
			}

			// DEF

			from				=	strText.find(',');
			if (string::npos != from)
			{
				if (0 == from)
				{
					if (1 == (int)strText.length())
						return;			

					double dX	=	atof (strText.c_str());

					strText		=	strText.substr(from + 1, ((int)strText.length() - 1) - from);
					m_Y			=	(unsigned int)(dX * m_fCorrection);

					m_TX		=	(int)(dX * fT);

					return;
				}

				double dX		=	atof (strText.substr(0, from).c_str());

				m_X				=	FormatNum (dX, m_fCorrection);
				m_TX			=	(int)(dX * fT);

				strText			=	strText.substr(from + 1, ((int)strText.length() - 1) - from);
				if (0 == (int)strText.length())
					return;

				double dY		=	atof (strText.c_str());
				m_Y				=	FormatNum (strText, m_fCorrection);
				m_TY			=	(int)(dY * fT);

				return;
			}

			if (strText.length())
			{
				double dX		=	atof (strText.c_str());
				m_X				=	(unsigned int)(dX * m_fCorrection);
				m_TX			=	(int)(dX * fT);
			}
		}

		inline unsigned int X () const 
		{
			return m_X;
		}

		inline unsigned int Y () const 
		{
			return m_Y;
		}

		inline int GetTX() const
		{
			return	m_TX;
		}

		inline int GetTY() const
		{
			return	m_TY;
		}

	private:

		inline unsigned int ToFixed(double dNumber) const
		{
			if (dNumber < 0.0)
			{		
				return (unsigned int)(((long)dNumber - dNumber) * 65536.0) | (0xffff << 16);
			}	

			if (dNumber > 0.0)
			{
				return (unsigned int)((dNumber - (long)dNumber) * 65536.0);
			}

			return 0;
		}

		inline unsigned int FormatNum (const std::string& strVal, double dMul) const
		{
			return FormatNum (atof (strVal.c_str()), dMul);
		}

		inline unsigned int FormatNum (double dVal, double dMul) const
		{
			dVal	=	dVal * dMul;
			if (dVal > 0.0)
				dVal	+=	0.5;
			if (dVal < 0.0)
				dVal	-=	0.5;

			return (unsigned int)dVal;
		}

	private:

		unsigned int	m_X;
		unsigned int	m_Y;

		double			m_fCorrection;

		int				m_TX;
		int				m_TY;
	};

	class CFPoint
	{
	public:

		CFPoint () : m_dX(0.0), m_dY(0.0)
		{

		}

		CFPoint (const std::string& value, double dX, double dY) : m_dX(dX), m_dY(dY)
		{
			if (value.length())
			{
				std::string strText	= value;
				strText.erase(std::remove(strText.begin(), strText.end(), L' '), strText.end());
				if (strText.length())
				{
					size_t find = strText.find(',');
					if (string::npos != find)
					{
						m_dX = atof ((strText.substr(0, find)).c_str());

						strText	= strText.substr(find + 1, ((int)strText.length() - 1) - find);

						if (strText.length())
							m_dY = atof (strText.c_str());
					}
					else
					{
						m_dX = atof (strText.c_str());
					}
				}
			}
		}

		inline double X()
		{
			return m_dX;
		}

		inline double Y()
		{
			return m_dY;
		}

	private:

		double m_dX;
		double m_dY;
	};

	class CMatrixF
	{
	public:

		CMatrixF () : m_XToX (0), m_XToY(0), m_YToX(0), m_YToY(0), m_PX(0), m_PY(0)
		{		  

		}	

		CMatrixF (const std::string& str) : m_XToX (0), m_XToY(0), m_YToX(0), m_YToY(0), m_PX(0), m_PY(0)
		{		  
			if (0 == str.length())
				return;

			std::string strText		=	str;
			strText.erase(std::remove(strText.begin(), strText.end(), L' '), strText.end());

			//

			size_t from				=	strText.find(',');
			if (string::npos == from)
				return;

			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				if (string::npos != number.find('f'))
					m_XToX			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_XToX			=	ToFixed (atof (number.c_str()) );

				if (string::npos == from)
					return;
			}

			strText					=	strText.substr(from + 1, strText.length() - 1 - from);

			//

			from					=	strText.find(',');
			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				if (string::npos != number.find('f'))
					m_YToX			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_YToX			=	ToFixed (atof (number.c_str()) );

				if (string::npos == from)
					return;
			}

			strText					=	strText.substr(from + 1, strText.length() - 1 - from);

			//

			from					=	strText.find(',');
			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				if (string::npos != number.find('f'))
					m_XToY			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_XToY			=	ToFixed (atof (number.c_str()));

				if (string::npos == from)
					return;
			}

			strText					=	strText.substr(from + 1, strText.length() - 1 - from);

			//

			from					=	strText.find(',');
			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				if (string::npos != number.find('f'))
					m_YToY			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_YToY			=	ToFixed (atof (number.c_str()));

				if (string::npos == from)
					return;
			}

			strText					=	strText.substr(from + 1, strText.length() - 1 - from);

			//

			from					=	strText.find(',');
			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				if (string::npos != number.find('f'))
					m_PX			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_PX			=	ToPerspective (atof (number.c_str()));

				if (string::npos == from)
					return;
			}

			strText					=	strText.substr(from + 1, strText.length() - 1 - from);

			//

			from					=	strText.find(',');
			if (0 != from)
			{
				std::string number	=	strText.substr(0, from);

				double df			=	atof (number.c_str());

				if (string::npos != number.find('f'))
					m_PY			=	atoi (number.substr(0,number.length()-1).c_str());
				else
					m_PY			=	ToPerspective (atof (number.c_str()));
			}
		}

		inline unsigned int XToX () 
		{
			return m_XToX;
		}

		inline unsigned int XToY () 
		{
			return m_XToY;
		}

		inline unsigned int YToX () 
		{
			return m_YToX;
		}

		inline unsigned int YToY () 
		{
			return m_YToY;
		}

		inline unsigned int PX () 
		{
			return m_PX;
		}

		inline unsigned int PY () 
		{
			return m_PY;
		}

	private:

		unsigned int ToFixed(double dNumber)
		{
			if (dNumber < 0.0)
			{		
				return (unsigned int)(((long)dNumber - dNumber) * 65536.0) | (0xffff << 16);
			}	

			if (dNumber > 0.0)
			{
				//return (unsigned int)((dNumber - (long)dNumber) * 65536.0);
				return (unsigned int)(dNumber  * 65536.0);
			}

			return 0;
		}

		unsigned int ToPerspective(double dNumber)
		{
			static const double WEIGHT	=	256.0;		//	если сохраням матрицу как это делает Office 2010 (через свойства Shadow), то вес по умолчанию 0x00000100

			if (dNumber < 0.0)
			{		
				return 0xffffffff - (unsigned int)((-1.0 * dNumber * WEIGHT * 65536.0));
			}	

			if (dNumber > 0.0)
			{
				return (unsigned int)(dNumber * WEIGHT * 65536.0);
			}

			return 0;
		}

	private:		 

		unsigned m_XToX;
		unsigned m_XToY;
		unsigned m_YToX;
		unsigned m_YToY;
		unsigned m_PX;
		unsigned m_PY;
	};

	class CEmu
	{
	public:
		CEmu() : m_nVal(0)
		{

		}

		CEmu(const std::string& str) : m_nVal(0)
		{
			if (0 == str.length())
				return;

			std::string strText	=	str;

			int nCorrection	=	1;

			strText.erase(std::remove(strText.begin(), strText.end(), L' '), strText.end());

			size_t from			=	strText.find("mm");
			if (string::npos != from)
			{
				strText.erase(std::remove(strText.begin(), strText.end(), L'm'), strText.end());
				nCorrection		=	36000;
			}

			from				=	strText.find("pt");
			if (string::npos != from)
			{
				strText.erase(std::remove(strText.begin(), strText.end(), L'p'), strText.end());
				strText.erase(std::remove(strText.begin(), strText.end(), L't'), strText.end());

				nCorrection		=	12700;
			}

			m_nVal				=	atoi(str.c_str()) * nCorrection;
		}

		inline int Get()
		{
			return m_nVal;
		}

	private:

		int	m_nVal;
	};
}

namespace DOCXDOCUTILS
{
	inline unsigned char ColorToIco (const OOX::Logic::Color& oColor)
	{
		OOX::Logic::ColorsTable colorsTable;

		std::string colorName	=	colorsTable.fromColor(oColor);
		if (std::string("auto") == colorName)
			colorName			=	std::string ("000000");

		std::map<string, byte> colorsMap;
		colorsMap.insert( pair<string, byte>( "auto", 0x00 ) );
		colorsMap.insert( pair<string, byte>( "black", 0x01 ) );
		colorsMap.insert( pair<string, byte>( "blue", 0x02 ) );
		colorsMap.insert( pair<string, byte>( "cyan", 0x03 ) );
		colorsMap.insert( pair<string, byte>( "green", 0x04 ) );
		colorsMap.insert( pair<string, byte>( "magenta", 0x05 ) );
		colorsMap.insert( pair<string, byte>( "red", 0x06 ) );
		colorsMap.insert( pair<string, byte>( "yellow", 0x07 ) );
		colorsMap.insert( pair<string, byte>( "white", 0x08 ) );
		colorsMap.insert( pair<string, byte>( "darkBlue", 0x09 ) );
		colorsMap.insert( pair<string, byte>( "darkCyan", 0x0A ) );
		colorsMap.insert( pair<string, byte>( "darkGreen", 0x0B ) );
		colorsMap.insert( pair<string, byte>( "darkMagenta", 0x0C ) );
		colorsMap.insert( pair<string, byte>( "darkRed", 0x0D ) );
		colorsMap.insert( pair<string, byte>( "darkYellow", 0x0E ) );
		colorsMap.insert( pair<string, byte>( "darkGray", 0x0F ) );
		colorsMap.insert( pair<string, byte>( "lightGray", 0x10 ) );

		return colorsMap[colorName];
	}

	inline unsigned char AlignFromString (const std::wstring& align)
	{
		//!!!TODO!!!
		if (align == std::wstring(L"left"))
			return 0;
		else if (align == std::wstring(L"center"))
			return 1;
		else if (align == std::wstring(L"right"))
			return 2;
		else if (align == std::wstring(L"both"))
			return 3;

		return 0;
	}

	inline short StiToIstd (short nDef)
	{
		if (nDef >= 0 && nDef <= 9)
			return nDef;

		if (65 == nDef)
			return 10;
		if (105 == nDef)
			return 11;
		if (107 == nDef)
			return 12;

		return -1;
	}
	inline static std::vector<int> GetValues(const std::string& strValues)
	{
		std::vector<int> values;
		if (strValues.empty())
			return values;

		int number = 0;
		std::string str = strValues;
		while(str.length())
		{
			size_t separator = str.find (',');
			if (string::npos != separator)
			{			
				std::string snumber = str.substr(0, separator);
				if (string::npos != snumber.find('f'))
					number = (int) (21600.0 * (atof (snumber.substr(0,snumber.length()-1).c_str()) / 65536.0));		//	????
				else
					number = atoi (snumber.c_str());

				values.push_back (number);
				str	= str.substr(separator + 1, str.length() - separator - 1);
			}
			else
			{
				if (string::npos != str.find('f'))
					number = atoi (str.substr(0,str.length()-1).c_str());
				else
					number = atoi (str.c_str());

				values.push_back (number);

				break;
			}
		}

		return values;
	}
}

namespace DOCXDOCUTILS
{
	inline unsigned int HexString2UInt (const std::string& value)
	{
		unsigned int summa = 0;
		for (int i = 0; i != value.size(); ++i)
			summa += HexChar2Int(value[i]) << (4 * (value.size() - i - 1));
		return summa;
	}

	inline unsigned int HexString2UInt2X (const std::string& value)
	{
		unsigned int summa = 0;
		for (int i = 0; i != value.size(); ++i)
		{
			summa += HexChar2Int(value[i]) << (4 * (value.size() - i - 1));
		}

		return summa;
	}

	inline unsigned int HexChar2Int (const char value)
	{
		if (value >= '0' && value <= '9')
			return value - '0';
		if (value >= 'a' && value <= 'f')
			return 10 + value - 'a';
		if (value >= 'A' && value <= 'F')
			return 10 + value - 'A';
		return 0;
	}
	inline unsigned int RGBToBGR (const unsigned int& nColor)
	{
		return ((nColor>>16) & 0xFF) | (0xFF00 & nColor) | (0xFF0000 & (nColor<<16));
	}

	inline BOOL GetFillColor(const std::string& value, unsigned int& nColor)
	{
		nColor	=	0;

		if (value.length() >= 6)
		{
			int from = value.find(' ');
			if (-1 == from)
			{
				if ('#' == value[0])
				{
					nColor = RGBToBGR(HexString2UInt(value.substr(1,value.length()-1)));
					return TRUE;
				}

				nColor = RGBToBGR(HexString2UInt(value));
				return TRUE;
			}
			else
			{
				if ('#' == value[0])
				{
					nColor = RGBToBGR(HexString2UInt(value.substr(1,from-1)));
					return TRUE;
				}

				nColor = RGBToBGR(HexString2UInt(value));
				return TRUE;
			}
		}
		else
		{
			if ('#' == value[0] && 4 == value.length())		//	#cfc -> CCFFCC
			{
				nColor	=	0;

				nColor	+=	HexChar2Int(value[1]);
				nColor	+=	HexChar2Int(value[1]) << 4;

				nColor	+=	HexChar2Int(value[2]) << 8;
				nColor	+=	HexChar2Int(value[2]) << 12;

				nColor	+=	HexChar2Int(value[3]) << 16;
				nColor	+=	HexChar2Int(value[3]) << 20;

				return TRUE;
			}
		}

		return FALSE;
	}
	inline unsigned int GetFixePointValue (const std::string& value)
	{
		if (value.length() > 1)
		{
			std::string strText = value;

			if ('f' == strText[strText.length()-1] )
			{
				std::string number	=	strText.substr(0,strText.length()-1);
				if (number.length ())
					return atoi (number.c_str());
			}
			else
			{			
				return (unsigned int)(atof (strText.c_str()) * 65536.0);
			}
		}

		return 0;
	}

	inline static int GetArcValue(const std::string& strValue)
	{
		if (0==strValue.length())
			return 0;

		if (string::npos != strValue.find('f'))
		{
			return (int) (21600.0 * (atof (strValue.substr(0,strValue.length()-1).c_str()) / 65536.0));		//	????
		}
		else
		{
			return (int)(21600.0 * atof (strValue.c_str()));
		}

		return 0;
	}
	inline static unsigned int ToFixed2(double dNumber)
	{
		if (dNumber < 0.0)
		{		
			return (unsigned int)(((long)dNumber - dNumber) * 65536.0) | (0xffff << 16);
		}	

		if (dNumber > 0.0)
		{
			return (unsigned int)((dNumber - (long)dNumber) * 65536.0);
		}

		return 0;
	}

	inline static double GetAngleValue (const std::string& strValue)
	{
		size_t from		=	strValue.find("fd");
		if (string::npos != from)
		{
			std::string strText		=	strValue;

			strText.erase(std::remove(strText.begin(), strText.end(), L'f'), strText.end());
			strText.erase(std::remove(strText.begin(), strText.end(), L'd'), strText.end());

			return atof(strText.c_str()) / 65536.0;
		}

		return atof(strValue.c_str());
	}
}

namespace DOCXDOCUTILS	//	fonts
{	
	inline ASCDocFileFormat::Constants::FontFamilyType FontFamilyFromString (const std::wstring& sFontFamily)
	{
		if (sFontFamily == std::wstring(L"auto"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeUnspecified;
		}
		else if (sFontFamily == std::wstring(L"roman"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeRomanSerif;
		}
		else if (sFontFamily == std::wstring(L"swiss"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeSwissSansSerif;
		}
		else if (sFontFamily == std::wstring(L"modern"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeModernMonospace;
		}
		else if (sFontFamily == std::wstring(L"script"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeScriptCursive;
		}
		else if (sFontFamily == std::wstring(L"decorative"))
		{
			return ASCDocFileFormat::Constants::fontFamilyTypeDecorativeFantasy;
		}

		return ASCDocFileFormat::Constants::fontFamilyTypeUnspecified;
	}

	inline ASCDocFileFormat::Constants::CharacterPitch FontPitchFromString (const std::wstring& sPitch)
	{
		if (sPitch == std::wstring(L"variable"))
			return ASCDocFileFormat::Constants::characterPitchVariable;
		else if (sPitch == std::wstring(L"fixed"))
			return ASCDocFileFormat::Constants::characterPitchFixed;

		return ASCDocFileFormat::Constants::characterPitchDefault;
	}

	inline unsigned char FontCharsetFromString (const std::wstring& Charset)
	{
		if (Charset == std::wstring(L"CC"))
			return RUSSIAN_CHARSET;
		else if (Charset == std::wstring(L"00"))
			return ANSI_CHARSET;
		else if (Charset == std::wstring(L"02"))
			return SYMBOL_CHARSET;
		else if (Charset == std::wstring(L"80"))
			return SHIFTJIS_CHARSET;
		else if (Charset == std::wstring(L"86"))
			return GB2312_CHARSET;

		return DEFAULT_CHARSET;  
	}
}

namespace DOCXDOCUTILS
{
	inline unsigned short GetStyleWrapType (const std::string& type) 
	{
		if (type == std::string("none"))
			return 3;
		if (type == std::string("topAndBottom"))
			return 1;
		if (type == std::string("square"))
			return 2;
		if (type == std::string("tight"))
			return 4;
		if (type == std::string("through"))
			return 5;

		return 0;
	}

}

namespace DOCXDOCUTILS
{
	inline bool GetFlipH (const std::string& strFlip)
	{
		if (strFlip.length())
		{
			if ((string::npos != strFlip.find('x')) || (string::npos != strFlip.find('1')))
				return true;
		}

		return false;
	}

	inline bool GetFlipV (const std::string& strFlip)
	{
		if (strFlip.length())
		{
			if ((string::npos != strFlip.find('y')) || (string::npos != strFlip.find('1')))
				return true;
		}

		return false;
	}
}

namespace DOCXDOCUTILS
{
	inline std::wstring GetInstrText_FieldCode(std::wstring InstrText, std::wstring& Source)
	{
		if (0 == InstrText.length())
			return std::wstring(L"");

		std::wstring instrText = InstrText;

		instrText.erase(std::remove(instrText.begin(), instrText.end(), L' '), instrText.end());

		if (L'H' == instrText[0])
		{
			std::wstring::size_type pos	= InstrText.find(L"HYPERLINK");

			if (std::wstring::npos != pos)
			{
				Source	=	InstrText.substr (pos + std::wstring(L"HYPERLINK").length(),InstrText.length() - pos + std::wstring(L"HYPERLINK").length() - 1);
				return std::wstring(L"HYPERLINK");
			}
		}

		if (L'P' == instrText[0])
		{
			if (std::wstring::npos != instrText.find(L"PAGEREF"))
				return std::wstring(L"PAGEREF");

			if (std::wstring::npos != instrText.find(L"PAGE"))
				return std::wstring(L"PAGE");
		}

		if (L'T' == instrText[0])
		{
			if (std::wstring::npos != instrText.find(L"TOC"))
				return std::wstring(L"TOC");
		}

		if (L'A' == instrText[0])
		{
			if (std::wstring::npos != instrText.find(L"ADDRESSBLOCK"))
				return std::wstring(L"ADDRESSBLOCK");
		}

		if (L'G' == instrText[0])
		{
			if (std::wstring::npos != instrText.find(L"GREETINGLINE"))
				return std::wstring(L"GREETINGLINE");
		}

		if (L'M' == instrText[0])
		{
			if (std::wstring::npos != instrText.find(L"MERGEFIELD"))
				return std::wstring(L"MERGEFIELD");
		}

		return std::wstring(L"");
	}	
}

namespace DOCX
{
	class CSColor
	{
	public:

		CSColor () :  m_nColor (0)
		{

		}

		inline BOOL Init (const std::string& Color)
		{
			if (Color.length() <= 0)
				return FALSE;

			CString color	=	CString(Color.c_str());
			wchar_t wChar	=	color.GetAt(0);
			switch (wChar)
			{
			case L'a':
				if      ( color.Find( _T("aliceBlue")			) >= 0) { m_nColor = RGB2(240,248,255);	return TRUE; } 
				else if ( color.Find( _T("antiqueWhite")		) >= 0) { m_nColor = RGB2(250,235,215);	return TRUE; } 
				else if ( color.Find( _T("aqua")				) >= 0) { m_nColor = RGB2(0,255,255);	return TRUE; } 
				else if ( color.Find( _T("aquamarine")			) >= 0) { m_nColor = RGB2(127,255,212);	return TRUE; } 
				else if ( color.Find( _T("azure")				) >= 0) { m_nColor = RGB2(240,255,255);	return TRUE; } 
				break;
			case L'b':
				if      ( color.Find( _T("beige")				) >= 0) { m_nColor = RGB2(245,245,220);	return TRUE; } 
				else if ( color.Find( _T("bisque")				) >= 0) { m_nColor = RGB2(255,228,196);	return TRUE; } 
				else if ( color.Find( _T("black")				) >= 0) { m_nColor = RGB2(0,0,0);		return TRUE; } 
				else if ( color.Find( _T("blanchedAlmond")		) >= 0) { m_nColor = RGB2(255,235,205);	return TRUE; } 
				else if ( color.Find( _T("blue")				) >= 0) { m_nColor = RGB2(0,0,255);		return TRUE; } 
				else if ( color.Find( _T("blueViolet")			) >= 0) { m_nColor = RGB2(138,43,226);	return TRUE; } 
				else if ( color.Find( _T("brown")				) >= 0) { m_nColor = RGB2(165,42,42);	return TRUE; } 
				else if ( color.Find( _T("burlyWood")			) >= 0) { m_nColor = RGB2(222,184,135);	return TRUE; } 
				break;
			case L'c':
				if      ( color.Find( _T("cadetBlue")			) >= 0) { m_nColor = RGB2(95,158,160);	return TRUE; } 
				else if ( color.Find( _T("chartreuse")			) >= 0) { m_nColor = RGB2(127,255,0);	return TRUE; } 
				else if ( color.Find( _T("chocolate")			) >= 0) { m_nColor = RGB2(210,105,30);	return TRUE; } 
				else if ( color.Find( _T("coral")				) >= 0) { m_nColor = RGB2(255,127,80);	return TRUE; } 
				else if ( color.Find( _T("cornflowerBlue")		) >= 0) { m_nColor = RGB2(100,149,237);	return TRUE; } 
				else if ( color.Find( _T("cornsilk")			) >= 0) { m_nColor = RGB2(255,248,220);	return TRUE; } 
				else if ( color.Find( _T("crimson")				) >= 0) { m_nColor = RGB2(220,20,60);	return TRUE; } 
				else if ( color.Find( _T("cyan")				) >= 0) { m_nColor = RGB2(0,255,255);	return TRUE; } 
				break;
			case L'd':
				if      ( color.Find( _T("darkBlue")			) >= 0) { m_nColor = RGB2(0,0,139);		return TRUE; } 
				else if ( color.Find( _T("darkCyan")			) >= 0) { m_nColor = RGB2(0,139,139);	return TRUE; } 
				else if ( color.Find( _T("darkGoldenrod")		) >= 0) { m_nColor = RGB2(184,134,11);	return TRUE; } 
				else if ( color.Find( _T("darkGray")			) >= 0) { m_nColor = RGB2(169,169,169);	return TRUE; } 
				else if ( color.Find( _T("darkGreen")			) >= 0) { m_nColor = RGB2(0,100,0);		return TRUE; } 
				else if ( color.Find( _T("darkGrey")			) >= 0) { m_nColor = RGB2(169,169,169);	return TRUE; } 
				else if ( color.Find( _T("darkKhaki")			) >= 0) { m_nColor = RGB2(189,183,107);	return TRUE; } 
				else if ( color.Find( _T("darkMagenta")			) >= 0) { m_nColor = RGB2(139,0,139);	return TRUE; } 
				else if ( color.Find( _T("darkOliveGreen")		) >= 0) { m_nColor = RGB2(85,107,47);	return TRUE; } 
				else if ( color.Find( _T("darkOrange")			) >= 0) { m_nColor = RGB2(255,140,0);	return TRUE; } 
				else if ( color.Find( _T("darkOrchid")			) >= 0) { m_nColor = RGB2(153,50,204);	return TRUE; } 
				else if ( color.Find( _T("darkRed")				) >= 0) { m_nColor = RGB2(139,0,0);		return TRUE; } 
				else if ( color.Find( _T("darkSalmon")			) >= 0) { m_nColor = RGB2(233,150,122);	return TRUE; } 
				else if ( color.Find( _T("darkSeaGreen")		) >= 0) { m_nColor = RGB2(143,188,143);	return TRUE; } 
				else if ( color.Find( _T("darkSlateBlue")		) >= 0) { m_nColor = RGB2(72,61,139);	return TRUE; } 
				else if ( color.Find( _T("darkSlateGray")		) >= 0) { m_nColor = RGB2(47,79,79);	return TRUE; } 
				else if ( color.Find( _T("darkSlateGrey")		) >= 0) { m_nColor = RGB2(47,79,79);	return TRUE; } 
				else if ( color.Find( _T("darkTurquoise")		) >= 0) { m_nColor = RGB2(0,206,209);	return TRUE; } 
				else if ( color.Find( _T("darkViolet")			) >= 0) { m_nColor = RGB2(148,0,211);	return TRUE; } 
				else if ( color.Find( _T("deepPink")			) >= 0) { m_nColor = RGB2(255,20,147);	return TRUE; } 
				else if ( color.Find( _T("deepSkyBlue")			) >= 0) { m_nColor = RGB2(0,191,255);	return TRUE; } 
				else if ( color.Find( _T("dimGray")				) >= 0) { m_nColor = RGB2(105,105,105);	return TRUE; } 
				else if ( color.Find( _T("dimGrey")				) >= 0) { m_nColor = RGB2(105,105,105);	return TRUE; } 
				else if ( color.Find( _T("dkBlue")				) >= 0) { m_nColor = RGB2(0,0,139);		return TRUE; } 
				else if ( color.Find( _T("dkCyan")				) >= 0) { m_nColor = RGB2(0,139,139);	return TRUE; } 
				else if ( color.Find( _T("dkGoldenrod")			) >= 0) { m_nColor = RGB2(184,134,11);	return TRUE; } 
				else if ( color.Find( _T("dkGray")				) >= 0) { m_nColor = RGB2(169,169,169);	return TRUE; } 
				else if ( color.Find( _T("dkGreen")				) >= 0) { m_nColor = RGB2(0,100,0);		return TRUE; } 
				else if ( color.Find( _T("dkGrey")				) >= 0) { m_nColor = RGB2(169,169,169);	return TRUE; } 
				else if ( color.Find( _T("dkKhaki")				) >= 0) { m_nColor = RGB2(189,183,107);	return TRUE; } 
				else if ( color.Find( _T("dkMagenta")			) >= 0) { m_nColor = RGB2(139,0,139);	return TRUE; } 
				else if ( color.Find( _T("dkOliveGreen")		) >= 0) { m_nColor = RGB2(85,107,47);	return TRUE; } 
				else if ( color.Find( _T("dkOrange")			) >= 0) { m_nColor = RGB2(255,140,0);	return TRUE; } 
				else if ( color.Find( _T("dkOrchid")			) >= 0) { m_nColor = RGB2(153,50,204);	return TRUE; } 
				else if ( color.Find( _T("dkRed")				) >= 0) { m_nColor = RGB2(139,0,0);		return TRUE; } 
				else if ( color.Find( _T("dkSalmon")			) >= 0) { m_nColor = RGB2(233,150,122);	return TRUE; } 
				else if ( color.Find( _T("dkSeaGreen")			) >= 0) { m_nColor = RGB2(143,188,139);	return TRUE; } 
				else if ( color.Find( _T("dkSlateBlue")			) >= 0) { m_nColor = RGB2(72,61,139);	return TRUE; } 
				else if ( color.Find( _T("dkSlateGray")			) >= 0) { m_nColor = RGB2(47,79,79);	return TRUE; } 
				else if ( color.Find( _T("dkSlateGrey")			) >= 0) { m_nColor = RGB2(47,79,79);	return TRUE; } 
				else if ( color.Find( _T("dkTurquoise")			) >= 0) { m_nColor = RGB2(0,206,209);	return TRUE; } 
				else if ( color.Find( _T("dkViolet")			) >= 0) { m_nColor = RGB2(148,0,211);	return TRUE; } 
				else if ( color.Find( _T("dodgerBlue")			) >= 0) { m_nColor = RGB2(30,144,255);	return TRUE; } 
				break;
			case L'f':
				if      ( color.Find( _T("firebrick")			) >= 0) { m_nColor = RGB2(178,34,34);	return TRUE; } 
				else if ( color.Find( _T("floralWhite")			) >= 0) { m_nColor = RGB2(255,250,240);	return TRUE; } 
				else if ( color.Find( _T("forestGreen")			) >= 0) { m_nColor = RGB2(34,139,34);	return TRUE; } 
				else if ( color.Find( _T("fuchsia")				) >= 0) { m_nColor = RGB2(255,0,255);	return TRUE; } 
				break;	   
			case L'g':
				if      ( color.Find( _T("gainsboro")			) >= 0) { m_nColor = RGB2(220,220,220);	return TRUE; } 
				else if ( color.Find( _T("ghostWhite")			) >= 0) { m_nColor = RGB2(248,248,255);	return TRUE; } 
				else if ( color.Find( _T("gold")				) >= 0) { m_nColor = RGB2(255,215,0);	return TRUE; } 
				else if ( color.Find( _T("goldenrod")			) >= 0) { m_nColor = RGB2(218,165,32);	return TRUE; } 
				else if ( color.Find( _T("gray")				) >= 0) { m_nColor = RGB2(128,128,128);	return TRUE; } 
				else if ( color.Find( _T("green")				) >= 0) { m_nColor = RGB2(0,128,0);		return TRUE; } 
				else if ( color.Find( _T("greenYellow")			) >= 0) { m_nColor = RGB2(173,255,47);	return TRUE; } 
				else if ( color.Find( _T("grey")				) >= 0) { m_nColor = RGB2(128,128,128);	return TRUE; } 
				break;	   
			case L'h':	   
				if      ( color.Find( _T("honeydew")			) >= 0) { m_nColor = RGB2(240,255,240);	return TRUE; } 
				else if ( color.Find( _T("hotPink")				) >= 0) { m_nColor = RGB2(255,105,180);	return TRUE; } 
				break;
			case L'i':
				if      ( color.Find( _T("indianRed")			) >= 0) { m_nColor = RGB2(205,92,92);	return TRUE; } 
				else if ( color.Find( _T("indigo")				) >= 0) { m_nColor = RGB2(75,0,130);	return TRUE; } 
				else if ( color.Find( _T("ivory")				) >= 0) { m_nColor = RGB2(255,255,240);	return TRUE; } 
				break;
			case L'k':
				if      ( color.Find( _T("khaki")				) >= 0) { m_nColor = RGB2(240,230,140);	return TRUE; } 
				break;
			case L'l':
				if      ( color.Find( _T("lavender")			) >= 0) { m_nColor = RGB2(230,230,250);	return TRUE; } 
				else if ( color.Find( _T("lavenderBlush")		) >= 0) { m_nColor = RGB2(255,240,245);	return TRUE; } 
				else if ( color.Find( _T("lawnGreen")			) >= 0) { m_nColor = RGB2(124,252,0);	return TRUE; } 
				else if ( color.Find( _T("lemonChiffon")		) >= 0) { m_nColor = RGB2(255,250,205);	return TRUE; } 
				else if ( color.Find( _T("lightBlue")			) >= 0) { m_nColor = RGB2(173,216,230);	return TRUE; } 
				else if ( color.Find( _T("lightCoral")			) >= 0) { m_nColor = RGB2(240,128,128);	return TRUE; } 
				else if ( color.Find( _T("lightCyan")			) >= 0) { m_nColor = RGB2(224,255,255);	return TRUE; } 
				else if ( color.Find( _T("lightGoldenrodYellow")) >= 0) { m_nColor = RGB2(250,250,210);	return TRUE; } 
				else if ( color.Find( _T("lightGray")			) >= 0) { m_nColor = RGB2(211,211,211);	return TRUE; } 
				else if ( color.Find( _T("lightGreen")			) >= 0) { m_nColor = RGB2(144,238,144);	return TRUE; } 
				else if ( color.Find( _T("lightGrey")			) >= 0) { m_nColor = RGB2(211,211,211);	return TRUE; } 
				else if ( color.Find( _T("lightPink")			) >= 0) { m_nColor = RGB2(255,182,193);	return TRUE; } 
				else if ( color.Find( _T("lightSalmon")			) >= 0) { m_nColor = RGB2(255,160,122);	return TRUE; } 
				else if ( color.Find( _T("lightSeaGreen")		) >= 0) { m_nColor = RGB2(32,178,170);	return TRUE; } 
				else if ( color.Find( _T("lightSkyBlue")		) >= 0) { m_nColor = RGB2(135,206,250);	return TRUE; } 
				else if ( color.Find( _T("lightSlateGray")		) >= 0) { m_nColor = RGB2(119,136,153);	return TRUE; } 
				else if ( color.Find( _T("lightSlateGrey")		) >= 0) { m_nColor = RGB2(119,136,153);	return TRUE; } 
				else if ( color.Find( _T("lightSteelBlue")		) >= 0) { m_nColor = RGB2(176,196,222);	return TRUE; } 
				else if ( color.Find( _T("lightYellow")			) >= 0) { m_nColor = RGB2(255,255,224);	return TRUE; } 
				else if ( color.Find( _T("lime")				) >= 0) { m_nColor = RGB2(0,255,0);		return TRUE; } 
				else if ( color.Find( _T("limeGreen")			) >= 0) { m_nColor = RGB2(50,205,50);	return TRUE; } 
				else if ( color.Find( _T("linen")				) >= 0) { m_nColor = RGB2(250,240,230);	return TRUE; } 
				else if ( color.Find( _T("ltBlue")				) >= 0) { m_nColor = RGB2(173,216,230);	return TRUE; } 
				else if ( color.Find( _T("ltCoral")				) >= 0) { m_nColor = RGB2(240,128,128);	return TRUE; } 
				else if ( color.Find( _T("ltCyan")				) >= 0) { m_nColor = RGB2(224,255,255);	return TRUE; } 
				else if ( color.Find( _T("ltGoldenrodYellow")	) >= 0) { m_nColor = RGB2(250,250,120);	return TRUE; } 
				else if ( color.Find( _T("ltGray")				) >= 0) { m_nColor = RGB2(211,211,211);	return TRUE; } 
				else if ( color.Find( _T("ltGreen")				) >= 0) { m_nColor = RGB2(144,238,144);	return TRUE; } 
				else if ( color.Find( _T("ltGrey")				) >= 0) { m_nColor = RGB2(211,211,211);	return TRUE; } 
				else if ( color.Find( _T("ltPink")				) >= 0) { m_nColor = RGB2(255,182,193);	return TRUE; } 
				else if ( color.Find( _T("ltSalmon")			) >= 0) { m_nColor = RGB2(255,160,122);	return TRUE; } 
				else if ( color.Find( _T("ltSeaGreen")			) >= 0) { m_nColor = RGB2(32,178,170);	return TRUE; } 
				else if ( color.Find( _T("ltSkyBlue")			) >= 0) { m_nColor = RGB2(135,206,250);	return TRUE; } 
				else if ( color.Find( _T("ltSlateGray")			) >= 0) { m_nColor = RGB2(119,136,153);	return TRUE; } 
				else if ( color.Find( _T("ltSlateGrey")			) >= 0) { m_nColor = RGB2(119,136,153);	return TRUE; } 
				else if ( color.Find( _T("ltSteelBlue")			) >= 0) { m_nColor = RGB2(176,196,222);	return TRUE; } 
				else if ( color.Find( _T("ltYellow")			) >= 0) { m_nColor = RGB2(255,255,224);	return TRUE; } 
				break;
			case L'm':
				if      ( color.Find( _T("magenta")				) >= 0) { m_nColor = RGB2(255,0,255);	return TRUE; } 
				else if ( color.Find( _T("maroon")				) >= 0) { m_nColor = RGB2(128,0,0);		return TRUE; } 
				else if ( color.Find( _T("medAquamarine")		) >= 0) { m_nColor = RGB2(102,205,170);	return TRUE; } 
				else if ( color.Find( _T("medBlue")				) >= 0) { m_nColor = RGB2(0,0,205);		return TRUE; } 
				else if ( color.Find( _T("mediumAquamarine")	) >= 0) { m_nColor = RGB2(102,205,170);	return TRUE; } 
				else if ( color.Find( _T("mediumBlue")			) >= 0) { m_nColor = RGB2(0,0,205);		return TRUE; } 
				else if ( color.Find( _T("mediumOrchid")		) >= 0) { m_nColor = RGB2(186,85,211);	return TRUE; } 
				else if ( color.Find( _T("mediumPurple")		) >= 0) { m_nColor = RGB2(147,112,219);	return TRUE; } 
				else if ( color.Find( _T("mediumSeaGreen")		) >= 0) { m_nColor = RGB2(60,179,113);	return TRUE; } 
				else if ( color.Find( _T("mediumSlateBlue")		) >= 0) { m_nColor = RGB2(123,104,238);	return TRUE; } 
				else if ( color.Find( _T("mediumSpringGreen")	) >= 0) { m_nColor = RGB2(0,250,154);	return TRUE; } 
				else if ( color.Find( _T("mediumTurquoise")		) >= 0) { m_nColor = RGB2(72,209,204);	return TRUE; } 
				else if ( color.Find( _T("mediumVioletRed")		) >= 0) { m_nColor = RGB2(199,21,133);	return TRUE; } 
				else if ( color.Find( _T("medOrchid")			) >= 0) { m_nColor = RGB2(186,85,211);	return TRUE; } 
				else if ( color.Find( _T("medPurple")			) >= 0) { m_nColor = RGB2(147,112,219);	return TRUE; } 
				else if ( color.Find( _T("medSeaGreen")			) >= 0) { m_nColor = RGB2(60,179,113);	return TRUE; } 
				else if ( color.Find( _T("medSlateBlue")		) >= 0) { m_nColor = RGB2(123,104,238);	return TRUE; } 
				else if ( color.Find( _T("medSpringGreen")		) >= 0) { m_nColor = RGB2(0,250,154);	return TRUE; } 
				else if ( color.Find( _T("medTurquoise")		) >= 0) { m_nColor = RGB2(72,209,204);	return TRUE; } 
				else if ( color.Find( _T("medVioletRed")		) >= 0) { m_nColor = RGB2(199,21,133);	return TRUE; } 
				else if ( color.Find( _T("midnightBlue")		) >= 0) { m_nColor = RGB2(25,25,112);	return TRUE; } 
				else if ( color.Find( _T("mintCream")			) >= 0) { m_nColor = RGB2(245,255,250);	return TRUE; } 
				else if ( color.Find( _T("mistyRose")			) >= 0) { m_nColor = RGB2(255,228,225);	return TRUE; } 
				else if ( color.Find( _T("moccasin")			) >= 0) { m_nColor = RGB2(255,228,181);	return TRUE; } 
				break;	  
			case L'n':
				if      ( color.Find( _T("navajoWhite")			) >= 0) { m_nColor = RGB2(255,222,173);	return TRUE; } 
				else if ( color.Find( _T("navy")				) >= 0) { m_nColor = RGB2(0,0,128);		return TRUE; } 
				break;
			case L'o':
				if      ( color.Find( _T("oldLace")				) >= 0) { m_nColor = RGB2(253,245,230);	return TRUE; } 
				else if ( color.Find( _T("olive")				) >= 0) { m_nColor = RGB2(128,128,0);	return TRUE; } 
				else if ( color.Find( _T("oliveDrab")			) >= 0) { m_nColor = RGB2(107,142,35);	return TRUE; } 
				else if ( color.Find( _T("orange")				) >= 0) { m_nColor = RGB2(255,165,0);	return TRUE; } 
				else if ( color.Find( _T("orangeRed")			) >= 0) { m_nColor = RGB2(255,69,0);	return TRUE; } 
				else if ( color.Find( _T("orchid")				) >= 0) { m_nColor = RGB2(218,112,214);	return TRUE; } 
				break;	 
			case L'p':	  
				if      ( color.Find( _T("paleGoldenrod")		) >= 0) { m_nColor = RGB2(238,232,170);	return TRUE; } 
				else if ( color.Find( _T("paleGreen")			) >= 0) { m_nColor = RGB2(152,251,152);	return TRUE; } 
				else if ( color.Find( _T("paleTurquoise")		) >= 0) { m_nColor = RGB2(175,238,238);	return TRUE; } 
				else if ( color.Find( _T("paleVioletRed")		) >= 0) { m_nColor = RGB2(219,112,147);	return TRUE; } 
				else if ( color.Find( _T("papayaWhip")			) >= 0) { m_nColor = RGB2(255,239,213);	return TRUE; } 
				else if ( color.Find( _T("peachPuff")			) >= 0) { m_nColor = RGB2(255,218,185);	return TRUE; } 
				else if ( color.Find( _T("peru")				) >= 0) { m_nColor = RGB2(205,133,63);	return TRUE; } 
				else if ( color.Find( _T("pink")				) >= 0) { m_nColor = RGB2(255,192,203);	return TRUE; } 
				else if ( color.Find( _T("plum")				) >= 0) { m_nColor = RGB2(221,160,221);	return TRUE; } 
				else if ( color.Find( _T("powderBlue")			) >= 0) { m_nColor = RGB2(176,224,230);	return TRUE; } 
				else if ( color.Find( _T("purple")				) >= 0) { m_nColor = RGB2(128,0,128);	return TRUE; } 
				break;	   
			case L'r':
				if      ( color.Find( _T("red")					) >= 0) { m_nColor = RGB2(255,0,0);		return TRUE; } 
				else if ( color.Find( _T("rosyBrown")			) >= 0) { m_nColor = RGB2(188,143,143);	return TRUE; } 
				else if ( color.Find( _T("royalBlue")			) >= 0) { m_nColor = RGB2(65,105,225);	return TRUE; } 
				break;	   
			case L's':	  
				if      ( color.Find( _T("saddleBrown")			) >= 0) { m_nColor = RGB2(139,69,19);	return TRUE; } 
				else if ( color.Find( _T("salmon")				) >= 0) { m_nColor = RGB2(250,128,114);	return TRUE; } 
				else if ( color.Find( _T("sandyBrown")			) >= 0) { m_nColor = RGB2(244,164,96);	return TRUE; } 
				else if ( color.Find( _T("seaGreen")			) >= 0) { m_nColor = RGB2(46,139,87);	return TRUE; } 
				else if ( color.Find( _T("seaShell")			) >= 0) { m_nColor = RGB2(255,245,238);	return TRUE; } 
				else if ( color.Find( _T("sienna")				) >= 0) { m_nColor = RGB2(160,82,45);	return TRUE; } 
				else if ( color.Find( _T("silver")				) >= 0) { m_nColor = RGB2(192,192,192);	return TRUE; } 
				else if ( color.Find( _T("skyBlue")				) >= 0) { m_nColor = RGB2(135,206,235);	return TRUE; } 
				else if ( color.Find( _T("slateBlue")			) >= 0) { m_nColor = RGB2(106,90,205);	return TRUE; } 
				else if ( color.Find( _T("slateGray")			) >= 0) { m_nColor = RGB2(112,128,144);	return TRUE; } 
				else if ( color.Find( _T("slateGrey")			) >= 0) { m_nColor = RGB2(112,128,144);	return TRUE; } 
				else if ( color.Find( _T("snow")				) >= 0) { m_nColor = RGB2(255,250,250);	return TRUE; } 
				else if ( color.Find( _T("springGreen")			) >= 0) { m_nColor = RGB2(0,255,127);	return TRUE; } 
				else if ( color.Find( _T("steelBlue")			) >= 0) { m_nColor = RGB2(70,130,180);	return TRUE; } 
				break;	  
			case L't':
				if      ( color.Find( _T("tan")					) >= 0) { m_nColor = RGB2(210,180,140);	return TRUE; } 
				else if ( color.Find( _T("teal")				) >= 0) { m_nColor = RGB2(0,128,128);	return TRUE; } 
				else if ( color.Find( _T("thistle")				) >= 0) { m_nColor = RGB2(216,191,216);	return TRUE; } 
				else if ( color.Find( _T("tomato")				) >= 0) { m_nColor = RGB2(255,99,71);	return TRUE; } 
				else if ( color.Find( _T("turquoise")			) >= 0) { m_nColor = RGB2(64,224,208);	return TRUE; } 
				break;
			case L'v':
				if      ( color.Find( _T("violet")				) >= 0) { m_nColor = RGB2(238,130,238);	return TRUE; } 
				break;
			case L'w':
				if      ( color.Find( _T("wheat")				) >= 0)	{ m_nColor = RGB2(245,222,179);	return TRUE; }
				else if ( color.Find( _T("white")				) >= 0)	{ m_nColor = RGB2(255,255,255);	return TRUE; }
				else if ( color.Find( _T("whiteSmoke")			) >= 0)	{ m_nColor = RGB2(245,245,245);	return TRUE; }
				break;	  
			case L'y':	  
				if      ( color.Find( _T("yellow")				) >= 0)	{ m_nColor = RGB2(255,255,0);	return TRUE; }  
				else if ( color.Find( _T("yellowGreen")			) >= 0)	{ m_nColor = RGB2(154,205,50);	return TRUE; } 
				break;
			}

			if (DOCXDOCUTILS::GetFillColor (Color, m_nColor))
				return TRUE;

			return FALSE;
		}

		inline int Color() 
		{
			return m_nColor;
		}

	private:

		inline unsigned int RGB2 (unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xF)	// TODO
		{
			return ( (b<<16) | (g<<8) | (r) );
		}

	private:

		unsigned int m_nColor;
	};
}

#ifdef _DEBUG		

namespace DOCXDOCUTILS
{
	inline void DebugStrPrint (std::wstring strMessage, const std::string& strSource)
	{
		OutputDebugStringW(strMessage.c_str());
		OutputDebugStringA(strSource.c_str());
		OutputDebugStringW(L"\n");
	}

	inline void DebugStrPrint (std::wstring strMessage, const std::wstring& strSource)
	{
		OutputDebugStringW(strMessage.c_str());
		OutputDebugStringW(strSource.c_str());
		OutputDebugStringW(L"\n");
	}
}

#endif

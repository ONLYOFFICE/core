/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "SimpleTypes_Base.h"
#include "SimpleTypes_Word.h"

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (22.8)
namespace SimpleTypes
{
	const static int shemeDefaultColor[] = 
	{
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,	
		0x00800000,	0x00008000,	0x00000080,	0x00808000,	0x00800080,	0x00008080,	0x00C0C0C0,	0x00808080,	
		0x009999FF,	0x00993366,	0x00FFFFCC,	0x00CCFFFF,	0x00660066,	0x00FF8080,	0x000066CC,	0x00CCCCFF,	
		0x00000080,	0x00FF00FF,	0x00FFFF00,	0x0000FFFF,	0x00800080,	0x00800000,	0x00008080,	0x000000FF,	
		0x0000CCFF,	0x00CCFFFF,	0x00CCFFCC,	0x00FFFF99,	0x0099CCFF,	0x00FF99CC,	0x00CC99FF,	0x00FFCC99,	
		0x003366FF,	0x0033CCCC,	0x0099CC00,	0x00FFCC00,	0x00FF9900,	0x00FF6600,	0x00666699,	0x00969696,	
		0x00003366,	0x00339966,	0x00003300,	0x00333300,	0x00993300,	0x00993366,	0x00333399,	0x00333333
	};
	const static int controlPanelColors2[] = 
	{
		0x00000000,	0x00FFFFFF,	0x00000000,	0x00FFFFFF,
		0x00000000,	0x00000000,	0x00000000,	0x00FFFFFF,	
		0x00FFFFFF,	0x00000000,	0x00FFFFFF,	0x00FFFFFF,	
		0x00000000,	0x00000000,	0x00000000,	0x00000000,	
		0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00000000,	
		0x00FFFFFF,	0x00000000,	0x00000000,	0x00000000,	
		0x00000000,	0x00000000,	0x00FFFFFF,	0x00FFFFFF
	};
	const static int controlPanelColors1[] = 
	{
		0x00FFFFFF,	0x00CCCCCC,	0x00FFFFFF,	0x006363CE,
		0x00DDDDDD,	0x00DDDDDD,	0x00888888,	0x00000000,	
		0x00000000,	0x00808080,	0x00B5D5FF,	0x00000000,	
		0x00FFFFFF,	0x00FFFFFF,	0x007F7F7F,	0x00FBFCC5,	
		0x00000000,	0x00F7F7F7,	0x00000000,	0x00FFFFFF,	
		0x00666666,	0x00C0C0C0,	0x00DDDDDD,	0x00C0C0C0,	
		0x00888888,	0x00FFFFFF,	0x00CCCCCC,	0x00000000
	};
	//--------------------------------------------------------------------------------
	// RelationshipId 22.8.2.1 (Part 1)
	//--------------------------------------------------------------------------------		

	class CRelationshipId
	{
	public:
		CRelationshipId() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(const std::wstring &sValue)
		{
			m_sValue = sValue;
		}


        std::wstring FromString(std::wstring &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CRelationshipId)

	private:

        std::wstring m_sValue;
	};


} // SimpleTypes

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (22.9)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// CalendarType 22.9.2.1 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ECalendarType
	{
		calendartypeGregorian            =  0,
		calendartypeGregorianArabic      =  1,
		calendartypeGregorianMeFrench    =  2,
		calendartypeGregorianUs          =  3,
		calendartypeGregorianXlitEnglish =  4,
		calendartypeGregorianXlitFrench  =  5,
		calendartypeHebrew               =  6,
		calendartypeHijri                =  7,
		calendartypeJapan                =  8,
		calendartypeKorea                =  9,
		calendartypeNone                 = 10,
		calendartypeSaka                 = 11,
		calendartypeTaiwan               = 12,
		calendartypeThai                 = 13
	};

	template<ECalendarType eDefValue = calendartypeNone>
	class CCalendarType : public CSimpleType<ECalendarType, eDefValue>
	{
	public:
		CCalendarType() {}

        virtual ECalendarType FromString(std::wstring &sValue)
		{
            if      ( L"gregorian"            == sValue ) this->m_eValue = calendartypeGregorian;
            else if ( L"gregorianArabic"      == sValue ) this->m_eValue = calendartypeGregorianArabic;
            else if ( L"gregorianMeFrench"    == sValue ) this->m_eValue = calendartypeGregorianMeFrench;
            else if ( L"gregorianUs"          == sValue ) this->m_eValue = calendartypeGregorianUs;
            else if ( L"gregorianXlitEnglish" == sValue ) this->m_eValue = calendartypeGregorianXlitEnglish;
            else if ( L"gregorianXlitFrench"  == sValue ) this->m_eValue = calendartypeGregorianXlitFrench;
            else if ( L"hebrew"               == sValue ) this->m_eValue = calendartypeHebrew;
            else if ( L"hijri"                == sValue ) this->m_eValue = calendartypeHijri;
            else if ( L"japan"                == sValue ) this->m_eValue = calendartypeJapan;
            else if ( L"korea"                == sValue ) this->m_eValue = calendartypeKorea;
            else if ( L"none"                 == sValue ) this->m_eValue = calendartypeNone;
            else if ( L"saka"                 == sValue ) this->m_eValue = calendartypeSaka;
            else if ( L"taiwan"               == sValue ) this->m_eValue = calendartypeTaiwan;
            else if ( L"thai"                 == sValue ) this->m_eValue = calendartypeThai;
            else											this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case calendartypeGregorian            : return L"gregorian";
			case calendartypeGregorianArabic      : return L"gregorianArabic";
			case calendartypeGregorianMeFrench    : return L"gregorianMeFrench";
			case calendartypeGregorianUs          : return L"gregorianUs";
			case calendartypeGregorianXlitEnglish : return L"gregorianXlitEnglish";
			case calendartypeGregorianXlitFrench  : return L"gregorianXlitFrench";
			case calendartypeHebrew               : return L"hebrew";
			case calendartypeHijri                : return L"hijri";
			case calendartypeJapan                : return L"japan";
			case calendartypeKorea                : return L"korea";
			case calendartypeNone                 : return L"none";
			case calendartypeSaka                 : return L"saka";
			case calendartypeTaiwan               : return L"taiwan";
			case calendartypeThai                 : return L"thai";
			default                               : return L"none";
			}
		}

		SimpleType_FromString     (ECalendarType)
		SimpleType_Operator_Equal (CCalendarType)
	};

	//--------------------------------------------------------------------------------
	// ConformanceClass 22.9.2.2 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EConformanceClass
	{
		conformanceclassStrict       = 0,
		conformanceclassTransitional = 1
	};

	template<EConformanceClass eDefValue = conformanceclassStrict>
	class CConformanceClass : public CSimpleType<EConformanceClass, conformanceclassStrict>
	{
	public:
		CConformanceClass() {}

        virtual EConformanceClass FromString(std::wstring &sValue)
		{
            if      ( L"strict"       == sValue )	this->m_eValue = conformanceclassStrict;
            else if ( L"transitional" == sValue )	this->m_eValue = conformanceclassTransitional;
            else									this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case conformanceclassStrict       : return L"strict";
			case conformanceclassTransitional : return L"transitional";
			default                           : return L"strict";
			}
		}

		SimpleType_FromString     (EConformanceClass)
		SimpleType_Operator_Equal (CConformanceClass)
	};
	//--------------------------------------------------------------------------------
	// Guid 22.9.2.4 (Part 1)
	//--------------------------------------------------------------------------------		

	class CGuid
	{
	public:
		CGuid() {}

        bool    FromString(std::wstring &sValue)
		{
			// GUID "{00000000-5BD2-4BC8-9F70-7020E1357FB2}"

			TGuid oZeroGUID = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			m_oGUID = oZeroGUID;

            if ( sValue.length() != 38 )
				return false;
			
            T_ULONG64 unTemp = 0;

            std::wstring sMidValue;
            sMidValue = sValue.substr( 1, 8 );

            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.a = (unsigned int)unTemp;

            sMidValue = sValue.substr( 10, 4 );
            if ( !HexToInt(sMidValue, unTemp ) )
				return false;

			m_oGUID.b = (unsigned short)unTemp;

            sMidValue = sValue.substr( 15, 4 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.c = (unsigned short)unTemp;

            sMidValue = sValue.substr( 20, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.d = (unsigned char)unTemp;


            sMidValue = sValue.substr( 22, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.e = (unsigned char)unTemp;

            sMidValue = sValue.substr( 25, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.f = (unsigned char)unTemp;

            sMidValue = sValue.substr( 27, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.g = (unsigned char)unTemp;

            sMidValue = sValue.substr( 29, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.h = (unsigned char)unTemp;

            sMidValue = sValue.substr( 31, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.i = (unsigned char)unTemp;

            sMidValue = sValue.substr( 33, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.j = (unsigned char)unTemp;

            sMidValue = sValue.substr( 35, 2 );
            if ( !HexToInt( sMidValue, unTemp ) )
				return false;

			m_oGUID.k = (unsigned char)unTemp;

			return true;
		}

		bool    FromString(const std::wstring& wsStr)
		{
			// TO DO: переделать
            std::wstring sTemp( wsStr.c_str() );
            return FromString( (std::wstring &)sTemp );
		}
        bool    FromString(const wchar_t* cwsStr)
        {
            std::wstring wsStr = cwsStr;
            return FromString( (const std::wstring&)wsStr );
        }
        std::wstring ToString  () const
		{
			std::wstringstream sstream;
			sstream << boost::wformat( L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" ) % m_oGUID.a % m_oGUID.b % m_oGUID.c % m_oGUID.d % m_oGUID.e % m_oGUID.f % m_oGUID.g % m_oGUID.h % m_oGUID.i % m_oGUID.j % m_oGUID.k;
			return sstream.str();
		}

		SimpleType_Operator_Equal (CGuid)

		bool IsZero()
		{
			return 0 == m_oGUID.a && 0 == m_oGUID.b && 0 == m_oGUID.c && 0 == m_oGUID.d && 0 == m_oGUID.e && 0 == m_oGUID.f && 0 == m_oGUID.g && 0 == m_oGUID.h && 0 == m_oGUID.i && 0 == m_oGUID.j && 0 == m_oGUID.k;
		}
	private:

        bool HexToInt(std::wstring& sValue, T_ULONG64& unResult)
		{
			bool bResult = true;

			unResult = 0;
            for ( int nIndex = (int)sValue.length() - 1, nMult = 0; nIndex >= 0; nIndex--, nMult += 4 )
			{
				unResult += HexToInt( (int)sValue[nIndex], bResult ) << nMult;
			}

			return bResult;
		}

		int	 HexToInt(int nHex, bool &bResult)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			bResult = false;

			return 0;
		}


	public:

		struct TGuid
		{
			unsigned int   a;
			unsigned short b;
			unsigned short c;
			unsigned char  d;
			unsigned char  e;
			unsigned char  f;
			unsigned char  g;
			unsigned char  h;
			unsigned char  i;
			unsigned char  j;
			unsigned char  k;
		} m_oGUID;

	};
	//--------------------------------------------------------------------------------
	// HexColorRGB 22.9.2.5 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CHexColorRGB : public CSimpleType<int, nDefValue>
	{
	public:
		CHexColorRGB() 
		{
			m_unR = 0;
			m_unG = 0;
			m_unB = 0;
			m_unA = 255;
		}

        virtual int     FromString(std::wstring &sValue)
		{
            if ( 6 <= sValue.length() )
            {
                std::wstring midString = sValue.substr( 0, 6 );
                Parse( midString );
            }
			else
                this->m_eValue = nDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            return XmlUtils::IntToString(this->m_eValue, L"%06X");
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CHexColorRGB)
		unsigned char Get_R() const
		{
			return m_unR;
		}
		unsigned char Get_G() const
		{
			return m_unG;
		}

		unsigned char Get_B() const
		{
			return m_unB;
		}
		unsigned char Get_A() const
		{
			return m_unA;
		}

		void          Set_RGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255)
		{
			m_unR = unR;
			m_unG = unG;
			m_unB = unB;
			m_unA = unA;

            this->m_eValue = ((int)m_unR << 16) + ((int)m_unG << 8) + m_unB;
		}

	private:

        void Parse(std::wstring& sValue)
		{
            if ( sValue.length() < 6 )
				return;

			m_unR = HexToInt( (int)sValue[1] ) + (unsigned char)(HexToInt( (int)sValue[0] ) << 4);
			m_unG = HexToInt( (int)sValue[3] ) + (unsigned char)(HexToInt( (int)sValue[2] ) << 4);
			m_unB = HexToInt( (int)sValue[5] ) + (unsigned char)(HexToInt( (int)sValue[4] ) << 4);
			m_unA = 255;

            this->m_eValue = ((int)m_unR << 16) + ((int)m_unG << 8) + m_unB;
		}

		int	HexToInt(int nHex)
		{
			if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
			if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
			if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

			return 0;
		}

	private:

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;	
		unsigned char m_unA;
	};

	//--------------------------------------------------------------------------------
	// Lang 22.9.2.6 (Part 1)
	//--------------------------------------------------------------------------------		

	// TO DO: сделать парсер языка по спецификации RFC 4646/BCP 47
	class CLang
	{
	public:
		CLang() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
			m_sValue = sValue;
		}


        std::wstring FromString(std::wstring &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CLang)

	private:

        std::wstring m_sValue;
	};


	//--------------------------------------------------------------------------------
	// OnOff 22.9.2.7 (Part 1)
	//--------------------------------------------------------------------------------		

	// Согласно части 4 стр. 1459, значений "on" и "off" быть не должно
	enum EOnOff
	{
		onoffFalse = 0,
		onoffTrue  = 1
	};
	enum EOnOffToString
	{
        onofftostringTrue   = 0,
        onofftostringT      = 1,
        onofftostringOn     = 2,
        onofftostring1      = 3
	};
	template<EOnOff eDefValue = onoffFalse>
	class COnOff : public CSimpleType<EOnOff, eDefValue>
	{
	public:
		COnOff() {}
		
		COnOff(const bool & bVal)
		{
			this->m_eValue = (false != bVal) ? onoffTrue : onoffFalse;
		}

        virtual EOnOff  FromString(std::wstring &sValue)
		{
            if      ( L"true"  == sValue ) this->m_eValue = onoffTrue;
            else if ( L"True"  == sValue ) this->m_eValue = onoffTrue;
            else if ( L"1"     == sValue ) this->m_eValue = onoffTrue;
            else if ( L"t"     == sValue ) this->m_eValue = onoffTrue;
            else if ( L"on"    == sValue ) this->m_eValue = onoffTrue;
            else if ( L"f"     == sValue ) this->m_eValue = onoffFalse;
            else if ( L"0"     == sValue ) this->m_eValue = onoffFalse;
            else if ( L"false" == sValue ) this->m_eValue = onoffFalse;
            else if ( L"False" == sValue ) this->m_eValue = onoffFalse;
            else if ( L"off"   == sValue ) this->m_eValue = onoffFalse;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}
		virtual EOnOff  FromStringA(const char* sValue)
		{
			if		(strcmp("1",		sValue) == 0) this->m_eValue = onoffTrue;
			else if	(strcmp("0",		sValue) == 0) this->m_eValue = onoffFalse;
			else if	(strcmp("true",		sValue) == 0) this->m_eValue = onoffTrue;
			else if	(strcmp("True",		sValue) == 0) this->m_eValue = onoffTrue;
			else if	(strcmp("t",		sValue) == 0) this->m_eValue = onoffTrue;
			else if	(strcmp("on",		sValue) == 0) this->m_eValue = onoffTrue;
			else if	(strcmp("f",		sValue) == 0) this->m_eValue = onoffFalse;
			else if	(strcmp("false",	sValue) == 0) this->m_eValue = onoffFalse;
			else if	(strcmp("False",	sValue) == 0) this->m_eValue = onoffFalse;
			else if	(strcmp("off",		sValue) == 0) this->m_eValue = onoffFalse;
			else                              this->m_eValue = eDefValue;

			return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case onoffFalse : return L"false";
			case onoffTrue  : return L"true";
			default         : return L"false";
			}
		}
        std::wstring ToString2(EOnOffToString eType) const
		{
			if (onofftostringTrue == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse:	return L"false";
				case onoffTrue:		return L"true";
				default:			return L"false";
				}
			}
			else if (onofftostring1 == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse:	return L"0";
				case onoffTrue:		return L"1";
				default:			return L"0";
				}
			}
			else if (onofftostringOn == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse:	return L"off";
				case onoffTrue:		return L"on";
				default:			return L"off";
				}
			}
			else if (onofftostringT == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse:	return L"f";
				case onoffTrue:		return L"t";
				default:			return L"f";
				}
			}
			return L"false";
		}
		std::wstring ToString3(EOnOffToString eType) const
		{
			if(onofftostringTrue == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return L"false";
				case onoffTrue  : return L"true";
				default         : return L"false";
				}
			}
			else if(onofftostring1 == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return L"0";
				case onoffTrue  : return L"1";
				default         : return L"0";
				}
			}
			else if(onofftostringOn == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return L"off";
				case onoffTrue  : return L"on";
				default         : return L"off";
				}
			}
			else if(onofftostringT == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return L"f";
				case onoffTrue  : return L"t";
				default         : return L"f";
				}
			}
			return L"false";
		}
		bool ToBool() 
		{
            return onoffTrue == this->m_eValue;
		}
		void FromBool(bool bVal) 
		{
            this->m_eValue = (false != bVal) ? onoffTrue : onoffFalse;
		}

		SimpleType_FromString     (EOnOff)
		SimpleType_Operator_Equal (COnOff)
	};

	//--------------------------------------------------------------------------------
	// Panose 22.9.2.8 (Part 1)
	//--------------------------------------------------------------------------------

	class CPanose
	{
	public:
		CPanose() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

        void    SetValue(std::wstring &sValue)
		{
			m_sValue = sValue;
		}


        std::wstring FromString(std::wstring &sValue)
		{
            if ( 20 > sValue.length() )
			{
				m_sValue = sValue;
                for ( size_t nIndex = 0; nIndex < 20 - sValue.length(); nIndex++ )
				{
					m_sValue += L"0";
				}
			}
            else if ( 20 == sValue.length() )
				m_sValue = sValue;
			else
			{
                m_sValue = sValue.substr( 0, 20 );
			}

			return m_sValue;
		}

        std::wstring ToString  () const
		{
			return m_sValue;
		}

        SimpleType_FromString2    (std::wstring)
		SimpleType_Operator_Equal (CPanose)

		unsigned char Get_Number(int nIndex)
		{
            if ( m_sValue.length() < 20 )
				return 0;

            nIndex = (std::max)( 0, (std::min)( 9, nIndex ) );

			unsigned int unChar1 = XmlUtils::GetDigit( m_sValue[2 * nIndex] );
			unsigned int unChar2 = XmlUtils::GetDigit( m_sValue[2 * nIndex + 1] );

			return (unChar2 + (unsigned char)(unChar1 << 4));
		}

	private:

        std::wstring m_sValue;
	};


	//--------------------------------------------------------------------------------
	// String 22.9.2.13 (Part 1)
	//--------------------------------------------------------------------------------

    // Ничего не делаем, используем ATL::std::wstring

	//--------------------------------------------------------------------------------
	// TwipsMeasure 22.9.2.14 (Part 1)
	//--------------------------------------------------------------------------------		

	class CTwipsMeasure : public CUniversalMeasure
	{
	public:
		CTwipsMeasure() {}

        virtual double  FromString(std::wstring &sValue)
		{
			Parse(sValue, 20);

			// В данном типе только положительные числа
			m_dValue = fabs( m_dValue );

			return m_dValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult;

			if ( m_bUnit )
				sResult = boost::lexical_cast<std::wstring>( m_dValue ) + L"pt";
			else
                sResult = std::to_wstring( (int)(m_dValue * 20) );

			return sResult;
		}

		virtual double FromPoints(double dValue)
		{
			m_dValue = fabs( dValue );
			return m_dValue;
		}
		virtual double FromInches(double dValue)
		{
			m_dValue = fabs( dValue ) * 72;
			return m_dValue;
		}


  
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CTwipsMeasure)
		UniversalMeasure_AdditionalOpearators(CTwipsMeasure)
	};




	//--------------------------------------------------------------------------------
	// UnsignedDecimalNumber 22.9.2.16 (Part 1)
	//--------------------------------------------------------------------------------		

	template<unsigned int unDefValue = 0>
	class CUnsignedDecimalNumber : public CSimpleType<unsigned int, unDefValue>
	{
	public:
		CUnsignedDecimalNumber() {}

        CUnsignedDecimalNumber(const CUnsignedDecimalNumber& obj)
        {
            this->m_eValue = obj.m_eValue;
        }
        CUnsignedDecimalNumber(const unsigned int& val)
        {
            this->m_eValue = val;
        }
        CUnsignedDecimalNumber(const _INT32& val)
        {
            this->m_eValue = (unsigned int)val;
        }
        virtual unsigned int FromString(std::wstring &sValue)
		{

            try
            {
                this->m_eValue = _wtoi( sValue.c_str() );
                return this->m_eValue;
            }
            catch(...)
            {
            }

            try
            {
                this->m_eValue = static_cast<int>(_wtoi64(sValue.c_str()));
			}
            catch(...)
            {
                this->m_eValue = 0;
            }

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            return std::to_wstring( this->m_eValue);
		}


        SimpleType_FromString     (unsigned int)
		SimpleType_Operator_Equal (CUnsignedDecimalNumber)
	};

	//--------------------------------------------------------------------------------
	// VerticalAlignRun 22.9.2.17 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EVerticalAlignRun
	{
		verticalalignrunBaseline    = 0,
		verticalalignrunSubscript   = 1,
		verticalalignrunSuperscript = 2
	};

	template<EVerticalAlignRun eDefValue = verticalalignrunBaseline>
	class CVerticalAlignRun : public CSimpleType<EVerticalAlignRun, eDefValue>
	{
	public:
		CVerticalAlignRun() {}

        virtual EVerticalAlignRun    FromString(std::wstring &sValue)
		{
            if      ( L"baseline"    == sValue ) this->m_eValue = verticalalignrunBaseline;
            else if ( L"subscript"   == sValue ) this->m_eValue = verticalalignrunSubscript;
            else if ( L"superscript" == sValue ) this->m_eValue = verticalalignrunSuperscript;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(this->m_eValue)
			{
			case verticalalignrunBaseline    : return L"baseline";
			case verticalalignrunSubscript   : return L"subscript";
			case verticalalignrunSuperscript : return L"superscript";
			default                          : return L"baseline";
			}
		}

		SimpleType_FromString     (EVerticalAlignRun)
		SimpleType_Operator_Equal (CVerticalAlignRun)
	};


	//--------------------------------------------------------------------------------
	// XAlign 22.9.2.18 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EXAlign
	{
		xalignCenter  = 0,
		xalignInside  = 1,
		xalignLeft    = 2,
		xalignOutside = 3,
		xalignRight   = 4
	};

	template<EXAlign eDefValue = xalignLeft>
	class CXAlign : public CSimpleType<EXAlign, eDefValue>
	{
	public:
		CXAlign() {}

        virtual EXAlign FromString(std::wstring &sValue)
		{
            if      ( L"center"  == sValue )	this->m_eValue = xalignCenter;
            else if ( L"inside"  == sValue )	this->m_eValue = xalignInside;
            else if ( L"left"    == sValue )	this->m_eValue = xalignLeft;
            else if ( L"outside" == sValue )	this->m_eValue = xalignOutside;
            else if ( L"right"   == sValue )	this->m_eValue = xalignRight;
            else								this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case xalignCenter  : return L"center";
			case xalignInside  : return L"inside";
			case xalignLeft    : return L"left";
			case xalignOutside : return L"outside";
			case xalignRight   : return L"right";
			default            : return L"left";
			}
		}

		SimpleType_FromString     (EXAlign)
		SimpleType_Operator_Equal (CXAlign)
	};




	//--------------------------------------------------------------------------------
	// YAlign 22.9.2.20 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EYAlign
	{
		yalignBottom  = 0,
		yalignCenter  = 1,
		yalignInline  = 2,
		yalignInside  = 3,
		yalignOutside = 4,
		yalignTop     = 5
	};

	template<EYAlign eDefValue = yalignTop>
	class CYAlign : public CSimpleType<EYAlign, eDefValue>
	{
	public:
		CYAlign() {}

        virtual EYAlign FromString(std::wstring &sValue)
		{
            if      ( L"bottom"		== sValue ) this->m_eValue = yalignBottom;
            else if ( L"bot"		== sValue ) this->m_eValue = yalignBottom;
            else if ( L"center"		== sValue ) this->m_eValue = yalignCenter;
            else if ( L"inline"		== sValue ) this->m_eValue = yalignInline;
            else if ( L"inside"		== sValue ) this->m_eValue = yalignInside;
            else if ( L"outside"	== sValue ) this->m_eValue = yalignOutside;
            else if ( L"top"		== sValue ) this->m_eValue = yalignTop;
            else								this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case yalignBottom  : return L"bottom";
			case yalignCenter  : return L"center";
			case yalignInline  : return L"inline";
			case yalignInside  : return L"inside";
			case yalignOutside : return L"outside";
			case yalignTop     : return L"top";
			default            : return L"top";
			}
		}

		SimpleType_FromString     (EYAlign)
		SimpleType_Operator_Equal (CYAlign)
	};





} // SimpleTypes

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (15.1.2 - part 4)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// ColorType 15.1.2.3 (Part 4)
	//--------------------------------------------------------------------------------
	enum EColorType
	{
		colortypeNone,
		colortypeRGB,
		colortypeAqua,
		colortypeBlack,
		colortypeBlue,
		colortypeFuchsia,
		colortypeGray,
		colortypeGreen,
		colortypeLime,
		colortypeMaroon,
		colortypeNavy,
		colortypeOlive,
		colortypePurple,
		colortypeRed,
		colortypeSilver,
		colortypeTeal,
		colortypeWhite,
		colortypeYellow,
	};
	template<EColorType eDefValue = colortypeNone>
	class CColorType : public CSimpleType<EColorType, eDefValue>
	{
    public:
        CColorType()
        {
            SetRGB();

			std::wstringstream sstream;
			sstream << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
			m_sValue = sstream.str();
        }

        virtual EColorType FromString(std::wstring& sValue)
        {
            wchar_t wsFirstChar = 0;
            if (sValue.length() > 0)
                wsFirstChar = sValue[0];

            m_sValue = sValue;
            if (wsFirstChar == _T('#'))
            {
                std::wstring sHexColor = sValue.substr(1);
                ByHexColor(sHexColor);
            }
            else
                ByColorName(sValue);

            return this->m_eValue;
        }

        virtual std::wstring ToString() const
        {
            return m_sValue;
        }

        void SetRGB(unsigned char unR, unsigned char unG, unsigned char unB)
        {
            this->m_eValue = colortypeRGB;
            m_unR = unR;
            m_unG = unG;
            m_unB = unB;

			std::wstringstream sstream;
			sstream << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
			m_sValue = sstream.str();
        }

        SimpleType_FromString     (EColorType)
        SimpleType_Operator_Equal (CColorType)

        unsigned char Get_R() const
        {
            return m_unR;
        }
        unsigned char Get_G() const
        {
            return m_unG;
        }

        unsigned char Get_B() const
        {
            return m_unB;
        }
        unsigned char Get_A() const
        {
            return 255;
        }



	private:
        void ByHexColor(std::wstring& sValue)
		{
            this->m_eValue = colortypeRGB;

			CHexColor<> hexColor;
			hexColor.FromString(sValue);

			m_unR = hexColor.Get_R();
			m_unG = hexColor.Get_G();
			m_unB = hexColor.Get_B();
		}
		void SetRGB()
		{
			if (this->m_eValue == colortypeRGB) return;

            switch(this->m_eValue)
			{
				case  colortypeAqua:
				{
					m_unR = 0x00;
					m_unG = 0xff;
					m_unB = 0xff;
				}break;
				case  colortypeBlack:
				{
					m_unR = 0x00;
					m_unG = 0x00;
					m_unB = 0x00;
				}break;
				case  colortypeBlue:
				{
					m_unR = 0x00;
					m_unG = 0x00;
					m_unB = 0xff;
				}break;
				case  colortypeFuchsia:
				{
					m_unR = 0xff;
					m_unG = 0x00;
					m_unB = 0xff;
				}break;
				case  colortypeGray:
				{
					m_unR = 0x80;
					m_unG = 0x80;
					m_unB = 0x80;
				}break;
				case  colortypeGreen:
				{
					m_unR = 0x00;
					m_unG = 0x80;
					m_unB = 0x00;
				}break;
				case  colortypeLime:
				{
					m_unR = 0x00;
					m_unG = 0xff;
					m_unB = 0x00;
				}break;
				case  colortypeMaroon:
				{
					m_unR = 0x80;
					m_unG = 0x00;
					m_unB = 0x00;
				}break;
				case  colortypeNavy:
				{
					m_unR = 0x00;
					m_unG = 0x00;
					m_unB = 0x80;
				}break;
				case  colortypeOlive:
				{
					m_unR = 0x80;
					m_unG = 0x80;
					m_unB = 0x00;
				}break;
				case  colortypePurple:
				{
					m_unR = 0x80;
					m_unG = 0x00;
					m_unB = 0x80;
				}break;
				case  colortypeRed:
				{
					m_unR = 0xff;
					m_unG = 0x00;
					m_unB = 0x00;
				}break;
				case  colortypeSilver:
				{
					m_unR = 0xc0;
					m_unG = 0xc0;
					m_unB = 0xc0;
				}break;
				case  colortypeTeal:
				{
					m_unR = 0x00;
					m_unG = 0x80;
					m_unB = 0x80;
				}break;
				case  colortypeWhite:
				{
					m_unR = 0xff;
					m_unG = 0xff;
					m_unB = 0xff;
				}break;
				case  colortypeYellow:
				{
					m_unR = 0xff;
					m_unG = 0xff;
					m_unB = 0;
				}break;
				case  colortypeNone:
				default:
				{
					m_unR = 0;
					m_unG = 0;
					m_unB = 0;
				}break;
			}
		}
        void ByColorName(std::wstring& sValue)
		{
			this->m_eValue = colortypeNone;

			if      (std::wstring::npos != sValue.find(L"aqua"))	this->m_eValue = colortypeAqua;
            else if (std::wstring::npos != sValue.find(L"black"))	this->m_eValue = colortypeBlack;
            else if (std::wstring::npos != sValue.find(L"blue"))	this->m_eValue = colortypeBlue;
            else if (std::wstring::npos != sValue.find(L"fuchsia"))	this->m_eValue = colortypeFuchsia;
            else if (std::wstring::npos != sValue.find(L"gray"))	this->m_eValue = colortypeGray;
            else if (std::wstring::npos != sValue.find(L"green"))	this->m_eValue = colortypeGreen;
            else if (std::wstring::npos != sValue.find(L"lime"))	this->m_eValue = colortypeLime;
            else if (std::wstring::npos != sValue.find(L"maroon"))	this->m_eValue = colortypeMaroon;
            else if (std::wstring::npos != sValue.find(L"navy"))	this->m_eValue = colortypeNavy;
            else if (std::wstring::npos != sValue.find(L"olive"))	this->m_eValue = colortypeOlive;
            else if (std::wstring::npos != sValue.find(L"purple"))	this->m_eValue = colortypePurple;
            else if (std::wstring::npos != sValue.find(L"red"))		this->m_eValue = colortypeRed;
            else if (std::wstring::npos != sValue.find(L"silver"))	this->m_eValue = colortypeSilver;
            else if (std::wstring::npos != sValue.find(L"teal"))	this->m_eValue = colortypeTeal;
            else if (std::wstring::npos != sValue.find(L"white"))	this->m_eValue = colortypeWhite;
            else if (std::wstring::npos != sValue.find(L"yellow"))	this->m_eValue = colortypeYellow;
			else if (std::wstring::npos != sValue.find(L"[") && std::wstring::npos != sValue.find(L"]"))
			{				
				size_t p1 = sValue.find(L"[");
				size_t p2 = sValue.find(L"]");
				std::wstring sIndex = p2 > p1 ? sValue.substr(p1 + 1, p2 - p1 - 1) : L"";

				if (!sIndex.empty())
				{
					int index = XmlUtils::GetInteger(sIndex);
					int nRGB = 0;
					if (index < 64)
					{
						nRGB = shemeDefaultColor[index];
					}
					else if (index > 64 && index < 92)
					{
						nRGB = controlPanelColors1[index - 65];
					}
					m_unR = static_cast<unsigned char>((nRGB >> 16) & 0xff);
					m_unG = static_cast<unsigned char>((nRGB >> 8) & 0xff);
					m_unB = static_cast<unsigned char>(nRGB & 0xff);
					this->m_eValue = colortypeRGB;
				}
			}

			SetRGB();
		}

	private:

        std::wstring m_sValue;

		unsigned char m_unR;
		unsigned char m_unG;
        unsigned char m_unB;
	};
	//--------------------------------------------------------------------------------
	// CTrueFalse 15.1.2.5 - 15.1.2.6 (Part 4)
	//--------------------------------------------------------------------------------		

	enum ETrueFalse
	{
		booleanFalse = 0,
		booleanTrue  = 1
	};

	template<ETrueFalse eDefValue = booleanFalse>
	class CTrueFalse : public CSimpleType<ETrueFalse, eDefValue>
	{
	public:
		CTrueFalse() {}

        virtual ETrueFalse FromString(std::wstring &sValue)
		{
            if      ( L"t"     == sValue )	this->m_eValue = booleanTrue;
            else if ( L"true"  == sValue )	this->m_eValue = booleanTrue;
            else if ( L"True"  == sValue )	this->m_eValue = booleanTrue;
            else if ( L""      == sValue )	this->m_eValue = booleanFalse;
            else if ( L"f"     == sValue )	this->m_eValue = booleanFalse;
            else if ( L"false" == sValue )	this->m_eValue = booleanFalse;
            else if ( L"False" == sValue )	this->m_eValue = booleanFalse;
            else							this->m_eValue = booleanFalse;

            return this->m_eValue;
		}

		bool GetBool()
		{
			return this->m_eValue == booleanTrue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(this->m_eValue)
			{
			case booleanFalse : return L"f";
			case booleanTrue  : return L"t";
			default           : return L"f";
			}
		}

		SimpleType_FromString     (ETrueFalse)
		SimpleType_Operator_Equal (CTrueFalse)
	};
	enum ESdtAppearance
	{
		sdtappearenceTags = 0,
		sdtappearenceBoundingBox = 1,
		sdtappearenceHidden = 2
	};

	template<ESdtAppearance eDefValue = sdtappearenceBoundingBox>
	class CSdtAppearance : public CSimpleType<ESdtAppearance, eDefValue>
	{
	public:
		CSdtAppearance() {}

		virtual ESdtAppearance FromString(std::wstring &sValue)
		{
			if      ( L"boundingBox"     == sValue )	this->m_eValue = sdtappearenceBoundingBox;
			else if ( L"tags"  == sValue )	this->m_eValue = sdtappearenceTags;
			else if ( L"hidden"  == sValue )	this->m_eValue = sdtappearenceHidden;

			return this->m_eValue;
		}

		virtual std::wstring    ToString  () const
		{
			switch(this->m_eValue)
			{
			case sdtappearenceTags : return L"tags";
			case sdtappearenceHidden  : return L"hidden";
			default           : return L"boundingBox";
			}
		}

		SimpleType_FromString     (ESdtAppearance)
		SimpleType_Operator_Equal (CSdtAppearance)
	};
} // SimpleTypes

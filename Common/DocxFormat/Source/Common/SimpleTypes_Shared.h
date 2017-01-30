/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
            if      ( _T("gregorian")            == sValue ) this->m_eValue = calendartypeGregorian;
            else if ( _T("gregorianArabic")      == sValue ) this->m_eValue = calendartypeGregorianArabic;
            else if ( _T("gregorianMeFrench")    == sValue ) this->m_eValue = calendartypeGregorianMeFrench;
            else if ( _T("gregorianUs")          == sValue ) this->m_eValue = calendartypeGregorianUs;
            else if ( _T("gregorianXlitEnglish") == sValue ) this->m_eValue = calendartypeGregorianXlitEnglish;
            else if ( _T("gregorianXlitFrench")  == sValue ) this->m_eValue = calendartypeGregorianXlitFrench;
            else if ( _T("hebrew")               == sValue ) this->m_eValue = calendartypeHebrew;
            else if ( _T("hijri")                == sValue ) this->m_eValue = calendartypeHijri;
            else if ( _T("japan")                == sValue ) this->m_eValue = calendartypeJapan;
            else if ( _T("korea")                == sValue ) this->m_eValue = calendartypeKorea;
            else if ( _T("none")                 == sValue ) this->m_eValue = calendartypeNone;
            else if ( _T("saka")                 == sValue ) this->m_eValue = calendartypeSaka;
            else if ( _T("taiwan")               == sValue ) this->m_eValue = calendartypeTaiwan;
            else if ( _T("thai")                 == sValue ) this->m_eValue = calendartypeThai;
            else                                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case calendartypeGregorian            : return _T("gregorian");
			case calendartypeGregorianArabic      : return _T("gregorianArabic");
			case calendartypeGregorianMeFrench    : return _T("gregorianMeFrench");
			case calendartypeGregorianUs          : return _T("gregorianUs");
			case calendartypeGregorianXlitEnglish : return _T("gregorianXlitEnglish");
			case calendartypeGregorianXlitFrench  : return _T("gregorianXlitFrench");
			case calendartypeHebrew               : return _T("hebrew");
			case calendartypeHijri                : return _T("hijri");
			case calendartypeJapan                : return _T("japan");
			case calendartypeKorea                : return _T("korea");
			case calendartypeNone                 : return _T("none");
			case calendartypeSaka                 : return _T("saka");
			case calendartypeTaiwan               : return _T("taiwan");
			case calendartypeThai                 : return _T("thai");
			default                               : return _T("none");
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
            if      ( _T("strict")       == sValue ) this->m_eValue = conformanceclassStrict;
            else if ( _T("transitional") == sValue ) this->m_eValue = conformanceclassTransitional;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case conformanceclassStrict       : return _T("strict");
			case conformanceclassTransitional : return _T("transitional");
			default                           : return _T("strict");
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

	private:

        bool HexToInt(std::wstring& sValue, T_ULONG64& unResult)
		{
			bool bResult = true;

			unResult = 0;
            for ( size_t nIndex = sValue.length() - 1, nMult = 0; nIndex >= 0; nIndex--, nMult += 4 )
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

        virtual EOnOff  FromString(std::wstring &sValue)
		{
            if      ( _T("true")  == sValue ) this->m_eValue = onoffTrue;
            else if ( _T("True")  == sValue ) this->m_eValue = onoffTrue;
            else if ( _T("1")     == sValue ) this->m_eValue = onoffTrue;
            else if ( _T("t")     == sValue ) this->m_eValue = onoffTrue;
            else if ( _T("on")    == sValue ) this->m_eValue = onoffTrue;
            else if ( _T("f")     == sValue ) this->m_eValue = onoffFalse;
            else if ( _T("0")     == sValue ) this->m_eValue = onoffFalse;
            else if ( _T("false") == sValue ) this->m_eValue = onoffFalse;
            else if ( _T("False") == sValue ) this->m_eValue = onoffFalse;
            else if ( _T("off")   == sValue ) this->m_eValue = onoffFalse;
            else                              this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case onoffFalse : return _T("false");
			case onoffTrue  : return _T("true");
			default         : return _T("false");
			}
		}
        std::wstring ToString2(EOnOffToString eType) const
		{
			if (onofftostringTrue == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse: return _T("false");
				case onoffTrue: return _T("true");
				default: return _T("false");
				}
			}
			else if (onofftostring1 == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse: return _T("0");
				case onoffTrue: return _T("1");
				default: return _T("0");
				}
			}
			else if (onofftostringOn == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse: return _T("off");
				case onoffTrue: return _T("on");
				default: return _T("off");
				}
			}
			else if (onofftostringT == eType)
			{
                switch (this->m_eValue)
				{
				case onoffFalse: return _T("f");
				case onoffTrue: return _T("t");
				default: return _T("f");
				}
			}
			return _T("false");
		}
		std::wstring ToString3(EOnOffToString eType) const
		{
			if(onofftostringTrue == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return _T("false");
				case onoffTrue  : return _T("true");
				default         : return _T("false");
				}
			}
			else if(onofftostring1 == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return _T("0");
				case onoffTrue  : return _T("1");
				default         : return _T("0");
				}
			}
			else if(onofftostringOn == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return _T("off");
				case onoffTrue  : return _T("on");
				default         : return _T("off");
				}
			}
			else if(onofftostringT == eType)
			{
                switch(this->m_eValue)
				{
				case onoffFalse : return _T("f");
				case onoffTrue  : return _T("t");
				default         : return _T("f");
				}
			}
			return _T("false");
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
					m_sValue += _T("0");
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

        virtual std::wstring      ToString  () const
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
            if      ( _T("baseline")    == sValue ) this->m_eValue = verticalalignrunBaseline;
            else if ( _T("subscript")   == sValue ) this->m_eValue = verticalalignrunSubscript;
            else if ( _T("superscript") == sValue ) this->m_eValue = verticalalignrunSuperscript;
            else                                    this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(this->m_eValue)
			{
			case verticalalignrunBaseline    : return _T("baseline");
			case verticalalignrunSubscript   : return _T("subscript");
			case verticalalignrunSuperscript : return _T("superscript");
			default                          : return _T("baseline");
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
            if      ( _T("center")  == sValue ) this->m_eValue = xalignCenter;
            else if ( _T("inside")  == sValue ) this->m_eValue = xalignInside;
            else if ( _T("left")    == sValue ) this->m_eValue = xalignLeft;
            else if ( _T("outside") == sValue ) this->m_eValue = xalignOutside;
            else if ( _T("right")   == sValue ) this->m_eValue = xalignRight;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case xalignCenter  : return _T("center");
			case xalignInside  : return _T("inside");
			case xalignLeft    : return _T("left");
			case xalignOutside : return _T("outside");
			case xalignRight   : return _T("right");
			default            : return _T("left");
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
            if      ( _T("bottom")  == sValue ) this->m_eValue = yalignBottom;
            else if ( _T("bot")		== sValue ) this->m_eValue = yalignBottom;
            else if ( _T("center")  == sValue ) this->m_eValue = yalignCenter;
            else if ( _T("inline")  == sValue ) this->m_eValue = yalignInline;
            else if ( _T("inside")  == sValue ) this->m_eValue = yalignInside;
            else if ( _T("outside") == sValue ) this->m_eValue = yalignOutside;
            else if ( _T("top")     == sValue ) this->m_eValue = yalignTop;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            switch(this->m_eValue)
			{
			case yalignBottom  : return _T("bottom");
			case yalignCenter  : return _T("center");
			case yalignInline  : return _T("inline");
			case yalignInside  : return _T("inside");
			case yalignOutside : return _T("outside");
			case yalignTop     : return _T("top");
			default            : return _T("top");
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
            if      (sValue.find(_T("aqua"))	>= 0)this->m_eValue = colortypeAqua;
            else if (sValue.find(_T("black"))	>= 0)this->m_eValue = colortypeBlack;
            else if (sValue.find(_T("blue"))	>= 0)this->m_eValue = colortypeBlue;
            else if (sValue.find(_T("fuchsia")) >= 0)this->m_eValue = colortypeFuchsia;
            else if (sValue.find(_T("gray"))	>= 0)this->m_eValue = colortypeGray;
            else if (sValue.find(_T("green"))	>= 0)this->m_eValue = colortypeGreen;
            else if (sValue.find(_T("lime"))	>= 0)this->m_eValue = colortypeLime;
            else if (sValue.find(_T("maroon"))	>= 0)this->m_eValue = colortypeMaroon;
            else if (sValue.find(_T("navy"))	>= 0)this->m_eValue = colortypeNavy;
            else if (sValue.find(_T("olive"))	>= 0)this->m_eValue = colortypeOlive;
            else if (sValue.find(_T("purple"))	>= 0)this->m_eValue = colortypePurple;
            else if (sValue.find(_T("red"))		>= 0)this->m_eValue = colortypeRed;
            else if (sValue.find(_T("silver"))	>= 0)this->m_eValue = colortypeSilver;
            else if (sValue.find(_T("teal"))	>= 0)this->m_eValue = colortypeTeal;
            else if (sValue.find(_T("white"))	>= 0)this->m_eValue = colortypeWhite;
            else if (sValue.find(_T("yellow"))	>= 0)this->m_eValue = colortypeYellow;
            else this->m_eValue = colortypeNone;

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
            if      ( _T("t")     == sValue ) this->m_eValue = booleanTrue;
            else if ( _T("true")  == sValue ) this->m_eValue = booleanTrue;
            else if ( _T("True")  == sValue ) this->m_eValue = booleanTrue;
            else if ( _T("")      == sValue ) this->m_eValue = booleanFalse;
            else if ( _T("f")     == sValue ) this->m_eValue = booleanFalse;
            else if ( _T("false") == sValue ) this->m_eValue = booleanFalse;
            else if ( _T("False") == sValue ) this->m_eValue = booleanFalse;
            else                              this->m_eValue = booleanFalse;

            return this->m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(this->m_eValue)
			{
			case booleanFalse : return _T("f");
			case booleanTrue  : return _T("t");
			default           : return _T("f");
			}
		}

		SimpleType_FromString     (ETrueFalse)
		SimpleType_Operator_Equal (CTrueFalse)
	};
} // SimpleTypes

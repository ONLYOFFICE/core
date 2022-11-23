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

#include "SimpleTypes_Shared.h"
#include "SimpleTypes_Word.h"
#include <sstream>
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include "../Base/Unit.h"

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

	CRelationshipId::CRelationshipId() {}

	std::wstring CRelationshipId::GetValue() const
	{
		return m_sValue;
	}

	void CRelationshipId::SetValue(const std::wstring &sValue)
	{
		m_sValue = sValue;
	}

	std::wstring CRelationshipId::FromString(const std::wstring &sValue)
	{
		m_sValue = sValue;

		return m_sValue;
	}

	std::wstring CRelationshipId::ToString  () const
	{
		return m_sValue;
	}

} // SimpleTypes

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (22.9)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// CalendarType 22.9.2.1 (Part 1)
	//--------------------------------------------------------------------------------	

	ECalendarType CCalendarType::FromString(const std::wstring &sValue)
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
		else											this->m_eValue = calendartypeNone;

		return this->m_eValue;
	}

	std::wstring CCalendarType::ToString  () const
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

	//--------------------------------------------------------------------------------
	// ConformanceClass 22.9.2.2 (Part 1)
	//--------------------------------------------------------------------------------

	EConformanceClass CConformanceClass::FromString(const std::wstring &sValue)
	{
		if      ( L"strict"       == sValue )	this->m_eValue = conformanceclassStrict;
		else if ( L"transitional" == sValue )	this->m_eValue = conformanceclassTransitional;
		else									this->m_eValue = conformanceclassTransitional;

		return this->m_eValue;
	}

	std::wstring CConformanceClass::ToString  () const
	{
		switch(this->m_eValue)
		{
		case conformanceclassStrict       : return L"strict";
		case conformanceclassTransitional : return L"transitional";
		default                           : return L"strict";
		}
	}

	//--------------------------------------------------------------------------------
	// Guid 22.9.2.4 (Part 1)
	//--------------------------------------------------------------------------------

	CGuid::CGuid() {}

	bool CGuid::FromString(const std::wstring &sValue)
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

	std::wstring CGuid::ToString  () const
	{
		std::wstringstream sstream;
		sstream << boost::wformat( L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" ) % m_oGUID.a % m_oGUID.b % m_oGUID.c % m_oGUID.d % m_oGUID.e % m_oGUID.f % m_oGUID.g % m_oGUID.h % m_oGUID.i % m_oGUID.j % m_oGUID.k;
		return sstream.str();
	}

	bool CGuid::IsZero()
	{
		return 0 == m_oGUID.a && 0 == m_oGUID.b && 0 == m_oGUID.c && 0 == m_oGUID.d && 0 == m_oGUID.e && 0 == m_oGUID.f && 0 == m_oGUID.g && 0 == m_oGUID.h && 0 == m_oGUID.i && 0 == m_oGUID.j && 0 == m_oGUID.k;
	}

	bool CGuid::HexToInt(std::wstring& sValue, T_ULONG64& unResult)
	{
		bool bResult = true;

		unResult = 0;
		for ( int nIndex = (int)sValue.length() - 1, nMult = 0; nIndex >= 0; nIndex--, nMult += 4 )
		{
			unResult += HexToInt( (int)sValue[nIndex], bResult ) << nMult;
		}

		return bResult;
	}

	int	CGuid::HexToInt(int nHex, bool &bResult)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		bResult = false;

		return 0;
	}

	//--------------------------------------------------------------------------------
	// HexColorRGB 22.9.2.5 (Part 1)
	//--------------------------------------------------------------------------------

	int	CHexColorRGB::HexToInt(int nHex)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (nHex - 'A' + 10);

		return 0;
	}

	void CHexColorRGB::Parse(const std::wstring& sValue)
	{
		if ( sValue.length() < 6 )
			return;

		m_unR = HexToInt( (int)sValue[1] ) + (unsigned char)(HexToInt( (int)sValue[0] ) << 4);
		m_unG = HexToInt( (int)sValue[3] ) + (unsigned char)(HexToInt( (int)sValue[2] ) << 4);
		m_unB = HexToInt( (int)sValue[5] ) + (unsigned char)(HexToInt( (int)sValue[4] ) << 4);
		m_unA = 255;

		this->m_eValue = ((int)m_unR << 16) + ((int)m_unG << 8) + m_unB;
	}

	int CHexColorRGB::FromString(const std::wstring &sValue)
	{
		if ( 6 <= sValue.length() )
		{
			std::wstring midString = sValue.substr( 0, 6 );
			Parse( midString );
		}
		else
			this->m_eValue = 0;

		return this->m_eValue;
	}

	std::wstring CHexColorRGB::ToString  () const
	{
		return XmlUtils::ToString(this->m_eValue, L"%06X");
	}

	unsigned char CHexColorRGB::Get_R() const
	{
		return m_unR;
	}

	unsigned char CHexColorRGB::Get_G() const
	{
		return m_unG;
	}

	unsigned char CHexColorRGB::Get_B() const
	{
		return m_unB;
	}

	unsigned char CHexColorRGB::Get_A() const
	{
		return m_unA;
	}

	void CHexColorRGB::Set_RGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA)
	{
		m_unR = unR;
		m_unG = unG;
		m_unB = unB;
		m_unA = unA;

		this->m_eValue = ((int)m_unR << 16) + ((int)m_unG << 8) + m_unB;
	}

	//--------------------------------------------------------------------------------
	// Lang 22.9.2.6 (Part 1)
	//--------------------------------------------------------------------------------

	// TO DO: сделать парсер языка по спецификации RFC 4646/BCP 47
	//class CLang
	//{
	//public:
	//	CLang() {}

	//       std::wstring GetValue() const
	//	{
	//		return m_sValue;
	//	}

	//       void    SetValue(std::wstring &sValue)
	//	{
	//		m_sValue = sValue;
	//	}


	//       std::wstring FromString(const std::wstring &sValue)
	//	{
	//		m_sValue = sValue;

	//		return m_sValue;
	//	}

	//       std::wstring ToString  () const
	//	{
	//		return m_sValue;
	//	}

	//       SimpleType_FromString2    (std::wstring)
	//	SimpleTypes_Default(CLang)

	//private:

	//       std::wstring m_sValue;
	//};


	//--------------------------------------------------------------------------------
	// OnOff 22.9.2.7 (Part 1)
	//--------------------------------------------------------------------------------

	// Согласно части 4 стр. 1459, значений "on" и "off" быть не должно

	COnOff::COnOff(const bool & bVal)
	{
		this->m_eValue = (false != bVal) ? onoffTrue : onoffFalse;
	}

	EOnOff COnOff::FromString(const std::wstring &sValue)
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
		else                              this->m_eValue = onoffFalse;

		return this->m_eValue;
	}

	/*EOnOff COnOff::FromStringA(const std::string& sValue)
	{
		if		("1" == sValue) this->m_eValue = onoffTrue;
		else if	("0" == sValue) this->m_eValue = onoffFalse;
		else if	("true" == sValue) this->m_eValue = onoffTrue;
		else if	("True" == sValue) this->m_eValue = onoffTrue;
		else if	("t" == sValue) this->m_eValue = onoffTrue;
		else if	("on" == sValue) this->m_eValue = onoffTrue;
		else if	("f" == sValue) this->m_eValue = onoffFalse;
		else if	("false" == sValue) this->m_eValue = onoffFalse;
		else if	("False" == sValue) this->m_eValue = onoffFalse;
		else if	("off" == sValue) this->m_eValue = onoffFalse;
		else this->m_eValue = onoffFalse;

		return this->m_eValue;
	}*/

	EOnOff COnOff::FromStringA(const char* sValue)
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
		else this->m_eValue = onoffFalse;

		return this->m_eValue;
	}

	std::wstring COnOff::ToString  () const
	{
		switch(this->m_eValue)
		{
		case onoffFalse : return L"false";
		case onoffTrue  : return L"true";
		default         : return L"false";
		}
	}

	std::wstring COnOff::ToString2(EOnOffToString eType) const
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

	std::wstring COnOff::ToString3(EOnOffToString eType) const
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

	bool COnOff::ToBool()
	{
		return onoffTrue == this->m_eValue;
	}

	void COnOff::FromBool(bool bVal)
	{
		this->m_eValue = (false != bVal) ? onoffTrue : onoffFalse;
	}

	CBool::CBool() {}

	CBool::CBool(const bool & bVal)
	{
		this->m_eValue = (false != bVal) ? onoffTrue : onoffFalse;
	}
	std::wstring CBool::ToString() const
	{
		switch (m_eValue)
		{
		case onoffTrue: return L"1";
		case onoffFalse:
		default:		return L"0";
		}
	}
	EOnOff CBool::FromString(const std::wstring &sValue)
	{
		return COnOff::FromString(sValue);
	}

	//--------------------------------------------------------------------------------
	// Panose 22.9.2.8 (Part 1)
	//--------------------------------------------------------------------------------

	CPanose::CPanose() {}

	std::wstring CPanose::GetValue() const
	{
		return m_sValue;
	}

	void CPanose::SetValue(std::wstring &sValue)
	{
		m_sValue = sValue;
	}


	std::wstring CPanose::FromString(const std::wstring &sValue)
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

	std::wstring CPanose::ToString  () const
	{
		return m_sValue;
	}

	unsigned char CPanose::Get_Number(int nIndex)
	{
		if ( m_sValue.length() < 20 )
			return 0;

		nIndex = (std::max)( 0, (std::min)( 9, nIndex ) );

		unsigned int unChar1 = XmlUtils::GetDigit( m_sValue[2 * nIndex] );
		unsigned int unChar2 = XmlUtils::GetDigit( m_sValue[2 * nIndex + 1] );

		return (unChar2 + (unsigned char)(unChar1 << 4));
	}

	//--------------------------------------------------------------------------------
	// String 22.9.2.13 (Part 1)
	//--------------------------------------------------------------------------------

	// Ничего не делаем, используем ATL::std::wstring

	//--------------------------------------------------------------------------------
	// TwipsMeasure 22.9.2.14 (Part 1)
	//--------------------------------------------------------------------------------

	CTwipsMeasure::CTwipsMeasure() {}

	double CTwipsMeasure::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 20);

		// В данном типе только положительные числа
		m_dValue = fabs( m_dValue );

		return m_dValue;
	}
	void CTwipsMeasure::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = FromTwips(dValue);
	}
	std::wstring CTwipsMeasure::ToString() const
	{
		std::wstring sResult;

		if ( m_bUnit )
			sResult = boost::lexical_cast<std::wstring>( m_dValue ) + L"pt";
		else
			sResult = std::to_wstring( (int)(m_dValue * 20) );

		return sResult;
	}

	double CTwipsMeasure::FromPoints(double dValue)
	{
		m_dValue = fabs( dValue );
		return m_dValue;
	}
	double CTwipsMeasure::FromInches(double dValue)
	{
		m_dValue = fabs( dValue ) * 72;
		return m_dValue;
	}

	//--------------------------------------------------------------------------------
	// UnsignedDecimalNumber 22.9.2.16 (Part 1)
	//--------------------------------------------------------------------------------

	CUnsignedDecimalNumber::CUnsignedDecimalNumber(const CUnsignedDecimalNumber& obj)
	{
		this->m_eValue = obj.m_eValue;
	}	

	CUnsignedDecimalNumber::CUnsignedDecimalNumber(const int& val)
	{
		this->m_eValue = (unsigned int)val;
	}

	unsigned int CUnsignedDecimalNumber::FromString(const std::wstring &sValue)
	{
		try
		{
			this->m_eValue = XmlUtils::GetInteger(sValue);
			return this->m_eValue;
		}
		catch(...)
		{
		}

		try
		{
			this->m_eValue = static_cast<int>(XmlUtils::GetInteger64(sValue));
		}
		catch(...)
		{
			this->m_eValue = 0;
		}

		return this->m_eValue;
	}

	std::wstring CUnsignedDecimalNumber::ToString() const
	{
		return std::to_wstring( this->m_eValue);
	}

	//--------------------------------------------------------------------------------
	// VerticalAlignRun 22.9.2.17 (Part 1)
	//--------------------------------------------------------------------------------

	EVerticalAlignRun CVerticalAlignRun::FromString(const std::wstring &sValue)
	{
		if      ( L"baseline"    == sValue || L"None" == sValue)		this->m_eValue = verticalalignrunBaseline;
		else if ( L"subscript"   == sValue || L"Subscript" == sValue)	this->m_eValue = verticalalignrunSubscript;
		else if ( L"superscript" == sValue || L"Superscript" == sValue)	this->m_eValue = verticalalignrunSuperscript;
		else this->m_eValue = verticalalignrunBaseline;

		return this->m_eValue;
	}

	std::wstring CVerticalAlignRun::ToString() const
	{
		switch(this->m_eValue)
		{
		case verticalalignrunBaseline    : return L"baseline";
		case verticalalignrunSubscript   : return L"subscript";
		case verticalalignrunSuperscript : return L"superscript";
		default                          : return L"baseline";
		}
	}

	//--------------------------------------------------------------------------------
	// XAlign 22.9.2.18 (Part 1)
	//--------------------------------------------------------------------------------

	EXAlign CXAlign::FromString(const std::wstring &sValue)
	{
		if      ( L"center"  == sValue )	this->m_eValue = xalignCenter;
		else if ( L"inside"  == sValue )	this->m_eValue = xalignInside;
		else if ( L"left"    == sValue )	this->m_eValue = xalignLeft;
		else if ( L"outside" == sValue )	this->m_eValue = xalignOutside;
		else if ( L"right"   == sValue )	this->m_eValue = xalignRight;
		else								this->m_eValue = xalignLeft;

		return this->m_eValue;
	}

	std::wstring CXAlign::ToString  () const
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

	//--------------------------------------------------------------------------------
	// YAlign 22.9.2.20 (Part 1)
	//--------------------------------------------------------------------------------

	EYAlign CYAlign::FromString(const std::wstring &sValue)
	{
		if      ( L"bottom"		== sValue ) this->m_eValue = yalignBottom;
		else if ( L"bot"		== sValue ) this->m_eValue = yalignBottom;
		else if ( L"center"		== sValue ) this->m_eValue = yalignCenter;
		else if ( L"inline"		== sValue ) this->m_eValue = yalignInline;
		else if ( L"inside"		== sValue ) this->m_eValue = yalignInside;
		else if ( L"outside"	== sValue ) this->m_eValue = yalignOutside;
		else if ( L"top"		== sValue ) this->m_eValue = yalignTop;
		else								this->m_eValue = yalignTop;

		return this->m_eValue;
	}

	std::wstring CYAlign::ToString  () const
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
} // SimpleTypes

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (15.1.2 - part 4)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// ColorType 15.1.2.3 (Part 4)
	//--------------------------------------------------------------------------------

	void CColorType::SetRGB()
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

	CColorType::CColorType()
	{
		SetRGB();

		std::wstringstream sstream;
		sstream << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
		m_sValue = sstream.str();
	}

	void CColorType::ByHexColor(const std::wstring& sValue)
	{
		this->m_eValue = colortypeRGB;

		CHexColor hexColor;
		hexColor.FromString(sValue);

		m_unR = hexColor.Get_R();
		m_unG = hexColor.Get_G();
		m_unB = hexColor.Get_B();
	}

	void CColorType::ByColorName(const std::wstring& sValue)
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

	EColorType CColorType::FromString(const std::wstring& sValue)
	{
		wchar_t wsFirstChar = 0;
		if (sValue.length() > 0)
			wsFirstChar = sValue[0];

		m_sValue = sValue;
		if (wsFirstChar == '#')
		{
			std::wstring sHexColor = sValue.substr(1);
			ByHexColor(sHexColor);
		}
		else
			ByColorName(sValue);

		return this->m_eValue;
	}

	std::wstring CColorType::ToString() const
	{
		return m_sValue;
	}

	void CColorType::SetRGB(unsigned char unR, unsigned char unG, unsigned char unB)
	{
		this->m_eValue = colortypeRGB;
		m_unR = unR;
		m_unG = unG;
		m_unB = unB;

		std::wstringstream sstream;
		sstream << boost::wformat( L"%02x%02x%02x" ) % m_unR % m_unG % m_unB;
		m_sValue = sstream.str();
	}

	unsigned char CColorType::Get_R() const
	{
		return m_unR;
	}

	unsigned char CColorType::Get_G() const
	{
		return m_unG;
	}

	unsigned char CColorType::Get_B() const
	{
		return m_unB;
	}

	unsigned char CColorType::Get_A() const
	{
		return 255;
	}

	//--------------------------------------------------------------------------------
	// CTrueFalse 15.1.2.5 - 15.1.2.6 (Part 4)
	//--------------------------------------------------------------------------------

	ETrueFalse CTrueFalse::FromString(const std::wstring &sValue)
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

	bool CTrueFalse::GetBool()
	{
		return this->m_eValue == booleanTrue;
	}

	std::wstring CTrueFalse::ToString  () const
	{
		switch(this->m_eValue)
		{
		case booleanFalse : return L"f";
		case booleanTrue  : return L"t";
		default           : return L"f";
		}
	}

	//--------------------------------------------------------------------------------
	// CSdtAppearance
	//--------------------------------------------------------------------------------

	ESdtAppearance CSdtAppearance::FromString(const std::wstring &sValue)
	{
		if      ( L"boundingBox"     == sValue )	this->m_eValue = sdtappearenceBoundingBox;
		else if ( L"tags"  == sValue )	this->m_eValue = sdtappearenceTags;
		else if ( L"hidden"  == sValue )	this->m_eValue = sdtappearenceHidden;

		return this->m_eValue;
	}

	std::wstring CSdtAppearance::ToString  () const
	{
		switch(this->m_eValue)
		{
		case sdtappearenceTags : return L"tags";
		case sdtappearenceHidden  : return L"hidden";
		default           : return L"boundingBox";
		}
	}

} // SimpleTypes

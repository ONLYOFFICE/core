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
#include "../../DesktopEditor/common/Types.h"

// Здесь представлены все простые типы SharedML из спецификации Office Open Xml (22.8)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// RelationshipId 22.8.2.1 (Part 1)
	//--------------------------------------------------------------------------------

	class CRelationshipId
	{
	public:
		CRelationshipId();

		std::wstring GetValue() const;
		void SetValue(const std::wstring &sValue);
		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CRelationshipId)

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

	DEFINE_SIMPLE_TYPE(CCalendarType, ECalendarType, calendartypeNone)

	//--------------------------------------------------------------------------------
	// ConformanceClass 22.9.2.2 (Part 1)
	//--------------------------------------------------------------------------------

	enum EConformanceClass
	{
		conformanceclassStrict       = 0,
		conformanceclassTransitional = 1
	};

	DEFINE_SIMPLE_TYPE(CConformanceClass, EConformanceClass, conformanceclassTransitional)

	//--------------------------------------------------------------------------------
	// Guid 22.9.2.4 (Part 1)
	//--------------------------------------------------------------------------------

	class CGuid
	{
	public:
		CGuid();

		bool FromString(const std::wstring &sValue);
		std::wstring ToString () const;

		SimpleTypes_DefaultString(CGuid)

		bool IsZero();

	private:

		bool HexToInt(std::wstring& sValue, T_ULONG64& unResult);
		int	 HexToInt(int nHex, bool &bResult);

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

	DEFINE_SIMPLE_TYPE_START(CHexColorRGB, int, 0)
	public:
		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

		void Set_RGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255);

	private:
		void Parse(const std::wstring& sValue);
		int	HexToInt(int nHex);

	private:
		unsigned char m_unR = 0;
		unsigned char m_unG = 0;
		unsigned char m_unB = 0;
		unsigned char m_unA = 255;
	};

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

	DEFINE_SIMPLE_TYPE_START(COnOff, EOnOff, onoffFalse)
	public:
		COnOff(const bool & bVal);

		//EOnOff FromStringA(const std::string& sValue);
		//EOnOff FromStringA(const char* sValue);

		std::wstring ToString2(EOnOffToString eType) const;
		std::wstring ToString3(EOnOffToString eType) const;

		bool ToBool();
		void FromBool(bool bVal);
	};

	//--------------------------------------------------------------------------------
	// Bool
	//--------------------------------------------------------------------------------

	class CBool : public COnOff
	{
	public:
		CBool();
		CBool(const bool & bVal);

		virtual std::wstring ToString() const;
		virtual EOnOff FromString(const std::wstring &sValue);
		
		SimpleTypes_Default(CBool)
	};

	//--------------------------------------------------------------------------------
	// Panose 22.9.2.8 (Part 1)
	//--------------------------------------------------------------------------------

	class CPanose
	{
	public:
		CPanose();

		std::wstring GetValue() const;
		void    SetValue(std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CPanose)

		unsigned char Get_Number(int nIndex);

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
		CTwipsMeasure();

		virtual double FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString() const;

		virtual double FromPoints(double dValue);
		virtual double FromInches(double dValue);

		SimpleTypes_DefaultD(CTwipsMeasure)
	};

	//--------------------------------------------------------------------------------
	// UnsignedDecimalNumber 22.9.2.16 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE_START(CUnsignedDecimalNumber, unsigned int, 0)
	public:
		CUnsignedDecimalNumber(const CUnsignedDecimalNumber& obj);
		CUnsignedDecimalNumber(const int& val);
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

	DEFINE_SIMPLE_TYPE(CVerticalAlignRun, EVerticalAlignRun, verticalalignrunBaseline)

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

	DEFINE_SIMPLE_TYPE(CXAlign, EXAlign, xalignLeft)

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

	DEFINE_SIMPLE_TYPE(CYAlign, EYAlign, yalignTop)

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

	DEFINE_SIMPLE_TYPE_START_NC(CColorType, EColorType, colortypeNone)
	public:
		CColorType();

		void SetRGB(unsigned char unR, unsigned char unG, unsigned char unB);

		unsigned char Get_R() const;
		unsigned char Get_G() const;
		unsigned char Get_B() const;
		unsigned char Get_A() const;

	private:
		void ByHexColor(const std::wstring& sValue);
		void SetRGB();
		void ByColorName(const std::wstring& sValue);

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

	DEFINE_SIMPLE_TYPE_START(CTrueFalse, ETrueFalse, booleanFalse)
	public:
		bool GetBool();
	};

	//--------------------------------------------------------------------------------
	// SdtAppearance
	//--------------------------------------------------------------------------------

	enum ESdtAppearance
	{
		sdtappearenceTags = 0,
		sdtappearenceBoundingBox = 1,
		sdtappearenceHidden = 2
	};

	DEFINE_SIMPLE_TYPE(CSdtAppearance, ESdtAppearance, sdtappearenceBoundingBox)

} // SimpleTypes

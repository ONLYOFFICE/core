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
#include "../Base/ASCWinAPI.h"  // GetSysColor
// Здесь представлены все простые типы Drawing-Main из спецификации Office Open Xml (20.1.10)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AdjAngle 20.1.10.1 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAdjAngle
	{
		adjangleAngle = 0,
		adjangleGuide = 1
	};

    template<EAdjAngle eDefValue = adjangleAngle>
    class CAdjAngle : public CSimpleType<EAdjAngle, eDefValue>
	{
	public:

		CAdjAngle() 
		{
			m_nAngle = 0;
		} 

        virtual EAdjAngle FromString(std::wstring &sValue)
		{
			m_nAngle = 0;
            m_sGuide.clear();

			Parse( sValue );

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            if ( adjangleAngle == this->m_eValue )
			{
                return std::to_wstring( m_nAngle );
			}
			else
			{
				return m_sGuide;
			}
		}

		SimpleType_FromString     (EAdjAngle)
		SimpleType_Operator_Equal (CAdjAngle)

		double  GetAngle() const
		{
			return m_nAngle / 60000.0;
		}

        std::wstring GetGuide() const
		{
			return m_sGuide;
		}

	private:

        void Parse(std::wstring &sValue)
		{
			bool bAngleValue = true;

            for ( size_t nIndex = 0; nIndex < sValue.length(); nIndex++ )
			{
				if ( !iswdigit( sValue[nIndex] ) )
				{
					bAngleValue = false;
					break;
				}
			}

			if ( bAngleValue )
			{
                this->m_eValue = adjangleAngle;
				m_nAngle = _wtoi( sValue.c_str() );
			}
			else
			{
                this->m_eValue = adjangleGuide;
				m_sGuide = sValue;
			}
		}


		int				m_nAngle;
        std::wstring	m_sGuide;
	};

	//--------------------------------------------------------------------------------
	// AdjCoordinate 20.1.10.2 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAdjCoordinate
	{
		adjcoordinateCoord = 0,
		adjcoordinateGuide = 1
	};

	template<EAdjCoordinate eDefValue = adjcoordinateCoord>
	class CAdjCoordinate : public CUniversalMeasure
	{
	public:

		CAdjCoordinate() 
		{
            this->m_eValue = eDefValue;
		} 

        virtual double  FromString(std::wstring &sValue)
		{
            m_sGuide.clear();

			Parse2( sValue );

			return m_dValue;
		}

        virtual std::wstring ToString() const
		{
            if ( adjcoordinateCoord == this->m_eValue )
			{
				if ( m_bUnit )
				{
                    return XmlUtils::DoubleToString(m_dValue, L"%.2f") + L"pt";
				}
				else
				{
                    return std::to_wstring( (int)Pt_To_Emu( m_dValue ) );
				}
			}
			else
			{
				return m_sGuide;
			}
		}

		SimpleType_FromString     (double)
		SimpleType_Operator_Equal (CAdjCoordinate)

        std::wstring GetGuide() const
		{
			return m_sGuide;
		}
			
		double GetValue() const
		{
			return m_dValue;
		}
	private:

        void Parse2(std::wstring &sValue)
		{
			Parse( sValue, 12700 );

			bool bGuide = false;
			if ( !m_bUnit )
			{
                for ( size_t nIndex = 0; nIndex < sValue.length(); nIndex++ )
				{
					if ( !iswdigit( sValue[nIndex] ) )
					{
						bGuide = true;
						break;
					}
				}
			}
			else
			{
				// Последние два символа не проверяем
                for ( size_t nIndex = 0; nIndex < sValue.length() - 2; nIndex++ )
				{
					if ( !iswdigit( sValue[nIndex] ) && sValue[nIndex] != '.' && sValue[nIndex] != '-' )
					{
						bGuide = true;
						break;
					}
				}
			}


			if ( bGuide )
			{
                this->m_eValue = adjcoordinateGuide;
				m_sGuide = sValue;
			}
			else
			{
                this->m_eValue = adjcoordinateCoord;
				// Значение хранится в m_dValue
			}
		}

	private:

        EAdjCoordinate      m_eValue;
        std::wstring        m_sGuide;
	};




	//--------------------------------------------------------------------------------
	// Angle 20.1.10.3 (Part 1)
	//--------------------------------------------------------------------------------

	template<int nDefValue = 0>
	class CAngle : public CSimpleType<int, nDefValue>
	{
	public:

		CAngle() {}

        virtual int     FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi( sValue.c_str() );

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            return std::to_wstring( this->m_eValue );
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CAngle)

		double  GetAngle() const
		{
            return this->m_eValue / 60000.0;
		}

	};

	//--------------------------------------------------------------------------------
	// AnimationBuildType 20.1.10.4 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EAnimationBuildType
	{
		animationbuildtypeAllAtOnce = 0
	};

	template<EAnimationBuildType eDefValue = animationbuildtypeAllAtOnce>
	class CAnimationBuildType : public CSimpleType<EAnimationBuildType, eDefValue>
	{
	public:
		CAnimationBuildType() {}

        virtual EAnimationBuildType FromString(std::wstring &sValue)
		{
            if      ( (L"allAtOnce") == sValue ) this->m_eValue = animationbuildtypeAllAtOnce;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring             ToString  () const
		{
            switch(this->m_eValue)
			{
			case animationbuildtypeAllAtOnce : return (L"allAtOnce");
			default                          : return (L"allAtOnce");
			}
		}

		SimpleType_FromString     (EAnimationBuildType)
		SimpleType_Operator_Equal (CAnimationBuildType)
	};

	//--------------------------------------------------------------------------------
	// AnimationChartBuildType 20.1.10.5 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EAnimationChartBuildType
	{
		animationchartbuildtypeAllAtOnce  = 0,
		animationchartbuildtypeCategory   = 1,
		animationchartbuildtypeCategoryEl = 2,
		animationchartbuildtypeSeries     = 3,
		animationchartbuildtypeSeriesEl   = 4
	};

	template<EAnimationChartBuildType eDefValue = animationchartbuildtypeAllAtOnce>
	class CAnimationChartBuildType : public CSimpleType<EAnimationChartBuildType, eDefValue>
	{
	public:
		CAnimationChartBuildType() {}

        virtual EAnimationChartBuildType FromString(std::wstring &sValue)
		{
            if      ( (L"allAtOnce")  == sValue ) this->m_eValue = animationchartbuildtypeAllAtOnce;
            else if ( (L"category")   == sValue ) this->m_eValue = animationchartbuildtypeCategory;
            else if ( (L"categoryEl") == sValue ) this->m_eValue = animationchartbuildtypeCategoryEl;
            else if ( (L"series")     == sValue ) this->m_eValue = animationchartbuildtypeSeries;
            else if ( (L"seriesEl")   == sValue ) this->m_eValue = animationchartbuildtypeSeriesEl;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring                  ToString  () const
		{
            switch(this->m_eValue)
			{
			case animationchartbuildtypeAllAtOnce  : return (L"allAtOnce");
			case animationchartbuildtypeCategory   : return (L"category");
			case animationchartbuildtypeCategoryEl : return (L"categoryEl");
			case animationchartbuildtypeSeries     : return (L"series");
			case animationchartbuildtypeSeriesEl   : return (L"seriesEl");
			default                                : return (L"allAtOnce");
			}
		}

		SimpleType_FromString     (EAnimationChartBuildType)
		SimpleType_Operator_Equal (CAnimationChartBuildType)
	};



	//--------------------------------------------------------------------------------
	// AnimationChartOnlyBuildType 20.1.10.6 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EAnimationChartOnlyBuildType
	{
		animationchartonlybuildtypeCategory   = 0,
		animationchartonlybuildtypeCategoryEl = 1,
		animationchartonlybuildtypeSeries     = 2,
		animationchartonlybuildtypeSeriesEl   = 3
	};

	template<EAnimationChartOnlyBuildType eDefValue = animationchartonlybuildtypeCategory>
	class CAnimationChartOnlyBuildType : public CSimpleType<EAnimationChartOnlyBuildType, eDefValue>
	{
	public:
		CAnimationChartOnlyBuildType() {}

        virtual EAnimationChartOnlyBuildType FromString(std::wstring &sValue)
		{
            if      ( (L"category")   == sValue ) this->m_eValue = animationchartonlybuildtypeCategory;
            else if ( (L"categoryEl") == sValue ) this->m_eValue = animationchartonlybuildtypeCategoryEl;
            else if ( (L"series")     == sValue ) this->m_eValue = animationchartonlybuildtypeSeries;
            else if ( (L"seriesEl")   == sValue ) this->m_eValue = animationchartonlybuildtypeSeriesEl;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring                      ToString  () const
		{
            switch(this->m_eValue)
			{
			case animationchartonlybuildtypeCategory   : return (L"category");
			case animationchartonlybuildtypeCategoryEl : return (L"categoryEl");
			case animationchartonlybuildtypeSeries     : return (L"series");
			case animationchartonlybuildtypeSeriesEl   : return (L"seriesEl");
			default                                    : return (L"category");
			}
		}

		SimpleType_FromString     (EAnimationChartOnlyBuildType)
		SimpleType_Operator_Equal (CAnimationChartOnlyBuildType)
	};



	//--------------------------------------------------------------------------------
	// AnimationDgmBuildType 20.1.10.7 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EAnimationDgmBuildType
	{
		animationdgmbuildtypeAllAtOnce = 0,
		animationdgmbuildtypeLvlAtOnce = 1,
		animationdgmbuildtypeLvlOne    = 2,
		animationdgmbuildtypeOne       = 3
	};

	template<EAnimationDgmBuildType eDefValue = animationdgmbuildtypeAllAtOnce>
	class CAnimationDgmBuildType : public CSimpleType<EAnimationDgmBuildType, eDefValue>
	{
	public:
		CAnimationDgmBuildType() {}

        virtual EAnimationDgmBuildType FromString(std::wstring &sValue)
		{
            if      ( (L"allAtOnce") == sValue ) this->m_eValue = animationdgmbuildtypeAllAtOnce;
            else if ( (L"lvlAtOnce") == sValue ) this->m_eValue = animationdgmbuildtypeLvlAtOnce;
            else if ( (L"lvlOne")    == sValue ) this->m_eValue = animationdgmbuildtypeLvlOne;
            else if ( (L"one")       == sValue ) this->m_eValue = animationdgmbuildtypeOne;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring                ToString  () const
		{
            switch(this->m_eValue)
			{
			case animationdgmbuildtypeAllAtOnce : return (L"allAtOnce");
			case animationdgmbuildtypeLvlAtOnce : return (L"lvlAtOnce");
			case animationdgmbuildtypeLvlOne    : return (L"lvlOne");
			case animationdgmbuildtypeOne       : return (L"one");
			default                             : return (L"allAtOnce");
			}
		}

		SimpleType_FromString     (EAnimationDgmBuildType)
		SimpleType_Operator_Equal (CAnimationDgmBuildType)
	};




	//--------------------------------------------------------------------------------
	// AnimationDgmOnlyBuildType 20.1.10.8 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EAnimationDgmOnlyBuildType
	{
		animationdgmonlybuildtypeLvlAtOnce = 0,
		animationdgmonlybuildtypeLvlOne    = 1,
		animationdgmonlybuildtypeOne       = 2
	};

	template<EAnimationDgmOnlyBuildType eDefValue = animationdgmonlybuildtypeOne>
	class CAnimationDgmOnlyBuildType : public CSimpleType<EAnimationDgmOnlyBuildType, eDefValue>
	{
	public:
		CAnimationDgmOnlyBuildType() {}

        virtual EAnimationDgmOnlyBuildType FromString(std::wstring &sValue)
		{
            if      ( (L"lvlAtOnce") == sValue ) this->m_eValue = animationdgmonlybuildtypeLvlAtOnce;
            else if ( (L"lvlOne")    == sValue ) this->m_eValue = animationdgmonlybuildtypeLvlOne;
            else if ( (L"one")       == sValue ) this->m_eValue = animationdgmonlybuildtypeOne;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring                    ToString  () const
		{
            switch(this->m_eValue)
			{
			case animationdgmonlybuildtypeLvlAtOnce : return (L"lvlAtOnce");
			case animationdgmonlybuildtypeLvlOne    : return (L"lvlOne");
			case animationdgmonlybuildtypeOne       : return (L"one");
			default                                 : return (L"lvlAtOnce");
			}
		}

		SimpleType_FromString     (EAnimationDgmOnlyBuildType)
		SimpleType_Operator_Equal (CAnimationDgmOnlyBuildType)
	};
	//--------------------------------------------------------------------------------
	// BevelPresetType 20.1.10.9 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EBevelPresetType
	{
		bevelpresettypeAngle        =  0,
		bevelpresettypeArtDeco      =  1,
		bevelpresettypeCircle       =  2,
		bevelpresettypeConvex       =  3,
		bevelpresettypeCoolSlant    =  4,
		bevelpresettypeCross        =  5,
		bevelpresettypeDivot        =  6,
		bevelpresettypeHardEdge     =  7,
		bevelpresettypeRelaxedInset =  8,
		bevelpresettypeRiblet       =  9,
		bevelpresettypeSlope        = 10,
		bevelpresettypeSoftRound    = 11
	};

	template<EBevelPresetType eDefValue = bevelpresettypeAngle>
	class CBevelPresetType : public CSimpleType<EBevelPresetType, eDefValue>
	{
	public:
		CBevelPresetType() {}

        virtual EBevelPresetType FromString(std::wstring &sValue)
		{
            if      ( (L"angle")        == sValue ) this->m_eValue = bevelpresettypeAngle;
            else if ( (L"artDeco")      == sValue ) this->m_eValue = bevelpresettypeArtDeco;
            else if ( (L"circle")       == sValue ) this->m_eValue = bevelpresettypeCircle;
            else if ( (L"convex")       == sValue ) this->m_eValue = bevelpresettypeConvex;
            else if ( (L"coolSlant")    == sValue ) this->m_eValue = bevelpresettypeCoolSlant;
            else if ( (L"cross")        == sValue ) this->m_eValue = bevelpresettypeCross;
            else if ( (L"divot")        == sValue ) this->m_eValue = bevelpresettypeDivot;
            else if ( (L"hardEdge")     == sValue ) this->m_eValue = bevelpresettypeHardEdge;
            else if ( (L"relaxedInset") == sValue ) this->m_eValue = bevelpresettypeRelaxedInset;
            else if ( (L"riblet")       == sValue ) this->m_eValue = bevelpresettypeRiblet;
            else if ( (L"slope")        == sValue ) this->m_eValue = bevelpresettypeSlope;
            else if ( (L"softRound")    == sValue ) this->m_eValue = bevelpresettypeSoftRound;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(this->m_eValue)
			{
			case bevelpresettypeAngle        : return (L"angle");
			case bevelpresettypeArtDeco      : return (L"artDeco");
			case bevelpresettypeCircle       : return (L"circle");
			case bevelpresettypeConvex       : return (L"convex");
			case bevelpresettypeCoolSlant    : return (L"coolSlant");
			case bevelpresettypeCross        : return (L"cross");
			case bevelpresettypeDivot        : return (L"divot");
			case bevelpresettypeHardEdge     : return (L"hardEdge");
			case bevelpresettypeRelaxedInset : return (L"relaxedInset");
			case bevelpresettypeRiblet       : return (L"riblet");
			case bevelpresettypeSlope        : return (L"slope");
			case bevelpresettypeSoftRound    : return (L"softRound");
			default                          : return (L"angle");
			}
		}

		SimpleType_FromString     (EBevelPresetType)
		SimpleType_Operator_Equal (CBevelPresetType)
	};

	//--------------------------------------------------------------------------------
	// BlackWhiteMode 20.1.10.10 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EBlackWhiteMode
	{
		blackwhitemodeAuto       =  0,
		blackwhitemodeBlack      =  1,
		blackwhitemodeBlackGray  =  2,
		blackwhitemodeBlackWhite =  3,
		blackwhitemodeClr        =  4,
		blackwhitemodeGray       =  5,
		blackwhitemodeGrayWhite  =  6,
		blackwhitemodeHidden     =  7,
		blackwhitemodeInvGray    =  8,
		blackwhitemodeLtGray     =  9,
		blackwhitemodeWhite      = 10
	};

	template<EBlackWhiteMode eDefValue = blackwhitemodeAuto>
	class CBlackWhiteMode : public CSimpleType<EBlackWhiteMode, eDefValue>
	{
	public:
		CBlackWhiteMode() {}

        virtual EBlackWhiteMode FromString(std::wstring &sValue)
		{
            if      ( (L"auto")       == sValue ) this->m_eValue = blackwhitemodeAuto;
            else if ( (L"black")      == sValue ) this->m_eValue = blackwhitemodeBlack;
            else if ( (L"blackGray")  == sValue ) this->m_eValue = blackwhitemodeBlackGray;
            else if ( (L"blackWhite") == sValue ) this->m_eValue = blackwhitemodeBlackWhite;
            else if ( (L"clr")        == sValue ) this->m_eValue = blackwhitemodeClr;
            else if ( (L"gray")       == sValue ) this->m_eValue = blackwhitemodeGray;
            else if ( (L"grayWhite")  == sValue ) this->m_eValue = blackwhitemodeGrayWhite;
            else if ( (L"hidden")     == sValue ) this->m_eValue = blackwhitemodeHidden;
            else if ( (L"invGray")    == sValue ) this->m_eValue = blackwhitemodeInvGray;
            else if ( (L"ltGray")     == sValue ) this->m_eValue = blackwhitemodeLtGray;
            else if ( (L"white")      == sValue ) this->m_eValue = blackwhitemodeWhite;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(this->m_eValue)
			{
			case blackwhitemodeAuto       : return (L"auto");
			case blackwhitemodeBlack      : return (L"black");
			case blackwhitemodeBlackGray  : return (L"blackGray");
			case blackwhitemodeBlackWhite : return (L"blackWhite");
			case blackwhitemodeClr        : return (L"clr");
			case blackwhitemodeGray       : return (L"gray");
			case blackwhitemodeGrayWhite  : return (L"grayWhite");
			case blackwhitemodeHidden     : return (L"hidden");
			case blackwhitemodeInvGray    : return (L"invGray");
			case blackwhitemodeLtGray     : return (L"ltGray");
			case blackwhitemodeWhite      : return (L"white");
			default                       : return (L"auto");
			}
		}

		SimpleType_FromString     (EBlackWhiteMode)
		SimpleType_Operator_Equal (CBlackWhiteMode)
	};

	//--------------------------------------------------------------------------------
	// BlendMode 20.1.10.11 (Part 1)
	//--------------------------------------------------------------------------------		

	enum EBlendMode
	{
		blendmodeDarken  = 0,
		blendmodeLighten = 1,
		blendmodeMult    = 2,
		blendmodeOver    = 3,
		blendmodeScreen  = 4
	};

	template<EBlendMode eDefValue = blendmodeMult>
	class CBlendMode : public CSimpleType<EBlendMode, eDefValue>
	{
	public:
		CBlendMode() {}

        virtual EBlendMode FromString(std::wstring &sValue)
		{
            if      ( (L"darken")  == sValue ) this->m_eValue = blendmodeDarken;
            else if ( (L"lighten") == sValue ) this->m_eValue = blendmodeLighten;
            else if ( (L"mult")    == sValue ) this->m_eValue = blendmodeMult;
            else if ( (L"over")    == sValue ) this->m_eValue = blendmodeOver;
            else if ( (L"screen")  == sValue ) this->m_eValue = blendmodeScreen;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(this->m_eValue)
			{
			case blendmodeDarken  : return (L"darken");
			case blendmodeLighten : return (L"lighten");
			case blendmodeMult    : return (L"mult");
			case blendmodeOver    : return (L"over");
			case blendmodeScreen  : return (L"screen");
			default               : return (L"mult");
			}
		}

		SimpleType_FromString     (EBlendMode)
		SimpleType_Operator_Equal (CBlendMode)
	};


	//--------------------------------------------------------------------------------
	// BlipCompression 20.1.10.12 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EBlipCompression
	{
		blipcompressionEmail   = 0,
		blipcompressionHQPrint = 1,
		blipcompressionNone    = 2,
		blipcompressionPrint   = 3,
		blipcompressionScreen  = 4
	};

	template<EBlipCompression eDefValue = blipcompressionNone>
	class CBlipCompression : public CSimpleType<EBlipCompression, eDefValue>
	{
	public:
		CBlipCompression() {}

        virtual EBlipCompression FromString(std::wstring &sValue)
		{
            if      ( (L"email")   == sValue ) this->m_eValue = blipcompressionEmail;
            else if ( (L"hqprint") == sValue ) this->m_eValue = blipcompressionHQPrint;
            else if ( (L"none")    == sValue ) this->m_eValue = blipcompressionNone;
            else if ( (L"print")   == sValue ) this->m_eValue = blipcompressionPrint;
            else if ( (L"screen")  == sValue ) this->m_eValue = blipcompressionScreen;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(this->m_eValue)
			{
			case blipcompressionEmail   : return (L"email");
			case blipcompressionHQPrint : return (L"hqprint");
			case blipcompressionNone    : return (L"none");
			case blipcompressionPrint   : return (L"print");
			case blipcompressionScreen  : return (L"screen");
			default                     : return (L"none");
			}
		}

		SimpleType_FromString     (EBlipCompression)
		SimpleType_Operator_Equal (CBlipCompression)
	};

	//--------------------------------------------------------------------------------
	// ColorSchemeIndex 20.1.10.14 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EColorSchemeIndex
	{
		colorschemeindexAccent1  = 0,
		colorschemeindexAccent2  = 1,
		colorschemeindexAccent3  = 2,
		colorschemeindexAccent4  = 3,
		colorschemeindexAccent5  = 4,
		colorschemeindexAccent6  = 5,
		colorschemeindexDk1      = 6,
		colorschemeindexDk2      = 7,
		colorschemeindexFolHlink = 8,
		colorschemeindexHlink    = 9,
		colorschemeindexLt1      = 10,
		colorschemeindexLt2      = 11,
	};

	template<EColorSchemeIndex eDefValue = colorschemeindexAccent1>
	class CColorSchemeIndex : public CSimpleType<EColorSchemeIndex, eDefValue>
	{
	public:
		CColorSchemeIndex() {}

        virtual EColorSchemeIndex FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':
                if      ( (L"accent1")  == sValue ) this->m_eValue = colorschemeindexAccent1;
                else if ( (L"accent2")  == sValue ) this->m_eValue = colorschemeindexAccent2;
                else if ( (L"accent3")  == sValue ) this->m_eValue = colorschemeindexAccent3;
                else if ( (L"accent4")  == sValue ) this->m_eValue = colorschemeindexAccent4;
                else if ( (L"accent5")  == sValue ) this->m_eValue = colorschemeindexAccent5;
                else if ( (L"accent6")  == sValue ) this->m_eValue = colorschemeindexAccent6;
				break;
			case 'd':
                if      ( (L"dk1")      == sValue ) this->m_eValue = colorschemeindexDk1;
                else if ( (L"dk2")      == sValue ) this->m_eValue = colorschemeindexDk2;
				break;
			case 'f':
                if      ( (L"folHlink") == sValue ) this->m_eValue = colorschemeindexFolHlink;
				break;
			case 'h':
                if      ( (L"hlink")    == sValue ) this->m_eValue = colorschemeindexHlink;
				break;
			case 'l':
                if      ( (L"lt1")      == sValue ) this->m_eValue = colorschemeindexLt1;
                else if ( (L"lt2")      == sValue ) this->m_eValue = colorschemeindexLt2;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case colorschemeindexAccent1 : return (L"accent1");			
			case colorschemeindexAccent2 : return (L"accent2");			
			case colorschemeindexAccent3 : return (L"accent3");			
			case colorschemeindexAccent4 : return (L"accent4");			
			case colorschemeindexAccent5 : return (L"accent5");			
			case colorschemeindexAccent6 : return (L"accent6");			
			case colorschemeindexDk1     : return (L"dk1");			
			case colorschemeindexDk2     : return (L"dk2");			
			case colorschemeindexFolHlink: return (L"folHlink");			
			case colorschemeindexHlink   : return (L"hlink");			
			case colorschemeindexLt1     : return (L"lt1");			
			case colorschemeindexLt2     : return (L"lt2");			
			default                      : return (L"accent1");
			}
		}

		SimpleType_FromString     (EColorSchemeIndex)
		SimpleType_Operator_Equal (CColorSchemeIndex)
	};
	//--------------------------------------------------------------------------------
	// CompoundLine 20.1.10.15 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ECompoundLine
	{
		compoundlineDbl       = 0,
		compoundlineSng       = 1,
		compoundlineThickThin = 2,
		compoundlineThinThick = 3,
		compoundlineTri       = 4,
	};

	template<ECompoundLine eDefValue = compoundlineSng>
	class CCompoundLine : public CSimpleType<ECompoundLine, eDefValue>
	{
	public:
		CCompoundLine() {}

        virtual ECompoundLine FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'd':
                if      ( (L"dbl")       == sValue ) this->m_eValue = compoundlineDbl;
				break;
			case 's':
                if      ( (L"sng")       == sValue ) this->m_eValue = compoundlineSng;
				break;
			case 't':
                if      ( (L"thickThin") == sValue ) this->m_eValue = compoundlineThickThin;
                else if ( (L"thinThick") == sValue ) this->m_eValue = compoundlineThinThick;
                else if ( (L"tri")       == sValue ) this->m_eValue = compoundlineTri;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case compoundlineDbl:       return (L"dbl");			
			case compoundlineSng:       return (L"sng");			
			case compoundlineThickThin: return (L"thickThin");			
			case compoundlineThinThick: return (L"thinThick");			
			case compoundlineTri:       return (L"tri");			
			default :                   return (L"sng");
			}
		}

		SimpleType_FromString     (ECompoundLine)
		SimpleType_Operator_Equal (CCompoundLine)
	};
	//--------------------------------------------------------------------------------
    // Coordinate 20.1.10.16 (Part 1)
	//--------------------------------------------------------------------------------
    class CCoordinate : public CUniversalMeasure
    {
    public:
        CCoordinate() {}

 		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
			m_bUnit  = false;
			m_dValue = dValue;
		}

        virtual double  FromString(std::wstring &sValue)
        {
            Parse(sValue, 12700);

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult;

            if ( m_bUnit )
                sResult = boost::lexical_cast<std::wstring>(m_dValue) + L"pt";
            else
                sResult = std::to_wstring( (__int64)m_dValue );

            return sResult;
        }

		__int64 ToEmu() const
		{
			return (__int64)Pt_To_Emu( m_dValue );
		}
 		double ToCm() const
		{
			return Pt_To_Cm( m_dValue );
		}
		SimpleType_FromString     (double)
        SimpleType_Operator_Equal (CCoordinate)
    };

	//--------------------------------------------------------------------------------
    // Coordinate32 20.1.10.17 (Part 1)
	//--------------------------------------------------------------------------------
	#define	CCoordinate32 CCoordinate
	//--------------------------------------------------------------------------------
	// DrawingElementId 20.1.10.21 (Part 1)
	//--------------------------------------------------------------------------------
    template<int nDefValue = 0>
	class CDrawingElementId : public CSimpleType<int, nDefValue>
    {
    public:
        CDrawingElementId() {}

        virtual int     FromString(std::wstring &sValue)
        {
            this->m_eValue = _wtoi( sValue.c_str() );

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString2      (int)
            SimpleType_Operator_Equal   (CDrawingElementId)
    };

	//--------------------------------------------------------------------------------
	// EffectContainerType 20.1.10.22 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EEffectContainerType
	{
		effectcontainertypeSib  = 0,
		effectcontainertypeTree = 1,
	};

	template<EEffectContainerType eDefValue = effectcontainertypeSib>
	class CEffectContainerType : public CSimpleType<EEffectContainerType, eDefValue>
	{
	public:
		CEffectContainerType() {}

        virtual EEffectContainerType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 's':
                if      ( (L"sib")  == sValue ) this->m_eValue = effectcontainertypeSib;
				break;
			case 't':
                if      ( (L"tree") == sValue ) this->m_eValue = effectcontainertypeTree;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(this->m_eValue)
			{
			case effectcontainertypeSib:  return (L"sib");			
			case effectcontainertypeTree: return (L"tree");		
			default :                     return (L"sib");
			}
		}

		SimpleType_FromString     (EEffectContainerType)
		SimpleType_Operator_Equal (CEffectContainerType)
	};


	//--------------------------------------------------------------------------------
	// FixedAngle 20.1.10.23 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CFixedAngle : public CSimpleType<int, nDefValue>
	{
	public:

		CFixedAngle() {}

		void SetValue(int nValue)
		{
            this->m_eValue = (std::min)( 5400000, (std::max)( -5400000, nValue ) );
		}

        virtual int     FromString(std::wstring &sValue)
		{
            this->m_eValue = (std::min)( 5400000, (std::max)( -5400000, _wtoi( sValue.c_str() ) ) );

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            std::wstring sResult = std::to_wstring( this->m_eValue );
			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CFixedAngle)

		double  GetAngle() const
		{
            return this->m_eValue / 60000.0;
		}

	};

	//--------------------------------------------------------------------------------
    // FixedPercentage 20.1.10.24 (Part 1) + 12.1.2.1 (Part4)
	//--------------------------------------------------------------------------------    
    class CFixedPercentage 
    {
    public:
        CFixedPercentage() 
		{
			m_dValue = 0;
		}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
            m_dValue = (std::min)( 100.0, (std::max)( -100.0, dValue ) );
		}

        virtual double FromString(std::wstring &sValue)
        {
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos && nLen > 0)
				{
					// Поправка 12.1.2.1 Part4
                    int nValue = (std::min)( 100000, (std::max)( -100000, _wtoi( sValue.c_str() ) ) );
					m_dValue = nValue / 1000.0;
				}
				else
					m_dValue = 0;
			}
			else
                m_dValue = (std::min)( 100.0, (std::max)( -100.0, _wtof( sValue.substr( 0, nLen - 1 ).c_str() ) ) );

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
			return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CFixedPercentage)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
	// FontCollectionIndex 20.1.10.25 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EFontCollectionIndex
	{
		fontcollectionindexMajor = 0,
		fontcollectionindexMinor = 1,
		fontcollectionindexNone  = 2,
	};

	template<EFontCollectionIndex eDefValue = fontcollectionindexNone>
	class CFontCollectionIndex : public CSimpleType<EFontCollectionIndex, eDefValue>
	{
	public:
		CFontCollectionIndex() {}

        virtual EFontCollectionIndex FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'm':
                if      ( (L"major") == sValue ) this->m_eValue = fontcollectionindexMajor;
                else if ( (L"minor") == sValue ) this->m_eValue = fontcollectionindexMinor;
				break;
			case 'n':
                if      ( (L"none")  == sValue ) this->m_eValue = fontcollectionindexNone;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(this->m_eValue)
			{
			case fontcollectionindexMajor: return (L"major");			
			case fontcollectionindexMinor: return (L"minor");		
			case fontcollectionindexNone : return (L"none");		
			default                      : return (L"none");
			}
		}

		SimpleType_FromString     (EFontCollectionIndex)
		SimpleType_Operator_Equal (CFontCollectionIndex)
	};


	//--------------------------------------------------------------------------------
	// FOVAngle 20.1.10.26 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CFOVAngle : public CSimpleType<int, nDefValue>
	{
	public:

		CFOVAngle() {}

		void SetValue(int nValue)
		{
            this->m_eValue = (std::min)( 10800000, (std::max)( 0, nValue ) );
		}

        virtual int     FromString(std::wstring &sValue)
		{
            this->m_eValue = (std::min)( 10800000, (std::max)( 0, _wtoi( sValue.c_str() ) ) );

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            std::wstring sResult = std::to_wstring( this->m_eValue );
			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CFOVAngle)

		double  GetAngle() const
		{
            return this->m_eValue / 60000.0;
		}

	};

	//--------------------------------------------------------------------------------
	// GeomGuideFormula 20.1.10.27 (Part 1)
	//--------------------------------------------------------------------------------		
	class CGeomGuideFormula
	{
	public:
		CGeomGuideFormula() {}

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
		SimpleType_Operator_Equal (CGeomGuideFormula)

	private:

		void Parse()
		{
			// TO DO: Сделать разбор формул. См. стр.3244 Part1.
		}

	private:

        std::wstring m_sValue;
	};
	//--------------------------------------------------------------------------------
	// GeomGuideName 20.1.10.28 (Part 1)
	//--------------------------------------------------------------------------------		
	class CGeomGuideName
	{
	public:
		CGeomGuideName() {}

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
		SimpleType_Operator_Equal (CGeomGuideName)

	private:

        std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// LightRigDirection 20.1.10.29 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELightRigDirection
	{
		lightrigdirectionB  = 0,
		lightrigdirectionBL = 1,
		lightrigdirectionBR = 2,
		lightrigdirectionL  = 3,
		lightrigdirectionR  = 4,
		lightrigdirectionT  = 5,
		lightrigdirectionTL = 6,
		lightrigdirectionTR = 7,
	};

	template<ELightRigDirection eDefValue = lightrigdirectionTR>
	class CLightRigDirection : public CSimpleType<ELightRigDirection, eDefValue>
	{
	public:
		CLightRigDirection() {}

        virtual ELightRigDirection FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'b':
                if      ( (L"b")  == sValue ) this->m_eValue = lightrigdirectionB;
                else if ( (L"bl") == sValue ) this->m_eValue = lightrigdirectionBL;
                else if ( (L"br") == sValue ) this->m_eValue = lightrigdirectionBR;
				break;
			case 'l':
                if      ( (L"l")  == sValue ) this->m_eValue = lightrigdirectionL;
				break;
			case 'r':
                if      ( (L"r")  == sValue ) this->m_eValue = lightrigdirectionR;
				break;
			case 't':
                if      ( (L"t")  == sValue ) this->m_eValue = lightrigdirectionT;
                else if ( (L"tl") == sValue ) this->m_eValue = lightrigdirectionTL;
                else if ( (L"tr") == sValue ) this->m_eValue = lightrigdirectionTR;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(this->m_eValue)
			{
			case lightrigdirectionB:  return (L"b");			
			case lightrigdirectionBL: return (L"bl");			
			case lightrigdirectionBR: return (L"br");		
			case lightrigdirectionL:  return (L"l");			
			case lightrigdirectionR:  return (L"r");			
			case lightrigdirectionT:  return (L"t");		
			case lightrigdirectionTL: return (L"tl");			
			case lightrigdirectionTR: return (L"tr");			
			default :                 return (L"tr");
			}
		}

		SimpleType_FromString     (ELightRigDirection)
		SimpleType_Operator_Equal (CLightRigDirection)
	};
	//--------------------------------------------------------------------------------
	// LightRigType 20.1.10.30 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELightRigType
	{
		lightrigtypeBalanced      = 0,
		lightrigtypeBrightRoom    = 1,
		lightrigtypeChilly        = 2,
		lightrigtypeContrasting   = 3,
		lightrigtypeFlat          = 4,
		lightrigtypeFlood         = 5,
		lightrigtypeFreezing      = 6,
		lightrigtypeGlow          = 7,
		lightrigtypeHarsh         = 8,
		lightrigtypeLegacyFlat1   = 9,
		lightrigtypeLegacyFlat2   = 10,
		lightrigtypeLegacyFlat3   = 11,
		lightrigtypeLegacyFlat4   = 12,
		lightrigtypeLegacyHarsh1  = 13,
		lightrigtypeLegacyHarsh2  = 14,
		lightrigtypeLegacyHarsh3  = 15,
		lightrigtypeLegacyHarsh4  = 16,
		lightrigtypeLegacyNormal1 = 17,
		lightrigtypeLegacyNormal2 = 18,
		lightrigtypeLegacyNormal3 = 19,
		lightrigtypeLegacyNormal4 = 20,
		lightrigtypeMorning       = 21,
		lightrigtypeSoft          = 22,
		lightrigtypeSunrise       = 23,
		lightrigtypeSunset        = 24,
		lightrigtypeThreePt       = 25,
		lightrigtypeTwoPt         = 26,
	};

	template<ELightRigType eDefValue = lightrigtypeBalanced>
	class CLightRigType : public CSimpleType<ELightRigType, eDefValue>
	{
	public:
		CLightRigType() {}

        virtual ELightRigType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'b':
                if      ( (L"balanced")    == sValue ) this->m_eValue = lightrigtypeBalanced;
                else if ( (L"brightRoom")  == sValue ) this->m_eValue = lightrigtypeBrightRoom;
				break;
			case 'c':
                if      ( (L"chilly")      == sValue ) this->m_eValue = lightrigtypeChilly;
                else if ( (L"contrasting") == sValue ) this->m_eValue = lightrigtypeContrasting;
				break;
			case 'f':
                if      ( (L"flat")        == sValue ) this->m_eValue = lightrigtypeFlat;
                else if ( (L"flood")       == sValue ) this->m_eValue = lightrigtypeFlood;
                else if ( (L"freezing")    == sValue ) this->m_eValue = lightrigtypeFreezing;
				break;
			case 'g':
                if      ( (L"glow")        == sValue ) this->m_eValue = lightrigtypeGlow;
				break;
			case 'h':
                if      ( (L"harsh")       == sValue ) this->m_eValue = lightrigtypeHarsh;
				break;
			case 'l':
                if      ( (L"legacyFlat1")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat1;
                else if ( (L"legacyFlat2")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat2;
                else if ( (L"legacyFlat3")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat3;
                else if ( (L"legacyFlat4")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat4;
                else if ( (L"legacyHarsh1")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh1;
                else if ( (L"legacyHarsh2")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh2;
                else if ( (L"legacyHarsh3")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh3;
                else if ( (L"legacyHarsh4")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh4;
                else if ( (L"legacyNormal1") == sValue ) this->m_eValue = lightrigtypeLegacyNormal1;
                else if ( (L"legacyNormal2") == sValue ) this->m_eValue = lightrigtypeLegacyNormal2;
                else if ( (L"legacyNormal3") == sValue ) this->m_eValue = lightrigtypeLegacyNormal3;
                else if ( (L"legacyNormal4") == sValue ) this->m_eValue = lightrigtypeLegacyNormal4;
				break;
			case 'm':
                if      ( (L"morning") == sValue ) this->m_eValue = lightrigtypeMorning;
				break;
			case 's':
                if      ( (L"soft")    == sValue ) this->m_eValue = lightrigtypeSoft;
                else if ( (L"sunrise") == sValue ) this->m_eValue = lightrigtypeSunrise;
                else if ( (L"sunset")  == sValue ) this->m_eValue = lightrigtypeSunset;
				break;
			case 't':
                if      ( (L"threePt") == sValue ) this->m_eValue = lightrigtypeThreePt;
                else if ( (L"twoPt")   == sValue ) this->m_eValue = lightrigtypeTwoPt;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case lightrigtypeBalanced      : return (L"balanced");
			case lightrigtypeBrightRoom    : return (L"brightRoom");
			case lightrigtypeChilly        : return (L"chilly");
			case lightrigtypeContrasting   : return (L"contrasting");
			case lightrigtypeFlat          : return (L"flat"); 
			case lightrigtypeFlood         : return (L"flood");
			case lightrigtypeFreezing      : return (L"freezing");
			case lightrigtypeGlow          : return (L"glow");
			case lightrigtypeHarsh         : return (L"harsh");
			case lightrigtypeLegacyFlat1   : return (L"legacyFlat1");
			case lightrigtypeLegacyFlat2   : return (L"legacyFlat2");
			case lightrigtypeLegacyFlat3   : return (L"legacyFlat3"); 
			case lightrigtypeLegacyFlat4   : return (L"legacyFlat4");
			case lightrigtypeLegacyHarsh1  : return (L"legacyHarsh1");
			case lightrigtypeLegacyHarsh2  : return (L"legacyHarsh2");
			case lightrigtypeLegacyHarsh3  : return (L"legacyHarsh3");
			case lightrigtypeLegacyHarsh4  : return (L"legacyHarsh4");
			case lightrigtypeLegacyNormal1 : return (L"legacyNormal1");
			case lightrigtypeLegacyNormal2 : return (L"legacyNormal2"); 
			case lightrigtypeLegacyNormal3 : return (L"legacyNormal3");
			case lightrigtypeLegacyNormal4 : return (L"legacyNormal4"); 
			case lightrigtypeMorning       : return (L"morning");
			case lightrigtypeSoft          : return (L"soft"); 
			case lightrigtypeSunrise       : return (L"sunrise"); 
			case lightrigtypeSunset        : return (L"sunset");
			case lightrigtypeThreePt       : return (L"threePt"); 
			case lightrigtypeTwoPt         : return (L"twoPt");			
			default                        : return (L"balanced");
			}
		}

		SimpleType_FromString     (ELightRigType)
		SimpleType_Operator_Equal (CLightRigType)
	};
	//--------------------------------------------------------------------------------
	// LineCap 20.1.10.31 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELineCap
	{
		linecapFlat = 0,
		linecapRnd  = 1,
		linecapSq   = 2,
	};

	template<ELineCap eDefValue = linecapRnd>
	class CLineCap : public CSimpleType<ELineCap, eDefValue>
	{
	public:
		CLineCap() {}

        virtual ELineCap FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'f':
                if      ( (L"flat") == sValue ) this->m_eValue = linecapFlat;
				break;
			case 'r':
                if      ( (L"rnd")  == sValue ) this->m_eValue = linecapRnd;
				break;
			case 's':
                if      ( (L"sq")   == sValue ) this->m_eValue = linecapSq;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring  ToString  () const
		{
            switch(this->m_eValue)
			{
			case linecapFlat: return (L"flat");			
			case linecapRnd:  return (L"rnd");			
			case linecapSq:   return (L"sq");			
			default :         return (L"rnd");
			}
		}

		SimpleType_FromString     (ELineCap)
		SimpleType_Operator_Equal (CLineCap)
	};
	//--------------------------------------------------------------------------------
	// LineEndLength 20.1.10.32 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELineEndLength
	{
		lineendlengthLarge  = 0,
		lineendlengthMedium = 1,
		lineendlengthSmall  = 2,
	};

	template<ELineEndLength eDefValue = lineendlengthMedium>
	class CLineEndLength : public CSimpleType<ELineEndLength, eDefValue>
	{
	public:
		CLineEndLength() {}

        virtual ELineEndLength FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'l':
                if      ( (L"lg")  == sValue ) this->m_eValue = lineendlengthLarge;
				break;
			case 'm':
                if      ( (L"med") == sValue ) this->m_eValue = lineendlengthMedium;
				break;
			case 's':
                if      ( (L"sm")  == sValue ) this->m_eValue = lineendlengthSmall;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(this->m_eValue)
			{
			case lineendlengthLarge:  return (L"lg");			
			case lineendlengthMedium: return (L"med");			
			case lineendlengthSmall:  return (L"sm");		
			default :                 return (L"med");
			}
		}

		SimpleType_FromString     (ELineEndLength)
		SimpleType_Operator_Equal (CLineEndLength)
	};


	//--------------------------------------------------------------------------------
	// LineEndType 20.1.10.33 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELineEndType
	{
		lineendtypeArrow    = 0,
		lineendtypeDiamond  = 1,
		lineendtypeNone     = 2,
		lineendtypeOval     = 3,
		lineendtypeStealth  = 4,
		lineendtypeTriangle = 5,
	};

	template<ELineEndType eDefValue = lineendtypeNone>
	class CLineEndType : public CSimpleType<ELineEndType, eDefValue>
	{
	public:
		CLineEndType() {}

        virtual ELineEndType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':
                if      ( (L"arrow")    == sValue ) this->m_eValue = lineendtypeArrow;
				break;
			case 'd':
                if      ( (L"diamond")  == sValue ) this->m_eValue = lineendtypeDiamond;
				break;
			case 'n':
                if      ( (L"none")     == sValue ) this->m_eValue = lineendtypeNone;
				break;
			case 'o':
                if      ( (L"oval")     == sValue ) this->m_eValue = lineendtypeOval;
				break;
			case 's':
                if      ( (L"stealth")  == sValue ) this->m_eValue = lineendtypeStealth;
				break;
			case 't':
                if      ( (L"triangle") == sValue ) this->m_eValue = lineendtypeTriangle;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring      ToString  () const
		{
            switch(this->m_eValue)
			{
			case lineendtypeArrow:    return (L"arrow");			
			case lineendtypeDiamond:  return (L"diamond");			
			case lineendtypeNone:     return (L"none");		
			case lineendtypeOval:     return (L"oval");			
			case lineendtypeStealth:  return (L"stealth");			
			case lineendtypeTriangle: return (L"triangle");		
			default :                 return (L"none");
			}
		}

		SimpleType_FromString     (ELineEndType)
		SimpleType_Operator_Equal (CLineEndType)
	};


	//--------------------------------------------------------------------------------
	// LineEndWidth 20.1.10.34 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ELineEndWidth
	{
		lineendwidthLarge  = 0,
		lineendwidthMedium = 1,
		lineendwidthSmall  = 2,
	};

	template<ELineEndWidth eDefValue = lineendwidthMedium>
	class CLineEndWidth : public CSimpleType<ELineEndWidth, eDefValue>
	{
	public:
		CLineEndWidth() {}

        virtual ELineEndWidth FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'l':
                if      ( (L"lg")  == sValue ) this->m_eValue = lineendwidthLarge;
				break;
			case 'm':
                if      ( (L"med") == sValue ) this->m_eValue = lineendwidthMedium;
				break;
			case 's':
                if      ( (L"sm")  == sValue ) this->m_eValue = lineendwidthSmall;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case lineendwidthLarge:  return (L"lg");			
			case lineendwidthMedium: return (L"med");			
			case lineendwidthSmall:  return (L"sm");		
			default :                return (L"med");
			}
		}

		SimpleType_FromString     (ELineEndWidth)
		SimpleType_Operator_Equal (CLineEndWidth)
	};


	//--------------------------------------------------------------------------------
    // LineWidth 20.1.10.35 (Part 1)
	//--------------------------------------------------------------------------------  
    template<__int64 nDefValue = 0>
    class CLineWidth : public CSimpleType<__int64, nDefValue>
    {
    public:
        CLineWidth() {}

        virtual __int64 FromString(std::wstring &sValue)
        {
            this->m_eValue = _wtoi64( sValue.c_str() );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 20116800)
                this->m_eValue = 20116800;

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (__int64)
        SimpleType_Operator_Equal (CLineWidth)

        double ToPoints()
        {
            return Emu_To_Pt( this->m_eValue );
        }

        double ToInches()
        {
            return Emu_To_Inch( this->m_eValue );
        }

		double FromEmu(const __int64 nEmu)
		{
            this->m_eValue = nEmu;
            return Emu_To_Pt( this->m_eValue );
		}
    };
	//--------------------------------------------------------------------------------
	// PathFillMode 20.1.10.37 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPathFillMode
	{
		pathfillmodeDarken      = 0,
		pathfillmodeDarkenLess  = 1,
		pathfillmodeLighten     = 2,
		pathfillmodeLightenLess = 3,
		pathfillmodeNone        = 4,
		pathfillmodeNorm        = 5,
	};

	template<EPathFillMode eDefValue = pathfillmodeNone>
	class CPathFillMode : public CSimpleType<EPathFillMode, eDefValue>
	{
	public:
		CPathFillMode() {}

        virtual EPathFillMode FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'd':
                if      ( (L"darken")     == sValue ) this->m_eValue = pathfillmodeDarken;
                else if ( (L"darkenLess") == sValue ) this->m_eValue = pathfillmodeDarkenLess;
				break;
			case 'l':
                if      ( (L"lighten")    == sValue ) this->m_eValue = pathfillmodeLighten;
                else if ( (L"darkenLess") == sValue ) this->m_eValue = pathfillmodeLightenLess;
				break;
			case 'n':
                if      ( (L"none")       == sValue ) this->m_eValue = pathfillmodeNone;
                else if ( (L"norm")       == sValue ) this->m_eValue = pathfillmodeNorm;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case pathfillmodeDarken:      return (L"darken");			
			case pathfillmodeDarkenLess:  return (L"darkenLess");			
			case pathfillmodeLighten:     return (L"lighten");		
			case pathfillmodeLightenLess: return (L"lightenLess");			
			case pathfillmodeNone:        return (L"none");			
			case pathfillmodeNorm:        return (L"norm");		
			default :                     return (L"none");
			}
		}

		SimpleType_FromString     (EPathFillMode)
		SimpleType_Operator_Equal (CPathFillMode)
	};


	//--------------------------------------------------------------------------------
	// PathShadeType 20.1.10.38 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPathShadeType
	{
		pathshadetypeCircle = 0,
		pathshadetypeRect   = 1,
		pathshadetypeShape  = 2,
	};

	template<EPathShadeType eDefValue = pathshadetypeRect>
	class CPathShadeType : public CSimpleType<EPathShadeType, eDefValue>
	{
	public:
		CPathShadeType() {}

        virtual EPathShadeType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"circle") == sValue ) this->m_eValue = pathshadetypeCircle;
				break;
			case 'r':
                if      ( (L"rect")   == sValue ) this->m_eValue = pathshadetypeRect;
				break;
			case 's':
                if      ( (L"shape")  == sValue ) this->m_eValue = pathshadetypeShape;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(this->m_eValue)
			{
			case pathshadetypeCircle: return (L"circle");			
			case pathshadetypeRect:   return (L"rect");			
			case pathshadetypeShape:  return (L"shape");		
			default :                 return (L"rect");
			}
		}

		SimpleType_FromString     (EPathShadeType)
		SimpleType_Operator_Equal (CPathShadeType)
	};


	//--------------------------------------------------------------------------------
	// PenAlignment 20.1.10.39 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPenAlignment
	{
		penalignmentCtr = 0,
		penalignmentIn  = 1,
	};

	template<EPenAlignment eDefValue = penalignmentCtr>
	class CPenAlignment : public CSimpleType<EPenAlignment, eDefValue>
	{
	public:
		CPenAlignment() {}

        virtual EPenAlignment FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"ctr") == sValue ) this->m_eValue = penalignmentCtr;
				break;
			case 'i':
                if      ( (L"in")  == sValue ) this->m_eValue = penalignmentIn;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case penalignmentCtr: return (L"ctr");			
			case penalignmentIn:  return (L"in");			
			default :             return (L"ctr");
			}
		}

		SimpleType_FromString     (EPenAlignment)
		SimpleType_Operator_Equal (CPenAlignment)
	};
	//--------------------------------------------------------------------------------
    // Percentage 20.1.10.40 (Part 1) + 12.1.2.2 (Part4)
	//--------------------------------------------------------------------------------
    class CPercentage 
    {
    public:
        CPercentage() 
		{
			m_dValue = 0;
		}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
			m_dValue = dValue;
		}

        virtual double FromString(std::wstring &sValue)
        {
            //todo странно что если пришло значение от 0 до 1, то m_dValue от 0 до 1. В других случаях от 0 до 100
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					//test
                    double dValue = sValue.empty() ? 0 : _wtof(sValue.c_str());
                    if (fabs(dValue) >= 0 && fabs(dValue) <=1 )
					{
						m_dValue = dValue;
					}
					else
					{
						// Поправка 12.1.2.2 (Part4)
                        m_dValue = dValue / 1000.0;
					}
				}
				else
					m_dValue = 0;
			}
			else
            {
                std::wstring strValue = sValue.substr( 0, nLen - 1 );
                m_dValue = strValue.empty() ? 0 : _wtof( strValue.c_str() );
            }

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
            return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }
        virtual std::wstring ToStringDecimalNumber  () const
		{
            std::wstring sResult = std::to_wstring( int(m_dValue * 1000.0) );

			return sResult;
		}

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CPercentage)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
    // PositiveCoordinate 20.1.10.42 (Part 1)
	//--------------------------------------------------------------------------------  
    template<__int64 nDefValue = 0>
    class CPositiveCoordinate : public CSimpleType<__int64, nDefValue>
    {
    public:
        CPositiveCoordinate() {}

        virtual __int64 FromString(std::wstring &sValue)
        {
            this->m_eValue = sValue.empty() ? 0 : _wtoi64( sValue.c_str() );

            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 27273042316900)
                this->m_eValue = 27273042316900;

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (__int64)
        SimpleType_Operator_Equal (CPositiveCoordinate)

        double ToPoints() const
        {
            return Emu_To_Pt( this->m_eValue );
        }
		double ToMM() const
        {
            return Emu_To_Mm( (double)this->m_eValue );
        }

        double ToInches()
        {
            return Emu_To_Inch( (double)this->m_eValue );
        }
        double ToTwips()
        {
            return Emu_To_Twips( (double)this->m_eValue );
        }
		double FromEmu(const __int64& nEmu)
		{
            this->m_eValue = nEmu;
            return Emu_To_Pt( (double)this->m_eValue );
		}
    };
	//--------------------------------------------------------------------------------
    // PositiveCoordinate32 20.1.10.43 (Part 1)
	//--------------------------------------------------------------------------------  
	#define CPositiveCoordinate32 CPositiveCoordinate
	//--------------------------------------------------------------------------------
	// PositiveFixedAngle 20.1.10.44 (Part 1)
	//--------------------------------------------------------------------------------		
	template<int nDefValue = 0>
	class CPositiveFixedAngle : public CSimpleType<int, nDefValue>
	{
	public:

		CPositiveFixedAngle() {}

		void SetValue(int nValue)
		{
           this->m_eValue = (std::min)( 21600000, (std::max)( 0, nValue ) );
		}

        virtual int     FromString(std::wstring &sValue)
		{
            this->m_eValue = (std::min)( 21600000, (std::max)( 0, _wtoi( sValue.c_str() ) ) );

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            std::wstring sResult = std::to_wstring( this->m_eValue );
			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CPositiveFixedAngle)

		double  GetAngle() const
		{
            return this->m_eValue / 60000.0;
		}

	};

	//--------------------------------------------------------------------------------
    // PositiveFixedPercentage 20.1.10.45 (Part 1) + 12.1.2.3 (Part4)
	//--------------------------------------------------------------------------------  
    class CPositiveFixedPercentage 
    {
    public:
        CPositiveFixedPercentage() {}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
            m_dValue = (std::min)( 100.0, (std::max)( 0.0, dValue ) );
		}

        virtual double FromString(std::wstring &sValue)
        {
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos && nLen > 0)
				{
					// Поправка 12.1.2.3 (Part4)
                    int nValue = (std::max)( 0, (std::min)( 100000, _wtoi( sValue.c_str() ) ) );
					m_dValue = nValue / 1000.0;
				}
				else
					m_dValue = 0;
			}
			else
                m_dValue = (std::min)( 100.0, (std::max)( 0.0, _wtof( sValue.substr( 0, nLen - 1 ).c_str() ) ) );

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
			return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CPositiveFixedPercentage)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
    // PositivePercentage 20.1.10.46 (Part 1) + 12.1.2.4 (Part4)
	//--------------------------------------------------------------------------------
    class CPositivePercentage 
    {
    public:
        CPositivePercentage() {}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
            m_dValue = (std::max)( 0.0, dValue );
		}

        virtual double FromString(std::wstring &sValue)
        {
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos && nLen > 0)
				{
					// Поправка 12.1.2.4 (Part4)
                    int nValue = (std::max)( 0, _wtoi( sValue.c_str() ) );
					m_dValue = nValue / 1000.0;
				}
				return
					m_dValue;
			}
			else
                m_dValue = (std::max)( 0.0, _wtof( sValue.substr( 0, nLen - 1 ).c_str() ) );

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
			return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CPositivePercentage)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
	// PresetCameraType 20.1.10.47 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetCameraType
	{
		presetcameratypeIsometricBottomDown, // (Isometric Bottom Down) 
		presetcameratypeIsometricBottomUp, // (Isometric Bottom Up) 
		presetcameratypeIsometricLeftDown, // (Isometric Left Down) 
		presetcameratypeIsometricLeftUp, // (Isometric Left Up) 
		presetcameratypeIsometricOffAxis1Left, // (Isometric Off Axis 1 Left) 
		presetcameratypeIsometricOffAxis1Right, // (Isometric Off Axis 1 Right) 
		presetcameratypeIsometricOffAxis1Top, // (Isometric Off Axis 1 Top) 
		presetcameratypeIsometricOffAxis2Left, // (Isometric Off Axis 2 Left) 
		presetcameratypeIsometricOffAxis2Right, // (Isometric Off Axis 2 Right
		presetcameratypeIsometricOffAxis2Top, // (Isometric Off Axis 2 Top) 
		presetcameratypeIsometricOffAxis3Bottom, // (Isometric Off Axis 3 Bottom) 
		presetcameratypeIsometricOffAxis3Left, // (Isometric Off Axis 3 Left) 
		presetcameratypeIsometricOffAxis3Right, // (Isometric Off Axis 3 Right) 
		presetcameratypeIsometricOffAxis4Bottom, // (Isometric Off Axis 4 Bottom) 
		presetcameratypeIsometricOffAxis4Left, // (Isometric Off Axis 4 Left) 
		presetcameratypeIsometricOffAxis4Right, // (Isometric Off Axis 4 Right) 
		presetcameratypeIsometricRightDown, // (Isometric Right Down) 
		presetcameratypeIsometricRightUp, // (Isometric Right Up) 
		presetcameratypeIsometricTopDown, // (Isometric Top Down) 
		presetcameratypeIsometricTopUp, // (Isometric Top Up) 
		presetcameratypeLegacyObliqueBottom, // (Legacy Oblique Bottom) 
		presetcameratypeLegacyObliqueBottomLeft, // (Legacy Oblique Bottom Left) 
		presetcameratypeLegacyObliqueBottomRight, // (Legacy Oblique Bottom Right) 
		presetcameratypeLegacyObliqueFront, // (Legacy Oblique Front) 
		presetcameratypeLegacyObliqueLeft, // (Legacy Oblique Left) 
		presetcameratypeLegacyObliqueRight, // (Legacy Oblique Right) 
		presetcameratypeLegacyObliqueTop, // (Legacy Oblique Top) 
		presetcameratypeLegacyObliqueTopLeft, // (Legacy Oblique Top Left) 
		presetcameratypeLegacyObliqueTopRight, // (Legacy Oblique Top Right) 
		presetcameratypeLegacyPerspectiveBottom, // (Legacy Perspective Bottom) 
		presetcameratypeLegacyPerspectiveBottomLeft, // (Legacy Perspective Bottom Left) 
		presetcameratypeLegacyPerspectiveBottomRight, // (Legacy Perspective Bottom Right) 
		presetcameratypeLegacyPerspectiveFront, // (Legacy Perspective Front) 
		presetcameratypeLegacyPerspectiveLeft, // (Legacy Perspective Left) 
		presetcameratypeLegacyPerspectiveRight, // (Legacy Perspective Right) 
		presetcameratypeLegacyPerspectiveTop, // (Legacy Perspective Top) 
		presetcameratypeLegacyPerspectiveTopLeft, // (Legacy Perspective Top Left) 
		presetcameratypeLegacyPerspectiveTopRight, // (Legacy Perspective Top Right) 
		presetcameratypeObliqueBottom, // (Oblique Bottom) 
		presetcameratypeObliqueBottomLeft, // (Oblique Bottom Left) 
		presetcameratypeObliqueBottomRight, // (Oblique Bottom Right) 
		presetcameratypeObliqueLeft, // (Oblique Left) 
		presetcameratypeObliqueRight, // (Oblique Right) 
		presetcameratypeObliqueTop, // (Oblique Top) 
		presetcameratypeObliqueTopLeft, // (Oblique Top Left) 
		presetcameratypeObliqueTopRight, // (Oblique Top Right) 
		presetcameratypeOrthographicFront, // (Orthographic Front) 
		presetcameratypePerspectiveAbove, // (Orthographic Above) 
		presetcameratypePerspectiveAboveLeftFacing, // (Perspective Above Left Facing) 
		presetcameratypePerspectiveAboveRightFacing, // (Perspective Above Right Facing) 
		presetcameratypePerspectiveBelow, // (Perspective Below) 
		presetcameratypePerspectiveContrastingLeftFacing, // (Perspective Contrasting Left Facing) 
		presetcameratypePerspectiveContrastingRightFacing, // (Perspective Contrasting Right Facing) 
		presetcameratypePerspectiveFront, // (Perspective Front) 
		presetcameratypePerspectiveHeroicExtremeLeftFacing, // (Perspective Heroic Extreme Left Facing) 
		presetcameratypePerspectiveHeroicExtremeRightFacing, // (Perspective Heroic Extreme Right Facing) 
		presetcameratypePerspectiveHeroicLeftFacing, // (Perspective Heroic Left Facing) 
		presetcameratypePerspectiveHeroicRightFacing, // (Perspective Heroic Right Facing) 
		presetcameratypePerspectiveLeft, // (Perspective Left) 
		presetcameratypePerspectiveRelaxed, // (Perspective Relaxed) 
		presetcameratypePerspectiveRelaxedModerately, // (Perspective Relaxed Moderately) 
		presetcameratypePerspectiveRight, // (Perspective Right) 
	};

	template<EPresetCameraType eDefValue = presetcameratypePerspectiveFront>
	class CPresetCameraType : public CSimpleType<EPresetCameraType, eDefValue>
	{
	public:
		CPresetCameraType() {}

        virtual EPresetCameraType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'i':
                if      ( (L"isometricBottomDown")					== sValue ) this->m_eValue = presetcameratypeIsometricBottomDown;
                else if ( (L"isometricBottomUp")					== sValue ) this->m_eValue = presetcameratypeIsometricBottomUp;
                else if ( (L"isometricLeftDown")					== sValue ) this->m_eValue = presetcameratypeIsometricLeftDown;
                else if ( (L"isometricLeftUp")						== sValue ) this->m_eValue = presetcameratypeIsometricLeftUp;
                else if ( (L"isometricOffAxis1Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Left;
                else if ( (L"isometricOffAxis1Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Right;
                else if ( (L"isometricOffAxis1Top")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Top;
                else if ( (L"isometricOffAxis2Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Left;
                else if ( (L"isometricOffAxis2Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Right;
                else if ( (L"isometricOffAxis2Top")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Top;
                else if ( (L"isometricOffAxis3Bottom")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Bottom ;
                else if ( (L"isometricOffAxis3Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Left;
                else if ( (L"isometricOffAxis3Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Right;
                else if ( (L"isometricOffAxis4Bottom")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Bottom;
                else if ( (L"isometricOffAxis4Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Left;
                else if ( (L"isometricOffAxis4Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Right;
                else if ( (L"isometricRightDown")					== sValue ) this->m_eValue = presetcameratypeIsometricRightDown;
                else if ( (L"isometricRightUp")					== sValue ) this->m_eValue = presetcameratypeIsometricRightUp;
                else if ( (L"isometricTopDown")					== sValue ) this->m_eValue = presetcameratypeIsometricTopDown;
                else if ( (L"isometricTopUp")						== sValue ) this->m_eValue = presetcameratypeIsometricTopUp;
				break;
			case 'l':
                if      ( (L"legacyObliqueBottom")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottom;
                else if ( (L"legacyObliqueBottomLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottomLeft;
                else if ( (L"legacyObliqueBottomRight")			== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottomRight;
                else if ( (L"legacyObliqueFront")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueFront;
                else if ( (L"legacyObliqueLeft")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueLeft;
                else if ( (L"legacyObliqueRight")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueRight;
                else if ( (L"legacyObliqueTop")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTop;
                else if ( (L"legacyObliqueTopLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTopLeft;
                else if ( (L"legacyObliqueTopRight")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTopRight;
                else if ( (L"legacyPerspectiveBottom")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottom;
                else if ( (L"legacyPerspectiveBottomLeft")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottomLeft;
                else if ( (L"legacyPerspectiveBottomRight")		== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottomRight;
                else if ( (L"legacyPerspectiveFront")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveFront;
                else if ( (L"legacyPerspectiveLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveLeft;
                else if ( (L"legacyPerspectiveRight")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveRight;
                else if ( (L"legacyPerspectiveTop")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTop;
                else if ( (L"legacyPerspectiveTopLeft")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTopLeft;
                else if ( (L"legacyPerspectiveTopRight")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTopRight;
				break;
			case 'o':
                if      ( (L"obliqueBottom")						== sValue ) this->m_eValue = presetcameratypeObliqueBottom;
                else if ( (L"obliqueBottomLeft")					== sValue ) this->m_eValue = presetcameratypeObliqueBottomLeft;
                else if ( (L"obliqueBottomRight")					== sValue ) this->m_eValue = presetcameratypeObliqueBottomRight;
                else if ( (L"obliqueLeft")							== sValue ) this->m_eValue = presetcameratypeObliqueLeft;
                else if ( (L"obliqueRight")						== sValue ) this->m_eValue = presetcameratypeObliqueRight;
                else if ( (L"obliqueTop")							== sValue ) this->m_eValue = presetcameratypeObliqueTop;
                else if ( (L"obliqueTopLeft")						== sValue ) this->m_eValue = presetcameratypeObliqueTopLeft;
                else if ( (L"obliqueTopRight")						== sValue ) this->m_eValue = presetcameratypeObliqueTopRight;
                else if ( (L"orthographicFront")					== sValue ) this->m_eValue = presetcameratypeOrthographicFront;
				break;
			case 'p':
                if      ( (L"perspectiveAbove")					== sValue ) this->m_eValue = presetcameratypePerspectiveAbove;
                else if ( (L"perspectiveAboveLeftFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveAboveLeftFacing;
                else if ( (L"perspectiveAboveRightFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveAboveRightFacing;
                else if ( (L"perspectiveBelow")					== sValue ) this->m_eValue = presetcameratypePerspectiveBelow;
                else if ( (L"perspectiveContrastingLeftFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveContrastingLeftFacing;
                else if ( (L"perspectiveContrastingRightFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveContrastingRightFacing;
                else if ( (L"perspectiveFront")			        == sValue ) this->m_eValue = presetcameratypePerspectiveFront;
                else if ( (L"perspectiveHeroicExtremeLeftFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicExtremeLeftFacing;
                else if ( (L"perspectiveHeroicExtremeRightFacing") == sValue ) this->m_eValue = presetcameratypePerspectiveHeroicExtremeRightFacing;
                else if ( (L"perspectiveHeroicLeftFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicLeftFacing;
                else if ( (L"perspectiveHeroicRightFacing")		== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicRightFacing;
                else if ( (L"perspectiveLeft")						== sValue ) this->m_eValue = presetcameratypePerspectiveLeft;
                else if ( (L"perspectiveRelaxed")					== sValue ) this->m_eValue = presetcameratypePerspectiveRelaxed;
                else if ( (L"perspectiveRelaxedModerately")		== sValue ) this->m_eValue = presetcameratypePerspectiveRelaxedModerately;
                else if ( (L"perspectiveRight")					== sValue ) this->m_eValue = presetcameratypePerspectiveRight;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetcameratypeIsometricBottomDown: return (L"isometricBottomDown"); // (Isometric Bottom Down) 
			case presetcameratypeIsometricBottomUp: return (L"isometricBottomUp"); // (Isometric Bottom Up) 
			case presetcameratypeIsometricLeftDown: return (L"isometricLeftDown"); // (Isometric Left Down) 
			case presetcameratypeIsometricLeftUp: return (L"isometricLeftUp"); // (Isometric Left Up) 
			case presetcameratypeIsometricOffAxis1Left: return (L"isometricOffAxis1Left"); // (Isometric Off Axis 1 Left) 
			case presetcameratypeIsometricOffAxis1Right: return (L"isometricOffAxis1Right"); // (Isometric Off Axis 1 Right) 
			case presetcameratypeIsometricOffAxis1Top: return (L"isometricOffAxis1Top"); // (Isometric Off Axis 1 Top) 
			case presetcameratypeIsometricOffAxis2Left: return (L"isometricOffAxis2Left"); // (Isometric Off Axis 2 Left) 
			case presetcameratypeIsometricOffAxis2Right: return (L"isometricOffAxis2Right"); // (Isometric Off Axis 2 Right
			case presetcameratypeIsometricOffAxis2Top: return (L"isometricOffAxis2Top"); // (Isometric Off Axis 2 Top) 
			case presetcameratypeIsometricOffAxis3Bottom: return (L"isometricOffAxis3Bottom"); // (Isometric Off Axis 3 Bottom) 
			case presetcameratypeIsometricOffAxis3Left: return (L"isometricOffAxis3Left"); // (Isometric Off Axis 3 Left) 
			case presetcameratypeIsometricOffAxis3Right: return (L"isometricOffAxis3Right"); // (Isometric Off Axis 3 Right) 
			case presetcameratypeIsometricOffAxis4Bottom: return (L"isometricOffAxis4Bottom"); // (Isometric Off Axis 4 Bottom) 
			case presetcameratypeIsometricOffAxis4Left: return (L"isometricOffAxis4Left"); // (Isometric Off Axis 4 Left) 
			case presetcameratypeIsometricOffAxis4Right: return (L"isometricOffAxis4Right"); // (Isometric Off Axis 4 Right) 
			case presetcameratypeIsometricRightDown:	return (L"isometricRightDown"); // (Isometric Right Down) 
			case presetcameratypeIsometricRightUp:		return (L"isometricRightUp"); // (Isometric Right Up) 
			case presetcameratypeIsometricTopDown:		return (L"isometricTopDown"); // (Isometric Top Down) 
			case presetcameratypeIsometricTopUp:		return (L"isometricTopUp"); // (Isometric Top Up) 
			case presetcameratypeLegacyObliqueBottom:	return (L"legacyObliqueBottom"); // (Legacy Oblique Bottom) 
			case presetcameratypeLegacyObliqueBottomLeft: return (L"legacyObliqueBottomLeft"); // (Legacy Oblique Bottom Left) 
			case presetcameratypeLegacyObliqueBottomRight: return (L"legacyObliqueBottomRight"); // (Legacy Oblique Bottom Right) 
			case presetcameratypeLegacyObliqueFront:	return (L"legacyObliqueFront"); // (Legacy Oblique Front) 
			case presetcameratypeLegacyObliqueLeft:		return (L"legacyObliqueLeft"); // (Legacy Oblique Left) 
			case presetcameratypeLegacyObliqueRight:	return (L"legacyObliqueRight"); // (Legacy Oblique Right) 
			case presetcameratypeLegacyObliqueTop:		return (L"legacyObliqueTop"); // (Legacy Oblique Top) 
			case presetcameratypeLegacyObliqueTopLeft:	return (L"legacyObliqueTopLeft"); // (Legacy Oblique Top Left) 
			case presetcameratypeLegacyObliqueTopRight: return (L"legacyObliqueTopRight"); // (Legacy Oblique Top Right) 
			case presetcameratypeLegacyPerspectiveBottom: return (L"legacyPerspectiveBottom"); // (Legacy Perspective Bottom) 
			case presetcameratypeLegacyPerspectiveBottomLeft: return (L"legacyPerspectiveBottomLeft"); // (Legacy Perspective Bottom Left) 
			case presetcameratypeLegacyPerspectiveBottomRight: return (L"legacyPerspectiveBottomRight"); // (Legacy Perspective Bottom Right) 
			case presetcameratypeLegacyPerspectiveFront: return (L"legacyPerspectiveFront"); // (Legacy Perspective Front) 
			case presetcameratypeLegacyPerspectiveLeft: return (L"legacyPerspectiveLeft"); // (Legacy Perspective Left) 
			case presetcameratypeLegacyPerspectiveRight: return (L"legacyPerspectiveRight"); // (Legacy Perspective Right) 
			case presetcameratypeLegacyPerspectiveTop: return (L"legacyPerspectiveTop"); // (Legacy Perspective Top) 
			case presetcameratypeLegacyPerspectiveTopLeft: return (L"legacyPerspectiveTopLeft"); // (Legacy Perspective Top Left) 
			case presetcameratypeLegacyPerspectiveTopRight: return (L"legacyPerspectiveTopRight"); // (Legacy Perspective Top Right) 
			case presetcameratypeObliqueBottom:			return (L"obliqueBottom"); // (Oblique Bottom) 
			case presetcameratypeObliqueBottomLeft:		return (L"obliqueBottomLeft"); // (Oblique Bottom Left) 
			case presetcameratypeObliqueBottomRight:	return (L"obliqueBottomRight"); // (Oblique Bottom Right) 
			case presetcameratypeObliqueLeft:			return (L"obliqueLeft"); // (Oblique Left) 
			case presetcameratypeObliqueRight: return (L"obliqueRight"); // (Oblique Right) 
			case presetcameratypeObliqueTop: return (L"obliqueTop"); // (Oblique Top) 
			case presetcameratypeObliqueTopLeft: return (L"obliqueTopLeft"); // (Oblique Top Left) 
			case presetcameratypeObliqueTopRight: return (L"obliqueTopRight"); // (Oblique Top Right) 
			case presetcameratypeOrthographicFront: return (L"orthographicFront"); // (Orthographic Front) 
			case presetcameratypePerspectiveAbove: return (L"perspectiveAbove"); // (Orthographic Above) 
			case presetcameratypePerspectiveAboveLeftFacing: return (L"perspectiveAboveLeftFacing"); // (Perspective Above Left Facing) 
			case presetcameratypePerspectiveAboveRightFacing: return (L"perspectiveAboveRightFacing"); // (Perspective Above Right Facing) 
			case presetcameratypePerspectiveBelow: return (L"perspectiveBelow"); // (Perspective Below) 
			case presetcameratypePerspectiveContrastingLeftFacing: return (L"perspectiveContrastingLeftFacing"); // (Perspective Contrasting Left Facing) 
			case presetcameratypePerspectiveContrastingRightFacing: return (L"perspectiveContrastingRightFacing"); // (Perspective Contrasting Right Facing) 
			case presetcameratypePerspectiveFront: return (L"perspectiveFront"); // (Perspective Front) 
			case presetcameratypePerspectiveHeroicExtremeLeftFacing: return (L"perspectiveHeroicExtremeLeftFacing"); // (Perspective Heroic Extreme Left Facing) 
			case presetcameratypePerspectiveHeroicExtremeRightFacing: return (L"perspectiveHeroicExtremeRightFacing"); // (Perspective Heroic Extreme Right Facing) 
			case presetcameratypePerspectiveHeroicLeftFacing: return (L"perspectiveHeroicLeftFacing"); // (Perspective Heroic Left Facing) 
			case presetcameratypePerspectiveHeroicRightFacing: return (L"perspectiveHeroicRightFacing"); // (Perspective Heroic Right Facing) 
			case presetcameratypePerspectiveLeft: return (L"perspectiveLeft"); // (Perspective Left) 
			case presetcameratypePerspectiveRelaxed: return (L"perspectiveRelaxed"); // (Perspective Relaxed) 
			case presetcameratypePerspectiveRelaxedModerately: return (L"perspectiveRelaxedModerately"); // (Perspective Relaxed Moderately) 
			case presetcameratypePerspectiveRight: return (L"perspectiveRight"); // (Perspective Right) 			
			default :								return (L"orthographicFront");
			}
		}

		SimpleType_FromString     (EPresetCameraType)
		SimpleType_Operator_Equal (CPresetCameraType)
	};


	//--------------------------------------------------------------------------------
	// PresetColorVal 20.1.10.48 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetColorVal
	{
		presetcolorvalAliceBlue = 0, // (Alice Blue Preset Color) 
		presetcolorvalAntiqueWhite, // (Antique White Preset Color) 
		presetcolorvalAqua, // (Aqua Preset Color) 
		presetcolorvalAquamarine, // (Aquamarine Preset Color) 
		presetcolorvalAzure, // (Azure Preset Color) 
		presetcolorvalBeige, // (Beige Preset Color) 
		presetcolorvalBisque, // (Bisque Preset Color) 
		presetcolorvalBlack, // (Black Preset Color) 
		presetcolorvalBlanchedAlmond, // (Blanched Almond Preset Color) 
		presetcolorvalBlue, // (Blue Preset Color) 
		presetcolorvalBlueViolet, // (Blue Violet Preset Color) 
		presetcolorvalBrown, // (Brown Preset Color) 
		presetcolorvalBurlyWood, // (Burly Wood Preset Color) 
		presetcolorvalCadetBlue, // (Cadet Blue Preset Color) 
		presetcolorvalChartreuse, // (Chartreuse Preset Color) 
		presetcolorvalChocolate, // (Chocolate Preset Color) 
		presetcolorvalCoral, // (Coral Preset Color) 
		presetcolorvalCornflowerBlue, // (Cornflower Blue Preset Color) 
		presetcolorvalCornsilk, // (Cornsilk Preset Color) 
		presetcolorvalCrimson, // (Crimson Preset Color) 
		presetcolorvalCyan, // (Cyan Preset Color) 
		presetcolorvalDarkBlue, // (Dark Blue Preset Color) 
		presetcolorvalDarkCyan, // (Dark Cyan Preset Color) 
		presetcolorvalDarkGoldenrod, // (Dark Goldenrod Preset Color) 
		presetcolorvalDarkGray, // (Dark Gray Preset Color) 
		presetcolorvalDarkGreen, // (Dark Green Preset Color) 
		presetcolorvalDarkGrey, // (Dark Gray Preset Color) 
		presetcolorvalDarkKhaki, // (Dark Khaki Preset Color) 
		presetcolorvalDarkMagenta, // (Dark Magenta Preset Color) 
		presetcolorvalDarkOliveGreen, // (Dark Olive Green Preset Color) 
		presetcolorvalDarkOrange, // (Dark Orange Preset Color) 
		presetcolorvalDarkOrchid, // (Dark Orchid Preset Color) 
		presetcolorvalDarkRed, // (Dark Red Preset Color) 
		presetcolorvalDarkSalmon, // (Dark Salmon Preset Color) 
		presetcolorvalDarkSeaGreen, // (Dark Sea Green Preset Color) 
		presetcolorvalDarkSlateBlue, // (Dark Slate Blue Preset Color) 
		presetcolorvalDarkSlateGray, // (Dark Slate Gray Preset Color) 
		presetcolorvalDarkSlateGrey, // (Dark Slate Gray Preset Color) 
		presetcolorvalDarkTurquoise, // (Dark Turquoise Preset Color) 
		presetcolorvalDarkViolet, // (Dark Violet Preset Color) 
		presetcolorvalDeepPink, // (Deep Pink Preset Color) 
		presetcolorvalDeepSkyBlue, // (Deep Sky Blue Preset Color) 
		presetcolorvalDimGray, // (Dim Gray Preset Color) 
		presetcolorvalDimGrey, // (Dim Gray Preset Color) 
		presetcolorvalDkBlue, // (Dark Blue Preset Color) 
		presetcolorvalDkCyan, // (Dark Cyan Preset Color) 
		presetcolorvalDkGoldenrod, // (Dark Goldenrod Preset Color) 
		presetcolorvalDkGray, // (Dark Gray Preset Color) 
		presetcolorvalDkGreen, // (Dark Green Preset Color) 
		presetcolorvalDkGrey, // (Dark Gray Preset Color) 
		presetcolorvalDkKhaki, // (Dark Khaki Preset Color) 
		presetcolorvalDkMagenta, // (Dark Magenta Preset Color) 
		presetcolorvalDkOliveGreen, // (Dark Olive Green Preset Color) 
		presetcolorvalDkOrange, // (Dark Orange Preset Color) 
		presetcolorvalDkOrchid, // (Dark Orchid Preset Color) 
		presetcolorvalDkRed, // (Dark Red Preset Color) 
		presetcolorvalDkSalmon, // (Dark Salmon Preset Color) 
		presetcolorvalDkSeaGreen, // (Dark Sea Green Preset Color) 
		presetcolorvalDkSlateBlue, // (Dark Slate Blue Preset Color) 
		presetcolorvalDkSlateGray, // (Dark Slate Gray Preset Color) 
		presetcolorvalDkSlateGrey, // (Dark Slate Gray Preset Color) 
		presetcolorvalDkTurquoise, // (Dark Turquoise Preset Color) 
		presetcolorvalDkViolet, // (Dark Violet Preset Color) 
		presetcolorvalDodgerBlue, // (Dodger Blue Preset Color) 
		presetcolorvalFirebrick, // (Firebrick Preset Color) 
		presetcolorvalFloralWhite, // (Floral White Preset Color) 
		presetcolorvalForestGreen, // (Forest Green Preset Color) 
		presetcolorvalFuchsia, // (Fuchsia Preset Color) 
		presetcolorvalGainsboro, // (Gainsboro Preset Color) 
		presetcolorvalGhostWhite, // (Ghost White Preset Color) 
		presetcolorvalGold, // (Gold Preset Color) 
		presetcolorvalGoldenrod, // (Goldenrod Preset Color) 
		presetcolorvalGray, // (Gray Preset Color) 
		presetcolorvalGreen, // (Green Preset Color) 
		presetcolorvalGreenYellow, // (Green Yellow Preset Color) 
		presetcolorvalGrey, // (Gray Preset Color) 
		presetcolorvalHoneydew, // (Honeydew Preset Color) 
		presetcolorvalHotPink, // (Hot Pink Preset Color) 
		presetcolorvalIndianRed, // (Indian Red Preset Color) 
		presetcolorvalIndigo, // (Indigo Preset Color) 
		presetcolorvalIvory, // (Ivory Preset Color) 
		presetcolorvalKhaki, // (Khaki Preset Color) 
		presetcolorvalLavender, // (Lavender Preset Color) 
		presetcolorvalLavenderBlush, // (Lavender Blush Preset Color) 
		presetcolorvalLawnGreen, // (Lawn Green Preset Color) 
		presetcolorvalLemonChiffon, // (Lemon Chiffon Preset Color) 
		presetcolorvalLightBlue, // (Light Blue Preset Color) 
		presetcolorvalLightCoral, // (Light Coral Preset Color) 
		presetcolorvalLightCyan, // (Light Cyan Preset Color) 
		presetcolorvalLightGoldenrodYellow, // (Light Goldenrod Yellow PreseColor) 
		presetcolorvalLightGray, // (Light Gray Preset Color) 
		presetcolorvalLightGreen, // (Light Green Preset Color) 
		presetcolorvalLightGrey, // (Light Gray Preset Color) 
		presetcolorvalLightPink, // (Light Pink Preset Color) 
		presetcolorvalLightSalmon, // (Light Salmon Preset Color) 
		presetcolorvalLightSeaGreen, // (Light Sea Green Preset Color) 
		presetcolorvalLightSkyBlue, // (Light Sky Blue Preset Color) 
		presetcolorvalLightSlateGray, // (Light Slate Gray Preset Color) 
		presetcolorvalLightSlateGrey, // (Light Slate Gray Preset Color) 
		presetcolorvalLightSteelBlue, // (Light Steel Blue Preset Color) 
		presetcolorvalLightYellow, // (Light Yellow Preset Color) 
		presetcolorvalLime, // (Lime Preset Color) 
		presetcolorvalLimeGreen, // (Lime Green Preset Color) 
		presetcolorvalLinen, // (Linen Preset Color) 
		presetcolorvalLtBlue, // (Light Blue Preset Color) 
		presetcolorvalLtCoral, // (Light Coral Preset Color) 
		presetcolorvalLtCyan, // (Light Cyan Preset Color) 
		presetcolorvalLtGoldenrodYellow, // (Light Goldenrod Yellow Preset Color) 
		presetcolorvalLtGray, // (Light Gray Preset Color) 
		presetcolorvalLtGreen, // (Light Green Preset Color) 
		presetcolorvalLtGrey, // (Light Gray Preset Color) 
		presetcolorvalLtPink, // (Light Pink Preset Color) 
		presetcolorvalLtSalmon, // (Light Salmon Preset Color) 
		presetcolorvalLtSeaGreen, // (Light Sea Green Preset Color) 
		presetcolorvalLtSkyBlue, // (Light Sky Blue Preset Color) 
		presetcolorvalLtSlateGray, // (Light Slate Gray Preset Color) 
		presetcolorvalLtSlateGrey, // (Light Slate Gray Preset Color) 
		presetcolorvalLtSteelBlue, // (Light Steel Blue Preset Color) 
		presetcolorvalLtYellow, // (Light Yellow Preset Color) 
		presetcolorvalMagenta, // (Magenta Preset Color) 
		presetcolorvalMaroon, // (Maroon Preset Color) 
		presetcolorvalMedAquamarine, // (Medium Aquamarine Preset Color) 
		presetcolorvalMedBlue, // (Medium Blue Preset Color) 
		presetcolorvalMediumAquamarine, // (Medium Aquamarine Preset Color) 
		presetcolorvalMediumBlue, // (Medium Blue Preset Color) 
		presetcolorvalMediumOrchid, // (Medium Orchid Preset Color) 
		presetcolorvalMediumPurple, // (Medium Purple Preset Color) 
		presetcolorvalMediumSeaGreen, // (Medium Sea Green Preset Color) 
		presetcolorvalMediumSlateBlue, // (Medium Slate Blue Preset Color) 
		presetcolorvalMediumSpringGreen, // (Medium Spring Green Preset Color) 
		presetcolorvalMediumTurquoise, // (Medium Turquoise Preset Color) 
		presetcolorvalMediumVioletRed, // (Medium Violet Red Preset Color) 
		presetcolorvalMedOrchid, // (Medium Orchid Preset Color) 
		presetcolorvalMedPurple, // (Medium Purple Preset Color) 
		presetcolorvalMedSeaGreen, // (Medium Sea Green Preset Color) 
		presetcolorvalMedSlateBlue, // (Medium Slate Blue Preset Color) 
		presetcolorvalMedSpringGreen, // (Medium Spring Green Preset Color)
		presetcolorvalMedTurquoise, // (Medium Turquoise Preset Color) 
		presetcolorvalMedVioletRed, // (Medium Violet Red Preset Color) 
		presetcolorvalMidnightBlue, // (Midnight Blue Preset Color) 
		presetcolorvalMintCream, // (Mint Cream Preset Color) 
		presetcolorvalMistyRose, // (Misty Rose Preset Color) 
		presetcolorvalMoccasin, // (Moccasin Preset Color) 
		presetcolorvalNavajoWhite, // (Navajo White Preset Color) 
		presetcolorvalNavy, // (Navy Preset Color) 
		presetcolorvalOldLace, // (Old Lace Preset Color) 
		presetcolorvalOlive, // (Olive Preset Color) 
		presetcolorvalOliveDrab, // (Olive Drab Preset Color) 
		presetcolorvalOrange, // (Orange Preset Color) 
		presetcolorvalOrangeRed, // (Orange Red Preset Color) 
		presetcolorvalOrchid, // (Orchid Preset Color) 
		presetcolorvalPaleGoldenrod, // (Pale Goldenrod Preset Color) 
		presetcolorvalPaleGreen, // (Pale Green Preset Color) 
		presetcolorvalPaleTurquoise, // (Pale Turquoise Preset Color) 
		presetcolorvalPaleVioletRed, // (Pale Violet Red Preset Color) 
		presetcolorvalPapayaWhip, // (Papaya Whip Preset Color) 
		presetcolorvalPeachPuff, // (Peach Puff Preset Color) 
		presetcolorvalPeru, // (Peru Preset Color) 
		presetcolorvalPink, // (Pink Preset Color) 
		presetcolorvalPlum, // (Plum Preset Color) 
		presetcolorvalPowderBlue, // (Powder Blue Preset Color) 
		presetcolorvalPurple, // (Purple Preset Color) 
		presetcolorvalRed, // (Red Preset Color) 
		presetcolorvalRosyBrown, // (Rosy Brown Preset Color) 
		presetcolorvalRoyalBlue, // (Royal Blue Preset Color) 
		presetcolorvalSaddleBrown, // (Saddle Brown Preset Color) 
		presetcolorvalSalmon, // (Salmon Preset Color) 
		presetcolorvalSandyBrown, // (Sandy Brown Preset Color) 
		presetcolorvalSeaGreen, // (Sea Green Preset Color) 
		presetcolorvalSeaShell, // (Sea Shell Preset Color) 
		presetcolorvalSienna, // (Sienna Preset Color) 
		presetcolorvalSilver, // (Silver Preset Color) 
		presetcolorvalSkyBlue, // (Sky Blue Preset Color) 
		presetcolorvalSlateBlue, // (Slate Blue Preset Color) 
		presetcolorvalSlateGray, // (Slate Gray Preset Color) 
		presetcolorvalSlateGrey, // (Slate Gray Preset Color) 
		presetcolorvalSnow, // (Snow Preset Color) 
		presetcolorvalSpringGreen, // (Spring Green Preset Color) 
		presetcolorvalSteelBlue, // (Steel Blue Preset Color) 
		presetcolorvalTan, // (Tan Preset Color) 
		presetcolorvalTeal, // (Teal Preset Color)
		presetcolorvalThistle, // (Thistle Preset Color) 
		presetcolorvalTomato, // (Tomato Preset Color) 
		presetcolorvalTurquoise, // (Turquoise Preset Color) 
		presetcolorvalViolet, // (Violet Preset Color) 
		presetcolorvalWheat, // (Wheat Preset Color) 
		presetcolorvalWhite, // (White Preset Color) 
		presetcolorvalWhiteSmoke, // (White Smoke Preset Color) 
		presetcolorvalYellow, // (Yellow Preset Color) 
		presetcolorvalYellowGreen, // (Yellow Green Preset Color) 
	};

	template<EPresetColorVal eDefValue = presetcolorvalBlack>
	class CPresetColorVal : public CSimpleType<EPresetColorVal, eDefValue>
	{
	public:
		CPresetColorVal() {}

        virtual EPresetColorVal FromString(std::wstring &sValue)
		{
            this->m_eValue = presetcolorvalBlack;
			SetRGBA( 0, 0, 0, 255 );

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':
                if      ( (L"aliceBlue")			== sValue ) { this->m_eValue = presetcolorvalAliceBlue; SetRGBA(240,248,255); }
                else if ( (L"antiqueWhite")		== sValue ) { this->m_eValue = presetcolorvalAntiqueWhite; SetRGBA(250,235,215); }
                else if ( (L"aqua")				== sValue ) { this->m_eValue = presetcolorvalAqua; SetRGBA(0,255,255); }
                else if ( (L"aquamarine")			== sValue ) { this->m_eValue = presetcolorvalAquamarine; SetRGBA(127,255,212); }
                else if ( (L"azure")				== sValue ) { this->m_eValue = presetcolorvalAzure; SetRGBA(240,255,255); }
				break;
			case 'b':
                if      ( (L"beige")				== sValue ) { this->m_eValue = presetcolorvalBeige; SetRGBA(245,245,220); }
                else if ( (L"bisque")				== sValue ) { this->m_eValue = presetcolorvalBisque ; SetRGBA(255,228,196); }
                else if ( (L"black")				== sValue ) { this->m_eValue = presetcolorvalBlack ; SetRGBA(0,0,0); }
                else if ( (L"blanchedAlmond")		== sValue ) { this->m_eValue = presetcolorvalBlanchedAlmond ; SetRGBA(255,235,205); }
                else if ( (L"blue")				== sValue ) { this->m_eValue = presetcolorvalBlue ; SetRGBA(0,0,255); }
                else if ( (L"blueViolet")			== sValue ) { this->m_eValue = presetcolorvalBlueViolet ; SetRGBA(138,43,226); }
                else if ( (L"brown")				== sValue ) { this->m_eValue = presetcolorvalBrown ; SetRGBA(165,42,42); }
                else if ( (L"burlyWood")			== sValue ) { this->m_eValue = presetcolorvalBurlyWood ; SetRGBA(222,184,135); }
				break;
			case 'c':
                if      ( (L"cadetBlue")			== sValue ) { this->m_eValue = presetcolorvalCadetBlue ; SetRGBA(95,158,160); }
                else if ( (L"chartreuse")			== sValue ) { this->m_eValue = presetcolorvalChartreuse ; SetRGBA(127,255,0); }
                else if ( (L"chocolate")			== sValue ) { this->m_eValue = presetcolorvalChocolate ; SetRGBA(210,105,30); }
                else if ( (L"coral")				== sValue ) { this->m_eValue = presetcolorvalCoral ; SetRGBA(255,127,80); }
                else if ( (L"cornflowerBlue")		== sValue ) { this->m_eValue = presetcolorvalCornflowerBlue ; SetRGBA(100,149,237); }
                else if ( (L"cornsilk")			== sValue ) { this->m_eValue = presetcolorvalCornsilk ; SetRGBA(255,248,220); }
                else if ( (L"crimson")				== sValue ) { this->m_eValue = presetcolorvalCrimson ; SetRGBA(220,20,60); }
                else if ( (L"cyan")				== sValue ) { this->m_eValue = presetcolorvalCyan ; SetRGBA(0,255,255); }
				break;
			case 'd':
                if      ( (L"darkBlue")			== sValue ) { this->m_eValue = presetcolorvalDarkBlue ; SetRGBA(0,0,139); }
                else if ( (L"darkCyan")			== sValue ) { this->m_eValue = presetcolorvalDarkCyan ; SetRGBA(0,139,139); }
                else if ( (L"darkGoldenrod")		== sValue ) { this->m_eValue = presetcolorvalDarkGoldenrod ; SetRGBA(184,134,11); }
                else if ( (L"darkGray")			== sValue ) { this->m_eValue = presetcolorvalDarkGray ; SetRGBA(169,169,169); }
                else if ( (L"darkGreen")			== sValue ) { this->m_eValue = presetcolorvalDarkGreen ; SetRGBA(0,100,0); }
                else if ( (L"darkGrey")			== sValue ) { this->m_eValue = presetcolorvalDarkGrey ; SetRGBA(169,169,169); }
                else if ( (L"darkKhaki")			== sValue ) { this->m_eValue = presetcolorvalDarkKhaki ; SetRGBA(189,183,107); }
                else if ( (L"darkMagenta")			== sValue ) { this->m_eValue = presetcolorvalDarkMagenta ; SetRGBA(139,0,139); }
                else if ( (L"darkOliveGreen")		== sValue ) { this->m_eValue = presetcolorvalDarkOliveGreen ; SetRGBA(85,107,47); }
                else if ( (L"darkOrange")			== sValue ) { this->m_eValue = presetcolorvalDarkOrange ; SetRGBA(255,140,0); }
                else if ( (L"darkOrchid")			== sValue ) { this->m_eValue = presetcolorvalDarkOrchid ; SetRGBA(153,50,204); }
                else if ( (L"darkRed")				== sValue ) { this->m_eValue = presetcolorvalDarkRed ; SetRGBA(139,0,0); }
                else if ( (L"darkSalmon")			== sValue ) { this->m_eValue = presetcolorvalDarkSalmon ; SetRGBA(233,150,122); }
                else if ( (L"darkSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalDarkSeaGreen ; SetRGBA(143,188,143); }
                else if ( (L"darkSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateBlue ; SetRGBA(72,61,139); }
                else if ( (L"darkSlateGray")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateGray ; SetRGBA(47,79,79); }
                else if ( (L"darkSlateGrey")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateGrey ; SetRGBA(47,79,79); }
                else if ( (L"darkTurquoise")		== sValue ) { this->m_eValue = presetcolorvalDarkTurquoise ; SetRGBA(0,206,209); }
                else if ( (L"darkViolet")			== sValue ) { this->m_eValue = presetcolorvalDarkViolet ; SetRGBA(148,0,211); }
                else if ( (L"deepPink")			== sValue ) { this->m_eValue = presetcolorvalDeepPink ; SetRGBA(255,20,147); }
                else if ( (L"deepSkyBlue")			== sValue ) { this->m_eValue = presetcolorvalDeepSkyBlue ; SetRGBA(0,191,255); }
                else if ( (L"dimGray")				== sValue ) { this->m_eValue = presetcolorvalDimGray ; SetRGBA(105,105,105); }
                else if ( (L"dimGrey")				== sValue ) { this->m_eValue = presetcolorvalDimGrey ; SetRGBA(105,105,105); }
                else if ( (L"dkBlue")				== sValue ) { this->m_eValue = presetcolorvalDkBlue ; SetRGBA(0,0,139); }
                else if ( (L"dkCyan")				== sValue ) { this->m_eValue = presetcolorvalDkCyan ; SetRGBA(0,139,139); }
                else if ( (L"dkGoldenrod")			== sValue ) { this->m_eValue = presetcolorvalDkGoldenrod ; SetRGBA(184,134,11); }
                else if ( (L"dkGray")				== sValue ) { this->m_eValue = presetcolorvalDkGray ; SetRGBA(169,169,169); }
                else if ( (L"dkGreen")				== sValue ) { this->m_eValue = presetcolorvalDkGreen ; SetRGBA(0,100,0); }
                else if ( (L"dkGrey")				== sValue ) { this->m_eValue = presetcolorvalDkGrey ; SetRGBA(169,169,169); }
                else if ( (L"dkKhaki")				== sValue ) { this->m_eValue = presetcolorvalDkKhaki ; SetRGBA(189,183,107); }
                else if ( (L"dkMagenta")			== sValue ) { this->m_eValue = presetcolorvalDkMagenta ; SetRGBA(139,0,139); }
                else if ( (L"dkOliveGreen")		== sValue ) { this->m_eValue = presetcolorvalDkOliveGreen ; SetRGBA(85,107,47); }
                else if ( (L"dkOrange")			== sValue ) { this->m_eValue = presetcolorvalDkOrange ; SetRGBA(255,140,0); }
                else if ( (L"dkOrchid")			== sValue ) { this->m_eValue = presetcolorvalDkOrchid ; SetRGBA(153,50,204); }
                else if ( (L"dkRed")				== sValue ) { this->m_eValue = presetcolorvalDkRed ; SetRGBA(139,0,0); }
                else if ( (L"dkSalmon")			== sValue ) { this->m_eValue = presetcolorvalDkSalmon ; SetRGBA(233,150,122); }
                else if ( (L"dkSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalDkSeaGreen ; SetRGBA(143,188,139); }
                else if ( (L"dkSlateBlue")			== sValue ) { this->m_eValue = presetcolorvalDkSlateBlue ; SetRGBA(72,61,139); }
                else if ( (L"dkSlateGray")			== sValue ) { this->m_eValue = presetcolorvalDkSlateGray ; SetRGBA(47,79,79); }
                else if ( (L"dkSlateGrey")			== sValue ) { this->m_eValue = presetcolorvalDkSlateGrey ; SetRGBA(47,79,79); }
                else if ( (L"dkTurquoise")			== sValue ) { this->m_eValue = presetcolorvalDkTurquoise ; SetRGBA(0,206,209); }
                else if ( (L"dkViolet")			== sValue ) { this->m_eValue = presetcolorvalDkViolet ; SetRGBA(148,0,211); }
                else if ( (L"dodgerBlue")			== sValue ) { this->m_eValue = presetcolorvalDodgerBlue ; SetRGBA(30,144,255); }
				break;
			case 'f':
                if      ( (L"firebrick")			== sValue ) { this->m_eValue = presetcolorvalFirebrick ; SetRGBA(178,34,34); }
                else if ( (L"floralWhite")			== sValue ) { this->m_eValue = presetcolorvalFloralWhite ; SetRGBA(255,250,240); }
                else if ( (L"forestGreen")			== sValue ) { this->m_eValue = presetcolorvalForestGreen ; SetRGBA(34,139,34); }
                else if ( (L"fuchsia")				== sValue ) { this->m_eValue = presetcolorvalFuchsia ; SetRGBA(255,0,255); }
				break;
			case 'g':
                if      ( (L"gainsboro")			== sValue ) { this->m_eValue = presetcolorvalGainsboro ; SetRGBA(220,220,220); }
                else if ( (L"ghostWhite")			== sValue ) { this->m_eValue = presetcolorvalGhostWhite ; SetRGBA(248,248,255); }
                else if ( (L"gold")				== sValue ) { this->m_eValue = presetcolorvalGold ; SetRGBA(255,215,0); }
                else if ( (L"goldenrod")			== sValue ) { this->m_eValue = presetcolorvalGoldenrod ; SetRGBA(218,165,32); }
                else if ( (L"gray")				== sValue ) { this->m_eValue = presetcolorvalGray ; SetRGBA(128,128,128); }
                else if ( (L"green")				== sValue ) { this->m_eValue = presetcolorvalGreen ; SetRGBA(0,128,0); }
                else if ( (L"greenYellow")			== sValue ) { this->m_eValue = presetcolorvalGreenYellow ; SetRGBA(173,255,47); }
                else if ( (L"grey")				== sValue ) { this->m_eValue = presetcolorvalGrey ; SetRGBA(128,128,128); }
				break;
			case 'h':
                if      ( (L"honeydew")			== sValue ) { this->m_eValue = presetcolorvalHoneydew ; SetRGBA(240,255,240); }
                else if ( (L"hotPink")				== sValue ) { this->m_eValue = presetcolorvalHotPink ; SetRGBA(255,105,180); }
				break;
			case 'i':
                if      ( (L"indianRed")			== sValue ) { this->m_eValue = presetcolorvalIndianRed ; SetRGBA(205,92,92); }
                else if ( (L"indigo")				== sValue ) { this->m_eValue = presetcolorvalIndigo ; SetRGBA(75,0,130); }
                else if ( (L"ivory")				== sValue ) { this->m_eValue = presetcolorvalIvory ; SetRGBA(255,255,240); }
				break;
			case 'k':
                if      ( (L"khaki")				== sValue ) { this->m_eValue = presetcolorvalKhaki ; SetRGBA(240,230,140); }
				break;
			case 'l':
                if      ( (L"lavender")			== sValue ) { this->m_eValue = presetcolorvalLavender ; SetRGBA(230,230,250); }
                else if ( (L"lavenderBlush")		== sValue ) { this->m_eValue = presetcolorvalLavenderBlush ; SetRGBA(255,240,245); }
                else if ( (L"lawnGreen")			== sValue ) { this->m_eValue = presetcolorvalLawnGreen ; SetRGBA(124,252,0); }
                else if ( (L"lemonChiffon")		== sValue ) { this->m_eValue = presetcolorvalLemonChiffon ; SetRGBA(255,250,205); }
                else if ( (L"lightBlue")			== sValue ) { this->m_eValue = presetcolorvalLightBlue ; SetRGBA(173,216,230); }
                else if ( (L"lightCoral")			== sValue ) { this->m_eValue = presetcolorvalLightCoral ; SetRGBA(240,128,128); }
                else if ( (L"lightCyan")			== sValue ) { this->m_eValue = presetcolorvalLightCyan ; SetRGBA(224,255,255); }
                else if ( (L"lightGoldenrodYellow")== sValue ) { this->m_eValue = presetcolorvalLightGoldenrodYellow; SetRGBA(250,250,210); }
                else if ( (L"lightGray")			== sValue ) { this->m_eValue = presetcolorvalLightGray ; SetRGBA(211,211,211); }
                else if ( (L"lightGreen")			== sValue ) { this->m_eValue = presetcolorvalLightGreen ; SetRGBA(144,238,144); }
                else if ( (L"lightGrey")			== sValue ) { this->m_eValue = presetcolorvalLightGrey ; SetRGBA(211,211,211); }
                else if ( (L"lightPink")			== sValue ) { this->m_eValue = presetcolorvalLightPink ; SetRGBA(255,182,193); }
                else if ( (L"lightSalmon")			== sValue ) { this->m_eValue = presetcolorvalLightSalmon ; SetRGBA(255,160,122); }
                else if ( (L"lightSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalLightSeaGreen ; SetRGBA(32,178,170); }
                else if ( (L"lightSkyBlue")		== sValue ) { this->m_eValue = presetcolorvalLightSkyBlue ; SetRGBA(135,206,250); }
                else if ( (L"lightSlateGray")		== sValue ) { this->m_eValue = presetcolorvalLightSlateGray ; SetRGBA(119,136,153); }
                else if ( (L"lightSlateGrey")		== sValue ) { this->m_eValue = presetcolorvalLightSlateGrey ; SetRGBA(119,136,153); }
                else if ( (L"lightSteelBlue")		== sValue ) { this->m_eValue = presetcolorvalLightSteelBlue ; SetRGBA(176,196,222); }
                else if ( (L"lightYellow")			== sValue ) { this->m_eValue = presetcolorvalLightYellow ; SetRGBA(255,255,224); }
                else if ( (L"lime")				== sValue ) { this->m_eValue = presetcolorvalLime ; SetRGBA(0,255,0); }
                else if ( (L"limeGreen")			== sValue ) { this->m_eValue = presetcolorvalLimeGreen ; SetRGBA(50,205,50); }
                else if ( (L"linen")				== sValue ) { this->m_eValue = presetcolorvalLinen; SetRGBA(250,240,230); }
                else if ( (L"ltBlue")				== sValue ) { this->m_eValue = presetcolorvalLtBlue ; SetRGBA(173,216,230); }
                else if ( (L"ltCoral")				== sValue ) { this->m_eValue = presetcolorvalLtCoral ; SetRGBA(240,128,128); }
                else if ( (L"ltCyan")				== sValue ) { this->m_eValue = presetcolorvalLtCyan ; SetRGBA(224,255,255); }
                else if ( (L"ltGoldenrodYellow")	== sValue ) { this->m_eValue = presetcolorvalLtGoldenrodYellow; SetRGBA(250,250,120); }
                else if ( (L"ltGray")				== sValue ) { this->m_eValue = presetcolorvalLtGray ; SetRGBA(211,211,211); }
                else if ( (L"ltGreen")				== sValue ) { this->m_eValue = presetcolorvalLtGreen ; SetRGBA(144,238,144); }
                else if ( (L"ltGrey")				== sValue ) { this->m_eValue = presetcolorvalLtGrey ; SetRGBA(211,211,211); }
                else if ( (L"ltPink")				== sValue ) { this->m_eValue = presetcolorvalLtPink ; SetRGBA(255,182,193); }
                else if ( (L"ltSalmon")			== sValue ) { this->m_eValue = presetcolorvalLtSalmon ; SetRGBA(255,160,122); }
                else if ( (L"ltSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalLtSeaGreen ; SetRGBA(32,178,170); }
                else if ( (L"ltSkyBlue")			== sValue ) { this->m_eValue = presetcolorvalLtSkyBlue ; SetRGBA(135,206,250); }
                else if ( (L"ltSlateGray")			== sValue ) { this->m_eValue = presetcolorvalLtSlateGray ; SetRGBA(119,136,153); }
                else if ( (L"ltSlateGrey")			== sValue ) { this->m_eValue = presetcolorvalLtSlateGrey ; SetRGBA(119,136,153); }
                else if ( (L"ltSteelBlue")			== sValue ) { this->m_eValue = presetcolorvalLtSteelBlue ; SetRGBA(176,196,222); }
                else if ( (L"ltYellow")			== sValue ) { this->m_eValue = presetcolorvalLtYellow ; SetRGBA(255,255,224); }
				break;
			case 'm':
                if      ( (L"magenta")				== sValue ) { this->m_eValue = presetcolorvalMagenta ; SetRGBA(255,0,255); }
                else if ( (L"maroon")				== sValue ) { this->m_eValue = presetcolorvalMaroon ; SetRGBA(128,0,0); }
                else if ( (L"medAquamarine")		== sValue ) { this->m_eValue = presetcolorvalMedAquamarine ; SetRGBA(102,205,170); }
                else if ( (L"medBlue")				== sValue ) { this->m_eValue = presetcolorvalMedBlue ; SetRGBA(0,0,205); }
                else if ( (L"mediumAquamarine")	== sValue ) { this->m_eValue = presetcolorvalMediumAquamarine; SetRGBA(102,205,170); }
                else if ( (L"mediumBlue")			== sValue ) { this->m_eValue = presetcolorvalMediumBlue ; SetRGBA(0,0,205); }
                else if ( (L"mediumOrchid")		== sValue ) { this->m_eValue = presetcolorvalMediumOrchid ; SetRGBA(186,85,211); }
                else if ( (L"mediumPurple")		== sValue ) { this->m_eValue = presetcolorvalMediumPurple ; SetRGBA(147,112,219); }
                else if ( (L"mediumSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalMediumSeaGreen ; SetRGBA(60,179,113); }
                else if ( (L"mediumSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalMediumSlateBlue ; SetRGBA(123,104,238); }
                else if ( (L"mediumSpringGreen")	== sValue ) { this->m_eValue = presetcolorvalMediumSpringGreen; SetRGBA(0,250,154); }
                else if ( (L"mediumTurquoise")		== sValue ) { this->m_eValue = presetcolorvalMediumTurquoise ; SetRGBA(72,209,204); }
                else if ( (L"mediumVioletRed")		== sValue ) { this->m_eValue = presetcolorvalMediumVioletRed ; SetRGBA(199,21,133); }
                else if ( (L"medOrchid")			== sValue ) { this->m_eValue = presetcolorvalMedOrchid ; SetRGBA(186,85,211); }
                else if ( (L"medPurple")			== sValue ) { this->m_eValue = presetcolorvalMedPurple ; SetRGBA(147,112,219); }
                else if ( (L"medSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalMedSeaGreen ; SetRGBA(60,179,113); }
                else if ( (L"medSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalMedSlateBlue ; SetRGBA(123,104,238); }
                else if ( (L"medSpringGreen")		== sValue ) { this->m_eValue = presetcolorvalMedSpringGreen ; SetRGBA(0,250,154); }
                else if ( (L"medTurquoise")		== sValue ) { this->m_eValue = presetcolorvalMedTurquoise ; SetRGBA(72,209,204); }
                else if ( (L"medVioletRed")		== sValue ) { this->m_eValue = presetcolorvalMedVioletRed ; SetRGBA(199,21,133); }
                else if ( (L"midnightBlue")		== sValue ) { this->m_eValue = presetcolorvalMidnightBlue ; SetRGBA(25,25,112); }
                else if ( (L"mintCream")			== sValue ) { this->m_eValue = presetcolorvalMintCream ; SetRGBA(245,255,250); }
                else if ( (L"mistyRose")			== sValue ) { this->m_eValue = presetcolorvalMistyRose ; SetRGBA(255,228,225); }
                else if ( (L"moccasin")			== sValue ) { this->m_eValue = presetcolorvalMoccasin ; SetRGBA(255,228,181); }
				break;
			case 'n':
                if      ( (L"navajoWhite")			== sValue ) { this->m_eValue = presetcolorvalNavajoWhite ; SetRGBA(255,222,173); }
                else if ( (L"navy")				== sValue ) { this->m_eValue = presetcolorvalNavy ; SetRGBA(0,0,128); }
				break;
			case 'o':
                if      ( (L"oldLace")				== sValue ) { this->m_eValue = presetcolorvalOldLace ; SetRGBA(253,245,230); }
                else if ( (L"olive")				== sValue ) { this->m_eValue = presetcolorvalOlive ; SetRGBA(128,128,0); }
                else if ( (L"oliveDrab")			== sValue ) { this->m_eValue = presetcolorvalOliveDrab ; SetRGBA(107,142,35); }
                else if ( (L"orange")				== sValue ) { this->m_eValue = presetcolorvalOrange ; SetRGBA(255,165,0); }
                else if ( (L"orangeRed")			== sValue ) { this->m_eValue = presetcolorvalOrangeRed ; SetRGBA(255,69,0); }
                else if ( (L"orchid")				== sValue ) { this->m_eValue = presetcolorvalOrchid ; SetRGBA(218,112,214); }
				break;
			case 'p':
                if      ( (L"paleGoldenrod")		== sValue ) { this->m_eValue = presetcolorvalPaleGoldenrod ; SetRGBA(238,232,170); }
                else if ( (L"paleGreen")			== sValue ) { this->m_eValue = presetcolorvalPaleGreen ; SetRGBA(152,251,152); }
                else if ( (L"paleTurquoise")		== sValue ) { this->m_eValue = presetcolorvalPaleTurquoise ; SetRGBA(175,238,238); }
                else if ( (L"paleVioletRed")		== sValue ) { this->m_eValue = presetcolorvalPaleVioletRed ; SetRGBA(219,112,147); }
                else if ( (L"papayaWhip")			== sValue ) { this->m_eValue = presetcolorvalPapayaWhip ; SetRGBA(255,239,213); }
                else if ( (L"peachPuff")			== sValue ) { this->m_eValue = presetcolorvalPeachPuff ; SetRGBA(255,218,185); }
                else if ( (L"peru")				== sValue ) { this->m_eValue = presetcolorvalPeru ; SetRGBA(205,133,63); }
                else if ( (L"pink")				== sValue ) { this->m_eValue = presetcolorvalPink ; SetRGBA(255,192,203); }
                else if ( (L"plum")				== sValue ) { this->m_eValue = presetcolorvalPlum ; SetRGBA(221,160,221); }
                else if ( (L"powderBlue")			== sValue ) { this->m_eValue = presetcolorvalPowderBlue ; SetRGBA(176,224,230); }
                else if ( (L"purple")				== sValue ) { this->m_eValue = presetcolorvalPurple ; SetRGBA(128,0,128); }
				break;
			case 'r':
                if      ( (L"red")					== sValue ) { this->m_eValue = presetcolorvalRed ; SetRGBA(255,0,0); }
                else if ( (L"rosyBrown")			== sValue ) { this->m_eValue = presetcolorvalRosyBrown ; SetRGBA(188,143,143); }
                else if ( (L"royalBlue")			== sValue ) { this->m_eValue = presetcolorvalRoyalBlue ; SetRGBA(65,105,225); }
				break;
			case 's':
                if      ( (L"saddleBrown")			== sValue ) { this->m_eValue = presetcolorvalSaddleBrown ; SetRGBA(139,69,19); }
                else if ( (L"salmon")				== sValue ) { this->m_eValue = presetcolorvalSalmon ; SetRGBA(250,128,114); }
                else if ( (L"sandyBrown")			== sValue ) { this->m_eValue = presetcolorvalSandyBrown ; SetRGBA(244,164,96); }
                else if ( (L"seaGreen")			== sValue ) { this->m_eValue = presetcolorvalSeaGreen ; SetRGBA(46,139,87); }
                else if ( (L"seaShell")			== sValue ) { this->m_eValue = presetcolorvalSeaShell ; SetRGBA(255,245,238); }
                else if ( (L"sienna")				== sValue ) { this->m_eValue = presetcolorvalSienna ; SetRGBA(160,82,45); }
                else if ( (L"silver")				== sValue ) { this->m_eValue = presetcolorvalSilver ; SetRGBA(192,192,192); }
                else if ( (L"skyBlue")				== sValue ) { this->m_eValue = presetcolorvalSkyBlue ; SetRGBA(135,206,235); }
                else if ( (L"slateBlue")			== sValue ) { this->m_eValue = presetcolorvalSlateBlue ; SetRGBA(106,90,205); }
                else if ( (L"slateGray")			== sValue ) { this->m_eValue = presetcolorvalSlateGray ; SetRGBA(112,128,144); }
                else if ( (L"slateGrey")			== sValue ) { this->m_eValue = presetcolorvalSlateGrey ; SetRGBA(112,128,144); }
                else if ( (L"snow")				== sValue ) { this->m_eValue = presetcolorvalSnow ; SetRGBA(255,250,250); }
                else if ( (L"springGreen")			== sValue ) { this->m_eValue = presetcolorvalSpringGreen ; SetRGBA(0,255,127); }
                else if ( (L"steelBlue")			== sValue ) { this->m_eValue = presetcolorvalSteelBlue ; SetRGBA(70,130,180); }
				break;
			case 't':
                if      ( (L"tan")					== sValue ) { this->m_eValue = presetcolorvalTan ; SetRGBA(210,180,140); }
                else if ( (L"teal")				== sValue ) { this->m_eValue = presetcolorvalTeal ; SetRGBA(0,128,128); }
                else if ( (L"thistle")				== sValue ) { this->m_eValue = presetcolorvalThistle ; SetRGBA(216,191,216); }
                else if ( (L"tomato")				== sValue ) { this->m_eValue = presetcolorvalTomato ; SetRGBA(255,99,71); }
                else if ( (L"turquoise")			== sValue ) { this->m_eValue = presetcolorvalTurquoise ; SetRGBA(64,224,208); }
				break;
			case 'v':
                if      ( (L"violet")				== sValue ) { this->m_eValue = presetcolorvalViolet ; SetRGBA(238,130,238); }
				break;
			case 'w':
                if      ( (L"wheat")				== sValue ) { this->m_eValue = presetcolorvalWheat ; SetRGBA(245,222,179); }
                else if ( (L"white")				== sValue ) { this->m_eValue = presetcolorvalWhite ; SetRGBA(255,255,255); }
                else if ( (L"whiteSmoke")			== sValue ) { this->m_eValue = presetcolorvalWhiteSmoke ; SetRGBA(245,245,245); }
				break;
			case 'y':
                if      ( (L"yellow")				== sValue ) { this->m_eValue = presetcolorvalYellow ; SetRGBA(255,255,0); }
                else if ( (L"yellowGreen")			== sValue ) { this->m_eValue = presetcolorvalYellowGreen ; SetRGBA(154,205,50); }
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetcolorvalAliceBlue:			return (L"aliceBlue");			
			case presetcolorvalAntiqueWhite:		return (L"antiqueWhite");		
			case presetcolorvalAqua:				return (L"aqua");				
			case presetcolorvalAquamarine:			return (L"aquamarine");			
			case presetcolorvalAzure:				return (L"azure");				
			case presetcolorvalBeige:				return (L"beige");				
			case presetcolorvalBisque:				return (L"bisque");				
			case presetcolorvalBlack :				return (L"black");				
			case presetcolorvalBlanchedAlmond :		return (L"blanchedAlmond");		
			case presetcolorvalBlue :				return (L"blue");				
			case presetcolorvalBlueViolet :			return (L"blueViolet");			
			case presetcolorvalBrown :				return (L"brown");				
			case presetcolorvalBurlyWood :			return (L"burlyWood");			
			case presetcolorvalCadetBlue :			return (L"cadetBlue");			
			case presetcolorvalChartreuse :			return (L"chartreuse");			
			case presetcolorvalChocolate :			return (L"chocolate");			
			case presetcolorvalCoral :				return (L"coral");				
			case presetcolorvalCornflowerBlue :		return (L"cornflowerBlue");		
			case presetcolorvalCornsilk :			return (L"cornsilk");			
			case presetcolorvalCrimson :			return (L"crimson");			
			case presetcolorvalCyan :				return (L"cyan");				
			case presetcolorvalDarkBlue :			return (L"darkBlue");			
			case presetcolorvalDarkCyan :			return (L"darkCyan");			
			case presetcolorvalDarkGoldenrod :		return (L"darkGoldenrod");		
			case presetcolorvalDarkGray :			return (L"darkGray");			
			case presetcolorvalDarkGreen:			return (L"darkGreen");			
			case presetcolorvalDarkGrey :			return (L"darkGrey");			
			case presetcolorvalDarkKhaki:			return (L"darkKhaki");			
			case presetcolorvalDarkMagenta :		return (L"darkMagenta");			
			case presetcolorvalDarkOliveGreen :		return (L"darkOliveGreen");		
			case presetcolorvalDarkOrange :			return (L"darkOrange");			
			case presetcolorvalDarkOrchid :			return (L"darkOrchid");			
			case presetcolorvalDarkRed :			return (L"darkRed");				
			case presetcolorvalDarkSalmon :			return (L"darkSalmon");			
			case presetcolorvalDarkSeaGreen :		return (L"darkSeaGreen");		
			case presetcolorvalDarkSlateBlue :		return (L"darkSlateBlue");		
			case presetcolorvalDarkSlateGray :		return (L"darkSlateGray");		
			case presetcolorvalDarkSlateGrey :		return (L"darkSlateGrey");		
			case presetcolorvalDarkTurquoise :		return (L"darkTurquoise");		
			case presetcolorvalDarkViolet :			return (L"darkViolet");			
			case presetcolorvalDeepPink :			return (L"deepPink");			
			case presetcolorvalDeepSkyBlue :		return (L"deepSkyBlue");			
			case presetcolorvalDimGray :			return (L"dimGray");				
			case presetcolorvalDimGrey :			return (L"dimGrey");				
			case presetcolorvalDkBlue :				return (L"dkBlue");				
			case presetcolorvalDkCyan :				return (L"dkCyan");				
			case presetcolorvalDkGoldenrod :		return (L"dkGoldenrod");			
			case presetcolorvalDkGray :				return (L"dkGray");				
			case presetcolorvalDkGreen :			return (L"dkGreen");				
			case presetcolorvalDkGrey :				return (L"dkGrey");				
			case presetcolorvalDkKhaki :			return (L"dkKhaki");				
			case presetcolorvalDkMagenta :			return (L"dkMagenta");			
			case presetcolorvalDkOliveGreen :		return (L"dkOliveGreen");		
			case presetcolorvalDkOrange :			return (L"dkOrange");			
			case presetcolorvalDkOrchid :			return (L"dkOrchid");			
			case presetcolorvalDkRed :				return (L"dkRed");				
			case presetcolorvalDkSalmon :			return (L"dkSalmon");			
			case presetcolorvalDkSeaGreen :			return (L"dkSeaGreen");			
			case presetcolorvalDkSlateBlue :		return (L"dkSlateBlue");			
			case presetcolorvalDkSlateGray :		return (L"dkSlateGray");			
			case presetcolorvalDkSlateGrey :		return (L"dkSlateGrey");			
			case presetcolorvalDkTurquoise :		return (L"dkTurquoise");			
			case presetcolorvalDkViolet :			return (L"dkViolet");			
			case presetcolorvalDodgerBlue :			return (L"dodgerBlue");			
			case presetcolorvalFirebrick :			return (L"firebrick");			
			case presetcolorvalFloralWhite :		return (L"floralWhite");			
			case presetcolorvalForestGreen :		return (L"forestGreen");			
			case presetcolorvalFuchsia :			return (L"fuchsia");				
			case presetcolorvalGainsboro :			return (L"gainsboro");			
			case presetcolorvalGhostWhite :			return (L"ghostWhite");			
			case presetcolorvalGold :				return (L"gold");				
			case presetcolorvalGoldenrod :			return (L"goldenrod");			
			case presetcolorvalGray :				return (L"gray");				
			case presetcolorvalGreen :				return (L"green");				
			case presetcolorvalGreenYellow :		return (L"greenYellow");			
			case presetcolorvalGrey :				return (L"grey");				
			case presetcolorvalHoneydew :			return (L"honeydew");			
			case presetcolorvalHotPink :			return (L"hotPink");				
			case presetcolorvalIndianRed :			return (L"indianRed");			
			case presetcolorvalIndigo :				return (L"indigo");				
			case presetcolorvalIvory :				return (L"ivory");				
			case presetcolorvalKhaki :				return (L"khaki");				
			case presetcolorvalLavender :			return (L"lavender");			
			case presetcolorvalLavenderBlush :		return (L"lavenderBlush");		
			case presetcolorvalLawnGreen:			return (L"lawnGreen");			
			case presetcolorvalLemonChiffon :		return (L"lemonChiffon");		
			case presetcolorvalLightBlue :			return (L"lightBlue");			
			case presetcolorvalLightCoral :			return (L"lightCoral");			
			case presetcolorvalLightCyan :			return (L"lightCyan");			
			case presetcolorvalLightGoldenrodYellow:return (L"lightGoldenrodYellow");
			case presetcolorvalLightGray :			return (L"lightGray");			
			case presetcolorvalLightGreen :			return (L"lightGreen");			
			case presetcolorvalLightGrey :			return (L"lightGrey");			
			case presetcolorvalLightPink :			return (L"lightPink");			
			case presetcolorvalLightSalmon :		return (L"lightSalmon");			
			case presetcolorvalLightSeaGreen :		return (L"lightSeaGreen");		
			case presetcolorvalLightSkyBlue :		return (L"lightSkyBlue");		
			case presetcolorvalLightSlateGray :		return (L"lightSlateGray");		
			case presetcolorvalLightSlateGrey :		return (L"lightSlateGrey");		
			case presetcolorvalLightSteelBlue :		return (L"lightSteelBlue");		
			case presetcolorvalLightYellow :		return (L"lightYellow");			
			case presetcolorvalLime :				return (L"lime");				
			case presetcolorvalLimeGreen :			return (L"limeGreen");			
			case presetcolorvalLinen:				return (L"linen");				
			case presetcolorvalLtBlue :				return (L"ltBlue");				
			case presetcolorvalLtCoral :			return (L"ltCoral");				
			case presetcolorvalLtCyan :				return (L"ltCyan");				
			case presetcolorvalLtGoldenrodYellow:	return (L"ltGoldenrodYellow");	
			case presetcolorvalLtGray :				return (L"ltGray");				
			case presetcolorvalLtGreen :			return (L"ltGreen");				
			case presetcolorvalLtGrey :				return (L"ltGrey");				
			case presetcolorvalLtPink :				return (L"ltPink");				
			case presetcolorvalLtSalmon :			return (L"ltSalmon");			
			case presetcolorvalLtSeaGreen :			return (L"ltSeaGreen");			
			case presetcolorvalLtSkyBlue :			return (L"ltSkyBlue");			
			case presetcolorvalLtSlateGray :		return (L"ltSlateGray");			
			case presetcolorvalLtSlateGrey :		return (L"ltSlateGrey");			
			case presetcolorvalLtSteelBlue :		return (L"ltSteelBlue");			
			case presetcolorvalLtYellow :			return (L"ltYellow");			
			case presetcolorvalMagenta :			return (L"magenta");				
			case presetcolorvalMaroon :				return (L"maroon");				
			case presetcolorvalMedAquamarine :		return (L"medAquamarine");		
			case presetcolorvalMedBlue :			return (L"medBlue");				
			case presetcolorvalMediumAquamarine:	return (L"mediumAquamarine");	
			case presetcolorvalMediumBlue :			return (L"mediumBlue");			
			case presetcolorvalMediumOrchid :		return (L"mediumOrchid");		
			case presetcolorvalMediumPurple :		return (L"mediumPurple");		
			case presetcolorvalMediumSeaGreen :		return (L"mediumSeaGreen");		
			case presetcolorvalMediumSlateBlue :	return (L"mediumSlateBlue");		
			case presetcolorvalMediumSpringGreen:	return (L"mediumSpringGreen");	
			case presetcolorvalMediumTurquoise :	return (L"mediumTurquoise");		
			case presetcolorvalMediumVioletRed :	return (L"mediumVioletRed");		
			case presetcolorvalMedOrchid :			return (L"medOrchid");			
			case presetcolorvalMedPurple :			return (L"medPurple");			
			case presetcolorvalMedSeaGreen :		return (L"medSeaGreen");			
			case presetcolorvalMedSlateBlue :		return (L"medSlateBlue");		
			case presetcolorvalMedSpringGreen :		return (L"medSpringGreen");		
			case presetcolorvalMedTurquoise :		return (L"medTurquoise");		
			case presetcolorvalMedVioletRed :		return (L"medVioletRed");		
			case presetcolorvalMidnightBlue :		return (L"midnightBlue");		
			case presetcolorvalMintCream :			return (L"mintCream");			
			case presetcolorvalMistyRose :			return (L"mistyRose");			
			case presetcolorvalMoccasin :			return (L"moccasin");			
			case presetcolorvalNavajoWhite :		return (L"navajoWhite");			
			case presetcolorvalNavy :				return (L"navy");				
			case presetcolorvalOldLace :			return (L"oldLace");				
			case presetcolorvalOlive :				return (L"olive");				
			case presetcolorvalOliveDrab :			return (L"oliveDrab");			
			case presetcolorvalOrange :				return (L"orange");				
			case presetcolorvalOrangeRed :			return (L"orangeRed");			
			case presetcolorvalOrchid :				return (L"orchid");				
			case presetcolorvalPaleGoldenrod :		return (L"paleGoldenrod");		
			case presetcolorvalPaleGreen :			return (L"paleGreen");			
			case presetcolorvalPaleTurquoise :		return (L"paleTurquoise");		
			case presetcolorvalPaleVioletRed :		return (L"paleVioletRed");		
			case presetcolorvalPapayaWhip :			return (L"papayaWhip");			
			case presetcolorvalPeachPuff :			return (L"peachPuff");			
			case presetcolorvalPeru :				return (L"peru");				
			case presetcolorvalPink :				return (L"pink");				
			case presetcolorvalPlum :				return (L"plum");				
			case presetcolorvalPowderBlue :			return (L"powderBlue");			
			case presetcolorvalPurple :				return (L"purple");				
			case presetcolorvalRed :				return (L"red");					
			case presetcolorvalRosyBrown :			return (L"rosyBrown");			
			case presetcolorvalRoyalBlue :			return (L"royalBlue");			
			case presetcolorvalSaddleBrown :		return (L"saddleBrown");			
			case presetcolorvalSalmon :				return (L"salmon");				
			case presetcolorvalSandyBrown :			return (L"sandyBrown");			
			case presetcolorvalSeaGreen :			return (L"seaGreen");			
			case presetcolorvalSeaShell :			return (L"seaShell");			
			case presetcolorvalSienna :				return (L"sienna");				
			case presetcolorvalSilver :				return (L"silver");				
			case presetcolorvalSkyBlue :			return (L"skyBlue");				
			case presetcolorvalSlateBlue :			return (L"slateBlue");			
			case presetcolorvalSlateGray :			return (L"slateGray");			
			case presetcolorvalSlateGrey :			return (L"slateGrey");			
			case presetcolorvalSnow :				return (L"snow");				
			case presetcolorvalSpringGreen :		return (L"springGreen");			
			case presetcolorvalSteelBlue :			return (L"steelBlue");			
			case presetcolorvalTan :				return (L"tan");					
			case presetcolorvalTeal :				return (L"teal");				
			case presetcolorvalThistle :			return (L"thistle");				
			case presetcolorvalTomato :				return (L"tomato");				
			case presetcolorvalTurquoise :			return (L"turquoise");			
			case presetcolorvalViolet :				return (L"violet");				
			case presetcolorvalWheat :				return (L"wheat");				
			case presetcolorvalWhite :				return (L"white");				
			case presetcolorvalWhiteSmoke :			return (L"whiteSmoke");			
			case presetcolorvalYellow :				return (L"yellow");				
			case presetcolorvalYellowGreen :		return (L"yellowGreen");			
			default :								return (L"black");
			}
		}

		SimpleType_FromString     (EPresetColorVal)
		SimpleType_Operator_Equal (CPresetColorVal)
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

		void SetRGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255)
		{
			m_unR = unR;
			m_unG = unG;
			m_unB = unB;
			m_unA = unA;
		}

	private:

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;
		unsigned char m_unA;
	};


	//--------------------------------------------------------------------------------
	// PresetLineDashVal 20.1.10.49 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetLineDashVal
	{
		presetlinedashvalDash          = 0,
		presetlinedashvalDashDot       = 1,
		presetlinedashvalDot           = 2,
		presetlinedashvalLgDash        = 3,
		presetlinedashvalLgDashDot     = 4,
		presetlinedashvalLgDashDotDot  = 5,
		presetlinedashvalSolid         = 6,
		presetlinedashvalSysDash       = 7,
		presetlinedashvalSysDashDot    = 8,
		presetlinedashvalSysDashDotDot = 9,
		presetlinedashvalSysDot        = 10,
	};

	template<EPresetLineDashVal eDefValue = presetlinedashvalSolid>
	class CPresetLineDashVal : public CSimpleType<EPresetLineDashVal, eDefValue>
	{
	public:
		CPresetLineDashVal() {}

        virtual EPresetLineDashVal FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'd':
                if      ( (L"dash")          == sValue ) this->m_eValue = presetlinedashvalDash;
                else if ( (L"dashDot")       == sValue ) this->m_eValue = presetlinedashvalDashDot;
                else if ( (L"dot")           == sValue ) this->m_eValue = presetlinedashvalDot;
				break;
			case 'l':
                if      ( (L"lgDash")        == sValue ) this->m_eValue = presetlinedashvalLgDash;
                else if ( (L"lgDashDot")     == sValue ) this->m_eValue = presetlinedashvalLgDashDot;
                else if ( (L"lgDashDotDot")  == sValue ) this->m_eValue = presetlinedashvalLgDashDotDot;
				break;
			case 's':
                if      ( (L"solid")         == sValue ) this->m_eValue = presetlinedashvalSolid;
                else if ( (L"sysDash")       == sValue ) this->m_eValue = presetlinedashvalSysDash;
                else if ( (L"sysDashDot")    == sValue ) this->m_eValue = presetlinedashvalSysDashDot;
                else if ( (L"sysDashDotDot") == sValue ) this->m_eValue = presetlinedashvalSysDashDotDot;
                else if ( (L"sysDot")        == sValue ) this->m_eValue = presetlinedashvalSysDot;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetlinedashvalDash:          return (L"dash");			
			case presetlinedashvalDashDot:       return (L"dashDot");			
			case presetlinedashvalDot:           return (L"dot");		
			case presetlinedashvalLgDash:        return (L"lgDash");			
			case presetlinedashvalLgDashDot:     return (L"lgDashDot");			
			case presetlinedashvalLgDashDotDot:  return (L"lgDashDotDot");		
			case presetlinedashvalSolid:         return (L"solid");			
			case presetlinedashvalSysDash:       return (L"sysDash");			
			case presetlinedashvalSysDashDot:    return (L"sysDashDot");		
			case presetlinedashvalSysDashDotDot: return (L"sysDashDotDot");		
			case presetlinedashvalSysDot:        return (L"sysDot");		
			default :                            return (L"solid");
			}
		}

		SimpleType_FromString     (EPresetLineDashVal)
		SimpleType_Operator_Equal (CPresetLineDashVal)
	};


	//--------------------------------------------------------------------------------
	// PresetMaterialType 20.1.10.50 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetMaterialType
	{
		presetmaterialtypeClear             = 0,
		presetmaterialtypeDkEdge            = 1,
		presetmaterialtypeFlat              = 2,
		presetmaterialtypeLegacyMatte       = 3,
		presetmaterialtypeLegacyMetal       = 4,
		presetmaterialtypeLegacyPlastic     = 5,
		presetmaterialtypeLegacyWireframe   = 6,
		presetmaterialtypeMatte             = 7,
		presetmaterialtypeMetal             = 8,
		presetmaterialtypePlastic           = 9,
		presetmaterialtypePowder            = 10,
		presetmaterialtypeSoftEdge          = 11,
		presetmaterialtypeSoftmetal         = 12,
		presetmaterialtypeTranslucentPowder = 13,
		presetmaterialtypeWarmMatte         = 14,
	};

	template<EPresetMaterialType eDefValue = presetmaterialtypeClear>
	class CPresetMaterialType : public CSimpleType<EPresetMaterialType, eDefValue>
	{
	public:
		CPresetMaterialType() {}

        virtual EPresetMaterialType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"clear")             == sValue ) this->m_eValue = presetmaterialtypeClear;
				break;
			case 'd':
                if      ( (L"dkEdge")            == sValue ) this->m_eValue = presetmaterialtypeDkEdge;
				break;
			case 'f':
                if      ( (L"flat")              == sValue ) this->m_eValue = presetmaterialtypeFlat;
				break;
			case 'l':
                if      ( (L"legacyMatte")       == sValue ) this->m_eValue = presetmaterialtypeLegacyMatte;
                else if ( (L"legacyMetal")       == sValue ) this->m_eValue = presetmaterialtypeLegacyMetal;
                else if ( (L"legacyPlastic")     == sValue ) this->m_eValue = presetmaterialtypeLegacyPlastic;
                else if ( (L"legacyWireframe")   == sValue ) this->m_eValue = presetmaterialtypeLegacyWireframe;
				break;
			case 'm':
                if      ( (L"matte")             == sValue ) this->m_eValue = presetmaterialtypeMatte;
                else if ( (L"metal")             == sValue ) this->m_eValue = presetmaterialtypeMetal;
				break;
			case 'p':
                if      ( (L"plastic")           == sValue ) this->m_eValue = presetmaterialtypePlastic;
                else if ( (L"powder")            == sValue ) this->m_eValue = presetmaterialtypePowder;
				break;
			case 's':
                if      ( (L"softEdge")          == sValue ) this->m_eValue = presetmaterialtypeSoftEdge;
                else if ( (L"softmetal")         == sValue ) this->m_eValue = presetmaterialtypeSoftmetal;
				break;
			case 't':
                if      ( (L"translucentPowder") == sValue ) this->m_eValue = presetmaterialtypeTranslucentPowder;
				break;
			case 'w':
                if      ( (L"warmMatte")         == sValue ) this->m_eValue = presetmaterialtypeWarmMatte;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring             ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetmaterialtypeClear             : return (L"clear");
			case presetmaterialtypeDkEdge            : return (L"dkEdge");
			case presetmaterialtypeFlat              : return (L"flat");
			case presetmaterialtypeLegacyMatte       : return (L"legacyMatte");
			case presetmaterialtypeLegacyMetal       : return (L"legacyMetal"); 
			case presetmaterialtypeLegacyPlastic     : return (L"legacyPlastic");
			case presetmaterialtypeLegacyWireframe   : return (L"legacyWireframe");
			case presetmaterialtypeMatte             : return (L"matte");
			case presetmaterialtypeMetal             : return (L"metal");
			case presetmaterialtypePlastic           : return (L"plastic");
			case presetmaterialtypePowder            : return (L"powder");
			case presetmaterialtypeSoftEdge          : return (L"softEdge"); 
			case presetmaterialtypeSoftmetal         : return (L"softmetal");
			case presetmaterialtypeTranslucentPowder : return (L"translucentPowder");
			case presetmaterialtypeWarmMatte         : return (L"warmMatte");
			default                                  : return (L"clear");
			}
		}

		SimpleType_FromString     (EPresetMaterialType)
		SimpleType_Operator_Equal (CPresetMaterialType)
	};
	//--------------------------------------------------------------------------------
	// PresetPatternVal 20.1.10.51 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetPatternVal
	{
		presetpatternvalCross = 0, // (Cross) 
		presetpatternvalDashDnDiag, // (Dashed Downward Diagonal) 
		presetpatternvalDashHorz, // (Dashed Horizontal) 
		presetpatternvalDashUpDiag, // (Dashed Upward DIagonal) 
		presetpatternvalDashVert, // (Dashed Vertical) 
		presetpatternvalDiagBrick, // (Diagonal Brick) 
		presetpatternvalDiagCross, // (Diagonal Cross) 
		presetpatternvalDivot, // (Divot) 
		presetpatternvalDkDnDiag, // (Dark Downward Diagonal) 
		presetpatternvalDkHorz, // (Dark Horizontal) 
		presetpatternvalDkUpDiag, // (Dark Upward Diagonal) 
		presetpatternvalDkVert, // (Dark Vertical) 
		presetpatternvalDnDiag, // (Downward Diagonal) 
		presetpatternvalDotDmnd, // (Dotted Diamond) 
		presetpatternvalDotGrid, // (Dotted Grid) 
		presetpatternvalHorz, // (Horizontal) 
		presetpatternvalHorzBrick, // (Horizontal Brick) 
		presetpatternvalLgCheck, // (Large Checker Board) 
		presetpatternvalLgConfetti, // (Large Confetti) 
		presetpatternvalLgGrid, // (Large Grid) 
		presetpatternvalLtDnDiag, // (Light Downward Diagonal) 
		presetpatternvalLtHorz, // (Light Horizontal) 
		presetpatternvalLtUpDiag, // (Light Upward Diagonal) 
		presetpatternvalLtVert, // (Light Vertical) 
		presetpatternvalNarHorz, // (Narrow Horizontal) 
		presetpatternvalNarVert, // (Narrow Vertical) 
		presetpatternvalOpenDmnd, // (Open Diamond) 
		presetpatternvalPct10, // (10%) 
		presetpatternvalPct20, // (20%) 
		presetpatternvalPct25, // (25%) 
		presetpatternvalPct30, // (30%) 
		presetpatternvalPct40, // (40%) 
		presetpatternvalPct5, // (5%) 
		presetpatternvalPct50, // (50%) 
		presetpatternvalPct60, // (60%) 
		presetpatternvalPct70, // (70%) 
		presetpatternvalPct75, // (75%) 
		presetpatternvalPct80, // (80%) 
		presetpatternvalPct90, // (90%) 
		presetpatternvalPlaid, // (Plaid) 
		presetpatternvalShingle, // (Shingle) 
		presetpatternvalSmCheck, // (Small Checker Board) 
		presetpatternvalSmConfetti, // (Small Confetti) 
		presetpatternvalSmGrid, // (Small Grid) 
		presetpatternvalSolidDmnd, // (Solid Diamond) 
		presetpatternvalSphere, // (Sphere) 
		presetpatternvalTrellis, // (Trellis) 
		presetpatternvalUpDiag, // (Upward Diagonal) 
		presetpatternvalVert, // (Vertical) 
		presetpatternvalWave, // (Wave) 
		presetpatternvalWdDnDiag, // (Wide Downward Diagonal) 
		presetpatternvalWdUpDiag, // (Wide Upward Diagonal) 
		presetpatternvalWeave, // (Weave) 
		presetpatternvalZigZag, // (Zig Zag) 
	};

	template<EPresetPatternVal eDefValue = presetpatternvalPct10>
	class CPresetPatternVal : public CSimpleType<EPresetPatternVal, eDefValue>
	{
	public:
		CPresetPatternVal() {}
        virtual EPresetPatternVal FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"cross")      == sValue ) this->m_eValue = presetpatternvalCross;
				break;
			case 'd':
                if      ( (L"dashDnDiag") == sValue ) this->m_eValue = presetpatternvalDashDnDiag;
                else if ( (L"dashHorz")   == sValue ) this->m_eValue = presetpatternvalDashHorz;
                else if ( (L"dashUpDiag") == sValue ) this->m_eValue = presetpatternvalDashUpDiag;
                else if ( (L"dashVert")   == sValue ) this->m_eValue = presetpatternvalDashVert;
                else if ( (L"diagBrick")  == sValue ) this->m_eValue = presetpatternvalDiagBrick;
                else if ( (L"diagCross")  == sValue ) this->m_eValue = presetpatternvalDiagCross;
                else if ( (L"divot")      == sValue ) this->m_eValue = presetpatternvalDivot;
                else if ( (L"dkDnDiag")   == sValue ) this->m_eValue = presetpatternvalDkDnDiag;
                else if ( (L"dkHorz")     == sValue ) this->m_eValue = presetpatternvalDkHorz;
                else if ( (L"dkUpDiag")   == sValue ) this->m_eValue = presetpatternvalDkUpDiag;
                else if ( (L"dkVert")     == sValue ) this->m_eValue = presetpatternvalDkVert;
                else if ( (L"dnDiag")     == sValue ) this->m_eValue = presetpatternvalDnDiag;
                else if ( (L"dotDmnd")    == sValue ) this->m_eValue = presetpatternvalDotDmnd;
                else if ( (L"dotGrid")    == sValue ) this->m_eValue = presetpatternvalDotGrid;
				break;
			case 'h':
                if      ( (L"horz")       == sValue ) this->m_eValue = presetpatternvalHorz;
                else if ( (L"horzBrick")  == sValue ) this->m_eValue = presetpatternvalHorzBrick;
				break;
			case 'l':
                if      ( (L"lgCheck")    == sValue ) this->m_eValue = presetpatternvalLgCheck;
                else if ( (L"lgConfetti") == sValue ) this->m_eValue = presetpatternvalLgConfetti;
                else if ( (L"lgGrid")     == sValue ) this->m_eValue = presetpatternvalLgGrid;
                else if ( (L"ltDnDiag")   == sValue ) this->m_eValue = presetpatternvalLtDnDiag;
                else if ( (L"ltHorz")     == sValue ) this->m_eValue = presetpatternvalLtHorz;
                else if ( (L"ltUpDiag")   == sValue ) this->m_eValue = presetpatternvalLtUpDiag;
                else if ( (L"ltVert")     == sValue ) this->m_eValue = presetpatternvalLtVert;
				break;
			case 'n':
                if      ( (L"narHorz")    == sValue ) this->m_eValue = presetpatternvalNarHorz;
                else if ( (L"narVert")    == sValue ) this->m_eValue = presetpatternvalNarVert;
				break;
			case 'o':
                if      ( (L"openDmnd")   == sValue ) this->m_eValue = presetpatternvalOpenDmnd;
				break;
			case 'p':
                if      ( (L"pct10")      == sValue ) this->m_eValue = presetpatternvalPct10;
                else if ( (L"pct20")      == sValue ) this->m_eValue = presetpatternvalPct20;
                else if ( (L"pct25")      == sValue ) this->m_eValue = presetpatternvalPct25;
                else if ( (L"pct30")      == sValue ) this->m_eValue = presetpatternvalPct30;
                else if ( (L"pct40")      == sValue ) this->m_eValue = presetpatternvalPct40;
                else if ( (L"pct5")       == sValue ) this->m_eValue = presetpatternvalPct5;
                else if ( (L"pct50")      == sValue ) this->m_eValue = presetpatternvalPct50;
                else if ( (L"pct60")      == sValue ) this->m_eValue = presetpatternvalPct60;
                else if ( (L"pct70")      == sValue ) this->m_eValue = presetpatternvalPct70;
                else if ( (L"pct75")      == sValue ) this->m_eValue = presetpatternvalPct75;
                else if ( (L"pct80")      == sValue ) this->m_eValue = presetpatternvalPct80;
                else if ( (L"pct90")      == sValue ) this->m_eValue = presetpatternvalPct90;
                else if ( (L"plaid")      == sValue ) this->m_eValue = presetpatternvalPlaid;
				break;
			case 's':
                if      ( (L"shingle")    == sValue ) this->m_eValue = presetpatternvalShingle;
                else if ( (L"smCheck")    == sValue ) this->m_eValue = presetpatternvalSmCheck;
                else if ( (L"smConfetti") == sValue ) this->m_eValue = presetpatternvalSmConfetti;
                else if ( (L"smGrid")     == sValue ) this->m_eValue = presetpatternvalSmGrid;
                else if ( (L"solidDmnd")  == sValue ) this->m_eValue = presetpatternvalSolidDmnd;
                else if ( (L"sphere")     == sValue ) this->m_eValue = presetpatternvalSphere;
				break;
			case 't':
                if      ( (L"trellis")    == sValue ) this->m_eValue = presetpatternvalTrellis;
				break;
			case 'u':
                if      ( (L"upDiag")     == sValue ) this->m_eValue = presetpatternvalUpDiag;
				break;
			case 'v':
                if      ( (L"vert")       == sValue ) this->m_eValue = presetpatternvalVert;
				break;
			case 'w':
                if      ( (L"wave")       == sValue ) this->m_eValue = presetpatternvalWave;
                else if ( (L"wdDnDiag")   == sValue ) this->m_eValue = presetpatternvalWdDnDiag;
                else if ( (L"wdUpDiag")   == sValue ) this->m_eValue = presetpatternvalWdUpDiag;
                else if ( (L"weave")      == sValue ) this->m_eValue = presetpatternvalWeave;
				break;
			case 'z':
                if      ( (L"zigZag")     == sValue ) this->m_eValue = presetpatternvalZigZag;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetpatternvalCross:      return (L"cross");
			case presetpatternvalDashDnDiag: return (L"dashDnDiag");
			case presetpatternvalDashHorz:   return (L"dashHorz");
			case presetpatternvalDashUpDiag: return (L"dashUpDiag");
			case presetpatternvalDashVert:   return (L"dashVert");
			case presetpatternvalDiagBrick:  return (L"diagBrick");
			case presetpatternvalDiagCross:  return (L"diagCross");
			case presetpatternvalDivot:      return (L"divot");
			case presetpatternvalDkDnDiag:   return (L"dkDnDiag");
			case presetpatternvalDkHorz:     return (L"dkHorz");
			case presetpatternvalDkUpDiag:   return (L"dkUpDiag");
			case presetpatternvalDkVert:     return (L"dkVert");
			case presetpatternvalDnDiag:     return (L"dnDiag");
			case presetpatternvalDotDmnd:    return (L"dotDmnd");
			case presetpatternvalDotGrid:    return (L"dotGrid");
			case presetpatternvalHorz:       return (L"horz");
			case presetpatternvalHorzBrick:  return (L"horzBrick");
			case presetpatternvalLgCheck:    return (L"lgCheck");
			case presetpatternvalLgConfetti: return (L"lgConfetti");
			case presetpatternvalLgGrid:     return (L"lgGrid");
			case presetpatternvalLtDnDiag:   return (L"ltDnDiag");
			case presetpatternvalLtHorz:     return (L"ltHorz");
			case presetpatternvalLtUpDiag:   return (L"ltUpDiag");
			case presetpatternvalLtVert:     return (L"ltVert");
			case presetpatternvalNarHorz:    return (L"narHorz");
			case presetpatternvalNarVert:    return (L"narVert");
			case presetpatternvalOpenDmnd:   return (L"openDmnd");
			case presetpatternvalPct10:      return (L"pct10");
			case presetpatternvalPct20:      return (L"pct20");
			case presetpatternvalPct25:      return (L"pct25");
			case presetpatternvalPct30:      return (L"pct30");
			case presetpatternvalPct40:      return (L"pct40");
			case presetpatternvalPct5:       return (L"pct5");
			case presetpatternvalPct50:      return (L"pct50");
			case presetpatternvalPct60:      return (L"pct60");
			case presetpatternvalPct70:      return (L"pct70");
			case presetpatternvalPct75:      return (L"pct75");
			case presetpatternvalPct80:      return (L"pct80");
			case presetpatternvalPct90:      return (L"pct90");
			case presetpatternvalPlaid:      return (L"plaid");
			case presetpatternvalShingle:    return (L"shingle");
			case presetpatternvalSmCheck:    return (L"smCheck");
			case presetpatternvalSmConfetti: return (L"smConfetti");
			case presetpatternvalSmGrid:     return (L"smGrid");
			case presetpatternvalSolidDmnd:  return (L"solidDmnd");
			case presetpatternvalSphere:     return (L"sphere");
			case presetpatternvalTrellis:    return (L"trellis");
			case presetpatternvalUpDiag:     return (L"upDiag");
			case presetpatternvalVert:       return (L"vert");
			case presetpatternvalWave:       return (L"wave");
			case presetpatternvalWdDnDiag:   return (L"wdDnDiag");
			case presetpatternvalWdUpDiag:   return (L"wdUpDiag");
			case presetpatternvalWeave:      return (L"weave");
			case presetpatternvalZigZag:     return (L"zigZag Zag");
			default :                        return (L"pct10");
			}
		}

		SimpleType_FromString     (EPresetPatternVal)
		SimpleType_Operator_Equal (CPresetPatternVal)
	};


	//--------------------------------------------------------------------------------
	// PresetShadowVal 20.1.10.52 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EPresetShadowVal
	{
		presetshadowvalShdw1  = 1,
		presetshadowvalShdw10 = 10,
		presetshadowvalShdw11 = 11,
		presetshadowvalShdw12 = 12,
		presetshadowvalShdw13 = 13,
		presetshadowvalShdw14 = 14,
		presetshadowvalShdw15 = 15,
		presetshadowvalShdw16 = 16,
		presetshadowvalShdw17 = 17,
		presetshadowvalShdw18 = 18,
		presetshadowvalShdw19 = 19,
		presetshadowvalShdw2  = 2,
		presetshadowvalShdw20 = 20,
		presetshadowvalShdw3  = 3,
		presetshadowvalShdw4  = 4,
		presetshadowvalShdw5  = 5,
		presetshadowvalShdw6  = 6,
		presetshadowvalShdw7  = 7,
		presetshadowvalShdw8  = 8,
		presetshadowvalShdw9  = 9,
	};

	template<EPresetShadowVal eDefValue = presetshadowvalShdw14>
	class CPresetShadowVal : public CSimpleType<EPresetShadowVal, eDefValue>
	{
	public:
		CPresetShadowVal() {}
        virtual EPresetShadowVal FromString(std::wstring &sValue)
		{
            if      ( (L"shdw1")  == sValue ) this->m_eValue = presetshadowvalShdw1;
            else if ( (L"shdw10") == sValue ) this->m_eValue = presetshadowvalShdw10;
            else if ( (L"shdw11") == sValue ) this->m_eValue = presetshadowvalShdw11;
            else if ( (L"shdw12") == sValue ) this->m_eValue = presetshadowvalShdw12;
            else if ( (L"shdw13") == sValue ) this->m_eValue = presetshadowvalShdw13;
            else if ( (L"shdw14") == sValue ) this->m_eValue = presetshadowvalShdw14;
            else if ( (L"shdw15") == sValue ) this->m_eValue = presetshadowvalShdw15;
            else if ( (L"shdw16") == sValue ) this->m_eValue = presetshadowvalShdw16;
            else if ( (L"shdw17") == sValue ) this->m_eValue = presetshadowvalShdw17;
            else if ( (L"shdw18") == sValue ) this->m_eValue = presetshadowvalShdw18;
            else if ( (L"shdw19") == sValue ) this->m_eValue = presetshadowvalShdw19;
            else if ( (L"shdw2")  == sValue ) this->m_eValue = presetshadowvalShdw2;
            else if ( (L"shdw20") == sValue ) this->m_eValue = presetshadowvalShdw20;
            else if ( (L"shdw3")  == sValue ) this->m_eValue = presetshadowvalShdw3;
            else if ( (L"shdw4")  == sValue ) this->m_eValue = presetshadowvalShdw4;
            else if ( (L"shdw5")  == sValue ) this->m_eValue = presetshadowvalShdw5;
            else if ( (L"shdw6")  == sValue ) this->m_eValue = presetshadowvalShdw6;
            else if ( (L"shdw7")  == sValue ) this->m_eValue = presetshadowvalShdw7;
            else if ( (L"shdw8")  == sValue ) this->m_eValue = presetshadowvalShdw8;
            else if ( (L"shdw9")  == sValue ) this->m_eValue = presetshadowvalShdw9;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring          ToString  () const
		{
            switch(this->m_eValue)
			{
			case presetshadowvalShdw1:  return (L"shdw1");			
			case presetshadowvalShdw2:  return (L"shdw2");		
			case presetshadowvalShdw3:  return (L"shdw3");				
			case presetshadowvalShdw4:  return (L"shdw4");			
			case presetshadowvalShdw5:  return (L"shdw5");				
			case presetshadowvalShdw6:  return (L"shdw6");				
			case presetshadowvalShdw7:  return (L"shdw7");				
			case presetshadowvalShdw8:  return (L"shdw8");				
			case presetshadowvalShdw9:  return (L"shdw9");		
			case presetshadowvalShdw10: return (L"shdw10");				
			case presetshadowvalShdw11: return (L"shdw11");			
			case presetshadowvalShdw12: return (L"shdw12");				
			case presetshadowvalShdw13: return (L"shdw13");			
			case presetshadowvalShdw14: return (L"shdw14");			
			case presetshadowvalShdw15: return (L"shdw15");			
			case presetshadowvalShdw16: return (L"shdw16");			
			case presetshadowvalShdw17: return (L"shdw17");				
			case presetshadowvalShdw18: return (L"shdw18");		
			case presetshadowvalShdw19: return (L"shdw19");			
			case presetshadowvalShdw20: return (L"shdw20");			
			default :                   return (L"shdw14");
			}
		}

		SimpleType_FromString     (EPresetShadowVal)
		SimpleType_Operator_Equal (CPresetShadowVal)
	};


	//--------------------------------------------------------------------------------
	// RectAlignment 20.1.10.53 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ERectAlignment
	{
		rectalignmentB   = 0,
		rectalignmentBL  = 1,
		rectalignmentBR  = 2,
		rectalignmentCtr = 3,
		rectalignmentL   = 4,
		rectalignmentR   = 5,
		rectalignmentT   = 6,
		rectalignmentTL  = 7,
		rectalignmentTR  = 8,
	};

	template<ERectAlignment eDefValue = rectalignmentBL>
	class CRectAlignment : public CSimpleType<ERectAlignment, eDefValue>
	{
	public:
		CRectAlignment() {}

        virtual ERectAlignment FromString(std::wstring &sValue)
		{
            if      ( (L"b")   == sValue ) this->m_eValue = rectalignmentB;
            else if ( (L"bl")  == sValue ) this->m_eValue = rectalignmentBL;
            else if ( (L"br")  == sValue ) this->m_eValue = rectalignmentBR;
            else if ( (L"ctr") == sValue ) this->m_eValue = rectalignmentCtr;
            else if ( (L"l")   == sValue ) this->m_eValue = rectalignmentL;
            else if ( (L"r")   == sValue ) this->m_eValue = rectalignmentR;
            else if ( (L"t")   == sValue ) this->m_eValue = rectalignmentT;
            else if ( (L"tl")  == sValue ) this->m_eValue = rectalignmentTL;
            else if ( (L"tr")  == sValue ) this->m_eValue = rectalignmentTR;
            else                            this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(this->m_eValue)
			{
			case rectalignmentB  : return (L"b");
			case rectalignmentBL : return (L"bl");
			case rectalignmentBR : return (L"br");
			case rectalignmentCtr: return (L"ctr");
			case rectalignmentL  : return (L"l");
			case rectalignmentR  : return (L"r");
			case rectalignmentT  : return (L"t");
			case rectalignmentTL : return (L"tl");
			case rectalignmentTR : return (L"tr");
			default              : return (L"bl");
			}
		}

		SimpleType_FromString     (ERectAlignment)
		SimpleType_Operator_Equal (CRectAlignment)
	};


	//--------------------------------------------------------------------------------
	// ShemeColorVal 20.1.10.54 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EShemeColorVal
	{
		shemecolorvalAccent1  = 0,
		shemecolorvalAccent2  = 1,
		shemecolorvalAccent3  = 2,
		shemecolorvalAccent4  = 3,
		shemecolorvalAccent5  = 4,
		shemecolorvalAccent6  = 5,
		shemecolorvalBg1      = 6,
		shemecolorvalBg2      = 7,
		shemecolorvalDk1      = 8,
		shemecolorvalDk2      = 9,
		shemecolorvalFolHlink = 10,
		shemecolorvalHlink    = 11,
		shemecolorvalLt1      = 12,
		shemecolorvalLt2      = 13,
		shemecolorvalPhClr    = 14,
		shemecolorvalTx1      = 15,
		shemecolorvalTx2      = 16,
	};

	template<EShemeColorVal eDefValue = shemecolorvalAccent1>
	class CShemeColorVal : public CSimpleType<EShemeColorVal, eDefValue>
	{
	public:
		CShemeColorVal() {}

        virtual EShemeColorVal FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;
            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':
                if      ( (L"accent1") == sValue ) this->m_eValue = shemecolorvalAccent1;
                else if ( (L"accent2") == sValue ) this->m_eValue = shemecolorvalAccent2;
                else if ( (L"accent3") == sValue ) this->m_eValue = shemecolorvalAccent3;
                else if ( (L"accent4") == sValue ) this->m_eValue = shemecolorvalAccent4;
                else if ( (L"accent5") == sValue ) this->m_eValue = shemecolorvalAccent5;
                else if ( (L"accent6") == sValue ) this->m_eValue = shemecolorvalAccent6;
				break;
			case 'b':
                if      ( (L"bg1") == sValue ) this->m_eValue = shemecolorvalBg1;
                else if ( (L"bg2") == sValue ) this->m_eValue = shemecolorvalBg2;
				break;
			case 'd':
                if      ( (L"dk1") == sValue ) this->m_eValue = shemecolorvalDk1;
                else if ( (L"dk2") == sValue ) this->m_eValue = shemecolorvalDk2;
				break;
			case 'f':
                if      ( (L"folHlink") == sValue ) this->m_eValue = shemecolorvalFolHlink;
				break;
			case 'h':
                if      ( (L"hlink") == sValue ) this->m_eValue = shemecolorvalHlink;
				break;
			case 'l':
                if      ( (L"lt1") == sValue ) this->m_eValue = shemecolorvalLt1;
                else if ( (L"lt2") == sValue ) this->m_eValue = shemecolorvalLt2;
				break;
			case 'p':
                if      ( (L"phClr") == sValue ) this->m_eValue = shemecolorvalPhClr;
				break;
			case 't':
                if      ( (L"tx1") == sValue ) this->m_eValue = shemecolorvalTx1;
                else if ( (L"tx2") == sValue ) this->m_eValue = shemecolorvalTx2;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(this->m_eValue)
			{
			case shemecolorvalAccent1:  return (L"accent1");			
			case shemecolorvalAccent2:  return (L"accent2");		
			case shemecolorvalAccent3:  return (L"accent3");				
			case shemecolorvalAccent4:  return (L"accent4");			
			case shemecolorvalAccent5:  return (L"accent5");				
			case shemecolorvalAccent6:  return (L"accent6");				
			case shemecolorvalBg1:      return (L"bg1");				
			case shemecolorvalBg2:      return (L"bg2");				
			case shemecolorvalDk1:      return (L"dk1");		
			case shemecolorvalDk2:      return (L"dk2");				
			case shemecolorvalFolHlink: return (L"folHlink");			
			case shemecolorvalHlink:    return (L"hlink");				
			case shemecolorvalLt1:      return (L"lt1");			
			case shemecolorvalLt2:      return (L"lt2");			
			case shemecolorvalPhClr:    return (L"phClr");			
			case shemecolorvalTx1:      return (L"tx1");			
			case shemecolorvalTx2:      return (L"tx2");				
			default :                   return (L"accent1");
			}
		}

		SimpleType_FromString     (EShemeColorVal)
		SimpleType_Operator_Equal (CShemeColorVal)
	};


	//--------------------------------------------------------------------------------
	// ShapeType 20.1.10.56 (Part 1)
	//--------------------------------------------------------------------------------		
	enum EShapeType
	{
		shapetypeAccentBorderCallout1 = 0,
		shapetypeAccentBorderCallout2,
		shapetypeAccentBorderCallout3,
		shapetypeAccentCallout1,
		shapetypeAccentCallout2,
		shapetypeAccentCallout3,
		shapetypeActionButtonBackPrevious,
		shapetypeActionButtonBeginning,
		shapetypeActionButtonBlank,
		shapetypeActionButtonDocument,
		shapetypeActionButtonEnd,
		shapetypeActionButtonForwardNext,
		shapetypeActionButtonHelp,
		shapetypeActionButtonHome,
		shapetypeActionButtonInformation,
		shapetypeActionButtonMovie,
		shapetypeActionButtonReturn,
		shapetypeActionButtonSound,
		shapetypeArc,
		shapetypeBentArrow,
		shapetypeBentConnector2,
		shapetypeBentConnector3,
		shapetypeBentConnector4,
		shapetypeBentConnector5,
		shapetypeBentUpArrow,
		shapetypeBevel,
		shapetypeBlockArc,
		shapetypeBorderCallout1,
		shapetypeBorderCallout2,
		shapetypeBorderCallout3,
		shapetypeBracePair,
		shapetypeBracketPair,
		shapetypeCallout1,
		shapetypeCallout2,
		shapetypeCallout3,
		shapetypeCan,
		shapetypeChartPlus,
		shapetypeChartStar,
		shapetypeChartX,
		shapetypeChevron,
		shapetypeChord,
		shapetypeCircularArrow,
		shapetypeCloud,
		shapetypeCloudCallout,
		shapetypeCorner,
		shapetypeCornerTabs,
		shapetypeCube,
		shapetypeCurvedConnector2,
		shapetypeCurvedConnector3,
		shapetypeCurvedConnector4,
		shapetypeCurvedConnector5,
		shapetypeCurvedDownArrow,
		shapetypeCurvedLeftArrow,
		shapetypeCurvedRightArrow,
		shapetypeCurvedUpArrow,
		shapetypeDecagon,
		shapetypeDiagStripe,
		shapetypeDiamond,
		shapetypeDodecagon,
		shapetypeDonut,
		shapetypeDoubleWave,
		shapetypeDownArrow,
		shapetypeDownArrowCallout,
		shapetypeEllipse,
		shapetypeEllipseRibbon,
		shapetypeEllipseRibbon2,
		shapetypeFlowChartAlternateProcess,
		shapetypeFlowChartCollate,
		shapetypeFlowChartConnector,
		shapetypeFlowChartDecision,
		shapetypeFlowChartDelay,
		shapetypeFlowChartDisplay,
		shapetypeFlowChartDocument,
		shapetypeFlowChartExtract,
		shapetypeFlowChartInputOutput,
		shapetypeFlowChartInternalStorage,
		shapetypeFlowChartMagneticDisk,
		shapetypeFlowChartMagneticDrum,
		shapetypeFlowChartMagneticTape,
		shapetypeFlowChartManualInput,
		shapetypeFlowChartManualOperation,
		shapetypeFlowChartMerge,
		shapetypeFlowChartMultidocument,
		shapetypeFlowChartOfflineStorage,
		shapetypeFlowChartOffpageConnector,
		shapetypeFlowChartOnlineStorage,
		shapetypeFlowChartOr,
		shapetypeFlowChartPredefinedProcess,
		shapetypeFlowChartPreparation,
		shapetypeFlowChartProcess,
		shapetypeFlowChartPunchedCard,
		shapetypeFlowChartPunchedTape,
		shapetypeFlowChartSort,
		shapetypeFlowChartSummingJunction,
		shapetypeFlowChartTerminator,
		shapetypeFoldedCorner,
		shapetypeFrame,
		shapetypeFunnel,
		shapetypeGear6,
		shapetypeGear9,
		shapetypeHalfFrame,
		shapetypeHeart,
		shapetypeHeptagon,
		shapetypeHexagon,
		shapetypeHomePlate,
		shapetypeHorizontalScroll,
		shapetypeIrregularSeal1,
		shapetypeIrregularSeal2,
		shapetypeLeftArrow,
		shapetypeLeftArrowCallout,
		shapetypeLeftBrace,
		shapetypeLeftBracket,
		shapetypeLeftCircularArrow,
		shapetypeLeftRightArrow,
		shapetypeLeftRightArrowCallout,
		shapetypeLeftRightCircularArrow,
		shapetypeLeftRightRibbon,
		shapetypeLeftRightUpArrow,
		shapetypeLeftUpArrow,
		shapetypeLightningBolt,
		shapetypeLine,
		shapetypeLineInv,
		shapetypeMathDivide,
		shapetypeMathEqual,
		shapetypeMathMinus,
		shapetypeMathMultiply,
		shapetypeMathNotEqual,
		shapetypeMathPlus,
		shapetypeMoon,
		shapetypeNonIsoscelesTrapezoid,
		shapetypeNoSmoking,
		shapetypeNotchedRightArrow,
		shapetypeOctagon,
		shapetypeParallelogram,
		shapetypePentagon,
		shapetypePie,
		shapetypePieWedge,
		shapetypePlaque,
		shapetypePlaqueTabs,
		shapetypePlus,
		shapetypeQuadArrow,
		shapetypeQuadArrowCallout,
		shapetypeRect,
		shapetypeRibbon,
		shapetypeRibbon2,
		shapetypeRightArrow,
		shapetypeRightArrowCallout,
		shapetypeRightBrace,
		shapetypeRightBracket,
		shapetypeRound1Rect,
		shapetypeRound2DiagRect,
		shapetypeRound2SameRect,
		shapetypeRoundRect,
		shapetypeRtTriangle,
		shapetypeSmileyFace,
		shapetypeSnip1Rect,
		shapetypeSnip2DiagRect,
		shapetypeSnip2SameRect,
		shapetypeSnipRoundRect,
		shapetypeSquareTabs,
		shapetypeStar10,
		shapetypeStar12,
		shapetypeStar16,
		shapetypeStar24,
		shapetypeStar32,
		shapetypeStar4,
		shapetypeStar5,
		shapetypeStar6,
		shapetypeStar7,
		shapetypeStar8,
		shapetypeStraightConnector1,
		shapetypeStripedRightArrow,
		shapetypeSun,
		shapetypeSwooshArrow,
		shapetypeTeardrop,
		shapetypeTrapezoid,
		shapetypeTriangle,
		shapetypeUpArrow,
		shapetypeUpArrowCallout,
		shapetypeUpDownArrow,
		shapetypeUpDownArrowCallout,
		shapetypeUturnArrow,
		shapetypeVerticalScroll,
		shapetypeWave,
		shapetypeWedgeEllipseCallout,
		shapetypeWedgeRectCallout,
		shapetypeWedgeRoundRectCallout,
////new//////////////
		 shapetypeBallon,
		 shapetypeRightUpArrow,
 //дубирует с WordArd - может задаваться как внутри текстого бокса, так и в виде объекта
		 shapetypeTextArchDownPour,
		 shapetypeTextArchUpPour,
		 shapetypeTextCanDown,
		 shapetypeTextCanUp,
		 shapetypeTextCirclePour,
		 shapetypeTextCurveDown,
		 shapetypeTextCurveUp,
		 shapetypeTextDeflate,
		 shapetypeTextDeflateBottom,
		 shapetypeTextDeflateInflate,
		 shapetypeTextDeflateInflateDeflat,
		 shapetypeTextDeflateTop,
		 shapetypeTextDoubleWave1,
		 shapetypeTextFadeDown,
		 shapetypeTextFadeLeft,
		 shapetypeTextFadeRight,
		 shapetypeTextFadeUp,
		 shapetypeTextInflateBottom,
		 shapetypeTextInflateTop,
		 shapetypeTextRingInside,
		 shapetypeTextRingOutside,
		 shapetypeTextWave1,
		 shapetypeTextWave2,
		 shapetypeTextWave4,
		 shapetypeThickArrow

	};

	template<EShapeType eDefValue = shapetypeRect>
	class CShapeType : public CSimpleType<EShapeType, eDefValue>
	{
	public:
		CShapeType() {}

        virtual EShapeType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':

                if      ( (L"accentBorderCallout1")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout1;
                else if ( (L"accentBorderCallout2")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout2;
                else if ( (L"accentBorderCallout3")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout3;
                else if ( (L"accentCallout1")			== sValue ) this->m_eValue = shapetypeAccentCallout1;
                else if ( (L"accentCallout2")			== sValue ) this->m_eValue = shapetypeAccentCallout2;
                else if ( (L"accentCallout3")			== sValue ) this->m_eValue = shapetypeAccentCallout3;
                else if ( (L"actionButtonBackPrevious")== sValue ) this->m_eValue = shapetypeActionButtonBackPrevious;
                else if ( (L"actionButtonBeginning")	== sValue ) this->m_eValue = shapetypeActionButtonBeginning;
                else if ( (L"actionButtonBlank")		== sValue ) this->m_eValue = shapetypeActionButtonBlank;
                else if ( (L"actionButtonDocument")	== sValue ) this->m_eValue = shapetypeActionButtonDocument;
                else if ( (L"actionButtonEnd")			== sValue ) this->m_eValue = shapetypeActionButtonEnd;
                else if ( (L"actionButtonForwardNext")	== sValue ) this->m_eValue = shapetypeActionButtonForwardNext;
                else if ( (L"actionButtonHelp")		== sValue ) this->m_eValue = shapetypeActionButtonHelp;
                else if ( (L"actionButtonHome")		== sValue ) this->m_eValue = shapetypeActionButtonHome;
                else if ( (L"actionButtonInformation")	== sValue ) this->m_eValue = shapetypeActionButtonInformation;
                else if ( (L"actionButtonMovie")		== sValue ) this->m_eValue = shapetypeActionButtonMovie;
                else if ( (L"actionButtonReturn")		== sValue ) this->m_eValue = shapetypeActionButtonReturn;
                else if ( (L"actionButtonSound")		== sValue ) this->m_eValue = shapetypeActionButtonSound;
                else if ( (L"arc")						== sValue ) this->m_eValue = shapetypeArc;
				break;

			case 'b':
                if      ( (L"bentArrow")				== sValue ) this->m_eValue = shapetypeBentArrow;
                else if ( (L"bentConnector2")			== sValue ) this->m_eValue = shapetypeBentConnector2;
                else if ( (L"bentConnector3")			== sValue ) this->m_eValue = shapetypeBentConnector3;
                else if ( (L"bentConnector4")			== sValue ) this->m_eValue = shapetypeBentConnector4;
                else if ( (L"bentConnector5")			== sValue ) this->m_eValue = shapetypeBentConnector5;
                else if ( (L"bentUpArrow")				== sValue ) this->m_eValue = shapetypeBentUpArrow;
                else if ( (L"bevel")					== sValue ) this->m_eValue = shapetypeBevel;
                else if ( (L"blockArc")				== sValue ) this->m_eValue = shapetypeBlockArc;
                else if ( (L"borderCallout1")			== sValue ) this->m_eValue = shapetypeBorderCallout1;
                else if ( (L"borderCallout2")			== sValue ) this->m_eValue = shapetypeBorderCallout2;
                else if ( (L"borderCallout3")			== sValue ) this->m_eValue = shapetypeBorderCallout3;
                else if ( (L"bracePair")				== sValue ) this->m_eValue = shapetypeBracePair;
                else if ( (L"bracketPair")				== sValue ) this->m_eValue = shapetypeBracketPair;
		/////new
                else if ( (L"Balloon") 				== sValue ) this->m_eValue = shapetypeBallon;
				break;

			case 'c':
                if      ( (L"callout1")				== sValue ) this->m_eValue = shapetypeCallout1;
                else if ( (L"callout2")				== sValue ) this->m_eValue = shapetypeCallout2;
                else if ( (L"callout3")				== sValue ) this->m_eValue = shapetypeCallout3;
                else if ( (L"can")						== sValue ) this->m_eValue = shapetypeCan;
                else if ( (L"chartPlus")				== sValue ) this->m_eValue = shapetypeChartPlus;
                else if ( (L"chartStar")				== sValue ) this->m_eValue = shapetypeChartStar;
                else if ( (L"chartX")					== sValue ) this->m_eValue = shapetypeChartX;
                else if ( (L"chevron")					== sValue ) this->m_eValue = shapetypeChevron;
                else if ( (L"chord")					== sValue ) this->m_eValue = shapetypeChord;
                else if ( (L"circularArrow")			== sValue ) this->m_eValue = shapetypeCircularArrow;
                else if ( (L"cloud")					== sValue ) this->m_eValue = shapetypeCloud;
                else if ( (L"cloudCallout")			== sValue ) this->m_eValue = shapetypeCloudCallout;
                else if ( (L"corner")					== sValue ) this->m_eValue = shapetypeCorner;
                else if ( (L"cornerTabs")				== sValue ) this->m_eValue = shapetypeCornerTabs;
                else if ( (L"cube")					== sValue ) this->m_eValue = shapetypeCube;
                else if ( (L"curvedConnector2")		== sValue ) this->m_eValue = shapetypeCurvedConnector2;
                else if ( (L"curvedConnector3")		== sValue ) this->m_eValue = shapetypeCurvedConnector3;
                else if ( (L"curvedConnector4")		== sValue ) this->m_eValue = shapetypeCurvedConnector4;
                else if ( (L"curvedConnector5")		== sValue ) this->m_eValue = shapetypeCurvedConnector5;
                else if ( (L"curvedDownArrow")			== sValue ) this->m_eValue = shapetypeCurvedDownArrow;
                else if ( (L"curvedLeftArrow")			== sValue ) this->m_eValue = shapetypeCurvedLeftArrow;
                else if ( (L"curvedRightArrow")		== sValue ) this->m_eValue = shapetypeCurvedRightArrow;
                else if ( (L"curvedUpArrow")			== sValue ) this->m_eValue = shapetypeCurvedUpArrow;
				break;

			case 'd':
                if      ( (L"decagon")					== sValue ) this->m_eValue = shapetypeDecagon;
                else if ( (L"diagStripe")				== sValue ) this->m_eValue = shapetypeDiagStripe;
                else if ( (L"diamond")					== sValue ) this->m_eValue = shapetypeDiamond;
                else if ( (L"dodecagon")				== sValue ) this->m_eValue = shapetypeDodecagon;
                else if ( (L"donut")					== sValue ) this->m_eValue = shapetypeDonut;
                else if ( (L"doubleWave")				== sValue ) this->m_eValue = shapetypeDoubleWave;
                else if ( (L"downArrow")				== sValue ) this->m_eValue = shapetypeDownArrow;
                else if ( (L"downArrowCallout")		== sValue ) this->m_eValue = shapetypeDownArrowCallout;
				break;

			case 'e':
                if      ( (L"ellipse")					== sValue ) this->m_eValue = shapetypeEllipse;
                else if ( (L"ellipseRibbon")			== sValue ) this->m_eValue = shapetypeEllipseRibbon;
                else if ( (L"ellipseRibbon2")			== sValue ) this->m_eValue = shapetypeEllipseRibbon2;
				break;

			case 'f':
                if      ( (L"flowChartAlternateProcess")	== sValue ) this->m_eValue = shapetypeFlowChartAlternateProcess;
                else if ( (L"flowChartCollate")			== sValue ) this->m_eValue = shapetypeFlowChartCollate;
                else if ( (L"flowChartConnector")			== sValue ) this->m_eValue = shapetypeFlowChartConnector;
                else if ( (L"flowChartDecision")			== sValue ) this->m_eValue = shapetypeFlowChartDecision;
                else if ( (L"flowChartDelay")				== sValue ) this->m_eValue = shapetypeFlowChartDelay;
                else if ( (L"flowChartDisplay")			== sValue ) this->m_eValue = shapetypeFlowChartDisplay;
                else if ( (L"flowChartDocument")			== sValue ) this->m_eValue = shapetypeFlowChartDocument;
                else if ( (L"flowChartExtract")			== sValue ) this->m_eValue = shapetypeFlowChartExtract;
                else if ( (L"flowChartInputOutput")		== sValue ) this->m_eValue = shapetypeFlowChartInputOutput;
                else if ( (L"flowChartInternalStorage")	== sValue ) this->m_eValue = shapetypeFlowChartInternalStorage;
                else if ( (L"flowChartMagneticDisk")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticDisk;
                else if ( (L"flowChartMagneticDrum")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticDrum;
                else if ( (L"flowChartMagneticTape")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticTape;
                else if ( (L"flowChartManualInput")		== sValue ) this->m_eValue = shapetypeFlowChartManualInput;
                else if ( (L"flowChartManualOperation")	== sValue ) this->m_eValue = shapetypeFlowChartManualOperation;
                else if ( (L"flowChartMerge")				== sValue ) this->m_eValue = shapetypeFlowChartMerge;
                else if ( (L"flowChartMultidocument")		== sValue ) this->m_eValue = shapetypeFlowChartMultidocument;
                else if ( (L"flowChartOfflineStorage")		== sValue ) this->m_eValue = shapetypeFlowChartOfflineStorage;
                else if ( (L"flowChartOffpageConnector")	== sValue ) this->m_eValue = shapetypeFlowChartOffpageConnector;
                else if ( (L"flowChartOnlineStorage")		== sValue ) this->m_eValue = shapetypeFlowChartOnlineStorage;
                else if ( (L"flowChartOr")					== sValue ) this->m_eValue = shapetypeFlowChartOr;
                else if ( (L"flowChartPredefinedProcess")	== sValue ) this->m_eValue = shapetypeFlowChartPredefinedProcess;
                else if ( (L"flowChartPreparation")		== sValue ) this->m_eValue = shapetypeFlowChartPreparation;
                else if ( (L"flowChartProcess")			== sValue ) this->m_eValue = shapetypeFlowChartProcess;
                else if ( (L"flowChartPunchedCard")		== sValue ) this->m_eValue = shapetypeFlowChartPunchedCard;
                else if ( (L"flowChartPunchedTape")		== sValue ) this->m_eValue = shapetypeFlowChartPunchedTape;
                else if ( (L"flowChartSort")				== sValue ) this->m_eValue = shapetypeFlowChartSort;
                else if ( (L"flowChartSummingJunction")	== sValue ) this->m_eValue = shapetypeFlowChartSummingJunction;
                else if ( (L"flowChartTerminator")			== sValue ) this->m_eValue = shapetypeFlowChartTerminator;
                else if ( (L"foldedCorner")				== sValue ) this->m_eValue = shapetypeFoldedCorner;
                else if ( (L"frame")						== sValue ) this->m_eValue = shapetypeFrame;
                else if ( (L"funnel")						== sValue ) this->m_eValue = shapetypeFunnel;
				break;

			case 'g':
                if      ( (L"gear6")					== sValue ) this->m_eValue = shapetypeGear6;
                else if ( (L"gear9")					== sValue ) this->m_eValue = shapetypeGear9;
				break;

			case 'h':
                if      ( (L"halfFrame")				== sValue ) this->m_eValue = shapetypeHalfFrame;
                else if ( (L"heart")					== sValue ) this->m_eValue = shapetypeHeart;
                else if ( (L"heptagon")				== sValue ) this->m_eValue = shapetypeHeptagon;
                else if ( (L"hexagon")					== sValue ) this->m_eValue = shapetypeHexagon;
                else if ( (L"homePlate")				== sValue ) this->m_eValue = shapetypeHomePlate;
                else if ( (L"horizontalScroll")		== sValue ) this->m_eValue = shapetypeHorizontalScroll;
				break;

			case 'i':
                if      ( (L"irregularSeal1")			== sValue ) this->m_eValue = shapetypeIrregularSeal1;
                else if ( (L"irregularSeal2")			== sValue ) this->m_eValue = shapetypeIrregularSeal2;
				break;

			case 'l':
                if      ( (L"leftArrow")				== sValue ) this->m_eValue = shapetypeLeftArrow;
                else if ( (L"leftArrowCallout")		== sValue ) this->m_eValue = shapetypeLeftArrowCallout;
                else if ( (L"leftBrace")				== sValue ) this->m_eValue = shapetypeLeftBrace;
                else if ( (L"leftBracket")				== sValue ) this->m_eValue = shapetypeLeftBracket;
                else if ( (L"leftCircularArrow")		== sValue ) this->m_eValue = shapetypeLeftCircularArrow;
                else if ( (L"leftRightArrow")			== sValue ) this->m_eValue = shapetypeLeftRightArrow;
                else if ( (L"leftRightArrowCallout")	== sValue ) this->m_eValue = shapetypeLeftRightArrowCallout;
                else if ( (L"leftRightCircularArrow")	== sValue ) this->m_eValue = shapetypeLeftRightCircularArrow;
                else if ( (L"leftRightRibbon")			== sValue ) this->m_eValue = shapetypeLeftRightRibbon;
                else if ( (L"leftRightUpArrow")		== sValue ) this->m_eValue = shapetypeLeftRightUpArrow;
                else if ( (L"leftUpArrow")				== sValue ) this->m_eValue = shapetypeLeftUpArrow;
                else if ( (L"lightningBolt")			== sValue ) this->m_eValue = shapetypeLightningBolt;
                else if ( (L"line")					== sValue ) this->m_eValue = shapetypeLine;
                else if ( (L"lineInv")					== sValue ) this->m_eValue = shapetypeLineInv;
				break;

			case 'm':
                if      ( (L"mathDivide")				== sValue ) this->m_eValue = shapetypeMathDivide;
                else if ( (L"mathEqual")				== sValue ) this->m_eValue = shapetypeMathEqual;
                else if ( (L"mathMinus")				== sValue ) this->m_eValue = shapetypeMathMinus;
                else if ( (L"mathMultiply")			== sValue ) this->m_eValue = shapetypeMathMultiply;
                else if ( (L"mathNotEqual")			== sValue ) this->m_eValue = shapetypeMathNotEqual;
                else if ( (L"mathPlus")				== sValue ) this->m_eValue = shapetypeMathPlus;
                else if ( (L"moon")					== sValue ) this->m_eValue = shapetypeMoon;
				break;

			case 'n':
                if      ( (L"nonIsoscelesTrapezoid")	== sValue ) this->m_eValue = shapetypeNonIsoscelesTrapezoid;
                else if ( (L"noSmoking")				== sValue ) this->m_eValue = shapetypeNoSmoking;
                else if ( (L"notchedRightArrow")		== sValue ) this->m_eValue = shapetypeNotchedRightArrow;
				break;

			case 'o':
                if      ( (L"octagon")					== sValue ) this->m_eValue = shapetypeOctagon;
				break;

			case 'p':
                if      ( (L"parallelogram")			== sValue ) this->m_eValue = shapetypeParallelogram;
                else if ( (L"pentagon")					== sValue ) this->m_eValue = shapetypePentagon;
                else if ( (L"pie")						== sValue ) this->m_eValue = shapetypePie;
                else if ( (L"pieWedge")					== sValue ) this->m_eValue = shapetypePieWedge;
                else if ( (L"plaque")					== sValue ) this->m_eValue = shapetypePlaque;
                else if ( (L"plaqueTabs")				== sValue ) this->m_eValue = shapetypePlaqueTabs;
                else if ( (L"plus")						== sValue ) this->m_eValue = shapetypePlus;
				break;

			case 'q':
                if      ( (L"quadArrow")				== sValue ) this->m_eValue = shapetypeQuadArrow;
                else if ( (L"quadArrowCallout")		== sValue ) this->m_eValue = shapetypeQuadArrowCallout;
				break;

			case 'r':
                if      ( (L"rect")					== sValue ) this->m_eValue = shapetypeRect;
                else if ( (L"ribbon")					== sValue ) this->m_eValue = shapetypeRibbon;
                else if ( (L"ribbon2")					== sValue ) this->m_eValue = shapetypeRibbon2;
                else if ( (L"rightArrow")				== sValue ) this->m_eValue = shapetypeRightArrow;
                else if ( (L"rightArrowCallout")		== sValue ) this->m_eValue = shapetypeRightArrowCallout;
                else if ( (L"rightBrace")				== sValue ) this->m_eValue = shapetypeRightBrace;
                else if ( (L"rightBracket")			== sValue ) this->m_eValue = shapetypeRightBracket;
                else if ( (L"round1Rect")				== sValue ) this->m_eValue = shapetypeRound1Rect;
                else if ( (L"round2DiagRect")			== sValue ) this->m_eValue = shapetypeRound2DiagRect;
                else if ( (L"round2SameRect")			== sValue ) this->m_eValue = shapetypeRound2SameRect;
                else if ( (L"roundRect")				== sValue ) this->m_eValue = shapetypeRoundRect;
                else if ( (L"rtTriangle")				== sValue ) this->m_eValue = shapetypeRtTriangle;
		////new
                else if ( (L"rightUpArrow")			== sValue ) this->m_eValue = 	 shapetypeRightUpArrow;
				break;

			case 's':
                if      ( (L"smileyFace")				== sValue ) this->m_eValue = shapetypeSmileyFace;
                else if ( (L"snip1Rect")				== sValue ) this->m_eValue = shapetypeSnip1Rect;
                else if ( (L"snip2DiagRect")			== sValue ) this->m_eValue = shapetypeSnip2DiagRect;
                else if ( (L"snip2SameRect")			== sValue ) this->m_eValue = shapetypeSnip2SameRect;
                else if ( (L"snipRoundRect")			== sValue ) this->m_eValue = shapetypeSnipRoundRect;
                else if ( (L"squareTabs")				== sValue ) this->m_eValue = shapetypeSquareTabs;
                else if ( (L"star10")					== sValue ) this->m_eValue = shapetypeStar10;
                else if ( (L"star12")					== sValue ) this->m_eValue = shapetypeStar12;
                else if ( (L"star16")					== sValue ) this->m_eValue = shapetypeStar16;
                else if ( (L"star24")					== sValue ) this->m_eValue = shapetypeStar24;
                else if ( (L"star32")					== sValue ) this->m_eValue = shapetypeStar32;
                else if ( (L"star4")					== sValue ) this->m_eValue = shapetypeStar4;
                else if ( (L"star5")					== sValue ) this->m_eValue = shapetypeStar5;
                else if ( (L"star6")					== sValue ) this->m_eValue = shapetypeStar6;
                else if ( (L"star7")					== sValue ) this->m_eValue = shapetypeStar7;
                else if ( (L"star8")					== sValue ) this->m_eValue = shapetypeStar8;
                else if ( (L"straightConnector1")		== sValue ) this->m_eValue = shapetypeStraightConnector1;
                else if ( (L"stripedRightArrow")		== sValue ) this->m_eValue = shapetypeStripedRightArrow;
                else if ( (L"sun")						== sValue ) this->m_eValue = shapetypeSun;
                else if ( (L"swooshArrow")				== sValue ) this->m_eValue = shapetypeSwooshArrow;
				break;

			case 't':
                if      ( (L"teardrop")				== sValue ) this->m_eValue = shapetypeTeardrop;
                else if ( (L"trapezoid")				== sValue ) this->m_eValue = shapetypeTrapezoid;
                else if ( (L"triangle")				== sValue ) this->m_eValue = shapetypeTriangle;
		/////new
                else if ( (L"textArchDownPour")		== sValue ) this->m_eValue = 	 shapetypeTextArchDownPour;
                else if ( (L"textArchUpPour")			== sValue ) this->m_eValue = 	 shapetypeTextArchUpPour;
                else if ( (L"textCanDown")				== sValue ) this->m_eValue = 	 shapetypeTextCanDown;
                else if ( (L"textCanUp")				== sValue ) this->m_eValue = 	 shapetypeTextCanUp;
                else if ( (L"textCirclePour")			== sValue ) this->m_eValue = 	 shapetypeTextCirclePour;
                else if ( (L"textCurveDown")			== sValue ) this->m_eValue = 	 shapetypeTextCurveDown;
                else if ( (L"textCurveUp")				== sValue ) this->m_eValue = 	 shapetypeTextCurveUp;
                else if ( (L"textDeflate")				== sValue ) this->m_eValue = 	 shapetypeTextDeflate;
                else if ( (L"textDeflateBottom")		== sValue ) this->m_eValue = 	 shapetypeTextDeflateBottom;
                else if ( (L"textDeflateInflate")		== sValue ) this->m_eValue = 	 shapetypeTextDeflateInflate;
                else if ( (L"textDeflateInflateDeflate")	== sValue ) this->m_eValue = 	 shapetypeTextDeflateInflateDeflat;
                else if ( (L"textDeflateTop")			== sValue ) this->m_eValue = 	 shapetypeTextDeflateTop;
                else if ( (L"textDoubleWave1")			== sValue ) this->m_eValue = 	 shapetypeTextDoubleWave1;
                else if ( (L"textFadeDown")			== sValue ) this->m_eValue = 	 shapetypeTextFadeDown;
                else if ( (L"textFadeLeft")			== sValue ) this->m_eValue = 	 shapetypeTextFadeLeft;
                else if ( (L"textFadeRight")			== sValue ) this->m_eValue = 	 shapetypeTextFadeRight;
                else if ( (L"textFadeUp")				== sValue ) this->m_eValue = 	 shapetypeTextFadeUp;
                else if ( (L"textInflateBottom")		== sValue ) this->m_eValue = 	 shapetypeTextInflateBottom;
                else if ( (L"textInflateTop")			== sValue ) this->m_eValue = 	 shapetypeTextInflateTop;
                else if ( (L"textRingInside")			== sValue ) this->m_eValue = 	 shapetypeTextRingInside;
                else if ( (L"textRingOutside")			== sValue ) this->m_eValue = 	 shapetypeTextRingOutside;
                else if ( (L"textWave1")				== sValue ) this->m_eValue = 	 shapetypeTextWave1;
                else if ( (L"textWave2")				== sValue ) this->m_eValue = 	 shapetypeTextWave2;
                else if ( (L"textWave4")				== sValue ) this->m_eValue = 	 shapetypeTextWave4;
                else if ( (L"thickArrow")				== sValue ) this->m_eValue = 	 shapetypeThickArrow;
				break;
			case 'u':
                if      ( (L"upArrow")					== sValue ) this->m_eValue = shapetypeUpArrow;
                else if ( (L"upArrowCallout")			== sValue ) this->m_eValue = shapetypeUpArrowCallout;
                else if ( (L"upDownArrow")				== sValue ) this->m_eValue = shapetypeUpDownArrow;
                else if ( (L"upDownArrowCallout")		== sValue ) this->m_eValue = shapetypeUpDownArrowCallout;
                else if ( (L"uturnArrow")				== sValue ) this->m_eValue = shapetypeUturnArrow;
				break;

			case 'v':
                if      ( (L"verticalScroll")			== sValue ) this->m_eValue = shapetypeVerticalScroll;
				break;

			case 'w':
                if      ( (L"wave")					== sValue ) this->m_eValue = shapetypeWave;
                else if ( (L"wedgeEllipseCallout")		== sValue ) this->m_eValue = shapetypeWedgeEllipseCallout;
                else if ( (L"wedgeRectCallout")		== sValue ) this->m_eValue = shapetypeWedgeRectCallout;
                else if ( (L"wedgeRoundRectCallout")	== sValue ) this->m_eValue = shapetypeWedgeRoundRectCallout;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring    ToString  () const
		{
            switch(this->m_eValue)
			{
			case shapetypeAccentBorderCallout1:			return (L"accentBorderCallout1");
			case shapetypeAccentBorderCallout2:			return (L"accentBorderCallout2");
			case shapetypeAccentBorderCallout3:			return (L"accentBorderCallout3");
			case shapetypeAccentCallout1:				return (L"accentCallout1");
			case shapetypeAccentCallout2:				return (L"accentCallout2");
			case shapetypeAccentCallout3:				return (L"accentCallout3");
			case shapetypeActionButtonBackPrevious:		return (L"actionButtonBackPrevious");
			case shapetypeActionButtonBeginning:		return (L"actionButtonBeginning");
			case shapetypeActionButtonBlank:			return (L"actionButtonBlank");
			case shapetypeActionButtonDocument:			return (L"actionButtonDocument");
			case shapetypeActionButtonEnd:				return (L"actionButtonEnd");
			case shapetypeActionButtonForwardNext:		return (L"actionButtonForwardNext");
			case shapetypeActionButtonHelp:				return (L"actionButtonHelp");
			case shapetypeActionButtonHome:				return (L"actionButtonHome");
			case shapetypeActionButtonInformation:		return (L"actionButtonInformation");
			case shapetypeActionButtonMovie:			return (L"actionButtonMovie");
			case shapetypeActionButtonReturn:			return (L"actionButtonReturn");
			case shapetypeActionButtonSound:			return (L"actionButtonSound");
			case shapetypeArc:							return (L"arc");
			case shapetypeBentArrow:					return (L"bentArrow");
			case shapetypeBentConnector2:				return (L"bentConnector2");
			case shapetypeBentConnector3:				return (L"bentConnector3");
			case shapetypeBentConnector4:				return (L"bentConnector4");
			case shapetypeBentConnector5:				return (L"bentConnector5");
			case shapetypeBentUpArrow:					return (L"bentUpArrow");
			case shapetypeBevel:						return (L"bevel");
			case shapetypeBlockArc:						return (L"blockArc");
			case shapetypeBorderCallout1:				return (L"borderCallout1");
			case shapetypeBorderCallout2:				return (L"borderCallout2");
			case shapetypeBorderCallout3:				return (L"borderCallout3");
			case shapetypeBracePair:					return (L"bracePair");
			case shapetypeBracketPair:					return (L"bracketPair");
			case shapetypeCallout1:						return (L"callout1");
			case shapetypeCallout2:						return (L"callout2");
			case shapetypeCallout3:						return (L"callout3");
			case shapetypeCan:							return (L"can");
			case shapetypeChartPlus:					return (L"chartPlus");
			case shapetypeChartStar:					return (L"chartStar");
			case shapetypeChartX:						return (L"chartX");
			case shapetypeChevron:						return (L"chevron");
			case shapetypeChord:						return (L"chord");
			case shapetypeCircularArrow:				return (L"circularArrow");
			case shapetypeCloud:						return (L"cloud");
			case shapetypeCloudCallout:					return (L"cloudCallout");
			case shapetypeCorner:						return (L"corner");
			case shapetypeCornerTabs:					return (L"cornerTabs");
			case shapetypeCube:							return (L"cube");
			case shapetypeCurvedConnector2:				return (L"curvedConnector2");
			case shapetypeCurvedConnector3: 			return (L"curvedConnector3");
			case shapetypeCurvedConnector4: 			return (L"curvedConnector4");
			case shapetypeCurvedConnector5: 			return (L"curvedConnector5");
			case shapetypeCurvedDownArrow:				return (L"curvedDownArrow");
			case shapetypeCurvedLeftArrow:				return (L"curvedLeftArrow");
			case shapetypeCurvedRightArrow:				return (L"curvedRightArrow");
			case shapetypeCurvedUpArrow:				return (L"curvedUpArrow");
			case shapetypeDecagon:						return (L"decagon");
			case shapetypeDiagStripe:					return (L"diagStripe");
			case shapetypeDiamond:						return (L"diamond");
			case shapetypeDodecagon:					return (L"dodecagon");
			case shapetypeDonut:						return (L"donut");
			case shapetypeDoubleWave:					return (L"doubleWave");
			case shapetypeDownArrow:					return (L"downArrow");
			case shapetypeDownArrowCallout:				return (L"downArrowCallout");
			case shapetypeEllipse:						return (L"ellipse");
			case shapetypeEllipseRibbon:				return (L"ellipseRibbon");
			case shapetypeEllipseRibbon2:				return (L"ellipseRibbon2");
			case shapetypeFlowChartAlternateProcess:	return (L"flowChartAlternateProcess");
			case shapetypeFlowChartCollate:				return (L"flowChartCollate");
			case shapetypeFlowChartConnector:			return (L"flowChartConnector");
			case shapetypeFlowChartDecision:			return (L"flowChartDecision");
			case shapetypeFlowChartDelay:				return (L"flowChartDelay");
			case shapetypeFlowChartDisplay:				return (L"flowChartDisplay");
			case shapetypeFlowChartDocument:			return (L"flowChartDocument");
			case shapetypeFlowChartExtract:				return (L"flowChartExtract");
			case shapetypeFlowChartInputOutput:			return (L"flowChartInputOutput");
			case shapetypeFlowChartInternalStorage:		return (L"flowChartInternalStorage");
			case shapetypeFlowChartMagneticDisk:		return (L"flowChartMagneticDisk");
			case shapetypeFlowChartMagneticDrum:		return (L"flowChartMagneticDrum");
			case shapetypeFlowChartMagneticTape:		return (L"flowChartMagneticTape");
			case shapetypeFlowChartManualInput:			return (L"flowChartManualInput");
			case shapetypeFlowChartManualOperation:		return (L"flowChartManualOperation");
			case shapetypeFlowChartMerge:				return (L"flowChartMerge");
			case shapetypeFlowChartMultidocument:		return (L"flowChartMultidocument");
			case shapetypeFlowChartOfflineStorage:		return (L"flowChartOfflineStorage");
			case shapetypeFlowChartOffpageConnector:	return (L"flowChartOffpageConnector");
			case shapetypeFlowChartOnlineStorage:		return (L"flowChartOnlineStorage");
			case shapetypeFlowChartOr:					return (L"flowChartOr");
			case shapetypeFlowChartPredefinedProcess:	return (L"flowChartPredefinedProcess");
			case shapetypeFlowChartPreparation:			return (L"flowChartPreparation");
			case shapetypeFlowChartProcess:				return (L"flowChartProcess");
			case shapetypeFlowChartPunchedCard:			return (L"flowChartPunchedCard");
			case shapetypeFlowChartPunchedTape:			return (L"flowChartPunchedTape");
			case shapetypeFlowChartSort:				return (L"flowChartSort");
			case shapetypeFlowChartSummingJunction:		return (L"flowChartSummingJunction");
			case shapetypeFlowChartTerminator:			return (L"flowChartTerminator");
			case shapetypeFoldedCorner:					return (L"foldedCorner");
			case shapetypeFrame:						return (L"frame");
			case shapetypeFunnel:						return (L"funnel");
			case shapetypeGear6:						return (L"gear6");
			case shapetypeGear9:						return (L"gear9");
			case shapetypeHalfFrame:					return (L"halfFrame");
			case shapetypeHeart:						return (L"heart");
			case shapetypeHeptagon:						return (L"heptagon");
			case shapetypeHexagon:						return (L"hexagon");
			case shapetypeHomePlate:					return (L"homePlate");
			case shapetypeHorizontalScroll:				return (L"horizontalScroll");
			case shapetypeIrregularSeal1:				return (L"irregularSeal1");
			case shapetypeIrregularSeal2:				return (L"irregularSeal2");
			case shapetypeLeftArrow:					return (L"leftArrow");
			case shapetypeLeftArrowCallout:				return (L"leftArrowCallout");
			case shapetypeLeftBrace:					return (L"leftBrace");
			case shapetypeLeftBracket:					return (L"leftBracket");
			case shapetypeLeftCircularArrow:			return (L"leftCircularArrow");
			case shapetypeLeftRightArrow:				return (L"leftRightArrow");
			case shapetypeLeftRightArrowCallout:		return (L"leftRightArrowCallout");
			case shapetypeLeftRightCircularArrow:		return (L"leftRightCircularArrow");
			case shapetypeLeftRightRibbon:				return (L"leftRightRibbon");
			case shapetypeLeftRightUpArrow:				return (L"leftRightUpArrow");
			case shapetypeLeftUpArrow:					return (L"leftUpArrow");
			case shapetypeLightningBolt:				return (L"lightningBolt");
			case shapetypeLine:							return (L"line");
			case shapetypeLineInv:						return (L"lineInv");
			case shapetypeMathDivide:					return (L"mathDivide");
			case shapetypeMathEqual:					return (L"mathEqual");
			case shapetypeMathMinus:					return (L"mathMinus");
			case shapetypeMathMultiply:					return (L"mathMultiply");
			case shapetypeMathNotEqual:					return (L"mathNotEqual");
			case shapetypeMathPlus:						return (L"mathPlus");
			case shapetypeMoon:							return (L"moon");
			case shapetypeNonIsoscelesTrapezoid:		return (L"nonIsoscelesTrapezoid");
			case shapetypeNoSmoking:					return (L"noSmoking");
			case shapetypeNotchedRightArrow:			return (L"notchedRightArrow");
			case shapetypeOctagon:						return (L"octagon");
			case shapetypeParallelogram:				return (L"parallelogram");
			case shapetypePentagon:						return (L"pentagon");
			case shapetypePie:							return (L"pie");
			case shapetypePieWedge:						return (L"pieWedge");
			case shapetypePlaque:						return (L"plaque");
			case shapetypePlaqueTabs:					return (L"plaqueTabs");
			case shapetypePlus:							return (L"plus");
			case shapetypeQuadArrow:					return (L"quadArrow");
			case shapetypeQuadArrowCallout:				return (L"quadArrowCallout");
			case shapetypeRect:							return (L"rect");
			case shapetypeRibbon:						return (L"ribbon");
			case shapetypeRibbon2:						return (L"ribbon2");
			case shapetypeRightArrow:					return (L"rightArrow");
			case shapetypeRightArrowCallout:			return (L"rightArrowCallout");
			case shapetypeRightBrace:					return (L"rightBrace");
			case shapetypeRightBracket:					return (L"rightBracket");
			case shapetypeRound1Rect:					return (L"round1Rect");
			case shapetypeRound2DiagRect:				return (L"round2DiagRect");
			case shapetypeRound2SameRect:				return (L"round2SameRect");
			case shapetypeRoundRect:					return (L"roundRect");
			case shapetypeRtTriangle:					return (L"rtTriangle");
			case shapetypeSmileyFace:					return (L"smileyFace");
			case shapetypeSnip1Rect:					return (L"snip1Rect");
			case shapetypeSnip2DiagRect:				return (L"snip2DiagRect");
			case shapetypeSnip2SameRect:				return (L"snip2SameRect");
			case shapetypeSnipRoundRect:				return (L"snipRoundRect");
			case shapetypeSquareTabs:					return (L"squareTabs");
			case shapetypeStar10:						return (L"star10");
			case shapetypeStar12:						return (L"star12");
			case shapetypeStar16:						return (L"star16");
			case shapetypeStar24:						return (L"star24");
			case shapetypeStar32:						return (L"star32");
			case shapetypeStar4:						return (L"star4");
			case shapetypeStar5:						return (L"star5");
			case shapetypeStar6:						return (L"star6");
			case shapetypeStar7:						return (L"star7");
			case shapetypeStar8:						return (L"star8");
			case shapetypeStraightConnector1:			return (L"straightConnector1");
			case shapetypeStripedRightArrow:			return (L"stripedRightArrow");
			case shapetypeSun:							return (L"sun");
			case shapetypeSwooshArrow:					return (L"swooshArrow");
			case shapetypeTeardrop:						return (L"teardrop");
			case shapetypeTrapezoid:					return (L"trapezoid");
			case shapetypeTriangle:						return (L"triangle");
			case shapetypeUpArrow:						return (L"upArrow");
			case shapetypeUpArrowCallout:				return (L"upArrowCallout");
			case shapetypeUpDownArrow:					return (L"upDownArrow");
			case shapetypeUpDownArrowCallout:			return (L"upDownArrowCallout");
			case shapetypeUturnArrow:					return (L"uturnArrow");
			case shapetypeVerticalScroll:				return (L"verticalScroll");
			case shapetypeWave:							return (L"wave");
			case shapetypeWedgeEllipseCallout:			return (L"wedgeEllipseCallout");
			case shapetypeWedgeRectCallout:				return (L"wedgeRectCallout");
			case shapetypeWedgeRoundRectCallout:		return (L"wedgeRoundRectCallout");	
	///new//
			case shapetypeBallon:						return (L"Balloon"); 
			case shapetypeRightUpArrow:					return (L"rightUpArrow");
			case shapetypeTextArchDownPour:				return (L"textArchDownPour");
			case shapetypeTextArchUpPour:				return (L"textArchUpPour");
			case shapetypeTextCanDown:					return (L"textCanDown");
			case shapetypeTextCanUp:					return (L"textCanUp");
			case shapetypeTextCirclePour:				return (L"textCirclePour");
			case shapetypeTextCurveDown:				return (L"textCurveDown");
			case shapetypeTextCurveUp:					return (L"textCurveUp");
			case shapetypeTextDeflate:					return (L"textDeflate");
			case shapetypeTextDeflateBottom:			return (L"textDeflateBottom");
			case shapetypeTextDeflateInflate:			return (L"textDeflateInflate");
			case shapetypeTextDeflateInflateDeflat:		return (L"textDeflateInflateDeflate");
			case shapetypeTextDeflateTop:				return (L"textDeflateTop");
			case shapetypeTextDoubleWave1:				return (L"textDoubleWave1");
			case shapetypeTextFadeDown:					return (L"textFadeDown");
			case shapetypeTextFadeLeft:					return (L"textFadeLeft");
			case shapetypeTextFadeRight:				return (L"textFadeRight");
			case shapetypeTextFadeUp:					return (L"textFadeUp");
			case shapetypeTextInflateBottom:			return (L"textInflateBottom");
			case shapetypeTextInflateTop:				return (L"textInflateTop");
			case shapetypeTextRingInside:				return (L"textRingInside");
			case shapetypeTextRingOutside:				return (L"textRingOutside");
			case shapetypeTextWave1:					return (L"textWave1");
			case shapetypeTextWave2:					return (L"textWave2");
			case shapetypeTextWave4:					return (L"textWave4");
			case shapetypeThickArrow:					return (L"thickArrow"); 

			default :									return (L"rect");
			}
		}

		SimpleType_FromString     (EShapeType)
		SimpleType_Operator_Equal (CShapeType)
		
		EShapeType GetValue() const
		{
            return this->m_eValue;
		}
	};


	//--------------------------------------------------------------------------------
    // SystemColorVal 20.1.10.58 (Part 1) (included from ASCWinAPI.h)
	//--------------------------------------------------------------------------------		



	enum ESystemColorVal
	{
		systemcolorval3dDkShadow = 0, // (3D Dark System Color) 
		systemcolorval3dLight, // (3D Light System Color) 
		systemcolorvalActiveBorder, // (Active Border System Color) 
		systemcolorvalActiveCaption, // (Active Caption System Color) 
		systemcolorvalAppWorkspace, // (Application Workspace System Color) 
		systemcolorvalBackground, // (Background System Color) 
		systemcolorvalBtnFace, // (Button Face System Color) 
		systemcolorvalBtnHighlight, // (Button Highlight System Color) 
		systemcolorvalBtnShadow, // (Button Shadow System Color) 
		systemcolorvalBtnText, // (Button Text System Color) 
		systemcolorvalCaptionText, // (Caption Text System Color) 
		systemcolorvalGradientActiveCaption, // (Gradient Active Caption System Color) 
		systemcolorvalGradientInactiveCaption, // (Gradient Inactive Caption System Color) 
		systemcolorvalGrayText, // (Gray Text System Color) 
		systemcolorvalHighlight, // (Highlight System Color) 
		systemcolorvalHighlightText, // (Highlight Text System Color) 
		systemcolorvalHotLight, // (Hot Light System Color) 
		systemcolorvalInactiveBorder, // (Inactive Border System Color) 
		systemcolorvalInactiveCaption, // (Inactive Caption System Color) 
		systemcolorvalInactiveCaptionText, // (Inactive Caption Text System Color) 
		systemcolorvalInfoBk, // (Info Back System Color) 
		systemcolorvalInfoText, // (Info Text System Color) 
		systemcolorvalMenu, // (Menu System Color) 
		systemcolorvalMenuBar, // (Menu Bar System Color) 
		systemcolorvalMenuHighlight, // (Menu Highlight System Color) 
		systemcolorvalMenuText, // (Menu Text System Color) 
		systemcolorvalScrollBar, // (Scroll Bar System Color) 
		systemcolorvalWindow, // (Window System Color) 
		systemcolorvalWindowFrame, // (Window Frame System Color) 
		systemcolorvalWindowText, // (Window Text System Color) 
	};

	template<ESystemColorVal eDefValue = systemcolorvalWindow>
	class CSystemColorVal : public CSimpleType<ESystemColorVal, eDefValue>
	{
	public:
		CSystemColorVal() {}

        virtual ESystemColorVal FromString(std::wstring &sValue)
		{
            this->m_eValue = systemcolorvalWindow;
			SetRGBA( 0, 0, 0, 255 );

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case '3':
                if      ( (L"3dDkShadow")              == sValue ) { this->m_eValue = systemcolorval3dDkShadow; SetRGBASys( COLOR_3DDKSHADOW ); }
                else if ( (L"3dLight")                 == sValue ) { this->m_eValue = systemcolorval3dLight; SetRGBASys( COLOR_3DLIGHT ); }
				break;
			case 'a':
                if      ( (L"activeBorder")            == sValue ) { this->m_eValue = systemcolorvalActiveBorder; SetRGBASys( COLOR_ACTIVEBORDER ); }
                else if ( (L"activeCaption")           == sValue ) { this->m_eValue = systemcolorvalActiveCaption; SetRGBASys( COLOR_ACTIVECAPTION ); }
                else if ( (L"appWorkspace")            == sValue ) { this->m_eValue = systemcolorvalAppWorkspace; SetRGBASys( COLOR_APPWORKSPACE ); }
				break;
			case 'b':
                if      ( (L"background")              == sValue ) { this->m_eValue = systemcolorvalBackground; SetRGBASys( COLOR_BACKGROUND ); }
                else if ( (L"btnFace")                 == sValue ) { this->m_eValue = systemcolorvalBtnFace ; SetRGBASys( COLOR_BTNFACE ); }
                else if ( (L"btnHighlight")            == sValue ) { this->m_eValue = systemcolorvalBtnHighlight ; SetRGBASys( COLOR_BTNHIGHLIGHT ); }
                else if ( (L"btnShadow")               == sValue ) { this->m_eValue = systemcolorvalBtnShadow ; SetRGBASys( COLOR_BTNSHADOW ); }
                else if ( (L"btnText")                 == sValue ) { this->m_eValue = systemcolorvalBtnText ; SetRGBASys( COLOR_BTNTEXT ); }
				break;
			case 'c':
                if      ( (L"captionText")             == sValue ) { this->m_eValue = systemcolorvalCaptionText ; SetRGBASys( COLOR_CAPTIONTEXT ); }
				break;
			case 'g':
                if      ( (L"gradientActiveCaption")   == sValue ) { this->m_eValue = systemcolorvalGradientActiveCaption ; SetRGBASys( COLOR_GRADIENTACTIVECAPTION ); }
                else if ( (L"gradientInactiveCaption") == sValue ) { this->m_eValue = systemcolorvalGradientInactiveCaption ; SetRGBASys( COLOR_GRADIENTINACTIVECAPTION ); }
                else if ( (L"grayText")                == sValue ) { this->m_eValue = systemcolorvalGrayText ; SetRGBASys( COLOR_GRAYTEXT ); }
				break;
			case 'h':
                if      ( (L"highlight")               == sValue ) { this->m_eValue = systemcolorvalHighlight ; SetRGBASys( COLOR_HIGHLIGHT ); }
                else if ( (L"highlightText")           == sValue ) { this->m_eValue = systemcolorvalHighlightText ; SetRGBASys( COLOR_HIGHLIGHTTEXT ); }
                else if ( (L"hotLight")                == sValue ) { this->m_eValue = systemcolorvalHotLight ; SetRGBASys( COLOR_HOTLIGHT ); }
				break;
			case 'i':
                if      ( (L"inactiveBorder")          == sValue ) { this->m_eValue = systemcolorvalInactiveBorder ; SetRGBASys( COLOR_INACTIVEBORDER ); }
                else if ( (L"inactiveCaption")         == sValue ) { this->m_eValue = systemcolorvalInactiveCaption ; SetRGBASys( COLOR_INACTIVECAPTION ); }
                else if ( (L"inactiveCaptionText")     == sValue ) { this->m_eValue = systemcolorvalInactiveCaptionText ; SetRGBASys( COLOR_INACTIVECAPTIONTEXT ); }
                else if ( (L"infoBk")                  == sValue ) { this->m_eValue = systemcolorvalInfoBk ; SetRGBASys( COLOR_INFOBK ); }
                else if ( (L"infoText")                == sValue ) { this->m_eValue = systemcolorvalInfoText ; SetRGBASys( COLOR_INFOTEXT ); }
				break;
			case 'm':
                if      ( (L"menu")                    == sValue ) { this->m_eValue = systemcolorvalMenu ; SetRGBASys( COLOR_MENU ); }
                else if ( (L"menuBar")                 == sValue ) { this->m_eValue = systemcolorvalMenuBar ; SetRGBASys( COLOR_MENUBAR ); }
                else if ( (L"menuHighlight")           == sValue ) { this->m_eValue = systemcolorvalMenuHighlight ; SetRGBASys( COLOR_MENUHILIGHT ); }
                else if ( (L"menuText")                == sValue ) { this->m_eValue = systemcolorvalMenuText ; SetRGBASys( COLOR_MENUTEXT ); }
				break;
			case 's':
                if      ( (L"scrollBar")               == sValue ) { this->m_eValue = systemcolorvalScrollBar ; SetRGBASys( COLOR_SCROLLBAR ); }
				break;
			case 'w':
                if      ( (L"window")                  == sValue ) { this->m_eValue = systemcolorvalWindow ; SetRGBASys( COLOR_WINDOW ); }
                else if ( (L"windowFrame")             == sValue ) { this->m_eValue = systemcolorvalWindowFrame ; SetRGBASys( COLOR_WINDOWFRAME ); }
                else if ( (L"windowText")              == sValue ) { this->m_eValue = systemcolorvalWindowText ; SetRGBASys( COLOR_WINDOWTEXT ); }
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring         ToString  () const
		{
            switch(this->m_eValue)
			{
			case systemcolorval3dDkShadow:              return (L"3dDkShadow");
			case systemcolorval3dLight:                 return (L"3dLight");
			case systemcolorvalActiveBorder:            return (L"activeBorder");
			case systemcolorvalActiveCaption:           return (L"activeCaption");
			case systemcolorvalAppWorkspace:            return (L"appWorkspace");
			case systemcolorvalBackground:              return (L"background");
			case systemcolorvalBtnFace:                 return (L"btnFace");
			case systemcolorvalBtnHighlight:            return (L"btnHighlight");
			case systemcolorvalBtnShadow:               return (L"btnShadow");
			case systemcolorvalBtnText:                 return (L"btnText");
			case systemcolorvalCaptionText:             return (L"captionText");
			case systemcolorvalGradientActiveCaption:   return (L"gradientActiveCaption");
			case systemcolorvalGradientInactiveCaption: return (L"gradientInactiveCaption");
			case systemcolorvalGrayText:                return (L"grayText");
			case systemcolorvalHighlight:               return (L"highlight");
			case systemcolorvalHighlightText:           return (L"highlightText");
			case systemcolorvalHotLight:                return (L"hotLight");
			case systemcolorvalInactiveBorder:          return (L"inactiveBorder");
			case systemcolorvalInactiveCaption:         return (L"inactiveCaption");
			case systemcolorvalInactiveCaptionText:     return (L"inactiveCaptionText");
			case systemcolorvalInfoBk:                  return (L"infoBk");
			case systemcolorvalInfoText:                return (L"infoText");
			case systemcolorvalMenu:                    return (L"menu");
			case systemcolorvalMenuBar:                 return (L"menuBar");
			case systemcolorvalMenuHighlight:           return (L"menuHighlight");
			case systemcolorvalMenuText:                return (L"menuText");
			case systemcolorvalScrollBar:               return (L"scrollBar");
			case systemcolorvalWindow:                  return (L"window");
			case systemcolorvalWindowFrame:             return (L"windowFrame");
			case systemcolorvalWindowText:              return (L"windowText");
			default :                                   return (L"window");
			}
		}

		SimpleType_FromString     (ESystemColorVal)
		SimpleType_Operator_Equal (CSystemColorVal)
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

		void SetRGBASys(int nIndex)
		{
            DWORD dwRGB = GetSysColor(nIndex);

			m_unB = static_cast<unsigned char>(dwRGB & 0xFF);
			m_unG = static_cast<unsigned char>((dwRGB & 0xFF00)>>8);
			m_unR = static_cast<unsigned char>((dwRGB & 0xFF0000)>>16);
			m_unA = 255;
		}

		void SetRGBA(unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA = 255)
		{
			m_unR = unR;
			m_unG = unG;
			m_unB = unB;
			m_unA = unA;
		}

	private:

		unsigned char m_unR;
		unsigned char m_unG;
		unsigned char m_unB;
		unsigned char m_unA;
	};

	//--------------------------------------------------------------------------------
	// TextAlignmentType 20.1.10.59 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextAlignmentType
	{
		textalignmenttypeL    = 0,
		textalignmenttypeCtr  = 1,
		textalignmenttypeDist = 2,
		textalignmenttypeJust = 3,
		textalignmenttypeR    = 4,
	};
	template<ETextAlignmentType eDefValue = textalignmenttypeL>
	class CTextAlignmentType : public CSimpleType<ETextAlignmentType, eDefValue>
	{
	public:
		CTextAlignmentType() {}

        virtual ETextAlignmentType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'l':
                if      ( (L"l")    == sValue ) this->m_eValue = textalignmenttypeL;
				break;
			case 'c':
                if      ( (L"ctr")  == sValue ) this->m_eValue = textalignmenttypeCtr;
				break;
			case 'd':
                if      ( (L"dist") == sValue ) this->m_eValue = textalignmenttypeDist;
				break;
			case 'j':
                if      ( (L"just") == sValue ) this->m_eValue = textalignmenttypeJust;
				break;
			case 'r':
                if      ( (L"r")    == sValue ) this->m_eValue = textalignmenttypeR;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(this->m_eValue)
			{
			case textalignmenttypeR   : return (L"r");			
			case textalignmenttypeCtr : return (L"ctr");		
			case textalignmenttypeDist: return (L"dist");		
			case textalignmenttypeJust: return (L"just");		
			case textalignmenttypeL   : return (L"l");		
			default                   : return (L"l");
			}
		}

		SimpleType_FromString     (ETextAlignmentType)
		SimpleType_Operator_Equal (CTextAlignmentType)
	};

	
	//--------------------------------------------------------------------------------
	// TextAnchoringType 20.1.10.60 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextAnchoringType
	{
		textanchoringtypeB    = 0,
		textanchoringtypeCtr  = 1,
		textanchoringtypeDist = 2,
		textanchoringtypeJust = 3,
		textanchoringtypeT    = 4,
	};

	template<ETextAnchoringType eDefValue = textanchoringtypeT>
	class CTextAnchoringType : public CSimpleType<ETextAnchoringType, eDefValue>
	{
	public:
		CTextAnchoringType() {}

        virtual ETextAnchoringType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'b':
                if      ( (L"b")    == sValue ) this->m_eValue = textanchoringtypeB;
				break;
			case 'c':
                if      ( (L"ctr")  == sValue ) this->m_eValue = textanchoringtypeCtr;
				break;
			case 'd':
                if      ( (L"dist") == sValue ) this->m_eValue = textanchoringtypeDist;
				break;
			case 'j':
                if      ( (L"just") == sValue ) this->m_eValue = textanchoringtypeJust;
				break;
			case 't':
                if      ( (L"t")    == sValue ) this->m_eValue = textanchoringtypeT;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(this->m_eValue)
			{
			case textanchoringtypeB   : return (L"b");			
			case textanchoringtypeCtr : return (L"ctr");		
			case textanchoringtypeDist: return (L"dist");		
			case textanchoringtypeJust: return (L"just");		
			case textanchoringtypeT   : return (L"t");		
			default                   : return (L"t");
			}
		}

		SimpleType_FromString     (ETextAnchoringType)
		SimpleType_Operator_Equal (CTextAnchoringType)
	};


	//--------------------------------------------------------------------------------
    // TextColumnCount 20.1.10.65 (Part 1)
	//--------------------------------------------------------------------------------  
    template<unsigned char unDefValue = 1>
    class CTextColumnCount : public CSimpleType<unsigned char, unDefValue>
    {
    public:
        CTextColumnCount() {}

		virtual void    SetValue  (unsigned char unValue)
		{
            this->m_eValue = (std::min)( 16, (std::max)( 1, (int) unValue ) );
		}
        virtual unsigned char FromString(std::wstring &sValue)
        {
            this->m_eValue = (unsigned char)_wtoi( sValue.c_str() );

            if (this->m_eValue < 1)
                this->m_eValue = 1;
            if (this->m_eValue > 16)
                this->m_eValue = 16;

            return this->m_eValue;
        }

        virtual std::wstring       ToString  () const
        {
            std::wstring sResult = std::to_wstring(this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (unsigned char)
        SimpleType_Operator_Equal (CTextColumnCount)
    };
	
	//--------------------------------------------------------------------------------
    // 20.1.10.66 TextFontAlignType (Font Alignment Types)
	//--------------------------------------------------------------------------------  
	enum ETextFontAlignType
	{
		textfontaligntypeAuto   = 0,
		textfontaligntypeCtr	= 1,
		textfontaligntypeBase	= 2,
		textfontaligntypeT		= 3,
		textfontaligntypeB		= 4,
	};
	template<ETextFontAlignType eDefValue = textfontaligntypeAuto>
	class CTextFontAlignType : public CSimpleType<ETextFontAlignType, eDefValue>
	{
	public:
		CTextFontAlignType() {}

        virtual ETextFontAlignType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'a':
                if      ( (L"a")    == sValue ) this->m_eValue = textfontaligntypeAuto;
				break;
			case 'c':
                if      ( (L"ctr")  == sValue ) this->m_eValue = textfontaligntypeCtr;
				break;
			case 'b':
                if      ( (L"base") == sValue ) this->m_eValue = textfontaligntypeBase;
                if      ( (L"b")    == sValue ) this->m_eValue = textfontaligntypeB;
			break;
			case 't':
                if      ( (L"just") == sValue ) this->m_eValue = textfontaligntypeT;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring            ToString  () const
		{
            switch(this->m_eValue)
			{
			case textfontaligntypeB   : return (L"b");			
			case textfontaligntypeCtr : return (L"ctr");		
			case textfontaligntypeT	  : return (L"t");		
			case textfontaligntypeBase: return (L"base");		
			case textfontaligntypeAuto: return (L"auto");		
			default                   : return (L"auto");
			}
		}

		SimpleType_FromString     (ETextFontAlignType)
		SimpleType_Operator_Equal (CTextFontAlignType)
	};

	
	//--------------------------------------------------------------------------------
    // TextFontScalePercentOrPercentString 20.1.10.67 (Part 1) + 12.1.2.5 (Part4)
	//--------------------------------------------------------------------------------
    class CTextFontScalePercentOrPercentString 
    {
    public:
        CTextFontScalePercentOrPercentString() 
		{
			m_dValue = 0;
		}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
			m_dValue = dValue;
		}

        virtual double FromString(std::wstring &sValue)
        {
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos && nLen > 0)
				{
					// Поправка 12.1.2.5 (Part4)
                    int nValue = (std::min)( 100000, (std::max)( 1000, _wtoi( sValue.c_str() ) ) );
					m_dValue = nValue / 1000.0;
				}
				return
					m_dValue = 0;
			}
			else
                m_dValue = _wtof( sValue.substr( 0, nLen - 1).c_str() );

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
			return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CTextFontScalePercentOrPercentString)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
	// TextHorzOverflowType 20.1.10.69 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextHorzOverflowType
	{
		texthorzoverflowtypeClip     = 0,
		texthorzoverflowtypeOverflow = 1,
	};

	template<ETextHorzOverflowType eDefValue = texthorzoverflowtypeOverflow>
	class CTextHorzOverflowType : public CSimpleType<ETextHorzOverflowType, eDefValue>
	{
	public:
		CTextHorzOverflowType() {}

        virtual ETextHorzOverflowType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"clip")     == sValue ) this->m_eValue = texthorzoverflowtypeClip;
				break;
			case 'o':
                if      ( (L"overflow") == sValue ) this->m_eValue = texthorzoverflowtypeOverflow;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring               ToString  () const
		{
            switch(this->m_eValue)
			{
			case texthorzoverflowtypeClip     : return (L"clip");			
			case texthorzoverflowtypeOverflow : return (L"overflow");		
			default                           : return (L"overflow");
			}
		}

		SimpleType_FromString     (ETextHorzOverflowType)
		SimpleType_Operator_Equal (CTextHorzOverflowType)
	};


	//--------------------------------------------------------------------------------
    // 20.1.10.70 ST_TextIndent (Text Indentation)
	//--------------------------------------------------------------------------------
    template<__int64 nDefValue = 0>
    class CTextIndent : public CSimpleType<__int64, nDefValue>
    {
    public:
        CTextIndent() {}

        virtual __int64 FromString(std::wstring &sValue)
        {
            this->m_eValue = _wtoi64( sValue.c_str() );
            if (this->m_eValue < -51206400)
                this->m_eValue = -51206400;
            if (this->m_eValue > 51206400)
                this->m_eValue = 51206400;

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (__int64)
        SimpleType_Operator_Equal (CTextIndent)

        double ToPoints()
        {
            return  Emu_To_Pt(this->m_eValue);
        }

        double ToInches()
        {
            return Emu_To_Inch( this->m_eValue );
        }
        double ToCm()
        {
            return Emu_To_Cm( this->m_eValue );
        }
    };
	//--------------------------------------------------------------------------------
    // 20.1.10.72 ST_TextMargin (Text Margin)
	//--------------------------------------------------------------------------------
    template<__int64 nDefValue = 0>
    class CTextMargin : public CSimpleType<__int64, nDefValue>
    {
    public:
        CTextMargin() {}

        virtual __int64 FromString(std::wstring &sValue)
        {
            this->m_eValue = _wtoi64( sValue.c_str() );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 51206400)
                this->m_eValue = 51206400;

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (__int64)
        SimpleType_Operator_Equal (CTextMargin)

        double ToPoints()
        {
            return  Emu_To_Pt(this->m_eValue);
        }

        double ToInches()
        {
            return Emu_To_Inch( this->m_eValue );
        }
        double ToCm()
        {
            return Emu_To_Cm( this->m_eValue );
        }
    };
//	//--------------------------------------------------------------------------------
//    // 20.1.10.74 ST_TextPoint (Text Point)
//	//--------------------------------------------------------------------------------
//    template<__int64 nDefValue = 0>
//    class CTextPoint : public CSimpleType<__int64, nDefValue>
//    {
//    public:
///*	The ST_TextPointUnqualified simple type (§20.1.10.75).
//	The ST_UniversalMeasure simple type (§22.9.2.15). */     
//	CTextPoint() {}
//
//        virtual __int64 FromString(std::wstring &sValue)
//        {
//            this->m_eValue = _wtoi64( sValue );
//            if (this->m_eValue < 0)
//                this->m_eValue = 0;
//            if (this->m_eValue > 51206400)
//                this->m_eValue = 51206400;
//
//            return this->m_eValue;
//        }
//
//        virtual std::wstring ToString  () const
//        {
//            std::wstring sResult = std::to_wstring( this->m_eValue);
//
//            return sResult;
//        }
//
//        SimpleType_FromString     (__int64)
//        SimpleType_Operator_Equal (TextPoint)
//
//        double ToPoints()
//        {
//            return  Emu_To_Pt(this->m_eValue);
//        }
//
//        double ToInches()
//        {
//            return Emu_To_Inch( this->m_eValue );
//        }
//        double ToCm()
//        {
//            return Emu_To_Cm( this->m_eValue );
//        }
//    };
	//--------------------------------------------------------------------------------
	// TextShapeType 20.1.10.76 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextShapeType
	{
		textshapetypeTextArchDown = 0,
		textshapetypeTextArchDownPour,
		textshapetypeTextArchUp,
		textshapetypeTextArchUpPour,
		textshapetypeTextButton,
		textshapetypeTextButtonPour,
		textshapetypeTextCanDown,
		textshapetypeTextCanUp,
		textshapetypeTextCascadeDown,
		textshapetypeTextCascadeUp,
		textshapetypeTextChevron,
		textshapetypeTextChevronInverted,
		textshapetypeTextCircle,
		textshapetypeTextCirclePour,
		textshapetypeTextCurveDown,
		textshapetypeTextCurveUp,
		textshapetypeTextDeflate,
		textshapetypeTextDeflateBottom,
		textshapetypeTextDeflateInflate,
		textshapetypeTextDeflateInflateDeflate,
		textshapetypeTextDeflateTop,
		textshapetypeTextDoubleWave1,
		textshapetypeTextFadeDown,
		textshapetypeTextFadeLeft,
		textshapetypeTextFadeRight,
		textshapetypeTextFadeUp,
		textshapetypeTextInflate,
		textshapetypeTextInflateBottom,
		textshapetypeTextInflateTop,
		textshapetypeTextNoShape,
		textshapetypeTextPlain,
		textshapetypeTextRingInside,
		textshapetypeTextRingOutside,
		textshapetypeTextSlantDown,
		textshapetypeTextSlantUp,
		textshapetypeTextStop,
		textshapetypeTextTriangle,
		textshapetypeTextTriangleInverted,
		textshapetypeTextWave1,
		textshapetypeTextWave2,
		textshapetypeTextWave4,
	};

	template<ETextShapeType eDefValue = textshapetypeTextPlain>
	class CTextShapeType : public CSimpleType<ETextShapeType, eDefValue>
	{
	public:
		CTextShapeType() {}

        virtual ETextShapeType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.length() <= 5 )
                return this->m_eValue;

			wchar_t wChar = sValue[4];
			switch ( wChar )
			{
			case 'A':

                if      ( (L"textArchDown")		== sValue ) this->m_eValue = textshapetypeTextArchDown;
                else if ( (L"textArchDownPour")	== sValue ) this->m_eValue = textshapetypeTextArchDownPour;
                else if ( (L"textArchUp")			== sValue ) this->m_eValue = textshapetypeTextArchUp;
                else if ( (L"textArchUpPour")		== sValue ) this->m_eValue = textshapetypeTextArchUpPour;
				break;

			case 'B':

                if      ( (L"textButton")			== sValue ) this->m_eValue = textshapetypeTextButton;
                else if ( (L"textButtonPour")		== sValue ) this->m_eValue = textshapetypeTextButtonPour;
				break;

			case 'C':

                if      ( (L"textCanDown")			== sValue ) this->m_eValue = textshapetypeTextCanDown;
                else if ( (L"textCanUp")			== sValue ) this->m_eValue = textshapetypeTextCanUp;
                else if ( (L"textCascadeDown")		== sValue ) this->m_eValue = textshapetypeTextCascadeDown;
                else if ( (L"textCascadeUp")		== sValue ) this->m_eValue = textshapetypeTextCascadeUp;
                else if ( (L"textChevron")			== sValue ) this->m_eValue = textshapetypeTextChevron;
                else if ( (L"textChevronInverted")	== sValue ) this->m_eValue = textshapetypeTextChevronInverted;
                else if ( (L"textCircle")			== sValue ) this->m_eValue = textshapetypeTextCircle;
                else if ( (L"textCirclePour")		== sValue ) this->m_eValue = textshapetypeTextCirclePour;
                else if ( (L"textCurveDown")		== sValue ) this->m_eValue = textshapetypeTextCurveDown;
                else if ( (L"textCurveUp")			== sValue ) this->m_eValue = textshapetypeTextCurveUp;
				break;

			case 'D':
                if      ( (L"textDeflate")			== sValue ) this->m_eValue = textshapetypeTextDeflate;
                else if ( (L"textDeflateBottom")	== sValue ) this->m_eValue = textshapetypeTextDeflateBottom;
                else if ( (L"textDeflateInflate")	== sValue ) this->m_eValue = textshapetypeTextDeflateInflate;
                else if ( (L"textDeflateInflateDeflate") == sValue ) this->m_eValue = textshapetypeTextDeflateInflateDeflate;
                else if ( (L"textDeflateTop")		== sValue ) this->m_eValue = textshapetypeTextDeflateTop;
                else if ( (L"textDoubleWave1")		== sValue ) this->m_eValue = textshapetypeTextDoubleWave1;
				break;

			case 'F':
                if      ( (L"textFadeDown")		== sValue ) this->m_eValue = textshapetypeTextFadeDown;
                else if ( (L"textFadeLeft")		== sValue ) this->m_eValue = textshapetypeTextFadeLeft;
                else if ( (L"textFadeRight")		== sValue ) this->m_eValue = textshapetypeTextFadeRight;
                else if ( (L"textFadeUp")			== sValue ) this->m_eValue = textshapetypeTextFadeUp;
				break;

			case 'I':

                if      ( (L"textInflate")			== sValue ) this->m_eValue = textshapetypeTextInflate;
                else if ( (L"textInflateBottom")	== sValue ) this->m_eValue = textshapetypeTextInflateBottom;
                else if ( (L"textInflateTop")		== sValue ) this->m_eValue = textshapetypeTextInflateTop;
				break;

			case 'N':
                if      ( (L"textNoShape")			== sValue ) this->m_eValue = textshapetypeTextNoShape;
				break;

			case 'P':

                if      ( (L"textPlain")			== sValue ) this->m_eValue = textshapetypeTextPlain;
				break;

			case 'R':

                if		( (L"textRingInside")		== sValue ) this->m_eValue = textshapetypeTextRingInside;
                else if ( (L"textRingOutside")		== sValue ) this->m_eValue = textshapetypeTextRingOutside;
				break;

			case 'S':

                if      ( (L"textSlantDown")		== sValue ) this->m_eValue = textshapetypeTextSlantDown;
                else if ( (L"textSlantUp")			== sValue ) this->m_eValue = textshapetypeTextSlantUp;
                else if ( (L"textStop")			== sValue ) this->m_eValue = textshapetypeTextStop;
				break;

			case 'T':
				
                if      ( (L"textTriangle")		== sValue ) this->m_eValue = textshapetypeTextTriangle;
                else if ( (L"textTriangleInverted")== sValue ) this->m_eValue = textshapetypeTextTriangleInverted;
				break;

			case 'W':
				
                if      ( (L"textWave1")			== sValue ) this->m_eValue = textshapetypeTextWave1;
                else if ( (L"textWave2")			== sValue ) this->m_eValue = textshapetypeTextWave2;
                else if ( (L"textWave4")			== sValue ) this->m_eValue = textshapetypeTextWave4;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring        ToString  () const
		{
            switch(this->m_eValue)
			{
			case textshapetypeTextArchDown:				return (L"textArchDown");
			case textshapetypeTextArchDownPour:			return (L"textArchDownPour");
			case textshapetypeTextArchUp:				return (L"textArchUp");
			case textshapetypeTextArchUpPour:			return (L"textArchUpPour");
			case textshapetypeTextButton:				return (L"textButton");
			case textshapetypeTextButtonPour:			return (L"textButtonPour");
			case textshapetypeTextCanDown:				return (L"textCanDown");
			case textshapetypeTextCanUp:				return (L"textCanUp");
			case textshapetypeTextCascadeDown:			return (L"textCascadeDown");
			case textshapetypeTextCascadeUp:			return (L"textCascadeUp");
			case textshapetypeTextChevron:				return (L"textChevron");
			case textshapetypeTextChevronInverted:		return (L"textChevronInverted");
			case textshapetypeTextCircle:				return (L"textCircle");
			case textshapetypeTextCirclePour:			return (L"textCirclePour");
			case textshapetypeTextCurveDown:			return (L"textCurveDown");
			case textshapetypeTextCurveUp:				return (L"textCurveUp");
			case textshapetypeTextDeflate:				return (L"textDeflate");
			case textshapetypeTextDeflateBottom:		return (L"textDeflateBottom");
			case textshapetypeTextDeflateInflate:		return (L"textDeflateInflate");
			case textshapetypeTextDeflateInflateDeflate:return (L"textDeflateInflateDeflate");
			case textshapetypeTextDeflateTop:			return (L"textDeflateTop");
			case textshapetypeTextDoubleWave1:			return (L"textDoubleWave1");
			case textshapetypeTextFadeDown:				return (L"textFadeDown");
			case textshapetypeTextFadeLeft:				return (L"textFadeLeft");
			case textshapetypeTextFadeRight:			return (L"textFadeRight");
			case textshapetypeTextFadeUp:				return (L"textFadeUp");
			case textshapetypeTextInflate:				return (L"textInflate");
			case textshapetypeTextInflateBottom:		return (L"textInflateBottom");
			case textshapetypeTextInflateTop:			return (L"textInflateTop");
			case textshapetypeTextNoShape:				return (L"textNoShape");
			case textshapetypeTextPlain:				return (L"textPlain");
			case textshapetypeTextRingInside:			return (L"textRingInside");
			case textshapetypeTextRingOutside:			return (L"textRingOutside");
			case textshapetypeTextSlantDown:			return (L"textSlantDown");
			case textshapetypeTextSlantUp:				return (L"textSlantUp");
			case textshapetypeTextStop:					return (L"textStop");
			case textshapetypeTextTriangle:				return (L"textTriangle");
			case textshapetypeTextTriangleInverted:		return (L"textTriangleInverted");
			case textshapetypeTextWave1:				return (L"textWave1");
			case textshapetypeTextWave2:				return (L"textWave2");
			case textshapetypeTextWave4:				return (L"textWave4");
			default :									return (L"textPlain");
			}
		}

		SimpleType_FromString     (ETextShapeType)
		SimpleType_Operator_Equal (CTextShapeType)
	};


	//--------------------------------------------------------------------------------
    // TextSpacingPercentOrPercentString 20.1.10.77 (Part 1) + 12.1.2.7 (Part4)
	//--------------------------------------------------------------------------------
    class CTextSpacingPercentOrPercentString
    {
    public:
        CTextSpacingPercentOrPercentString() 
		{
			m_dValue = 0;
		}

		double GetValue() const
		{
			return m_dValue;
		}

		void   SetValue(double dValue)
		{
			m_dValue = dValue;
		}

        virtual double FromString(std::wstring &sValue)
        {
            int nPos = (int)sValue.find( '%' );
            int nLen = (int)sValue.length();
            if ( -1 == nPos || nPos != (int)sValue.length() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos && nLen > 0)
				{
					// Поправка 12.1.2.7 (Part4)
                    int nValue = (std::min)( 13200000, (std::max)( 0, _wtoi( sValue.c_str() ) ) );
					m_dValue = nValue / 1000.0;
				}
				return m_dValue;
			}
			else
                m_dValue = _wtof( sValue.substr( 0, nLen - 1 ).c_str() );

            return m_dValue;
        }

        virtual std::wstring ToString  () const
        {
			return boost::lexical_cast<std::wstring>(m_dValue) + L"%";
        }

        SimpleType_FromString2    (double)
        SimpleType_Operator_Equal (CTextSpacingPercentOrPercentString)

	private:

		double m_dValue;
    };
	//--------------------------------------------------------------------------------
    // 20.1.10.78 ST_TextSpacingPoint (Text Spacing Point)
	//--------------------------------------------------------------------------------
    template<__int64 nDefValue = 0>
    class CTextSpacingPoint : public CSimpleType<__int64, nDefValue>
    {
    public:
        CTextSpacingPoint() {}

        virtual __int64 FromString(std::wstring &sValue)
        {
            this->m_eValue = _wtoi64( sValue.c_str() );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 158400)
                this->m_eValue = 158400;

            return this->m_eValue;
        }

        virtual std::wstring ToString  () const
        {
            std::wstring sResult = std::to_wstring( this->m_eValue);

            return sResult;
        }

        SimpleType_FromString     (__int64)
        SimpleType_Operator_Equal (CTextSpacingPoint)

        double ToPoints()
        {
            return  this->m_eValue;
        }

        double ToInches()
        {
            return Pt_To_Inch( this->m_eValue );
        }
        double ToCm()
        {
            return Pt_To_Cm( this->m_eValue );
        }
    };
	//--------------------------------------------------------------------------------
	// TextTypeface 20.1.10.81 (Part 1)
	//--------------------------------------------------------------------------------		
	class CTextTypeface
	{
	public:
		CTextTypeface() {}

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
		SimpleType_Operator_Equal (CTextTypeface)

	private:

        std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// TextVerticalType 20.1.10.83 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextVerticalType
	{
		textverticaltypeEaVert         = 0,
		textverticaltypeHorz           = 1,
		textverticaltypeMongolianVert  = 2,
		textverticaltypeVert           = 3,
		textverticaltypeVert270        = 4,
		textverticaltypeWordArtVert    = 5,
		textverticaltypeWordArtVertRtl = 6,
	};

	template<ETextVerticalType eDefValue = textverticaltypeHorz>
	class CTextVerticalType : public CSimpleType<ETextVerticalType, eDefValue>
	{
	public:
		CTextVerticalType() {}

        virtual ETextVerticalType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'e':
                if      ( (L"eaVert")         == sValue ) this->m_eValue = textverticaltypeEaVert;
				break;
			case 'h':
                if      ( (L"horz")           == sValue ) this->m_eValue = textverticaltypeHorz;
				break;
			case 'm':
                if      ( (L"mongolianVert")  == sValue ) this->m_eValue = textverticaltypeMongolianVert;
				break;
			case 'v':
                if      ( (L"vert")           == sValue ) this->m_eValue = textverticaltypeVert;
                else if ( (L"vert270")        == sValue ) this->m_eValue = textverticaltypeVert270;
				break;
			case 'w':
                if      ( (L"wordArtVert")    == sValue ) this->m_eValue = textverticaltypeWordArtVert;
                else if ( (L"wordArtVertRtl") == sValue ) this->m_eValue = textverticaltypeWordArtVertRtl;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case textverticaltypeEaVert         : return (L"eaVert");			
			case textverticaltypeHorz           : return (L"horz");		
			case textverticaltypeMongolianVert  : return (L"mongolianVert");			
			case textverticaltypeVert           : return (L"vert");		
			case textverticaltypeVert270        : return (L"vert270");			
			case textverticaltypeWordArtVert    : return (L"wordArtVert");		
			case textverticaltypeWordArtVertRtl : return (L"wordArtVertRtl");		
			default                             : return (L"horz");
			}
		}

		SimpleType_FromString     (ETextVerticalType)
		SimpleType_Operator_Equal (CTextVerticalType)
	};


	//--------------------------------------------------------------------------------
	// TextVertOverflowType 20.1.10.84 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextVertOverflowType
	{
		textvertoverflowtypeClip     = 0,
		textvertoverflowtypeEllipsis = 1,
		textvertoverflowtypeOverflow = 2,
	};

	template<ETextVertOverflowType eDefValue = textvertoverflowtypeOverflow>
	class CTextVertOverflowType : public CSimpleType<ETextVertOverflowType, eDefValue>
	{
	public:
		CTextVertOverflowType() {}

        virtual ETextVertOverflowType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'c':
                if      ( (L"clip")     == sValue ) this->m_eValue = textvertoverflowtypeClip;
				break;
			case 'e':
                if      ( (L"ellipsis") == sValue ) this->m_eValue = textvertoverflowtypeEllipsis;
				break;
			case 'o':
                if      ( (L"overflow") == sValue ) this->m_eValue = textvertoverflowtypeOverflow;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring              ToString  () const
		{
            switch(this->m_eValue)
			{
			case textvertoverflowtypeClip     : return (L"clip");			
			case textvertoverflowtypeEllipsis : return (L"ellipsis");		
			case textvertoverflowtypeOverflow : return (L"overflow");			
			default                           : return (L"overflow");
			}
		}

		SimpleType_FromString     (ETextVertOverflowType)
		SimpleType_Operator_Equal (CTextVertOverflowType)
	};


	//--------------------------------------------------------------------------------
	// TextWrappingType 20.1.10.85 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETextWrappingType
	{
		textwrappingtypeNone   = 0,
		textwrappingtypeSquare = 1,
	};

	template<ETextWrappingType eDefValue = textwrappingtypeNone>
	class CTextWrappingType : public CSimpleType<ETextWrappingType, eDefValue>
	{
	public:
		CTextWrappingType() {}

        virtual ETextWrappingType FromString(std::wstring &sValue)
		{
            this->m_eValue = eDefValue;

            if ( sValue.empty() )
                return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'n':
                if      ( (L"none")   == sValue ) this->m_eValue = textwrappingtypeNone;
				break;
			case 's':
                if      ( (L"square") == sValue ) this->m_eValue = textwrappingtypeSquare;
				break;
			}

            return this->m_eValue;
		}

        virtual std::wstring           ToString  () const
		{
            switch(this->m_eValue)
			{
			case textwrappingtypeNone   : return (L"none");			
			case textwrappingtypeSquare : return (L"square");		
			default                     : return (L"none");
			}
		}

		SimpleType_FromString     (ETextWrappingType)
		SimpleType_Operator_Equal (CTextWrappingType)
	};


	//--------------------------------------------------------------------------------
	// TileFlipMode 20.1.10.86 (Part 1)
	//--------------------------------------------------------------------------------		
	enum ETileFlipMode
	{
		tileflipmodeNone = 0,
		tileflipmodeX    = 1,
		tileflipmodeXY   = 2,
		tileflipmodeY    = 3,
	};

	template<ETileFlipMode eDefValue = tileflipmodeNone>
	class CTileFlipMode : public CSimpleType<ETileFlipMode, eDefValue>
	{
	public:
		CTileFlipMode() {}

        virtual ETileFlipMode FromString(std::wstring &sValue)
		{
            if      ( (L"none") == sValue ) this->m_eValue = tileflipmodeNone;
            else if ( (L"x")    == sValue ) this->m_eValue = tileflipmodeX;
            else if ( (L"xy")   == sValue ) this->m_eValue = tileflipmodeXY;
            else if ( (L"y")    == sValue ) this->m_eValue = tileflipmodeY;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring       ToString  () const
		{
            switch(this->m_eValue)
			{
			case tileflipmodeNone: return (L"none");
			case tileflipmodeX   : return (L"x");
			case tileflipmodeXY  : return (L"xy");
			case tileflipmodeY   : return (L"y");
			default              : return (L"none");
			}
		}

		SimpleType_FromString     (ETileFlipMode)
		SimpleType_Operator_Equal (CTileFlipMode)
	};


} // SimpleTypes

// Здесь представлены все простые типы Drawing-Word из спецификации Office Open Xml (20.4.3)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AlignH 20.4.3.1 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAlignH
	{
		alignhCenter  = 0,
		alignhInside  = 1,
		alignhLeft    = 2,
		alignhOutside = 3,
		alignhRight   = 4
	};

	template<EAlignH eDefValue = alignhLeft>
	class CAlignH : public CSimpleType<EAlignH, eDefValue>
	{
	public:

		CAlignH() {} 

        virtual EAlignH FromString(std::wstring &sValue)
		{
            if       ( (L"center")  == sValue ) this->m_eValue = alignhCenter;
            else if  ( (L"inside")  == sValue ) this->m_eValue = alignhInside;
            else if  ( (L"left")    == sValue ) this->m_eValue = alignhLeft;
            else if  ( (L"outside") == sValue ) this->m_eValue = alignhOutside;
            else if  ( (L"right")   == sValue ) this->m_eValue = alignhRight;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            switch(this->m_eValue)
			{
			case alignhCenter  : return (L"center");
			case alignhInside  : return (L"inside");
			case alignhLeft    : return (L"left");
			case alignhOutside : return (L"outside");
			case alignhRight   : return (L"right");
			default            : return (L"left");
			}
		}

		SimpleType_FromString     (EAlignH)
		SimpleType_Operator_Equal (CAlignH)
	};



	//--------------------------------------------------------------------------------
	// AlignV 20.4.3.2 (Part 1)
	//--------------------------------------------------------------------------------

	enum EAlignV
	{
		alignvBottom  = 0,
		alignvCenter  = 1,
		alignvInside  = 2,
		alignvOutside = 3,
		alignvTop     = 4
	};

	template<EAlignV eDefValue = alignvTop>
	class CAlignV : public CSimpleType<EAlignV, eDefValue>
	{
	public:

		CAlignV() {} 

        virtual EAlignV FromString(std::wstring &sValue)
		{
            if       ( (L"bottom")  == sValue ) this->m_eValue = alignvBottom;
            else if  ( (L"center")  == sValue ) this->m_eValue = alignvCenter;
            else if  ( (L"inside")  == sValue ) this->m_eValue = alignvInside;
            else if  ( (L"outside") == sValue ) this->m_eValue = alignvOutside;
            else if  ( (L"top")     == sValue ) this->m_eValue = alignvTop;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring ToString() const
		{
            switch(this->m_eValue)
			{
			case alignvBottom  : return (L"bottom");
			case alignvCenter  : return (L"center");
			case alignvInside  : return (L"inside");
			case alignvOutside : return (L"outside");
			case alignvTop     : return (L"top");
			default            : return (L"top");
			}
		}

		SimpleType_FromString     (EAlignV)
		SimpleType_Operator_Equal (CAlignV)
	};



	//--------------------------------------------------------------------------------
	// PositionOffset 10.4.3.3 (Part 1)
	//--------------------------------------------------------------------------------

	template<__int64 nDefValue = 0>
	class CPositionOffset : public CSimpleType<__int64, nDefValue>
	{
	public:
		CPositionOffset() {}

        virtual __int64 FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi64( sValue.c_str() );

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            std::wstring sResult = std::to_wstring( this->m_eValue);

			return sResult;
		}


		SimpleType_FromString     (__int64)
		SimpleType_Operator_Equal (CPositionOffset)

		double ToPoints()
		{
            return Emu_To_Pt( this->m_eValue );
		}
		double ToMM() const
		{
            return Emu_To_Mm( (double)this->m_eValue );
		}
		double ToInches()
		{
            return Emu_To_Inch( this->m_eValue );
		}
		double ToTwips()
		{
            return Emu_To_Twips((double) this->m_eValue );
		}	
	};

	//--------------------------------------------------------------------------------
	// RelFromH 20.4.3.4 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERelFromH
	{
		relfromhCharacter     = 0,
		relfromhColumn        = 1,
		relfromhInsideMargin  = 2,
		relfromhLeftMargin    = 3,
		relfromhMargin        = 4,
		relfromhOutsideMargin = 5,
		relfromhPage          = 6,
		relfromhRightMargin   = 7
	};

	template<ERelFromH eDefValue = relfromhPage>
	class CRelFromH : public CSimpleType<ERelFromH, eDefValue>
	{
	public:

		CRelFromH() {} 

        virtual ERelFromH FromString(std::wstring &sValue)
		{
            if       ( (L"character")     == sValue ) this->m_eValue = relfromhCharacter;
            else if  ( (L"column")        == sValue ) this->m_eValue = relfromhColumn;
            else if  ( (L"insideMargin")  == sValue ) this->m_eValue = relfromhInsideMargin;
            else if  ( (L"leftMargin")    == sValue ) this->m_eValue = relfromhLeftMargin;
            else if  ( (L"margin")        == sValue ) this->m_eValue = relfromhMargin;
            else if  ( (L"outsideMargin") == sValue ) this->m_eValue = relfromhOutsideMargin;
            else if  ( (L"page")          == sValue ) this->m_eValue = relfromhPage;
            else if  ( (L"rightMargin")   == sValue ) this->m_eValue = relfromhRightMargin;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            switch(this->m_eValue)
			{
			case relfromhCharacter     : return (L"character");
			case relfromhColumn        : return (L"column");
			case relfromhInsideMargin  : return (L"insideMargin");
			case relfromhLeftMargin    : return (L"leftMargin");
			case relfromhMargin        : return (L"margin");
			case relfromhOutsideMargin : return (L"outsideMargin");
			case relfromhPage          : return (L"page");
			case relfromhRightMargin   : return (L"rightMargin");
			default                    : return (L"page");
			}
		}

		SimpleType_FromString     (ERelFromH)
		SimpleType_Operator_Equal (CRelFromH)
	};




	//--------------------------------------------------------------------------------
	// RelFromV 20.4.3.5 (Part 1)
	//--------------------------------------------------------------------------------

	enum ERelFromV
	{
		relfromvBottomMargin  = 0,
		relfromvInsideMargin  = 1,
		relfromvLine          = 2,
		relfromvMargin        = 3,
		relfromvOutsideMargin = 4,
		relfromvPage          = 5,
		relfromvParagraph     = 6,
		relfromvTopMargin     = 7
	};

	template<ERelFromV eDefValue = relfromvPage>
	class CRelFromV : public CSimpleType<ERelFromV, eDefValue>
	{
	public:

		CRelFromV() {} 

        virtual ERelFromV FromString(std::wstring &sValue)
		{
            if       ( (L"bottomMargin")  == sValue ) this->m_eValue = relfromvBottomMargin;
            else if  ( (L"insideMargin")  == sValue ) this->m_eValue = relfromvInsideMargin;
            else if  ( (L"line")          == sValue ) this->m_eValue = relfromvLine;
            else if  ( (L"margin")        == sValue ) this->m_eValue = relfromvMargin;
            else if  ( (L"outsideMargin") == sValue ) this->m_eValue = relfromvOutsideMargin;
            else if  ( (L"page")          == sValue ) this->m_eValue = relfromvPage;
            else if  ( (L"paragraph")     == sValue ) this->m_eValue = relfromvParagraph;
            else if  ( (L"topMargin")     == sValue ) this->m_eValue = relfromvTopMargin;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            switch(this->m_eValue)
			{
			case relfromvBottomMargin  : return (L"bottomMargin");
			case relfromvInsideMargin  : return (L"insideMargin");
			case relfromvLine          : return (L"line");
			case relfromvMargin        : return (L"margin");
			case relfromvOutsideMargin : return (L"outsideMargin");
			case relfromvPage          : return (L"page");
			case relfromvParagraph     : return (L"paragraph");
			case relfromvTopMargin     : return (L"topMargin");
			default                    : return (L"page");
			}
		}

		SimpleType_FromString     (ERelFromV)
		SimpleType_Operator_Equal (CRelFromV)
	};

	enum ESizeRelFromH
	{
		sizerelfromhMargin     = 0,
		sizerelfromhPage        = 1,
		sizerelfromhLeftMargin    = 2,
		sizerelfromhRightMargin   = 3,
		sizerelfromhInsideMargin  = 4,
		sizerelfromhOutsideMargin = 5
	};

	template<ESizeRelFromH eDefValue = sizerelfromhPage>
	class CSizeRelFromH : public CSimpleType<ESizeRelFromH, eDefValue>
	{
	public:

		CSizeRelFromH() {}

        virtual ESizeRelFromH FromString(std::wstring &sValue)
		{
            if       ( (L"margin")			== sValue ) this->m_eValue = sizerelfromhMargin;
            else if  ( (L"page")			== sValue ) this->m_eValue = sizerelfromhPage;
            else if  ( (L"leftMargin")		== sValue ) this->m_eValue = sizerelfromhLeftMargin;
            else if  ( (L"rightMargin")		== sValue ) this->m_eValue = sizerelfromhRightMargin;
            else if  ( (L"insideMargin")	== sValue ) this->m_eValue = sizerelfromhInsideMargin;
            else if  ( (L"outsideMargin")	== sValue ) this->m_eValue = sizerelfromhOutsideMargin;
            else										this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            switch(this->m_eValue)
			{
				case sizerelfromhMargin			: return (L"margin");
				case sizerelfromhPage			: return (L"page");
				case sizerelfromhLeftMargin		: return (L"leftMargin");
				case sizerelfromhRightMargin    : return (L"rightMargin");
				case sizerelfromhInsideMargin	: return (L"insideMargin");
				case sizerelfromhOutsideMargin	: return (L"outsideMargin");
			}
			return L"margin";
		}

		SimpleType_FromString     (ESizeRelFromH)
		SimpleType_Operator_Equal (CSizeRelFromH)
	};

	enum ESizeRelFromV
	{
		sizerelfromvMargin			= 0,
		sizerelfromvPage			= 1,
		sizerelfromvTopMargin		= 2,
		sizerelfromvBottomMargin	= 3,
		sizerelfromvInsideMargin	= 4,
		sizerelfromvOutsideMargin	= 5
	};

	template<ESizeRelFromV eDefValue = sizerelfromvPage>
	class CSizeRelFromV : public CSimpleType<ESizeRelFromV, eDefValue>
	{
	public:

		CSizeRelFromV() {}

        virtual ESizeRelFromV FromString(std::wstring &sValue)
		{
            if       ( (L"margin")			== sValue ) this->m_eValue = sizerelfromvMargin;
            else if  ( (L"page")			== sValue ) this->m_eValue = sizerelfromvPage;
            else if  ( (L"topMargin")		== sValue ) this->m_eValue = sizerelfromvTopMargin;
            else if  ( (L"bottomMargin")    == sValue ) this->m_eValue = sizerelfromvBottomMargin;
            else if  ( (L"insideMargin")	== sValue ) this->m_eValue = sizerelfromvInsideMargin;
            else if  ( (L"outsideMargin")	== sValue ) this->m_eValue = sizerelfromvOutsideMargin;
            else										this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            switch(this->m_eValue)
			{
				case sizerelfromvMargin			: return (L"margin");
				case sizerelfromvPage			: return (L"page");
				case sizerelfromvTopMargin		: return (L"topMargin");
				case sizerelfromvBottomMargin	: return (L"bottomMargin");
				case sizerelfromvInsideMargin	: return (L"insideMargin");
				case sizerelfromvOutsideMargin	: return (L"outsideMargin");
			}
			return L"margin";
		}

		SimpleType_FromString     (ESizeRelFromV)
		SimpleType_Operator_Equal (CSizeRelFromV)
	};

	//--------------------------------------------------------------------------------
	// WrapDistance 10.4.3.6 (Part 1)
	//--------------------------------------------------------------------------------

	template<__int64 nDefValue = 0>
	class CWrapDistance : public CSimpleType<__int64, nDefValue>
	{
	public:
		CWrapDistance() {}

        virtual __int64 FromString(std::wstring &sValue)
		{
            this->m_eValue = wcstoul( sValue.c_str(), NULL, 10 );

            return this->m_eValue;
		}

        virtual std::wstring ToString  () const
		{
            return std::to_wstring(this->m_eValue);
		}


		SimpleType_FromString     (__int64)
		SimpleType_Operator_Equal (CWrapDistance)

		double ToPoints()
		{
            return Emu_To_Pt( this->m_eValue );
		}
		double ToMM() const 
		{
            return Emu_To_Mm( (double)this->m_eValue );
		}

		double ToInches()
		{
            return Emu_To_Inch( this->m_eValue );
		}
		double ToTwips()
		{
            return Emu_To_Twips( (double) this->m_eValue );
		}
	};


	//--------------------------------------------------------------------------------
	// WrapText 20.4.3.7 (Part 1)
	//--------------------------------------------------------------------------------

	enum EWrapText
	{
		wraptextBothSides = 0,
		wraptextLargest   = 1,
		wraptextLeft      = 2,
		wraptextRight     = 3
	};

	template<EWrapText eDefValue = wraptextLeft>
	class CWrapText : public CSimpleType<EWrapText, eDefValue>
	{
	public:

		CWrapText() {} 

        virtual EWrapText FromString(std::wstring &sValue)
		{
            if       ( (L"bothSides") == sValue ) this->m_eValue = wraptextBothSides;
            else if  ( (L"largest")   == sValue ) this->m_eValue = wraptextLargest;
            else if  ( (L"left")      == sValue ) this->m_eValue = wraptextLeft;
            else if  ( (L"right")     == sValue ) this->m_eValue = wraptextRight;
            else								  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

        virtual std::wstring   ToString() const
		{
            switch(this->m_eValue)
			{
			case wraptextBothSides : return (L"bothSides");
			case wraptextLargest   : return (L"largest");
			case wraptextLeft      : return (L"left");
			case wraptextRight     : return (L"right");
			default                : return (L"left");
			}
		}

		SimpleType_FromString     (EWrapText)
		SimpleType_Operator_Equal (CWrapText)
	};


} // SimpleTypes

// Здесь представлены все простые типы Drawing-Spreadsheet из спецификации Office Open Xml (20.5.3)
namespace SimpleTypes
{

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Charts из спецификации Office Open Xml (21.2.3, 21.3.3)
namespace SimpleTypes
{

} // SimpleTypes

// Здесь представлены все простые типы Drawing-Diagrams из спецификации Office Open Xml (21.4.7)
namespace SimpleTypes
{

} // SimpleTypes


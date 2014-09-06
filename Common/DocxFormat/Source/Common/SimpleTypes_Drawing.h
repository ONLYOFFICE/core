#pragma once

#include "SimpleTypes_Base.h"
#include <stdlib.h>
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

		virtual EAdjAngle FromString(CString &sValue)
		{
			m_nAngle = 0;
			m_sGuide.Empty();

			Parse( sValue );

            return this->m_eValue;
		}

		virtual CString   ToString() const 
		{
            if ( adjangleAngle == this->m_eValue )
			{
				CString sResult;
				sResult.Format( _T("%d"), m_nAngle );
				return sResult;
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

		CString GetGuide() const
		{
			return m_sGuide;
		}

	private:

		void Parse(CString &sValue)
		{
			bool bAngleValue = true;

			for ( int nIndex = 0; nIndex < sValue.GetLength(); nIndex++ )
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
				m_nAngle = _wtoi( sValue.GetString() );
			}
			else
			{
                this->m_eValue = adjangleGuide;
				m_sGuide = sValue;
			}
		}

	private:

		int     m_nAngle;
		CString m_sGuide;
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

		virtual double  FromString(CString &sValue)
		{
			m_sGuide.Empty();

			Parse2( sValue );

			return m_dValue;
		}

		virtual CString ToString() const 
		{
            if ( adjcoordinateCoord == this->m_eValue )
			{
				if ( m_bUnit )
				{
					CString sResult;
					sResult.Format( _T("%.2fpt"), m_dValue );
					return sResult;
				}
				else
				{
					CString sResult;
					sResult.Format( _T("%d"), (int)Pt_To_Emu( m_dValue ) );
					return sResult;
				}
			}
			else
			{
				return m_sGuide;
			}
		}

		SimpleType_FromString     (double)
		SimpleType_Operator_Equal (CAdjCoordinate)

		CString GetGuide() const
		{
			return m_sGuide;
		}
			
		double GetValue() const
		{
			return m_dValue;
		}
	private:

		void Parse2(CString &sValue)
		{
			Parse( sValue, 12700 );

			bool bGuide = false;
			if ( !m_bUnit )
			{
				for ( int nIndex = 0; nIndex < sValue.GetLength(); nIndex++ )
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
				for ( int nIndex = 0; nIndex < sValue.GetLength() - 2; nIndex++ )
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

        EAdjCoordinate m_eValue;
		CString        m_sGuide;
	};




	//--------------------------------------------------------------------------------
	// Angle 20.1.10.3 (Part 1)
	//--------------------------------------------------------------------------------

	template<int nDefValue = 0>
	class CAngle : public CSimpleType<int, nDefValue>
	{
	public:

		CAngle() {}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue.GetString() );

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue );
			return sResult;
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

		virtual EAnimationBuildType FromString(CString &sValue)
		{
            if      ( _T("allAtOnce") == sValue ) this->m_eValue = animationbuildtypeAllAtOnce;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString             ToString  () const 
		{
            switch(this->m_eValue)
			{
			case animationbuildtypeAllAtOnce : return _T("allAtOnce");
			default                          : return _T("allAtOnce");
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

		virtual EAnimationChartBuildType FromString(CString &sValue)
		{
            if      ( _T("allAtOnce")  == sValue ) this->m_eValue = animationchartbuildtypeAllAtOnce;
            else if ( _T("category")   == sValue ) this->m_eValue = animationchartbuildtypeCategory;
            else if ( _T("categoryEl") == sValue ) this->m_eValue = animationchartbuildtypeCategoryEl;
            else if ( _T("series")     == sValue ) this->m_eValue = animationchartbuildtypeSeries;
            else if ( _T("seriesEl")   == sValue ) this->m_eValue = animationchartbuildtypeSeriesEl;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString                  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case animationchartbuildtypeAllAtOnce  : return _T("allAtOnce");
			case animationchartbuildtypeCategory   : return _T("category");
			case animationchartbuildtypeCategoryEl : return _T("categoryEl");
			case animationchartbuildtypeSeries     : return _T("series");
			case animationchartbuildtypeSeriesEl   : return _T("seriesEl");
			default                                : return _T("allAtOnce");
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

		virtual EAnimationChartOnlyBuildType FromString(CString &sValue)
		{
            if      ( _T("category")   == sValue ) this->m_eValue = animationchartonlybuildtypeCategory;
            else if ( _T("categoryEl") == sValue ) this->m_eValue = animationchartonlybuildtypeCategoryEl;
            else if ( _T("series")     == sValue ) this->m_eValue = animationchartonlybuildtypeSeries;
            else if ( _T("seriesEl")   == sValue ) this->m_eValue = animationchartonlybuildtypeSeriesEl;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString                      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case animationchartonlybuildtypeCategory   : return _T("category");
			case animationchartonlybuildtypeCategoryEl : return _T("categoryEl");
			case animationchartonlybuildtypeSeries     : return _T("series");
			case animationchartonlybuildtypeSeriesEl   : return _T("seriesEl");
			default                                    : return _T("category");
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

		virtual EAnimationDgmBuildType FromString(CString &sValue)
		{
            if      ( _T("allAtOnce") == sValue ) this->m_eValue = animationdgmbuildtypeAllAtOnce;
            else if ( _T("lvlAtOnce") == sValue ) this->m_eValue = animationdgmbuildtypeLvlAtOnce;
            else if ( _T("lvlOne")    == sValue ) this->m_eValue = animationdgmbuildtypeLvlOne;
            else if ( _T("one")       == sValue ) this->m_eValue = animationdgmbuildtypeOne;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString                ToString  () const 
		{
            switch(this->m_eValue)
			{
			case animationdgmbuildtypeAllAtOnce : return _T("allAtOnce");
			case animationdgmbuildtypeLvlAtOnce : return _T("lvlAtOnce");
			case animationdgmbuildtypeLvlOne    : return _T("lvlOne");
			case animationdgmbuildtypeOne       : return _T("one");
			default                             : return _T("allAtOnce");
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

		virtual EAnimationDgmOnlyBuildType FromString(CString &sValue)
		{
            if      ( _T("lvlAtOnce") == sValue ) this->m_eValue = animationdgmonlybuildtypeLvlAtOnce;
            else if ( _T("lvlOne")    == sValue ) this->m_eValue = animationdgmonlybuildtypeLvlOne;
            else if ( _T("one")       == sValue ) this->m_eValue = animationdgmonlybuildtypeOne;
            else                                  this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString                    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case animationdgmonlybuildtypeLvlAtOnce : return _T("lvlAtOnce");
			case animationdgmonlybuildtypeLvlOne    : return _T("lvlOne");
			case animationdgmonlybuildtypeOne       : return _T("one");
			default                                 : return _T("lvlAtOnce");
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

		virtual EBevelPresetType FromString(CString &sValue)
		{
            if      ( _T("angle")        == sValue ) this->m_eValue = bevelpresettypeAngle;
            else if ( _T("artDeco")      == sValue ) this->m_eValue = bevelpresettypeArtDeco;
            else if ( _T("circle")       == sValue ) this->m_eValue = bevelpresettypeCircle;
            else if ( _T("convex")       == sValue ) this->m_eValue = bevelpresettypeConvex;
            else if ( _T("coolSlant")    == sValue ) this->m_eValue = bevelpresettypeCoolSlant;
            else if ( _T("cross")        == sValue ) this->m_eValue = bevelpresettypeCross;
            else if ( _T("divot")        == sValue ) this->m_eValue = bevelpresettypeDivot;
            else if ( _T("hardEdge")     == sValue ) this->m_eValue = bevelpresettypeHardEdge;
            else if ( _T("relaxedInset") == sValue ) this->m_eValue = bevelpresettypeRelaxedInset;
            else if ( _T("riblet")       == sValue ) this->m_eValue = bevelpresettypeRiblet;
            else if ( _T("slope")        == sValue ) this->m_eValue = bevelpresettypeSlope;
            else if ( _T("softRound")    == sValue ) this->m_eValue = bevelpresettypeSoftRound;
            else                                     this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case bevelpresettypeAngle        : return _T("angle");
			case bevelpresettypeArtDeco      : return _T("artDeco");
			case bevelpresettypeCircle       : return _T("circle");
			case bevelpresettypeConvex       : return _T("convex");
			case bevelpresettypeCoolSlant    : return _T("coolSlant");
			case bevelpresettypeCross        : return _T("cross");
			case bevelpresettypeDivot        : return _T("divot");
			case bevelpresettypeHardEdge     : return _T("hardEdge");
			case bevelpresettypeRelaxedInset : return _T("relaxedInset");
			case bevelpresettypeRiblet       : return _T("riblet");
			case bevelpresettypeSlope        : return _T("slope");
			case bevelpresettypeSoftRound    : return _T("softRound");
			default                          : return _T("angle");
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

		virtual EBlackWhiteMode FromString(CString &sValue)
		{
            if      ( _T("auto")       == sValue ) this->m_eValue = blackwhitemodeAuto;
            else if ( _T("black")      == sValue ) this->m_eValue = blackwhitemodeBlack;
            else if ( _T("blackGray")  == sValue ) this->m_eValue = blackwhitemodeBlackGray;
            else if ( _T("blackWhite") == sValue ) this->m_eValue = blackwhitemodeBlackWhite;
            else if ( _T("clr")        == sValue ) this->m_eValue = blackwhitemodeClr;
            else if ( _T("gray")       == sValue ) this->m_eValue = blackwhitemodeGray;
            else if ( _T("grayWhite")  == sValue ) this->m_eValue = blackwhitemodeGrayWhite;
            else if ( _T("hidden")     == sValue ) this->m_eValue = blackwhitemodeHidden;
            else if ( _T("invGray")    == sValue ) this->m_eValue = blackwhitemodeInvGray;
            else if ( _T("ltGray")     == sValue ) this->m_eValue = blackwhitemodeLtGray;
            else if ( _T("white")      == sValue ) this->m_eValue = blackwhitemodeWhite;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case blackwhitemodeAuto       : return _T("auto");
			case blackwhitemodeBlack      : return _T("black");
			case blackwhitemodeBlackGray  : return _T("blackGray");
			case blackwhitemodeBlackWhite : return _T("blackWhite");
			case blackwhitemodeClr        : return _T("clr");
			case blackwhitemodeGray       : return _T("gray");
			case blackwhitemodeGrayWhite  : return _T("grayWhite");
			case blackwhitemodeHidden     : return _T("hidden");
			case blackwhitemodeInvGray    : return _T("invGray");
			case blackwhitemodeLtGray     : return _T("ltGray");
			case blackwhitemodeWhite      : return _T("white");
			default                       : return _T("auto");
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

		virtual EBlendMode FromString(CString &sValue)
		{
            if      ( _T("darken")  == sValue ) this->m_eValue = blendmodeDarken;
            else if ( _T("lighten") == sValue ) this->m_eValue = blendmodeLighten;
            else if ( _T("mult")    == sValue ) this->m_eValue = blendmodeMult;
            else if ( _T("over")    == sValue ) this->m_eValue = blendmodeOver;
            else if ( _T("screen")  == sValue ) this->m_eValue = blendmodeScreen;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case blendmodeDarken  : return _T("darken");
			case blendmodeLighten : return _T("lighten");
			case blendmodeMult    : return _T("mult");
			case blendmodeOver    : return _T("over");
			case blendmodeScreen  : return _T("screen");
			default               : return _T("mult");
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

		virtual EBlipCompression FromString(CString &sValue)
		{
            if      ( _T("email")   == sValue ) this->m_eValue = blipcompressionEmail;
            else if ( _T("hqprint") == sValue ) this->m_eValue = blipcompressionHQPrint;
            else if ( _T("none")    == sValue ) this->m_eValue = blipcompressionNone;
            else if ( _T("print")   == sValue ) this->m_eValue = blipcompressionPrint;
            else if ( _T("screen")  == sValue ) this->m_eValue = blipcompressionScreen;
            else                                this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case blipcompressionEmail   : return _T("email");
			case blipcompressionHQPrint : return _T("hqprint");
			case blipcompressionNone    : return _T("none");
			case blipcompressionPrint   : return _T("print");
			case blipcompressionScreen  : return _T("screen");
			default                     : return _T("none");
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

		virtual EColorSchemeIndex FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':
                if      ( _T("accent1")  == sValue ) this->m_eValue = colorschemeindexAccent1;
                else if ( _T("accent2")  == sValue ) this->m_eValue = colorschemeindexAccent2;
                else if ( _T("accent3")  == sValue ) this->m_eValue = colorschemeindexAccent3;
                else if ( _T("accent4")  == sValue ) this->m_eValue = colorschemeindexAccent4;
                else if ( _T("accent5")  == sValue ) this->m_eValue = colorschemeindexAccent5;
                else if ( _T("accent6")  == sValue ) this->m_eValue = colorschemeindexAccent6;
				break;
			case 'd':
                if      ( _T("dk1")      == sValue ) this->m_eValue = colorschemeindexDk1;
                else if ( _T("dk2")      == sValue ) this->m_eValue = colorschemeindexDk2;
				break;
			case 'f':
                if      ( _T("folHlink") == sValue ) this->m_eValue = colorschemeindexFolHlink;
				break;
			case 'h':
                if      ( _T("hlink")    == sValue ) this->m_eValue = colorschemeindexHlink;
				break;
			case 'l':
                if      ( _T("lt1")      == sValue ) this->m_eValue = colorschemeindexLt1;
                else if ( _T("lt2")      == sValue ) this->m_eValue = colorschemeindexLt2;
				break;
			}

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case colorschemeindexAccent1 : return _T("accent1");			
			case colorschemeindexAccent2 : return _T("accent2");			
			case colorschemeindexAccent3 : return _T("accent3");			
			case colorschemeindexAccent4 : return _T("accent4");			
			case colorschemeindexAccent5 : return _T("accent5");			
			case colorschemeindexAccent6 : return _T("accent6");			
			case colorschemeindexDk1     : return _T("dk1");			
			case colorschemeindexDk2     : return _T("dk2");			
			case colorschemeindexFolHlink: return _T("folHlink");			
			case colorschemeindexHlink   : return _T("hlink");			
			case colorschemeindexLt1     : return _T("lt1");			
			case colorschemeindexLt2     : return _T("lt2");			
			default                      : return _T("accent1");
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

		virtual ECompoundLine FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'd':
                if      ( _T("dbl")       == sValue ) this->m_eValue = compoundlineDbl;
				break;
			case 's':
                if      ( _T("sng")       == sValue ) this->m_eValue = compoundlineSng;
				break;
			case 't':
                if      ( _T("thickThin") == sValue ) this->m_eValue = compoundlineThickThin;
                else if ( _T("thinThick") == sValue ) this->m_eValue = compoundlineThinThick;
                else if ( _T("tri")       == sValue ) this->m_eValue = compoundlineTri;
				break;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case compoundlineDbl:       return _T("dbl");			
			case compoundlineSng:       return _T("sng");			
			case compoundlineThickThin: return _T("thickThin");			
			case compoundlineThinThick: return _T("thinThick");			
			case compoundlineTri:       return _T("tri");			
			default :                   return _T("sng");
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

		virtual double  FromString(CString &sValue)
        {
            Parse(sValue, 12700);

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;

            if ( m_bUnit )
                sResult.Format( _T("%fpt"), m_dValue);
            else
                sResult.Format( _T("%d"), (__int64)m_dValue );

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

        virtual int     FromString(CString &sValue)
        {
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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

		virtual EEffectContainerType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 's':
                if      ( _T("sib")  == sValue ) this->m_eValue = effectcontainertypeSib;
				break;
			case 't':
                if      ( _T("tree") == sValue ) this->m_eValue = effectcontainertypeTree;
				break;
			}

            return this->m_eValue;
		}

		virtual CString              ToString  () const 
		{
            switch(this->m_eValue)
			{
			case effectcontainertypeSib:  return _T("sib");			
			case effectcontainertypeTree: return _T("tree");		
			default :                     return _T("sib");
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
            this->m_eValue = min( 5400000, max( -5400000, nValue ) );
		}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = min( 5400000, max( -5400000, _wtoi( sValue.GetString() ) ) );

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue );
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
			m_dValue = min( 100, max( -100, dValue ) );
		}

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					// Поправка 12.1.2.1 Part4
					int nValue = min( 100000, max( -100000, _wtoi( sValue ) ) );
					m_dValue = nValue / 1000.0;
				}
				else
					m_dValue = 0;
			}
			else
				m_dValue = min( 100, max( -100, _wtof( sValue.Mid( 0, nLen - 1 ) ) ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

            return sResult;
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

		virtual EFontCollectionIndex FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'm':
                if      ( _T("major") == sValue ) this->m_eValue = fontcollectionindexMajor;
                else if ( _T("minor") == sValue ) this->m_eValue = fontcollectionindexMinor;
				break;
			case 'n':
                if      ( _T("none")  == sValue ) this->m_eValue = fontcollectionindexNone;
				break;
			}

            return this->m_eValue;
		}

		virtual CString              ToString  () const 
		{
            switch(this->m_eValue)
			{
			case fontcollectionindexMajor: return _T("major");			
			case fontcollectionindexMinor: return _T("minor");		
			case fontcollectionindexNone : return _T("none");		
			default                      : return _T("none");
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
            this->m_eValue = min( 10800000, max( 0, nValue ) );
		}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = min( 10800000, max( 0, _wtoi( sValue.GetString() ) ) );

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue );
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

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			m_sValue = sValue;
		}


		CString FromString(CString &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CGeomGuideFormula)

	private:

		void Parse()
		{
			// TO DO: Сделать разбор формул. См. стр.3244 Part1.
		}

	private:

		CString m_sValue;
	};
	//--------------------------------------------------------------------------------
	// GeomGuideName 20.1.10.28 (Part 1)
	//--------------------------------------------------------------------------------		
	class CGeomGuideName
	{
	public:
		CGeomGuideName() {}

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			m_sValue = sValue;
		}


		CString FromString(CString &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CGeomGuideName)

	private:

		CString m_sValue;
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

		virtual ELightRigDirection FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'b':
                if      ( _T("b")  == sValue ) this->m_eValue = lightrigdirectionB;
                else if ( _T("bl") == sValue ) this->m_eValue = lightrigdirectionBL;
                else if ( _T("br") == sValue ) this->m_eValue = lightrigdirectionBR;
				break;
			case 'l':
                if      ( _T("l")  == sValue ) this->m_eValue = lightrigdirectionL;
				break;
			case 'r':
                if      ( _T("r")  == sValue ) this->m_eValue = lightrigdirectionR;
				break;
			case 't':
                if      ( _T("t")  == sValue ) this->m_eValue = lightrigdirectionT;
                else if ( _T("tl") == sValue ) this->m_eValue = lightrigdirectionTL;
                else if ( _T("tr") == sValue ) this->m_eValue = lightrigdirectionTR;
				break;
			}

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lightrigdirectionB:  return _T("b");			
			case lightrigdirectionBL: return _T("bl");			
			case lightrigdirectionBR: return _T("br");		
			case lightrigdirectionL:  return _T("l");			
			case lightrigdirectionR:  return _T("r");			
			case lightrigdirectionT:  return _T("t");		
			case lightrigdirectionTL: return _T("tl");			
			case lightrigdirectionTR: return _T("tr");			
			default :                 return _T("tr");
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

		virtual ELightRigType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'b':
                if      ( _T("balanced")    == sValue ) this->m_eValue = lightrigtypeBalanced;
                else if ( _T("brightRoom")  == sValue ) this->m_eValue = lightrigtypeBrightRoom;
				break;
			case 'c':
                if      ( _T("chilly")      == sValue ) this->m_eValue = lightrigtypeChilly;
                else if ( _T("contrasting") == sValue ) this->m_eValue = lightrigtypeContrasting;
				break;
			case 'f':
                if      ( _T("flat")        == sValue ) this->m_eValue = lightrigtypeFlat;
                else if ( _T("flood")       == sValue ) this->m_eValue = lightrigtypeFlood;
                else if ( _T("freezing")    == sValue ) this->m_eValue = lightrigtypeFreezing;
				break;
			case 'g':
                if      ( _T("glow")        == sValue ) this->m_eValue = lightrigtypeGlow;
				break;
			case 'h':
                if      ( _T("harsh")       == sValue ) this->m_eValue = lightrigtypeHarsh;
				break;
			case 'l':
                if      ( _T("legacyFlat1")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat1;
                else if ( _T("legacyFlat2")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat2;
                else if ( _T("legacyFlat3")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat3;
                else if ( _T("legacyFlat4")   == sValue ) this->m_eValue = lightrigtypeLegacyFlat4;
                else if ( _T("legacyHarsh1")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh1;
                else if ( _T("legacyHarsh2")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh2;
                else if ( _T("legacyHarsh3")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh3;
                else if ( _T("legacyHarsh4")  == sValue ) this->m_eValue = lightrigtypeLegacyHarsh4;
                else if ( _T("legacyNormal1") == sValue ) this->m_eValue = lightrigtypeLegacyNormal1;
                else if ( _T("legacyNormal2") == sValue ) this->m_eValue = lightrigtypeLegacyNormal2;
                else if ( _T("legacyNormal3") == sValue ) this->m_eValue = lightrigtypeLegacyNormal3;
                else if ( _T("legacyNormal4") == sValue ) this->m_eValue = lightrigtypeLegacyNormal4;
				break;
			case 'm':
                if      ( _T("morning") == sValue ) this->m_eValue = lightrigtypeMorning;
				break;
			case 's':
                if      ( _T("soft")    == sValue ) this->m_eValue = lightrigtypeSoft;
                else if ( _T("sunrise") == sValue ) this->m_eValue = lightrigtypeSunrise;
                else if ( _T("sunset")  == sValue ) this->m_eValue = lightrigtypeSunset;
				break;
			case 't':
                if      ( _T("threePt") == sValue ) this->m_eValue = lightrigtypeThreePt;
                else if ( _T("twoPt")   == sValue ) this->m_eValue = lightrigtypeTwoPt;
				break;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lightrigtypeBalanced      : return _T("balanced");
			case lightrigtypeBrightRoom    : return _T("brightRoom");
			case lightrigtypeChilly        : return _T("chilly");
			case lightrigtypeContrasting   : return _T("contrasting");
			case lightrigtypeFlat          : return _T("flat"); 
			case lightrigtypeFlood         : return _T("flood");
			case lightrigtypeFreezing      : return _T("freezing");
			case lightrigtypeGlow          : return _T("glow");
			case lightrigtypeHarsh         : return _T("harsh");
			case lightrigtypeLegacyFlat1   : return _T("legacyFlat1");
			case lightrigtypeLegacyFlat2   : return _T("legacyFlat2");
			case lightrigtypeLegacyFlat3   : return _T("legacyFlat3"); 
			case lightrigtypeLegacyFlat4   : return _T("legacyFlat4");
			case lightrigtypeLegacyHarsh1  : return _T("legacyHarsh1");
			case lightrigtypeLegacyHarsh2  : return _T("legacyHarsh2");
			case lightrigtypeLegacyHarsh3  : return _T("legacyHarsh3");
			case lightrigtypeLegacyHarsh4  : return _T("legacyHarsh4");
			case lightrigtypeLegacyNormal1 : return _T("legacyNormal1");
			case lightrigtypeLegacyNormal2 : return _T("legacyNormal2"); 
			case lightrigtypeLegacyNormal3 : return _T("legacyNormal3");
			case lightrigtypeLegacyNormal4 : return _T("legacyNormal4"); 
			case lightrigtypeMorning       : return _T("morning");
			case lightrigtypeSoft          : return _T("soft"); 
			case lightrigtypeSunrise       : return _T("sunrise"); 
			case lightrigtypeSunset        : return _T("sunset");
			case lightrigtypeThreePt       : return _T("threePt"); 
			case lightrigtypeTwoPt         : return _T("twoPt");			
			default                        : return _T("balanced");
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

		virtual ELineCap FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'f':
                if      ( _T("flat") == sValue ) this->m_eValue = linecapFlat;
				break;
			case 'r':
                if      ( _T("rnd")  == sValue ) this->m_eValue = linecapRnd;
				break;
			case 's':
                if      ( _T("sq")   == sValue ) this->m_eValue = linecapSq;
				break;
			}

            return this->m_eValue;
		}

		virtual CString  ToString  () const 
		{
            switch(this->m_eValue)
			{
			case linecapFlat: return _T("flat");			
			case linecapRnd:  return _T("rnd");			
			case linecapSq:   return _T("sq");			
			default :         return _T("rnd");
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

		virtual ELineEndLength FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'l':
                if      ( _T("lg")  == sValue ) this->m_eValue = lineendlengthLarge;
				break;
			case 'm':
                if      ( _T("med") == sValue ) this->m_eValue = lineendlengthMedium;
				break;
			case 's':
                if      ( _T("sm")  == sValue ) this->m_eValue = lineendlengthSmall;
				break;
			}

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lineendlengthLarge:  return _T("lg");			
			case lineendlengthMedium: return _T("med");			
			case lineendlengthSmall:  return _T("sm");		
			default :                 return _T("med");
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

		virtual ELineEndType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':
                if      ( _T("arrow")    == sValue ) this->m_eValue = lineendtypeArrow;
				break;
			case 'd':
                if      ( _T("diamond")  == sValue ) this->m_eValue = lineendtypeDiamond;
				break;
			case 'n':
                if      ( _T("none")     == sValue ) this->m_eValue = lineendtypeNone;
				break;
			case 'o':
                if      ( _T("oval")     == sValue ) this->m_eValue = lineendtypeOval;
				break;
			case 's':
                if      ( _T("stealth")  == sValue ) this->m_eValue = lineendtypeStealth;
				break;
			case 't':
                if      ( _T("triangle") == sValue ) this->m_eValue = lineendtypeTriangle;
				break;
			}

            return this->m_eValue;
		}

		virtual CString      ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lineendtypeArrow:    return _T("arrow");			
			case lineendtypeDiamond:  return _T("diamond");			
			case lineendtypeNone:     return _T("none");		
			case lineendtypeOval:     return _T("oval");			
			case lineendtypeStealth:  return _T("stealth");			
			case lineendtypeTriangle: return _T("triangle");		
			default :                 return _T("none");
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

		virtual ELineEndWidth FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'l':
                if      ( _T("lg")  == sValue ) this->m_eValue = lineendwidthLarge;
				break;
			case 'm':
                if      ( _T("med") == sValue ) this->m_eValue = lineendwidthMedium;
				break;
			case 's':
                if      ( _T("sm")  == sValue ) this->m_eValue = lineendwidthSmall;
				break;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case lineendwidthLarge:  return _T("lg");			
			case lineendwidthMedium: return _T("med");			
			case lineendwidthSmall:  return _T("sm");		
			default :                return _T("med");
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

        virtual __int64 FromString(CString &sValue)
        {
            this->m_eValue = _wtoi64( sValue );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 20116800)
                this->m_eValue = 20116800;

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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

		virtual EPathFillMode FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'd':
                if      ( _T("darken")     == sValue ) this->m_eValue = pathfillmodeDarken;
                else if ( _T("darkenLess") == sValue ) this->m_eValue = pathfillmodeDarkenLess;
				break;
			case 'l':
                if      ( _T("lighten")    == sValue ) this->m_eValue = pathfillmodeLighten;
                else if ( _T("darkenLess") == sValue ) this->m_eValue = pathfillmodeLightenLess;
				break;
			case 'n':
                if      ( _T("none")       == sValue ) this->m_eValue = pathfillmodeNone;
                else if ( _T("norm")       == sValue ) this->m_eValue = pathfillmodeNorm;
				break;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pathfillmodeDarken:      return _T("darken");			
			case pathfillmodeDarkenLess:  return _T("darkenLess");			
			case pathfillmodeLighten:     return _T("lighten");		
			case pathfillmodeLightenLess: return _T("lightenLess");			
			case pathfillmodeNone:        return _T("none");			
			case pathfillmodeNorm:        return _T("norm");		
			default :                     return _T("none");
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

		virtual EPathShadeType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("circle") == sValue ) this->m_eValue = pathshadetypeCircle;
				break;
			case 'r':
                if      ( _T("rect")   == sValue ) this->m_eValue = pathshadetypeRect;
				break;
			case 's':
                if      ( _T("shape")  == sValue ) this->m_eValue = pathshadetypeShape;
				break;
			}

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case pathshadetypeCircle: return _T("circle");			
			case pathshadetypeRect:   return _T("rect");			
			case pathshadetypeShape:  return _T("shape");		
			default :                 return _T("rect");
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

		virtual EPenAlignment FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("ctr") == sValue ) this->m_eValue = penalignmentCtr;
				break;
			case 'i':
                if      ( _T("in")  == sValue ) this->m_eValue = penalignmentIn;
				break;
			}

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case penalignmentCtr: return _T("ctr");			
			case penalignmentIn:  return _T("in");			
			default :             return _T("ctr");
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

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					//test
					double dValue = _wtof(sValue);
					if (dValue >= 0 && dValue <=1 )
					{
						m_dValue = dValue  ;
					}
					else
					{
						// Поправка 12.1.2.2 (Part4)
						int nValue = _wtoi( sValue );
						m_dValue = nValue / 1000.0;
					}
				}
				else
					m_dValue = 0;
			}
			else
				m_dValue = _wtof( sValue.Mid( 0, nLen - 1 ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

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

        virtual __int64 FromString(CString &sValue)
        {
            this->m_eValue = _wtoi64( sValue );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 27273042316900)
                this->m_eValue = 27273042316900;

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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
            return Emu_To_Mm( this->m_eValue );
        }

        double ToInches()
        {
            return Emu_To_Inch( this->m_eValue );
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
            this->m_eValue = min( 21600000, max( 0, nValue ) );
		}

		virtual int     FromString(CString &sValue)
		{
            this->m_eValue = min( 21600000, max( 0, _wtoi( sValue.GetString() ) ) );

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue );
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
			m_dValue = min( 100, max( 0, dValue ) );
		}

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					// Поправка 12.1.2.3 (Part4)
					int nValue = max( 0, min( 100000, _wtoi( sValue ) ) );
					m_dValue = nValue / 1000.0;
				}
				else
					m_dValue = 0;
			}
			else
				m_dValue = min( 100, max( 0, _wtof( sValue.Mid( 0, nLen - 1 ) ) ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

            return sResult;
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
			m_dValue = max( 0, dValue );
		}

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					// Поправка 12.1.2.4 (Part4)
					int nValue = max( 0, _wtoi( sValue ) );
					m_dValue = nValue / 1000.0;
				}
				return
					m_dValue;
			}
			else
				m_dValue = max( 0, _wtof( sValue.Mid( 0, nLen - 1 ) ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

            return sResult;
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

		virtual EPresetCameraType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'i':
                if      ( _T("isometricBottomDown")					== sValue ) this->m_eValue = presetcameratypeIsometricBottomDown;
                else if ( _T("isometricBottomUp")					== sValue ) this->m_eValue = presetcameratypeIsometricBottomUp;
                else if ( _T("isometricLeftDown")					== sValue ) this->m_eValue = presetcameratypeIsometricLeftDown;
                else if ( _T("isometricLeftUp")						== sValue ) this->m_eValue = presetcameratypeIsometricLeftUp;
                else if ( _T("isometricOffAxis1Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Left;
                else if ( _T("isometricOffAxis1Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Right;
                else if ( _T("isometricOffAxis1Top")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis1Top;
                else if ( _T("isometricOffAxis2Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Left;
                else if ( _T("isometricOffAxis2Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Right;
                else if ( _T("isometricOffAxis2Top")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis2Top;
                else if ( _T("isometricOffAxis3Bottom")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Bottom ;
                else if ( _T("isometricOffAxis3Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Left;
                else if ( _T("isometricOffAxis3Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis3Right;
                else if ( _T("isometricOffAxis4Bottom")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Bottom;
                else if ( _T("isometricOffAxis4Left")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Left;
                else if ( _T("isometricOffAxis4Right")				== sValue ) this->m_eValue = presetcameratypeIsometricOffAxis4Right;
                else if ( _T("isometricRightDown")					== sValue ) this->m_eValue = presetcameratypeIsometricRightDown;
                else if ( _T("isometricRightUp")					== sValue ) this->m_eValue = presetcameratypeIsometricRightUp;
                else if ( _T("isometricTopDown")					== sValue ) this->m_eValue = presetcameratypeIsometricTopDown;
                else if ( _T("isometricTopUp")						== sValue ) this->m_eValue = presetcameratypeIsometricTopUp;
				break;
			case 'l':
                if      ( _T("legacyObliqueBottom")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottom;
                else if ( _T("legacyObliqueBottomLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottomLeft;
                else if ( _T("legacyObliqueBottomRight")			== sValue ) this->m_eValue = presetcameratypeLegacyObliqueBottomRight;
                else if ( _T("legacyObliqueFront")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueFront;
                else if ( _T("legacyObliqueLeft")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueLeft;
                else if ( _T("legacyObliqueRight")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueRight;
                else if ( _T("legacyObliqueTop")					== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTop;
                else if ( _T("legacyObliqueTopLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTopLeft;
                else if ( _T("legacyObliqueTopRight")				== sValue ) this->m_eValue = presetcameratypeLegacyObliqueTopRight;
                else if ( _T("legacyPerspectiveBottom")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottom;
                else if ( _T("legacyPerspectiveBottomLeft")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottomLeft;
                else if ( _T("legacyPerspectiveBottomRight")		== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveBottomRight;
                else if ( _T("legacyPerspectiveFront")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveFront;
                else if ( _T("legacyPerspectiveLeft")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveLeft;
                else if ( _T("legacyPerspectiveRight")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveRight;
                else if ( _T("legacyPerspectiveTop")				== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTop;
                else if ( _T("legacyPerspectiveTopLeft")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTopLeft;
                else if ( _T("legacyPerspectiveTopRight")			== sValue ) this->m_eValue = presetcameratypeLegacyPerspectiveTopRight;
				break;
			case 'o':
                if      ( _T("obliqueBottom")						== sValue ) this->m_eValue = presetcameratypeObliqueBottom;
                else if ( _T("obliqueBottomLeft")					== sValue ) this->m_eValue = presetcameratypeObliqueBottomLeft;
                else if ( _T("obliqueBottomRight")					== sValue ) this->m_eValue = presetcameratypeObliqueBottomRight;
                else if ( _T("obliqueLeft")							== sValue ) this->m_eValue = presetcameratypeObliqueLeft;
                else if ( _T("obliqueRight")						== sValue ) this->m_eValue = presetcameratypeObliqueRight;
                else if ( _T("obliqueTop")							== sValue ) this->m_eValue = presetcameratypeObliqueTop;
                else if ( _T("obliqueTopLeft")						== sValue ) this->m_eValue = presetcameratypeObliqueTopLeft;
                else if ( _T("obliqueTopRight")						== sValue ) this->m_eValue = presetcameratypeObliqueTopRight;
                else if ( _T("orthographicFront")					== sValue ) this->m_eValue = presetcameratypeOrthographicFront;
				break;
			case 'p':
                if      ( _T("perspectiveAbove")					== sValue ) this->m_eValue = presetcameratypePerspectiveAbove;
                else if ( _T("perspectiveAboveLeftFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveAboveLeftFacing;
                else if ( _T("perspectiveAboveRightFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveAboveRightFacing;
                else if ( _T("perspectiveBelow")					== sValue ) this->m_eValue = presetcameratypePerspectiveBelow;
                else if ( _T("perspectiveContrastingLeftFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveContrastingLeftFacing;
                else if ( _T("perspectiveContrastingRightFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveContrastingRightFacing;
                else if ( _T("perspectiveFront")			        == sValue ) this->m_eValue = presetcameratypePerspectiveFront;
                else if ( _T("perspectiveHeroicExtremeLeftFacing")	== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicExtremeLeftFacing;
                else if ( _T("perspectiveHeroicExtremeRightFacing") == sValue ) this->m_eValue = presetcameratypePerspectiveHeroicExtremeRightFacing;
                else if ( _T("perspectiveHeroicLeftFacing")			== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicLeftFacing;
                else if ( _T("perspectiveHeroicRightFacing")		== sValue ) this->m_eValue = presetcameratypePerspectiveHeroicRightFacing;
                else if ( _T("perspectiveLeft")						== sValue ) this->m_eValue = presetcameratypePerspectiveLeft;
                else if ( _T("perspectiveRelaxed")					== sValue ) this->m_eValue = presetcameratypePerspectiveRelaxed;
                else if ( _T("perspectiveRelaxedModerately")		== sValue ) this->m_eValue = presetcameratypePerspectiveRelaxedModerately;
                else if ( _T("perspectiveRight")					== sValue ) this->m_eValue = presetcameratypePerspectiveRight;
				break;
			}

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetcameratypeIsometricBottomDown: return _T("isometricBottomDown"); // (Isometric Bottom Down) 
			case presetcameratypeIsometricBottomUp: return _T("isometricBottomUp"); // (Isometric Bottom Up) 
			case presetcameratypeIsometricLeftDown: return _T("isometricLeftDown"); // (Isometric Left Down) 
			case presetcameratypeIsometricLeftUp: return _T("isometricLeftUp"); // (Isometric Left Up) 
			case presetcameratypeIsometricOffAxis1Left: return _T("isometricOffAxis1Left"); // (Isometric Off Axis 1 Left) 
			case presetcameratypeIsometricOffAxis1Right: return _T("isometricOffAxis1Right"); // (Isometric Off Axis 1 Right) 
			case presetcameratypeIsometricOffAxis1Top: return _T("isometricOffAxis1Top"); // (Isometric Off Axis 1 Top) 
			case presetcameratypeIsometricOffAxis2Left: return _T("isometricOffAxis2Left"); // (Isometric Off Axis 2 Left) 
			case presetcameratypeIsometricOffAxis2Right: return _T("isometricOffAxis2Right"); // (Isometric Off Axis 2 Right
			case presetcameratypeIsometricOffAxis2Top: return _T("isometricOffAxis2Top"); // (Isometric Off Axis 2 Top) 
			case presetcameratypeIsometricOffAxis3Bottom: return _T("isometricOffAxis3Bottom"); // (Isometric Off Axis 3 Bottom) 
			case presetcameratypeIsometricOffAxis3Left: return _T("isometricOffAxis3Left"); // (Isometric Off Axis 3 Left) 
			case presetcameratypeIsometricOffAxis3Right: return _T("isometricOffAxis3Right"); // (Isometric Off Axis 3 Right) 
			case presetcameratypeIsometricOffAxis4Bottom: return _T("isometricOffAxis4Bottom"); // (Isometric Off Axis 4 Bottom) 
			case presetcameratypeIsometricOffAxis4Left: return _T("isometricOffAxis4Left"); // (Isometric Off Axis 4 Left) 
			case presetcameratypeIsometricOffAxis4Right: return _T("isometricOffAxis4Right"); // (Isometric Off Axis 4 Right) 
			case presetcameratypeIsometricRightDown: return _T("isometricRightDown"); // (Isometric Right Down) 
			case presetcameratypeIsometricRightUp: return _T("isometricRightUp"); // (Isometric Right Up) 
			case presetcameratypeIsometricTopDown: return _T("isometricTopDown"); // (Isometric Top Down) 
			case presetcameratypeIsometricTopUp: return _T("isometricTopUp"); // (Isometric Top Up) 
			case presetcameratypeLegacyObliqueBottom: return _T("legacyObliqueBottom"); // (Legacy Oblique Bottom) 
			case presetcameratypeLegacyObliqueBottomLeft: return _T("legacyObliqueBottomLeft"); // (Legacy Oblique Bottom Left) 
			case presetcameratypeLegacyObliqueBottomRight: return _T("legacyObliqueBottomRight"); // (Legacy Oblique Bottom Right) 
			case presetcameratypeLegacyObliqueFront: return _T("legacyObliqueFront"); // (Legacy Oblique Front) 
			case presetcameratypeLegacyObliqueLeft: return _T("legacyObliqueLeft"); // (Legacy Oblique Left) 
			case presetcameratypeLegacyObliqueRight: return _T("legacyObliqueRight"); // (Legacy Oblique Right) 
			case presetcameratypeLegacyObliqueTop: return _T("legacyObliqueTop"); // (Legacy Oblique Top) 
			case presetcameratypeLegacyObliqueTopLeft: return _T("legacyObliqueTopLeft"); // (Legacy Oblique Top Left) 
			case presetcameratypeLegacyObliqueTopRight: return _T("legacyObliqueTopRight"); // (Legacy Oblique Top Right) 
			case presetcameratypeLegacyPerspectiveBottom: return _T("legacyPerspectiveBottom"); // (Legacy Perspective Bottom) 
			case presetcameratypeLegacyPerspectiveBottomLeft: return _T("legacyPerspectiveBottomLeft"); // (Legacy Perspective Bottom Left) 
			case presetcameratypeLegacyPerspectiveBottomRight: return _T("legacyPerspectiveBottomRight"); // (Legacy Perspective Bottom Right) 
			case presetcameratypeLegacyPerspectiveFront: return _T("legacyPerspectiveFront"); // (Legacy Perspective Front) 
			case presetcameratypeLegacyPerspectiveLeft: return _T("legacyPerspectiveLeft"); // (Legacy Perspective Left) 
			case presetcameratypeLegacyPerspectiveRight: return _T("legacyPerspectiveRight"); // (Legacy Perspective Right) 
			case presetcameratypeLegacyPerspectiveTop: return _T("legacyPerspectiveTop"); // (Legacy Perspective Top) 
			case presetcameratypeLegacyPerspectiveTopLeft: return _T("legacyPerspectiveTopLeft"); // (Legacy Perspective Top Left) 
			case presetcameratypeLegacyPerspectiveTopRight: return _T("legacyPerspectiveTopRight"); // (Legacy Perspective Top Right) 
			case presetcameratypeObliqueBottom: return _T("obliqueBottom"); // (Oblique Bottom) 
			case presetcameratypeObliqueBottomLeft: return _T("obliqueBottomLeft"); // (Oblique Bottom Left) 
			case presetcameratypeObliqueBottomRight: return _T("obliqueBottomRight"); // (Oblique Bottom Right) 
			case presetcameratypeObliqueLeft: return _T("obliqueLeft"); // (Oblique Left) 
			case presetcameratypeObliqueRight: return _T("obliqueRight"); // (Oblique Right) 
			case presetcameratypeObliqueTop: return _T("obliqueTop"); // (Oblique Top) 
			case presetcameratypeObliqueTopLeft: return _T("obliqueTopLeft"); // (Oblique Top Left) 
			case presetcameratypeObliqueTopRight: return _T("obliqueTopRight"); // (Oblique Top Right) 
			case presetcameratypeOrthographicFront: return _T("orthographicFront"); // (Orthographic Front) 
			case presetcameratypePerspectiveAbove: return _T("perspectiveAbove"); // (Orthographic Above) 
			case presetcameratypePerspectiveAboveLeftFacing: return _T("perspectiveAboveLeftFacing"); // (Perspective Above Left Facing) 
			case presetcameratypePerspectiveAboveRightFacing: return _T("perspectiveAboveRightFacing"); // (Perspective Above Right Facing) 
			case presetcameratypePerspectiveBelow: return _T("perspectiveBelow"); // (Perspective Below) 
			case presetcameratypePerspectiveContrastingLeftFacing: return _T("perspectiveContrastingLeftFacing"); // (Perspective Contrasting Left Facing) 
			case presetcameratypePerspectiveContrastingRightFacing: return _T("perspectiveContrastingRightFacing"); // (Perspective Contrasting Right Facing) 
			case presetcameratypePerspectiveFront: return _T("perspectiveFront"); // (Perspective Front) 
			case presetcameratypePerspectiveHeroicExtremeLeftFacing: return _T("perspectiveHeroicExtremeLeftFacing"); // (Perspective Heroic Extreme Left Facing) 
			case presetcameratypePerspectiveHeroicExtremeRightFacing: return _T("perspectiveHeroicExtremeRightFacing"); // (Perspective Heroic Extreme Right Facing) 
			case presetcameratypePerspectiveHeroicLeftFacing: return _T("perspectiveHeroicLeftFacing"); // (Perspective Heroic Left Facing) 
			case presetcameratypePerspectiveHeroicRightFacing: return _T("perspectiveHeroicRightFacing"); // (Perspective Heroic Right Facing) 
			case presetcameratypePerspectiveLeft: return _T("perspectiveLeft"); // (Perspective Left) 
			case presetcameratypePerspectiveRelaxed: return _T("perspectiveRelaxed"); // (Perspective Relaxed) 
			case presetcameratypePerspectiveRelaxedModerately: return _T("perspectiveRelaxedModerately"); // (Perspective Relaxed Moderately) 
			case presetcameratypePerspectiveRight: return _T("perspectiveRight"); // (Perspective Right) 			
			default :								return _T("orthographicFront");
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

		virtual EPresetColorVal FromString(CString &sValue)
		{
            this->m_eValue = presetcolorvalBlack;
			SetRGBA( 0, 0, 0, 255 );

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':
                if      ( _T("aliceBlue")			== sValue ) { this->m_eValue = presetcolorvalAliceBlue; SetRGBA(240,248,255); }
                else if ( _T("antiqueWhite")		== sValue ) { this->m_eValue = presetcolorvalAntiqueWhite; SetRGBA(250,235,215); }
                else if ( _T("aqua")				== sValue ) { this->m_eValue = presetcolorvalAqua; SetRGBA(0,255,255); }
                else if ( _T("aquamarine")			== sValue ) { this->m_eValue = presetcolorvalAquamarine; SetRGBA(127,255,212); }
                else if ( _T("azure")				== sValue ) { this->m_eValue = presetcolorvalAzure; SetRGBA(240,255,255); }
				break;
			case 'b':
                if      ( _T("beige")				== sValue ) { this->m_eValue = presetcolorvalBeige; SetRGBA(245,245,220); }
                else if ( _T("bisque")				== sValue ) { this->m_eValue = presetcolorvalBisque ; SetRGBA(255,228,196); }
                else if ( _T("black")				== sValue ) { this->m_eValue = presetcolorvalBlack ; SetRGBA(0,0,0); }
                else if ( _T("blanchedAlmond")		== sValue ) { this->m_eValue = presetcolorvalBlanchedAlmond ; SetRGBA(255,235,205); }
                else if ( _T("blue")				== sValue ) { this->m_eValue = presetcolorvalBlue ; SetRGBA(0,0,255); }
                else if ( _T("blueViolet")			== sValue ) { this->m_eValue = presetcolorvalBlueViolet ; SetRGBA(138,43,226); }
                else if ( _T("brown")				== sValue ) { this->m_eValue = presetcolorvalBrown ; SetRGBA(165,42,42); }
                else if ( _T("burlyWood")			== sValue ) { this->m_eValue = presetcolorvalBurlyWood ; SetRGBA(222,184,135); }
				break;
			case 'c':
                if      ( _T("cadetBlue")			== sValue ) { this->m_eValue = presetcolorvalCadetBlue ; SetRGBA(95,158,160); }
                else if ( _T("chartreuse")			== sValue ) { this->m_eValue = presetcolorvalChartreuse ; SetRGBA(127,255,0); }
                else if ( _T("chocolate")			== sValue ) { this->m_eValue = presetcolorvalChocolate ; SetRGBA(210,105,30); }
                else if ( _T("coral")				== sValue ) { this->m_eValue = presetcolorvalCoral ; SetRGBA(255,127,80); }
                else if ( _T("cornflowerBlue")		== sValue ) { this->m_eValue = presetcolorvalCornflowerBlue ; SetRGBA(100,149,237); }
                else if ( _T("cornsilk")			== sValue ) { this->m_eValue = presetcolorvalCornsilk ; SetRGBA(255,248,220); }
                else if ( _T("crimson")				== sValue ) { this->m_eValue = presetcolorvalCrimson ; SetRGBA(220,20,60); }
                else if ( _T("cyan")				== sValue ) { this->m_eValue = presetcolorvalCyan ; SetRGBA(0,255,255); }
				break;
			case 'd':
                if      ( _T("darkBlue")			== sValue ) { this->m_eValue = presetcolorvalDarkBlue ; SetRGBA(0,0,139); }
                else if ( _T("darkCyan")			== sValue ) { this->m_eValue = presetcolorvalDarkCyan ; SetRGBA(0,139,139); }
                else if ( _T("darkGoldenrod")		== sValue ) { this->m_eValue = presetcolorvalDarkGoldenrod ; SetRGBA(184,134,11); }
                else if ( _T("darkGray")			== sValue ) { this->m_eValue = presetcolorvalDarkGray ; SetRGBA(169,169,169); }
                else if ( _T("darkGreen")			== sValue ) { this->m_eValue = presetcolorvalDarkGreen ; SetRGBA(0,100,0); }
                else if ( _T("darkGrey")			== sValue ) { this->m_eValue = presetcolorvalDarkGrey ; SetRGBA(169,169,169); }
                else if ( _T("darkKhaki")			== sValue ) { this->m_eValue = presetcolorvalDarkKhaki ; SetRGBA(189,183,107); }
                else if ( _T("darkMagenta")			== sValue ) { this->m_eValue = presetcolorvalDarkMagenta ; SetRGBA(139,0,139); }
                else if ( _T("darkOliveGreen")		== sValue ) { this->m_eValue = presetcolorvalDarkOliveGreen ; SetRGBA(85,107,47); }
                else if ( _T("darkOrange")			== sValue ) { this->m_eValue = presetcolorvalDarkOrange ; SetRGBA(255,140,0); }
                else if ( _T("darkOrchid")			== sValue ) { this->m_eValue = presetcolorvalDarkOrchid ; SetRGBA(153,50,204); }
                else if ( _T("darkRed")				== sValue ) { this->m_eValue = presetcolorvalDarkRed ; SetRGBA(139,0,0); }
                else if ( _T("darkSalmon")			== sValue ) { this->m_eValue = presetcolorvalDarkSalmon ; SetRGBA(233,150,122); }
                else if ( _T("darkSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalDarkSeaGreen ; SetRGBA(143,188,143); }
                else if ( _T("darkSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateBlue ; SetRGBA(72,61,139); }
                else if ( _T("darkSlateGray")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateGray ; SetRGBA(47,79,79); }
                else if ( _T("darkSlateGrey")		== sValue ) { this->m_eValue = presetcolorvalDarkSlateGrey ; SetRGBA(47,79,79); }
                else if ( _T("darkTurquoise")		== sValue ) { this->m_eValue = presetcolorvalDarkTurquoise ; SetRGBA(0,206,209); }
                else if ( _T("darkViolet")			== sValue ) { this->m_eValue = presetcolorvalDarkViolet ; SetRGBA(148,0,211); }
                else if ( _T("deepPink")			== sValue ) { this->m_eValue = presetcolorvalDeepPink ; SetRGBA(255,20,147); }
                else if ( _T("deepSkyBlue")			== sValue ) { this->m_eValue = presetcolorvalDeepSkyBlue ; SetRGBA(0,191,255); }
                else if ( _T("dimGray")				== sValue ) { this->m_eValue = presetcolorvalDimGray ; SetRGBA(105,105,105); }
                else if ( _T("dimGrey")				== sValue ) { this->m_eValue = presetcolorvalDimGrey ; SetRGBA(105,105,105); }
                else if ( _T("dkBlue")				== sValue ) { this->m_eValue = presetcolorvalDkBlue ; SetRGBA(0,0,139); }
                else if ( _T("dkCyan")				== sValue ) { this->m_eValue = presetcolorvalDkCyan ; SetRGBA(0,139,139); }
                else if ( _T("dkGoldenrod")			== sValue ) { this->m_eValue = presetcolorvalDkGoldenrod ; SetRGBA(184,134,11); }
                else if ( _T("dkGray")				== sValue ) { this->m_eValue = presetcolorvalDkGray ; SetRGBA(169,169,169); }
                else if ( _T("dkGreen")				== sValue ) { this->m_eValue = presetcolorvalDkGreen ; SetRGBA(0,100,0); }
                else if ( _T("dkGrey")				== sValue ) { this->m_eValue = presetcolorvalDkGrey ; SetRGBA(169,169,169); }
                else if ( _T("dkKhaki")				== sValue ) { this->m_eValue = presetcolorvalDkKhaki ; SetRGBA(189,183,107); }
                else if ( _T("dkMagenta")			== sValue ) { this->m_eValue = presetcolorvalDkMagenta ; SetRGBA(139,0,139); }
                else if ( _T("dkOliveGreen")		== sValue ) { this->m_eValue = presetcolorvalDkOliveGreen ; SetRGBA(85,107,47); }
                else if ( _T("dkOrange")			== sValue ) { this->m_eValue = presetcolorvalDkOrange ; SetRGBA(255,140,0); }
                else if ( _T("dkOrchid")			== sValue ) { this->m_eValue = presetcolorvalDkOrchid ; SetRGBA(153,50,204); }
                else if ( _T("dkRed")				== sValue ) { this->m_eValue = presetcolorvalDkRed ; SetRGBA(139,0,0); }
                else if ( _T("dkSalmon")			== sValue ) { this->m_eValue = presetcolorvalDkSalmon ; SetRGBA(233,150,122); }
                else if ( _T("dkSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalDkSeaGreen ; SetRGBA(143,188,139); }
                else if ( _T("dkSlateBlue")			== sValue ) { this->m_eValue = presetcolorvalDkSlateBlue ; SetRGBA(72,61,139); }
                else if ( _T("dkSlateGray")			== sValue ) { this->m_eValue = presetcolorvalDkSlateGray ; SetRGBA(47,79,79); }
                else if ( _T("dkSlateGrey")			== sValue ) { this->m_eValue = presetcolorvalDkSlateGrey ; SetRGBA(47,79,79); }
                else if ( _T("dkTurquoise")			== sValue ) { this->m_eValue = presetcolorvalDkTurquoise ; SetRGBA(0,206,209); }
                else if ( _T("dkViolet")			== sValue ) { this->m_eValue = presetcolorvalDkViolet ; SetRGBA(148,0,211); }
                else if ( _T("dodgerBlue")			== sValue ) { this->m_eValue = presetcolorvalDodgerBlue ; SetRGBA(30,144,255); }
				break;
			case 'f':
                if      ( _T("firebrick")			== sValue ) { this->m_eValue = presetcolorvalFirebrick ; SetRGBA(178,34,34); }
                else if ( _T("floralWhite")			== sValue ) { this->m_eValue = presetcolorvalFloralWhite ; SetRGBA(255,250,240); }
                else if ( _T("forestGreen")			== sValue ) { this->m_eValue = presetcolorvalForestGreen ; SetRGBA(34,139,34); }
                else if ( _T("fuchsia")				== sValue ) { this->m_eValue = presetcolorvalFuchsia ; SetRGBA(255,0,255); }
				break;
			case 'g':
                if      ( _T("gainsboro")			== sValue ) { this->m_eValue = presetcolorvalGainsboro ; SetRGBA(220,220,220); }
                else if ( _T("ghostWhite")			== sValue ) { this->m_eValue = presetcolorvalGhostWhite ; SetRGBA(248,248,255); }
                else if ( _T("gold")				== sValue ) { this->m_eValue = presetcolorvalGold ; SetRGBA(255,215,0); }
                else if ( _T("goldenrod")			== sValue ) { this->m_eValue = presetcolorvalGoldenrod ; SetRGBA(218,165,32); }
                else if ( _T("gray")				== sValue ) { this->m_eValue = presetcolorvalGray ; SetRGBA(128,128,128); }
                else if ( _T("green")				== sValue ) { this->m_eValue = presetcolorvalGreen ; SetRGBA(0,128,0); }
                else if ( _T("greenYellow")			== sValue ) { this->m_eValue = presetcolorvalGreenYellow ; SetRGBA(173,255,47); }
                else if ( _T("grey")				== sValue ) { this->m_eValue = presetcolorvalGrey ; SetRGBA(128,128,128); }
				break;
			case 'h':
                if      ( _T("honeydew")			== sValue ) { this->m_eValue = presetcolorvalHoneydew ; SetRGBA(240,255,240); }
                else if ( _T("hotPink")				== sValue ) { this->m_eValue = presetcolorvalHotPink ; SetRGBA(255,105,180); }
				break;
			case 'i':
                if      ( _T("indianRed")			== sValue ) { this->m_eValue = presetcolorvalIndianRed ; SetRGBA(205,92,92); }
                else if ( _T("indigo")				== sValue ) { this->m_eValue = presetcolorvalIndigo ; SetRGBA(75,0,130); }
                else if ( _T("ivory")				== sValue ) { this->m_eValue = presetcolorvalIvory ; SetRGBA(255,255,240); }
				break;
			case 'k':
                if      ( _T("khaki")				== sValue ) { this->m_eValue = presetcolorvalKhaki ; SetRGBA(240,230,140); }
				break;
			case 'l':
                if      ( _T("lavender")			== sValue ) { this->m_eValue = presetcolorvalLavender ; SetRGBA(230,230,250); }
                else if ( _T("lavenderBlush")		== sValue ) { this->m_eValue = presetcolorvalLavenderBlush ; SetRGBA(255,240,245); }
                else if ( _T("lawnGreen")			== sValue ) { this->m_eValue = presetcolorvalLawnGreen ; SetRGBA(124,252,0); }
                else if ( _T("lemonChiffon")		== sValue ) { this->m_eValue = presetcolorvalLemonChiffon ; SetRGBA(255,250,205); }
                else if ( _T("lightBlue")			== sValue ) { this->m_eValue = presetcolorvalLightBlue ; SetRGBA(173,216,230); }
                else if ( _T("lightCoral")			== sValue ) { this->m_eValue = presetcolorvalLightCoral ; SetRGBA(240,128,128); }
                else if ( _T("lightCyan")			== sValue ) { this->m_eValue = presetcolorvalLightCyan ; SetRGBA(224,255,255); }
                else if ( _T("lightGoldenrodYellow")== sValue ) { this->m_eValue = presetcolorvalLightGoldenrodYellow; SetRGBA(250,250,210); }
                else if ( _T("lightGray")			== sValue ) { this->m_eValue = presetcolorvalLightGray ; SetRGBA(211,211,211); }
                else if ( _T("lightGreen")			== sValue ) { this->m_eValue = presetcolorvalLightGreen ; SetRGBA(144,238,144); }
                else if ( _T("lightGrey")			== sValue ) { this->m_eValue = presetcolorvalLightGrey ; SetRGBA(211,211,211); }
                else if ( _T("lightPink")			== sValue ) { this->m_eValue = presetcolorvalLightPink ; SetRGBA(255,182,193); }
                else if ( _T("lightSalmon")			== sValue ) { this->m_eValue = presetcolorvalLightSalmon ; SetRGBA(255,160,122); }
                else if ( _T("lightSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalLightSeaGreen ; SetRGBA(32,178,170); }
                else if ( _T("lightSkyBlue")		== sValue ) { this->m_eValue = presetcolorvalLightSkyBlue ; SetRGBA(135,206,250); }
                else if ( _T("lightSlateGray")		== sValue ) { this->m_eValue = presetcolorvalLightSlateGray ; SetRGBA(119,136,153); }
                else if ( _T("lightSlateGrey")		== sValue ) { this->m_eValue = presetcolorvalLightSlateGrey ; SetRGBA(119,136,153); }
                else if ( _T("lightSteelBlue")		== sValue ) { this->m_eValue = presetcolorvalLightSteelBlue ; SetRGBA(176,196,222); }
                else if ( _T("lightYellow")			== sValue ) { this->m_eValue = presetcolorvalLightYellow ; SetRGBA(255,255,224); }
                else if ( _T("lime")				== sValue ) { this->m_eValue = presetcolorvalLime ; SetRGBA(0,255,0); }
                else if ( _T("limeGreen")			== sValue ) { this->m_eValue = presetcolorvalLimeGreen ; SetRGBA(50,205,50); }
                else if ( _T("linen")				== sValue ) { this->m_eValue = presetcolorvalLinen; SetRGBA(250,240,230); }
                else if ( _T("ltBlue")				== sValue ) { this->m_eValue = presetcolorvalLtBlue ; SetRGBA(173,216,230); }
                else if ( _T("ltCoral")				== sValue ) { this->m_eValue = presetcolorvalLtCoral ; SetRGBA(240,128,128); }
                else if ( _T("ltCyan")				== sValue ) { this->m_eValue = presetcolorvalLtCyan ; SetRGBA(224,255,255); }
                else if ( _T("ltGoldenrodYellow")	== sValue ) { this->m_eValue = presetcolorvalLtGoldenrodYellow; SetRGBA(250,250,120); }
                else if ( _T("ltGray")				== sValue ) { this->m_eValue = presetcolorvalLtGray ; SetRGBA(211,211,211); }
                else if ( _T("ltGreen")				== sValue ) { this->m_eValue = presetcolorvalLtGreen ; SetRGBA(144,238,144); }
                else if ( _T("ltGrey")				== sValue ) { this->m_eValue = presetcolorvalLtGrey ; SetRGBA(211,211,211); }
                else if ( _T("ltPink")				== sValue ) { this->m_eValue = presetcolorvalLtPink ; SetRGBA(255,182,193); }
                else if ( _T("ltSalmon")			== sValue ) { this->m_eValue = presetcolorvalLtSalmon ; SetRGBA(255,160,122); }
                else if ( _T("ltSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalLtSeaGreen ; SetRGBA(32,178,170); }
                else if ( _T("ltSkyBlue")			== sValue ) { this->m_eValue = presetcolorvalLtSkyBlue ; SetRGBA(135,206,250); }
                else if ( _T("ltSlateGray")			== sValue ) { this->m_eValue = presetcolorvalLtSlateGray ; SetRGBA(119,136,153); }
                else if ( _T("ltSlateGrey")			== sValue ) { this->m_eValue = presetcolorvalLtSlateGrey ; SetRGBA(119,136,153); }
                else if ( _T("ltSteelBlue")			== sValue ) { this->m_eValue = presetcolorvalLtSteelBlue ; SetRGBA(176,196,222); }
                else if ( _T("ltYellow")			== sValue ) { this->m_eValue = presetcolorvalLtYellow ; SetRGBA(255,255,224); }
				break;
			case 'm':
                if      ( _T("magenta")				== sValue ) { this->m_eValue = presetcolorvalMagenta ; SetRGBA(255,0,255); }
                else if ( _T("maroon")				== sValue ) { this->m_eValue = presetcolorvalMaroon ; SetRGBA(128,0,0); }
                else if ( _T("medAquamarine")		== sValue ) { this->m_eValue = presetcolorvalMedAquamarine ; SetRGBA(102,205,170); }
                else if ( _T("medBlue")				== sValue ) { this->m_eValue = presetcolorvalMedBlue ; SetRGBA(0,0,205); }
                else if ( _T("mediumAquamarine")	== sValue ) { this->m_eValue = presetcolorvalMediumAquamarine; SetRGBA(102,205,170); }
                else if ( _T("mediumBlue")			== sValue ) { this->m_eValue = presetcolorvalMediumBlue ; SetRGBA(0,0,205); }
                else if ( _T("mediumOrchid")		== sValue ) { this->m_eValue = presetcolorvalMediumOrchid ; SetRGBA(186,85,211); }
                else if ( _T("mediumPurple")		== sValue ) { this->m_eValue = presetcolorvalMediumPurple ; SetRGBA(147,112,219); }
                else if ( _T("mediumSeaGreen")		== sValue ) { this->m_eValue = presetcolorvalMediumSeaGreen ; SetRGBA(60,179,113); }
                else if ( _T("mediumSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalMediumSlateBlue ; SetRGBA(123,104,238); }
                else if ( _T("mediumSpringGreen")	== sValue ) { this->m_eValue = presetcolorvalMediumSpringGreen; SetRGBA(0,250,154); }
                else if ( _T("mediumTurquoise")		== sValue ) { this->m_eValue = presetcolorvalMediumTurquoise ; SetRGBA(72,209,204); }
                else if ( _T("mediumVioletRed")		== sValue ) { this->m_eValue = presetcolorvalMediumVioletRed ; SetRGBA(199,21,133); }
                else if ( _T("medOrchid")			== sValue ) { this->m_eValue = presetcolorvalMedOrchid ; SetRGBA(186,85,211); }
                else if ( _T("medPurple")			== sValue ) { this->m_eValue = presetcolorvalMedPurple ; SetRGBA(147,112,219); }
                else if ( _T("medSeaGreen")			== sValue ) { this->m_eValue = presetcolorvalMedSeaGreen ; SetRGBA(60,179,113); }
                else if ( _T("medSlateBlue")		== sValue ) { this->m_eValue = presetcolorvalMedSlateBlue ; SetRGBA(123,104,238); }
                else if ( _T("medSpringGreen")		== sValue ) { this->m_eValue = presetcolorvalMedSpringGreen ; SetRGBA(0,250,154); }
                else if ( _T("medTurquoise")		== sValue ) { this->m_eValue = presetcolorvalMedTurquoise ; SetRGBA(72,209,204); }
                else if ( _T("medVioletRed")		== sValue ) { this->m_eValue = presetcolorvalMedVioletRed ; SetRGBA(199,21,133); }
                else if ( _T("midnightBlue")		== sValue ) { this->m_eValue = presetcolorvalMidnightBlue ; SetRGBA(25,25,112); }
                else if ( _T("mintCream")			== sValue ) { this->m_eValue = presetcolorvalMintCream ; SetRGBA(245,255,250); }
                else if ( _T("mistyRose")			== sValue ) { this->m_eValue = presetcolorvalMistyRose ; SetRGBA(255,228,225); }
                else if ( _T("moccasin")			== sValue ) { this->m_eValue = presetcolorvalMoccasin ; SetRGBA(255,228,181); }
				break;
			case 'n':
                if      ( _T("navajoWhite")			== sValue ) { this->m_eValue = presetcolorvalNavajoWhite ; SetRGBA(255,222,173); }
                else if ( _T("navy")				== sValue ) { this->m_eValue = presetcolorvalNavy ; SetRGBA(0,0,128); }
				break;
			case 'o':
                if      ( _T("oldLace")				== sValue ) { this->m_eValue = presetcolorvalOldLace ; SetRGBA(253,245,230); }
                else if ( _T("olive")				== sValue ) { this->m_eValue = presetcolorvalOlive ; SetRGBA(128,128,0); }
                else if ( _T("oliveDrab")			== sValue ) { this->m_eValue = presetcolorvalOliveDrab ; SetRGBA(107,142,35); }
                else if ( _T("orange")				== sValue ) { this->m_eValue = presetcolorvalOrange ; SetRGBA(255,165,0); }
                else if ( _T("orangeRed")			== sValue ) { this->m_eValue = presetcolorvalOrangeRed ; SetRGBA(255,69,0); }
                else if ( _T("orchid")				== sValue ) { this->m_eValue = presetcolorvalOrchid ; SetRGBA(218,112,214); }
				break;
			case 'p':
                if      ( _T("paleGoldenrod")		== sValue ) { this->m_eValue = presetcolorvalPaleGoldenrod ; SetRGBA(238,232,170); }
                else if ( _T("paleGreen")			== sValue ) { this->m_eValue = presetcolorvalPaleGreen ; SetRGBA(152,251,152); }
                else if ( _T("paleTurquoise")		== sValue ) { this->m_eValue = presetcolorvalPaleTurquoise ; SetRGBA(175,238,238); }
                else if ( _T("paleVioletRed")		== sValue ) { this->m_eValue = presetcolorvalPaleVioletRed ; SetRGBA(219,112,147); }
                else if ( _T("papayaWhip")			== sValue ) { this->m_eValue = presetcolorvalPapayaWhip ; SetRGBA(255,239,213); }
                else if ( _T("peachPuff")			== sValue ) { this->m_eValue = presetcolorvalPeachPuff ; SetRGBA(255,218,185); }
                else if ( _T("peru")				== sValue ) { this->m_eValue = presetcolorvalPeru ; SetRGBA(205,133,63); }
                else if ( _T("pink")				== sValue ) { this->m_eValue = presetcolorvalPink ; SetRGBA(255,192,203); }
                else if ( _T("plum")				== sValue ) { this->m_eValue = presetcolorvalPlum ; SetRGBA(221,160,221); }
                else if ( _T("powderBlue")			== sValue ) { this->m_eValue = presetcolorvalPowderBlue ; SetRGBA(176,224,230); }
                else if ( _T("purple")				== sValue ) { this->m_eValue = presetcolorvalPurple ; SetRGBA(128,0,128); }
				break;
			case 'r':
                if      ( _T("red")					== sValue ) { this->m_eValue = presetcolorvalRed ; SetRGBA(255,0,0); }
                else if ( _T("rosyBrown")			== sValue ) { this->m_eValue = presetcolorvalRosyBrown ; SetRGBA(188,143,143); }
                else if ( _T("royalBlue")			== sValue ) { this->m_eValue = presetcolorvalRoyalBlue ; SetRGBA(65,105,225); }
				break;
			case 's':
                if      ( _T("saddleBrown")			== sValue ) { this->m_eValue = presetcolorvalSaddleBrown ; SetRGBA(139,69,19); }
                else if ( _T("salmon")				== sValue ) { this->m_eValue = presetcolorvalSalmon ; SetRGBA(250,128,114); }
                else if ( _T("sandyBrown")			== sValue ) { this->m_eValue = presetcolorvalSandyBrown ; SetRGBA(244,164,96); }
                else if ( _T("seaGreen")			== sValue ) { this->m_eValue = presetcolorvalSeaGreen ; SetRGBA(46,139,87); }
                else if ( _T("seaShell")			== sValue ) { this->m_eValue = presetcolorvalSeaShell ; SetRGBA(255,245,238); }
                else if ( _T("sienna")				== sValue ) { this->m_eValue = presetcolorvalSienna ; SetRGBA(160,82,45); }
                else if ( _T("silver")				== sValue ) { this->m_eValue = presetcolorvalSilver ; SetRGBA(192,192,192); }
                else if ( _T("skyBlue")				== sValue ) { this->m_eValue = presetcolorvalSkyBlue ; SetRGBA(135,206,235); }
                else if ( _T("slateBlue")			== sValue ) { this->m_eValue = presetcolorvalSlateBlue ; SetRGBA(106,90,205); }
                else if ( _T("slateGray")			== sValue ) { this->m_eValue = presetcolorvalSlateGray ; SetRGBA(112,128,144); }
                else if ( _T("slateGrey")			== sValue ) { this->m_eValue = presetcolorvalSlateGrey ; SetRGBA(112,128,144); }
                else if ( _T("snow")				== sValue ) { this->m_eValue = presetcolorvalSnow ; SetRGBA(255,250,250); }
                else if ( _T("springGreen")			== sValue ) { this->m_eValue = presetcolorvalSpringGreen ; SetRGBA(0,255,127); }
                else if ( _T("steelBlue")			== sValue ) { this->m_eValue = presetcolorvalSteelBlue ; SetRGBA(70,130,180); }
				break;
			case 't':
                if      ( _T("tan")					== sValue ) { this->m_eValue = presetcolorvalTan ; SetRGBA(210,180,140); }
                else if ( _T("teal")				== sValue ) { this->m_eValue = presetcolorvalTeal ; SetRGBA(0,128,128); }
                else if ( _T("thistle")				== sValue ) { this->m_eValue = presetcolorvalThistle ; SetRGBA(216,191,216); }
                else if ( _T("tomato")				== sValue ) { this->m_eValue = presetcolorvalTomato ; SetRGBA(255,99,71); }
                else if ( _T("turquoise")			== sValue ) { this->m_eValue = presetcolorvalTurquoise ; SetRGBA(64,224,208); }
				break;
			case 'v':
                if      ( _T("violet")				== sValue ) { this->m_eValue = presetcolorvalViolet ; SetRGBA(238,130,238); }
				break;
			case 'w':
                if      ( _T("wheat")				== sValue ) { this->m_eValue = presetcolorvalWheat ; SetRGBA(245,222,179); }
                else if ( _T("white")				== sValue ) { this->m_eValue = presetcolorvalWhite ; SetRGBA(255,255,255); }
                else if ( _T("whiteSmoke")			== sValue ) { this->m_eValue = presetcolorvalWhiteSmoke ; SetRGBA(245,245,245); }
				break;
			case 'y':
                if      ( _T("yellow")				== sValue ) { this->m_eValue = presetcolorvalYellow ; SetRGBA(255,255,0); }
                else if ( _T("yellowGreen")			== sValue ) { this->m_eValue = presetcolorvalYellowGreen ; SetRGBA(154,205,50); }
				break;
			}

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetcolorvalAliceBlue:			return _T("aliceBlue");			
			case presetcolorvalAntiqueWhite:		return _T("antiqueWhite");		
			case presetcolorvalAqua:				return _T("aqua");				
			case presetcolorvalAquamarine:			return _T("aquamarine");			
			case presetcolorvalAzure:				return _T("azure");				
			case presetcolorvalBeige:				return _T("beige");				
			case presetcolorvalBisque:				return _T("bisque");				
			case presetcolorvalBlack :				return _T("black");				
			case presetcolorvalBlanchedAlmond :		return _T("blanchedAlmond");		
			case presetcolorvalBlue :				return _T("blue");				
			case presetcolorvalBlueViolet :			return _T("blueViolet");			
			case presetcolorvalBrown :				return _T("brown");				
			case presetcolorvalBurlyWood :			return _T("burlyWood");			
			case presetcolorvalCadetBlue :			return _T("cadetBlue");			
			case presetcolorvalChartreuse :			return _T("chartreuse");			
			case presetcolorvalChocolate :			return _T("chocolate");			
			case presetcolorvalCoral :				return _T("coral");				
			case presetcolorvalCornflowerBlue :		return _T("cornflowerBlue");		
			case presetcolorvalCornsilk :			return _T("cornsilk");			
			case presetcolorvalCrimson :			return _T("crimson");			
			case presetcolorvalCyan :				return _T("cyan");				
			case presetcolorvalDarkBlue :			return _T("darkBlue");			
			case presetcolorvalDarkCyan :			return _T("darkCyan");			
			case presetcolorvalDarkGoldenrod :		return _T("darkGoldenrod");		
			case presetcolorvalDarkGray :			return _T("darkGray");			
			case presetcolorvalDarkGreen:			return _T("darkGreen");			
			case presetcolorvalDarkGrey :			return _T("darkGrey");			
			case presetcolorvalDarkKhaki:			return _T("darkKhaki");			
			case presetcolorvalDarkMagenta :		return _T("darkMagenta");			
			case presetcolorvalDarkOliveGreen :		return _T("darkOliveGreen");		
			case presetcolorvalDarkOrange :			return _T("darkOrange");			
			case presetcolorvalDarkOrchid :			return _T("darkOrchid");			
			case presetcolorvalDarkRed :			return _T("darkRed");				
			case presetcolorvalDarkSalmon :			return _T("darkSalmon");			
			case presetcolorvalDarkSeaGreen :		return _T("darkSeaGreen");		
			case presetcolorvalDarkSlateBlue :		return _T("darkSlateBlue");		
			case presetcolorvalDarkSlateGray :		return _T("darkSlateGray");		
			case presetcolorvalDarkSlateGrey :		return _T("darkSlateGrey");		
			case presetcolorvalDarkTurquoise :		return _T("darkTurquoise");		
			case presetcolorvalDarkViolet :			return _T("darkViolet");			
			case presetcolorvalDeepPink :			return _T("deepPink");			
			case presetcolorvalDeepSkyBlue :		return _T("deepSkyBlue");			
			case presetcolorvalDimGray :			return _T("dimGray");				
			case presetcolorvalDimGrey :			return _T("dimGrey");				
			case presetcolorvalDkBlue :				return _T("dkBlue");				
			case presetcolorvalDkCyan :				return _T("dkCyan");				
			case presetcolorvalDkGoldenrod :		return _T("dkGoldenrod");			
			case presetcolorvalDkGray :				return _T("dkGray");				
			case presetcolorvalDkGreen :			return _T("dkGreen");				
			case presetcolorvalDkGrey :				return _T("dkGrey");				
			case presetcolorvalDkKhaki :			return _T("dkKhaki");				
			case presetcolorvalDkMagenta :			return _T("dkMagenta");			
			case presetcolorvalDkOliveGreen :		return _T("dkOliveGreen");		
			case presetcolorvalDkOrange :			return _T("dkOrange");			
			case presetcolorvalDkOrchid :			return _T("dkOrchid");			
			case presetcolorvalDkRed :				return _T("dkRed");				
			case presetcolorvalDkSalmon :			return _T("dkSalmon");			
			case presetcolorvalDkSeaGreen :			return _T("dkSeaGreen");			
			case presetcolorvalDkSlateBlue :		return _T("dkSlateBlue");			
			case presetcolorvalDkSlateGray :		return _T("dkSlateGray");			
			case presetcolorvalDkSlateGrey :		return _T("dkSlateGrey");			
			case presetcolorvalDkTurquoise :		return _T("dkTurquoise");			
			case presetcolorvalDkViolet :			return _T("dkViolet");			
			case presetcolorvalDodgerBlue :			return _T("dodgerBlue");			
			case presetcolorvalFirebrick :			return _T("firebrick");			
			case presetcolorvalFloralWhite :		return _T("floralWhite");			
			case presetcolorvalForestGreen :		return _T("forestGreen");			
			case presetcolorvalFuchsia :			return _T("fuchsia");				
			case presetcolorvalGainsboro :			return _T("gainsboro");			
			case presetcolorvalGhostWhite :			return _T("ghostWhite");			
			case presetcolorvalGold :				return _T("gold");				
			case presetcolorvalGoldenrod :			return _T("goldenrod");			
			case presetcolorvalGray :				return _T("gray");				
			case presetcolorvalGreen :				return _T("green");				
			case presetcolorvalGreenYellow :		return _T("greenYellow");			
			case presetcolorvalGrey :				return _T("grey");				
			case presetcolorvalHoneydew :			return _T("honeydew");			
			case presetcolorvalHotPink :			return _T("hotPink");				
			case presetcolorvalIndianRed :			return _T("indianRed");			
			case presetcolorvalIndigo :				return _T("indigo");				
			case presetcolorvalIvory :				return _T("ivory");				
			case presetcolorvalKhaki :				return _T("khaki");				
			case presetcolorvalLavender :			return _T("lavender");			
			case presetcolorvalLavenderBlush :		return _T("lavenderBlush");		
			case presetcolorvalLawnGreen:			return _T("lawnGreen");			
			case presetcolorvalLemonChiffon :		return _T("lemonChiffon");		
			case presetcolorvalLightBlue :			return _T("lightBlue");			
			case presetcolorvalLightCoral :			return _T("lightCoral");			
			case presetcolorvalLightCyan :			return _T("lightCyan");			
			case presetcolorvalLightGoldenrodYellow:return _T("lightGoldenrodYellow");
			case presetcolorvalLightGray :			return _T("lightGray");			
			case presetcolorvalLightGreen :			return _T("lightGreen");			
			case presetcolorvalLightGrey :			return _T("lightGrey");			
			case presetcolorvalLightPink :			return _T("lightPink");			
			case presetcolorvalLightSalmon :		return _T("lightSalmon");			
			case presetcolorvalLightSeaGreen :		return _T("lightSeaGreen");		
			case presetcolorvalLightSkyBlue :		return _T("lightSkyBlue");		
			case presetcolorvalLightSlateGray :		return _T("lightSlateGray");		
			case presetcolorvalLightSlateGrey :		return _T("lightSlateGrey");		
			case presetcolorvalLightSteelBlue :		return _T("lightSteelBlue");		
			case presetcolorvalLightYellow :		return _T("lightYellow");			
			case presetcolorvalLime :				return _T("lime");				
			case presetcolorvalLimeGreen :			return _T("limeGreen");			
			case presetcolorvalLinen:				return _T("linen");				
			case presetcolorvalLtBlue :				return _T("ltBlue");				
			case presetcolorvalLtCoral :			return _T("ltCoral");				
			case presetcolorvalLtCyan :				return _T("ltCyan");				
			case presetcolorvalLtGoldenrodYellow:	return _T("ltGoldenrodYellow");	
			case presetcolorvalLtGray :				return _T("ltGray");				
			case presetcolorvalLtGreen :			return _T("ltGreen");				
			case presetcolorvalLtGrey :				return _T("ltGrey");				
			case presetcolorvalLtPink :				return _T("ltPink");				
			case presetcolorvalLtSalmon :			return _T("ltSalmon");			
			case presetcolorvalLtSeaGreen :			return _T("ltSeaGreen");			
			case presetcolorvalLtSkyBlue :			return _T("ltSkyBlue");			
			case presetcolorvalLtSlateGray :		return _T("ltSlateGray");			
			case presetcolorvalLtSlateGrey :		return _T("ltSlateGrey");			
			case presetcolorvalLtSteelBlue :		return _T("ltSteelBlue");			
			case presetcolorvalLtYellow :			return _T("ltYellow");			
			case presetcolorvalMagenta :			return _T("magenta");				
			case presetcolorvalMaroon :				return _T("maroon");				
			case presetcolorvalMedAquamarine :		return _T("medAquamarine");		
			case presetcolorvalMedBlue :			return _T("medBlue");				
			case presetcolorvalMediumAquamarine:	return _T("mediumAquamarine");	
			case presetcolorvalMediumBlue :			return _T("mediumBlue");			
			case presetcolorvalMediumOrchid :		return _T("mediumOrchid");		
			case presetcolorvalMediumPurple :		return _T("mediumPurple");		
			case presetcolorvalMediumSeaGreen :		return _T("mediumSeaGreen");		
			case presetcolorvalMediumSlateBlue :	return _T("mediumSlateBlue");		
			case presetcolorvalMediumSpringGreen:	return _T("mediumSpringGreen");	
			case presetcolorvalMediumTurquoise :	return _T("mediumTurquoise");		
			case presetcolorvalMediumVioletRed :	return _T("mediumVioletRed");		
			case presetcolorvalMedOrchid :			return _T("medOrchid");			
			case presetcolorvalMedPurple :			return _T("medPurple");			
			case presetcolorvalMedSeaGreen :		return _T("medSeaGreen");			
			case presetcolorvalMedSlateBlue :		return _T("medSlateBlue");		
			case presetcolorvalMedSpringGreen :		return _T("medSpringGreen");		
			case presetcolorvalMedTurquoise :		return _T("medTurquoise");		
			case presetcolorvalMedVioletRed :		return _T("medVioletRed");		
			case presetcolorvalMidnightBlue :		return _T("midnightBlue");		
			case presetcolorvalMintCream :			return _T("mintCream");			
			case presetcolorvalMistyRose :			return _T("mistyRose");			
			case presetcolorvalMoccasin :			return _T("moccasin");			
			case presetcolorvalNavajoWhite :		return _T("navajoWhite");			
			case presetcolorvalNavy :				return _T("navy");				
			case presetcolorvalOldLace :			return _T("oldLace");				
			case presetcolorvalOlive :				return _T("olive");				
			case presetcolorvalOliveDrab :			return _T("oliveDrab");			
			case presetcolorvalOrange :				return _T("orange");				
			case presetcolorvalOrangeRed :			return _T("orangeRed");			
			case presetcolorvalOrchid :				return _T("orchid");				
			case presetcolorvalPaleGoldenrod :		return _T("paleGoldenrod");		
			case presetcolorvalPaleGreen :			return _T("paleGreen");			
			case presetcolorvalPaleTurquoise :		return _T("paleTurquoise");		
			case presetcolorvalPaleVioletRed :		return _T("paleVioletRed");		
			case presetcolorvalPapayaWhip :			return _T("papayaWhip");			
			case presetcolorvalPeachPuff :			return _T("peachPuff");			
			case presetcolorvalPeru :				return _T("peru");				
			case presetcolorvalPink :				return _T("pink");				
			case presetcolorvalPlum :				return _T("plum");				
			case presetcolorvalPowderBlue :			return _T("powderBlue");			
			case presetcolorvalPurple :				return _T("purple");				
			case presetcolorvalRed :				return _T("red");					
			case presetcolorvalRosyBrown :			return _T("rosyBrown");			
			case presetcolorvalRoyalBlue :			return _T("royalBlue");			
			case presetcolorvalSaddleBrown :		return _T("saddleBrown");			
			case presetcolorvalSalmon :				return _T("salmon");				
			case presetcolorvalSandyBrown :			return _T("sandyBrown");			
			case presetcolorvalSeaGreen :			return _T("seaGreen");			
			case presetcolorvalSeaShell :			return _T("seaShell");			
			case presetcolorvalSienna :				return _T("sienna");				
			case presetcolorvalSilver :				return _T("silver");				
			case presetcolorvalSkyBlue :			return _T("skyBlue");				
			case presetcolorvalSlateBlue :			return _T("slateBlue");			
			case presetcolorvalSlateGray :			return _T("slateGray");			
			case presetcolorvalSlateGrey :			return _T("slateGrey");			
			case presetcolorvalSnow :				return _T("snow");				
			case presetcolorvalSpringGreen :		return _T("springGreen");			
			case presetcolorvalSteelBlue :			return _T("steelBlue");			
			case presetcolorvalTan :				return _T("tan");					
			case presetcolorvalTeal :				return _T("teal");				
			case presetcolorvalThistle :			return _T("thistle");				
			case presetcolorvalTomato :				return _T("tomato");				
			case presetcolorvalTurquoise :			return _T("turquoise");			
			case presetcolorvalViolet :				return _T("violet");				
			case presetcolorvalWheat :				return _T("wheat");				
			case presetcolorvalWhite :				return _T("white");				
			case presetcolorvalWhiteSmoke :			return _T("whiteSmoke");			
			case presetcolorvalYellow :				return _T("yellow");				
			case presetcolorvalYellowGreen :		return _T("yellowGreen");			
			default :								return _T("black");
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

		virtual EPresetLineDashVal FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'd':
                if      ( _T("dash")          == sValue ) this->m_eValue = presetlinedashvalDash;
                else if ( _T("dashDot")       == sValue ) this->m_eValue = presetlinedashvalDashDot;
                else if ( _T("dot")           == sValue ) this->m_eValue = presetlinedashvalDot;
				break;
			case 'l':
                if      ( _T("lgDash")        == sValue ) this->m_eValue = presetlinedashvalLgDash;
                else if ( _T("lgDashDot")     == sValue ) this->m_eValue = presetlinedashvalLgDashDot;
                else if ( _T("lgDashDotDot")  == sValue ) this->m_eValue = presetlinedashvalLgDashDotDot;
				break;
			case 's':
                if      ( _T("solid")         == sValue ) this->m_eValue = presetlinedashvalSolid;
                else if ( _T("sysDash")       == sValue ) this->m_eValue = presetlinedashvalSysDash;
                else if ( _T("sysDashDot")    == sValue ) this->m_eValue = presetlinedashvalSysDashDot;
                else if ( _T("sysDashDotDot") == sValue ) this->m_eValue = presetlinedashvalSysDashDotDot;
                else if ( _T("sysDot")        == sValue ) this->m_eValue = presetlinedashvalSysDot;
				break;
			}

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetlinedashvalDash:          return _T("dash");			
			case presetlinedashvalDashDot:       return _T("dashDot");			
			case presetlinedashvalDot:           return _T("dot");		
			case presetlinedashvalLgDash:        return _T("lgDash");			
			case presetlinedashvalLgDashDot:     return _T("lgDashDot");			
			case presetlinedashvalLgDashDotDot:  return _T("lgDashDotDot");		
			case presetlinedashvalSolid:         return _T("solid");			
			case presetlinedashvalSysDash:       return _T("sysDash");			
			case presetlinedashvalSysDashDot:    return _T("sysDashDot");		
			case presetlinedashvalSysDashDotDot: return _T("sysDashDotDot");		
			case presetlinedashvalSysDot:        return _T("sysDot");		
			default :                            return _T("solid");
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

		virtual EPresetMaterialType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("clear")             == sValue ) this->m_eValue = presetmaterialtypeClear;
				break;
			case 'd':
                if      ( _T("dkEdge")            == sValue ) this->m_eValue = presetmaterialtypeDkEdge;
				break;
			case 'f':
                if      ( _T("flat")              == sValue ) this->m_eValue = presetmaterialtypeFlat;
				break;
			case 'l':
                if      ( _T("legacyMatte")       == sValue ) this->m_eValue = presetmaterialtypeLegacyMatte;
                else if ( _T("legacyMetal")       == sValue ) this->m_eValue = presetmaterialtypeLegacyMetal;
                else if ( _T("legacyPlastic")     == sValue ) this->m_eValue = presetmaterialtypeLegacyPlastic;
                else if ( _T("legacyWireframe")   == sValue ) this->m_eValue = presetmaterialtypeLegacyWireframe;
				break;
			case 'm':
                if      ( _T("matte")             == sValue ) this->m_eValue = presetmaterialtypeMatte;
                else if ( _T("metal")             == sValue ) this->m_eValue = presetmaterialtypeMetal;
				break;
			case 'p':
                if      ( _T("plastic")           == sValue ) this->m_eValue = presetmaterialtypePlastic;
                else if ( _T("powder")            == sValue ) this->m_eValue = presetmaterialtypePowder;
				break;
			case 's':
                if      ( _T("softEdge")          == sValue ) this->m_eValue = presetmaterialtypeSoftEdge;
                else if ( _T("softmetal")         == sValue ) this->m_eValue = presetmaterialtypeSoftmetal;
				break;
			case 't':
                if      ( _T("translucentPowder") == sValue ) this->m_eValue = presetmaterialtypeTranslucentPowder;
				break;
			case 'w':
                if      ( _T("warmMatte")         == sValue ) this->m_eValue = presetmaterialtypeWarmMatte;
				break;
			}

            return this->m_eValue;
		}

		virtual CString             ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetmaterialtypeClear             : return _T("clear");
			case presetmaterialtypeDkEdge            : return _T("dkEdge");
			case presetmaterialtypeFlat              : return _T("flat");
			case presetmaterialtypeLegacyMatte       : return _T("legacyMatte");
			case presetmaterialtypeLegacyMetal       : return _T("legacyMetal"); 
			case presetmaterialtypeLegacyPlastic     : return _T("legacyPlastic");
			case presetmaterialtypeLegacyWireframe   : return _T("legacyWireframe");
			case presetmaterialtypeMatte             : return _T("matte");
			case presetmaterialtypeMetal             : return _T("metal");
			case presetmaterialtypePlastic           : return _T("plastic");
			case presetmaterialtypePowder            : return _T("powder");
			case presetmaterialtypeSoftEdge          : return _T("softEdge"); 
			case presetmaterialtypeSoftmetal         : return _T("softmetal");
			case presetmaterialtypeTranslucentPowder : return _T("translucentPowder");
			case presetmaterialtypeWarmMatte         : return _T("warmMatte");
			default                                  : return _T("clear");
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
		virtual EPresetPatternVal FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("cross")      == sValue ) this->m_eValue = presetpatternvalCross;
				break;
			case 'd':
                if      ( _T("dashDnDiag") == sValue ) this->m_eValue = presetpatternvalDashDnDiag;
                else if ( _T("dashHorz")   == sValue ) this->m_eValue = presetpatternvalDashHorz;
                else if ( _T("dashUpDiag") == sValue ) this->m_eValue = presetpatternvalDashUpDiag;
                else if ( _T("dashVert")   == sValue ) this->m_eValue = presetpatternvalDashVert;
                else if ( _T("diagBrick")  == sValue ) this->m_eValue = presetpatternvalDiagBrick;
                else if ( _T("diagCross")  == sValue ) this->m_eValue = presetpatternvalDiagCross;
                else if ( _T("divot")      == sValue ) this->m_eValue = presetpatternvalDivot;
                else if ( _T("dkDnDiag")   == sValue ) this->m_eValue = presetpatternvalDkDnDiag;
                else if ( _T("dkHorz")     == sValue ) this->m_eValue = presetpatternvalDkHorz;
                else if ( _T("dkUpDiag")   == sValue ) this->m_eValue = presetpatternvalDkUpDiag;
                else if ( _T("dkVert")     == sValue ) this->m_eValue = presetpatternvalDkVert;
                else if ( _T("dnDiag")     == sValue ) this->m_eValue = presetpatternvalDnDiag;
                else if ( _T("dotDmnd")    == sValue ) this->m_eValue = presetpatternvalDotDmnd;
                else if ( _T("dotGrid")    == sValue ) this->m_eValue = presetpatternvalDotGrid;
				break;
			case 'h':
                if      ( _T("horz")       == sValue ) this->m_eValue = presetpatternvalHorz;
                else if ( _T("horzBrick")  == sValue ) this->m_eValue = presetpatternvalHorzBrick;
				break;
			case 'l':
                if      ( _T("lgCheck")    == sValue ) this->m_eValue = presetpatternvalLgCheck;
                else if ( _T("lgConfetti") == sValue ) this->m_eValue = presetpatternvalLgConfetti;
                else if ( _T("lgGrid")     == sValue ) this->m_eValue = presetpatternvalLgGrid;
                else if ( _T("ltDnDiag")   == sValue ) this->m_eValue = presetpatternvalLtDnDiag;
                else if ( _T("ltHorz")     == sValue ) this->m_eValue = presetpatternvalLtHorz;
                else if ( _T("ltUpDiag")   == sValue ) this->m_eValue = presetpatternvalLtUpDiag;
                else if ( _T("ltVert")     == sValue ) this->m_eValue = presetpatternvalLtVert;
				break;
			case 'n':
                if      ( _T("narHorz")    == sValue ) this->m_eValue = presetpatternvalNarHorz;
                else if ( _T("narVert")    == sValue ) this->m_eValue = presetpatternvalNarVert;
				break;
			case 'o':
                if      ( _T("openDmnd")   == sValue ) this->m_eValue = presetpatternvalOpenDmnd;
				break;
			case 'p':
                if      ( _T("pct10")      == sValue ) this->m_eValue = presetpatternvalPct10;
                else if ( _T("pct20")      == sValue ) this->m_eValue = presetpatternvalPct20;
                else if ( _T("pct25")      == sValue ) this->m_eValue = presetpatternvalPct25;
                else if ( _T("pct30")      == sValue ) this->m_eValue = presetpatternvalPct30;
                else if ( _T("pct40")      == sValue ) this->m_eValue = presetpatternvalPct40;
                else if ( _T("pct5")       == sValue ) this->m_eValue = presetpatternvalPct5;
                else if ( _T("pct50")      == sValue ) this->m_eValue = presetpatternvalPct50;
                else if ( _T("pct60")      == sValue ) this->m_eValue = presetpatternvalPct60;
                else if ( _T("pct70")      == sValue ) this->m_eValue = presetpatternvalPct70;
                else if ( _T("pct75")      == sValue ) this->m_eValue = presetpatternvalPct75;
                else if ( _T("pct80")      == sValue ) this->m_eValue = presetpatternvalPct80;
                else if ( _T("pct90")      == sValue ) this->m_eValue = presetpatternvalPct90;
                else if ( _T("plaid")      == sValue ) this->m_eValue = presetpatternvalPlaid;
				break;
			case 's':
                if      ( _T("shingle")    == sValue ) this->m_eValue = presetpatternvalShingle;
                else if ( _T("smCheck")    == sValue ) this->m_eValue = presetpatternvalSmCheck;
                else if ( _T("smConfetti") == sValue ) this->m_eValue = presetpatternvalSmConfetti;
                else if ( _T("smGrid")     == sValue ) this->m_eValue = presetpatternvalSmGrid;
                else if ( _T("solidDmnd")  == sValue ) this->m_eValue = presetpatternvalSolidDmnd;
                else if ( _T("sphere")     == sValue ) this->m_eValue = presetpatternvalSphere;
				break;
			case 't':
                if      ( _T("trellis")    == sValue ) this->m_eValue = presetpatternvalTrellis;
				break;
			case 'u':
                if      ( _T("upDiag")     == sValue ) this->m_eValue = presetpatternvalUpDiag;
				break;
			case 'v':
                if      ( _T("vert")       == sValue ) this->m_eValue = presetpatternvalVert;
				break;
			case 'w':
                if      ( _T("wave")       == sValue ) this->m_eValue = presetpatternvalWave;
                else if ( _T("wdDnDiag")   == sValue ) this->m_eValue = presetpatternvalWdDnDiag;
                else if ( _T("wdUpDiag")   == sValue ) this->m_eValue = presetpatternvalWdUpDiag;
                else if ( _T("weave")      == sValue ) this->m_eValue = presetpatternvalWeave;
				break;
			case 'z':
                if      ( _T("zigZag")     == sValue ) this->m_eValue = presetpatternvalZigZag;
				break;
			}

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetpatternvalCross:      return _T("cross");
			case presetpatternvalDashDnDiag: return _T("dashDnDiag");
			case presetpatternvalDashHorz:   return _T("dashHorz");
			case presetpatternvalDashUpDiag: return _T("dashUpDiag");
			case presetpatternvalDashVert:   return _T("dashVert");
			case presetpatternvalDiagBrick:  return _T("diagBrick");
			case presetpatternvalDiagCross:  return _T("diagCross");
			case presetpatternvalDivot:      return _T("divot");
			case presetpatternvalDkDnDiag:   return _T("dkDnDiag");
			case presetpatternvalDkHorz:     return _T("dkHorz");
			case presetpatternvalDkUpDiag:   return _T("dkUpDiag");
			case presetpatternvalDkVert:     return _T("dkVert");
			case presetpatternvalDnDiag:     return _T("dnDiag");
			case presetpatternvalDotDmnd:    return _T("dotDmnd");
			case presetpatternvalDotGrid:    return _T("dotGrid");
			case presetpatternvalHorz:       return _T("horz");
			case presetpatternvalHorzBrick:  return _T("horzBrick");
			case presetpatternvalLgCheck:    return _T("lgCheck");
			case presetpatternvalLgConfetti: return _T("lgConfetti");
			case presetpatternvalLgGrid:     return _T("lgGrid");
			case presetpatternvalLtDnDiag:   return _T("ltDnDiag");
			case presetpatternvalLtHorz:     return _T("ltHorz");
			case presetpatternvalLtUpDiag:   return _T("ltUpDiag");
			case presetpatternvalLtVert:     return _T("ltVert");
			case presetpatternvalNarHorz:    return _T("narHorz");
			case presetpatternvalNarVert:    return _T("narVert");
			case presetpatternvalOpenDmnd:   return _T("openDmnd");
			case presetpatternvalPct10:      return _T("pct10");
			case presetpatternvalPct20:      return _T("pct20");
			case presetpatternvalPct25:      return _T("pct25");
			case presetpatternvalPct30:      return _T("pct30");
			case presetpatternvalPct40:      return _T("pct40");
			case presetpatternvalPct5:       return _T("pct5");
			case presetpatternvalPct50:      return _T("pct50");
			case presetpatternvalPct60:      return _T("pct60");
			case presetpatternvalPct70:      return _T("pct70");
			case presetpatternvalPct75:      return _T("pct75");
			case presetpatternvalPct80:      return _T("pct80");
			case presetpatternvalPct90:      return _T("pct90");
			case presetpatternvalPlaid:      return _T("plaid");
			case presetpatternvalShingle:    return _T("shingle");
			case presetpatternvalSmCheck:    return _T("smCheck");
			case presetpatternvalSmConfetti: return _T("smConfetti");
			case presetpatternvalSmGrid:     return _T("smGrid");
			case presetpatternvalSolidDmnd:  return _T("solidDmnd");
			case presetpatternvalSphere:     return _T("sphere");
			case presetpatternvalTrellis:    return _T("trellis");
			case presetpatternvalUpDiag:     return _T("upDiag");
			case presetpatternvalVert:       return _T("vert");
			case presetpatternvalWave:       return _T("wave");
			case presetpatternvalWdDnDiag:   return _T("wdDnDiag");
			case presetpatternvalWdUpDiag:   return _T("wdUpDiag");
			case presetpatternvalWeave:      return _T("weave");
			case presetpatternvalZigZag:     return _T("zigZag Zag");
			default :                        return _T("pct10");
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
		virtual EPresetShadowVal FromString(CString &sValue)
		{
            if      ( _T("shdw1")  == sValue ) this->m_eValue = presetshadowvalShdw1;
            else if ( _T("shdw10") == sValue ) this->m_eValue = presetshadowvalShdw10;
            else if ( _T("shdw11") == sValue ) this->m_eValue = presetshadowvalShdw11;
            else if ( _T("shdw12") == sValue ) this->m_eValue = presetshadowvalShdw12;
            else if ( _T("shdw13") == sValue ) this->m_eValue = presetshadowvalShdw13;
            else if ( _T("shdw14") == sValue ) this->m_eValue = presetshadowvalShdw14;
            else if ( _T("shdw15") == sValue ) this->m_eValue = presetshadowvalShdw15;
            else if ( _T("shdw16") == sValue ) this->m_eValue = presetshadowvalShdw16;
            else if ( _T("shdw17") == sValue ) this->m_eValue = presetshadowvalShdw17;
            else if ( _T("shdw18") == sValue ) this->m_eValue = presetshadowvalShdw18;
            else if ( _T("shdw19") == sValue ) this->m_eValue = presetshadowvalShdw19;
            else if ( _T("shdw2")  == sValue ) this->m_eValue = presetshadowvalShdw2;
            else if ( _T("shdw20") == sValue ) this->m_eValue = presetshadowvalShdw20;
            else if ( _T("shdw3")  == sValue ) this->m_eValue = presetshadowvalShdw3;
            else if ( _T("shdw4")  == sValue ) this->m_eValue = presetshadowvalShdw4;
            else if ( _T("shdw5")  == sValue ) this->m_eValue = presetshadowvalShdw5;
            else if ( _T("shdw6")  == sValue ) this->m_eValue = presetshadowvalShdw6;
            else if ( _T("shdw7")  == sValue ) this->m_eValue = presetshadowvalShdw7;
            else if ( _T("shdw8")  == sValue ) this->m_eValue = presetshadowvalShdw8;
            else if ( _T("shdw9")  == sValue ) this->m_eValue = presetshadowvalShdw9;
            else                               this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString          ToString  () const 
		{
            switch(this->m_eValue)
			{
			case presetshadowvalShdw1:  return _T("shdw1");			
			case presetshadowvalShdw2:  return _T("shdw2");		
			case presetshadowvalShdw3:  return _T("shdw3");				
			case presetshadowvalShdw4:  return _T("shdw4");			
			case presetshadowvalShdw5:  return _T("shdw5");				
			case presetshadowvalShdw6:  return _T("shdw6");				
			case presetshadowvalShdw7:  return _T("shdw7");				
			case presetshadowvalShdw8:  return _T("shdw8");				
			case presetshadowvalShdw9:  return _T("shdw9");		
			case presetshadowvalShdw10: return _T("shdw10");				
			case presetshadowvalShdw11: return _T("shdw11");			
			case presetshadowvalShdw12: return _T("shdw12");				
			case presetshadowvalShdw13: return _T("shdw13");			
			case presetshadowvalShdw14: return _T("shdw14");			
			case presetshadowvalShdw15: return _T("shdw15");			
			case presetshadowvalShdw16: return _T("shdw16");			
			case presetshadowvalShdw17: return _T("shdw17");				
			case presetshadowvalShdw18: return _T("shdw18");		
			case presetshadowvalShdw19: return _T("shdw19");			
			case presetshadowvalShdw20: return _T("shdw20");			
			default :                   return _T("shdw14");
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

		virtual ERectAlignment FromString(CString &sValue)
		{
            if      ( _T("b")   == sValue ) this->m_eValue = rectalignmentB;
            else if ( _T("bl")  == sValue ) this->m_eValue = rectalignmentBL;
            else if ( _T("br")  == sValue ) this->m_eValue = rectalignmentBR;
            else if ( _T("ctr") == sValue ) this->m_eValue = rectalignmentCtr;
            else if ( _T("l")   == sValue ) this->m_eValue = rectalignmentL;
            else if ( _T("r")   == sValue ) this->m_eValue = rectalignmentR;
            else if ( _T("t")   == sValue ) this->m_eValue = rectalignmentT;
            else if ( _T("tl")  == sValue ) this->m_eValue = rectalignmentTL;
            else if ( _T("tr")  == sValue ) this->m_eValue = rectalignmentTR;
            else                            this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case rectalignmentB  : return _T("b");
			case rectalignmentBL : return _T("bl");
			case rectalignmentBR : return _T("br");
			case rectalignmentCtr: return _T("ctr");
			case rectalignmentL  : return _T("l");
			case rectalignmentR  : return _T("r");
			case rectalignmentT  : return _T("t");
			case rectalignmentTL : return _T("tl");
			case rectalignmentTR : return _T("tr");
			default              : return _T("bl");
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

		virtual EShemeColorVal FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;
			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':
                if      ( _T("accent1") == sValue ) this->m_eValue = shemecolorvalAccent1;
                else if ( _T("accent2") == sValue ) this->m_eValue = shemecolorvalAccent2;
                else if ( _T("accent3") == sValue ) this->m_eValue = shemecolorvalAccent3;
                else if ( _T("accent4") == sValue ) this->m_eValue = shemecolorvalAccent4;
                else if ( _T("accent5") == sValue ) this->m_eValue = shemecolorvalAccent5;
                else if ( _T("accent6") == sValue ) this->m_eValue = shemecolorvalAccent6;
				break;
			case 'b':
                if      ( _T("bg1") == sValue ) this->m_eValue = shemecolorvalBg1;
                else if ( _T("bg2") == sValue ) this->m_eValue = shemecolorvalBg2;
				break;
			case 'd':
                if      ( _T("dk1") == sValue ) this->m_eValue = shemecolorvalDk1;
                else if ( _T("dk2") == sValue ) this->m_eValue = shemecolorvalDk2;
				break;
			case 'f':
                if      ( _T("folHlink") == sValue ) this->m_eValue = shemecolorvalFolHlink;
				break;
			case 'h':
                if      ( _T("hlink") == sValue ) this->m_eValue = shemecolorvalHlink;
				break;
			case 'l':
                if      ( _T("lt1") == sValue ) this->m_eValue = shemecolorvalLt1;
                else if ( _T("lt2") == sValue ) this->m_eValue = shemecolorvalLt2;
				break;
			case 'p':
                if      ( _T("phClr") == sValue ) this->m_eValue = shemecolorvalPhClr;
				break;
			case 't':
                if      ( _T("tx1") == sValue ) this->m_eValue = shemecolorvalTx1;
                else if ( _T("tx2") == sValue ) this->m_eValue = shemecolorvalTx2;
				break;
			}

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case shemecolorvalAccent1:  return _T("accent1");			
			case shemecolorvalAccent2:  return _T("accent2");		
			case shemecolorvalAccent3:  return _T("accent3");				
			case shemecolorvalAccent4:  return _T("accent4");			
			case shemecolorvalAccent5:  return _T("accent5");				
			case shemecolorvalAccent6:  return _T("accent6");				
			case shemecolorvalBg1:      return _T("bg1");				
			case shemecolorvalBg2:      return _T("bg2");				
			case shemecolorvalDk1:      return _T("dk1");		
			case shemecolorvalDk2:      return _T("dk2");				
			case shemecolorvalFolHlink: return _T("folHlink");			
			case shemecolorvalHlink:    return _T("hlink");				
			case shemecolorvalLt1:      return _T("lt1");			
			case shemecolorvalLt2:      return _T("lt2");			
			case shemecolorvalPhClr:    return _T("phClr");			
			case shemecolorvalTx1:      return _T("tx1");			
			case shemecolorvalTx2:      return _T("tx2");				
			default :                   return _T("accent1");
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

		virtual EShapeType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':

                if      ( _T("accentBorderCallout1")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout1;
                else if ( _T("accentBorderCallout2")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout2;
                else if ( _T("accentBorderCallout3")	== sValue ) this->m_eValue = shapetypeAccentBorderCallout3;
                else if ( _T("accentCallout1")			== sValue ) this->m_eValue = shapetypeAccentCallout1;
                else if ( _T("accentCallout2")			== sValue ) this->m_eValue = shapetypeAccentCallout2;
                else if ( _T("accentCallout3")			== sValue ) this->m_eValue = shapetypeAccentCallout3;
                else if ( _T("actionButtonBackPrevious")== sValue ) this->m_eValue = shapetypeActionButtonBackPrevious;
                else if ( _T("actionButtonBeginning")	== sValue ) this->m_eValue = shapetypeActionButtonBeginning;
                else if ( _T("actionButtonBlank")		== sValue ) this->m_eValue = shapetypeActionButtonBlank;
                else if ( _T("actionButtonDocument")	== sValue ) this->m_eValue = shapetypeActionButtonDocument;
                else if ( _T("actionButtonEnd")			== sValue ) this->m_eValue = shapetypeActionButtonEnd;
                else if ( _T("actionButtonForwardNext")	== sValue ) this->m_eValue = shapetypeActionButtonForwardNext;
                else if ( _T("actionButtonHelp")		== sValue ) this->m_eValue = shapetypeActionButtonHelp;
                else if ( _T("actionButtonHome")		== sValue ) this->m_eValue = shapetypeActionButtonHome;
                else if ( _T("actionButtonInformation")	== sValue ) this->m_eValue = shapetypeActionButtonInformation;
                else if ( _T("actionButtonMovie")		== sValue ) this->m_eValue = shapetypeActionButtonMovie;
                else if ( _T("actionButtonReturn")		== sValue ) this->m_eValue = shapetypeActionButtonReturn;
                else if ( _T("actionButtonSound")		== sValue ) this->m_eValue = shapetypeActionButtonSound;
                else if ( _T("arc")						== sValue ) this->m_eValue = shapetypeArc;
				break;

			case 'b':
                if      ( _T("bentArrow")				== sValue ) this->m_eValue = shapetypeBentArrow;
                else if ( _T("bentConnector2")			== sValue ) this->m_eValue = shapetypeBentConnector2;
                else if ( _T("bentConnector3")			== sValue ) this->m_eValue = shapetypeBentConnector3;
                else if ( _T("bentConnector4")			== sValue ) this->m_eValue = shapetypeBentConnector4;
                else if ( _T("bentConnector5")			== sValue ) this->m_eValue = shapetypeBentConnector5;
                else if ( _T("bentUpArrow")				== sValue ) this->m_eValue = shapetypeBentUpArrow;
                else if ( _T("bevel")					== sValue ) this->m_eValue = shapetypeBevel;
                else if ( _T("blockArc")				== sValue ) this->m_eValue = shapetypeBlockArc;
                else if ( _T("borderCallout1")			== sValue ) this->m_eValue = shapetypeBorderCallout1;
                else if ( _T("borderCallout2")			== sValue ) this->m_eValue = shapetypeBorderCallout2;
                else if ( _T("borderCallout3")			== sValue ) this->m_eValue = shapetypeBorderCallout3;
                else if ( _T("bracePair")				== sValue ) this->m_eValue = shapetypeBracePair;
                else if ( _T("bracketPair")				== sValue ) this->m_eValue = shapetypeBracketPair;
		/////new
                else if ( _T("Balloon") 				== sValue ) this->m_eValue = shapetypeBallon;
				break;

			case 'c':
                if      ( _T("callout1")				== sValue ) this->m_eValue = shapetypeCallout1;
                else if ( _T("callout2")				== sValue ) this->m_eValue = shapetypeCallout2;
                else if ( _T("callout3")				== sValue ) this->m_eValue = shapetypeCallout3;
                else if ( _T("can")						== sValue ) this->m_eValue = shapetypeCan;
                else if ( _T("chartPlus")				== sValue ) this->m_eValue = shapetypeChartPlus;
                else if ( _T("chartStar")				== sValue ) this->m_eValue = shapetypeChartStar;
                else if ( _T("chartX")					== sValue ) this->m_eValue = shapetypeChartX;
                else if ( _T("chevron")					== sValue ) this->m_eValue = shapetypeChevron;
                else if ( _T("chord")					== sValue ) this->m_eValue = shapetypeChord;
                else if ( _T("circularArrow")			== sValue ) this->m_eValue = shapetypeCircularArrow;
                else if ( _T("cloud")					== sValue ) this->m_eValue = shapetypeCloud;
                else if ( _T("cloudCallout")			== sValue ) this->m_eValue = shapetypeCloudCallout;
                else if ( _T("corner")					== sValue ) this->m_eValue = shapetypeCorner;
                else if ( _T("cornerTabs")				== sValue ) this->m_eValue = shapetypeCornerTabs;
                else if ( _T("cube")					== sValue ) this->m_eValue = shapetypeCube;
                else if ( _T("curvedConnector2")		== sValue ) this->m_eValue = shapetypeCurvedConnector2;
                else if ( _T("curvedConnector3")		== sValue ) this->m_eValue = shapetypeCurvedConnector3;
                else if ( _T("curvedConnector4")		== sValue ) this->m_eValue = shapetypeCurvedConnector4;
                else if ( _T("curvedConnector5")		== sValue ) this->m_eValue = shapetypeCurvedConnector5;
                else if ( _T("curvedDownArrow")			== sValue ) this->m_eValue = shapetypeCurvedDownArrow;
                else if ( _T("curvedLeftArrow")			== sValue ) this->m_eValue = shapetypeCurvedLeftArrow;
                else if ( _T("curvedRightArrow")		== sValue ) this->m_eValue = shapetypeCurvedRightArrow;
                else if ( _T("curvedUpArrow")			== sValue ) this->m_eValue = shapetypeCurvedUpArrow;
				break;

			case 'd':
                if      ( _T("decagon")					== sValue ) this->m_eValue = shapetypeDecagon;
                else if ( _T("diagStripe")				== sValue ) this->m_eValue = shapetypeDiagStripe;
                else if ( _T("diamond")					== sValue ) this->m_eValue = shapetypeDiamond;
                else if ( _T("dodecagon")				== sValue ) this->m_eValue = shapetypeDodecagon;
                else if ( _T("donut")					== sValue ) this->m_eValue = shapetypeDonut;
                else if ( _T("doubleWave")				== sValue ) this->m_eValue = shapetypeDoubleWave;
                else if ( _T("downArrow")				== sValue ) this->m_eValue = shapetypeDownArrow;
                else if ( _T("downArrowCallout")		== sValue ) this->m_eValue = shapetypeDownArrowCallout;
				break;

			case 'e':
                if      ( _T("ellipse")					== sValue ) this->m_eValue = shapetypeEllipse;
                else if ( _T("ellipseRibbon")			== sValue ) this->m_eValue = shapetypeEllipseRibbon;
                else if ( _T("ellipseRibbon2")			== sValue ) this->m_eValue = shapetypeEllipseRibbon2;
				break;

			case 'f':
                if      ( _T("flowChartAlternateProcess")	== sValue ) this->m_eValue = shapetypeFlowChartAlternateProcess;
                else if ( _T("flowChartCollate")			== sValue ) this->m_eValue = shapetypeFlowChartCollate;
                else if ( _T("flowChartConnector")			== sValue ) this->m_eValue = shapetypeFlowChartConnector;
                else if ( _T("flowChartDecision")			== sValue ) this->m_eValue = shapetypeFlowChartDecision;
                else if ( _T("flowChartDelay")				== sValue ) this->m_eValue = shapetypeFlowChartDelay;
                else if ( _T("flowChartDisplay")			== sValue ) this->m_eValue = shapetypeFlowChartDisplay;
                else if ( _T("flowChartDocument")			== sValue ) this->m_eValue = shapetypeFlowChartDocument;
                else if ( _T("flowChartExtract")			== sValue ) this->m_eValue = shapetypeFlowChartExtract;
                else if ( _T("flowChartInputOutput")		== sValue ) this->m_eValue = shapetypeFlowChartInputOutput;
                else if ( _T("flowChartInternalStorage")	== sValue ) this->m_eValue = shapetypeFlowChartInternalStorage;
                else if ( _T("flowChartMagneticDisk")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticDisk;
                else if ( _T("flowChartMagneticDrum")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticDrum;
                else if ( _T("flowChartMagneticTape")		== sValue ) this->m_eValue = shapetypeFlowChartMagneticTape;
                else if ( _T("flowChartManualInput")		== sValue ) this->m_eValue = shapetypeFlowChartManualInput;
                else if ( _T("flowChartManualOperation")	== sValue ) this->m_eValue = shapetypeFlowChartManualOperation;
                else if ( _T("flowChartMerge")				== sValue ) this->m_eValue = shapetypeFlowChartMerge;
                else if ( _T("flowChartMultidocument")		== sValue ) this->m_eValue = shapetypeFlowChartMultidocument;
                else if ( _T("flowChartOfflineStorage")		== sValue ) this->m_eValue = shapetypeFlowChartOfflineStorage;
                else if ( _T("flowChartOffpageConnector")	== sValue ) this->m_eValue = shapetypeFlowChartOffpageConnector;
                else if ( _T("flowChartOnlineStorage")		== sValue ) this->m_eValue = shapetypeFlowChartOnlineStorage;
                else if ( _T("flowChartOr")					== sValue ) this->m_eValue = shapetypeFlowChartOr;
                else if ( _T("flowChartPredefinedProcess")	== sValue ) this->m_eValue = shapetypeFlowChartPredefinedProcess;
                else if ( _T("flowChartPreparation")		== sValue ) this->m_eValue = shapetypeFlowChartPreparation;
                else if ( _T("flowChartProcess")			== sValue ) this->m_eValue = shapetypeFlowChartProcess;
                else if ( _T("flowChartPunchedCard")		== sValue ) this->m_eValue = shapetypeFlowChartPunchedCard;
                else if ( _T("flowChartPunchedTape")		== sValue ) this->m_eValue = shapetypeFlowChartPunchedTape;
                else if ( _T("flowChartSort")				== sValue ) this->m_eValue = shapetypeFlowChartSort;
                else if ( _T("flowChartSummingJunction")	== sValue ) this->m_eValue = shapetypeFlowChartSummingJunction;
                else if ( _T("flowChartTerminator")			== sValue ) this->m_eValue = shapetypeFlowChartTerminator;
                else if ( _T("foldedCorner")				== sValue ) this->m_eValue = shapetypeFoldedCorner;
                else if ( _T("frame")						== sValue ) this->m_eValue = shapetypeFrame;
                else if ( _T("funnel")						== sValue ) this->m_eValue = shapetypeFunnel;
				break;

			case 'g':
                if      ( _T("gear6")					== sValue ) this->m_eValue = shapetypeGear6;
                else if ( _T("gear9")					== sValue ) this->m_eValue = shapetypeGear9;
				break;

			case 'h':
                if      ( _T("halfFrame")				== sValue ) this->m_eValue = shapetypeHalfFrame;
                else if ( _T("heart")					== sValue ) this->m_eValue = shapetypeHeart;
                else if ( _T("heptagon")				== sValue ) this->m_eValue = shapetypeHeptagon;
                else if ( _T("hexagon")					== sValue ) this->m_eValue = shapetypeHexagon;
                else if ( _T("homePlate")				== sValue ) this->m_eValue = shapetypeHomePlate;
                else if ( _T("horizontalScroll")		== sValue ) this->m_eValue = shapetypeHorizontalScroll;
				break;

			case 'i':
                if      ( _T("irregularSeal1")			== sValue ) this->m_eValue = shapetypeIrregularSeal1;
                else if ( _T("irregularSeal2")			== sValue ) this->m_eValue = shapetypeIrregularSeal2;
				break;

			case 'l':
                if      ( _T("leftArrow")				== sValue ) this->m_eValue = shapetypeLeftArrow;
                else if ( _T("leftArrowCallout")		== sValue ) this->m_eValue = shapetypeLeftArrowCallout;
                else if ( _T("leftBrace")				== sValue ) this->m_eValue = shapetypeLeftBrace;
                else if ( _T("leftBracket")				== sValue ) this->m_eValue = shapetypeLeftBracket;
                else if ( _T("leftCircularArrow")		== sValue ) this->m_eValue = shapetypeLeftCircularArrow;
                else if ( _T("leftRightArrow")			== sValue ) this->m_eValue = shapetypeLeftRightArrow;
                else if ( _T("leftRightArrowCallout")	== sValue ) this->m_eValue = shapetypeLeftRightArrowCallout;
                else if ( _T("leftRightCircularArrow")	== sValue ) this->m_eValue = shapetypeLeftRightCircularArrow;
                else if ( _T("leftRightRibbon")			== sValue ) this->m_eValue = shapetypeLeftRightRibbon;
                else if ( _T("leftRightUpArrow")		== sValue ) this->m_eValue = shapetypeLeftRightUpArrow;
                else if ( _T("leftUpArrow")				== sValue ) this->m_eValue = shapetypeLeftUpArrow;
                else if ( _T("lightningBolt")			== sValue ) this->m_eValue = shapetypeLightningBolt;
                else if ( _T("line")					== sValue ) this->m_eValue = shapetypeLine;
                else if ( _T("lineInv")					== sValue ) this->m_eValue = shapetypeLineInv;
				break;

			case 'm':
                if      ( _T("mathDivide")				== sValue ) this->m_eValue = shapetypeMathDivide;
                else if ( _T("mathEqual")				== sValue ) this->m_eValue = shapetypeMathEqual;
                else if ( _T("mathMinus")				== sValue ) this->m_eValue = shapetypeMathMinus;
                else if ( _T("mathMultiply")			== sValue ) this->m_eValue = shapetypeMathMultiply;
                else if ( _T("mathNotEqual")			== sValue ) this->m_eValue = shapetypeMathNotEqual;
                else if ( _T("mathPlus")				== sValue ) this->m_eValue = shapetypeMathPlus;
                else if ( _T("moon")					== sValue ) this->m_eValue = shapetypeMoon;
				break;

			case 'n':
                if      ( _T("nonIsoscelesTrapezoid")	== sValue ) this->m_eValue = shapetypeNonIsoscelesTrapezoid;
                else if ( _T("noSmoking")				== sValue ) this->m_eValue = shapetypeNoSmoking;
                else if ( _T("notchedRightArrow")		== sValue ) this->m_eValue = shapetypeNotchedRightArrow;
				break;

			case 'o':
                if      ( _T("octagon")					== sValue ) this->m_eValue = shapetypeOctagon;
				break;

			case 'p':
                if      ( _T("parallelogram")			== sValue ) this->m_eValue = shapetypeParallelogram;
                else if ( _T("pentagon")				== sValue ) this->m_eValue = shapetypePentagon;
                else if ( _T("pie")						== sValue ) this->m_eValue = shapetypePie;
                else if ( _T("pieWedge")				== sValue ) this->m_eValue = shapetypePieWedge;
                else if ( _T("plaque")					== sValue ) this->m_eValue = shapetypePlaque;
                else if ( _T("plaqueTabs")				== sValue ) this->m_eValue = shapetypePlaqueTabs;
                else if ( _T("plus")					== sValue ) this->m_eValue = shapetypePlus;
				break;

			case 'q':
                if      ( _T("quadArrow")				== sValue ) this->m_eValue = shapetypeQuadArrow;
                else if ( _T("quadArrowCallout")		== sValue ) this->m_eValue = shapetypeQuadArrowCallout;
				break;

			case 'r':
                if      ( _T("rect")					== sValue ) this->m_eValue = shapetypeRect;
                else if ( _T("ribbon")					== sValue ) this->m_eValue = shapetypeRibbon;
                else if ( _T("ribbon2")					== sValue ) this->m_eValue = shapetypeRibbon2;
                else if ( _T("rightArrow")				== sValue ) this->m_eValue = shapetypeRightArrow;
                else if ( _T("rightArrowCallout")		== sValue ) this->m_eValue = shapetypeRightArrowCallout;
                else if ( _T("rightBrace")				== sValue ) this->m_eValue = shapetypeRightBrace;
                else if ( _T("rightBracket")			== sValue ) this->m_eValue = shapetypeRightBracket;
                else if ( _T("round1Rect")				== sValue ) this->m_eValue = shapetypeRound1Rect;
                else if ( _T("round2DiagRect")			== sValue ) this->m_eValue = shapetypeRound2DiagRect;
                else if ( _T("round2SameRect")			== sValue ) this->m_eValue = shapetypeRound2SameRect;
                else if ( _T("roundRect")				== sValue ) this->m_eValue = shapetypeRoundRect;
                else if ( _T("rtTriangle")				== sValue ) this->m_eValue = shapetypeRtTriangle;
		////new
                else if ( _T("rightUpArrow")			== sValue ) this->m_eValue = 	 shapetypeRightUpArrow;
				break;

			case 's':
                if      ( _T("smileyFace")				== sValue ) this->m_eValue = shapetypeSmileyFace;
                else if ( _T("snip1Rect")				== sValue ) this->m_eValue = shapetypeSnip1Rect;
                else if ( _T("snip2DiagRect")			== sValue ) this->m_eValue = shapetypeSnip2DiagRect;
                else if ( _T("snip2SameRect")			== sValue ) this->m_eValue = shapetypeSnip2SameRect;
                else if ( _T("snipRoundRect")			== sValue ) this->m_eValue = shapetypeSnipRoundRect;
                else if ( _T("squareTabs")				== sValue ) this->m_eValue = shapetypeSquareTabs;
                else if ( _T("star10")					== sValue ) this->m_eValue = shapetypeStar10;
                else if ( _T("star12")					== sValue ) this->m_eValue = shapetypeStar12;
                else if ( _T("star16")					== sValue ) this->m_eValue = shapetypeStar16;
                else if ( _T("star24")					== sValue ) this->m_eValue = shapetypeStar24;
                else if ( _T("star32")					== sValue ) this->m_eValue = shapetypeStar32;
                else if ( _T("star4")					== sValue ) this->m_eValue = shapetypeStar4;
                else if ( _T("star5")					== sValue ) this->m_eValue = shapetypeStar5;
                else if ( _T("star6")					== sValue ) this->m_eValue = shapetypeStar6;
                else if ( _T("star7")					== sValue ) this->m_eValue = shapetypeStar7;
                else if ( _T("star8")					== sValue ) this->m_eValue = shapetypeStar8;
                else if ( _T("straightConnector1")		== sValue ) this->m_eValue = shapetypeStraightConnector1;
                else if ( _T("stripedRightArrow")		== sValue ) this->m_eValue = shapetypeStripedRightArrow;
                else if ( _T("sun")						== sValue ) this->m_eValue = shapetypeSun;
                else if ( _T("swooshArrow")				== sValue ) this->m_eValue = shapetypeSwooshArrow;
				break;

			case 't':
                if      ( _T("teardrop")				== sValue ) this->m_eValue = shapetypeTeardrop;
                else if ( _T("trapezoid")				== sValue ) this->m_eValue = shapetypeTrapezoid;
                else if ( _T("triangle")				== sValue ) this->m_eValue = shapetypeTriangle;
		/////new
                else if ( _T("textArchDownPour")		== sValue ) this->m_eValue = 	 shapetypeTextArchDownPour;
                else if ( _T("textArchUpPour")			== sValue ) this->m_eValue = 	 shapetypeTextArchUpPour;
                else if ( _T("textCanDown")				== sValue ) this->m_eValue = 	 shapetypeTextCanDown;
                else if ( _T("textCanUp")				== sValue ) this->m_eValue = 	 shapetypeTextCanUp;
                else if ( _T("textCirclePour")			== sValue ) this->m_eValue = 	 shapetypeTextCirclePour;
                else if ( _T("textCurveDown")			== sValue ) this->m_eValue = 	 shapetypeTextCurveDown;
                else if ( _T("textCurveUp")				== sValue ) this->m_eValue = 	 shapetypeTextCurveUp;
                else if ( _T("textDeflate")				== sValue ) this->m_eValue = 	 shapetypeTextDeflate;
                else if ( _T("textDeflateBottom")		== sValue ) this->m_eValue = 	 shapetypeTextDeflateBottom;
                else if ( _T("textDeflateInflate")		== sValue ) this->m_eValue = 	 shapetypeTextDeflateInflate;
                else if ( _T("textDeflateInflateDeflate")	== sValue ) this->m_eValue = 	 shapetypeTextDeflateInflateDeflat;
                else if ( _T("textDeflateTop")			== sValue ) this->m_eValue = 	 shapetypeTextDeflateTop;
                else if ( _T("textDoubleWave1")			== sValue ) this->m_eValue = 	 shapetypeTextDoubleWave1;
                else if ( _T("textFadeDown")			== sValue ) this->m_eValue = 	 shapetypeTextFadeDown;
                else if ( _T("textFadeLeft")			== sValue ) this->m_eValue = 	 shapetypeTextFadeLeft;
                else if ( _T("textFadeRight")			== sValue ) this->m_eValue = 	 shapetypeTextFadeRight;
                else if ( _T("textFadeUp")				== sValue ) this->m_eValue = 	 shapetypeTextFadeUp;
                else if ( _T("textInflateBottom")		== sValue ) this->m_eValue = 	 shapetypeTextInflateBottom;
                else if ( _T("textInflateTop")			== sValue ) this->m_eValue = 	 shapetypeTextInflateTop;
                else if ( _T("textRingInside")			== sValue ) this->m_eValue = 	 shapetypeTextRingInside;
                else if ( _T("textRingOutside")			== sValue ) this->m_eValue = 	 shapetypeTextRingOutside;
                else if ( _T("textWave1")				== sValue ) this->m_eValue = 	 shapetypeTextWave1;
                else if ( _T("textWave2")				== sValue ) this->m_eValue = 	 shapetypeTextWave2;
                else if ( _T("textWave4")				== sValue ) this->m_eValue = 	 shapetypeTextWave4;
                else if ( _T("thickArrow")				== sValue ) this->m_eValue = 	 shapetypeThickArrow;
				break;
			case 'u':
                if      ( _T("upArrow")					== sValue ) this->m_eValue = shapetypeUpArrow;
                else if ( _T("upArrowCallout")			== sValue ) this->m_eValue = shapetypeUpArrowCallout;
                else if ( _T("upDownArrow")				== sValue ) this->m_eValue = shapetypeUpDownArrow;
                else if ( _T("upDownArrowCallout")		== sValue ) this->m_eValue = shapetypeUpDownArrowCallout;
                else if ( _T("uturnArrow")				== sValue ) this->m_eValue = shapetypeUturnArrow;
				break;

			case 'v':
                if      ( _T("verticalScroll")			== sValue ) this->m_eValue = shapetypeVerticalScroll;
				break;

			case 'w':
                if      ( _T("wave")					== sValue ) this->m_eValue = shapetypeWave;
                else if ( _T("wedgeEllipseCallout")		== sValue ) this->m_eValue = shapetypeWedgeEllipseCallout;
                else if ( _T("wedgeRectCallout")		== sValue ) this->m_eValue = shapetypeWedgeRectCallout;
                else if ( _T("wedgeRoundRectCallout")	== sValue ) this->m_eValue = shapetypeWedgeRoundRectCallout;
				break;
			}

            return this->m_eValue;
		}

		virtual CString    ToString  () const 
		{
            switch(this->m_eValue)
			{
			case shapetypeAccentBorderCallout1:			return _T("accentBorderCallout1");
			case shapetypeAccentBorderCallout2:			return _T("accentBorderCallout2");
			case shapetypeAccentBorderCallout3:			return _T("accentBorderCallout3");
			case shapetypeAccentCallout1:				return _T("accentCallout1");
			case shapetypeAccentCallout2:				return _T("accentCallout2");
			case shapetypeAccentCallout3:				return _T("accentCallout3");
			case shapetypeActionButtonBackPrevious:		return _T("actionButtonBackPrevious");
			case shapetypeActionButtonBeginning:		return _T("actionButtonBeginning");
			case shapetypeActionButtonBlank:			return _T("actionButtonBlank");
			case shapetypeActionButtonDocument:			return _T("actionButtonDocument");
			case shapetypeActionButtonEnd:				return _T("actionButtonEnd");
			case shapetypeActionButtonForwardNext:		return _T("actionButtonForwardNext");
			case shapetypeActionButtonHelp:				return _T("actionButtonHelp");
			case shapetypeActionButtonHome:				return _T("actionButtonHome");
			case shapetypeActionButtonInformation:		return _T("actionButtonInformation");
			case shapetypeActionButtonMovie:			return _T("actionButtonMovie");
			case shapetypeActionButtonReturn:			return _T("actionButtonReturn");
			case shapetypeActionButtonSound:			return _T("actionButtonSound");
			case shapetypeArc:							return _T("arc");
			case shapetypeBentArrow:					return _T("bentArrow");
			case shapetypeBentConnector2:				return _T("bentConnector2");
			case shapetypeBentConnector3:				return _T("bentConnector3");
			case shapetypeBentConnector4:				return _T("bentConnector4");
			case shapetypeBentConnector5:				return _T("bentConnector5");
			case shapetypeBentUpArrow:					return _T("bentUpArrow");
			case shapetypeBevel:						return _T("bevel");
			case shapetypeBlockArc:						return _T("blockArc");
			case shapetypeBorderCallout1:				return _T("borderCallout1");
			case shapetypeBorderCallout2:				return _T("borderCallout2");
			case shapetypeBorderCallout3:				return _T("borderCallout3");
			case shapetypeBracePair:					return _T("bracePair");
			case shapetypeBracketPair:					return _T("bracketPair");
			case shapetypeCallout1:						return _T("callout1");
			case shapetypeCallout2:						return _T("callout2");
			case shapetypeCallout3:						return _T("callout3");
			case shapetypeCan:							return _T("can");
			case shapetypeChartPlus:					return _T("chartPlus");
			case shapetypeChartStar:					return _T("chartStar");
			case shapetypeChartX:						return _T("chartX");
			case shapetypeChevron:						return _T("chevron");
			case shapetypeChord:						return _T("chord");
			case shapetypeCircularArrow:				return _T("circularArrow");
			case shapetypeCloud:						return _T("cloud");
			case shapetypeCloudCallout:					return _T("cloudCallout");
			case shapetypeCorner:						return _T("corner");
			case shapetypeCornerTabs:					return _T("cornerTabs");
			case shapetypeCube:							return _T("cube");
			case shapetypeCurvedConnector2:				return _T("curvedConnector2");
			case shapetypeCurvedConnector3: 			return _T("curvedConnector3");
			case shapetypeCurvedConnector4: 			return _T("curvedConnector4");
			case shapetypeCurvedConnector5: 			return _T("curvedConnector5");
			case shapetypeCurvedDownArrow:				return _T("curvedDownArrow");
			case shapetypeCurvedLeftArrow:				return _T("curvedLeftArrow");
			case shapetypeCurvedRightArrow:				return _T("curvedRightArrow");
			case shapetypeCurvedUpArrow:				return _T("curvedUpArrow");
			case shapetypeDecagon:						return _T("decagon");
			case shapetypeDiagStripe:					return _T("diagStripe");
			case shapetypeDiamond:						return _T("diamond");
			case shapetypeDodecagon:					return _T("dodecagon");
			case shapetypeDonut:						return _T("donut");
			case shapetypeDoubleWave:					return _T("doubleWave");
			case shapetypeDownArrow:					return _T("downArrow");
			case shapetypeDownArrowCallout:				return _T("downArrowCallout");
			case shapetypeEllipse:						return _T("ellipse");
			case shapetypeEllipseRibbon:				return _T("ellipseRibbon");
			case shapetypeEllipseRibbon2:				return _T("ellipseRibbon2");
			case shapetypeFlowChartAlternateProcess:	return _T("flowChartAlternateProcess");
			case shapetypeFlowChartCollate:				return _T("flowChartCollate");
			case shapetypeFlowChartConnector:			return _T("flowChartConnector");
			case shapetypeFlowChartDecision:			return _T("flowChartDecision");
			case shapetypeFlowChartDelay:				return _T("flowChartDelay");
			case shapetypeFlowChartDisplay:				return _T("flowChartDisplay");
			case shapetypeFlowChartDocument:			return _T("flowChartDocument");
			case shapetypeFlowChartExtract:				return _T("flowChartExtract");
			case shapetypeFlowChartInputOutput:			return _T("flowChartInputOutput");
			case shapetypeFlowChartInternalStorage:		return _T("flowChartInternalStorage");
			case shapetypeFlowChartMagneticDisk:		return _T("flowChartMagneticDisk");
			case shapetypeFlowChartMagneticDrum:		return _T("flowChartMagneticDrum");
			case shapetypeFlowChartMagneticTape:		return _T("flowChartMagneticTape");
			case shapetypeFlowChartManualInput:			return _T("flowChartManualInput");
			case shapetypeFlowChartManualOperation:		return _T("flowChartManualOperation");
			case shapetypeFlowChartMerge:				return _T("flowChartMerge");
			case shapetypeFlowChartMultidocument:		return _T("flowChartMultidocument");
			case shapetypeFlowChartOfflineStorage:		return _T("flowChartOfflineStorage");
			case shapetypeFlowChartOffpageConnector:	return _T("flowChartOffpageConnector");
			case shapetypeFlowChartOnlineStorage:		return _T("flowChartOnlineStorage");
			case shapetypeFlowChartOr:					return _T("flowChartOr");
			case shapetypeFlowChartPredefinedProcess:	return _T("flowChartPredefinedProcess");
			case shapetypeFlowChartPreparation:			return _T("flowChartPreparation");
			case shapetypeFlowChartProcess:				return _T("flowChartProcess");
			case shapetypeFlowChartPunchedCard:			return _T("flowChartPunchedCard");
			case shapetypeFlowChartPunchedTape:			return _T("flowChartPunchedTape");
			case shapetypeFlowChartSort:				return _T("flowChartSort");
			case shapetypeFlowChartSummingJunction:		return _T("flowChartSummingJunction");
			case shapetypeFlowChartTerminator:			return _T("flowChartTerminator");
			case shapetypeFoldedCorner:					return _T("foldedCorner");
			case shapetypeFrame:						return _T("frame");
			case shapetypeFunnel:						return _T("funnel");
			case shapetypeGear6:						return _T("gear6");
			case shapetypeGear9:						return _T("gear9");
			case shapetypeHalfFrame:					return _T("halfFrame");
			case shapetypeHeart:						return _T("heart");
			case shapetypeHeptagon:						return _T("heptagon");
			case shapetypeHexagon:						return _T("hexagon");
			case shapetypeHomePlate:					return _T("homePlate");
			case shapetypeHorizontalScroll:				return _T("horizontalScroll");
			case shapetypeIrregularSeal1:				return _T("irregularSeal1");
			case shapetypeIrregularSeal2:				return _T("irregularSeal2");
			case shapetypeLeftArrow:					return _T("leftArrow");
			case shapetypeLeftArrowCallout:				return _T("leftArrowCallout");
			case shapetypeLeftBrace:					return _T("leftBrace");
			case shapetypeLeftBracket:					return _T("leftBracket");
			case shapetypeLeftCircularArrow:			return _T("leftCircularArrow");
			case shapetypeLeftRightArrow:				return _T("leftRightArrow");
			case shapetypeLeftRightArrowCallout:		return _T("leftRightArrowCallout");
			case shapetypeLeftRightCircularArrow:		return _T("leftRightCircularArrow");
			case shapetypeLeftRightRibbon:				return _T("leftRightRibbon");
			case shapetypeLeftRightUpArrow:				return _T("leftRightUpArrow");
			case shapetypeLeftUpArrow:					return _T("leftUpArrow");
			case shapetypeLightningBolt:				return _T("lightningBolt");
			case shapetypeLine:							return _T("line");
			case shapetypeLineInv:						return _T("lineInv");
			case shapetypeMathDivide:					return _T("mathDivide");
			case shapetypeMathEqual:					return _T("mathEqual");
			case shapetypeMathMinus:					return _T("mathMinus");
			case shapetypeMathMultiply:					return _T("mathMultiply");
			case shapetypeMathNotEqual:					return _T("mathNotEqual");
			case shapetypeMathPlus:						return _T("mathPlus");
			case shapetypeMoon:							return _T("moon");
			case shapetypeNonIsoscelesTrapezoid:		return _T("nonIsoscelesTrapezoid");
			case shapetypeNoSmoking:					return _T("noSmoking");
			case shapetypeNotchedRightArrow:			return _T("notchedRightArrow");
			case shapetypeOctagon:						return _T("octagon");
			case shapetypeParallelogram:				return _T("parallelogram");
			case shapetypePentagon:						return _T("pentagon");
			case shapetypePie:							return _T("pie");
			case shapetypePieWedge:						return _T("pieWedge");
			case shapetypePlaque:						return _T("plaque");
			case shapetypePlaqueTabs:					return _T("plaqueTabs");
			case shapetypePlus:							return _T("plus");
			case shapetypeQuadArrow:					return _T("quadArrow");
			case shapetypeQuadArrowCallout:				return _T("quadArrowCallout");
			case shapetypeRect:							return _T("rect");
			case shapetypeRibbon:						return _T("ribbon");
			case shapetypeRibbon2:						return _T("ribbon2");
			case shapetypeRightArrow:					return _T("rightArrow");
			case shapetypeRightArrowCallout:			return _T("rightArrowCallout");
			case shapetypeRightBrace:					return _T("rightBrace");
			case shapetypeRightBracket:					return _T("rightBracket");
			case shapetypeRound1Rect:					return _T("round1Rect");
			case shapetypeRound2DiagRect:				return _T("round2DiagRect");
			case shapetypeRound2SameRect:				return _T("round2SameRect");
			case shapetypeRoundRect:					return _T("roundRect");
			case shapetypeRtTriangle:					return _T("rtTriangle");
			case shapetypeSmileyFace:					return _T("smileyFace");
			case shapetypeSnip1Rect:					return _T("snip1Rect");
			case shapetypeSnip2DiagRect:				return _T("snip2DiagRect");
			case shapetypeSnip2SameRect:				return _T("snip2SameRect");
			case shapetypeSnipRoundRect:				return _T("snipRoundRect");
			case shapetypeSquareTabs:					return _T("squareTabs");
			case shapetypeStar10:						return _T("star10");
			case shapetypeStar12:						return _T("star12");
			case shapetypeStar16:						return _T("star16");
			case shapetypeStar24:						return _T("star24");
			case shapetypeStar32:						return _T("star32");
			case shapetypeStar4:						return _T("star4");
			case shapetypeStar5:						return _T("star5");
			case shapetypeStar6:						return _T("star6");
			case shapetypeStar7:						return _T("star7");
			case shapetypeStar8:						return _T("star8");
			case shapetypeStraightConnector1:			return _T("straightConnector1");
			case shapetypeStripedRightArrow:			return _T("stripedRightArrow");
			case shapetypeSun:							return _T("sun");
			case shapetypeSwooshArrow:					return _T("swooshArrow");
			case shapetypeTeardrop:						return _T("teardrop");
			case shapetypeTrapezoid:					return _T("trapezoid");
			case shapetypeTriangle:						return _T("triangle");
			case shapetypeUpArrow:						return _T("upArrow");
			case shapetypeUpArrowCallout:				return _T("upArrowCallout");
			case shapetypeUpDownArrow:					return _T("upDownArrow");
			case shapetypeUpDownArrowCallout:			return _T("upDownArrowCallout");
			case shapetypeUturnArrow:					return _T("uturnArrow");
			case shapetypeVerticalScroll:				return _T("verticalScroll");
			case shapetypeWave:							return _T("wave");
			case shapetypeWedgeEllipseCallout:			return _T("wedgeEllipseCallout");
			case shapetypeWedgeRectCallout:				return _T("wedgeRectCallout");
			case shapetypeWedgeRoundRectCallout:		return _T("wedgeRoundRectCallout");	
	///new//
			case shapetypeBallon:						return _T("Balloon"); 
			case shapetypeRightUpArrow:					return _T("rightUpArrow");
			case shapetypeTextArchDownPour:				return _T("textArchDownPour");
			case shapetypeTextArchUpPour:				return _T("textArchUpPour");
			case shapetypeTextCanDown:					return _T("textCanDown");
			case shapetypeTextCanUp:					return _T("textCanUp");
			case shapetypeTextCirclePour:				return _T("textCirclePour");
			case shapetypeTextCurveDown:				return _T("textCurveDown");
			case shapetypeTextCurveUp:					return _T("textCurveUp");
			case shapetypeTextDeflate:					return _T("textDeflate");
			case shapetypeTextDeflateBottom:			return _T("textDeflateBottom");
			case shapetypeTextDeflateInflate:			return _T("textDeflateInflate");
			case shapetypeTextDeflateInflateDeflat:		return _T("textDeflateInflateDeflate");
			case shapetypeTextDeflateTop:				return _T("textDeflateTop");
			case shapetypeTextDoubleWave1:				return _T("textDoubleWave1");
			case shapetypeTextFadeDown:					return _T("textFadeDown");
			case shapetypeTextFadeLeft:					return _T("textFadeLeft");
			case shapetypeTextFadeRight:				return _T("textFadeRight");
			case shapetypeTextFadeUp:					return _T("textFadeUp");
			case shapetypeTextInflateBottom:			return _T("textInflateBottom");
			case shapetypeTextInflateTop:				return _T("textInflateTop");
			case shapetypeTextRingInside:				return _T("textRingInside");
			case shapetypeTextRingOutside:				return _T("textRingOutside");
			case shapetypeTextWave1:					return _T("textWave1");
			case shapetypeTextWave2:					return _T("textWave2");
			case shapetypeTextWave4:					return _T("textWave4");
			case shapetypeThickArrow:					return _T("thickArrow"); 

			default :									return _T("rect");
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
	// SystemColorVal 20.1.10.58 (Part 1)
	//--------------------------------------------------------------------------------		

    // from winuser.h ( http://msdn.microsoft.com/en-us/library/windows/desktop/ms724371(v=vs.85).aspx )
    #ifndef COLOR_3DDKSHADOW
    #define COLOR_3DDKSHADOW            21
    #endif

    #ifndef COLOR_3DFACE
    #define COLOR_3DFACE                15
    #endif

    #ifndef COLOR_3DHIGHLIGHT
    #define COLOR_3DHIGHLIGHT           20
    #endif

    #ifndef COLOR_3DHILIGHT
    #define COLOR_3DHILIGHT             20
    #endif

    #ifndef COLOR_3DLIGHT
    #define COLOR_3DLIGHT               22
    #endif

    #ifndef COLOR_3DSHADOW
    #define COLOR_3DSHADOW              16
    #endif

    #ifndef COLOR_ACTIVEBORDER
    #define COLOR_ACTIVEBORDER          10
    #endif

    #ifndef COLOR_ACTIVECAPTION
    #define COLOR_ACTIVECAPTION         2
    #endif

    #ifndef COLOR_APPWORKSPACE
    #define COLOR_APPWORKSPACE          12
    #endif

    #ifndef COLOR_BACKGROUND
    #define COLOR_BACKGROUND            1
    #endif

    #ifndef COLOR_BTNFACE
    #define COLOR_BTNFACE               15
    #endif

    #ifndef COLOR_BTNHIGHLIGHT
    #define COLOR_BTNHIGHLIGHT          20
    #endif

    #ifndef COLOR_BTNHILIGHT
    #define COLOR_BTNHILIGHT            20
    #endif

    #ifndef COLOR_BTNSHADOW
    #define COLOR_BTNSHADOW             16
    #endif

    #ifndef COLOR_BTNTEXT
    #define COLOR_BTNTEXT               18
    #endif

    #ifndef COLOR_CAPTIONTEXT
    #define COLOR_CAPTIONTEXT           9
    #endif

    #ifndef COLOR_DESKTOP
    #define COLOR_DESKTOP               1
    #endif

    #ifndef COLOR_GRAYTEXT
    #define COLOR_GRAYTEXT              17
    #endif

    #ifndef COLOR_HIGHLIGHT
    #define COLOR_HIGHLIGHT             13
    #endif

    #ifndef COLOR_HIGHLIGHTTEXT
    #define COLOR_HIGHLIGHTTEXT        14
    #endif

	#ifndef COLOR_HOTLIGHT
    #define COLOR_HOTLIGHT              26
	#endif

    #ifndef COLOR_INACTIVEBORDER
    #define COLOR_INACTIVEBORDER        11
    #endif

    #ifndef COLOR_INACTIVECAPTION
    #define COLOR_INACTIVECAPTION        3
    #endif


    #ifndef COLOR_INACTIVECAPTIONTEXT
    #define COLOR_INACTIVECAPTIONTEXT    19
    #endif

    #ifndef COLOR_INFOBK
    #define COLOR_INFOBK                24
    #endif

    #ifndef COLOR_INFOTEXT
    #define COLOR_INFOTEXT              23
    #endif

    #ifndef COLOR_MENU
    #define COLOR_MENU                  4
    #endif

	#ifndef COLOR_GRADIENTACTIVECAPTION
	#define COLOR_GRADIENTACTIVECAPTION 27
	#endif

	#ifndef COLOR_GRADIENTINACTIVECAPTION
	#define COLOR_GRADIENTINACTIVECAPTION 28
	#endif

	#ifndef COLOR_MENUHILIGHT
	#define COLOR_MENUHILIGHT       29
	#endif

	#ifndef COLOR_MENUBAR
	#define COLOR_MENUBAR           30
	#endif


    #ifndef COLOR_MENUTEXT
    #define COLOR_MENUTEXT          7
    #endif

    #ifndef COLOR_SCROLLBAR
    #define COLOR_SCROLLBAR         0
    #endif

    #ifndef COLOR_WINDOW
    #define COLOR_WINDOW            5
    #endif

    #ifndef COLOR_WINDOWFRAME
    #define COLOR_WINDOWFRAME       6
    #endif

    #ifndef COLOR_WINDOWTEXT
    #define COLOR_WINDOWTEXT        8
    #endif


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

		virtual ESystemColorVal FromString(CString &sValue)
		{
            this->m_eValue = systemcolorvalWindow;
			SetRGBA( 0, 0, 0, 255 );

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case '3':
                if      ( _T("3dDkShadow")              == sValue ) { this->m_eValue = systemcolorval3dDkShadow; SetRGBASys( COLOR_3DDKSHADOW ); }
                else if ( _T("3dLight")                 == sValue ) { this->m_eValue = systemcolorval3dLight; SetRGBASys( COLOR_3DLIGHT ); }
				break;
			case 'a':
                if      ( _T("activeBorder")            == sValue ) { this->m_eValue = systemcolorvalActiveBorder; SetRGBASys( COLOR_ACTIVEBORDER ); }
                else if ( _T("activeCaption")           == sValue ) { this->m_eValue = systemcolorvalActiveCaption; SetRGBASys( COLOR_ACTIVECAPTION ); }
                else if ( _T("appWorkspace")            == sValue ) { this->m_eValue = systemcolorvalAppWorkspace; SetRGBASys( COLOR_APPWORKSPACE ); }
				break;
			case 'b':
                if      ( _T("background")              == sValue ) { this->m_eValue = systemcolorvalBackground; SetRGBASys( COLOR_BACKGROUND ); }
                else if ( _T("btnFace")                 == sValue ) { this->m_eValue = systemcolorvalBtnFace ; SetRGBASys( COLOR_BTNFACE ); }
                else if ( _T("btnHighlight")            == sValue ) { this->m_eValue = systemcolorvalBtnHighlight ; SetRGBASys( COLOR_BTNHIGHLIGHT ); }
                else if ( _T("btnShadow")               == sValue ) { this->m_eValue = systemcolorvalBtnShadow ; SetRGBASys( COLOR_BTNSHADOW ); }
                else if ( _T("btnText")                 == sValue ) { this->m_eValue = systemcolorvalBtnText ; SetRGBASys( COLOR_BTNTEXT ); }
				break;
			case 'c':
                if      ( _T("captionText")             == sValue ) { this->m_eValue = systemcolorvalCaptionText ; SetRGBASys( COLOR_CAPTIONTEXT ); }
				break;
			case 'g':
                if      ( _T("gradientActiveCaption")   == sValue ) { this->m_eValue = systemcolorvalGradientActiveCaption ; SetRGBASys( COLOR_GRADIENTACTIVECAPTION ); }
                else if ( _T("gradientInactiveCaption") == sValue ) { this->m_eValue = systemcolorvalGradientInactiveCaption ; SetRGBASys( COLOR_GRADIENTINACTIVECAPTION ); }
                else if ( _T("grayText")                == sValue ) { this->m_eValue = systemcolorvalGrayText ; SetRGBASys( COLOR_GRAYTEXT ); }
				break;
			case 'h':
                if      ( _T("highlight")               == sValue ) { this->m_eValue = systemcolorvalHighlight ; SetRGBASys( COLOR_HIGHLIGHT ); }
                else if ( _T("highlightText")           == sValue ) { this->m_eValue = systemcolorvalHighlightText ; SetRGBASys( COLOR_HIGHLIGHTTEXT ); }
                else if ( _T("hotLight")                == sValue ) { this->m_eValue = systemcolorvalHotLight ; SetRGBASys( COLOR_HOTLIGHT ); }
				break;
			case 'i':
                if      ( _T("inactiveBorder")          == sValue ) { this->m_eValue = systemcolorvalInactiveBorder ; SetRGBASys( COLOR_INACTIVEBORDER ); }
                else if ( _T("inactiveCaption")         == sValue ) { this->m_eValue = systemcolorvalInactiveCaption ; SetRGBASys( COLOR_INACTIVECAPTION ); }
                else if ( _T("inactiveCaptionText")     == sValue ) { this->m_eValue = systemcolorvalInactiveCaptionText ; SetRGBASys( COLOR_INACTIVECAPTIONTEXT ); }
                else if ( _T("infoBk")                  == sValue ) { this->m_eValue = systemcolorvalInfoBk ; SetRGBASys( COLOR_INFOBK ); }
                else if ( _T("infoText")                == sValue ) { this->m_eValue = systemcolorvalInfoText ; SetRGBASys( COLOR_INFOTEXT ); }
				break;
			case 'm':
                if      ( _T("menu")                    == sValue ) { this->m_eValue = systemcolorvalMenu ; SetRGBASys( COLOR_MENU ); }
                else if ( _T("menuBar")                 == sValue ) { this->m_eValue = systemcolorvalMenuBar ; SetRGBASys( COLOR_MENUBAR ); }
                else if ( _T("menuHighlight")           == sValue ) { this->m_eValue = systemcolorvalMenuHighlight ; SetRGBASys( COLOR_MENUHILIGHT ); }
                else if ( _T("menuText")                == sValue ) { this->m_eValue = systemcolorvalMenuText ; SetRGBASys( COLOR_MENUTEXT ); }
				break;
			case 's':
                if      ( _T("scrollBar")               == sValue ) { this->m_eValue = systemcolorvalScrollBar ; SetRGBASys( COLOR_SCROLLBAR ); }
				break;
			case 'w':
                if      ( _T("window")                  == sValue ) { this->m_eValue = systemcolorvalWindow ; SetRGBASys( COLOR_WINDOW ); }
                else if ( _T("windowFrame")             == sValue ) { this->m_eValue = systemcolorvalWindowFrame ; SetRGBASys( COLOR_WINDOWFRAME ); }
                else if ( _T("windowText")              == sValue ) { this->m_eValue = systemcolorvalWindowText ; SetRGBASys( COLOR_WINDOWTEXT ); }
				break;
			}

            return this->m_eValue;
		}

		virtual CString         ToString  () const 
		{
            switch(this->m_eValue)
			{
			case systemcolorval3dDkShadow:              return _T("3dDkShadow");
			case systemcolorval3dLight:                 return _T("3dLight");
			case systemcolorvalActiveBorder:            return _T("activeBorder");
			case systemcolorvalActiveCaption:           return _T("activeCaption");
			case systemcolorvalAppWorkspace:            return _T("appWorkspace");
			case systemcolorvalBackground:              return _T("background");
			case systemcolorvalBtnFace:                 return _T("btnFace");
			case systemcolorvalBtnHighlight:            return _T("btnHighlight");
			case systemcolorvalBtnShadow:               return _T("btnShadow");
			case systemcolorvalBtnText:                 return _T("btnText");
			case systemcolorvalCaptionText:             return _T("captionText");
			case systemcolorvalGradientActiveCaption:   return _T("gradientActiveCaption");
			case systemcolorvalGradientInactiveCaption: return _T("gradientInactiveCaption");
			case systemcolorvalGrayText:                return _T("grayText");
			case systemcolorvalHighlight:               return _T("highlight");
			case systemcolorvalHighlightText:           return _T("highlightText");
			case systemcolorvalHotLight:                return _T("hotLight");
			case systemcolorvalInactiveBorder:          return _T("inactiveBorder");
			case systemcolorvalInactiveCaption:         return _T("inactiveCaption");
			case systemcolorvalInactiveCaptionText:     return _T("inactiveCaptionText");
			case systemcolorvalInfoBk:                  return _T("infoBk");
			case systemcolorvalInfoText:                return _T("infoText");
			case systemcolorvalMenu:                    return _T("menu");
			case systemcolorvalMenuBar:                 return _T("menuBar");
			case systemcolorvalMenuHighlight:           return _T("menuHighlight");
			case systemcolorvalMenuText:                return _T("menuText");
			case systemcolorvalScrollBar:               return _T("scrollBar");
			case systemcolorvalWindow:                  return _T("window");
			case systemcolorvalWindowFrame:             return _T("windowFrame");
			case systemcolorvalWindowText:              return _T("windowText");
			default :                                   return _T("window");
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


#ifndef _WIN32
        static DWORD GetSysColor(const int nIndex)
        {
            // get color values from any windows theme
            return 0;
        }

#endif
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

		virtual ETextAlignmentType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'l':
                if      ( _T("l")    == sValue ) this->m_eValue = textalignmenttypeL;
				break;
			case 'c':
                if      ( _T("ctr")  == sValue ) this->m_eValue = textalignmenttypeCtr;
				break;
			case 'd':
                if      ( _T("dist") == sValue ) this->m_eValue = textalignmenttypeDist;
				break;
			case 'j':
                if      ( _T("just") == sValue ) this->m_eValue = textalignmenttypeJust;
				break;
			case 'r':
                if      ( _T("r")    == sValue ) this->m_eValue = textalignmenttypeR;
				break;
			}

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textalignmenttypeR   : return _T("r");			
			case textalignmenttypeCtr : return _T("ctr");		
			case textalignmenttypeDist: return _T("dist");		
			case textalignmenttypeJust: return _T("just");		
			case textalignmenttypeL   : return _T("l");		
			default                   : return _T("l");
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

		virtual ETextAnchoringType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'b':
                if      ( _T("b")    == sValue ) this->m_eValue = textanchoringtypeB;
				break;
			case 'c':
                if      ( _T("ctr")  == sValue ) this->m_eValue = textanchoringtypeCtr;
				break;
			case 'd':
                if      ( _T("dist") == sValue ) this->m_eValue = textanchoringtypeDist;
				break;
			case 'j':
                if      ( _T("just") == sValue ) this->m_eValue = textanchoringtypeJust;
				break;
			case 't':
                if      ( _T("t")    == sValue ) this->m_eValue = textanchoringtypeT;
				break;
			}

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textanchoringtypeB   : return _T("b");			
			case textanchoringtypeCtr : return _T("ctr");		
			case textanchoringtypeDist: return _T("dist");		
			case textanchoringtypeJust: return _T("just");		
			case textanchoringtypeT   : return _T("t");		
			default                   : return _T("t");
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
            this->m_eValue = min( 16, max( 1, unValue ) );
		}
        virtual unsigned char FromString(CString &sValue)
        {
            this->m_eValue = (unsigned char)_wtoi( sValue );

            if (this->m_eValue < 1)
                this->m_eValue = 1;
            if (this->m_eValue > 16)
                this->m_eValue = 16;

            return this->m_eValue;
        }

        virtual CString       ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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

		virtual ETextFontAlignType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'a':
                if      ( _T("a")    == sValue ) this->m_eValue = textfontaligntypeAuto;
				break;
			case 'c':
                if      ( _T("ctr")  == sValue ) this->m_eValue = textfontaligntypeCtr;
				break;
			case 'b':
                if      ( _T("base") == sValue ) this->m_eValue = textfontaligntypeBase;
                if      ( _T("b")    == sValue ) this->m_eValue = textfontaligntypeB;
			break;
			case 't':
                if      ( _T("just") == sValue ) this->m_eValue = textfontaligntypeT;
				break;
			}

            return this->m_eValue;
		}

		virtual CString            ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textfontaligntypeB   : return _T("b");			
			case textfontaligntypeCtr : return _T("ctr");		
			case textfontaligntypeT	  : return _T("t");		
			case textfontaligntypeBase: return _T("base");		
			case textfontaligntypeAuto: return _T("auto");		
			default                   : return _T("auto");
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

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					// Поправка 12.1.2.5 (Part4)
					int nValue = min( 100000, max( 1000, _wtoi( sValue ) ) );
					m_dValue = nValue / 1000.0;
				}
				return
					m_dValue = 0;
			}
			else
				m_dValue = _wtof( sValue.Mid( 0, nLen - 1 ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

            return sResult;
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

		virtual ETextHorzOverflowType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("clip")     == sValue ) this->m_eValue = texthorzoverflowtypeClip;
				break;
			case 'o':
                if      ( _T("overflow") == sValue ) this->m_eValue = texthorzoverflowtypeOverflow;
				break;
			}

            return this->m_eValue;
		}

		virtual CString               ToString  () const 
		{
            switch(this->m_eValue)
			{
			case texthorzoverflowtypeClip     : return _T("clip");			
			case texthorzoverflowtypeOverflow : return _T("overflow");		
			default                           : return _T("overflow");
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

        virtual __int64 FromString(CString &sValue)
        {
            this->m_eValue = _wtoi64( sValue );
            if (this->m_eValue < -51206400)
                this->m_eValue = -51206400;
            if (this->m_eValue > 51206400)
                this->m_eValue = 51206400;

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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

        virtual __int64 FromString(CString &sValue)
        {
            this->m_eValue = _wtoi64( sValue );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 51206400)
                this->m_eValue = 51206400;

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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
//        virtual __int64 FromString(CString &sValue)
//        {
//            this->m_eValue = _wtoi64( sValue );
//            if (m_eValue < 0)
//                this->m_eValue = 0;
//            if (m_eValue > 51206400)
//                this->m_eValue = 51206400;
//
//            return this->m_eValue;
//        }
//
//        virtual CString ToString  () const 
//        {
//            CString sResult;
//            sResult.Format( _T("%d"), this->m_eValue);
//
//            return sResult;
//        }
//
//        SimpleType_FromString     (__int64)
//        SimpleType_Operator_Equal (TextPoint)
//
//        double ToPoints()
//        {
//            return  Emu_To_Pt(m_eValue);
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

		virtual ETextShapeType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 5 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(4);
			switch ( wChar )
			{
			case 'A':

                if      ( _T("textArchDown")		== sValue ) this->m_eValue = textshapetypeTextArchDown;
                else if ( _T("textArchDownPour")	== sValue ) this->m_eValue = textshapetypeTextArchDownPour;
                else if ( _T("textArchUp")			== sValue ) this->m_eValue = textshapetypeTextArchUp;
                else if ( _T("textArchUpPour")		== sValue ) this->m_eValue = textshapetypeTextArchUpPour;
				break;

			case 'B':

                if      ( _T("textButton")			== sValue ) this->m_eValue = textshapetypeTextButton;
                else if ( _T("textButtonPour")		== sValue ) this->m_eValue = textshapetypeTextButtonPour;
				break;

			case 'C':

                if      ( _T("textCanDown")			== sValue ) this->m_eValue = textshapetypeTextCanDown;
                else if ( _T("textCanUp")			== sValue ) this->m_eValue = textshapetypeTextCanUp;
                else if ( _T("textCascadeDown")		== sValue ) this->m_eValue = textshapetypeTextCascadeDown;
                else if ( _T("textCascadeUp")		== sValue ) this->m_eValue = textshapetypeTextCascadeUp;
                else if ( _T("textChevron")			== sValue ) this->m_eValue = textshapetypeTextChevron;
                else if ( _T("textChevronInverted")	== sValue ) this->m_eValue = textshapetypeTextChevronInverted;
                else if ( _T("textCircle")			== sValue ) this->m_eValue = textshapetypeTextCircle;
                else if ( _T("textCirclePour")		== sValue ) this->m_eValue = textshapetypeTextCirclePour;
                else if ( _T("textCurveDown")		== sValue ) this->m_eValue = textshapetypeTextCurveDown;
                else if ( _T("textCurveUp")			== sValue ) this->m_eValue = textshapetypeTextCurveUp;
				break;

			case 'D':
                if      ( _T("textDeflate")			== sValue ) this->m_eValue = textshapetypeTextDeflate;
                else if ( _T("textDeflateBottom")	== sValue ) this->m_eValue = textshapetypeTextDeflateBottom;
                else if ( _T("textDeflateInflate")	== sValue ) this->m_eValue = textshapetypeTextDeflateInflate;
                else if ( _T("textDeflateInflateDeflate") == sValue ) this->m_eValue = textshapetypeTextDeflateInflateDeflate;
                else if ( _T("textDeflateTop")		== sValue ) this->m_eValue = textshapetypeTextDeflateTop;
                else if ( _T("textDoubleWave1")		== sValue ) this->m_eValue = textshapetypeTextDoubleWave1;
				break;

			case 'F':
                if      ( _T("textFadeDown")		== sValue ) this->m_eValue = textshapetypeTextFadeDown;
                else if ( _T("textFadeLeft")		== sValue ) this->m_eValue = textshapetypeTextFadeLeft;
                else if ( _T("textFadeRight")		== sValue ) this->m_eValue = textshapetypeTextFadeRight;
                else if ( _T("textFadeUp")			== sValue ) this->m_eValue = textshapetypeTextFadeUp;
				break;

			case 'I':

                if      ( _T("textInflate")			== sValue ) this->m_eValue = textshapetypeTextInflate;
                else if ( _T("textInflateBottom")	== sValue ) this->m_eValue = textshapetypeTextInflateBottom;
                else if ( _T("textInflateTop")		== sValue ) this->m_eValue = textshapetypeTextInflateTop;
				break;

			case 'N':
                if      ( _T("textNoShape")			== sValue ) this->m_eValue = textshapetypeTextNoShape;
				break;

			case 'P':

                if      ( _T("textPlain")			== sValue ) this->m_eValue = textshapetypeTextPlain;
				break;

			case 'R':

                if		( _T("textRingInside")		== sValue ) this->m_eValue = textshapetypeTextRingInside;
                else if ( _T("textRingOutside")		== sValue ) this->m_eValue = textshapetypeTextRingOutside;
				break;

			case 'S':

                if      ( _T("textSlantDown")		== sValue ) this->m_eValue = textshapetypeTextSlantDown;
                else if ( _T("textSlantUp")			== sValue ) this->m_eValue = textshapetypeTextSlantUp;
                else if ( _T("textStop")			== sValue ) this->m_eValue = textshapetypeTextStop;
				break;

			case 'T':
				
                if      ( _T("textTriangle")		== sValue ) this->m_eValue = textshapetypeTextTriangle;
                else if ( _T("textTriangleInverted")== sValue ) this->m_eValue = textshapetypeTextTriangleInverted;
				break;

			case 'W':
				
                if      ( _T("textWave1")			== sValue ) this->m_eValue = textshapetypeTextWave1;
                else if ( _T("textWave2")			== sValue ) this->m_eValue = textshapetypeTextWave2;
                else if ( _T("textWave4")			== sValue ) this->m_eValue = textshapetypeTextWave4;
				break;
			}

            return this->m_eValue;
		}

		virtual CString        ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textshapetypeTextArchDown:				return _T("textArchDown");
			case textshapetypeTextArchDownPour:			return _T("textArchDownPour");
			case textshapetypeTextArchUp:				return _T("textArchUp");
			case textshapetypeTextArchUpPour:			return _T("textArchUpPour");
			case textshapetypeTextButton:				return _T("textButton");
			case textshapetypeTextButtonPour:			return _T("textButtonPour");
			case textshapetypeTextCanDown:				return _T("textCanDown");
			case textshapetypeTextCanUp:				return _T("textCanUp");
			case textshapetypeTextCascadeDown:			return _T("textCascadeDown");
			case textshapetypeTextCascadeUp:			return _T("textCascadeUp");
			case textshapetypeTextChevron:				return _T("textChevron");
			case textshapetypeTextChevronInverted:		return _T("textChevronInverted");
			case textshapetypeTextCircle:				return _T("textCircle");
			case textshapetypeTextCirclePour:			return _T("textCirclePour");
			case textshapetypeTextCurveDown:			return _T("textCurveDown");
			case textshapetypeTextCurveUp:				return _T("textCurveUp");
			case textshapetypeTextDeflate:				return _T("textDeflate");
			case textshapetypeTextDeflateBottom:		return _T("textDeflateBottom");
			case textshapetypeTextDeflateInflate:		return _T("textDeflateInflate");
			case textshapetypeTextDeflateInflateDeflate:return _T("textDeflateInflateDeflate");
			case textshapetypeTextDeflateTop:			return _T("textDeflateTop");
			case textshapetypeTextDoubleWave1:			return _T("textDoubleWave1");
			case textshapetypeTextFadeDown:				return _T("textFadeDown");
			case textshapetypeTextFadeLeft:				return _T("textFadeLeft");
			case textshapetypeTextFadeRight:			return _T("textFadeRight");
			case textshapetypeTextFadeUp:				return _T("textFadeUp");
			case textshapetypeTextInflate:				return _T("textInflate");
			case textshapetypeTextInflateBottom:		return _T("textInflateBottom");
			case textshapetypeTextInflateTop:			return _T("textInflateTop");
			case textshapetypeTextNoShape:				return _T("textNoShape");
			case textshapetypeTextPlain:				return _T("textPlain");
			case textshapetypeTextRingInside:			return _T("textRingInside");
			case textshapetypeTextRingOutside:			return _T("textRingOutside");
			case textshapetypeTextSlantDown:			return _T("textSlantDown");
			case textshapetypeTextSlantUp:				return _T("textSlantUp");
			case textshapetypeTextStop:					return _T("textStop");
			case textshapetypeTextTriangle:				return _T("textTriangle");
			case textshapetypeTextTriangleInverted:		return _T("textTriangleInverted");
			case textshapetypeTextWave1:				return _T("textWave1");
			case textshapetypeTextWave2:				return _T("textWave2");
			case textshapetypeTextWave4:				return _T("textWave4");
			default :									return _T("textPlain");
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

        virtual double FromString(CString &sValue)
        {
			int nPos = sValue.Find( '%' );
			int nLen = sValue.GetLength();
			if ( -1 == nPos || nPos != sValue.GetLength() - 1 || nLen <= 0  )
			{
				if ( -1 == nPos )
				{
					// Поправка 12.1.2.7 (Part4)
					int nValue = min( 13200000, max( 0, _wtoi( sValue ) ) );
					m_dValue = nValue / 1000.0;
				}
				return m_dValue;
			}
			else
				m_dValue = _wtof( sValue.Mid( 0, nLen - 1 ) );

            return m_dValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%f%%"), m_dValue );

            return sResult;
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

        virtual __int64 FromString(CString &sValue)
        {
            this->m_eValue = _wtoi64( sValue );
            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 158400)
                this->m_eValue = 158400;

            return this->m_eValue;
        }

        virtual CString ToString  () const 
        {
            CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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

		CString GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(CString &sValue)
		{
			m_sValue = sValue;
		}


		CString FromString(CString &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

		CString ToString  () const 
		{
			return m_sValue;
		}

		SimpleType_FromString2    (CString)
		SimpleType_Operator_Equal (CTextTypeface)

	private:

		CString m_sValue;
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

		virtual ETextVerticalType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'e':
                if      ( _T("eaVert")         == sValue ) this->m_eValue = textverticaltypeEaVert;
				break;
			case 'h':
                if      ( _T("horz")           == sValue ) this->m_eValue = textverticaltypeHorz;
				break;
			case 'm':
                if      ( _T("mongolianVert")  == sValue ) this->m_eValue = textverticaltypeMongolianVert;
				break;
			case 'v':
                if      ( _T("vert")           == sValue ) this->m_eValue = textverticaltypeVert;
                else if ( _T("vert270")        == sValue ) this->m_eValue = textverticaltypeVert270;
				break;
			case 'w':
                if      ( _T("wordArtVert")    == sValue ) this->m_eValue = textverticaltypeWordArtVert;
                else if ( _T("wordArtVertRtl") == sValue ) this->m_eValue = textverticaltypeWordArtVertRtl;
				break;
			}

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textverticaltypeEaVert         : return _T("eaVert");			
			case textverticaltypeHorz           : return _T("horz");		
			case textverticaltypeMongolianVert  : return _T("mongolianVert");			
			case textverticaltypeVert           : return _T("vert");		
			case textverticaltypeVert270        : return _T("vert270");			
			case textverticaltypeWordArtVert    : return _T("wordArtVert");		
			case textverticaltypeWordArtVertRtl : return _T("wordArtVertRtl");		
			default                             : return _T("horz");
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

		virtual ETextVertOverflowType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'c':
                if      ( _T("clip")     == sValue ) this->m_eValue = textvertoverflowtypeClip;
				break;
			case 'e':
                if      ( _T("ellipsis") == sValue ) this->m_eValue = textvertoverflowtypeEllipsis;
				break;
			case 'o':
                if      ( _T("overflow") == sValue ) this->m_eValue = textvertoverflowtypeOverflow;
				break;
			}

            return this->m_eValue;
		}

		virtual CString              ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textvertoverflowtypeClip     : return _T("clip");			
			case textvertoverflowtypeEllipsis : return _T("ellipsis");		
			case textvertoverflowtypeOverflow : return _T("overflow");			
			default                           : return _T("overflow");
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

		virtual ETextWrappingType FromString(CString &sValue)
		{
            this->m_eValue = eDefValue;

			if ( sValue.GetLength() <= 0 )
                return this->m_eValue;

			wchar_t wChar = sValue.GetAt(0);
			switch ( wChar )
			{
			case 'n':
                if      ( _T("none")   == sValue ) this->m_eValue = textwrappingtypeNone;
				break;
			case 's':
                if      ( _T("square") == sValue ) this->m_eValue = textwrappingtypeSquare;
				break;
			}

            return this->m_eValue;
		}

		virtual CString           ToString  () const 
		{
            switch(this->m_eValue)
			{
			case textwrappingtypeNone   : return _T("none");			
			case textwrappingtypeSquare : return _T("square");		
			default                     : return _T("none");
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

		virtual ETileFlipMode FromString(CString &sValue)
		{
            if      ( _T("none") == sValue ) this->m_eValue = tileflipmodeNone;
            else if ( _T("x")    == sValue ) this->m_eValue = tileflipmodeX;
            else if ( _T("xy")   == sValue ) this->m_eValue = tileflipmodeXY;
            else if ( _T("y")    == sValue ) this->m_eValue = tileflipmodeY;
            else                             this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString       ToString  () const 
		{
            switch(this->m_eValue)
			{
			case tileflipmodeNone: return _T("none");
			case tileflipmodeX   : return _T("x");
			case tileflipmodeXY  : return _T("xy");
			case tileflipmodeY   : return _T("y");
			default              : return _T("none");
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

		virtual EAlignH FromString(CString &sValue)
		{
            if       ( _T("center")  == sValue ) this->m_eValue = alignhCenter;
            else if  ( _T("inside")  == sValue ) this->m_eValue = alignhInside;
            else if  ( _T("left")    == sValue ) this->m_eValue = alignhLeft;
            else if  ( _T("outside") == sValue ) this->m_eValue = alignhOutside;
            else if  ( _T("right")   == sValue ) this->m_eValue = alignhRight;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
            switch(this->m_eValue)
			{
			case alignhCenter  : return _T("center");
			case alignhInside  : return _T("inside");
			case alignhLeft    : return _T("left");
			case alignhOutside : return _T("outside");
			case alignhRight   : return _T("right");
			default            : return _T("left");
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

		virtual EAlignV FromString(CString &sValue)
		{
            if       ( _T("bottom")  == sValue ) this->m_eValue = alignvBottom;
            else if  ( _T("center")  == sValue ) this->m_eValue = alignvCenter;
            else if  ( _T("inside")  == sValue ) this->m_eValue = alignvInside;
            else if  ( _T("outside") == sValue ) this->m_eValue = alignvOutside;
            else if  ( _T("top")     == sValue ) this->m_eValue = alignvTop;
            else                                 this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString ToString() const 
		{
            switch(this->m_eValue)
			{
			case alignvBottom  : return _T("bottom");
			case alignvCenter  : return _T("center");
			case alignvInside  : return _T("inside");
			case alignvOutside : return _T("outside");
			case alignvTop     : return _T("top");
			default            : return _T("top");
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

		virtual __int64 FromString(CString &sValue)
		{
            this->m_eValue = _wtoi64( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

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
            return Emu_To_Mm( this->m_eValue );
		}
		double ToInches()
		{
            return Emu_To_Inch( this->m_eValue );
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

		virtual ERelFromH FromString(CString &sValue)
		{
            if       ( _T("character")     == sValue ) this->m_eValue = relfromhCharacter;
            else if  ( _T("column")        == sValue ) this->m_eValue = relfromhColumn;
            else if  ( _T("insideMargin")  == sValue ) this->m_eValue = relfromhInsideMargin;
            else if  ( _T("leftMargin")    == sValue ) this->m_eValue = relfromhLeftMargin;
            else if  ( _T("margin")        == sValue ) this->m_eValue = relfromhMargin;
            else if  ( _T("outsideMargin") == sValue ) this->m_eValue = relfromhOutsideMargin;
            else if  ( _T("page")          == sValue ) this->m_eValue = relfromhPage;
            else if  ( _T("rightMargin")   == sValue ) this->m_eValue = relfromhRightMargin;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString() const 
		{
            switch(this->m_eValue)
			{
			case relfromhCharacter     : return _T("character");
			case relfromhColumn        : return _T("column");
			case relfromhInsideMargin  : return _T("insideMargin");
			case relfromhLeftMargin    : return _T("leftMargin");
			case relfromhMargin        : return _T("margin");
			case relfromhOutsideMargin : return _T("outsideMargin");
			case relfromhPage          : return _T("page");
			case relfromhRightMargin   : return _T("rightMargin");
			default                    : return _T("page");
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

		virtual ERelFromV FromString(CString &sValue)
		{
            if       ( _T("bottomMargin")  == sValue ) this->m_eValue = relfromvBottomMargin;
            else if  ( _T("insideMargin")  == sValue ) this->m_eValue = relfromvInsideMargin;
            else if  ( _T("line")          == sValue ) this->m_eValue = relfromvLine;
            else if  ( _T("margin")        == sValue ) this->m_eValue = relfromvMargin;
            else if  ( _T("outsideMargin") == sValue ) this->m_eValue = relfromvOutsideMargin;
            else if  ( _T("page")          == sValue ) this->m_eValue = relfromvPage;
            else if  ( _T("paragraph")     == sValue ) this->m_eValue = relfromvParagraph;
            else if  ( _T("topMargin")     == sValue ) this->m_eValue = relfromvTopMargin;
            else                                       this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString() const 
		{
            switch(this->m_eValue)
			{
			case relfromvBottomMargin  : return _T("bottomMargin");
			case relfromvInsideMargin  : return _T("insideMargin");
			case relfromvLine          : return _T("line");
			case relfromvMargin        : return _T("margin");
			case relfromvOutsideMargin : return _T("outsideMargin");
			case relfromvPage          : return _T("page");
			case relfromvParagraph     : return _T("paragraph");
			case relfromvTopMargin     : return _T("topMargin");
			default                    : return _T("page");
			}
		}

		SimpleType_FromString     (ERelFromV)
		SimpleType_Operator_Equal (CRelFromV)
	};




	//--------------------------------------------------------------------------------
	// WrapDistance 10.4.3.6 (Part 1)
	//--------------------------------------------------------------------------------

	template<__int64 nDefValue = 0>
	class CWrapDistance : public CSimpleType<__int64, nDefValue>
	{
	public:
		CWrapDistance() {}

		virtual __int64 FromString(CString &sValue)
		{
            this->m_eValue = _wtoi( sValue );

            return this->m_eValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
            sResult.Format( _T("%d"), this->m_eValue);

			return sResult;
		}


		SimpleType_FromString     (__int64)
		SimpleType_Operator_Equal (CWrapDistance)

		double ToPoints()
		{
            return Emu_To_Pt( this->m_eValue );
		}
		double ToMM() const 
		{
            return Emu_To_Mm( this->m_eValue );
		}

		double ToInches()
		{
            return Emu_To_Inch( this->m_eValue );
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

		virtual EWrapText FromString(CString &sValue)
		{
            if       ( _T("bothSides") == sValue ) this->m_eValue = wraptextBothSides;
            else if  ( _T("largest")   == sValue ) this->m_eValue = wraptextLargest;
            else if  ( _T("left")      == sValue ) this->m_eValue = wraptextLeft;
            else if  ( _T("right")     == sValue ) this->m_eValue = wraptextRight;
            else                                   this->m_eValue = eDefValue;

            return this->m_eValue;
		}

		virtual CString   ToString() const 
		{
            switch(this->m_eValue)
			{
			case wraptextBothSides : return _T("bothSides");
			case wraptextLargest   : return _T("largest");
			case wraptextLeft      : return _T("left");
			case wraptextRight     : return _T("right");
			default                : return _T("left");
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


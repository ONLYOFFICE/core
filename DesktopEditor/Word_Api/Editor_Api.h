#ifndef _BUILD_EDITOR_API_CROSSPLATFORM_H_
#define _BUILD_EDITOR_API_CROSSPLATFORM_H_

#include "./Editor_Defines.h"

#include <string>

// colors
namespace NSEditorApi
{
	class CAscColorSimple
	{
	public:
		BYTE R;
		BYTE G;
		BYTE B;
		BYTE A;

	public:
		CAscColorSimple()
		{
			R = 0;
			G = 0;
			B = 0;
			A = 0;
		}
	};

	class CColorMod
	{
	private:
		std::string m_sName;
		int m_nValue;

	public:
		CColorMod()
		{
			m_sName		= "";
			m_nValue	= 0;
		}

		LINK_PROPERTY_STRINGA(Name)
		LINK_PROPERTY_INT(Value)
	};

	class CAscColor
	{
	private:
		int m_nType; // c_oAscColor_COLOR_TYPE
		
		BYTE m_nR;
		BYTE m_nG;
		BYTE m_nB;
		BYTE m_nA;

		bool m_bAuto;
		
		int m_nValue;
		int m_nColorSchemeId;

		CColorMod* m_pMods;

	public:
		CAscColor()
		{
			m_nType = c_oAscColor_COLOR_TYPE_SRGB;

			m_nR = 0;
			m_nG = 0;
			m_nB = 0;
			m_nA = 255;

			m_bAuto = false;

			m_nValue = -1;
			m_nColorSchemeId = -1;

			m_pMods = NULL;
		}
		~CAscColor()
		{
			if (NULL != m_pMods)
				delete [] m_pMods;
		}

		LINK_PROPERTY_INT(Type)
		LINK_PROPERTY_BYTE(R)
		LINK_PROPERTY_BYTE(G)
		LINK_PROPERTY_BYTE(B)
		LINK_PROPERTY_BYTE(A)
		LINK_PROPERTY_BOOL(Auto)
		LINK_PROPERTY_INT(Value)
		LINK_PROPERTY_INT(ColorSchemeId)

		// эти методы дергает лишь sdk. оно должно следить за удалением само.
		CColorMod* GetMods() { return m_pMods; }
		void SetMods(CColorMod* pMods) { m_pMods = pMods; }		
	};
}

// document
namespace NSEditorApi
{
	class CAscEditorPermissions
	{
	private:
		bool m_bCanEdit;
		bool m_bCanDownload;
		bool m_bCanCoAuthoring;
		bool m_bCanBranding;
		bool m_bIsAutosaveEnabled;
		int m_nAutosaveMinInterval;

	public:
		CAscEditorPermissions()
		{
			m_bCanEdit				= true;
			m_bCanDownload			= true;
			m_bCanCoAuthoring		= true;
			m_bCanBranding			= true;
			m_bIsAutosaveEnabled	= true;
			m_nAutosaveMinInterval	= 300;
		}

	public:
		LINK_PROPERTY_BOOL(CanEdit)
		LINK_PROPERTY_BOOL(CanDownload)
		LINK_PROPERTY_BOOL(CanCoAuthoring)
		LINK_PROPERTY_BOOL(CanBranding)
		LINK_PROPERTY_BOOL(IsAutosaveEnabled)
		LINK_PROPERTY_INT(AutosaveMinInterval)
	};

	class CAscLicense
	{
	private:
		js_wrapper<std::wstring> m_sCustomer;
		js_wrapper<std::wstring> m_sCustomerAddr;
		js_wrapper<std::wstring> m_sCustomerWww;
		js_wrapper<std::wstring> m_sCustomerMail;
		js_wrapper<std::wstring> m_sCustomerInfo;
		js_wrapper<std::wstring> m_sCustomerLogo;

	public:
		CAscLicense()
		{
			m_sCustomer.SetNull();
			m_sCustomerAddr.SetNull();
			m_sCustomerWww.SetNull();
			m_sCustomerMail.SetNull();
			m_sCustomerInfo.SetNull();
			m_sCustomerLogo.SetNull();
		}

		LINK_PROPERTY_STRING_JS(Customer)
		LINK_PROPERTY_STRING_JS(CustomerAddr)
		LINK_PROPERTY_STRING_JS(CustomerWww)
		LINK_PROPERTY_STRING_JS(CustomerMail)
		LINK_PROPERTY_STRING_JS(CustomerInfo)
		LINK_PROPERTY_STRING_JS(CustomerLogo)
	};

	class CAscDocumentOpenProgress
	{
	private:
		int m_nType;

		int m_nFontsCount;
		int m_nFontCurrent;

		int m_nImagesCount;
		int m_nImageCurrent;

	public:
		CAscDocumentOpenProgress()
		{
			m_nType = c_oAscAsyncAction_Open;

			m_nFontsCount = 0;
			m_nFontCurrent = 0;

			m_nImagesCount = 0;
			m_nImageCurrent = 0;
		}

		LINK_PROPERTY_INT(Type)
		LINK_PROPERTY_INT(FontsCount)
		LINK_PROPERTY_INT(FontCurrent)
		LINK_PROPERTY_INT(ImagesCount)
		LINK_PROPERTY_INT(ImageCurrent)
	};

	class CAscDocumentInfo
	{
	private:
		js_wrapper<std::wstring> m_sId;
		js_wrapper<std::wstring> m_sUrl;
		js_wrapper<std::wstring> m_sTitle;
		js_wrapper<std::wstring> m_sFormat;
		js_wrapper<std::wstring> m_sVKey;
		js_wrapper<std::wstring> m_sUserId;
		js_wrapper<std::wstring> m_sUserName;

	public:
		CAscDocumentInfo()
		{
			m_sId.SetNull();
			m_sUrl.SetNull();
			m_sTitle.SetNull();
			m_sFormat.SetNull();
			m_sVKey.SetNull();
			m_sUserId.SetNull();
			m_sUserName.SetNull();
		}

		LINK_PROPERTY_STRING_JS(Id)
		LINK_PROPERTY_STRING_JS(Url)
		LINK_PROPERTY_STRING_JS(Title)
		LINK_PROPERTY_STRING_JS(Format)
		LINK_PROPERTY_STRING_JS(VKey)
		LINK_PROPERTY_STRING_JS(UserId)
		LINK_PROPERTY_STRING_JS(UserName)
	};
}

// base
namespace NSEditorApi
{
	class CAscRect
	{
	public:
		int X;
		int Y;
		int Width;
		int Height;

	public:
		CAscRect()
		{
			X = 0;
			Y = 0;
			Width = 0;
			Height = 0;
		}
	};

	class CAscPaddings
	{
	private:
		js_wrapper<double> m_dLeft;
		js_wrapper<double> m_dTop;
		js_wrapper<double> m_dRight;
		js_wrapper<double> m_dBottom;

	public:
		CAscPaddings()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Left)
		LINK_PROPERTY_DOUBLE_JS(Top)
		LINK_PROPERTY_DOUBLE_JS(Right)
		LINK_PROPERTY_DOUBLE_JS(Bottom)
	};

	class CAscPosition
	{
	private:
		js_wrapper<double> m_dX;
		js_wrapper<double> m_dY;

	public:
		CAscPosition()
		{
			m_dX.SetNull();
			m_dY.SetNull();
		}

		LINK_PROPERTY_DOUBLE_JS(X)
		LINK_PROPERTY_DOUBLE_JS(Y)
	};
}

// fill
namespace NSEditorApi
{
	class CAscFillSolid
	{
	private:
		CAscColor m_oColor;

	public:
		CAscFillSolid()
		{
		}

		CAscColor& get_Color() { return m_oColor; }
	};

	class CAscFillBlip
	{
	private:
		int				m_nType; // c_oAscFillBlipType_
		std::wstring	m_sUrl;
		int				m_nTextureId;

	public:
		CAscFillBlip()
		{
			m_nType = c_oAscFillBlipType_STRETCH;
			m_sUrl = L"";
			m_nTextureId = -1;
		}

		LINK_PROPERTY_INT(Type)
		LINK_PROPERTY_STRING(Url)
		LINK_PROPERTY_INT(TextureId)
	};

	class CAscFillHatch
	{
	private:
		int m_nPatternType;
		CAscColor m_oFg;
		CAscColor m_oBg;

	public:
		CAscFillHatch()
		{
			m_nPatternType = -1;
		}

		LINK_PROPERTY_INT(PatternType)

		CAscColor& get_Fg() { return m_oFg; }
		CAscColor& get_Bg() { return m_oBg; }
	};

	class CAscFillGrad
	{
	private:
		int			m_nCountColors;
		CAscColor*	m_pColors;
		int*		m_pPositions;

		int			m_nGradType;

		double		m_dLinearAngle;
		bool		m_bLinearScale;

		int			m_nPathType;

	public:
		CAscFillGrad()
		{
			m_nCountColors	= 0;
			m_pColors		= NULL;
			m_pPositions	= NULL;

			m_nGradType = 0;

			m_dLinearAngle = 0;
			m_bLinearScale = 0;

			m_nPathType = 0;
		}

		inline int get_CountColors() { return m_nCountColors; }
		inline CAscColor* get_Colors() { return m_pColors; }
		inline int* get_Positions() { return m_pPositions; }

		LINK_PROPERTY_INT(GradType)
		LINK_PROPERTY_DOUBLE(LinearAngle)
		LINK_PROPERTY_BOOL(LinearScale)
		LINK_PROPERTY_INT(PathType)
	};

	class CAscFill
	{
	private:
		js_wrapper<int>		m_nType;
		js_wrapper<void*>	m_pFill;
		js_wrapper<int>		m_nTransparent;

	public:
		CAscFill()
		{
			m_nType.SetNull();
			m_pFill.SetNull();
			m_nTransparent.SetNull();
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_INT_JS(Transparent)
		
		inline js_wrapper<void*> get_Fill() { return m_pFill; }

		inline void put_Fill(js_wrapper<void*> pFill)
		{
			m_pFill = pFill;
		}
	};
}

// stroke
namespace NSEditorApi
{
	class CAscStroke
	{
	private:
		js_wrapper<int>			m_nType;
		js_wrapper<double>		m_dWidth;
		js_wrapper<CAscColor>	m_oColor;

		js_wrapper<BYTE> m_nLineJoin;
		js_wrapper<BYTE> m_nLineCap;

		js_wrapper<BYTE> m_nLineBeginStyle;
		js_wrapper<BYTE> m_nLineBeginSize;

		js_wrapper<BYTE> m_nLineEndStyle;
		js_wrapper<BYTE> m_nLineEndSize;

		js_wrapper<bool> m_bCanChangeArrows;

	public:
		CAscStroke()
		{
			m_nType.SetNull();
			m_dWidth.SetNull();
			m_oColor.SetNull();

			m_nLineJoin.SetNull();
			m_nLineCap.SetNull();

			m_nLineBeginSize.SetNull();
			m_nLineBeginStyle.SetNull();

			m_nLineEndSize.SetNull();
			m_nLineEndStyle.SetNull();

			m_bCanChangeArrows = false;
		}

		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_DOUBLE_JS(Width)
		
		LINK_PROPERTY_BYTE_JS(LineJoin)
		LINK_PROPERTY_BYTE_JS(LineCap)
		LINK_PROPERTY_BYTE_JS(LineBeginStyle)
		LINK_PROPERTY_BYTE_JS(LineBeginSize)
		LINK_PROPERTY_BYTE_JS(LineEndStyle)
		LINK_PROPERTY_BYTE_JS(LineEndSize)

		LINK_PROPERTY_BOOL_JS(CanChangeArrows)
	};
}

// shape/image/chart props
namespace NSEditorApi
{
	class CAscShapeProp
	{
		js_wrapper<int>				m_nType;

		js_wrapper<CAscFill>		m_oFill;
		js_wrapper<CAscStroke>		m_oStroke;

		js_wrapper<CAscPaddings>	m_oPaddings;

		js_wrapper<bool>			m_bCanFill;
		js_wrapper<bool>			m_bFromChart;

	public:
		CAscShapeProp()
		{
			m_nType.SetNull();
			m_oFill.SetNull();
			m_oStroke.SetNull();
			m_oPaddings.SetNull();

			m_bCanFill = true;
			m_bFromChart = false;
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_BOOL_JS(CanFill)
		LINK_PROPERTY_BOOL_JS(FromChart)

		LINK_PROPERTY_OBJECT_JS(CAscFill, Fill)
		LINK_PROPERTY_OBJECT_JS(CAscStroke, Stroke)		
	};

	class CAscImagePosition
	{
	private:
		js_wrapper<int>		m_nRelativeFrom;
		js_wrapper<bool>	m_bUseAlign;
		js_wrapper<int>		m_nAlign;
		js_wrapper<int>		m_nValue;

	public:
		CAscImagePosition()
		{
		}

		LINK_PROPERTY_INT_JS(RelativeFrom)
		LINK_PROPERTY_BOOL_JS(UseAlign)
		LINK_PROPERTY_INT_JS(Align)
		LINK_PROPERTY_INT_JS(Value)
	};

	class CAscImageSize
	{
	private:
		double m_dWidth;
		double m_dHeight;
		bool m_bIsCorrect;

	public:
		CAscImageSize()
		{
			m_dWidth = 0;
			m_dHeight = 0;
			m_bIsCorrect = false;
		}

		LINK_PROPERTY_DOUBLE(Width)
		LINK_PROPERTY_DOUBLE(Height)
		LINK_PROPERTY_BOOL(IsCorrect)
	};

	class CAscChartProperties
	{
		// TODO:
	};

	class CAscImageProp
	{
	private:
		js_wrapper<bool> m_bCanBeFlow;

		js_wrapper<double> m_dWidth;
		js_wrapper<double> m_dHeight;

		js_wrapper<int> m_nWrappingStyle;

		js_wrapper<CAscPaddings> m_oPaddings;
		js_wrapper<CAscPosition> m_oPosition;

		js_wrapper<bool> m_bAllowOverlap;

		js_wrapper<CAscImagePosition> m_oPositionH;
		js_wrapper<CAscImagePosition> m_oPositionV;

		js_wrapper<int> m_nInternalPosition;

		js_wrapper<std::wstring> m_sUrl;
		js_wrapper<bool> m_bLocked;

		js_wrapper<CAscChartProperties> m_oChartProperties;
		js_wrapper<CAscShapeProp> m_oShapeProperties;

		js_wrapper<int> m_nChangeLevel;
		js_wrapper<bool> m_bGroup;

		js_wrapper<bool> m_bFromGroup;
		
		js_wrapper<bool> m_bSeveralCharts;
		js_wrapper<int> m_nSeveralChartTypes;
		js_wrapper<int> m_nSeveralChartStyles;

		js_wrapper<int> m_nVerticalTextAlign;

	public:
		LINK_PROPERTY_BOOL_JS(CanBeFlow)
		
		LINK_PROPERTY_DOUBLE_JS(Width)
		LINK_PROPERTY_DOUBLE_JS(Height)

		LINK_PROPERTY_INT_JS(WrappingStyle)

		LINK_PROPERTY_OBJECT_JS(CAscPaddings, Paddings)
		LINK_PROPERTY_OBJECT_JS(CAscPosition, Position)

		LINK_PROPERTY_BOOL_JS(AllowOverlap)

		LINK_PROPERTY_OBJECT_JS(CAscImagePosition, PositionH)
		LINK_PROPERTY_OBJECT_JS(CAscImagePosition, PositionV)

		LINK_PROPERTY_INT_JS(InternalPosition)

		LINK_PROPERTY_STRING_JS(Url)
		LINK_PROPERTY_BOOL_JS(Locked)

		LINK_PROPERTY_OBJECT_JS(CAscChartProperties, ChartProperties)
		LINK_PROPERTY_OBJECT_JS(CAscShapeProp, ShapeProperties)

		LINK_PROPERTY_INT_JS(ChangeLevel)

		LINK_PROPERTY_BOOL_JS(Group)
		LINK_PROPERTY_BOOL_JS(FromGroup)
		LINK_PROPERTY_BOOL_JS(SeveralCharts)

		LINK_PROPERTY_INT_JS(SeveralChartTypes)
		LINK_PROPERTY_INT_JS(SeveralChartStyles)
		LINK_PROPERTY_INT_JS(VerticalTextAlign)
	};
}

// section
namespace NSEditorApi
{
	class CAscSection
	{
	private:
		js_wrapper<double> m_dPageWidth;
		js_wrapper<double> m_dPageHeight;

		js_wrapper<double> m_dMarginLeft;
		js_wrapper<double> m_dMarginRight;
		js_wrapper<double> m_dMarginTop;
		js_wrapper<double> m_dMarginBottom;

	public:
		CAscSection()
		{
			m_dPageWidth	= (double)0;
			m_dPageHeight	= (double)0;

			m_dMarginLeft	= (double)0;
			m_dMarginRight	= (double)0;
			m_dMarginTop	= (double)0;
			m_dMarginBottom = (double)0;
		}

		LINK_PROPERTY_DOUBLE_JS(PageWidth)
		LINK_PROPERTY_DOUBLE_JS(PageHeight)
		LINK_PROPERTY_DOUBLE_JS(MarginLeft)
		LINK_PROPERTY_DOUBLE_JS(MarginRight)
		LINK_PROPERTY_DOUBLE_JS(MarginTop)
		LINK_PROPERTY_DOUBLE_JS(MarginBottom)
	};
}

// text
namespace NSEditorApi
{
	class CAscListType
	{
	private:
		js_wrapper<int> m_nType;
		js_wrapper<int> m_nSubType;

	public:

		CAscListType()
		{
			m_nType.SetNull();
			m_nSubType.SetNull();
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_INT_JS(SubType)
	};

	class CAscTextFontFamily
	{
	private:
		js_wrapper<std::wstring> m_sName;
		js_wrapper<int> m_nIndex;

	public:

		CAscTextFontFamily()
		{
			m_sName = L"Times New Roman";
			m_nIndex = -1;
		}

		LINK_PROPERTY_STRING_JS(Name)
		LINK_PROPERTY_INT_JS(Index)
	};

	class CAscTextPr
	{
	private:
		js_wrapper<bool> m_bBold;
		js_wrapper<bool> m_bItalic;
		js_wrapper<bool> m_bUnderline;
		js_wrapper<bool> m_bStrikeout;

		js_wrapper<CAscTextFontFamily> m_oFontFamily;
		js_wrapper<double> m_dFontSize;

		js_wrapper<CAscColor> m_oColor;

		js_wrapper<int> m_nVertAlign;
		js_wrapper<CAscColor> m_oHighLight; // null = none. undefined = no init

		js_wrapper<bool> m_bDStrikeout;
		
		js_wrapper<bool> m_bCaps;
		js_wrapper<bool> m_bSmallCaps;

	public:

		CAscTextPr()
		{			
		}

		LINK_PROPERTY_BOOL_JS(Bold)
		LINK_PROPERTY_BOOL_JS(Italic)
		LINK_PROPERTY_BOOL_JS(Underline)
		LINK_PROPERTY_BOOL_JS(Strikeout)

		LINK_PROPERTY_OBJECT_JS(CAscTextFontFamily, FontFamily)

		LINK_PROPERTY_DOUBLE_JS(FontSize)
		LINK_PROPERTY_INT_JS(VertAlign)

		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)
		LINK_PROPERTY_OBJECT_JS(CAscColor, HighLight)

		LINK_PROPERTY_BOOL_JS(DStrikeout)
		LINK_PROPERTY_BOOL_JS(Caps)
		LINK_PROPERTY_BOOL_JS(SmallCaps)
	};

	class CAscParagraphInd
	{
	private:
		js_wrapper<double>	m_dLeft;
		js_wrapper<double>	m_dRight;
		js_wrapper<double>	m_dFirstLine;

		CAscParagraphInd()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Left)
		LINK_PROPERTY_DOUBLE_JS(Right)
		LINK_PROPERTY_DOUBLE_JS(FirstLine)
	};

	class CAscParagraphSpacing
	{
	private:
		js_wrapper<double>	m_dLine;
		js_wrapper<int>		m_nLineRule;
		js_wrapper<double>	m_dBefore;
		js_wrapper<double>	m_dAfter;

	public:
		
		CAscParagraphSpacing()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Line)
		LINK_PROPERTY_INT_JS(LineRule)
		LINK_PROPERTY_DOUBLE_JS(Before)
		LINK_PROPERTY_DOUBLE_JS(After)
	};

	class CAscParagraphShd
	{
	private:
		js_wrapper<int>			m_nType;
		js_wrapper<CAscColor>	m_oColor;

	public:

		CAscParagraphShd()
		{
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)
	};

	class CAscParagraphPr
	{
	private:
		js_wrapper<bool>		m_bContextualSpacing;

		js_wrapper<CAscParagraphInd> m_oInd;

		js_wrapper<bool>		m_bKeepLines;
		js_wrapper<bool>		m_bKeepNext;
		js_wrapper<bool>		m_bWidowControl;
		js_wrapper<bool>		m_bPageBreakBefore;

		js_wrapper<CAscParagraphSpacing> m_oSpacing;
		
		//this.Brd               = undefined;
		js_wrapper<CAscParagraphShd> m_oShd;
        
		js_wrapper<bool>		m_bLocked;
		js_wrapper<bool>		m_bCanAddTable;

        //this.Tabs              = undefined;

		js_wrapper<bool>		m_bCanAddDropCap;

		js_wrapper<bool>		m_bSubscript;
		js_wrapper<bool>		m_bSuperscript;
		js_wrapper<bool>		m_bSmallCaps;
		js_wrapper<bool>		m_bAllCaps;
		js_wrapper<bool>		m_bStrikeout;
		js_wrapper<bool>		m_bDStrikeout;

		js_wrapper<double>		m_dTextSpacing;
		js_wrapper<double>      m_dPosition;

	public:

		CAscParagraphPr()
		{
		}

		LINK_PROPERTY_BOOL_JS(ContextualSpacing)
		
		LINK_PROPERTY_OBJECT_JS(CAscParagraphInd, Ind)

		LINK_PROPERTY_BOOL_JS(KeepLines)
		LINK_PROPERTY_BOOL_JS(KeepNext)
		LINK_PROPERTY_BOOL_JS(WidowControl)
		LINK_PROPERTY_BOOL_JS(PageBreakBefore)

		LINK_PROPERTY_OBJECT_JS(CAscParagraphSpacing, Spacing)

		//
		LINK_PROPERTY_OBJECT_JS(CAscParagraphShd, Shd)

		LINK_PROPERTY_BOOL_JS(Locked)
		LINK_PROPERTY_BOOL_JS(CanAddTable)

		//

		LINK_PROPERTY_BOOL_JS(CanAddDropCap)

		LINK_PROPERTY_BOOL_JS(Subscript)
		LINK_PROPERTY_BOOL_JS(Superscript)
		LINK_PROPERTY_BOOL_JS(SmallCaps)
		LINK_PROPERTY_BOOL_JS(AllCaps)
		LINK_PROPERTY_BOOL_JS(Strikeout)
		LINK_PROPERTY_BOOL_JS(DStrikeout)

		LINK_PROPERTY_DOUBLE_JS(TextSpacing)
		LINK_PROPERTY_DOUBLE_JS(Position)
	};
}

// common
namespace NSEditorApi
{
	class CAscColorScheme
	{
	private:
		std::wstring m_sName;
		
		bool				m_bIsDelete;
		CAscColorSimple*	m_pColors;
		int					m_lColorsCount;

	public:
		CAscColorScheme()
		{
			m_sName			= L"";
			m_bIsDelete		= true;
			m_pColors		= NULL;
			m_lColorsCount	= 0;
		}
		~CAscColorScheme()
		{
			if (m_bIsDelete)
			{
				if (NULL != m_pColors)
					delete [] m_pColors;
				m_pColors = NULL;
			}
		}

		void Create(const std::wstring& sName, const bool& bDelete, const int& nCountColors)
		{
			if (m_bIsDelete)
			{
				if (NULL != m_pColors)
					delete [] m_pColors;
				m_pColors = NULL;
			}

			m_sName			= sName;
			m_bIsDelete		= bDelete;
			m_lColorsCount	= nCountColors;

			if (0 < m_lColorsCount)
			{
				m_pColors = new CAscColorSimple[m_lColorsCount];
			}
		}

		LINK_PROPERTY_STRING(Name)
		
		inline int get_ColorsCount() { return m_lColorsCount; }
		inline CAscColorSimple* get_Colors() { return m_pColors; }
		void put_AscColorSimple(CAscColorSimple* pColors, int lCount)
		{
			if (NULL != m_pColors)
				delete [] m_pColors;
			m_pColors = pColors;
			m_lColorsCount = lCount;
		}
	};

	class CAscTexture
	{
	private:
		int m_nId;
		std::wstring m_sImage;

	public:
		CAscTexture()
		{
			m_nId = 0;
			m_sImage = L"";
		}

		LINK_PROPERTY_INT(Id)
		LINK_PROPERTY_STRING(Image)
	};
}

#endif //_BUILD_EDITOR_API_CROSSPLATFORM_H_

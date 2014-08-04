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

// common
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

class CAscShapeProp
{
    int m_nType;

	CAscFill m_oFill;
	CAscStroke m_oStroke;

	CAscPaddings m_oPaddings;

	bool m_bCanFill;
	bool m_bFromChart;

public:
	CAscShapeProp()
	{
		m_nType = 0;

		m_bCanFill = true;
		m_bFromChart = false;
	}

	LINK_PROPERTY_INT(Type)
	LINK_PROPERTY_BOOL(CanFill)
	LINK_PROPERTY_BOOL(FromChart)

	CAscFill& get_Fill() { return m_oFill; }
	CAscStroke& get_Stroke() { return m_oStroke; }
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
	std::wstring m_sId;
	std::wstring m_sUrl;
	std::wstring m_sTitle;
	std::wstring m_sFormat;
	std::wstring m_sVKey;
	std::wstring m_sUserId;
	std::wstring m_sUserName;

public:
	CAscDocumentInfo()
	{
		m_sId = L"";
		m_sUrl = L"";
		m_sTitle = L"";
		m_sFormat = L"";
		m_sVKey = L"";
		m_sUserId = L"";
		m_sUserName = L"";
	}

	LINK_PROPERTY_STRING(Id)
	LINK_PROPERTY_STRING(Url)
	LINK_PROPERTY_STRING(Title)
	LINK_PROPERTY_STRING(Format)
	LINK_PROPERTY_STRING(VKey)
	LINK_PROPERTY_STRING(UserId)
	LINK_PROPERTY_STRING(UserName)
};

class CAscListType
{
private:
	int m_nType;
	int m_nSubType;

public:

	CAscListType()
	{
		m_nType = -1;
		m_nSubType = -1;
	}

	LINK_PROPERTY_INT(Type)
	LINK_PROPERTY_INT(SubType)
};

class CAscSection
{
private:
	double m_dPageWidth;
	double m_dPageHeight;

	double m_dMarginLeft;
	double m_dMarginRight;
	double m_dMarginTop;
	double m_dMarginBottom;

public:
	CAscSection()
	{
		m_dPageWidth = 0;
		m_dPageHeight = 0;

		m_dMarginLeft = 0;
		m_dMarginRight = 0;
		m_dMarginTop = 0;
		m_dMarginBottom = 0;
	}

	LINK_PROPERTY_DOUBLE(PageWidth)
	LINK_PROPERTY_DOUBLE(PageHeight)
	LINK_PROPERTY_DOUBLE(MarginLeft)
	LINK_PROPERTY_DOUBLE(MarginRight)
	LINK_PROPERTY_DOUBLE(MarginTop)
	LINK_PROPERTY_DOUBLE(MarginBottom)
};

class CAscImagePosition
{
private:
	int m_nRelativeFrom;
	bool m_bUseAlign;
	int m_nAlign;
	int m_nValue;

public:
	CAscImagePosition()
	{
		m_nRelativeFrom = 0;
		m_bUseAlign = false;
		m_nAlign = 0;
		m_nValue = 0;
	}

	LINK_PROPERTY_INT(RelativeFrom)
	LINK_PROPERTY_BOOL(UseAlign)
	LINK_PROPERTY_INT(Align)
	LINK_PROPERTY_INT(Value)
};

class CAscPosition
{
private:
	double m_dX;
	double m_dY;

public:
	CAscPosition()
	{
		m_dX = 0;
		m_dY = 0;
	}

	LINK_PROPERTY_DOUBLE(X)
	LINK_PROPERTY_DOUBLE(Y)
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

class CAscImageProp
{
private:
	bool m_bCanBeFlow;

	double m_dWidth;
	double m_dHeight;

	int m_nWrappingStyle;

	CAscPaddings m_oPaddings;
	CAscPosition m_oPosition;

	bool m_bAllowOverlap;

	CAscImagePosition m_oPositionH;
	CAscImagePosition m_oPositionV;

	int m_nInternalPosition;

	std::wstring m_sUrl;
	bool m_bLocked;

	int m_nChangeLevel;
	bool m_bGroup;

	// TODO:
};

}

#endif //_BUILD_EDITOR_API_CROSSPLATFORM_H_

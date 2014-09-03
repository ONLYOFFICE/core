#ifndef _BUILD_EDITOR_API_CROSSPLATFORM_H_
#define _BUILD_EDITOR_API_CROSSPLATFORM_H_

#include "./Editor_Defines.h"

#include <string>

#ifdef max
#undef max
#endif

#include <vector>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace NSEditorApi
{
	class IMenuEventDataBase
	{
	protected:
		ULONG m_lRef;

	public:
		IMenuEventDataBase()
		{
			m_lRef = 1;
		}

		virtual ~IMenuEventDataBase()
		{
		}

		virtual ULONG AddRef() 
		{ 
			++m_lRef;
			return m_lRef;
		}
		virtual ULONG Release()
		{
			ULONG ret = --m_lRef;
			if (0 == m_lRef)
				delete this;
			return ret;
		}
	};
}

namespace NSEditorApi
{

#define LINK_PROPERTY_INT(memberName)					\
	inline int get_##memberName()						\
	{													\
		return m_n##memberName;							\
	}													\
	inline void put_##memberName(const int& newVal)		\
	{													\
		m_n##memberName = newVal;						\
	}

#define LINK_PROPERTY_DOUBLE(memberName)				\
	inline double get_##memberName()					\
	{													\
		return m_d##memberName;							\
	}													\
	inline void put_##memberName(const double& newVal)	\
	{													\
		m_d##memberName = newVal;						\
	}

#define LINK_PROPERTY_BOOL(memberName)					\
	inline bool get_##memberName()						\
	{													\
		return m_b##memberName;							\
	}													\
	inline void put_##memberName(const bool& newVal)	\
	{													\
		m_b##memberName = newVal;						\
	}

#define LINK_PROPERTY_BYTE(memberName)					\
	inline unsigned char get_##memberName()				\
	{													\
		return m_n##memberName;							\
	}													\
	inline void put_##memberName(const unsigned char& newVal)	\
	{															\
		m_n##memberName = newVal;								\
	}

#define LINK_PROPERTY_STRING(memberName)						\
	inline std::wstring get_##memberName()						\
	{															\
		return m_s##memberName;									\
	}															\
	inline void put_##memberName(const std::wstring& newVal)	\
	{															\
		m_s##memberName = newVal;								\
	}
#define LINK_PROPERTY_STRINGA(memberName)						\
	inline std::string get_##memberName()						\
	{															\
		return m_s##memberName;									\
	}															\
	inline void put_##memberName(const std::string& newVal)		\
	{															\
		m_s##memberName = newVal;								\
	}

// JS
#define LINK_PROPERTY_INT_JS(memberName)								\
	inline js_wrapper<int>& get_##memberName()							\
	{																	\
		return m_n##memberName;											\
	}																	\
	inline void put_##memberName(const int& newVal)						\
	{																	\
		m_n##memberName = newVal;										\
	}																	\
	inline void put_##memberName(const js_wrapper<int>& newVal)			\
	{																	\
		m_n##memberName = newVal;										\
	}

#define LINK_PROPERTY_DOUBLE_JS(memberName)								\
	inline js_wrapper<double>& get_##memberName()						\
	{																	\
		return m_d##memberName;											\
	}																	\
	inline void put_##memberName(const double& newVal)					\
	{																	\
		m_d##memberName = newVal;										\
	}																	\
	inline void put_##memberName(const js_wrapper<double>& newVal)		\
	{																	\
		m_d##memberName = newVal;										\
	}

#define LINK_PROPERTY_BOOL_JS(memberName)								\
	inline js_wrapper<bool>& get_##memberName()							\
	{																	\
		return m_b##memberName;											\
	}																	\
	inline void put_##memberName(const bool& newVal)					\
	{																	\
		m_b##memberName = newVal;										\
	}																	\
	inline void put_##memberName(const js_wrapper<bool>& newVal)		\
	{																	\
		m_b##memberName = newVal;										\
	}

#define LINK_PROPERTY_BYTE_JS(memberName)								\
	inline js_wrapper<unsigned char>& get_##memberName()				\
	{																	\
		return m_n##memberName;											\
	}																	\
	inline void put_##memberName(const unsigned char& newVal)			\
	{																	\
		m_n##memberName = newVal;										\
	}																	\
	inline void put_##memberName(const js_wrapper<unsigned char>& newVal)		\
	{																	\
		m_n##memberName = newVal;										\
	}

#define LINK_PROPERTY_STRING_JS(memberName)									\
	inline js_wrapper<std::wstring>& get_##memberName()						\
	{																		\
		return m_s##memberName;												\
	}																		\
	inline void put_##memberName(const std::wstring& newVal)				\
	{																		\
		m_s##memberName = newVal;											\
	}																		\
	inline void put_##memberName(const js_wrapper<std::wstring>& newVal)	\
	{																		\
		m_s##memberName = newVal;											\
	}

#define LINK_PROPERTY_STRINGA_JS(memberName)								\
	inline js_wrapper<std::string>& get_##memberName()						\
	{																		\
		return m_s##memberName;												\
	}																		\
	inline void put_##memberName(const std::string& newVal)					\
	{																		\
		m_s##memberName = newVal;											\
	}																		\
	inline void put_##memberName(const js_wrapper<std::string>& newVal)		\
	{																		\
		m_s##memberName = newVal;											\
	}

#define LINK_PROPERTY_OBJECT_JS(objectType, memberName)						\
	inline js_wrapper<objectType>& get_##memberName()						\
	{																		\
		return m_o##memberName;												\
	}																		\
	inline void put_##memberName(const js_wrapper<objectType>& newVal)		\
	{																		\
		m_o##memberName = newVal;											\
	}																		\
	inline void put_##memberName(objectType* newVal)						\
	{																		\
		m_o##memberName = newVal;											\
	}

template<typename Type>
class js_wrapper : public NSEditorApi::IMenuEventDataBase
{
protected:
	Type* m_pPointer;
	bool m_bIsNull;

public:
	js_wrapper()
	{
		m_pPointer = NULL;
		m_bIsNull = false;
	}
	js_wrapper(const js_wrapper<Type>& oOther)
	{
		m_pPointer = NULL;
		
		if (oOther.m_bIsNull)
			m_bIsNull = true;
		else
		{
			m_bIsNull = false;

			if ( NULL != oOther.m_pPointer )
				m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );
		}
	}
	js_wrapper(Type* pOther)
	{
		m_pPointer = pOther;
		m_bIsNull = false;
	}
	virtual ~js_wrapper()
	{
		if (NULL != m_pPointer)
			delete m_pPointer;
	}

public:
	inline Type& operator*()  { return *m_pPointer; }
	inline Type* operator->() { return  m_pPointer; }

	inline Type& operator*() const  { return *m_pPointer; }
	inline Type* operator->() const { return  m_pPointer; }

	inline const Type& get()const { return  *m_pPointer; } 
	inline Type& get() { return  *m_pPointer; }

public:
	js_wrapper<Type>& operator=(const js_wrapper<Type> &oOther)
	{
		if (NULL != m_pPointer)
		{
			delete m_pPointer;
			m_pPointer = NULL;
		}

		if (oOther.m_bIsNull)
			m_bIsNull = true;
		else
		{
			m_bIsNull = false;

			if ( NULL != oOther.m_pPointer )
				m_pPointer = new Type( (const Type&)*(oOther.m_pPointer) );
		}
		return *this;
	}
	js_wrapper<Type>& operator=(Type* pType)
	{
		if (NULL != m_pPointer)
			delete m_pPointer;

		m_pPointer	= pType;
		m_bIsNull = false;
		return *this;
	}
	js_wrapper<Type>& operator=(const Type& oSrc)
	{
		if (NULL != m_pPointer)
			delete m_pPointer;

		m_pPointer	= new Type(oSrc);
		m_bIsNull = false;
		return *this;
	}

public:
	inline bool IsNull() const
	{ 
		return m_bIsNull;
	}
	inline bool IsUndefined() const
	{ 
		return (NULL == m_pPointer);
	}
	inline bool IsInit() const
	{
		return ((NULL != m_pPointer) && !m_bIsNull);
	}
	inline void SetNull()
	{
		m_bIsNull = true;
	}
	inline void ReleaseNoAttack()
	{
		m_pPointer = NULL;
	}
};

}

// colors
namespace NSEditorApi
{
	class CAscColorSimple : public IMenuEventDataBase
	{
	public:
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;

	public:
		CAscColorSimple()
		{
			R = 0;
			G = 0;
			B = 0;
			A = 0;
		}
		virtual ~CAscColorSimple()
		{
		}
	};

	class CColorMod : public IMenuEventDataBase
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
		virtual ~CColorMod()
		{
		}

		LINK_PROPERTY_STRINGA(Name)
		LINK_PROPERTY_INT(Value)
	};

	class CColorMods : public IMenuEventDataBase
	{
	private:
		CColorMod* m_pMods;
		int m_lCount;

	public:
		CColorMods()
		{
			m_pMods = NULL;
			m_lCount = 0;
		}
		virtual ~CColorMods()
		{
			if (NULL != m_pMods)
				delete [] m_pMods;
		}

		int GetCount() { return m_lCount; }
		CColorMod* GetMods() { return m_pMods; }

		void SetMods(CColorMod* pMods, int nCount)
		{
			m_pMods = pMods;
			m_lCount = nCount;
		}
	};

	class CAscColor	: public IMenuEventDataBase
	{
	private:
		js_wrapper<int> m_nType; // c_oAscColor_COLOR_TYPE
		
		js_wrapper<unsigned char> m_nR;
		js_wrapper<unsigned char> m_nG;
		js_wrapper<unsigned char> m_nB;
		js_wrapper<unsigned char> m_nA;

		js_wrapper<bool> m_bAuto;
		
		js_wrapper<int> m_nValue;
		js_wrapper<int> m_nColorSchemeId;

		js_wrapper<CColorMods> m_oMods;

	public:
		CAscColor()
		{
		}
		virtual ~CAscColor()
		{
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_BYTE_JS(R)
		LINK_PROPERTY_BYTE_JS(G)
		LINK_PROPERTY_BYTE_JS(B)
		LINK_PROPERTY_BYTE_JS(A)
		LINK_PROPERTY_BOOL_JS(Auto)
		LINK_PROPERTY_INT_JS(Value)
		LINK_PROPERTY_INT_JS(ColorSchemeId)
		LINK_PROPERTY_OBJECT_JS(CColorMods, Mods)
	};
}

// document
namespace NSEditorApi
{
	class CAscEditorPermissions	: public IMenuEventDataBase
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
		virtual ~CAscEditorPermissions()
		{
		}

	public:
		LINK_PROPERTY_BOOL(CanEdit)
		LINK_PROPERTY_BOOL(CanDownload)
		LINK_PROPERTY_BOOL(CanCoAuthoring)
		LINK_PROPERTY_BOOL(CanBranding)
		LINK_PROPERTY_BOOL(IsAutosaveEnabled)
		LINK_PROPERTY_INT(AutosaveMinInterval)
	};

	class CAscLicense : public IMenuEventDataBase
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
		virtual ~CAscLicense()
		{
		}

		LINK_PROPERTY_STRING_JS(Customer)
		LINK_PROPERTY_STRING_JS(CustomerAddr)
		LINK_PROPERTY_STRING_JS(CustomerWww)
		LINK_PROPERTY_STRING_JS(CustomerMail)
		LINK_PROPERTY_STRING_JS(CustomerInfo)
		LINK_PROPERTY_STRING_JS(CustomerLogo)
	};

	class CAscDocumentOpenProgress : public IMenuEventDataBase
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
		virtual ~CAscDocumentOpenProgress()
		{
		}

		LINK_PROPERTY_INT(Type)
		LINK_PROPERTY_INT(FontsCount)
		LINK_PROPERTY_INT(FontCurrent)
		LINK_PROPERTY_INT(ImagesCount)
		LINK_PROPERTY_INT(ImageCurrent)
	};

	class CAscDocumentInfo : public IMenuEventDataBase
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
		virtual ~CAscDocumentInfo()
		{
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
	class CAscFillBase
	{
	public:
		CAscFillBase()
		{
		}
		virtual ~CAscFillBase()
		{
		}
	};

	class CAscFillSolid : public CAscFillBase
	{
	private:
		js_wrapper<CAscColor> m_oColor;

	public:
		CAscFillSolid()
		{
		}
		virtual ~CAscFillSolid()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)
	};

	class CAscFillBlip : public CAscFillBase
	{
	private:
		js_wrapper<int>				m_nType; // c_oAscFillBlipType_
		js_wrapper<std::wstring>	m_sUrl;
		js_wrapper<int>				m_nTextureId;

	public:
		CAscFillBlip()
		{
		}
		virtual ~CAscFillBlip()
		{
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_STRING_JS(Url)
		LINK_PROPERTY_INT_JS(TextureId)
	};

	class CAscFillHatch : public CAscFillBase
	{
	private:
		js_wrapper<int>			m_nPatternType;
		js_wrapper<CAscColor>	m_oFg;
		js_wrapper<CAscColor>	m_oBg;

	public:
		CAscFillHatch()
		{
		}
		virtual ~CAscFillHatch()
		{
		}

		LINK_PROPERTY_INT_JS(PatternType)

		LINK_PROPERTY_OBJECT_JS(CAscColor, Fg)
		LINK_PROPERTY_OBJECT_JS(CAscColor, Bg)
	};

	class CAscFillGradColors
	{
	private:
		js_wrapper<CAscColor>*	m_pColors;
		js_wrapper<int>*		m_pPositions;
		int m_lCount;

	public:

		CAscFillGradColors()
		{
			m_pColors = NULL;
			m_pPositions = NULL;
			m_lCount = 0;			
		}
		virtual ~CAscFillGradColors()
		{
			if (NULL != m_pColors)
				delete [] m_pColors;
			if (NULL != m_pPositions)
				delete [] m_pPositions;
		}

		int GetCount() { return m_lCount; }
		js_wrapper<CAscColor>* GetColors() { return m_pColors; }
		js_wrapper<int>* GetPositions() { return m_pPositions; }
		void SetColors(js_wrapper<CAscColor>* pColors, js_wrapper<int>* pPositions, int nCount)
		{
			m_pColors = pColors;
			m_pPositions = pPositions;
			m_lCount = nCount;
		}
	};

	class CAscFillGrad : public CAscFillBase
	{
	private:
		js_wrapper<CAscFillGradColors>	m_oColors;

		js_wrapper<int>					m_nGradType;

		js_wrapper<double>				m_dLinearAngle;
		js_wrapper<bool>				m_bLinearScale;

		js_wrapper<int>					m_nPathType;

	public:
		CAscFillGrad()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscFillGradColors, Colors)

		LINK_PROPERTY_INT_JS(GradType)
		LINK_PROPERTY_DOUBLE_JS(LinearAngle)
		LINK_PROPERTY_BOOL_JS(LinearScale)
		LINK_PROPERTY_INT_JS(PathType)
	};

	class CAscFill : public IMenuEventDataBase
	{
	private:
		js_wrapper<int>				m_nType;
		js_wrapper<CAscFillBase>	m_oFill;
		js_wrapper<int>				m_nTransparent;

	public:
		CAscFill()
		{
		}
		virtual ~CAscFill()
		{
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_INT_JS(Transparent)
		
		LINK_PROPERTY_OBJECT_JS(CAscFillBase, Fill)
	};
}

// stroke
namespace NSEditorApi
{
	class CAscStroke : public IMenuEventDataBase
	{
	private:
		js_wrapper<int>			m_nType;
		js_wrapper<double>		m_dWidth;
		js_wrapper<CAscColor>	m_oColor;

		js_wrapper<unsigned char> m_nLineJoin;
		js_wrapper<unsigned char> m_nLineCap;

		js_wrapper<unsigned char> m_nLineBeginStyle;
		js_wrapper<unsigned char> m_nLineBeginSize;

		js_wrapper<unsigned char> m_nLineEndStyle;
		js_wrapper<unsigned char> m_nLineEndSize;

		js_wrapper<bool> m_bCanChangeArrows;

	public:
		CAscStroke()
		{
			m_bCanChangeArrows = false;
		}
		virtual ~CAscStroke()
		{
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
	class CAscShapeProp : public IMenuEventDataBase
	{
		js_wrapper<std::wstring>	m_sType;

		js_wrapper<CAscFill>		m_oFill;
		js_wrapper<CAscStroke>		m_oStroke;

		js_wrapper<CAscPaddings>	m_oPaddings;

		js_wrapper<bool>			m_bCanFill;
		js_wrapper<bool>			m_bFromChart;

	public:
		CAscShapeProp()
		{
			m_bCanFill = true;
			m_bFromChart = false;
		}
		virtual ~CAscShapeProp()
		{
		}

		LINK_PROPERTY_STRING_JS(Type)
		LINK_PROPERTY_BOOL_JS(CanFill)
		LINK_PROPERTY_BOOL_JS(FromChart)

		LINK_PROPERTY_OBJECT_JS(CAscFill, Fill)
		LINK_PROPERTY_OBJECT_JS(CAscStroke, Stroke)
		LINK_PROPERTY_OBJECT_JS(CAscPaddings, Paddings)	
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

	class CAscValAxisSettings
    {
	private:
		js_wrapper<int>			m_nMinValRule;		// c_oAscValAxisRule_
		js_wrapper<int>			m_nMinVal;
		js_wrapper<int>			m_nMaxValRule;		// c_oAscValAxisRule_
		js_wrapper<int>			m_nMaxVal;

        js_wrapper<bool>		m_bInsertValOrder;

		js_wrapper<bool>		m_bLogScale;
		js_wrapper<int>			m_nLogBase;

        js_wrapper<int>			m_nDispUnitsRule;	// c_oAscValAxUnits_
		js_wrapper<int>			m_nUnits;			// c_oAscValAxUnits_

		js_wrapper<bool>		m_bShowUnitsOnChart;

		js_wrapper<int>			m_nMajorTickMark;	// c_oAscTickMark_
		js_wrapper<int>			m_nMinorTickMark;	// c_oAscTickMark_

		js_wrapper<int>			m_nTickLabelsPos;	// c_oAscTickLabelsPos_

		js_wrapper<int>			m_nCrossesRule;		// c_oAscCrossesRule_
		js_wrapper<int>			m_nCrosses;

		js_wrapper<int>			m_nAxisType;		// c_oAscAxisType_

	public:
		CAscValAxisSettings()
		{
		}

		LINK_PROPERTY_INT_JS(MinValRule)
		LINK_PROPERTY_INT_JS(MinVal)
		LINK_PROPERTY_INT_JS(MaxValRule)
		LINK_PROPERTY_INT_JS(MaxVal)
		
		LINK_PROPERTY_BOOL_JS(InsertValOrder)

		LINK_PROPERTY_BOOL_JS(LogScale)
		LINK_PROPERTY_INT_JS(LogBase)

		LINK_PROPERTY_INT_JS(DispUnitsRule)
		LINK_PROPERTY_INT_JS(Units)

		LINK_PROPERTY_BOOL_JS(ShowUnitsOnChart)

		LINK_PROPERTY_INT_JS(MajorTickMark)
		LINK_PROPERTY_INT_JS(MinorTickMark)

		LINK_PROPERTY_INT_JS(TickLabelsPos)

		LINK_PROPERTY_INT_JS(CrossesRule)
		LINK_PROPERTY_INT_JS(Crosses)

		LINK_PROPERTY_INT_JS(AxisType)
    };

	class CAscCatAxisSettings
    {
	private:
        js_wrapper<double>		m_dInternalBetweenTick;
		js_wrapper<int>			m_nIntervalBetweenLabelsRule;	// c_oAscBetweenLabelsRule_
		js_wrapper<double>		m_dInternalBetweenLabels;

		js_wrapper<bool>		m_bInvertCatOrder;
		js_wrapper<double>		m_dLabelsAxisDistance;

		js_wrapper<int>			m_nMajorTickMark;	// c_oAscTickMark_
		js_wrapper<int>			m_nMinorTickMark;	// c_oAscTickMark_

		js_wrapper<int>			m_nTickLabelsPos;	// c_oAscTickLabelsPos_

		js_wrapper<int>			m_nCrossesRule;		// c_oAscCrossesRule_
		js_wrapper<int>			m_nCrosses;

		js_wrapper<int>			m_nLabelsPosition;	// c_oAscLabelsPosition_

		js_wrapper<int>			m_nAxisType;		// c_oAscAxisType_

	public:
		CAscCatAxisSettings()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(InternalBetweenTick)
		LINK_PROPERTY_INT_JS(IntervalBetweenLabelsRule)
		LINK_PROPERTY_DOUBLE_JS(InternalBetweenLabels)

		LINK_PROPERTY_BOOL_JS(InvertCatOrder)
		LINK_PROPERTY_DOUBLE_JS(LabelsAxisDistance)

		LINK_PROPERTY_INT_JS(MajorTickMark)
		LINK_PROPERTY_INT_JS(MinorTickMark)
		LINK_PROPERTY_INT_JS(TickLabelsPos)
		LINK_PROPERTY_INT_JS(CrossesRule)
		LINK_PROPERTY_INT_JS(Crosses)
		LINK_PROPERTY_INT_JS(LabelsPosition)
		LINK_PROPERTY_INT_JS(AxisType)
    };

	class CAscChartProperties : public IMenuEventDataBase
	{
	private:
		js_wrapper<int>				m_nStyle;
		js_wrapper<int>				m_nTitle;	// c_oAscChartTitleShowSettings_
        
		js_wrapper<int>				m_nRowCols;

        js_wrapper<int>				m_nHorAxisLabel;	// c_oAscChartHorAxisLabelShowSettings_
		js_wrapper<int>				m_nVertAxisLabel;	// c_oAscChartVertAxisLabelShowSettings_

		js_wrapper<int>				m_nLegendPos;		// c_oAscChartLegendShowSettings_
		js_wrapper<int>				m_nDataLabelPos;	// c_oAscChartDataLabelsPos_

		js_wrapper<int>				m_nHorAx;
		js_wrapper<int>				m_nVertAx;
        
		js_wrapper<int>				m_nHorGridLines;	// c_oAscGridLinesSettings_
		js_wrapper<int>				m_nVertGridLines;	// c_oAscGridLinesSettings_
        
		js_wrapper<int>				m_nType;

		js_wrapper<bool>			m_bShowSerName;
		js_wrapper<bool>			m_bShowCatName;
		js_wrapper<bool>			m_bShowVal;
        
		js_wrapper<std::wstring>	m_sSeparator;

		js_wrapper<CAscValAxisSettings>	m_oHorAxisProps;
		js_wrapper<CAscValAxisSettings>	m_oVertAxisProps;
        
		js_wrapper<std::wstring>	m_sRange;
		js_wrapper<bool>			m_bInColumns;

		js_wrapper<bool>			m_bShowMarker;
		js_wrapper<bool>			m_bLine;
		js_wrapper<bool>			m_bSmooth;

	public:
		CAscChartProperties()
		{
		}
		virtual ~CAscChartProperties()
		{
		}

		LINK_PROPERTY_INT_JS(Style)
		LINK_PROPERTY_INT_JS(Title)

		LINK_PROPERTY_INT_JS(RowCols)

		LINK_PROPERTY_INT_JS(HorAxisLabel)
		LINK_PROPERTY_INT_JS(VertAxisLabel)

		LINK_PROPERTY_INT_JS(LegendPos)
		LINK_PROPERTY_INT_JS(DataLabelPos)
		LINK_PROPERTY_INT_JS(HorAx)
		LINK_PROPERTY_INT_JS(VertAx)

		LINK_PROPERTY_INT_JS(HorGridLines)
		LINK_PROPERTY_INT_JS(VertGridLines)
		LINK_PROPERTY_INT_JS(Type)

		LINK_PROPERTY_BOOL_JS(ShowSerName)
		LINK_PROPERTY_BOOL_JS(ShowCatName)
		LINK_PROPERTY_BOOL_JS(ShowVal)

		LINK_PROPERTY_STRING_JS(Separator)

		LINK_PROPERTY_OBJECT_JS(CAscValAxisSettings, HorAxisProps)
		LINK_PROPERTY_OBJECT_JS(CAscValAxisSettings, VertAxisProps)

		LINK_PROPERTY_STRING_JS(Range)
		LINK_PROPERTY_BOOL_JS(InColumns)

		LINK_PROPERTY_BOOL_JS(ShowMarker)
		LINK_PROPERTY_BOOL_JS(Line)
		LINK_PROPERTY_BOOL_JS(Smooth)
	};

	class CAscImageProp : public IMenuEventDataBase
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
		js_wrapper<int> m_nGroup;

		js_wrapper<bool> m_bFromGroup;
		
		js_wrapper<bool> m_bSeveralCharts;
		js_wrapper<int> m_nSeveralChartTypes;
		js_wrapper<int> m_nSeveralChartStyles;

		js_wrapper<int> m_nVerticalTextAlign;

	public:
		CAscImageProp()
		{
		}
		virtual ~CAscImageProp()
		{
		}

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

		LINK_PROPERTY_INT_JS(Group)
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
	class CAscSection : public IMenuEventDataBase
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
		virtual ~CAscSection()
		{
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
        CAscTextFontFamily(const CAscTextFontFamily& oSrc)
		{
			m_sName = oSrc.m_sName;
			m_nIndex = oSrc.m_nIndex;
		}

		LINK_PROPERTY_STRING_JS(Name)
		LINK_PROPERTY_INT_JS(Index)
	};

	class CAscTextPr : public IMenuEventDataBase
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
		virtual ~CAscTextPr()
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

	public:
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
		js_wrapper<bool>	m_bBeforeAuto;
		js_wrapper<double>	m_dAfter;
		js_wrapper<bool>	m_bAfterAuto;

	public:
		
		CAscParagraphSpacing()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Line)
		LINK_PROPERTY_INT_JS(LineRule)
		LINK_PROPERTY_DOUBLE_JS(Before)
		LINK_PROPERTY_DOUBLE_JS(After)
		LINK_PROPERTY_BOOL_JS(BeforeAuto)
		LINK_PROPERTY_BOOL_JS(AfterAuto)
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

	class CAscBorder
	{
	private:
		js_wrapper<CAscColor>	m_oColor;
		js_wrapper<double>		m_dSize;
		js_wrapper<int>			m_nValue;
		js_wrapper<double>		m_dSpace;

	public:

		CAscBorder()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)
		LINK_PROPERTY_DOUBLE_JS(Size)
		LINK_PROPERTY_INT_JS(Value)
		LINK_PROPERTY_DOUBLE_JS(Space)
	};

	class CAscParagraphBorders
	{
	private:
		js_wrapper<CAscBorder> m_oLeft;
		js_wrapper<CAscBorder> m_oTop;
		js_wrapper<CAscBorder> m_oRight;
		js_wrapper<CAscBorder> m_oBottom;
		js_wrapper<CAscBorder> m_oBetween;

	public:

		CAscParagraphBorders()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscBorder, Left)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Top)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Right)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Bottom)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Between)
	};

	class CAscParagraphTab
	{
	private:
		js_wrapper<double>	m_dPos;
		js_wrapper<int>		m_nValue;

	public:

		CAscParagraphTab()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Pos)
		LINK_PROPERTY_INT_JS(Value)
	};

	class CAscParagraphTabs
	{
	private:
		CAscParagraphTab* m_pTabs;
		int m_lCount;

	public:

		CAscParagraphTabs()
		{
			m_pTabs = NULL;
			m_lCount = 0;
		}
		~CAscParagraphTabs()
		{
			if (NULL != m_pTabs)
				delete [] m_pTabs;
		}

		int GetCount() { return m_lCount; }
		CAscParagraphTab* GetTabs() { return m_pTabs; }
		void SetTabs(CAscParagraphTab* pTabs, int nCount)
		{
			m_pTabs = pTabs;
			m_lCount = nCount;
		}
	};

	class CAscParagraphFrame
	{
	private:
		js_wrapper<bool>		m_bFromDropCapMenu;
		js_wrapper<int>			m_nDropCap;				// c_oAscDropCap_
		
		js_wrapper<double>		m_dW;
		js_wrapper<double>		m_dH;

		js_wrapper<int>			m_nHAnchor;
		js_wrapper<int>			m_nHRule;				// linerule_
		js_wrapper<double>		m_dHSpace;

		js_wrapper<int>			m_nVAnchor;
		js_wrapper<double>		m_dVSpace;

		js_wrapper<double>		m_dX;
		js_wrapper<double>		m_dY;

		js_wrapper<int>			m_nXAlign;
		js_wrapper<int>			m_nYAlign;

		js_wrapper<int>			m_nLines;
		js_wrapper<int>			m_nWrap;				// c_oAsc_wrap_

		js_wrapper<CAscParagraphBorders>	m_oBrd;
		js_wrapper<CAscParagraphShd>		m_oShd;
		js_wrapper<CAscTextFontFamily>		m_oFontFamily;

	public:
		CAscParagraphFrame()
		{
		}

		LINK_PROPERTY_BOOL_JS(FromDropCapMenu)
		LINK_PROPERTY_INT_JS(DropCap)

		LINK_PROPERTY_DOUBLE_JS(W)
		LINK_PROPERTY_DOUBLE_JS(H)

		LINK_PROPERTY_INT_JS(HAnchor)
		LINK_PROPERTY_INT_JS(HRule)
		LINK_PROPERTY_DOUBLE_JS(HSpace)

		LINK_PROPERTY_INT_JS(VAnchor)
		LINK_PROPERTY_DOUBLE_JS(VSpace)

		LINK_PROPERTY_DOUBLE_JS(X)
		LINK_PROPERTY_DOUBLE_JS(Y)

		LINK_PROPERTY_INT_JS(XAlign)
		LINK_PROPERTY_INT_JS(YAlign)

		LINK_PROPERTY_INT_JS(Lines)
		LINK_PROPERTY_INT_JS(Wrap)

		LINK_PROPERTY_OBJECT_JS(CAscParagraphBorders, Brd)
		LINK_PROPERTY_OBJECT_JS(CAscParagraphShd, Shd)
		LINK_PROPERTY_OBJECT_JS(CAscTextFontFamily, FontFamily)
	};

	class CAscParagraphPr : public IMenuEventDataBase
	{
	private:
		js_wrapper<bool>		m_bContextualSpacing;

		js_wrapper<CAscParagraphInd> m_oInd;

		js_wrapper<bool>		m_bKeepLines;
		js_wrapper<bool>		m_bKeepNext;
		js_wrapper<bool>		m_bWidowControl;
		js_wrapper<bool>		m_bPageBreakBefore;

		js_wrapper<CAscParagraphSpacing> m_oSpacing;
		
		js_wrapper<CAscParagraphBorders> m_oBrd;
		js_wrapper<CAscParagraphShd> m_oShd;
        
		js_wrapper<bool>		m_bLocked;
		js_wrapper<bool>		m_bCanAddTable;
		js_wrapper<bool>		m_bCanAddDropCap;

		js_wrapper<double>		m_dDefaultTab;
        js_wrapper<CAscParagraphTabs> m_oTabs;

		js_wrapper<CAscParagraphFrame> m_oFramePr;
		
		js_wrapper<bool>		m_bSubscript;
		js_wrapper<bool>		m_bSuperscript;
		js_wrapper<bool>		m_bSmallCaps;
		js_wrapper<bool>		m_bAllCaps;
		js_wrapper<bool>		m_bStrikeout;
		js_wrapper<bool>		m_bDStrikeout;

		js_wrapper<double>		m_dTextSpacing;
		js_wrapper<double>      m_dPosition;

		js_wrapper<CAscListType>			m_oListType;
		js_wrapper<std::wstring>			m_sStyle;
		js_wrapper<int>						m_nJc;

	public:

		CAscParagraphPr()
		{
		}
		virtual ~CAscParagraphPr()
		{
		}

		LINK_PROPERTY_BOOL_JS(ContextualSpacing)
		
		LINK_PROPERTY_OBJECT_JS(CAscParagraphInd, Ind)

		LINK_PROPERTY_BOOL_JS(KeepLines)
		LINK_PROPERTY_BOOL_JS(KeepNext)
		LINK_PROPERTY_BOOL_JS(WidowControl)
		LINK_PROPERTY_BOOL_JS(PageBreakBefore)

		LINK_PROPERTY_OBJECT_JS(CAscParagraphSpacing, Spacing)

		LINK_PROPERTY_OBJECT_JS(CAscParagraphBorders, Brd)
		LINK_PROPERTY_OBJECT_JS(CAscParagraphShd, Shd)

		LINK_PROPERTY_BOOL_JS(Locked)
		LINK_PROPERTY_BOOL_JS(CanAddTable)
		LINK_PROPERTY_BOOL_JS(CanAddDropCap)

		LINK_PROPERTY_DOUBLE_JS(DefaultTab)
		LINK_PROPERTY_OBJECT_JS(CAscParagraphTabs, Tabs)

		LINK_PROPERTY_OBJECT_JS(CAscParagraphFrame, FramePr)

		LINK_PROPERTY_BOOL_JS(Subscript)
		LINK_PROPERTY_BOOL_JS(Superscript)
		LINK_PROPERTY_BOOL_JS(SmallCaps)
		LINK_PROPERTY_BOOL_JS(AllCaps)
		LINK_PROPERTY_BOOL_JS(Strikeout)
		LINK_PROPERTY_BOOL_JS(DStrikeout)

		LINK_PROPERTY_DOUBLE_JS(TextSpacing)
		LINK_PROPERTY_DOUBLE_JS(Position)

		LINK_PROPERTY_OBJECT_JS(CAscListType, ListType)
		LINK_PROPERTY_STRING_JS(Style)
		LINK_PROPERTY_INT_JS(Jc)
	};
}

// table
namespace NSEditorApi
{
	class CAscCellMargins
	{
	private:
		js_wrapper<double> m_dLeft;
		js_wrapper<double> m_dTop;
		js_wrapper<double> m_dRight;
		js_wrapper<double> m_dBottom;
		js_wrapper<int>	   m_nFlag;	

	public:
		CAscCellMargins()
		{
		}

		LINK_PROPERTY_DOUBLE_JS(Left)
		LINK_PROPERTY_DOUBLE_JS(Top)
		LINK_PROPERTY_DOUBLE_JS(Right)
		LINK_PROPERTY_DOUBLE_JS(Bottom)
		LINK_PROPERTY_INT_JS(Flag)
	};

	class CAscTableBorders
	{
	private:
		js_wrapper<CAscBorder> m_oLeft;
		js_wrapper<CAscBorder> m_oTop;
		js_wrapper<CAscBorder> m_oRight;
		js_wrapper<CAscBorder> m_oBottom;
		js_wrapper<CAscBorder> m_oInsideH;
		js_wrapper<CAscBorder> m_oInsideV;

	public:

		CAscTableBorders()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscBorder, Left)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Top)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Right)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, Bottom)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, InsideH)
		LINK_PROPERTY_OBJECT_JS(CAscBorder, InsideV)
	};

	class CAscCellBackground
	{
	private:
		js_wrapper<CAscColor>		m_oColor;
		js_wrapper<int>				m_nValue;

	public:
		CAscCellBackground()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscColor, Color)
		LINK_PROPERTY_INT_JS(Value)
	};

	class CAscTableLook
	{
	private:
		js_wrapper<bool>		m_bFirstCol;
		js_wrapper<bool>		m_bFirstRow;
		js_wrapper<bool>		m_bLastCol;
		js_wrapper<bool>		m_bLastRow;
		js_wrapper<bool>		m_bBandHor;
		js_wrapper<bool>		m_bBandVer;

	public:
		CAscTableLook()
		{
		}

		LINK_PROPERTY_BOOL_JS(FirstCol)
		LINK_PROPERTY_BOOL_JS(FirstRow)
		LINK_PROPERTY_BOOL_JS(LastCol)
		LINK_PROPERTY_BOOL_JS(LastRow)
		LINK_PROPERTY_BOOL_JS(BandHor)
		LINK_PROPERTY_BOOL_JS(BandVer)
	};

	class CAscTableAnchorPosition
	{
	public:
		// Рассчитанные координаты
		double CalcX;
		double CalcY;

		// Данные для Flow-объектов
		double W;
		double H;
		double X;
		double Y;
		double Left_Margin;
		double Right_Margin;
		double Top_Margin;
		double Bottom_Margin;
		double Page_W;
		double Page_H;

		double X_min;
		double Y_min;
		double X_max;
		double Y_max;

	public:
		CAscTableAnchorPosition()
		{
			CalcX         = 0;
			CalcY         = 0;

			W             = 0;
			H             = 0;
			X             = 0;
			Y             = 0;
			Left_Margin   = 0;
			Right_Margin  = 0;
			Top_Margin    = 0;
			Bottom_Margin = 0;
			Page_W        = 0;
			Page_H        = 0;

			X_min         = 0;
			Y_min         = 0;
			X_max         = 0;
			Y_max         = 0;
		}

		// По значению CalcX получем Value
		double Calculate_X_Value(const int& RelativeFrom)
		{
			switch (RelativeFrom)
			{
				case c_oAscHAnchor_Text:
				case c_oAscHAnchor_Margin:
				{
					return CalcX - Left_Margin;
				}
				case c_oAscHAnchor_Page:
				{
					return CalcX - X_min;
				}
				case c_oAscHAnchor_PageInternal:
				{
					return CalcX;
				}
				default:
					break;
			}

			return 0;
		}

		// По значению CalcY и заданному RelativeFrom получем Value
		double Calculate_Y_Value(const int& RelativeFrom)
		{
			switch (RelativeFrom)
			{
				case c_oAscVAnchor_Margin:
				{
					return CalcY - Top_Margin;
				}
				case c_oAscVAnchor_Page:
				{
					return CalcY;
				}
				case c_oAscVAnchor_Text:
				{
					return CalcY - Y;
				}
			}

			return 0;
		}
	};

	class CAscTableCellSplit
	{
	public:
		int Rows;
		int Cols;

		CAscTableCellSplit()
		{
			Rows = 0;
			Cols = 0;
		}
	};

	class CAscTableProperties : public IMenuEventDataBase
	{
	private:
		js_wrapper<bool>			m_bCanBeFlow;
		js_wrapper<bool>			m_bCellSelect;

		js_wrapper<double>			m_dWidth;
		js_wrapper<double>			m_dSpacing;

		js_wrapper<CAscPaddings>	m_oDefaultMargins;
		js_wrapper<CAscCellMargins>	m_oCellMargins;

		js_wrapper<int>				m_nAlignment;
		js_wrapper<double>			m_dIndent;

		js_wrapper<int>				m_nWrappingStyle;

		js_wrapper<CAscPaddings>	m_oPaddings;

		js_wrapper<CAscTableBorders>	m_oTableBorders;
		js_wrapper<CAscTableBorders>	m_oCellBorders;

		js_wrapper<CAscCellBackground>	m_oTableBackground;
		js_wrapper<CAscCellBackground>	m_oCellBackground;

		js_wrapper<CAscPosition>		m_oPosition;

		js_wrapper<CAscImagePosition>	m_oPositionH;
		js_wrapper<CAscImagePosition>	m_oPositionV;

		js_wrapper<CAscTableAnchorPosition> m_oInternalPosition;

		js_wrapper<bool>				m_bForSelectedCells;
		js_wrapper<std::wstring>		m_sStyle;
		js_wrapper<CAscTableLook>		m_oLook;
		js_wrapper<int>					m_nRowsInHeader;
		js_wrapper<int>					m_nCellsVAlign;		// c_oAscVertAlignJc_
		
		js_wrapper<bool>				m_bAllowOverlap;

		js_wrapper<int>					m_nLayout;
		js_wrapper<bool>				m_bLocked;

	public:
		CAscTableProperties()
		{
			m_bCellSelect = false;
			m_bLocked = false;
		}
		virtual ~CAscTableProperties()
		{
		}

		LINK_PROPERTY_BOOL_JS(CanBeFlow)
		LINK_PROPERTY_BOOL_JS(CellSelect)

		LINK_PROPERTY_DOUBLE_JS(Width)
		LINK_PROPERTY_DOUBLE_JS(Spacing)

		LINK_PROPERTY_OBJECT_JS(CAscPaddings, DefaultMargins)
		LINK_PROPERTY_OBJECT_JS(CAscCellMargins, CellMargins)

		LINK_PROPERTY_INT_JS(Alignment)
		LINK_PROPERTY_DOUBLE_JS(Indent)
		LINK_PROPERTY_INT_JS(WrappingStyle)

		LINK_PROPERTY_OBJECT_JS(CAscPaddings, Paddings)
		LINK_PROPERTY_OBJECT_JS(CAscTableBorders, TableBorders)
		LINK_PROPERTY_OBJECT_JS(CAscTableBorders, CellBorders)
		LINK_PROPERTY_OBJECT_JS(CAscCellBackground, TableBackground)
		LINK_PROPERTY_OBJECT_JS(CAscCellBackground, CellBackground)
		LINK_PROPERTY_OBJECT_JS(CAscPosition, Position)
		LINK_PROPERTY_OBJECT_JS(CAscImagePosition, PositionH)
		LINK_PROPERTY_OBJECT_JS(CAscImagePosition, PositionV)

		LINK_PROPERTY_OBJECT_JS(CAscTableAnchorPosition, InternalPosition)

		LINK_PROPERTY_BOOL_JS(ForSelectedCells)
		LINK_PROPERTY_STRING_JS(Style)
		LINK_PROPERTY_OBJECT_JS(CAscTableLook, Look)

		LINK_PROPERTY_INT_JS(RowsInHeader)
		LINK_PROPERTY_INT_JS(CellsVAlign)

		LINK_PROPERTY_BOOL_JS(AllowOverlap)
		LINK_PROPERTY_INT_JS(Layout)
		LINK_PROPERTY_BOOL_JS(Locked)

		double get_Value_X(const int& RelativeFrom) 
		{ 
			if (m_oInternalPosition.IsInit()) 
				return m_oInternalPosition->Calculate_X_Value(RelativeFrom);  
			return 0; 
		}
		double get_Value_Y(const int& RelativeFrom) 
		{ 
			if (m_oInternalPosition.IsInit()) 
				return m_oInternalPosition->Calculate_Y_Value(RelativeFrom);  
			return 0; 
		}
	};
}

// header/footer
namespace NSEditorApi
{
	class CAscHeaderFooterPr : public IMenuEventDataBase
	{
	private:
		js_wrapper<int>		m_nType;
		js_wrapper<double>	m_dPosition;
		js_wrapper<bool>	m_bDifferentFirst;
		js_wrapper<bool>	m_bDifferentEvenOdd;
		js_wrapper<bool>	m_bLinkToPrevious;
		js_wrapper<bool>	m_bLocked;

	public:

		CAscHeaderFooterPr()
		{
		}
		virtual ~CAscHeaderFooterPr()
		{
		}

		LINK_PROPERTY_INT_JS(Type)
		LINK_PROPERTY_DOUBLE_JS(Position)
		LINK_PROPERTY_BOOL_JS(DifferentFirst)
		LINK_PROPERTY_BOOL_JS(DifferentEvenOdd)
		LINK_PROPERTY_BOOL_JS(LinkToPrevious)
		LINK_PROPERTY_BOOL_JS(Locked)
	};
}

// hyperlink
namespace NSEditorApi
{
	class CAscHyperlinkPr : public IMenuEventDataBase
	{
	private:
		js_wrapper<std::wstring> m_sText;
		js_wrapper<std::wstring> m_sValue;
		js_wrapper<std::wstring> m_sToolTip;

	public:

		CAscHyperlinkPr()
		{
		}
		virtual ~CAscHyperlinkPr()
		{
		}

		LINK_PROPERTY_STRING_JS(Text)
		LINK_PROPERTY_STRING_JS(Value)
		LINK_PROPERTY_STRING_JS(ToolTip)
	};
}

// common
namespace NSEditorApi
{
	class CAscColorScheme : public IMenuEventDataBase
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
		virtual ~CAscColorScheme()
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

	class CAscTexture : public IMenuEventDataBase
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
		virtual ~CAscTexture()
		{
		}

		LINK_PROPERTY_INT(Id)
		LINK_PROPERTY_STRING(Image)
	};
}

// insert
namespace NSEditorApi
{
	// not release data!!!
	class CAscImageRaw
	{
	public:
		unsigned char*	Data;
		int				Width;
		int				Height;

		bool			Release;

	public:
		CAscImageRaw()
		{
			Data = NULL;
			Width = 0;
			Height = 0;
			Release = false;
		}
		~CAscImageRaw()
		{
			if (NULL != Data && Release)
				delete [] Data;
		}

		CAscImageRaw& operator=(const CAscImageRaw& oSrc)
		{
			Data	= oSrc.Data;
			Width	= oSrc.Width;
			Height	= oSrc.Height;
			Release = false;
			return *this;
		}
	};

	class CAscInsertImage : public IMenuEventDataBase
	{
	private:
		js_wrapper<CAscImageRaw>	m_oRaw;
		js_wrapper<std::wstring>	m_sPath;
		js_wrapper<std::string>		m_sBase64;	

	public:
		CAscInsertImage()
		{
		}
		virtual ~CAscInsertImage()
		{
		}

		LINK_PROPERTY_OBJECT_JS(CAscImageRaw, Raw)
		LINK_PROPERTY_STRING_JS(Path)
		LINK_PROPERTY_STRINGA_JS(Base64)
	};

	class CAscInsertTable : public IMenuEventDataBase
	{
	private:
		js_wrapper<std::wstring>	m_sStyle;
		js_wrapper<int>				m_nColumns;
		js_wrapper<int>				m_nRows;

	public:
		CAscInsertTable()
		{
		}
		virtual ~CAscInsertTable()
		{
		}

		LINK_PROPERTY_STRING_JS(Style)
		LINK_PROPERTY_INT_JS(Columns)
		LINK_PROPERTY_INT_JS(Rows)
	};

	typedef CAscShapeProp CAscInsertShape;
	typedef CAscHyperlinkPr CAscInsertHyperlink;

	class CAscMethodParamInt : public IMenuEventDataBase
	{
	private:
		js_wrapper<int>	m_nValue;

	public:
		CAscMethodParamInt()
		{
		}
		virtual ~CAscMethodParamInt()
		{
		}

		LINK_PROPERTY_INT_JS(Value)
	};

	typedef CAscMethodParamInt CAscInsertSectionBreak;
	typedef CAscMethodParamInt CAscInsertPageNumber;
}

namespace NSEditorApi
{
	class CAscStyleImage
	{
	public:
		std::wstring	Name;
		CAscImageRaw	Image;

	public:
		CAscStyleImage()
		{
		}
	};

	class CAscStyleImages : public IMenuEventDataBase
	{
	public:
		std::vector<CAscStyleImage> m_arStyles;

		CAscStyleImages()
		{
		}
		virtual ~CAscStyleImages()
		{
		}
	};
}

namespace NSEditorApi
{
	class CAscMenuEvent : public IMenuEventDataBase
	{
	public:
		int					m_nType;
		IMenuEventDataBase*	m_pData;

	public:
		CAscMenuEvent()
		{
			m_nType = -1;
			m_pData = NULL;
		}
		virtual ~CAscMenuEvent()
		{
			if (NULL != m_pData)
				delete m_pData;
		}
	};

	class CAscMenuEventStackData : public IMenuEventDataBase
	{
	public:
		std::vector<CAscMenuEvent*> m_data;

	public:
		CAscMenuEventStackData()
		{
		}
		virtual ~CAscMenuEventStackData()
		{
			for (std::vector<CAscMenuEvent*>::iterator i = m_data.begin(); i != m_data.end(); ++i)
			{
				CAscMenuEvent* data = *i;
				if (NULL != data)
					delete data;
			}
		}
	};

	class CAscMenuEventListener
	{
	public:
		// memory release!!!
		virtual void OnEvent(CAscMenuEvent* pEvent)
		{
			if (NULL != pEvent)
				delete pEvent;
		}
		virtual bool IsSupportEvent(int nEventType)
		{
			return true;
		}
	};

	class CAscMenuController
	{
		// release memory in sdk
		virtual void Apply(CAscMenuEvent* pEvent)
		{
			if (NULL != pEvent)
				delete pEvent;
		}
	};
}

#endif //_BUILD_EDITOR_API_CROSSPLATFORM_H_

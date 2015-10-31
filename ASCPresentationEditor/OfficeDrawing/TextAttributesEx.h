#pragma once
#include "Structures.h"
#include "TextStructures.h"

#if !defined(_WIN32) && !defined (_WIN64)
    //#include "./../../DesktopEditor/cximage/CxImage/ximadef.h"
    //#include "../../ASCOfficePPTXFile/PPTXFormat/Logic/rectdef.h"
    #include "../../DesktopEditor/graphics/aggplustypes.h"
#endif

namespace NSPresentationEditor
{
	class CTheme;
	class CLayout;
	
	class CTextAttributesEx
	{
	public:
		LONG	m_lTextMasterType; // only ppt property
	public:
		DWORD	m_lTextType;
		DWORD	m_lStyleThemeIndex;
		int		m_lPlaceholderType;
		int		m_lPlaceholderID;

		// граница
        Aggplus::RECT m_oBounds;

		// настройки по-умолчанию
		CTextAttributes			m_oAttributes;
		std::vector<CParagraph>	m_arParagraphs;

		// параметры ВСЕГО текста (наверное тут их много будет потом)
        bool	m_bVertical;

		int		m_lWrapMode; // 0 - square, default; 1 - none wrap

		// для ппт. чтобы не менять счас ничего
		CTextRuler				m_oRuler;

		CTextStyles				m_oLayoutStyles;
		CTextStyles				m_oStyles;

		// из пптх
		bool					m_bIsSlideFontRef;
		int						m_lFontRef;

	public:
		CTextAttributesEx() :
			m_oAttributes(),
			m_arParagraphs(),
			m_oRuler(),
			m_oLayoutStyles(),
			m_oStyles()
		{
			m_lTextType			= -1;
			m_lPlaceholderType	= -1;
			m_lPlaceholderID	= -1;

			m_lStyleThemeIndex	= 0;//default

			m_lFontRef			= -1;
			m_bIsSlideFontRef	= false;
					
			m_oBounds.left		= 0;
			m_oBounds.top		= 0;
			m_oBounds.right		= 50;
			m_oBounds.bottom	= 50;

			m_bVertical			= false;
			m_lWrapMode			= 0;

			m_lTextMasterType	= -1;
		}
		CTextAttributesEx& operator =(const CTextAttributesEx& oSrc)
		{
			m_oBounds		= oSrc.m_oBounds;

			m_lTextType			= oSrc.m_lTextType;
			m_lPlaceholderType	= oSrc.m_lPlaceholderType;
			m_lPlaceholderID	= oSrc.m_lPlaceholderID;

			m_lFontRef			= oSrc.m_lFontRef;
			m_bIsSlideFontRef	= oSrc.m_bIsSlideFontRef;

			m_oAttributes	= oSrc.m_oAttributes;
			m_bVertical		= oSrc.m_bVertical;
			m_lWrapMode		= oSrc.m_lWrapMode;

			m_arParagraphs.insert(m_arParagraphs.end(), oSrc.m_arParagraphs.begin(), oSrc.m_arParagraphs.end());
			m_oRuler = oSrc.m_oRuler;

			m_oLayoutStyles		= oSrc.m_oLayoutStyles;
			m_oStyles			= oSrc.m_oStyles;

			m_lTextMasterType = oSrc.m_lTextMasterType;

			return *this;
		}

		CTextAttributesEx(const CTextAttributesEx& oSrc)
		{
			*this = oSrc;
		}

		~CTextAttributesEx()
		{
			m_arParagraphs.clear();
		}

	public:

		inline void NormalizeString(CString& strText)
		{
			strText.Replace(_T("&"),	_T("&amp;"));
			strText.Replace(_T("'"),	_T("&apos;"));
			strText.Replace(_T("<"),	_T("&lt;"));
			strText.Replace(_T(">"),	_T("&gt;"));
			strText.Replace(_T("\""),	_T("&quot;"));
		}

		void RecalcParagraphs	(CTheme* pTheme = NULL);
		void RecalcParagraphsPPT();
		void ApplyThemeStyle	(CTheme* pTheme = NULL);
		
		void ApplyRuler			(CTheme* pTheme);
		void ApplyRuler			(CTextPFRun* pPar, WORD lIndentLevel);
		bool IsEmptyText()
		{
			return (0 == m_arParagraphs.size()) ? true : false;
		}
	};
}

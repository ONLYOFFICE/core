#pragma once
#include "Structures.h"
#include "../../Common/DocxFormat/Source/Base/Nullable.h"

namespace NSPresentationEditor
{

	#define DEFAULT_BEFORE(EXIST_PARAM, PARAM)  \
	if (!EXIST_PARAM && oSrc.##EXIST_PARAM)     \
	{										    \
		EXIST_PARAM = true;						\
		PARAM = oSrc.##PARAM;					\
	}

	#define APPLY_AFTER(EXIST_PARAM, PARAM)		\
	if (oSrc.##EXIST_PARAM)						\
	{										    \
		EXIST_PARAM = true;						\
		PARAM = oSrc.##PARAM;					\
	}

	static CString ToNode(const NSCommon::nullable_base<WORD>& prop, const CString& strName)
	{
		if (!prop.is_init())
			return _T("");
		
		int n = (int)prop.get();
		CString strVal = XmlUtils::IntToString(n);
		return _T("<") + strName + _T(">") + strVal + _T("</") + strName + _T(">");
	}
	static CString ToNode(const NSCommon::nullable_base<LONG>& prop, const CString& strName)
	{
		if (!prop.is_init())
			return _T("");
		
		int n = (int)prop.get();
		CString strVal = XmlUtils::IntToString(n);
		return _T("<") + strName + _T(">") + strVal + _T("</") + strName + _T(">");
	}
	static CString ToNode(const NSCommon::nullable_base<DWORD>& prop, const CString& strName)
	{
		if (!prop.is_init())
			return _T("");
		
		CString strVal = XmlUtils::UIntToString((size_t)prop.get());
		return _T("<") + strName + _T(">") + strVal + _T("</") + strName + _T(">");
	}
	static CString ToNode(const NSCommon::nullable_base<double>& prop, const CString& strName)
	{
		if (!prop.is_init())
			return _T("");
		
		CString strVal = XmlUtils::DoubleToString(prop.get());
		return _T("<") + strName + _T(">") + strVal + _T("</") + strName + _T(">");
	}
	static CString ToNode(const NSCommon::nullable_base<CColor>& prop, const CString& strName)
	{
		if (!prop.is_init())
			return _T("");
		
		CString strVal = XmlUtils::UIntToString(prop->GetLONG());
		return _T("<") + strName + _T(">") + strVal + _T("</") + strName + _T(">");
	}

	class CFontProperties
	{
	public:
		BYTE					FontNameProp;
		std::wstring			strFontName;
		std::wstring			strPanose;
		std::wstring			strPitchFamily;
		LONG					lFontFixed;
		std::vector<BYTE>		arFontCharsets;

	public:
		CFontProperties() : FontNameProp(0), strFontName(_T("")), strPanose(_T("")), strPitchFamily(_T("")), lFontFixed(0), arFontCharsets()
		{
		}
		~CFontProperties()
		{
		}
		CFontProperties(const CFontProperties& oSrc)
		{
			*this = oSrc;
		}
		CFontProperties& operator=(const CFontProperties& oSrc)
		{
			FontNameProp		= oSrc.FontNameProp;
			strFontName			= oSrc.strFontName;
			strPanose			= oSrc.strPanose;
			strPitchFamily		= oSrc.strPitchFamily;
			lFontFixed			= oSrc.lFontFixed;

			for (int i =0 ; i< oSrc.arFontCharsets.size(); i++)
				arFontCharsets.push_back(oSrc.arFontCharsets[i]);

			return *this;
		}

		AVSINLINE void SetFont(CFont& oFont)
		{
			strFontName		= oFont.Name;
			strPanose		= oFont.Panose;
			strPitchFamily	= oFont.PitchFamily;
			lFontFixed		= (LONG)oFont.Monospace;
			arFontCharsets.clear();
			arFontCharsets.push_back(oFont.Charset);
		}
	};

	class CTextCFRun
	{
	public:

        NSCommon::nullable_base<bool>			FontBold;
        NSCommon::nullable_base<bool>			FontItalic;
        NSCommon::nullable_base<bool>			FontUnderline;
        NSCommon::nullable_base<bool>			FontStrikeout;
        NSCommon::nullable_base<bool>			FontShadow;
		
		NSCommon::nullable_base<WORD>			Typeface;			// fontRef
		NSCommon::nullable_base<WORD>			EAFontRef;			// eaFontRef
		NSCommon::nullable_base<WORD>			AnsiFontRef;		// ansiFontRef
		NSCommon::nullable_base<WORD>			SymbolFontRef;		// SymbolFontRef

		NSCommon::nullable_base<double>			BaseLineOffset;		// baseline offset

		NSCommon::nullable_base<CColor>			Color;				// text color
		NSCommon::nullable_base<WORD>			Size;				// font size


		NSCommon::nullable_base<WORD>			Cap;				// 0 - none, 1 - TEXT, 2 - text

		NSCommon::nullable_base<CFontProperties>	FontProperties;
		NSCommon::nullable_base<WORD>			Language;

	public:

		CTextCFRun() : FontBold(), FontItalic(), FontUnderline(), FontStrikeout(), FontShadow(),
			Typeface(), EAFontRef(), AnsiFontRef(), SymbolFontRef(),
			BaseLineOffset(), Color(), Size(), FontProperties(), Cap()
		{
		}
		CTextCFRun(const CTextCFRun& oSrc)
		{
			*this = oSrc;
		}
		CTextCFRun& operator=(const CTextCFRun& oSrc)
		{
			FontBold		= oSrc.FontBold;
			FontItalic		= oSrc.FontItalic;
			FontUnderline	= oSrc.FontUnderline;
			FontStrikeout	= oSrc.FontStrikeout;
			FontShadow		= oSrc.FontShadow;
			
			Typeface		= oSrc.Typeface;
			EAFontRef		= oSrc.EAFontRef;
			AnsiFontRef		= oSrc.AnsiFontRef;
			SymbolFontRef	= oSrc.SymbolFontRef;

			BaseLineOffset	= oSrc.BaseLineOffset;

			Color			= oSrc.Color;
			Size			= oSrc.Size;

			FontProperties	= oSrc.FontProperties;

			Cap				= oSrc.Cap;

			Language		= oSrc.Language;

			return *this;
		}
		~CTextCFRun()
		{
		}
		
		AVSINLINE void ApplyBefore(const CTextCFRun& oSrc)
		{
			if (!FontBold.is_init()) 				FontBold = oSrc.FontBold;
			if (!FontItalic.is_init())				FontItalic = oSrc.FontItalic;
			if (!FontUnderline.is_init())			FontUnderline = oSrc.FontUnderline;
			if (!FontStrikeout.is_init())			FontStrikeout = oSrc.FontStrikeout;
			if (!FontShadow.is_init())				FontShadow = oSrc.FontShadow;
			
			if (!Typeface.is_init())				Typeface = oSrc.Typeface;
			if (!EAFontRef.is_init())				EAFontRef = oSrc.EAFontRef;
			if (!AnsiFontRef.is_init())				AnsiFontRef = oSrc.AnsiFontRef;
			if (!SymbolFontRef.is_init())			SymbolFontRef = oSrc.SymbolFontRef;

			if (!BaseLineOffset.is_init())			BaseLineOffset = oSrc.BaseLineOffset;

			if (!Color.is_init())					Color = oSrc.Color;

			if (!Size.is_init())					Size = oSrc.Size;

			if (!Cap.is_init())						Cap = oSrc.Cap;

			if (!FontProperties.is_init())			FontProperties = oSrc.FontProperties;

			if (!Language.is_init())				Language = oSrc.Language;
		}
		AVSINLINE void ApplyAfter(const CTextCFRun& oSrc)
		{
			if (oSrc.FontBold.is_init())			FontBold = oSrc.FontBold;
			if (oSrc.FontItalic.is_init())			FontItalic = oSrc.FontItalic;
			if (oSrc.FontUnderline.is_init())		FontUnderline = oSrc.FontUnderline;
			if (oSrc.FontStrikeout.is_init())		FontStrikeout = oSrc.FontStrikeout;
			if (oSrc.FontShadow.is_init())			FontShadow = oSrc.FontShadow;
			
			bool bTypefaceSetUp = false;
			if (oSrc.Typeface.is_init())
			{
				Typeface = oSrc.Typeface;
				bTypefaceSetUp = true;
			}
			if (oSrc.EAFontRef.is_init())			EAFontRef = oSrc.EAFontRef;
			if (oSrc.AnsiFontRef.is_init())			AnsiFontRef = oSrc.AnsiFontRef;
			if (oSrc.SymbolFontRef.is_init())		SymbolFontRef = oSrc.SymbolFontRef;

			if (oSrc.BaseLineOffset.is_init())		BaseLineOffset = oSrc.BaseLineOffset;

			if (oSrc.Color.is_init())				Color = oSrc.Color;

			if (oSrc.Size.is_init())				Size = oSrc.Size;

			if (oSrc.Cap.is_init())					Cap = oSrc.Cap;

			if (oSrc.FontProperties.is_init())
			{
				FontProperties = oSrc.FontProperties;
				if (!bTypefaceSetUp)
					Typeface.reset();
			}
			if (oSrc.Language.is_init())
				Language = oSrc.Language;
		}

		CString ToString(LONG lCount)
		{
			NSPresentationEditor::CXmlWriter oWriter;
                        oWriter.WriteNodeBegin(_T("Character"), true);
			oWriter.WriteAttributeLONG(_T("count"), lCount);
                        oWriter.WriteNodeEnd(_T("Character"), true, false);

			oWriter.WriteNodeBegin(_T("Attributes"));

			int nBold	= 0;
			int nItalic = 0;
			
			CString strXml = _T("");

			if (FontBold.is_init())
			{
				if (FontBold.get())
				{
					strXml += _T("<font-bold>1</font-bold>");
					nBold = 1;
				}
			}
			if (FontItalic.is_init())
			{
				if (FontItalic.get())
				{
					strXml += _T("<font-italic>1</font-italic>");
					nItalic = 1;
				}
			}
			if (FontUnderline.is_init())
			{
				if (FontUnderline.get())
				{
					strXml += _T("<font-underline>1</font-underline>");
				}
			}
			if (FontStrikeout.is_init())
			{
				if (FontStrikeout.get())
				{
					strXml += _T("<font-strikeout>1</font-strikeout>");
				}
			}
			if (FontShadow.is_init())
			{
				if (FontShadow.get())
				{
					strXml += _T("<shadow-visible>1</shadow-visible>");
				}
			}

			strXml += ToNode(BaseLineOffset, _T("baseline-shift"));
			strXml += ToNode(Color, _T("brush-color1"));
			strXml += ToNode(Size, _T("font-size"));

			if (FontProperties.is_init())
			{
				strXml += (_T("<font-name>") + std_string2string(FontProperties->strFontName) + _T("</font-name>"));

				oWriter.WriteString(strXml);
				oWriter.WriteNodeEnd(_T("Attributes"));

				// font properties
				strXml = _T("<FontProperties>");
				strXml += (_T("<Name value='") + std_string2string(FontProperties->strFontName) + _T("' />"));

				if (0 < FontProperties->arFontCharsets.size())
					strXml += (_T("<Charset value='") + XmlUtils::IntToString((int)FontProperties->arFontCharsets[0]) + _T("' />"));

				if (_T("unknown") != FontProperties->strPitchFamily)
					strXml += (_T("<FamilyClass name='") + std_string2string(FontProperties->strPitchFamily) + _T("' />"));

				if (-1 != FontProperties->lFontFixed)
					strXml += (_T("<FixedWidth value='") + XmlUtils::IntToString(FontProperties->lFontFixed) + _T("' />"));

				if (_T("") != FontProperties->strPanose)
					strXml += (_T("<Panose value='") + std_string2string(FontProperties->strPanose) + _T("' />"));

				strXml += (_T("<Style bold='") + XmlUtils::IntToString(nBold) + _T("' italic='") + XmlUtils::IntToString(nItalic) + _T("' />"));

				strXml += _T("</FontProperties>");
			}
			else
			{
				oWriter.WriteNodeEnd(_T("Attributes"));
			}

			oWriter.WriteString(strXml);

			oWriter.WriteNodeEnd(_T("Character"));
			return oWriter.GetXmlString();
		}

	};

	class CTextPFRun
	{
	public:

		NSCommon::nullable_base<bool>				hasBullet;

		NSCommon::nullable_base<WORD>				bulletFontRef;
		NSCommon::nullable_base<WORD>				bulletSize;
		NSCommon::nullable_base<WCHAR>				bulletChar;
		NSCommon::nullable_base<CColor>				bulletColor;
		NSCommon::nullable_base<CFontProperties>	bulletFontProperties;

		NSCommon::nullable_base<WORD>				textAlignment;
		NSCommon::nullable_base<LONG>				lineSpacing;
		NSCommon::nullable_base<LONG>				spaceBefore;
		NSCommon::nullable_base<LONG>				spaceAfter;
		NSCommon::nullable_base<LONG>				leftMargin;
		NSCommon::nullable_base<LONG>				indent;
		NSCommon::nullable_base<LONG>				defaultTabSize;
		
		std::vector<DWORD>							tabStops;

		bool										bIsOneLine;

		NSCommon::nullable_base<WORD>				fontAlign;
		NSCommon::nullable_base<WORD>				textDirection;

		NSCommon::nullable_base<WORD>				wrapFlags;

		CTextPFRun() : hasBullet(), bulletFontRef(), bulletSize(), bulletChar(), bulletColor(),
			textAlignment(), lineSpacing(), spaceBefore(), spaceAfter(), leftMargin(), indent(),
			defaultTabSize(), tabStops(), bIsOneLine(false), fontAlign(), wrapFlags()
		{
		}

		CTextPFRun(const CTextPFRun& oSrc)
		{
			*this = oSrc;
		}

		CTextPFRun& operator =(const CTextPFRun& oSrc)
		{
			hasBullet			= oSrc.hasBullet;

			bulletFontRef		= oSrc.bulletFontRef;
			bulletSize			= oSrc.bulletSize;
			bulletChar			= oSrc.bulletChar;
			bulletColor			= oSrc.bulletColor;
			bulletFontProperties = oSrc.bulletFontProperties;

			textAlignment		= oSrc.textAlignment;
			lineSpacing			= oSrc.lineSpacing;
			spaceBefore			= oSrc.spaceBefore;
			spaceAfter			= oSrc.spaceAfter;
			leftMargin			= oSrc.leftMargin;
			indent				= oSrc.indent;
			defaultTabSize		= oSrc.defaultTabSize;

			tabStops			= oSrc.tabStops;

			fontAlign			= oSrc.fontAlign;
			textDirection		= oSrc.textDirection;
			wrapFlags			= oSrc.wrapFlags;

			bIsOneLine			= oSrc.bIsOneLine;

			return *this;
		}

		void ApplyBefore(const CTextPFRun& oSrc)
		{
			if (!hasBullet.is_init())				hasBullet			= oSrc.hasBullet;

			if (!bulletSize.is_init())				bulletSize			= oSrc.bulletSize;
			if (!bulletChar.is_init())
			{
				bulletChar			= oSrc.bulletChar;
				bulletFontRef		= oSrc.bulletFontRef;
				bulletFontProperties.reset();
			}
			if (!bulletColor.is_init())				bulletColor			= oSrc.bulletColor;

			if (!textAlignment.is_init())			textAlignment = oSrc.textAlignment;
			if (!lineSpacing.is_init())				lineSpacing = oSrc.lineSpacing;
			if (!spaceBefore.is_init())				spaceBefore = oSrc.spaceBefore;
			if (!spaceAfter.is_init())				spaceAfter = oSrc.spaceAfter;
			if (!leftMargin.is_init())				leftMargin = oSrc.leftMargin;
			if (!indent.is_init())					indent = oSrc.indent;
			if (!defaultTabSize.is_init())			defaultTabSize = oSrc.defaultTabSize;

			if (!fontAlign.is_init())				fontAlign = oSrc.fontAlign;
			if (!textDirection.is_init())			textDirection = oSrc.textDirection;
			if (!wrapFlags.is_init())				wrapFlags = oSrc.wrapFlags;

			if ((0 == tabStops.size()) && (0 != oSrc.tabStops.size()))
			{
				tabStops.insert(tabStops.end(), oSrc.tabStops.begin(), oSrc.tabStops.end());
			}
		}

		void ApplyAfter(const CTextPFRun& oSrc)
		{
			if (oSrc.hasBullet.is_init())			hasBullet = oSrc.hasBullet;

			if (oSrc.bulletSize.is_init())			bulletSize = oSrc.bulletSize;
			if (oSrc.bulletChar.is_init())
			{
				bulletChar		= oSrc.bulletChar;
				bulletFontRef	= oSrc.bulletFontRef;
				bulletFontProperties.reset();
			}
			if (oSrc.bulletColor.is_init())			bulletColor		= oSrc.bulletColor;

			if (oSrc.textAlignment.is_init())		textAlignment	= oSrc.textAlignment;
			if (oSrc.lineSpacing.is_init())			lineSpacing		= oSrc.lineSpacing;
			if (oSrc.spaceBefore.is_init())			spaceBefore		= oSrc.spaceBefore;
			if (oSrc.spaceAfter.is_init())			spaceAfter		= oSrc.spaceAfter;
			if (oSrc.leftMargin.is_init())			leftMargin		= oSrc.leftMargin;
			if (oSrc.indent.is_init())				indent			= oSrc.indent;
			if (oSrc.defaultTabSize.is_init())		defaultTabSize	= oSrc.defaultTabSize;

			if (oSrc.fontAlign.is_init())			fontAlign		= oSrc.fontAlign;
			if (oSrc.textDirection.is_init())		textDirection	= oSrc.textDirection;
			if (oSrc.wrapFlags.is_init())			wrapFlags		= oSrc.wrapFlags;

			if (0 != oSrc.tabStops.size())
			{
				tabStops.insert(tabStops.begin(), oSrc.tabStops.begin(), oSrc.tabStops.end());
			}
		}

		CString ToString(LONG lCount)
		{
			return L"";
		}

	};

	class CTextRuler
	{
	public:
		NSCommon::nullable_base<LONG>	DefaultTabSize;
		NSCommon::nullable_base<SHORT>	CLevels;
		NSCommon::nullable_base<SHORT>	TabStops;

		NSCommon::nullable_base<LONG>	LeftMargin1;
		NSCommon::nullable_base<LONG>	LeftMargin2;
		NSCommon::nullable_base<LONG>	LeftMargin3;
		NSCommon::nullable_base<LONG>	LeftMargin4;
		NSCommon::nullable_base<LONG>	LeftMargin5;

		NSCommon::nullable_base<LONG>	Indent1;
		NSCommon::nullable_base<LONG>	Indent2;
		NSCommon::nullable_base<LONG>	Indent3;
		NSCommon::nullable_base<LONG>	Indent4;
		NSCommon::nullable_base<LONG>	Indent5;

		std::vector<DWORD> tabsStops;

	public:
		CTextRuler() : DefaultTabSize(), CLevels(), TabStops(),
			LeftMargin1(), LeftMargin2(), LeftMargin3(), LeftMargin4(), LeftMargin5(),
			Indent1(), Indent2(), Indent3(), Indent4(), Indent5(), tabsStops()
		{
		}

		CTextRuler(const CTextRuler& oSrc)
		{
			*this = oSrc;
		}

		CTextRuler& operator =(const CTextRuler& oSrc)
		{
			DefaultTabSize	= oSrc.DefaultTabSize;
			CLevels			= oSrc.CLevels;
			TabStops		= oSrc.TabStops;

			LeftMargin1 = oSrc.LeftMargin1;
			LeftMargin2 = oSrc.LeftMargin2;
			LeftMargin3 = oSrc.LeftMargin3;
			LeftMargin4 = oSrc.LeftMargin4;
			LeftMargin5 = oSrc.LeftMargin5;

			Indent1 = oSrc.Indent1;
			Indent2 = oSrc.Indent2;
			Indent3 = oSrc.Indent3;
			Indent4 = oSrc.Indent4;
			Indent5 = oSrc.Indent5;

			tabsStops = oSrc.tabsStops;

			return *this;
		}
		~CTextRuler()
		{
			tabsStops.clear();
		}

		void ApplyBefore(const CTextRuler& oSrc)
		{
			if (!DefaultTabSize.is_init())			DefaultTabSize = oSrc.DefaultTabSize;
			if (!CLevels.is_init())					CLevels = oSrc.CLevels;
			
			if (!LeftMargin1.is_init())				LeftMargin1 = oSrc.LeftMargin1;
			if (!LeftMargin2.is_init())				LeftMargin2 = oSrc.LeftMargin2;
			if (!LeftMargin3.is_init())				LeftMargin3 = oSrc.LeftMargin3;
			if (!LeftMargin4.is_init())				LeftMargin4 = oSrc.LeftMargin4;
			if (!LeftMargin5.is_init())				LeftMargin5 = oSrc.LeftMargin5;

			if (!Indent1.is_init())					Indent1 = oSrc.Indent1;
			if (!Indent2.is_init())					Indent2 = oSrc.Indent2;
			if (!Indent3.is_init())					Indent3 = oSrc.Indent3;
			if (!Indent4.is_init())					Indent4 = oSrc.Indent4;
			if (!Indent5.is_init())					Indent5 = oSrc.Indent5;

			tabsStops.insert(tabsStops.end(), oSrc.tabsStops.begin(), oSrc.tabsStops.end());
		}

		CString ToString()
		{
			return L"";
		}
	};

	class CTextSIRun
	{
	public:
		bool bIsExt;
		DWORD lCount;

		bool bSpell;
		bool bLang;
		bool bAltLang;

		bool bPp10ext;
		bool bBidi;
		bool bSmartTag;

		WORD Spell;
		WORD Lang;
		WORD AltLang;

		WORD Bidi;
		BYTE pp10runid;

		bool bGramma;

		std::vector<DWORD> arSmartTags;

	public:

		CTextSIRun()
		{
			bIsExt = true;
			lCount  = 0;

			bSpell = false;
			bLang = false;
			bAltLang = false;

			bPp10ext = false;
			bBidi = false;
			bSmartTag = false;

			Spell = 0;
			Lang = 0;
			AltLang = 0;

			Bidi = 0;
			pp10runid = 0;

			bGramma = false;
		}

		CTextSIRun(const CTextSIRun& oSrc)
		{
			*this = oSrc;
		}

		CTextSIRun& operator =(const CTextSIRun& oSrc)
		{
			bIsExt		= oSrc.bIsExt;
			lCount		= oSrc.lCount;

			bSpell		= oSrc.bSpell;
			bLang		= oSrc.bLang;
			bAltLang	= oSrc.bAltLang;

			bPp10ext	= oSrc.bPp10ext;
			bBidi		= oSrc.bBidi;
			bSmartTag	= oSrc.bSmartTag;

			Spell		= oSrc.Spell;
			Lang		= oSrc.Lang;
			AltLang		= oSrc.AltLang;

			Bidi		= oSrc.Bidi;
			pp10runid	= oSrc.pp10runid;

			bGramma		= oSrc.bGramma;

			arSmartTags = oSrc.arSmartTags;

			return *this;
		}
		
		void ApplyBefore(const CTextSIRun& oSrc)
		{
            /*
             * gcc 4.8 doesn't understand this construction:
             *
                        DEFAULT_BEFORE(bSpell, Spell);
                        DEFAULT_BEFORE(bLang, Lang);
                        DEFAULT_BEFORE(bAltLang, AltLang);
                        DEFAULT_BEFORE(bBidi, Bidi);
                        DEFAULT_BEFORE(bPp10ext, pp10runid);
            */
            if (!bSpell && oSrc.bSpell)
            {
                bSpell = true;
                Spell = oSrc.Spell;
            }
            if (!bLang && oSrc.bLang)
            {
                bLang = true;
                Lang = oSrc.Lang;
            }
            if (!bAltLang && oSrc.bAltLang)
            {
                bAltLang = true;
                AltLang = oSrc.AltLang;
            }
            if (!bBidi && oSrc.bBidi)
            {
                bBidi = true;
                Bidi = oSrc.Bidi;
            }
            if (!bPp10ext && oSrc.bPp10ext)
            {
                bPp10ext = true;
                pp10runid = oSrc.pp10runid;
            }


			if (!bGramma)
			{
				bGramma = oSrc.bGramma;			
			}
		}

		CString ToString()
		{
			return L"";
		}
	};


	class CSpan
	{
	public:
		CTextCFRun			m_oRun;
		std::wstring		m_strText;

	public:
		CSpan() : m_oRun(), m_strText(_T(""))
		{
		}
		CSpan(const CSpan& oSrc)
		{
			*this = oSrc;
		}
		CSpan& operator=(const CSpan& oSrc)
		{
			m_oRun		= oSrc.m_oRun;
			m_strText	= oSrc.m_strText;

			return *this;
		}
		~CSpan()
		{
		}
	};

	class CTextStyleLevel
	{
	public:
		CTextPFRun m_oPFRun;
		CTextCFRun m_oCFRun;

	public:
		CTextStyleLevel() : m_oPFRun(), m_oCFRun()
		{
		}
		CTextStyleLevel(const CTextStyleLevel& oSrc)
		{
			*this = oSrc;
		}
		CTextStyleLevel& operator=(const CTextStyleLevel& oSrc)
		{
			m_oPFRun	= oSrc.m_oPFRun;
			m_oCFRun	= oSrc.m_oCFRun;

			return *this;
		}

		void ApplyAfter(const CTextStyleLevel& oSrc)
		{
			m_oPFRun.ApplyAfter(oSrc.m_oPFRun);
			m_oCFRun.ApplyAfter(oSrc.m_oCFRun);
		}
		void ApplyBefore(const CTextStyleLevel& oSrc)
		{
			m_oPFRun.ApplyBefore(oSrc.m_oPFRun);
			m_oCFRun.ApplyBefore(oSrc.m_oCFRun);
		}
	};

	//нигде не применяется ???
	//class CTextFullInfo
	//{
	//public:
	//	CTextPFRun m_oPF;
	//	CTextCFRun m_oCF;
	//	CTextRuler m_oRuler;
	//	CTextSIRun m_oSI;

	//public:
	//	CTextFullInfo() : m_oPF(), m_oCF(), m_oRuler(), m_oSI()
	//	{
	//	}

	//	CTextFullInfo(const CTextFullInfo& oSrc)
	//	{
	//		*this = oSrc;
	//	}

	//	CTextFullInfo& operator=(const CTextFullInfo& oSrc)
	//	{
	//		m_oPF		= oSrc.m_oPF;
	//		m_oCF		= oSrc.m_oCF;
	//		m_oRuler	= oSrc.m_oRuler;
	//		m_oSI		= oSrc.m_oSI;
	//		return *this;
	//	}
	//};
	
	class CTextStyles
	{
	public:
		NSCommon::nullable_base<CTextStyleLevel> m_pLevels[10];

	public:
		CTextStyles()
		{
			for (int i = 0; i < 10; ++i)
			{
				m_pLevels[i] = NULL;
			}
		}
		CTextStyles(const CTextStyles& oSrc)
		{
			*this = oSrc;
		}
		CTextStyles& operator=(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
			{
				m_pLevels[i] = oSrc.m_pLevels[i];
			}
			return *this;
		}
		void SetStyles(CTextStyles* pStyles)
		{
			for (int i = 0; i < 10; ++i)
			{
				m_pLevels[i] = pStyles->m_pLevels[i];
			}
		}
		void SetLanguage(nullable<WORD> & language)
		{
			if (!language.is_init()) return;

			for (int i = 0; i < 10; ++i)
			{
				if (m_pLevels[i].is_init())
				{
					m_pLevels[i]->m_oCFRun.Language = language;
				}
			}
		}
		void ApplyAfter(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (!m_pLevels[i].is_init())
				{
					m_pLevels[i] = oSrc.m_pLevels[i];
				}
				else if (oSrc.m_pLevels[i].is_init())
				{
					m_pLevels[i]->ApplyAfter(oSrc.m_pLevels[i].get());
				}

			}
		}
		void ApplyBefore(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (!m_pLevels[i].is_init())
				{
					m_pLevels[i] = oSrc.m_pLevels[i];
				}
				else if (oSrc.m_pLevels[i].is_init())
				{
					m_pLevels[i]->ApplyBefore(oSrc.m_pLevels[i].get());
				}

			}
		}
	};

	class CParagraph
	{
	public:
		int m_lTextType;
		int m_lTextLevel;
		int m_lStyleThemeIndex;

		CTextPFRun			m_oPFRun;
		std::vector<CSpan>	m_arSpans;

	public:
		CParagraph() : m_oPFRun(), m_arSpans()
		{
			m_lTextType			= 0;
			m_lTextLevel		= 0;
			m_lStyleThemeIndex	= 0;
		}
		CParagraph(const CParagraph& oSrc)
		{
			*this = oSrc;
		}
		CParagraph& operator=(const CParagraph& oSrc)
		{
			m_lTextLevel		= oSrc.m_lTextLevel;
			m_lTextType			= oSrc.m_lTextType;
			m_lStyleThemeIndex	= oSrc.m_lStyleThemeIndex;

			m_oPFRun			= oSrc.m_oPFRun;

			m_arSpans			= oSrc.m_arSpans;

			return *this;
		}
		~CParagraph()
		{
			m_arSpans.clear();
		}
		AVSINLINE void CheckErrors()
		{
			//if (IsEmpty())
			//{
   //             m_oPFRun.hasBullet = false;
			//}
			size_t nCountS = m_arSpans.size();
			for (size_t i = 0; i < nCountS; ++i)
			{
				std::replace( m_arSpans[i].m_strText.begin(), m_arSpans[i].m_strText.end(), (TCHAR)(11), (TCHAR)(13)); 
			}
		}
		AVSINLINE bool IsEmpty()
		{
			size_t nCountSpans = m_arSpans.size();
			for (size_t i = 0; i < nCountSpans; ++i)
			{
				int nLen = m_arSpans[i].m_strText.length();

				if (nLen > 1)
					return false;

				if ((nLen == 1) && ((TCHAR)(13) != m_arSpans[i].m_strText[0]))
					return false;
			}
			return true;
		}
	};
}

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
		BYTE			FontNameProp;
		CString			strFontName;
		CString			strPanose;
		CString			strPitchFamily;
		LONG			lFontFixed;
		std::vector<BYTE> arFontCharsets;

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

			return *this;
		}
		~CTextCFRun()
		{
		}
		
		AVSINLINE void ApplyBefore(const CTextCFRun& oSrc)
		{
			if (!FontBold.is_init())
				FontBold = oSrc.FontBold;
			if (!FontItalic.is_init())
				FontItalic = oSrc.FontItalic;
			if (!FontUnderline.is_init())
				FontUnderline = oSrc.FontUnderline;
			if (!FontStrikeout.is_init())
				FontStrikeout = oSrc.FontStrikeout;
			if (!FontShadow.is_init())
				FontShadow = oSrc.FontShadow;
			
			if (!Typeface.is_init())
				Typeface = oSrc.Typeface;
			if (!EAFontRef.is_init())
				EAFontRef = oSrc.EAFontRef;
			if (!AnsiFontRef.is_init())
				AnsiFontRef = oSrc.AnsiFontRef;
			if (!SymbolFontRef.is_init())
				SymbolFontRef = oSrc.SymbolFontRef;

			if (!BaseLineOffset.is_init())
				BaseLineOffset = oSrc.BaseLineOffset;

			if (!Color.is_init())
				Color = oSrc.Color;

			if (!Size.is_init())
				Size = oSrc.Size;

			if (!Cap.is_init())
				Cap = oSrc.Cap;

			if (!FontProperties.is_init())
				FontProperties = oSrc.FontProperties;
		}
		AVSINLINE void ApplyAfter(const CTextCFRun& oSrc)
		{
			if (oSrc.FontBold.is_init())
				FontBold = oSrc.FontBold;
			if (oSrc.FontItalic.is_init())
				FontItalic = oSrc.FontItalic;
			if (oSrc.FontUnderline.is_init())
				FontUnderline = oSrc.FontUnderline;
			if (oSrc.FontStrikeout.is_init())
				FontStrikeout = oSrc.FontStrikeout;
			if (oSrc.FontShadow.is_init())
				FontShadow = oSrc.FontShadow;
			
			bool bTypefaceSetUp = false;
			if (oSrc.Typeface.is_init())
			{
				Typeface = oSrc.Typeface;
				bTypefaceSetUp = true;
			}
			if (oSrc.EAFontRef.is_init())
				EAFontRef = oSrc.EAFontRef;
			if (oSrc.AnsiFontRef.is_init())
				AnsiFontRef = oSrc.AnsiFontRef;
			if (oSrc.SymbolFontRef.is_init())
				SymbolFontRef = oSrc.SymbolFontRef;

			if (oSrc.BaseLineOffset.is_init())
				BaseLineOffset = oSrc.BaseLineOffset;

			if (oSrc.Color.is_init())
				Color = oSrc.Color;

			if (oSrc.Size.is_init())
				Size = oSrc.Size;

			if (oSrc.Cap.is_init())
				Cap = oSrc.Cap;

			if (oSrc.FontProperties.is_init())
			{
				FontProperties = oSrc.FontProperties;
				if (!bTypefaceSetUp)
					Typeface.reset();
			}
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
				strXml += (_T("<font-name>") + FontProperties->strFontName + _T("</font-name>"));

				oWriter.WriteString(strXml);
				oWriter.WriteNodeEnd(_T("Attributes"));

				// font properties
				strXml = _T("<FontProperties>");
				strXml += (_T("<Name value='") + FontProperties->strFontName + _T("' />"));

				if (0 < FontProperties->arFontCharsets.size())
					strXml += (_T("<Charset value='") + XmlUtils::IntToString((int)FontProperties->arFontCharsets[0]) + _T("' />"));

				if (_T("unknown") != FontProperties->strPitchFamily)
					strXml += (_T("<FamilyClass name='") + FontProperties->strPitchFamily + _T("' />"));

				if (-1 != FontProperties->lFontFixed)
					strXml += (_T("<FixedWidth value='") + XmlUtils::IntToString(FontProperties->lFontFixed) + _T("' />"));

				if (_T("") != FontProperties->strPanose)
					strXml += (_T("<Panose value='") + FontProperties->strPanose + _T("' />"));

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

		CString ToXmlEditor(CString& strClass, CString& strText)
		{
			CString strStyle = _T("");
			
			if (FontBold.is_init())
			{
				if (FontBold.get())
				{
					strStyle += _T("font-weight: bold;");
				}
				else
				{
					strStyle += _T("font-weight: normal;");
				}
			}
			if (FontItalic.is_init())
			{
				if (FontItalic.get())
				{
					strStyle += _T("font-style: italic;");
				}
				else
				{
					strStyle += _T("font-style: normal;");
				}
			}
			if (FontUnderline.is_init())
			{
				if (FontUnderline.get())
				{
					strStyle += _T("text-decoration: underline;");
				}
				else
				{
					strStyle += _T("text-decoration: none;");
				}
			}
			/*if (FontStrikeout.is_init())
			{
				if (FontStrikeout.get())
				{
					strStyle += _T("text-decoration: line-through;");
				}
				else
				{
					strStyle += _T("text-decoration: normal;");
				}
			}*/
			if (FontShadow.is_init())
			{
				if (FontShadow.get())
				{
					strStyle += _T("text-shadow: black 1px 1px 2px;");
				}
				else
				{
					strStyle += _T("text-shadow: none;");
				}
			}
			if (Cap.is_init())
			{
				WORD lCap = Cap.get();
				if (1 == lCap)
					strStyle += _T("text-transform: uppercase;");
				else if (2 == lCap)
					strStyle += _T("text-transform: lowercase;");
				else
					strStyle += _T("text-transform: none;");
			}

			if (Size.is_init())
			{
				CString strFontSize = _T("");
				strFontSize.Format(_T("font-size: %dpt;"), (int)Size.get());
				strStyle += strFontSize;			
			}

			if (Color.is_init())
			{
				if (-1 != Color->m_lSchemeIndex)
				{
					CString strTMColor = _T("");
					strTMColor.Format(_T("tm_color:%d;"), Color->m_lSchemeIndex + 1);
					strStyle += strTMColor;

					CString strColor = _T("");
					strColor.Format(_T(" color%d"), Color->m_lSchemeIndex + 1);
					strClass += strColor;
				}
				else
				{
					CString strColor = _T("");
					strColor.Format(_T("color: #%06x;"), (int)Color->GetLONG_RGB());
					strStyle += strColor;
				}
			}

                        bool bIsFontNamPres = (FontProperties.is_init() && (_T("") != FontProperties->strFontName));
			if (Typeface.is_init() && !bIsFontNamPres)
			{
				CString strFont = _T("");
				strFont.Format(_T(" font_d%d"), (long)Typeface.get() + 1);
				strClass += strFont;
			}			
			else if (bIsFontNamPres)
			{
				strStyle += _T("font-family: ");
				strStyle += FontProperties->strFontName;
				strStyle += _T(";");
			}

			strText.Replace((WCHAR)(11), '\n');
			//strText.Replace(_T("\n"), _T("VVODbryakVVOD"));//xml2_reader 
			//strText.Replace(_T("\n"), _T("<br\>"));

			if (_T("\n") == strText)
			{
				//return _T("<br class=\"") + strClass + _T("\" style=\"") + strStyle + _T("\" />");
				return _T("<span class=\"") + strClass + _T("\" style=\"") + strStyle + _T("\"><br/></span>");
			}

			return _T("<span class=\"") + strClass + _T("\" style=\"") + strStyle + _T("\">") + strText + _T("</span>");
		}
	};

	class CTextPFRun
	{
	public:

                NSCommon::nullable_base<bool>		hasBullet;

		NSCommon::nullable_base<WORD>		bulletFontRef;
		NSCommon::nullable_base<WORD>		bulletSize;
		NSCommon::nullable_base<WCHAR>		bulletChar;
		NSCommon::nullable_base<CColor>		bulletColor;

		NSCommon::nullable_base<WORD>		textAlignment;
		NSCommon::nullable_base<LONG>		lineSpacing;
		NSCommon::nullable_base<LONG>		spaceBefore;
		NSCommon::nullable_base<LONG>		spaceAfter;
		NSCommon::nullable_base<LONG>		leftMargin;
		NSCommon::nullable_base<LONG>		indent;
		NSCommon::nullable_base<LONG>		defaultTabSize;

		std::vector<DWORD>					tabStops;

		bool								bIsOneLine;

		NSCommon::nullable_base<WORD>		fontAlign;
		NSCommon::nullable_base<WORD>		textDirection;

		NSCommon::nullable_base<WORD>		wrapFlags;

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
			hasBullet		= oSrc.hasBullet;

			bulletFontRef	= oSrc.bulletFontRef;
			bulletSize		= oSrc.bulletSize;
			bulletChar		= oSrc.bulletChar;
			bulletColor		= oSrc.bulletColor;

			textAlignment	= oSrc.textAlignment;
			lineSpacing		= oSrc.lineSpacing;
			spaceBefore		= oSrc.spaceBefore;
			spaceAfter		= oSrc.spaceAfter;
			leftMargin		= oSrc.leftMargin;
			indent			= oSrc.indent;
			defaultTabSize	= oSrc.defaultTabSize;

			tabStops		= oSrc.tabStops;

			fontAlign		= oSrc.fontAlign;
			textDirection	= oSrc.textDirection;
			wrapFlags		= oSrc.wrapFlags;

			bIsOneLine		= oSrc.bIsOneLine;

			return *this;
		}

		void ApplyBefore(const CTextPFRun& oSrc)
		{
			if (!hasBullet.is_init())
				hasBullet = oSrc.hasBullet;

			if (!bulletFontRef.is_init())
				bulletFontRef = oSrc.bulletFontRef;
			if (!bulletSize.is_init())
				bulletSize = oSrc.bulletSize;
			if (!bulletChar.is_init())
				bulletChar = oSrc.bulletChar;
			if (!bulletColor.is_init())
				bulletColor = oSrc.bulletColor;

			if (!textAlignment.is_init())
				textAlignment = oSrc.textAlignment;
			if (!lineSpacing.is_init())
				lineSpacing = oSrc.lineSpacing;
			if (!spaceBefore.is_init())
				spaceBefore = oSrc.spaceBefore;
			if (!spaceAfter.is_init())
				spaceAfter = oSrc.spaceAfter;
			if (!leftMargin.is_init())
				leftMargin = oSrc.leftMargin;
			if (!indent.is_init())
				indent = oSrc.indent;
			if (!defaultTabSize.is_init())
				defaultTabSize = oSrc.defaultTabSize;

			if (!fontAlign.is_init())
				fontAlign = oSrc.fontAlign;
			if (!textDirection.is_init())
				textDirection = oSrc.textDirection;
			if (!wrapFlags.is_init())
				wrapFlags = oSrc.wrapFlags;

			if ((0 == tabStops.size()) && (0 != oSrc.tabStops.size()))
			{
				tabStops.insert(tabStops.end(), oSrc.tabStops.begin(), oSrc.tabStops.end());
			}
		}

		void ApplyAfter(const CTextPFRun& oSrc)
		{
			if (oSrc.hasBullet.is_init())
				hasBullet = oSrc.hasBullet;

			if (oSrc.bulletFontRef.is_init())
				bulletFontRef = oSrc.bulletFontRef;
			if (oSrc.bulletSize.is_init())
				bulletSize = oSrc.bulletSize;
			if (oSrc.bulletChar.is_init())
				bulletChar = oSrc.bulletChar;
			if (oSrc.bulletColor.is_init())
				bulletColor = oSrc.bulletColor;

			if (oSrc.textAlignment.is_init())
				textAlignment = oSrc.textAlignment;
			if (oSrc.lineSpacing.is_init())
				lineSpacing = oSrc.lineSpacing;
			if (oSrc.spaceBefore.is_init())
				spaceBefore = oSrc.spaceBefore;
			if (oSrc.spaceAfter.is_init())
				spaceAfter = oSrc.spaceAfter;
			if (oSrc.leftMargin.is_init())
				leftMargin = oSrc.leftMargin;
			if (oSrc.indent.is_init())
				indent = oSrc.indent;
			if (oSrc.defaultTabSize.is_init())
				defaultTabSize = oSrc.defaultTabSize;

			if (oSrc.fontAlign.is_init())
				fontAlign = oSrc.fontAlign;
			if (oSrc.textDirection.is_init())
				textDirection = oSrc.textDirection;
			if (oSrc.wrapFlags.is_init())
				wrapFlags = oSrc.wrapFlags;

			if (0 != oSrc.tabStops.size())
			{
				tabStops.insert(tabStops.begin(), oSrc.tabStops.begin(), oSrc.tabStops.end());
			}
		}

		CString ToString(LONG lCount)
		{
			XmlUtils::CXmlWriter oWriter;
                        oWriter.WriteNodeBegin(_T("Paragraph"), true);
			//oWriter.WriteAttribute(_T("type"), CDirectory::ToString(TextType));
			//oWriter.WriteAttribute(_T("level"), CDirectory::ToString(Level));
			oWriter.WriteAttribute(_T("count"), CDirectory::ToString(lCount));
			oWriter.WriteAttribute(_T("oneline"), bIsOneLine ? _T("1") : _T("0"));
			
                        oWriter.WriteNodeEnd(_T("Paragraph"), true, false);
			
			CString strXml = _T("");
                        if (hasBullet.is_init() && (true == hasBullet.get()))
			{
                strXml += (_T("<bulletflag>255</bulletflag>"));
				
				if (!bulletChar.is_init())
				{
                    CString temp = _T("<bulletchar>");
                    temp += _T('\x2022');
                    temp += _T("</bulletchar>");
                    strXml += temp;
				}
			}
			
			if (bulletChar.is_init())
            {
                CString temp = _T("<bulletchar>");
                temp +=  _T('\x2022');
                temp += _T("</bulletchar>");
                strXml += temp;
			}
			strXml += ToNode(bulletFontRef, _T("bulletfontref"));
			strXml += ToNode(bulletSize, _T("bulletsize"));
			strXml += ToNode(bulletColor, _T("bulletcolor"));

			strXml += ToNode(textAlignment, _T("textalignment"));
			strXml += ToNode(lineSpacing, _T("linespacing"));
			strXml += ToNode(spaceBefore, _T("spacebefore"));
			strXml += ToNode(spaceAfter, _T("spaceafter"));
			strXml += ToNode(leftMargin, _T("leftmargin"));
			strXml += ToNode(indent, _T("indent"));
			strXml += ToNode(defaultTabSize, _T("defaulttabsize"));

			strXml += ToNode(textDirection, _T("textdirectional"));
			strXml += ToNode(fontAlign, _T("fontalign"));
			
			if (wrapFlags.is_init())
			{
				int lIsWord = 1; // characters
				if (0x02 == (0x02 & wrapFlags.get()))
				{
					lIsWord = 0; // words
				}
				strXml += (_T("<wrapflags>") + XmlUtils::IntToString(lIsWord) + _T("</wrapflags>"));
			}
			
			size_t nCount = tabStops.size();
			if (0 < nCount)
			{
				strXml += _T("<tabstops>");
				for (size_t i = 0; i < nCount; ++i)
				{
					strXml += (_T("<tabstop>") + XmlUtils::UIntToString((size_t)tabStops[i]) + _T("</tabstop>"));
				}
				strXml += _T("</tabstops>");
			}

			oWriter.WriteString(strXml);

			oWriter.WriteNodeEnd(_T("Paragraph"));
			return oWriter.GetXmlString();
		}

		CString ToXmlEditor(CString& strClass, CString& strSpans, CGeomShapeInfo& oInfo)
		{
			CString strStyle = _T("margin-top:0px;margin-bottom:0px;margin-right:0px;");

			int nAlign = 0;
			if (textAlignment.is_init())
			{
				nAlign = (int)textAlignment.get();
			
				CString strAlign = _T("text-align:center;");
				if (0 == nAlign)
					strAlign = _T("text-align:left;");
				else if (2 == nAlign)
					strAlign = _T("text-align:right;");
				else if (3 == nAlign)
					strAlign = _T("text-align:justify;");

				strStyle += strAlign;
			}

			if (lineSpacing.is_init() && 0 != oInfo.m_lOriginalWidth)
			{
				int lnSpc = (int)lineSpacing.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("lnspc:%d;"), (int)((-1000 * oInfo.m_dWidth * lnSpc) / oInfo.m_lOriginalWidth));
					strStyle += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("lnspc:%d;"), -lnSpc);
					strStyle += strSpc;
				}
			}

			if (spaceAfter.is_init() && 0 != oInfo.m_lOriginalWidth)
			{
				int lnSpc = (int)spaceAfter.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcafter:%d;"), (int)((-1000 * oInfo.m_dWidth * lnSpc) / oInfo.m_lOriginalWidth));
					strStyle += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcafter:%d;"), -lnSpc);
					strStyle += strSpc;
				}
			}

			if (spaceBefore.is_init() && 0 != oInfo.m_lOriginalWidth)
			{
				int lnSpc = (int)spaceBefore.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcbefore:%d;"), (int)((-1000 * oInfo.m_dWidth * lnSpc) / oInfo.m_lOriginalWidth));
					strStyle += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcbefore:%d;"), -lnSpc);
					strStyle += strSpc;
				}
			}
			int nIndent = 0;
			if (indent.is_init() && 0 != oInfo.m_lOriginalWidth)
			{
				nIndent = (int)(oInfo.m_dWidth * indent.get() * 96.0 / (oInfo.m_lOriginalWidth * 25.4));
				// дебильный html не дает настроить буллет, поэтому:
				//nIndent = 0;

				CString strIndent = _T("");
				strIndent.Format(_T("text-indent:%dpx;"), (int)nIndent);
				strStyle += strIndent;
			}
			int nMargin = 0;
			if (leftMargin.is_init() && 0 != oInfo.m_lOriginalWidth)
			{
				nMargin = (int)(oInfo.m_dWidth * leftMargin.get() * 96.0 / (oInfo.m_lOriginalWidth * 25.4));

				CString strMargin = _T("");
				strMargin.Format(_T("margin-left:%dpx;"), (int)nMargin);
				strStyle += strMargin;
			}
			
			if (bulletColor.is_init())
			{
				if (-1 != bulletColor->m_lSchemeIndex)
				{
					CString strTMColor = _T("");
					strTMColor.Format(_T("color:"), bulletColor->m_lSchemeIndex + 1);
					strStyle += (_T("tm_") + strTMColor);
					
					CString strClassColor = _T("");
					strClassColor.Format(_T(" color%d"), bulletColor->m_lSchemeIndex + 1);
					strClass += strClassColor;
				}
				else
				{
					CString strColor = _T("");
					strColor.Format(_T("color: #%06x;"), (int)bulletColor->GetLONG_RGB());
					strStyle += strColor;
				}
			}

                        if (hasBullet.is_init() && (hasBullet.get() == true))
			{
				return _T("<ul style=\"padding:0px;margin:0px 0px 0px 20px;list-style-position:outside;\"><li class=\"" + strClass + "\" style=\"") + 
											strStyle + _T("\">") + strSpans + _T("</li></ul>\n");
			}
			return _T("<p class=\"") + strClass + _T("\" style=\"padding: 0px;") + strStyle + _T("\">") + strSpans + _T("</p>\n");
		}
	};

	class CTextRuler
	{
	public:
		NSCommon::nullable_base<SHORT>	DefaultTabSize;
		NSCommon::nullable_base<SHORT>	CLevels;
		NSCommon::nullable_base<SHORT>	TabStops;

		NSCommon::nullable_base<SHORT>	LeftMargin1;
		NSCommon::nullable_base<SHORT>	LeftMargin2;
		NSCommon::nullable_base<SHORT>	LeftMargin3;
		NSCommon::nullable_base<SHORT>	LeftMargin4;
		NSCommon::nullable_base<SHORT>	LeftMargin5;

		NSCommon::nullable_base<SHORT>	Indent1;
		NSCommon::nullable_base<SHORT>	Indent2;
		NSCommon::nullable_base<SHORT>	Indent3;
		NSCommon::nullable_base<SHORT>	Indent4;
		NSCommon::nullable_base<SHORT>	Indent5;

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
			if (!DefaultTabSize.is_init())
				DefaultTabSize = oSrc.DefaultTabSize;
			if (!CLevels.is_init())
				CLevels = oSrc.CLevels;
			
			if (!LeftMargin1.is_init())
				LeftMargin1 = oSrc.LeftMargin1;
			if (!LeftMargin2.is_init())
				LeftMargin2 = oSrc.LeftMargin2;
			if (!LeftMargin3.is_init())
				LeftMargin3 = oSrc.LeftMargin3;
			if (!LeftMargin4.is_init())
				LeftMargin4 = oSrc.LeftMargin4;
			if (!LeftMargin5.is_init())
				LeftMargin5 = oSrc.LeftMargin5;

			if (!Indent1.is_init())
				Indent1 = oSrc.Indent1;
			if (!Indent2.is_init())
				Indent2 = oSrc.Indent2;
			if (!Indent3.is_init())
				Indent3 = oSrc.Indent3;
			if (!Indent4.is_init())
				Indent4 = oSrc.Indent4;
			if (!Indent5.is_init())
				Indent5 = oSrc.Indent5;

			tabsStops.insert(tabsStops.end(), oSrc.tabsStops.begin(), oSrc.tabsStops.end());
		}

		CString ToString()
		{
			NSPresentationEditor::CXmlWriter oWriter;
			oWriter.WriteNodeBegin(_T("TextRuler"));
			
			if (CLevels.is_init())
			{
				oWriter.WriteNodeValueDWORD(_T("CLevels"), (DWORD)CLevels.get());
			}
			if (DefaultTabSize.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("DefaultTabSize"), (LONG)DefaultTabSize.get());
			}
			if (LeftMargin1.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("LeftMargin1"), (LONG)LeftMargin1.get());
			}
			if (Indent1.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("Indent1"), (LONG)Indent1.get());
			}
			if (LeftMargin2.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("LeftMargin2"), (LONG)LeftMargin2.get());
			}
			if (Indent2.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("Indent2"), (LONG)Indent2.get());
			}
			if (LeftMargin3.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("LeftMargin3"), (LONG)LeftMargin3.get());
			}
			if (Indent3.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("Indent3"), (LONG)Indent3.get());
			}
			if (LeftMargin4.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("LeftMargin4"), (LONG)LeftMargin4.get());
			}
			if (Indent4.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("Indent4"), (LONG)Indent4.get());
			}
			if (LeftMargin5.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("LeftMargin5"), (LONG)LeftMargin5.get());
			}
			if (Indent5.is_init())
			{
				oWriter.WriteNodeValueLONG(_T("Indent5"), (LONG)Indent5.get());
			}

			size_t tabssize = tabsStops.size();
			if (0 != tabssize)
			{
				oWriter.WriteNodeBegin(_T("TabStops"));
				for (size_t i = 0; i < tabssize; ++i)
				{
					oWriter.WriteNodeValueDWORD(_T("tabStop"), tabsStops[i]);
				}
				oWriter.WriteNodeEnd(_T("TabStops"));
			}

			oWriter.WriteNodeEnd(_T("TextRuler"));
			return oWriter.GetXmlString();
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
			NSPresentationEditor::CXmlWriter oWriter;
			oWriter.WriteNodeBegin(_T("SIRun"));

			if (bSpell)
			{
				oWriter.WriteNodeValueDWORD(_T("Spell"), (DWORD)Spell);
			}
			if (bLang)
			{
				oWriter.WriteNodeValueDWORD(_T("Lang"), (DWORD)Lang);
			}
			if (bAltLang)
			{
				oWriter.WriteNodeValueDWORD(_T("AltLang"), (DWORD)AltLang);
			}
			if (bBidi)
			{
				oWriter.WriteNodeValueDWORD(_T("Bidi"), (DWORD)Bidi);
			}
			if (bPp10ext)
			{
				oWriter.WriteNodeValueDWORD(_T("pp10runid"), (DWORD)pp10runid);
				oWriter.WriteNodeValueBool(_T("bGramma"), bGramma);
			}

			if (bSmartTag)
			{
				oWriter.WriteNodeBegin(_T("SmartTags"));
				for (size_t i = 0; i < arSmartTags.size(); ++i)
				{
					oWriter.WriteNodeValueDWORD(_T("tag"), arSmartTags[i]);
				}
				oWriter.WriteNodeEnd(_T("SmartTags"));
			}

			oWriter.WriteNodeEnd(_T("SIRun"));
			return oWriter.GetXmlString();
		}
	};


	class CSpan
	{
	public:
		CTextCFRun	m_oRun;
		CString		m_strText;

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

		CString ToXmlEditor(const CMetricInfo& oInfo, bool bIsLayout, LONG lTextOrPlaceholderType, LONG lID, LONG lNumLevel)
		{
			NSPresentationEditor::CStringWriter oWriter;
			
			// первый стиль - PF -------------------------------------------
			CString strStylePF = _T("");
			if (!bIsLayout)
				strStylePF.Format(_T(".s%d_lvl%u_pf { "), lTextOrPlaceholderType, lNumLevel);
			else
			{
				if (-1 == lID)
				{
					strStylePF.Format(_T(".t%d_lvl%u_pf { "), lTextOrPlaceholderType, lNumLevel);
				}
				else
				{
					strStylePF.Format(_T(".t%d_lvl%u_id%d_pf { "), lTextOrPlaceholderType, lNumLevel, lID);
				}
			}

			oWriter.WriteString(strStylePF);

			if (m_oPFRun.textAlignment.is_init())
			{
				int nAlign = (int)m_oPFRun.textAlignment.get();

				if (0 == nAlign)
					oWriter.WriteString(_T("text-align:left;"));
				else if (2 == nAlign)
					oWriter.WriteString(_T("text-align:right;"));
				else if (3 == nAlign)
					oWriter.WriteString(_T("text-align:justify;"));
				else
					oWriter.WriteString(_T("text-align:center;"));
			}

			int nMargin = 0;
			int nIndent = 0;
			if (m_oPFRun.leftMargin.is_init() && (0 != oInfo.m_lUnitsHor))
			{
				double dKoef = (double)m_oPFRun.leftMargin.get() / oInfo.m_lUnitsHor;
				nMargin = (int)(oInfo.m_lMillimetresHor * dKoef * 96.0 / 25.4);
			}
			if (m_oPFRun.indent.is_init() && (0 != oInfo.m_lUnitsHor))
			{
				double dKoef = (double)m_oPFRun.indent.get() / oInfo.m_lUnitsHor;
				// дебильный html не дает настроить буллет, поэтому:
				nIndent = (int)(oInfo.m_lMillimetresHor * dKoef * 96.0 / 25.4);
			}

			CString strPaddings = _T("");
			strPaddings.Format(_T("margin-left: %dpx;text-indent: %dpx;"), nMargin, nIndent);

			if (m_oPFRun.lineSpacing.is_init() && 0 != oInfo.m_lUnitsHor)
			{
				int lnSpc = (int)m_oPFRun.lineSpacing.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("lnspc:%d;"), (int)((-1000 * oInfo.m_lMillimetresHor * lnSpc) / oInfo.m_lUnitsHor));
					strPaddings += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("lnspc:%d;"), -lnSpc);
					strPaddings += strSpc;
				}
			}

			if (m_oPFRun.spaceAfter.is_init() && 0 != oInfo.m_lUnitsHor)
			{
				int lnSpc = (int)m_oPFRun.spaceAfter.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcafter:%d;"), (int)((-1000 * oInfo.m_lMillimetresHor * lnSpc) / oInfo.m_lUnitsHor));
					strPaddings += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcafter:%d;"), -lnSpc);
					strPaddings += strSpc;
				}
			}

			if (m_oPFRun.spaceBefore.is_init() && 0 != oInfo.m_lUnitsHor)
			{
				int lnSpc = (int)m_oPFRun.spaceBefore.get();
				if (lnSpc < 0)
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcbefore:%d;"), (int)((-1000 * oInfo.m_lMillimetresHor * lnSpc) / oInfo.m_lUnitsHor));
					strPaddings += strSpc;
				}
				else
				{
					CString strSpc = _T("");
					strSpc.Format(_T("spcbefore:%d;"), -lnSpc);
					strPaddings += strSpc;
				}
			}

			oWriter.WriteString(strPaddings);

			// bulletColor
			if (m_oPFRun.bulletColor.is_init())
			{
				CString strBulletColor = _T("");
				strBulletColor.Format(_T("color: #%06x;"), m_oPFRun.bulletColor->GetLONG_RGB());
				oWriter.WriteString(strBulletColor);
			}

			CString strEndPF = _T(" }\n");
			oWriter.WriteString(strEndPF);

			// -------------------------------------------------------------

			// второй стиль - CF -------------------------------------------
			CString strStyleCF = _T("");
			if (!bIsLayout)
				strStyleCF.Format(_T(".s%d_lvl%u_cf { "), lTextOrPlaceholderType, lNumLevel);
			else
			{
				if (-1 == lID)
				{
					strStyleCF.Format(_T(".t%d_lvl%u_cf { "), lTextOrPlaceholderType, lNumLevel);
				}
				else
				{
					strStyleCF.Format(_T(".t%d_lvl%u_id%d_cf { "), lTextOrPlaceholderType, lNumLevel, lID);
				}
			}

			oWriter.WriteString(strStyleCF);
			// -------------------------------------------------------------

			// run
			if (m_oCFRun.FontBold.is_init())
			{
				if (m_oCFRun.FontBold.get())
				{
					oWriter.WriteString(_T("font-weight: bold;"));
				}
				else
				{
					oWriter.WriteString(_T("font-weight: normal;"));
				}
			}
			if (m_oCFRun.FontItalic.is_init())
			{
				if (m_oCFRun.FontItalic.get())
				{
					oWriter.WriteString(_T("font-style: italic;"));
				}
				else
				{
					oWriter.WriteString(_T("font-style: normal;"));
				}
			}
			if (m_oCFRun.FontUnderline.is_init())
			{
				if (m_oCFRun.FontUnderline.get())
				{
					oWriter.WriteString(_T("text-decoration: underline;"));
				}
				else
				{
					oWriter.WriteString(_T("text-decoration: none;"));
				}
			}
			/*if (m_oCFRun.FontStrikeout.is_init())
			{
				if (m_oCFRun.FontStrikeout.get())
				{
					oWriter.WriteString(_T("text-decoration: line-through;"));
				}
				else
				{
					oWriter.WriteString(_T("text-decoration: normal;"));
				}
			}*/
			if (m_oCFRun.FontShadow.is_init())
			{
				if (m_oCFRun.FontShadow.get())
				{
					oWriter.WriteString(_T("text-shadow: black 1px 1px 2px;"));
				}
				else
				{
					oWriter.WriteString(_T("text-shadow: none;"));
				}
			}
			
			if (m_oCFRun.Size.is_init())
			{
				CString strFontSize = _T("");
				strFontSize.Format(_T("font-size: %dpt;"), (int)m_oCFRun.Size.get());
				oWriter.WriteString(strFontSize);
			}
			if (m_oCFRun.Color.is_init())
			{
				if (-1 != m_oCFRun.Color->m_lSchemeIndex)
				{
					CString strColorScheme = _T("");
					strColorScheme.Format(_T("tm_color:%d;"), m_oCFRun.Color->m_lSchemeIndex + 1);
					oWriter.WriteString(strColorScheme);
				}

				CString strColor = _T("");
				strColor.Format(_T("color: #%06x;"), m_oCFRun.Color->GetLONG_RGB());
				oWriter.WriteString(strColor);
			}

			if (m_oCFRun.Typeface.is_init())
			{
				CString strFontScheme = _T("");
				strFontScheme.Format(_T("tm_font:%d;"), (int)m_oCFRun.Typeface.get() + 1);
				oWriter.WriteString(strFontScheme);
			}
			
			if (m_oCFRun.FontProperties.is_init())
			{
				oWriter.WriteString(_T("font-family: "));
				oWriter.WriteString(m_oCFRun.FontProperties->strFontName);
				oWriter.WriteString(_T("; }\n"));
			}
			else
			{
				oWriter.WriteString(_T(" }\n"));
			}
			return oWriter.GetData();
		}
	};

	class CTextFullInfo
	{
	public:
		CTextPFRun m_oPF;
		CTextCFRun m_oCF;
		CTextRuler m_oRuler;
		CTextSIRun m_oSI;

	public:
		CTextFullInfo() : m_oPF(), m_oCF(), m_oRuler(), m_oSI()
		{
		}

		CTextFullInfo(const CTextFullInfo& oSrc)
		{
			*this = oSrc;
		}

		CTextFullInfo& operator=(const CTextFullInfo& oSrc)
		{
			m_oPF		= oSrc.m_oPF;
			m_oCF		= oSrc.m_oCF;
			m_oRuler	= oSrc.m_oRuler;
			m_oSI		= oSrc.m_oSI;
			return *this;
		}
	};
	
	class CTextStyles
	{
	public:
		NSCommon::nullable_base<CTextStyleLevel> m_pLevels[10];

	public:
		CTextStyles()
		{
			for (int i = 0; i < 10; ++i)
				m_pLevels[i] = NULL;
		}
		CTextStyles(const CTextStyles& oSrc)
		{
			*this = oSrc;
		}
		CTextStyles& operator=(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
				m_pLevels[i] = oSrc.m_pLevels[i];

			return *this;
		}
		void SetStyles(CTextStyles* pStyles)
		{
			for (int i = 0; i < 10; ++i)
				m_pLevels[i] = pStyles->m_pLevels[i];
		}
		void ApplyAfter(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (!m_pLevels[i].is_init())
					m_pLevels[i] = oSrc.m_pLevels[i];
				else if (oSrc.m_pLevels[i].is_init())
					m_pLevels[i]->ApplyAfter(oSrc.m_pLevels[i].get());

			}
		}
		void ApplyBefore(const CTextStyles& oSrc)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (!m_pLevels[i].is_init())
					m_pLevels[i] = oSrc.m_pLevels[i];
				else if (oSrc.m_pLevels[i].is_init())
					m_pLevels[i]->ApplyBefore(oSrc.m_pLevels[i].get());

			}
		}

		CString ToXmlEditor(const CMetricInfo& oInfo, bool bIsLayout, LONG lTextOrPlaceholderType, LONG lID)
		{
			NSPresentationEditor::CStringWriter oWriter;
			
			CString strTypeStyle = _T("");
			strTypeStyle.Format(_T("<Style type=\"%u\">"), lTextOrPlaceholderType);
			oWriter.WriteString(strTypeStyle);

			for (size_t i = 0; i < 10; ++i)
			{
				// теперь разбиваем на два стиля.
				// все это потому, что не нужно никаких отступов в спанах.
				// да и цвета нужно разделять. (буллет или не буллет)

				if (m_pLevels[i].is_init())
					oWriter.WriteString(m_pLevels[i]->ToXmlEditor(oInfo, bIsLayout, lTextOrPlaceholderType, lID, (LONG)i));
			}

			oWriter.WriteString(_T("</Style>"));
			return oWriter.GetData();
		}
	};

	class CParagraph
	{
	public:
		LONG m_lTextType;
		LONG m_lTextLevel;

		CTextPFRun m_oPFRun;
		std::vector<CSpan> m_arSpans;

	public:
		CParagraph() : m_oPFRun(), m_arSpans()
		{
			m_lTextType		= 0;
			m_lTextLevel	= 0;
		}
		CParagraph(const CParagraph& oSrc)
		{
			*this = oSrc;
		}
		CParagraph& operator=(const CParagraph& oSrc)
		{
			m_lTextLevel	= oSrc.m_lTextLevel;
			m_lTextType		= oSrc.m_lTextType;

			m_oPFRun		= oSrc.m_oPFRun;

			m_arSpans = oSrc.m_arSpans;

			return *this;
		}
		~CParagraph()
		{
			m_arSpans.clear();
		}
		AVSINLINE void CheckErrors()
		{
			if (IsEmpty())
			{
                m_oPFRun.hasBullet = false;
			}
			size_t nCountS = m_arSpans.size();
			for (size_t i = 0; i < nCountS; ++i)
			{
				m_arSpans[i].m_strText.Replace((TCHAR)(11), (TCHAR)('\n'));
			}
		}
		AVSINLINE bool IsEmpty()
		{
			size_t nCountSpans = m_arSpans.size();
			for (size_t i = 0; i < nCountSpans; ++i)
			{
				int nLen = m_arSpans[i].m_strText.GetLength();

				if (nLen > 1)
					return false;

				if ((nLen == 1) && ((_T(" ") != m_arSpans[i].m_strText) && (_T("\n") != m_arSpans[i].m_strText)))
					return false;
			}
			return true;
		}
	};
}

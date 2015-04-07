#pragma once

#include "CSS.h"

class CStylesWriter
{
public:
	AVSINLINE static CString GetTextAnchor(const WORD& value)
	{
		if (0 == value)
			return _T("t");
		if (1 == value)
			return _T("ctr");
		if (2 == value)
			return _T("b");
		return _T("t");
	}

	AVSINLINE static CString GetTextAlign(const WORD& value)
	{
		if (0 == value)
			return _T("l");
		if (1 == value)
			return _T("ctr");
		if (2 == value)
			return _T("r");
		if (3 == value)
			return _T("just");
		if (4 == value)
			return _T("dist");
		if (5 == value)
			return _T("thaiDist");
		if (6 == value)
			return _T("justLow");
		return _T("l");
	}

	AVSINLINE static CString GetColorInScheme(const LONG& lIndex)
	{
		switch (lIndex)
		{
		case 0:	 return _T("phClr");
		case 1:	 return _T("bg1");
		case 2:	 return _T("tx1");
		case 3:	 return _T("bg2");
		case 4:	 return _T("tx2");
		case 5:	 return _T("accent1");
		case 6:	 return _T("accent2");
		case 7:	 return _T("accent3");
		case 8:	 return _T("accent4");
		case 9:	 return _T("accent5");
		case 10: return _T("accent6");
		case 11: return _T("hlink");
		case 12: return _T("folHlink");
		case 13: return _T("lt1");
		case 14: return _T("dk1");
		case 15: return _T("lt2");
		case 16: return _T("dk2");
		break;
		};
		return _T("none");
	}

	AVSINLINE static CString GetFontAlign(const WORD& value)
	{
		if (0 == value)
			return _T("base");
		if (1 == value)
			return _T("t");
		if (2 == value)
			return _T("ctr");
		if (3 == value)
			return _T("b");
		return _T("auto");
	}

	static CString ConvertStyles(NSPresentationEditor::CTextStyles& oStyles, NSPresentationEditor::CMetricInfo& oMetricInfo, int nCount = 10)
	{
		NSPresentationEditor::CStringWriter oWriter;
		ConvertStyles(oStyles, oMetricInfo, oWriter, nCount);
		return oWriter.GetData();
	}
	static void ConvertStyles(NSPresentationEditor::CTextStyles& oStyles, NSPresentationEditor::CMetricInfo& oMetricInfo, 
		NSPresentationEditor::CStringWriter& oWriter, int nCount = 10)
	{
		for (int i = 0; i < nCount; ++i)
		{
			if (oStyles.m_pLevels[i].is_init())
				ConvertStyleLevel(oStyles.m_pLevels[i].get(), oMetricInfo, oWriter, i);
		}
	}
	static CString ConvertStyleLevel(NSPresentationEditor::CTextStyleLevel& oLevel, NSPresentationEditor::CMetricInfo& oMetricInfo, const int& nLevel)
	{
		NSPresentationEditor::CStringWriter oWriter;
		ConvertStyleLevel(oLevel, oMetricInfo, oWriter, nLevel);
		return oWriter.GetData();
	}

	static void ConvertStyleLevel(NSPresentationEditor::CTextStyleLevel& oLevel, NSPresentationEditor::CMetricInfo& oMetricInfo,
		NSPresentationEditor::CStringWriter& oWriter, const int& nLevel)
	{
		CString str1 = _T("");
		if (nLevel == 9)
			str1 = _T("<a:defPPr");
		else
			str1.Format(_T("<a:lvl%dpPr"), nLevel + 1);

		oWriter.WriteString(str1);

		NSPresentationEditor::CTextPFRun* pPF = &oLevel.m_oPFRun;

		if (pPF->fontAlign.is_init())
		{
			CString strProp = GetFontAlign(pPF->fontAlign.get());
			oWriter.WriteString(_T(" fontAlgn=\"") + strProp + _T("\""));
		}
		if (pPF->leftMargin.is_init())
		{
			CString strProp = _T("");
			strProp.Format(_T(" marL=\"%d\""), pPF->leftMargin.get());
			oWriter.WriteString(strProp);
		}
		if (pPF->indent.is_init())
		{
			CString strProp = _T("");
			strProp.Format(_T(" indent=\"%d\""), pPF->indent.get());
			oWriter.WriteString(strProp);
		}
		if (pPF->textAlignment.is_init())
		{
			CString strProp = GetTextAlign(pPF->textAlignment.get());
			oWriter.WriteString(_T(" algn=\"") + strProp + _T("\""));
		}
		if (pPF->defaultTabSize.is_init())
		{
			CString strProp = _T("");
			strProp.Format(_T(" defTabSz=\"%d\""), pPF->defaultTabSize.get());
			oWriter.WriteString(strProp);
		}
		CString str2 = _T(">");
		oWriter.WriteString(str2);

		if (pPF->hasBullet.is_init())
		{
			if (pPF->hasBullet.get())
			{
                CString strB = _T("<a:buChar char=\"AAAAA\"/>"); //todooo кружочек
				oWriter.WriteString(strB);
			}
			else
			{
				CString strB = _T("<a:buNone/>");
				oWriter.WriteString(strB);
			}
		}

		double dKoef1 = 3.52777778;
		if (pPF->lineSpacing.is_init())
		{
			LONG val = pPF->lineSpacing.get();
			if (val > 0)
			{
				CString str = _T("");
				str.Format(_T("<a:lnSpc><a:spcPts val=\"%d\"/></a:lnSpc>"), (int)(val / dKoef1));
				oWriter.WriteString(str);
			}
			else
			{
				CString str = _T("");
				str.Format(_T("<a:lnSpc><a:spcPct val=\"%d\"/></a:lnSpc>"), -val * 1000);
				oWriter.WriteString(str);
			}
		}
		if (pPF->spaceAfter.is_init())
		{
			LONG val = pPF->spaceAfter.get();
			if (val > 0)
			{
				CString str = _T("");
				str.Format(_T("<a:spcAft><a:spcPts val=\"%d\"/></a:spcAft>"), (int)(val / dKoef1));
				oWriter.WriteString(str);
			}
			else
			{
				CString str = _T("");
				str.Format(_T("<a:spcAft><a:spcPct val=\"%d\"/></a:spcAft>"), -val * 1000);
				oWriter.WriteString(str);
			}
		}
		if (pPF->spaceBefore.is_init())
		{
			LONG val = pPF->spaceBefore.get();
			if (val > 0)
			{
				CString str = _T("");
				str.Format(_T("<a:spcBef><a:spcPts val=\"%d\"/></a:spcBef>"), (int)(val / dKoef1));
				oWriter.WriteString(str);
			}
			else
			{
				CString str = _T("");
				str.Format(_T("<a:spcBef><a:spcPct val=\"%d\"/></a:spcBef>"), -val * 1000);
				oWriter.WriteString(str);
			}
		}

		CString strCF1 = _T("<a:defRPr");
		oWriter.WriteString(strCF1);

		NSPresentationEditor::CTextCFRun* pCF = &oLevel.m_oCFRun;
		
		if (pCF->Size.is_init())
		{
			CString strProp = _T("");
			strProp.Format(_T(" sz=\"%d\""), (int)(100 * pCF->Size.get()));
			oWriter.WriteString(strProp);
		}
		if (pCF->FontBold.is_init())
		{
			if (pCF->FontBold.get())
				oWriter.WriteString(_T(" b=\"1\""));
			else
				oWriter.WriteString(_T(" b=\"0\""));
		}
		if (pCF->FontItalic.is_init())
		{
			if (pCF->FontItalic.get())
				oWriter.WriteString(_T(" i=\"1\""));
			else
				oWriter.WriteString(_T(" i=\"0\""));
		}
		oWriter.WriteString(_T(">"));

		if (pCF->Color.is_init())
		{
			if (pCF->Color->m_lSchemeIndex != -1)
			{
				CString strProp = _T("<a:solidFill><a:schemeClr val=\"") + GetColorInScheme(pCF->Color->m_lSchemeIndex) + _T("\"/></a:solidFill>");
				oWriter.WriteString(strProp);
			}
			else
			{
				CString strColor = _T("");
				strColor.Format(_T("%06x"), pCF->Color->GetLONG_RGB());

				CString strProp = _T("<a:solidFill><a:srgbClr val=\"") + strColor + _T("\"/></a:solidFill>");
				oWriter.WriteString(strProp);
			}
		}

		if (pCF->Typeface.is_init())
		{
			if (0 == pCF->Typeface.get())
			{
				CString strProp = _T("<a:latin typeface=\"+mj-lt\"/>");
				oWriter.WriteString(strProp);
			}
			else
			{
				CString strProp = _T("<a:latin typeface=\"+mn-lt\"/>");
				oWriter.WriteString(strProp);
			}
		}
		else if (pCF->FontProperties.is_init())
		{
			oWriter.WriteString(_T("<a:latin typeface=\"") + pCF->FontProperties->strFontName + _T("\"/>"));
		}

		CString strCF2 = _T("</a:defRPr>");
		oWriter.WriteString(strCF2);		

		CString str3 = _T("");
		if (nLevel == 9)
			str3 = _T("</a:defPPr>");
		else
			str3.Format(_T("</a:lvl%dpPr>"), nLevel + 1);

		oWriter.WriteString(str3);
	}
};

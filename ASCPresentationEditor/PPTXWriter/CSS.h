#pragma once
#include "../../Common/ASCUtils.h"
#include "../OfficeDrawing/TextAttributesEx.h"

#include <map>

class CCSS
{
public:
	std::map<CString, CString>	m_mapSettings;
	CString						m_strClassName;

public:
	CCSS()
	{
		Clear();
	}
	~CCSS()
	{
	}
	AVSINLINE void Clear()
	{
		m_strClassName = _T("");
		m_mapSettings.clear();
	}

public:
	void LoadFromString(CString& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	= 0;
		TCHAR* pDataMem = pData;

		while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('\n') == *pDataMem) || (TCHAR('.') == *pDataMem)))
		{
			++nPosition;
			++pDataMem;
		}

		int nPosNameStart = nPosition;
		while ((nPosition < nCount) && (TCHAR(' ') != *pDataMem) && (TCHAR('{') != *pDataMem))
		{
			++nPosition;
			++pDataMem;
		}
		m_strClassName = strParams.Mid(nPosNameStart, nPosition - nPosNameStart);

		while (true)
		{
			++nPosition;
			++pDataMem;
			
			// пропускаем пробелы
			while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('{') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
			while ((nPosition < nCount) && (TCHAR(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			CString strName = strParams.Mid(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(';') != *pDataMem) && (TCHAR('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			CString strValue = strParams.Mid(nPosOld, nPosition - nPosOld);

			m_mapSettings.insert(std::pair<CString, CString>(strName, strValue));
		}
	}

	void LoadFromString2(CString& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	= 0;
		TCHAR* pDataMem = pData;

		m_strClassName = _T("");

		while (true)
		{
			// пропускаем пробелы
			while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('{') == *pDataMem) || (TCHAR(';') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
			while ((nPosition < nCount) && (TCHAR(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			CString strName = strParams.Mid(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(';') != *pDataMem) && (TCHAR('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			CString strValue = strParams.Mid(nPosOld, nPosition - nPosOld);

			m_mapSettings.insert(std::pair<CString, CString>(strName, strValue));
		}
	}

public:

	AVSINLINE void LoadColor(NSPresentationEditor::CColor& oColor)
	{
		std::map<CString, CString>::iterator pPair = m_mapSettings.find(_T("color"));
		if (m_mapSettings.end() != pPair)
		{
			int nLen = pPair->second.GetLength();
			if (0 == nLen)
				return;
			if (pPair->second.GetAt(0) == TCHAR('#'))
				oColor.FromString(pPair->second);
			else if ((3 < nLen) && (pPair->second.Mid(0, 3) == _T("rgb")))
			{
				TCHAR* pBuffer		= pPair->second.GetBuffer();
				TCHAR* pBuffer1		= pBuffer;
				TCHAR* pBuffer2		= pBuffer;
				TCHAR* pBufferEnd	= pBuffer + nLen;
				
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				oColor.R = GetColor(pBuffer1, pBuffer2);

				pBuffer1 = pBuffer2;
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				oColor.G = GetColor(pBuffer1, pBuffer2);

				pBuffer1 = pBuffer2;
				while ((pBuffer1 < pBufferEnd) && !XmlUtils::IsDigit(*pBuffer1))
					++pBuffer1;
				pBuffer2 = pBuffer1;
				while ((pBuffer2 < pBufferEnd) && XmlUtils::IsDigit(*pBuffer2))
					++pBuffer2;
				oColor.B = GetColor(pBuffer1, pBuffer2);

				oColor.A = 0xFF;
				oColor.m_lSchemeIndex = -1;
			}
		}
	}

	AVSINLINE void LoadFont(NSPresentationEditor::CFont& oFont)
	{
		std::map<CString, CString>::iterator pPair = m_mapSettings.find(_T("font-family"));
		if (m_mapSettings.end() != pPair)
		{
			oFont.Name = pPair->second;
		}
		pPair = m_mapSettings.find(_T("tmdocs_charset"));
		if (m_mapSettings.end() != pPair)
		{
			oFont.Charset = (BYTE)XmlUtils::GetInteger(pPair->second);
		}
		pPair = m_mapSettings.find(_T("tmdocs_monospace"));
		if (m_mapSettings.end() != pPair)
		{
			oFont.Monospace = (BOOL)(pPair->second == _T("1"));
		}
		pPair = m_mapSettings.find(_T("tmdocs_panose"));
		if (m_mapSettings.end() != pPair)
		{
			oFont.Panose = pPair->second;
		}
	}

	AVSINLINE void LoadTextStyle(NSPresentationEditor::CTextStyleLevel& oLevel)
	{
		std::map<CString, CString>::iterator pPair;

		pPair = m_mapSettings.find(_T("text-align"));
		if (m_mapSettings.end() != pPair)
		{
			WORD nAlign = 1;
			if (_T("left") == pPair->second)
				nAlign = 0;
			else if (_T("right") == pPair->second)
				nAlign = 2;
			else if (_T("justify") == pPair->second)
				nAlign = 3;

			oLevel.m_oPFRun.textAlignment = nAlign;
		}

		pPair = m_mapSettings.find(_T("lnspc"));
		if (m_mapSettings.end() != pPair)
		{
			oLevel.m_oPFRun.lineSpacing = (LONG)XmlUtils::GetInteger(pPair->second);			
		}
		pPair = m_mapSettings.find(_T("spcafter"));
		if (m_mapSettings.end() != pPair)
		{
			oLevel.m_oPFRun.spaceAfter = (LONG)XmlUtils::GetInteger(pPair->second);			
		}
		pPair = m_mapSettings.find(_T("spcbefore"));
		if (m_mapSettings.end() != pPair)
		{
			oLevel.m_oPFRun.spaceBefore = (LONG)XmlUtils::GetInteger(pPair->second);			
		}

		WORD lStyle = 0;
		pPair = m_mapSettings.find(_T("font-weight"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("bold") == pPair->second)
                oLevel.m_oCFRun.FontBold = true;
			else
                oLevel.m_oCFRun.FontBold = false;
		}

		pPair = m_mapSettings.find(_T("font-style"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("italic") == pPair->second)
                oLevel.m_oCFRun.FontItalic = true;
			else
                oLevel.m_oCFRun.FontItalic = false;
		}

		pPair = m_mapSettings.find(_T("text-decoration"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("underline") == pPair->second)
                oLevel.m_oCFRun.FontUnderline = true;
			else
                oLevel.m_oCFRun.FontUnderline = false;
		}

		pPair = m_mapSettings.find(_T("font-size"));
		if (m_mapSettings.end() != pPair)
		{
			CString strTemp = pPair->second.Mid(0, pPair->second.GetLength() - 2);

			oLevel.m_oCFRun.Size = (WORD)XmlUtils::GetInteger(strTemp);
		}

		pPair = m_mapSettings.find(_T("color"));
		if (m_mapSettings.end() != pPair)
		{
			oLevel.m_oCFRun.Color = new NSPresentationEditor::CColor();
			oLevel.m_oCFRun.Color->FromString(pPair->second);
		}

		pPair = m_mapSettings.find(_T("font-family"));
		if (m_mapSettings.end() != pPair)
		{
			oLevel.m_oCFRun.FontProperties = new NSPresentationEditor::CFontProperties();
			oLevel.m_oCFRun.FontProperties->strFontName = pPair->second;
		}
	}

	AVSINLINE void LoadStylePF(NSPresentationEditor::CTextPFRun& oRun, const NSPresentationEditor::CMetricInfo& oInfo)
	{
		std::map<CString, CString>::iterator pPair  = m_mapSettings.find(_T("text-align"));
		
		if (m_mapSettings.end() != pPair)
		{
			WORD nAlign = 1;
			if (_T("left") == pPair->second)
				nAlign = 0;
			else if (_T("right") == pPair->second)
				nAlign = 2;
			else if (_T("justify") == pPair->second)
				nAlign = 3;

			oRun.textAlignment = nAlign;
		}

		double dKoef = (double)oInfo.m_lUnitsHor * 25.4 / (oInfo.m_lMillimetresHor * 96);

		pPair = m_mapSettings.find(_T("text-indent"));
		if (m_mapSettings.end() != pPair)
		{
			CString strVal = pPair->second;
			CorrectStringNum(strVal);

			oRun.indent = (LONG)(XmlUtils::GetInteger(strVal) * dKoef);
		}
		pPair = m_mapSettings.find(_T("margin-left"));
		if (m_mapSettings.end() != pPair)
		{
			CString strVal = pPair->second;
			CorrectStringNum(strVal);

			oRun.leftMargin = (LONG)(XmlUtils::GetInteger(strVal) * dKoef);
		}
		pPair = m_mapSettings.find(_T("color"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.bulletColor = new NSPresentationEditor::CColor();
			oRun.bulletColor->FromString(pPair->second);

			pPair = m_mapSettings.find(_T("tm_color"));
			if (m_mapSettings.end() != pPair)
			{
				oRun.bulletColor->m_lSchemeIndex = (LONG)XmlUtils::GetInteger(pPair->second) - 1;
			}
		}

		pPair = m_mapSettings.find(_T("lnspc"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.lineSpacing = (LONG)XmlUtils::GetInteger(pPair->second);			
		}
		pPair = m_mapSettings.find(_T("spcafter"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.spaceAfter = (LONG)XmlUtils::GetInteger(pPair->second);			
		}
		pPair = m_mapSettings.find(_T("spcbefore"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.spaceBefore = (LONG)XmlUtils::GetInteger(pPair->second);			
		}
	}
	AVSINLINE void LoadStyleCF(NSPresentationEditor::CTextCFRun& oRun, const NSPresentationEditor::CMetricInfo& oInfo)
	{
		std::map<CString, CString>::iterator pPair;

		pPair = m_mapSettings.find(_T("font-weight"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("bold") == pPair->second)
				oRun.FontBold = (BOOL)TRUE;
			else
				oRun.FontBold = (BOOL)FALSE;
		}

		pPair = m_mapSettings.find(_T("font-style"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("italic") == pPair->second)
				oRun.FontItalic = (BOOL)TRUE;
			else
				oRun.FontItalic = (BOOL)FALSE;
		}

		pPair = m_mapSettings.find(_T("text-transform"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("uppercase") == pPair->second)
				oRun.Cap = (WORD)1;
			else if (_T("lowercase") == pPair->second)
				oRun.Cap = (WORD)2;
			else
				oRun.Cap = (WORD)0;
		}
		pPair = m_mapSettings.find(_T("text-decoration"));
		if (m_mapSettings.end() != pPair)
		{
			if (_T("underline") == pPair->second)
				oRun.FontUnderline = (BOOL)TRUE;
			else 
				oRun.FontUnderline = (BOOL)FALSE;
		}

		pPair = m_mapSettings.find(_T("font-size"));
		if (m_mapSettings.end() != pPair)
		{
			CString strVal = pPair->second;
			CorrectStringNum(strVal);

			oRun.Size = (WORD)(XmlUtils::GetInteger(strVal));
		}

		pPair = m_mapSettings.find(_T("color"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.Color = new NSPresentationEditor::CColor();
			oRun.Color->FromString(pPair->second);
		}
		else
		{
			pPair = m_mapSettings.find(_T("tm_color"));
			if (m_mapSettings.end() != pPair)
			{
				oRun.Color = new NSPresentationEditor::CColor();
				oRun.Color->m_lSchemeIndex = (LONG)XmlUtils::GetInteger(pPair->second) - 1;
			}
		}

		pPair = m_mapSettings.find(_T("font-family"));
		if (m_mapSettings.end() != pPair)
		{
			oRun.FontProperties = new NSPresentationEditor::CFontProperties();
			oRun.FontProperties->strFontName = pPair->second;
		}
		
	}

private:
	BYTE GetColor(TCHAR* pChar1, TCHAR* pChar2)
	{
		if (pChar1 == pChar2)
			return 0;
		CString s(pChar1, (int)(pChar2 - pChar1));
		return (BYTE)XmlUtils::GetInteger(s);
	}
public:

	void CorrectStringNum(CString& strValue)
	{
		int nStart	= 0;
		int nEnd	= strValue.GetLength() - 1;

		while (nStart <= nEnd)
		{
			if (((TCHAR)('-') == strValue[nStart]) || ((TCHAR('0') <= strValue[nStart]) && (TCHAR('9') >= strValue[nStart])))
				break;
			++nStart;
		}
		while (nStart <= nEnd)
		{
			if ((TCHAR('0') <= strValue[nEnd]) && (TCHAR('9') >= strValue[nEnd]))
				break;
			--nEnd;
		}

		if (nStart > nEnd)
		{
			strValue = _T("0");
			return;
		}
		strValue = strValue.Mid(nStart, nEnd - nStart + 1);		
	}

};

class CStylesCSS
{
public: 
	std::vector<CCSS> m_arStyles;

public:
	CStylesCSS() : m_arStyles()
	{
	}
	~CStylesCSS()
	{
	}
	AVSINLINE void Clear()
	{
		m_arStyles.clear();
	}

public:
	void LoadStyles(CString& strParams)
	{
		Clear();

		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	 = 0;
		int nPositionOld = 0;
		TCHAR* pDataMem = pData;

		while (nPosition < nCount)
		{
			if (*pDataMem == TCHAR('}'))
			{
				CString strTemp = strParams.Mid(nPositionOld, nPosition - nPositionOld + 1);
				
				CCSS elem;
				m_arStyles.push_back(elem);
				m_arStyles.back().LoadFromString(strTemp);

				nPositionOld = nPosition + 1;
			}

			++nPosition;
			++pDataMem;
		}
	}
};

using namespace NSPresentationEditor;
class CTextLoader
{
public:
	static void ConvertText(XmlUtils::CXmlNode& oNode, CTextAttributesEx& oText, CMetricInfo oMetric)
	{
		oText.m_arParagraphs.clear();

		XmlUtils::CXmlNodes oParagraphs;
		if (oNode.GetNodes(_T("*"), oParagraphs))
		{
			int nCountPFs = oParagraphs.GetCount();

			int nNumberPF = 0;
			for (int i = 0; i < nCountPFs; ++i)
			{
				XmlUtils::CXmlNode oNodePar;
				oParagraphs.GetAt(i, oNodePar);

				CParagraph elem;
				oText.m_arParagraphs.push_back(elem);

				CParagraph* pParagraph = &oText.m_arParagraphs[nNumberPF];
				++nNumberPF;

				if (_T("p") == oNodePar.GetName())
				{
					pParagraph->m_oPFRun.hasBullet = FALSE;

					CString strClass = oNodePar.GetAttribute(_T("class"));

					int n1 = strClass.Find(TCHAR('_'));
					int n2 = -1;
					if (n1 != -1)
						n2 = strClass.Find(TCHAR('_'), n1 + 1);

					if (n2 != -1)
					{
						pParagraph->m_lTextType			= XmlUtils::GetInteger(strClass.Mid(1, n1 - 1));
						n1 += 4;
						pParagraph->m_lTextLevel		= XmlUtils::GetInteger(strClass.Mid(n1, n2 - n1));
						oText.m_lTextType	= pParagraph->m_lTextType;
					}
					
					
					CCSS oStylePar;
                    CString sAttribute = oNodePar.GetAttribute(_T("style"));
                    oStylePar.LoadFromString2(sAttribute);

					oStylePar.LoadStylePF(pParagraph->m_oPFRun, oMetric);

					XmlUtils::CXmlNodes oSpans;
					if (oNodePar.GetNodes(_T("*"), oSpans))
					{
						int nCountCFs = oSpans.GetCount();
						for (int nSpan = 0; nSpan < nCountCFs; ++nSpan)
						{
							XmlUtils::CXmlNode oNodeSpan;
							oSpans.GetAt(nSpan, oNodeSpan);

							if (_T("span") == oNodeSpan.GetName())
							{
								CSpan oSpan;
								oSpan.m_strText = oNodeSpan.GetTextExt();
								
								CCSS oStyleSpan;
                                CString sAttribute = oNodeSpan.GetAttribute(_T("style"));
                                oStyleSpan.LoadFromString2(sAttribute);
								oStyleSpan.LoadStyleCF(oSpan.m_oRun, oMetric);

								pParagraph->m_arSpans.push_back(oSpan);
							}
							else if (FALSE)
							{
								CSpan oSpan;
								oSpan.m_strText = _T("\n");

								pParagraph->m_arSpans.push_back(oSpan);
							}
						}
					}
				}
				else
				{
					pParagraph->m_oPFRun.hasBullet = TRUE;

					CString strClass = oNodePar.GetAttribute(_T("class"));

					int n1 = strClass.Find(TCHAR('_'));
					int n2 = -1;
					if (n1 != -1)
						n2 = strClass.Find(TCHAR('_'), n1 + 1);

					if (n2 != -1)
					{
						pParagraph->m_lTextType			= XmlUtils::GetInteger(strClass.Mid(1, n1 - 1));
						n1 += 4;
						pParagraph->m_lTextLevel		= XmlUtils::GetInteger(strClass.Mid(n1, n2 - n1));
						oText.m_lTextType	= pParagraph->m_lTextType;
					}
					
					XmlUtils::CXmlNode oNodeLi;
					if (oNodePar.GetNode(_T("li"), oNodeLi))
					{
						CCSS oStylePar;
                        CString sAttribute = oNodeLi.GetAttribute(_T("style"));
                        oStylePar.LoadFromString2(sAttribute);
						oStylePar.LoadStylePF(pParagraph->m_oPFRun, oMetric);

						XmlUtils::CXmlNodes oSpans;
						if (oNodeLi.GetNodes(_T("*"), oSpans))
						{
							int nCountCFs = oSpans.GetCount();
							for (int nSpan = 0; nSpan < nCountCFs; ++nSpan)
							{
								XmlUtils::CXmlNode oNodeSpan;
								oSpans.GetAt(nSpan, oNodeSpan);

								if (_T("span") == oNodeSpan.GetName())
								{
									CSpan oSpan;
									oSpan.m_strText = oNodeSpan.GetTextExt();
									
									CCSS oStyleSpan;
                                    CString sAttribute = oNodeSpan.GetAttribute(_T("style"));
                                    oStyleSpan.LoadFromString2(sAttribute);
									oStyleSpan.LoadStyleCF(oSpan.m_oRun, oMetric);

									pParagraph->m_arSpans.push_back(oSpan);
								}
								else if (FALSE)
								{
									CSpan oSpan;
									oSpan.m_strText = _T("\n");

									pParagraph->m_arSpans.push_back(oSpan);
								}
							}
						}
					}
				}

				size_t nCountS = pParagraph->m_arSpans.size();
				if (0 < nCountS)
				{
					if (_T("\n") == pParagraph->m_arSpans[nCountS - 1].m_strText)
					{
						pParagraph->m_arSpans[nCountS - 1].m_strText = _T(" ");
						if (1 == nCountS)
							pParagraph->m_oPFRun.hasBullet = false;
					}
				}
			}
		}
	}

	static void ConvertText2(XmlUtils::CXmlNode& oNode, CTextAttributesEx& oText, CMetricInfo oMetric)
	{
		oText.m_arParagraphs.clear();

		XmlUtils::CXmlNodes oParagraphs;
		if (oNode.GetNodes(_T("*"), oParagraphs))
		{
			int nCountPFs = oParagraphs.GetCount();

			int nNumberPF = 0;
			for (int i = 0; i < nCountPFs; ++i)
			{
				XmlUtils::CXmlNode oNodePar;
				oParagraphs.GetAt(i, oNodePar);

				CParagraph elem;
				oText.m_arParagraphs.push_back(elem);

				CParagraph* pParagraph = &oText.m_arParagraphs[nNumberPF];
				++nNumberPF;

				if (_T("p") == oNodePar.GetName())
				{
					pParagraph->m_oPFRun.hasBullet = (BOOL)FALSE;

					CString strClass = oNodePar.GetAttribute(_T("class"));

					int n1 = strClass.Find(TCHAR('_'));
					int n2 = -1;
					if (n1 != -1)
						n2 = strClass.Find(TCHAR('_'), n1 + 1);

					if (n2 != -1)
					{
						pParagraph->m_lTextType			= XmlUtils::GetInteger(strClass.Mid(1, n1 - 1));
						n1 += 4;
						pParagraph->m_lTextLevel		= XmlUtils::GetInteger(strClass.Mid(n1, n2 - n1));
						oText.m_lTextType	= pParagraph->m_lTextType;
					}

					CTextLoader::ConvertParagraph(oNodePar, pParagraph, oMetric);
				}
				else
				{
					pParagraph->m_oPFRun.hasBullet = (BOOL)TRUE;

					CString strClass = oNodePar.GetAttribute(_T("class"));

					int n1 = strClass.Find(TCHAR('_'));
					int n2 = -1;
					if (n1 != -1)
						n2 = strClass.Find(TCHAR('_'), n1 + 1);

					if (n2 != -1)
					{
						pParagraph->m_lTextType			= XmlUtils::GetInteger(strClass.Mid(1, n1 - 1));
						n1 += 4;
						pParagraph->m_lTextLevel		= XmlUtils::GetInteger(strClass.Mid(n1, n2 - n1));
						oText.m_lTextType	= pParagraph->m_lTextType;
					}
					LONG lParLevel = pParagraph->m_lTextLevel;
					
					XmlUtils::CXmlNodes oNodeLis;
					if (oNodePar.GetNodes(_T("li"), oNodeLis))
					{
						int nCountLi = oNodeLis.GetCount();
						for (int j = 0; j < nCountLi; ++j)
						{
							XmlUtils::CXmlNode oNodeLi;
							oNodeLis.GetAt(j, oNodeLi);

							if (0 == j)
							{
								CTextLoader::ConvertParagraph(oNodeLi, pParagraph, oMetric);
							}
							else
							{
								CParagraph elem;
								oText.m_arParagraphs.push_back(elem);

								pParagraph = &oText.m_arParagraphs[nNumberPF];
								++nNumberPF;

								pParagraph->m_oPFRun.hasBullet = (BOOL)TRUE;
								pParagraph->m_lTextType = oText.m_lTextType;
								pParagraph->m_lTextLevel = max(0, lParLevel);

								CTextLoader::ConvertParagraph(oNodeLi, pParagraph, oMetric);
							}
						}
					}
				}
			}
		}
	}

	static void ConvertParagraph(XmlUtils::CXmlNode& oNode, CParagraph* pParagraph, CMetricInfo oMetric)
	{
		CCSS oStylePar;
        CString sAttribute = oNode.GetAttribute(_T("style"));
        oStylePar.LoadFromString2(sAttribute);
		oStylePar.LoadStylePF(pParagraph->m_oPFRun, oMetric);

		XmlUtils::CXmlNodes oNodes;
		if (oNode.GetChilds(oNodes))
		{
			int nCount = oNodes.GetCount();

			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oNodeS;
				oNodes.GetAt(i, oNodeS);

				CString sName = oNodeS.GetName();

				if (_T("#text") == sName)
				{
					CSpan oSpan;
					oSpan.m_strText = oNodeS.GetXml();
					pParagraph->m_arSpans.push_back(oSpan);
				}
				else if (_T("br") == sName)
				{
					CSpan oSpan;
					CTextCFRun oRunBR;
					CCSS _oStyleCF;
                    CString sAttribute = oNodeS.GetAttribute(_T("style"));
                    _oStyleCF.LoadFromString2(sAttribute);
					_oStyleCF.LoadStyleCF(oRunBR, oMetric);

					oSpan.m_oRun = oRunBR;
					oSpan.m_strText = _T("\n");
					pParagraph->m_arSpans.push_back(oSpan);
				}
				else if (_T("span") == sName)
				{
					ConvertSpan(pParagraph, oMetric, oNodeS, NULL);
				}
			}
		}
	}

	static void ConvertSpan(CParagraph* pParagraph, CMetricInfo oMetric, XmlUtils::CXmlNode& oNode, CTextCFRun* pCFRun)
	{
		CTextCFRun oRun;

		CCSS oStyleCF;
        CString sAttribute = oNode.GetAttribute(_T("style"));
        oStyleCF.LoadFromString2(sAttribute);
		oStyleCF.LoadStyleCF(oRun, oMetric);

		if (NULL != pCFRun)
			oRun.ApplyBefore(*pCFRun);

		XmlUtils::CXmlNodes oNodes;
		if (oNode.GetChilds(oNodes))
		{
			int nCount = oNodes.GetCount();

			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oNodeS;
				oNodes.GetAt(i, oNodeS);

				CString sName = oNodeS.GetName();

				if (_T("#text") == sName)
				{
					CSpan oSpan;
					oSpan.m_oRun = oRun;
					oSpan.m_strText = oNodeS.GetXml();

					CorrectXmlString2(oSpan.m_strText);

					pParagraph->m_arSpans.push_back(oSpan);
				}
				else if (_T("br") == sName)
				{
					CTextCFRun oRunBR;
					CCSS _oStyleCF;
                    CString sAttribute = oNodeS.GetAttribute(_T("style"));
                    _oStyleCF.LoadFromString2(sAttribute);
					_oStyleCF.LoadStyleCF(oRunBR, oMetric);
					oRunBR.ApplyBefore(oRun);

					CSpan oSpan;
					oSpan.m_oRun = oRunBR;
					oSpan.m_strText = _T("\n");
					pParagraph->m_arSpans.push_back(oSpan);
				}
				else if (_T("span") == sName)
				{
					ConvertSpan(pParagraph, oMetric, oNodeS, &oRun);
				}
			}
		}		
	}
};

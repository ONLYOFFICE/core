#pragma once
#include "..\stdafx.h"
#include "Const.h"
#include "StringWriter.h"
#include "..\Graphics\Structures.h"
#include "..\Graphics\Matrix.h"
#include "FontManager.h"

namespace NSHtmlRenderer
{
	static wchar_t g_wc_amp		= wchar_t('&');
	static wchar_t g_wc_apos	= wchar_t('\'');
	static wchar_t g_wc_lt		= wchar_t('<');
	static wchar_t g_wc_qt		= wchar_t('>');
	static wchar_t g_wc_quot	= wchar_t('\"');

	static _bstr_t g_bstr_amp	= L"&amp;";
	static _bstr_t g_bstr_apos	= L"&apos;";
	static _bstr_t g_bstr_lt	= L"&lt;";
	static _bstr_t g_bstr_qt	= L"&gt;";
	static _bstr_t g_bstr_quot	= L"\"";

	class CStyles
	{
	private:
		CAtlMap<CString, LONG>			m_mapStyles;
		LONG							m_lNexrID;

	public:
		NSStrings::CStringWriter		m_oWriterCSS;
		double							m_dDpiY;

	public:
		CStyles() : m_mapStyles(), m_lNexrID(0)
		{
		}
		~CStyles()
		{
			m_mapStyles.RemoveAll();
		}

	public:
		void NewDocument()
		{
			m_mapStyles.RemoveAll();
			m_lNexrID = 0;

			m_oWriterCSS.Clear();


			CString strCSS = 
_T(".blockpage { position: relative; background: #FFFFFF; padding: 0; float: none; margin: 0px auto; overflow: hidden; box-shadow: 0 0 10px rgba(0,0,0,0.5); -moz-box-shadow: 0 0 10px rgba(0,0,0,0.5); -webkit-box-shadow: 0 0 10px rgba(0,0,0,0.5); }\n\
.blockpagebetween { width: 100%; height: 20px; background: #FEFEFE; padding: 0px; float: none; text-align: center; }\n\
.bt { position: absolute; margin-top: 0px; padding: 0px; float: none; }\n\
.bs { margin-top: 0px; padding: 0px; float: none; }\n\
.rt { position: absolute; font-size: 0; padding: 0px; margin-left: 0px; margin-top: 0px; border-style: solid; }\n\
._svg { -moz-user-select: none;-khtml-user-select: none;user-select: none; width:100%; height:100%;}\n\
v\\:vmlframe { behavior:url(#default#VML); display: block; width: 100%; height: 100%; position: absolute; padding: 0px; margin-left:0px;margin-top:0px; }\n\
v\\:*{behavior:url(#default#VML)}\n");

			m_oWriterCSS.WriteString(strCSS);
		}
		void CloseDocument()
		{
			m_mapStyles.RemoveAll();
			m_lNexrID = 0;

			m_oWriterCSS.Clear();
		}

		void WriteStylesForDiffBrowsers(const CString& strDir)
		{
			CString strIE = _T(".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; text-align: justify; text-align-last: justify; }");
			CString strOp = _T(".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; display:inline-table; text-align: justify; text-align-last: justify; }\
.bp:after { content:\"\"; display:inline-block; width:100%; font-size: 0px; height:0; overflow:hidden; }");
			CString strOt = _T(".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; display:inline-table; white-space:pre-line; text-align: justify; text-align-last: justify; }\
.bp:after { content:\"\"; display:inline-block; width:100%; font-size: 0px; height:0; overflow:hidden; }");

			CDirectory::SaveToFile(strDir + _T("\\css_ie.css"), strIE);
			CDirectory::SaveToFile(strDir + _T("\\css_opera.css"), strOp);
			CDirectory::SaveToFile(strDir + _T("\\css_other.css"), strOt);
		}

		LONG GetClassID(NSStructures::CFont* pFont, NSStructures::CBrush* pBrush)
		{
			CString strFormat = _T("");

			int nSize = round(m_dDpiY * pFont->Size / 72);

			if (nSize >= 10)
				--nSize;

			if (pFont->Italic && pFont->Bold)
			{
				strFormat.Format(g_string_text_css_ib, pFont->Name, nSize, ConvertColor(pBrush->Color1));
			}
			else if (pFont->Italic)
			{
				strFormat.Format(g_string_text_css_i, pFont->Name, nSize, ConvertColor(pBrush->Color1));
			}
			else if (pFont->Bold)
			{
				strFormat.Format(g_string_text_css_b, pFont->Name, nSize, ConvertColor(pBrush->Color1));
			}
			else
			{
				strFormat.Format(g_string_text_css, pFont->Name, nSize, ConvertColor(pBrush->Color1));
			}

			CAtlMap<CString, LONG>::CPair* pPair = m_mapStyles.Lookup(strFormat);

			if (NULL == pPair)
			{
				// новый стиль
				++m_lNexrID;
				m_mapStyles.SetAt(strFormat, m_lNexrID);

				CString strCSS = _T("");				
				strCSS.Format(g_string_style_name, m_lNexrID);

				m_oWriterCSS.WriteString(strCSS);
				m_oWriterCSS.WriteString(strFormat);
				m_oWriterCSS.WriteString(g_bstr_script_close);

				return m_lNexrID;
			}

			return pPair->m_value;
		}
	};

	class CWord
	{
	public:
		NSStructures::CFont		m_oFont;
		NSStructures::CBrush	m_oBrush;
		NSStrings::CTextItem	m_oText;
		
		double					m_dX;
		double					m_dY;
		double					m_dWidth;
		double					m_dHeight;
		double					m_dBaseLinePos;
		
		double					m_dWidthWithoutSpaces;

	public:
		CWord() : m_oFont(), m_oBrush(), m_oText(10)
		{
			m_dX				= 0;
			m_dY				= 0;
			m_dWidth			= 0;
			m_dHeight			= 0;
			m_dBaseLinePos		= 0;

			m_dWidthWithoutSpaces = 0;
		}
		CWord(CFontManager* pFontManager, NSStructures::CBrush* pBrush, NSStrings::CTextItem& oText, 
			double& x, double& y, double& width, double& height, double& baseoffset) : m_oFont(pFontManager->m_oFont.m_oFont), m_oBrush(*pBrush), m_oText(oText), 
			m_dX(x), m_dY(y), m_dWidth(width), m_dHeight(height), m_dBaseLinePos(baseoffset), m_dWidthWithoutSpaces(width)
		{
			if (GetCountSpaceLast() == m_oText.GetCurSize())
				m_dWidthWithoutSpaces = 0;

			if (_T("") != pFontManager->m_strCurrentPickFont)
			{
				m_oFont.Name	= pFontManager->m_strCurrentPickFont;
				m_oFont.SetStyle(pFontManager->m_lCurrentPictFontStyle);
			}
		}
		CWord(const CWord& oSrc)
		{
			*this = oSrc;
		}
		CWord& operator=(const CWord& oSrc)
		{
			m_oFont				= oSrc.m_oFont;
			m_oBrush			= oSrc.m_oBrush;
			m_oText				= oSrc.m_oText;
			
			m_dX				= oSrc.m_dX;
			m_dY				= oSrc.m_dY;
			m_dWidth			= oSrc.m_dWidth;
			m_dHeight			= oSrc.m_dHeight;
			m_dBaseLinePos		= oSrc.m_dBaseLinePos;

			m_dWidthWithoutSpaces	= oSrc.m_dWidthWithoutSpaces;
			return *this;
		}

		long GetCountSpaceLast()
		{
			LONG lCount		= (LONG)m_oText.GetCurSize();
			wchar_t* pData	= m_oText.GetBuffer(); 
			LONG lCur = 0;
			while (lCount > 0)
			{
				if (((WCHAR)' ') == pData[lCount - 1])
					++lCur;
				else
					break;

				--lCount;
			}
			return lCur;
		}
		long GetCountLastPoints()
		{
			LONG lCount		= (LONG)m_oText.GetCurSize();
			wchar_t* pData	= m_oText.GetBuffer() + lCount - 1;
			LONG lCur = 0;
			while (lCount > 0)
			{
				if (((WCHAR)'.') == *pData)
					++lCur;
				else
					break;

				--lCount;
			}
			return lCur;
		}

		AVSINLINE void GetCorrectString(CString& strText)
		{
			strText.Replace(_T("&"),	_T("&amp;"));			
			strText.Replace(_T("'"),	_T("&apos;"));
			strText.Replace(_T("<"),	_T("&lt;"));
			strText.Replace(_T(">"),	_T("&gt;"));
			strText.Replace(_T("\""),	_T("&quot;"));
		}
		AVSINLINE void WriteToStringWriter(NSStrings::CStringWriter& oWriter)
		{
			size_t nCurrent = 0;
			size_t nCount	= m_oText.GetCurSize();

			size_t nCurrentOld = nCurrent;
			wchar_t* pData = m_oText.GetBuffer();
			wchar_t* pStartData = pData;

			while (nCurrent < nCount)
			{
				wchar_t c = *pData++;

				if (g_wc_amp == c)
				{
					if (nCurrentOld != nCurrent)
						oWriter.WriteString(pStartData, nCurrent - nCurrentOld);

					oWriter.WriteString(g_bstr_amp);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				/*else if (g_wc_apos == c)
				{
					if (nCurrentOld != nCurrent)
						oWriter.WriteString(pStartData, nCurrent - nCurrentOld);

					oWriter.WriteString(g_bstr_apos);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}*/
				else if (g_wc_lt == c)
				{
					if (nCurrentOld != nCurrent)
						oWriter.WriteString(pStartData, nCurrent - nCurrentOld);

					oWriter.WriteString(g_bstr_lt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_qt == c)
				{
					if (nCurrentOld != nCurrent)
						oWriter.WriteString(pStartData, nCurrent - nCurrentOld);

					oWriter.WriteString(g_bstr_qt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_quot == c)
				{
					if (nCurrentOld != nCurrent)
						oWriter.WriteString(pStartData, nCurrent - nCurrentOld);

					oWriter.WriteString(g_bstr_quot);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else
				{
					++nCurrent;
				}
			}

			if (nCurrentOld != nCurrent)
				oWriter.WriteString(pStartData, nCurrent - nCurrentOld);			
		}
	};

	class CTextLine
	{
	public:
		NSStrings::CStringWriter* m_pTextMeasurer;
		CAtlList<CWord*> m_arWords;
		LONG m_lNextID;
		double m_dMinTop;

		LONG m_lIndexZ;

	public:
		CTextLine() : m_arWords(), m_lIndexZ(0)
		{
		}
		CTextLine(const CTextLine& oSrc)
		{
			*this = oSrc;
		}
		CTextLine& operator=(const CTextLine& oSrc)
		{
			Clear();
			
			POSITION pos = oSrc.m_arWords.GetHeadPosition();

			while (NULL != pos)
			{
				CWord* pWord	= oSrc.m_arWords.GetNext(pos);
				CWord* pWordNew	= new CWord(*pWord);

				m_arWords.AddTail(pWordNew);
			}

			return *this;
		}

	public:
		void Clear()
		{
			POSITION pos = m_arWords.GetHeadPosition();

			while (NULL != pos)
			{
				CWord* pWord = m_arWords.GetNext(pos);
				RELEASEOBJECT(pWord);
			}

			m_arWords.RemoveAll();

			m_dMinTop = 10000000;
		}
		void AddWord(CWord* pWord)
		{
			// смотрим, какое слово предыдущее.
			// если оно - полностью пробел - то он прищел из пдф. а значит он любой длины может быть.
			// зачем он нужен?? непонятно. Поэтому удаляем такое слово
			POSITION pos = m_arWords.GetTailPosition();
			if (NULL != pos)
			{
				CWord* pTail = m_arWords.GetNext(pos);
				if (pTail->GetCountSpaceLast() == pTail->m_oText.GetCurSize())
				{
					m_arWords.RemoveTail();
					RELEASEOBJECT(pTail);
				}
			}
			
			if (m_dMinTop > pWord->m_dY)
				m_dMinTop = pWord->m_dY;
			
			m_arWords.AddTail(pWord);
		}
		CWord* GetLast()
		{
			return m_arWords.GetTail();
		}
		CWord* GetHead()
		{
			return m_arWords.GetHead();
		}
		size_t GetCount()
		{
			return m_arWords.GetCount();
		}

	public:

		inline void Write(NSStrings::CStringWriter* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles, const LONG& lIndexZ)
		{
			return Write3(pWriter, dDpiX, dDpiY, pStyles, lIndexZ);
		}

		void Write1(NSStrings::CStringWriter* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles)
		{
			// временно 
			//return Write2(pWriter, dDpiX, dDpiY);

			size_t nCountWords = m_arWords.GetCount();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
			CWord* pWord = m_arWords.GetTail();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;
			
			CString strPar = _T("");
			strPar.Format(g_string_paragraph2, pWord->m_oFont.Name, (int)(dDpiY * pWord->m_oFont.Size / 72), ConvertColor(pWord->m_oBrush.Color1));

			pWriter->WriteString(strPar);

			POSITION pos = m_arWords.GetHeadPosition();
			POSITION posOld = m_arWords.GetTailPosition();
			while (NULL != pos)
			{
				pWord = m_arWords.GetNext(pos);

				if (pWord->m_oFont.IsEqual(pFont0) && (pWord->m_oBrush.IsEqual(pBrush0)))
				{
					CString strSpan = _T("");
					strSpan.Format(g_string_span1, (int)pWord->m_dX, (int)pWord->m_dY);

					pWriter->WriteString(strSpan);
				}
				else
				{
					CString strSpan = _T("");
					strSpan.Format(g_string_span3, pWord->m_oFont.Name, (int)(dDpiY * pWord->m_oFont.Size / 72), ConvertColor(pWord->m_oBrush.Color1));

					pWriter->WriteString(strSpan);
				}

				//pWriter->WriteString(pWord->m_strText);
				pWord->WriteToStringWriter(*pWriter);

				if (pos != posOld)
				{
					pWriter->WriteString(g_bstr_space);
				}
				pWriter->WriteString(g_bstr_span_end);
			}

			pWriter->WriteString(g_bstr_paragraph_end);
		}
		void Write2(NSStrings::CStringWriter* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles)
		{
			size_t nCountWords = m_arWords.GetCount();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
			CWord* pWord = m_arWords.GetHead();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;

			
			CString strPar = _T("");
			strPar.Format(g_string_paragraph3, (int)pWord->m_dX, (int)pWord->m_dY, pWord->m_oFont.Name, (int)(dDpiY * pWord->m_oFont.Size / 72), ConvertColor(pWord->m_oBrush.Color1));

			pWriter->WriteString(strPar);

			CString strText = _T("");
			POSITION pos = m_arWords.GetHeadPosition();
			while (NULL != pos)
			{
				pWord = m_arWords.GetNext(pos);
				strText += pWord->m_oText.GetCString();

				if (NULL != pos)
					strText += _T(" ");
			}

			pWriter->WriteString(strText);

			pWriter->WriteString(g_bstr_paragraph_end);
		}

		void Write3(NSStrings::CStringWriter* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles, const LONG& lIndexZ)
		{
			DeleteEmptyWords();

			size_t nCountWords = m_arWords.GetCount();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
			CWord* pWord = m_arWords.GetHead();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;

			pWord->m_dWidth = pWord->m_dWidthWithoutSpaces;

			int nLeft  = (int)pWord->m_dX;
			int nRight = (int)(pWord->m_dX + pWord->m_dWidthWithoutSpaces);

			double dRight = pWord->m_dX + pWord->m_dWidthWithoutSpaces;

			POSITION pos = m_arWords.GetHeadPosition();
			m_arWords.GetNext(pos);
			while (NULL != pos)
			{
				pWord = m_arWords.GetNext(pos);
				pWord->m_dWidth = pWord->m_dWidthWithoutSpaces;
				nRight = (int)(pWord->m_dX + pWord->m_dWidth);
			}

			pos		= m_arWords.GetHeadPosition();
			pWord	= m_arWords.GetHead();

			pStyles->m_dDpiY = dDpiY;
			LONG lID = pStyles->GetClassID(&pWord->m_oFont, &pWord->m_oBrush);

			LONG lDecoration = pWord->m_oFont.GetTextDecorationStyle();

			CString strPar = _T("");
			strPar.Format(g_string_paragraph_style, lID, (int)pWord->m_dX, (int)m_dMinTop, nRight - nLeft);

			pWriter->WriteString(strPar);
			pWriter->WriteString(g_bstr_nobr_start);

			if ((0 == lDecoration) || (3 < lDecoration))
			{
				//pWriter->WriteString(pWord->GetCorrectString());
				pWord->WriteToStringWriter(*pWriter);
			}
			else
			{
				switch (lDecoration)
				{
				case 1:
					pWriter->WriteString(g_string_span_style_underline2);
					break;
				case 2:
					pWriter->WriteString(g_string_span_style_strike2);
					break;
				case 3:
					pWriter->WriteString(g_string_span_style_underline_strike2);
					break;
				default:
					break;
				}

				//pWriter->WriteString(pWord->GetCorrectString());
				pWord->WriteToStringWriter(*pWriter);
				pWriter->WriteString(g_bstr_span_end);
			}

			m_arWords.GetNext(pos);

			bool bSpanClose = false;
			while (NULL != pos)
			{
				pWord = m_arWords.GetNext(pos);

				if (1.0 < (pWord->m_dX - dRight))
				{
					pWriter->WriteString(g_bstr_space2);
				}
				dRight = pWord->m_dX + pWord->m_dWidthWithoutSpaces;

				if (bSpanClose)
					pWriter->WriteString(g_bstr_span_end);

				bSpanClose = true;

				lID = pStyles->GetClassID(&pWord->m_oFont, &pWord->m_oBrush);

				CString strSpan = _T("");

				lDecoration = pWord->m_oFont.GetTextDecorationStyle();
				switch (lDecoration)
				{
				case 0:
					strSpan.Format(g_string_span_style, lID);
					break;
				case 1:
					strSpan.Format(g_string_span_style_underline, lID);
					break;
				case 2:
					strSpan.Format(g_string_span_style_strike, lID);
					break;
				case 3:
					strSpan.Format(g_string_span_style_underline_strike, lID);
					break;
				default:
					strSpan.Format(g_string_span_style, lID);
					break;
				}

				pWriter->WriteString(strSpan);
				//pWriter->WriteString(pWord->GetCorrectString());
				pWord->WriteToStringWriter(*pWriter);
			}

			if (bSpanClose)
				pWriter->WriteString(g_bstr_span_end);

			pWriter->WriteString(g_bstr_nobr_end);
			pWriter->WriteString(g_bstr_paragraph_end);

			//CString strLine = _T("");
			//strLine.Format(g_string_lineFunc, m_lNextID++, nRight - nLeft);
			//m_pTextMeasurer->WriteString(strLine);
		}

		void DeleteEmptyWords()
		{
			POSITION pos = m_arWords.GetHeadPosition();
			CWord* pWord = NULL;
			while (NULL != pos)
			{
				POSITION posOld = pos;
				pWord = m_arWords.GetNext(pos);

				if (pWord->GetCountSpaceLast() == pWord->m_oText.GetCurSize())
				{
					m_arWords.RemoveAt(posOld);
					RELEASEOBJECT(pWord);
				}
			}

			if (0 != m_arWords.GetCount())
			{
				// удалим последние пробелы линии.
				pWord = m_arWords.GetTail();
				pWord->m_oText.RemoveLastSpaces();
			}
		}

		void Merge()
		{
			POSITION pos		= m_arWords.GetHeadPosition();
			if (NULL == pos)
				return;
			
			POSITION posCur		= pos;
			CWord* pWordPrev	= m_arWords.GetNext(posCur);

			double dRightPos	= pWordPrev->m_dX + pWordPrev->m_dWidth;			

			while (NULL != posCur)
			{
				POSITION posCurOld = posCur;

				CWord* pWord = m_arWords.GetNext(posCur);
				bool bNextWord = true;
				
				if (FABS(pWord->m_dX - dRightPos) < 0.1)
				{
					// одно и тоже слово
					bNextWord = false;
				}

				dRightPos = pWord->m_dX + pWord->m_dWidth;
				
				if ((pWord->m_oFont.IsEqual(&pWordPrev->m_oFont)) && (pWord->m_oBrush.IsEqual(&pWordPrev->m_oBrush)))
				{
					// продолжаем текущий спан
					if (bNextWord)
					{
						pWordPrev->m_oText.AddSpace();
					}
					pWordPrev->m_oText += pWord->m_oText;

					m_arWords.RemoveAt(posCurOld);
					RELEASEOBJECT(pWord);
				}
				else
				{
					if (bNextWord)
						pWordPrev->m_oText.AddSpace();

					pos			= posCur;
					pWordPrev	= pWord;
				}
			}
		}
	};

	class CText
	{
	public:
		CFontManager	m_oFontManager;
		CTextLine		m_oCurrentLine;

		NSStrings::CStringWriter* m_pWriter;

		//NSStrings::CStringWriter m_oTextMeasurer;

		CStyles*		m_pStyles;

		double			m_dDpiX;
		double			m_dDpiY;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSHtmlRenderer::CMatrix* m_pTransform;

		LONG			m_lIndexZ;

		bool			m_bIsNewLine;
		bool			m_bIsPDFTextStyle;

		NSStrings::CTextItem m_oTextItem;
		
	private:
		double			m_dEpsX;
		double			m_dEpsY;

	public:
		CText() : m_oFontManager(), m_oCurrentLine(), m_oTextItem(10)//, m_oTextMeasurer()
 		{
			m_pWriter	= NULL;
			m_dEpsX		= 0.5;
			m_dEpsY		= 0.5;

			m_pPen		= NULL;
			m_pBrush	= NULL;
			m_pFont		= NULL;

			//m_oCurrentLine.m_pTextMeasurer = &m_oTextMeasurer;

			m_pStyles	= NULL;

			m_pTransform	= NULL;

			m_lIndexZ	= 0;

			m_bIsNewLine = false;
			m_bIsPDFTextStyle = false;
		}

		void SetParams(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CFont* pFont, CStyles* pStyles, NSHtmlRenderer::CMatrix* pTransform)
		{
			m_pPen					= pPen;
			m_pBrush				= pBrush;
			m_pFont					= pFont;

			m_oFontManager.m_pFont	= pFont;

			m_oFontManager.m_pTransform = pTransform;

			m_pStyles				= pStyles;

			m_pTransform			= pTransform;
		}

		~CText()
		{
		}

		void NewPage(double& dDpiX, double& dDpiY)
		{
			m_dDpiX	= dDpiX;
			m_dDpiY = dDpiY;

			m_pPen->SetDefaultParams();
			m_pBrush->SetDefaultParams();
			m_pFont->SetDefaultParams();
		}
		void ClosePage()
		{
			m_oCurrentLine.Write(m_pWriter, m_dDpiX, m_dDpiY, m_pStyles, m_lIndexZ);
		}

		void NewDocument()
		{
			m_oCurrentLine.m_lNextID = 1;
			m_oCurrentLine.m_dMinTop = 1000000;

			m_pPen->SetDefaultParams();
			m_pBrush->SetDefaultParams();
			m_pFont->SetDefaultParams();
		}

	
	protected:

		AVSINLINE bool GetIsNewLine()
		{
			if (m_bIsNewLine)
			{
				m_bIsNewLine = false;
				return true;
			}
			return false;
		}
		
		void CommandText1(double& x, double& y, double& width, double& height, double& baselineoffset)
		{
			double dScaleFont = ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);
			BOOL bIsFontChanged = !m_oFontManager.m_oFont.m_oFont.IsEqual2(m_pFont, dScaleFont);

			BOOL bIsTextDecorationChanged = ((m_oFontManager.m_oFont.m_oFont.Underline != m_pFont->Underline) ||
												(m_oFontManager.m_oFont.m_oFont.Strikeout != m_pFont->Strikeout));

			if (bIsFontChanged)
			{
				m_oFontManager.m_oFont.m_oFont = *m_pFont;
				m_oFontManager.m_oFont.m_oFont.Size *= dScaleFont;
			}
			else if (bIsTextDecorationChanged)
			{
				m_oFontManager.m_oFont.m_oFont.Underline	= m_pFont->Underline;
				m_oFontManager.m_oFont.m_oFont.Strikeout	= m_pFont->Strikeout;
			}

			// никакого подбора здесь нет. нужное имя должно быть выставлено у m_pFont			
			m_oFontManager.m_strCurrentPickFont		= _T("");

			bool bIsNewLine = GetIsNewLine();

			if (0 == width)
			{
				// откуда такое может быть?? формулы??
				m_oFontManager.LoadCurrentFont();

				double dOffset = m_oFontManager.m_oFont.m_dBaselineOffsetHTML;
				dOffset *= (m_dDpiY / 72.0);

				y -= dOffset;
				baselineoffset = dOffset;

				// текст не измерен. для начала нужно его измерить
				double _x = 0;
				double _y = 0;
				double _w = 0;
				double _h = 0;

				LONG lGid = m_oFontManager.GetStringGid();
				m_oFontManager.SetStringGid(0);
				m_oFontManager.MeasureStringPix(m_oTextItem.GetCString(), x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				m_oFontManager.SetStringGid(lGid);

				width  = _w;
				height = _h;
			}

			double dYPos		= y + baselineoffset;
			LONG lCountWords	= (LONG)m_oCurrentLine.GetCount();

			if (0 == lCountWords)
			{
				// слов в линии нет, без всяких проверок создаем линию
				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
				return;
			}

			CWord* pTail = m_oCurrentLine.GetLast();
			double dYPosLine = pTail->m_dBaseLinePos;

			if (FABS(dYPos - dYPosLine) > m_dEpsY)
			{
				// новая линия
				// сначала запишем предыдущую линию
				m_oCurrentLine.Write(m_pWriter, m_dDpiX, m_dDpiY, m_pStyles, m_lIndexZ);

				m_oCurrentLine.Clear();

				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
				return;
			}
			
			double dRight = pTail->m_dX + pTail->m_dWidth;
			if ((x + 0.5 < dRight) || (((x - dRight) * c_ag_Inch_to_MM / m_dDpiX) > g_lNewNoJustifySpace) || (bIsNewLine))
			{
				// здесь либо текст стал левее, либо расстояние слишком большое.

				// создадим новую линию
				m_oCurrentLine.Write(m_pWriter, m_dDpiX, m_dDpiY, m_pStyles, m_lIndexZ);

				m_oCurrentLine.Clear();

				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
				return;
			}

			// текст уже измерен
			if (!bIsFontChanged && pTail->m_oBrush.IsEqual(m_pBrush) && !bIsTextDecorationChanged)
			{
				if (1.0 < FABS(pTail->m_dX + pTail->m_dWidth - x))
				{
					// это не одно и то же слово!!!
					pTail->m_oText.AddSpace();
				}

				pTail->m_oText += m_oTextItem;
				pTail->m_dWidth = (x + width - pTail->m_dX);
				pTail->m_dWidthWithoutSpaces = pTail->m_dWidth;
			}
			else
			{
				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
			}
		}
		void CommandText2(BSTR bsGid, double& x, double& y, double& width, double& height, double& baselineoffset)
		{
			bool bIsNewLine = GetIsNewLine();

			double dScaleFont = ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);
			BOOL bIsFontChanged = !m_oFontManager.m_oFont.m_oFont.IsEqual2(m_pFont, dScaleFont);

			if (bIsFontChanged)
			{
				m_oFontManager.m_oFont.m_oFont = *m_pFont;
				m_oFontManager.m_oFont.m_oFont.Size *= dScaleFont;

				m_oFontManager.LoadCurrentFont();
				m_oFontManager.CalculateSpace();

				m_pFont->SetStyle(m_oFontManager.m_oFont.m_oFont.GetStyle());

				m_oFontManager.GenerateFontName(m_oTextItem, true);
			}
			else
			{
				m_oFontManager.GenerateFontName(m_oTextItem, false);
			}

			double dOffset = m_oFontManager.m_oFont.m_dBaselineOffsetHTML;
			
			dOffset *= (m_dDpiY / 72.0);

			y -= dOffset;
			baselineoffset = dOffset;

			// текст не измерен. для начала нужно его измерить
			double _x = 0;
			double _y = 0;
			double _w = 0;
			double _h = 0;

			bool bIsMeasuring = false;

			if (0 >= width)
			{	
				if (NULL == bsGid)
				{
					m_oFontManager.SetStringGid(0);
					BSTR bsText = m_oTextItem.GetCString().AllocSysString();
					m_oFontManager.MeasureStringPix(bsText, x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
					SysFreeString(bsText);
				}
				else
				{
					m_oFontManager.SetStringGid(1);
					m_oFontManager.MeasureStringPix(bsGid, x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
				}

				width  = _w;
				height = _h;

				bIsMeasuring = true;
			}

			double dYPos		= y + baselineoffset;
			LONG lCountWords	= (LONG)m_oCurrentLine.GetCount();

			if (0 == lCountWords)
			{
				// слов в линии нет, без всяких проверок создаем линию
				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);

				return;
			}

			double dYPosLine = m_oCurrentLine.GetHead()->m_dBaseLinePos;

			if (FABS(dYPos - dYPosLine) > m_dEpsY)
			{
				// новая линия
				// сначала запишем предыдущую линию
				m_oCurrentLine.Write(m_pWriter, m_dDpiX, m_dDpiY, m_pStyles, m_lIndexZ);

				m_oCurrentLine.Clear();
				
				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);

				return;
			}

			// теперь проверяем, продолжать ли слово
			//double dSpace = MMToPX(m_oFontManager.m_dSpaceWidthMM, m_dDpiX) / 2.0;
			double dSpace = 0.5 * m_dDpiX / c_dInchToMM;
			if (bIsMeasuring)
			{
				dSpace = m_oFontManager.m_dSpaceWidthMM * m_dDpiX / 25.4;
			}

			CWord* pWordLast = m_oCurrentLine.GetLast();

			double dRight = pWordLast->m_dX + pWordLast->m_dWidthWithoutSpaces;
			if ((x + 10 < dRight) || (((x - dRight) * c_ag_Inch_to_MM / m_dDpiX) > g_lNewNoJustifySpace) || (bIsNewLine))
			{
				// создадим новую линию
				m_oCurrentLine.Write(m_pWriter, m_dDpiX, m_dDpiY, m_pStyles, m_lIndexZ);

				m_oCurrentLine.Clear();

				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
				return;
			}

			bool bIsBrushChanged = !pWordLast->m_oBrush.IsEqual(m_pBrush);
			double dDelta = (x - (pWordLast->m_dX + pWordLast->m_dWidth));
			if ((dDelta > dSpace) || bIsFontChanged || bIsBrushChanged)
			{
				if (m_oTextItem.IsSpace())
				{
					if (0 == pWordLast->GetCountSpaceLast())
					{
						pWordLast->m_oText += m_oTextItem;
						pWordLast->m_dWidth = x + width - pWordLast->m_dX;
					}
					return;
				}
				
				if (dDelta > dSpace)
					pWordLast->m_oText.AddSpace();

				if (bIsFontChanged || bIsBrushChanged)
				{
					CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
					m_oCurrentLine.AddWord(pWord);					
				}
				else
				{
					pWordLast->m_oText += m_oTextItem;
					pWordLast->m_dWidth					= x + width - pWordLast->m_dX;
					pWordLast->m_dWidthWithoutSpaces	= pWordLast->m_dWidth;
				}
			}
			else
			{
				// продолжаем слово
				
				// сначала заглушку на строку, начинающуюся пробелами
				if (pWordLast->GetCountSpaceLast() == pWordLast->m_oText.GetCurSize())
				{
					pWordLast->m_oText = m_oTextItem;
					pWordLast->m_dX		 = x;
					pWordLast->m_dWidth	 = width;
				}
				else
				{
					pWordLast->m_oText += m_oTextItem;
					pWordLast->m_dWidth		= x + width - pWordLast->m_dX; // чтобы не накапливалась ошибка, не используем сумму длин

					if (5 < pWordLast->GetCountLastPoints())
						m_bIsNewLine = true;
				}

				if (!m_oTextItem.IsSpace())
					pWordLast->m_dWidthWithoutSpaces = pWordLast->m_dWidth;
			}
		}
		
	public:
		AVSINLINE void CommandText(BSTR& bsText, BSTR& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset, LONG& lIndexZ)
		{
			if (NULL == bsText)
				return;

			m_lIndexZ = lIndexZ;
			m_oTextItem.SetText(bsText);

			if (0 == m_oTextItem.GetCurSize())
				return;

			if (bsGid != NULL)
			{
				m_oTextItem.CorrectUnicode(m_oFontManager.m_mapUnicode);
				return CommandText2(bsGid, x, y, width, height, baselineoffset);
			}
			if (m_bIsPDFTextStyle)
			{
				return CommandText2(bsGid, x, y, width, height, baselineoffset); 
			}

			return CommandText1(x, y, width, height, baselineoffset);
		}
	};

}
#ifndef _ASC_HTMLRENDERER_TEXT_H_
#define _ASC_HTMLRENDERER_TEXT_H_

#include "FontManager.h"

namespace NSHtmlRenderer
{
	class CStyles
	{
	private:
        std::map<std::wstring, LONG>	m_mapStyles;
		LONG							m_lNexrID;

	public:
        NSStringUtils::CStringBuilder	m_oWriterCSS;
		double							m_dDpiY;

	public:
		CStyles() : m_mapStyles(), m_lNexrID(0)
		{
		}
		~CStyles()
		{
            m_mapStyles.clear();
		}

	public:
		void NewDocument()
		{
            m_mapStyles.clear();
			m_lNexrID = 0;

			m_oWriterCSS.Clear();


            std::wstring strCSS =
L".blockpage { position: relative; background: #FFFFFF; padding: 0; float: none; margin: 0px auto; overflow: hidden; box-shadow: 0 0 10px rgba(0,0,0,0.5); -moz-box-shadow: 0 0 10px rgba(0,0,0,0.5); -webkit-box-shadow: 0 0 10px rgba(0,0,0,0.5); }\n\
.blockpagebetween { width: 100%; height: 20px; background: #FEFEFE; padding: 0px; float: none; text-align: center; }\n\
.bt { position: absolute; margin-top: 0px; padding: 0px; float: none; }\n\
.bs { margin-top: 0px; padding: 0px; float: none; }\n\
.rt { position: absolute; font-size: 0; padding: 0px; margin-left: 0px; margin-top: 0px; border-style: solid; }\n\
._svg { -moz-user-select: none;-khtml-user-select: none;user-select: none; width:100%; height:100%;}\n\
v\\:vmlframe { behavior:url(#default#VML); display: block; width: 100%; height: 100%; position: absolute; padding: 0px; margin-left:0px;margin-top:0px; }\n\
v\\:*{behavior:url(#default#VML)}\n";

			m_oWriterCSS.WriteString(strCSS);
		}
		void CloseDocument()
		{
            m_mapStyles.clear();
			m_lNexrID = 0;

			m_oWriterCSS.Clear();
		}

        void WriteStylesForDiffBrowsers(const std::wstring& strDir)
		{
            std::wstring strIE = L".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; text-align: justify; text-align-last: justify; }";
            std::wstring strOp = L".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; display:inline-table; text-align: justify; text-align-last: justify; }\
.bp:after { content:\"\"; display:inline-block; width:100%; font-size: 0px; height:0; overflow:hidden; }";
            std::wstring strOt = L".bp { position: absolute; z-index:100; margin-left: 0px; margin-top: 0px; padding: 0px; float: none; display:inline-table; white-space:pre-line; text-align: justify; text-align-last: justify; }\
.bp:after { content:\"\"; display:inline-block; width:100%; font-size: 0px; height:0; overflow:hidden; }";

            NSFile::CFileBinary::SaveToFile(strDir + L"\\css_ie.css", strIE);
            NSFile::CFileBinary::SaveToFile(strDir + L"\\css_opera.css", strOp);
            NSFile::CFileBinary::SaveToFile(strDir + L"\\css_other.css", strOt);
		}

		LONG GetClassID(NSStructures::CFont* pFont, NSStructures::CBrush* pBrush)
		{
            int nSize = round(m_dDpiY * pFont->Size / 72);

			if (nSize >= 10)
				--nSize;

            NSStringUtils::CStringBuilder oBuilder;
            oBuilder.WriteString(L"font-family: ");
            oBuilder.WriteString(pFont->Name);
            oBuilder.WriteString(L"; font-size: ");
            oBuilder.AddInt(nSize);
            oBuilder.WriteString(L"px; color:");
            oBuilder.WriteHexColor3(pBrush->Color1);

			if (pFont->Italic && pFont->Bold)
			{
                oBuilder.WriteString(L"; font-style:italic; font-weight:bold;", 38);
			}
			else if (pFont->Italic)
			{
                oBuilder.WriteString(L"; font-style:italic; font-weight:normal;", 40);
			}
			else if (pFont->Bold)
			{
                oBuilder.WriteString(L"; font-weight:bold; font-style:normal;", 38);
			}
			else
			{
                oBuilder.WriteString(L"; font-style:normal; font-weight:normal;", 40);
			}

            std::wstring sFormat = oBuilder.GetData();
            std::map<std::wstring, LONG>::iterator iFind = m_mapStyles.find(sFormat);

            if (iFind == m_mapStyles.end())
			{
				// новый стиль
				++m_lNexrID;
                m_mapStyles.insert(std::pair<std::wstring, LONG>(sFormat, m_lNexrID));

                m_oWriterCSS.WriteString(L".s", 2);
                m_oWriterCSS.AddInt(m_lNexrID);
                m_oWriterCSS.WriteString(L" {", 2);
                m_oWriterCSS.WriteString(sFormat);
                m_oWriterCSS.WriteString(L"}\n", 2);
				return m_lNexrID;
			}

            return iFind->second;
		}
	};

	class CWord
	{
	public:
		NSStructures::CFont		m_oFont;
		NSStructures::CBrush	m_oBrush;
        NSStringUtils::CStringBuilder	m_oText;
		
		double					m_dX;
		double					m_dY;
		double					m_dWidth;
		double					m_dHeight;
		double					m_dBaseLinePos;
		
		double					m_dWidthWithoutSpaces;

	public:
        CWord() : m_oFont(), m_oBrush(), m_oText()
		{
			m_dX				= 0;
			m_dY				= 0;
			m_dWidth			= 0;
			m_dHeight			= 0;
			m_dBaseLinePos		= 0;

			m_dWidthWithoutSpaces = 0;
		}
        CWord(CFontManager* pFontManager, NSStructures::CBrush* pBrush, NSStringUtils::CStringBuilder& oText,
			double& x, double& y, double& width, double& height, double& baseoffset) : m_oFont(pFontManager->m_oFont.m_oFont), m_oBrush(*pBrush), m_oText(oText), 
			m_dX(x), m_dY(y), m_dWidth(width), m_dHeight(height), m_dBaseLinePos(baseoffset), m_dWidthWithoutSpaces(width)
		{
			if (GetCountSpaceLast() == m_oText.GetCurSize())
				m_dWidthWithoutSpaces = 0;

            if (L"" != pFontManager->m_strCurrentPickFont)
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

        inline void WriteToStringWriter(NSStringUtils::CStringBuilder& oWriter)
		{
            oWriter.WriteEncodeXmlString(m_oText.GetBuffer(), (int)m_oText.GetCurSize());
        }
	};

	class CTextLine
	{
	public:
        NSStringUtils::CStringBuilder* m_pTextMeasurer;
        std::list<CWord*> m_arWords;
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
			
            for (std::list<CWord*>::const_iterator iter = oSrc.m_arWords.begin(); iter != oSrc.m_arWords.end(); iter++)
            {
                CWord* pWord = new CWord(**iter);
                m_arWords.push_back(pWord);
            }

            return *this;
		}

	public:
		void Clear()
        {
            for (std::list<CWord*>::iterator iter = m_arWords.begin(); iter != m_arWords.end(); iter++)
			{
                CWord* pWord = *iter;
				RELEASEOBJECT(pWord);
			}

            m_arWords.clear();

			m_dMinTop = 10000000;
		}
		void AddWord(CWord* pWord)
		{
			// смотрим, какое слово предыдущее.
			// если оно - полностью пробел - то он прищел из пдф. а значит он любой длины может быть.
			// зачем он нужен?? непонятно. Поэтому удаляем такое слово
            if (m_arWords.size() > 0)
			{
                std::list<CWord*>::iterator iter = m_arWords.end();
                iter--;
                CWord* pTail = *iter;
				if (pTail->GetCountSpaceLast() == pTail->m_oText.GetCurSize())
				{
                    m_arWords.pop_back();
					RELEASEOBJECT(pTail);
				}
			}
			
			if (m_dMinTop > pWord->m_dY)
				m_dMinTop = pWord->m_dY;
			
            m_arWords.push_back(pWord);
		}
		CWord* GetLast()
		{
            if (0 == m_arWords.size())
                return NULL;

            std::list<CWord*>::iterator iter = m_arWords.end();
            iter--;
            return *iter;
		}
		CWord* GetHead()
		{
            if (0 == m_arWords.size())
                return NULL;

            return *m_arWords.begin();
		}
		size_t GetCount()
		{
            return m_arWords.size();
		}

	public:

        inline void Write(NSStringUtils::CStringBuilder* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles, const LONG& lIndexZ)
		{
			return Write3(pWriter, dDpiX, dDpiY, pStyles, lIndexZ);
		}

        void Write1(NSStringUtils::CStringBuilder* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles)
		{
			// временно 
			//return Write2(pWriter, dDpiX, dDpiY);

            size_t nCountWords = m_arWords.size();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
            CWord* pWord = GetLast();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;
			
            pWriter->WriteString(L"<p class=\"bp\" style=\"font-family: ", 34);
            pWriter->WriteString(pWord->m_oFont.Name);
            pWriter->WriteString(L"; font-size: ");
            pWriter->AddInt((int)(dDpiY * pWord->m_oFont.Size / 72));
            pWriter->WriteString(L"px; color:", 10);
            pWriter->WriteHexColor3(pWord->m_oBrush.Color1);
            pWriter->WriteString(L";\">");

            for (std::list<CWord*>::iterator pos = m_arWords.begin(); pos != m_arWords.end();)
			{
                pWord = *pos;

				if (pWord->m_oFont.IsEqual(pFont0) && (pWord->m_oBrush.IsEqual(pBrush0)))
				{
                    pWriter->WriteString(L"<span class=\"bt\" style=\"left:", 29);
                    pWriter->AddInt((int)pWord->m_dX);
                    pWriter->WriteString(L"; top:", 6);
                    pWriter->AddInt((int)pWord->m_dY);
                    pWriter->WriteString(L";\">", 3);
				}
				else
				{
                    pWriter->WriteString(L"<span class=\"bs\" style=\"font-family: ", 37);
                    pWriter->WriteString(pWord->m_oFont.Name);
                    pWriter->WriteString(L"; font-size: ", 13);
                    pWriter->AddInt((int)(dDpiY * pWord->m_oFont.Size / 72));
                    pWriter->WriteString(L"px; color:", 10);
                    pWriter->WriteHexColor3(pWord->m_oBrush.Color1);
                    pWriter->WriteString(L";\">", 3);
				}

				pWord->WriteToStringWriter(*pWriter);

                pos++;
                if (pos != m_arWords.end())
				{
                    pWriter->WriteString(L"&nbsp;", 6);
				}
                pWriter->WriteString(L"</span>", 7);
			}

            pWriter->WriteString(L"</p>\n", 5);
		}
        void Write2(NSStringUtils::CStringBuilder* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles)
		{
            size_t nCountWords = m_arWords.size();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
            CWord* pWord = GetHead();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;

            pWriter->WriteString(L"<p class=\"bp\" style=\"left: ", 27);
            pWriter->AddInt((int)pWord->m_dX);
            pWriter->WriteString(L"px; top: ", 9);
            pWriter->AddInt((int)pWord->m_dY);
            pWriter->WriteString(L"px; font-family: ", 17);
            pWriter->WriteString(pWord->m_oFont.Name);
            pWriter->WriteString(L"; font-size: ", 13);
            pWriter->AddInt((int)(dDpiY * pWord->m_oFont.Size / 72));
            pWriter->WriteString(L"px; color:", 10);
            pWriter->WriteHexColor3(pWord->m_oBrush.Color1);
            pWriter->WriteString(L"; white-space: nowrap;\">", 24);
			
            for (std::list<CWord*>::iterator pos = m_arWords.begin(); pos != m_arWords.end();)
            {
                pWord = *pos;
                pWriter->Write(pWord->m_oText);

                pos++;
                if (pos != m_arWords.end())
                    pWriter->AddCharSafe(' ');
            }

            pWriter->WriteString(L"</p>\n", 5);
		}

        void Write3(NSStringUtils::CStringBuilder* pWriter, double& dDpiX, double& dDpiY, CStyles* pStyles, const LONG& lIndexZ)
		{
			DeleteEmptyWords();

            size_t nCountWords = m_arWords.size();
			if (0 == nCountWords)
				return;

			// генерим параграф, один на строку
            CWord* pWord = GetHead();
			NSStructures::CFont* pFont0		= &pWord->m_oFont;
			NSStructures::CBrush* pBrush0	= &pWord->m_oBrush;

			pWord->m_dWidth = pWord->m_dWidthWithoutSpaces;

			int nLeft  = (int)pWord->m_dX;
			int nRight = (int)(pWord->m_dX + pWord->m_dWidthWithoutSpaces);

			double dRight = pWord->m_dX + pWord->m_dWidthWithoutSpaces;

            std::list<CWord*>::iterator pos = m_arWords.begin();
            pos++;

            while (pos != m_arWords.end())
            {
                pWord->m_dWidth = pWord->m_dWidthWithoutSpaces;
                nRight = (int)(pWord->m_dX + pWord->m_dWidth);
                pos++;
            }

            pos		= m_arWords.begin();
            pWord	= *pos;

			pStyles->m_dDpiY = dDpiY;
			LONG lID = pStyles->GetClassID(&pWord->m_oFont, &pWord->m_oBrush);

			LONG lDecoration = pWord->m_oFont.GetTextDecorationStyle();

            pWriter->WriteString(L"<p class=\"bp s", 14);
            pWriter->AddInt(lID);
            pWriter->WriteString(L"\" style=\"left: ", 15);
            pWriter->AddInt((int)pWord->m_dX);
            pWriter->WriteString(L"px; top: ", 9);
            pWriter->AddInt((int)m_dMinTop);
            pWriter->WriteString(L"px; width: ", 11);
            pWriter->AddInt(nRight - nLeft);
            pWriter->WriteString(L"px;\"><nobr>", 11);

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
                    pWriter->WriteString(L"<span style=\"text-decoration:underline;\">", 41);
					break;
				case 2:
                    pWriter->WriteString(L"<span style=\"text-decoration:line-through;\">", 44);
					break;
				case 3:
                    pWriter->WriteString(L"<span style=\"text-decoration:underline line-through;\">", 54);
					break;
				default:
					break;
				}

				//pWriter->WriteString(pWord->GetCorrectString());
				pWord->WriteToStringWriter(*pWriter);
                pWriter->WriteString(L"</span>", 7);
			}

            pos++;

			bool bSpanClose = false;
            while (pos != m_arWords.end())
			{
                pWord = *pos;

				if (1.0 < (pWord->m_dX - dRight))
				{
                    pWriter->AddCharSafe(' ');
				}
				dRight = pWord->m_dX + pWord->m_dWidthWithoutSpaces;

				if (bSpanClose)
                    pWriter->WriteString(L"</span>", 7);

				bSpanClose = true;

				lID = pStyles->GetClassID(&pWord->m_oFont, &pWord->m_oBrush);

				lDecoration = pWord->m_oFont.GetTextDecorationStyle();
				switch (lDecoration)
				{
				case 0:
                    pWriter->WriteString(L"<span class=\"s", 14);
                    pWriter->AddInt(lID);
                    pWriter->WriteString(L"\">", 2);
					break;
				case 1:
                    pWriter->WriteString(L"<span class=\"s", 14);
                    pWriter->AddInt(lID);
                    pWriter->WriteString(L"\" style=\"text-decoration:underline;\">", 37);
					break;
				case 2:
                    pWriter->WriteString(L"<span class=\"s", 14);
                    pWriter->AddInt(lID);
                    pWriter->WriteString(L"\" style=\"text-decoration:line-through;\">", 40);
					break;
				case 3:
                    pWriter->WriteString(L"<span class=\"s", 14);
                    pWriter->AddInt(lID);
                    pWriter->WriteString(L"\" style=\"text-decoration:underline line-through;\">", 50);
					break;
				default:
                    pWriter->WriteString(L"<span class=\"s", 14);
                    pWriter->AddInt(lID);
                    pWriter->WriteString(L"\">", 2);
					break;
				}

				pWord->WriteToStringWriter(*pWriter);

                pos++;
			}

			if (bSpanClose)
                pWriter->WriteString(L"</span>", 7);

            pWriter->WriteString(L"</nobr></p>\n", 12);

			//CString strLine = _T("");
			//strLine.Format(g_string_lineFunc, m_lNextID++, nRight - nLeft);
			//m_pTextMeasurer->WriteString(strLine);
		}

		void DeleteEmptyWords()
		{
            std::list<CWord*>::iterator pos = m_arWords.begin();
            while (pos != m_arWords.end())
			{
                CWord* pWord = *pos;

				if (pWord->GetCountSpaceLast() == pWord->m_oText.GetCurSize())
				{
                    pos = m_arWords.erase(pos);
					RELEASEOBJECT(pWord);
                    continue;
				}

                pos++;
			}

            if (0 != m_arWords.size())
			{
				// удалим последние пробелы линии.
                CWord* pWord = GetLast();
				pWord->m_oText.RemoveLastSpaces();
			}
		}

		void Merge()
		{
            std::list<CWord*>::iterator pos = m_arWords.begin();
            if (pos == m_arWords.end())
				return;
			
            std::list<CWord*>::iterator posCur = pos;
            CWord* pWordPrev	= *pos;
            posCur++;

			double dRightPos	= pWordPrev->m_dX + pWordPrev->m_dWidth;			

            while (posCur != m_arWords.end())
			{
                std::list<CWord*>::iterator posCurOld = posCur;

                CWord* pWord = *posCur;
                posCur++;
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
                        pWordPrev->m_oText.AddCharSafe(' ');
					}
                    pWordPrev->m_oText.Write(pWord->m_oText);

                    posCur = m_arWords.erase(posCurOld);
					RELEASEOBJECT(pWord);
				}
				else
				{
					if (bNextWord)
                        pWordPrev->m_oText.AddCharSafe(' ');

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

        NSStringUtils::CStringBuilder* m_pWriter;

		//NSStrings::CStringWriter m_oTextMeasurer;

		CStyles*		m_pStyles;

		double			m_dDpiX;
		double			m_dDpiY;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

        Aggplus::CMatrix* m_pTransform;

		LONG			m_lIndexZ;

		bool			m_bIsNewLine;
		bool			m_bIsPDFTextStyle;

        NSStringUtils::CStringBuilder m_oTextItem;
		
	private:
		double			m_dEpsX;
		double			m_dEpsY;

	public:
        CText() : m_oFontManager(), m_oCurrentLine(), m_oTextItem()//, m_oTextMeasurer()
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

        void SetParams(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CFont* pFont, CStyles* pStyles, Aggplus::CMatrix* pTransform)
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

        inline bool GetIsNewLine()
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
            bool bIsFontChanged = !CFontManagerBase::IsEqual2(&m_oFontManager.m_oFont.m_oFont, m_pFont, dScaleFont);

            bool bIsTextDecorationChanged = ((m_oFontManager.m_oFont.m_oFont.Underline != m_pFont->Underline) ||
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
            m_oFontManager.m_strCurrentPickFont		= L"";

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
                m_oFontManager.MeasureStringPix(m_oTextItem.GetData(), x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
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
                    pTail->m_oText.AddCharSafe(' ');
				}

                pTail->m_oText.Write(m_oTextItem);
				pTail->m_dWidth = (x + width - pTail->m_dX);
				pTail->m_dWidthWithoutSpaces = pTail->m_dWidth;
			}
			else
			{
				CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
				m_oCurrentLine.AddWord(pWord);
			}
		}
        void CommandText2(const std::wstring& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset)
		{
			bool bIsNewLine = GetIsNewLine();

			double dScaleFont = ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);
            bool bIsFontChanged = !CFontManagerBase::IsEqual2(&m_oFontManager.m_oFont.m_oFont, m_pFont, dScaleFont);

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
                if (bsGid.empty())
				{
					m_oFontManager.SetStringGid(0);
                    m_oFontManager.MeasureStringPix(m_oTextItem.GetData(), x, y, _x, _y, _w, _h, CFontManager::MeasureTypePosition);
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
                        pWordLast->m_oText.Write(m_oTextItem);
						pWordLast->m_dWidth = x + width - pWordLast->m_dX;
					}
					return;
				}
				
				if (dDelta > dSpace)
                    pWordLast->m_oText.AddCharSafe(' ');

				if (bIsFontChanged || bIsBrushChanged)
				{
					CWord* pWord = new CWord(&m_oFontManager, m_pBrush, m_oTextItem, x, y, width, height, dYPos);
					m_oCurrentLine.AddWord(pWord);					
				}
				else
				{
                    pWordLast->m_oText.Write(m_oTextItem);
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
                    pWordLast->m_oText.Write(m_oTextItem);
					pWordLast->m_dWidth		= x + width - pWordLast->m_dX; // чтобы не накапливалась ошибка, не используем сумму длин

					if (5 < pWordLast->GetCountLastPoints())
						m_bIsNewLine = true;
				}

				if (!m_oTextItem.IsSpace())
					pWordLast->m_dWidthWithoutSpaces = pWordLast->m_dWidth;
			}
		}
		
	public:
        inline void CommandText(const std::wstring& bsText, const std::wstring& bsGid, double& x, double& y, double& width, double& height, double& baselineoffset, LONG& lIndexZ)
		{
            if (bsText.empty())
				return;

			m_lIndexZ = lIndexZ;
			m_oTextItem.SetText(bsText);

			if (0 == m_oTextItem.GetCurSize())
				return;

            if (!bsGid.empty())
			{
                // TODO: m_oTextItem.CorrectUnicode(m_oFontManager.m_mapUnicode);
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

#endif // _ASC_HTMLRENDERER_TEXT_H_

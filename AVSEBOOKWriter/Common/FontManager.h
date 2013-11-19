#pragma once
#include "Common.h"
#include "FontManagerBase.h"

namespace NSCommon
{
	using namespace NSFontManager;

	const double c_dDpiX		= 72.0;
	const double c_dDpiY		= 72.0;

	class CFontManager : public CFontManagerBase
	{
	public:
		NSStructures::CFont*			m_pFont;
		NSCommon::CMatrix*				m_pTransform;

		double							m_dSpaceWidthMM;

	public:
		CFontManager() : m_pFont(NULL), CFontManagerBase()
		{
			m_pTransform = NULL;
			m_dSpaceWidthMM = 0;
		}
		virtual ~CFontManager()
		{
		}

	public:
		virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true)
		{
			if (NULL == m_pManager)
				return;

			double dSize = m_pFont->Size;
			double dSizeFont = dSize * ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);

			m_pFont->Size = dSizeFont;

			if (m_pFont->IsEqual2(&m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

			if (_T("") == m_pFont->Path)
			{
				CFontManagerBase::LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle());
			}
			else
			{
				CFontManagerBase::LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, lFaceIndex);

				m_pFont->SetStyle(m_oFont.m_oProperties.m_lStyle);
				m_oFont.m_oFont.SetStyle(m_oFont.m_oProperties.m_lStyle);

				bIsPath = true;
			}

			CalculateSpace();
		}

		AVSINLINE void CalculateSpace()
		{
			LONG lGid = 0;
			m_pManager->GetStringGID(&lGid);
			m_pManager->SetStringGID(FALSE);
			
			m_pManager->LoadString(L" ", 0, 0);
			float _x = 0;
			float _y = 0;
			float _w = 0;
			float _h = 0;

			m_pManager->MeasureString2(&_x, &_y, &_w, &_h);

			m_dSpaceWidthMM = (double)_w * c_dPixToMM;
			if (0 >= m_dSpaceWidthMM)
			{
				m_dSpaceWidthMM = 1.0;
			}

			m_pManager->SetStringGID(lGid);
		}
	};

	class CFontDstGenerator
	{
	public:
		NSStructures::CFont*	m_pFonts;
		LONG					m_lCurrentIndex;
		LONG					m_lCountFonts;
		LONG					m_lSize;

	public:
		CFontDstGenerator()
		{
			m_lSize			= 50;
			m_pFonts		= NULL;
			m_lCurrentIndex	= -1;
			m_lCountFonts	= 0;

			Grow();
		}
		~CFontDstGenerator()
		{
			RELEASEARRAYOBJECTS(m_pFonts);
		}
	public:
		LONG AddFont(NSStructures::CFont* pFont, BOOL bIsFontChanged, CFontManagerBase* pBase)
		{
			if (-1 != m_lCurrentIndex && !bIsFontChanged)
				return m_lCurrentIndex;

			LONG lIndex = m_lCountFonts - 1;
			while (lIndex >= 0)
			{
				if (m_pFonts[lIndex].IsEqual4(pFont))
					break;
				--lIndex;
			}
			if (lIndex >= 0)
			{
				m_lCurrentIndex = lIndex;
				return m_lCurrentIndex;
			}

			if (m_lCountFonts == m_lSize)
			{
				// нужно перевыделить память
				Grow();
			}

			m_lCurrentIndex = m_lCountFonts;
			++m_lCountFonts;
			m_pFonts[m_lCurrentIndex] = *pFont;

			// теперь нужно найти к нему путь...
			m_pFonts[m_lCurrentIndex].Path = pBase->GetFontPath(&m_pFonts[m_lCurrentIndex]);
			return m_lCurrentIndex;
		}

		LONG AddFont2(NSStructures::CFont* pFont, BOOL bIsFontChanged, CFontManagerBase* pBase)
		{
			if (-1 != m_lCurrentIndex && !bIsFontChanged)
				return m_lCurrentIndex;

			CString strName = pFont->Name;
			LONG lStyle = pFont->GetStyle();

			pFont->Name = pBase->m_strCurrentPickFont;
			pFont->SetStyle(pBase->m_lCurrentPictFontStyle);

			LONG lIndex = m_lCountFonts - 1;
			while (lIndex >= 0)
			{
				if (m_pFonts[lIndex].IsEqual4(pFont))
					break;
				--lIndex;
			}
			if (lIndex >= 0)
			{
				m_lCurrentIndex = lIndex;

				pFont->Name = strName;
				pFont->SetStyle(lStyle);

				return m_lCurrentIndex;
			}

			if (m_lCountFonts == m_lSize)
			{
				// нужно перевыделить память
				Grow();
			}

			m_lCurrentIndex = m_lCountFonts;
			++m_lCountFonts;
			m_pFonts[m_lCurrentIndex] = *pFont;

			// теперь нужно найти к нему путь...
			m_pFonts[m_lCurrentIndex].Path = pBase->GetFontPath(&m_pFonts[m_lCurrentIndex]);

			pFont->Name = strName;
			pFont->SetStyle(lStyle);

			return m_lCurrentIndex;
		}

	public:
		void WriteFonts(CString strFolderDst, CString& strCSS, CString& strItems)
		{
			// копируем шрифты туда, куда нужно и отдаем css
			strCSS = _T("");
			strItems = _T("");
			for (LONG nFont = 0; nFont < m_lCountFonts; ++nFont)
			{
				CString strName = m_pFonts[nFont].Name;
				strName.Replace(_T(" "), _T("_avs_space_"));

				CString strCssMem = _T("@font-face {font-family:") + strName + _T(";font-weight:");
				if (m_pFonts[nFont].Bold)
					strCssMem += _T("bold;font-style:");
				else
					strCssMem += _T("normal;font-style:");
				
				if (m_pFonts[nFont].Italic)
					strCssMem += _T("italic;src:url(fonts/");
				else
					strCssMem += _T("normal;src:url(fonts/");

				int nStart1 = m_pFonts[nFont].Path.ReverseFind((TCHAR)'\\');
				int nStart2 = m_pFonts[nFont].Path.ReverseFind((TCHAR)'/');

				int nStart	= max(nStart1, nStart2);
				int nEnd	= m_pFonts[nFont].Path.GetLength();

				CString strDstName = m_pFonts[nFont].Path.Mid(nStart + 1, nEnd - nStart - 1);
				strDstName.Replace(_T(" "), _T("_avs_space_"));
				CString strFile = strFolderDst + _T("\\fonts\\") + strDstName;

				DWORD dwFileAttrib = ::GetFileAttributes(strFile);
				if (dwFileAttrib == INVALID_FILE_ATTRIBUTES)
				{
					// такого шрифта еще не было
					::CopyFile(m_pFonts[nFont].Path, strFile, TRUE);
				}

				nEnd = strDstName.Find(TCHAR('.'));
				CString strId = strDstName;
				if (-1 != nEnd)
					strId = strId.Mid(0, nEnd);

				strItems += (_T("<item id=\"") + strId + _T("\" href=\"fonts/") + strDstName + _T("\" media-type=\"application/x-font-ttf\"/>"));

				strCssMem += strDstName;
				strCssMem += _T(");}\n");

				strCSS += strCssMem;
			}
		}

	protected:
		void Grow()
		{
			if (NULL == m_pFonts)
			{
				m_pFonts = new NSStructures::CFont[m_lSize];
				return;
			}

			NSStructures::CFont* pNewBuffer = new NSStructures::CFont[m_lSize * 2];
			for (LONG i = 0; i < m_lSize; ++i)
			{
				pNewBuffer[i] = m_pFonts[i];
			}
			RELEASEARRAYOBJECTS(m_pFonts);
			m_pFonts = pNewBuffer;

			m_lSize *= 2;
		}
	};
}
#pragma once
#include "Common.h"
#include "FontManagerBase.h"

namespace NSDocxRenderer
{
	using namespace NSFontManager;

	const double c_dDpiX		= 72.0;
	const double c_dDpiY		= 72.0;

	enum TextAssociationType
	{
		TextAssociationTypeDefault	= 0,
		TextAssociationTypeLine		= 1,
		TextAssociationTypeNoFrames	= 2,
		TextAssociationTypeBlock	= 3
	};

	class CFontTableEntry
	{
	public:
		CString							m_strFamilyName;
		CString							m_strPANOSE;
		LONG							m_lStyle;
		CAtlArray<DWORD>				m_arSignature;
		bool							m_bIsFixedWidth;

	public:
		CFontTableEntry() : m_arSignature()
		{
			m_strFamilyName		= _T("");
			m_strPANOSE			= _T("");
			m_lStyle			= 0;
			m_bIsFixedWidth		= false;
		}
		~CFontTableEntry()
		{
		}
		CFontTableEntry(const CFontTableEntry& oSrc)
		{
			*this = oSrc;
		}
		CFontTableEntry& operator =(const CFontTableEntry& oSrc)
		{
			m_strFamilyName	= oSrc.m_strFamilyName;
			m_strPANOSE		= oSrc.m_strPANOSE;
			m_lStyle		= oSrc.m_lStyle;
			m_arSignature.Copy(oSrc.m_arSignature);
			m_bIsFixedWidth	= oSrc.m_bIsFixedWidth;

			return *this;
		}
	};

	class CFontTable
	{
	public:
		CAtlMap<CString, CFontTableEntry> m_mapTable;

	public:
		CFontTable() : m_mapTable()
		{
		}
	};

	class CFontManager : public CFontManagerBase
	{
	public:
		NSStructures::CFont*			m_pFont;
		NSDocxRenderer::CMatrix*		m_pTransform;
		double							m_dSpaceWidthMM;

	public:
		CFontTable						m_oFontTable;

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
		AVSINLINE void Init()
		{
			m_oFontTable.m_mapTable.RemoveAll();
		}
		AVSINLINE void AddFontToMap()
		{
			CAtlMap<CString, CFontTableEntry>::CPair* pPair = m_oFontTable.m_mapTable.Lookup(m_oFont.m_strFamilyName);
			if (NULL == pPair)
			{
				CFontTableEntry oEntry;
				oEntry.m_strFamilyName	= m_oFont.m_strFamilyName;
				oEntry.m_strPANOSE		= m_oFont.m_strPANOSE;
				oEntry.m_lStyle			= m_oFont.m_lStyle;
				oEntry.m_bIsFixedWidth	= m_oFont.m_bIsFixedWidth;
				oEntry.m_arSignature.Copy(m_oFont.m_arSignature);

				m_oFontTable.m_mapTable.SetAt(m_oFont.m_strFamilyName, oEntry);
			}
		}

	public:
		virtual void LoadFont(long lFaceIndex = 0, bool bNeedAddToMap = true)
		{
			if (NULL == m_pManager)
				return;

			double dSize = m_pFont->Size;
			double dSizeFont = dSize * ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);

			double dPix = m_pFont->CharSpace / c_dPixToMM;

			m_pFont->Size = dSizeFont;

			if (m_pFont->IsEqual2(&m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				m_pManager->SetCharSpacing(dPix);
				m_pManager->SetStringGID(m_oFont.m_oFont.StringGID);
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

			if (_T("") == m_pFont->Path)
			{
				CFontManagerBase::LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle(), c_dDpiX, c_dDpiY);
			}
			else
			{
				CFontManagerBase::LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, c_dDpiX, c_dDpiY, lFaceIndex);

				m_pFont->SetStyle(m_oFont.m_lStyle);
				m_oFont.m_oFont.SetStyle(m_oFont.m_lStyle);

				bIsPath = true;
			}

			long lGid = 0;
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

			LoadFontMetrics();
			LoadFontParams(bIsPath);

			if (bNeedAddToMap)
				AddFontToMap();
		}
		
		AVSINLINE void MeasureString(const CString& strText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
		{
			BSTR bsText = strText.AllocSysString();
			MeasureString(bsText, x, y, dBoxX, dBoxY, dBoxWidth, dBoxHeight, measureType);
			SysFreeString(bsText);
		}
		AVSINLINE void MeasureStringUNICODE(const CString& strText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
		{
			m_pManager->SetStringGID(FALSE);
			MeasureString(strText, x, y, dBoxX, dBoxY, dBoxWidth, dBoxHeight, measureType);
			m_pManager->SetStringGID(TRUE);
		}
		AVSINLINE void MeasureStringUNICODE(BSTR strText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
		{
			m_pManager->SetStringGID(FALSE);
			MeasureString(strText, x, y, dBoxX, dBoxY, dBoxWidth, dBoxHeight, measureType);
			m_pManager->SetStringGID(TRUE);
		}
		void MeasureString(BSTR bsText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
		{
			LoadFont();

			dBoxX		= 0;
			dBoxY		= 0;
			dBoxWidth	= 0;
			dBoxHeight	= 0;

			if (NULL == m_pManager)
				return;

			m_pManager->LoadString(bsText, (float)x, (float)y);

			float fx		= 0;
			float fy		= 0;
			float fwidth	= 0;
			float fheight	= 0;

			if (MeasureTypeGlyph == measureType)
			{
				m_pManager->MeasureString(&fx, &fy, &fwidth, &fheight);
			}
			else if (MeasureTypePosition == measureType)
			{
				m_pManager->MeasureString2(&fx, &fy, &fwidth, &fheight);
			}

			dBoxX		= (double)fx;
			dBoxY		= (double)fy;
			dBoxWidth	= (double)fwidth;
			dBoxHeight	= (double)fheight;

			// переводим в миллиметры
			dBoxX		*= c_dPixToMM;
			dBoxY		*= c_dPixToMM;
			dBoxWidth	*= c_dPixToMM;
			dBoxHeight	*= c_dPixToMM;
		}


		__forceinline double GetBaseLineOffset()
		{
			LoadFont();

			double d1 = 3 * (m_oFont.m_dLineSpacing - m_oFont.m_dDescent) - m_oFont.m_dAscent;
			d1 /= 2.0;

			d1 *= (m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);
			return d1;
		}

		__forceinline double GetFontHeight()
		{
			return c_dPtToMM * (m_oFont.m_dLineSpacing * m_oFont.m_oFont.Size ) / m_oFont.m_dEmHeight;
		}

		__forceinline void SetStringGid(const LONG& lGid)
		{
			if (NULL != m_pManager)
				m_pManager->SetStringGID(lGid);
		}

		__forceinline void GenerateFontName2(CString& strText)
		{
			bool bIsNeedAddToMap = CFontManagerBase::GenerateFontName(strText);

			if (bIsNeedAddToMap)
			{
				CAtlMap<CString, CFontTableEntry>::CPair* pPair = m_oFontTable.m_mapTable.Lookup(m_strCurrentPickFont);
				if (NULL == pPair)
				{
					CFontTableEntry oEntry;
					oEntry.m_strFamilyName	= m_strCurrentPickFont;
					oEntry.m_strPANOSE		= m_oFont.m_strPANOSE;
					oEntry.m_lStyle			= m_oFont.m_lStyle;
					oEntry.m_bIsFixedWidth	= m_oFont.m_bIsFixedWidth;
					oEntry.m_arSignature.Copy(m_oFont.m_arSignature);

					m_oFontTable.m_mapTable.SetAt(m_oFont.m_oFont.Path, oEntry);
				}
			}
		}
	};

	class CFontManagerLight
	{
	private:
		CString m_strFontName;
		LONG	m_lFontStyle;
		double	m_dSize;

		double	m_dSpaceWidth;

		AVSGraphics::IASCFontManager*	m_pManager;
	
	public:
		CFontManagerLight()
		{
			m_strFontName	= _T("");
			m_lFontStyle	= 0;
			m_dSize			= 0;
			m_dSpaceWidth	= 0;

			m_pManager = NULL;
			CoCreateInstance(AVSGraphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, AVSGraphics::IID_IASCFontManager, (void**)&m_pManager);

			m_pManager->Initialize(L"");
			m_pManager->SetDefaultFont(L"Arial");
		}
		~CFontManagerLight()
		{
			RELEASEINTERFACE(m_pManager);
		}

		AVSINLINE double GetSpaceWidth()
		{
			return m_dSpaceWidth;
		}

	public:
		AVSINLINE void LoadFont(CString& strFontName, LONG& lStyle, double& dSize, const BOOL& bIsGID)
		{
			if ((strFontName == m_strFontName) && (lStyle == m_lFontStyle) && (dSize == m_dSize))
			{
				m_pManager->SetStringGID(bIsGID);
				return;
			}

			m_strFontName	= strFontName;
			m_lFontStyle	= lStyle;
			m_dSize			= dSize;

			BSTR bsName = m_strFontName.AllocSysString();
			m_pManager->LoadFontByName(bsName, (float)m_dSize, m_lFontStyle, c_dDpiX, c_dDpiY);
			SysFreeString(bsName);

			CString strSpace = _T(" ");
			m_dSpaceWidth = MeasureStringWidth(strSpace);

			m_pManager->SetStringGID(bIsGID);
		}

		AVSINLINE double MeasureStringWidth(CString& sText)
		{
			BSTR bsText = sText.AllocSysString();
			m_pManager->LoadString(bsText, (float)0, (float)0);
			SysFreeString(bsText);

			float fx		= 0;
			float fy		= 0;
			float fwidth	= 0;
			float fheight	= 0;

			m_pManager->MeasureString2(&fx, &fy, &fwidth, &fheight);

			return fwidth * c_dPixToMM;
		}
	};

}
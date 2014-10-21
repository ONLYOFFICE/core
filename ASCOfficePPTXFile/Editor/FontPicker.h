#pragma once

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "BinReaderWriterDefines.h"
#ifdef _WIN32
    #include "FontCutter.h"
#endif

//#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

namespace NSFontCutter
{
	class CFontDstManager
	{
	public:		
		std::map<CString, CString> m_mapPicks;
		CApplicationFonts m_oApplicationFonts;
		CFontManager* m_pFontManager;

		CString m_strFontsDir;

		BOOL					m_bIsEmbeddedFonts;

#ifdef _WIN32
        CEmbeddedFontsManager	m_oEmbeddedFonts;
#endif
	public:
		CFontDstManager() : m_mapPicks()
		{
			m_strFontsDir = _T("");

			m_pFontManager = NULL;
			m_bIsEmbeddedFonts = FALSE;
		}
		~CFontDstManager()
		{
			RELEASEOBJECT(m_pFontManager);
		}

		void Init(const CString& strDir)
		{
			if(strDir.IsEmpty())
				m_oApplicationFonts.Initialize();
			else
				m_oApplicationFonts.InitializeFromFolder(string2std_string(strDir));
			m_pFontManager = m_oApplicationFonts.GenerateFontManager();
		}

		CString GetTypefacePickByName(const CString& strTypeface)
		{
			CString sFind = strTypeface;

			int nFindTh = sFind.Find(_T("+mj"));
			if (0 == nFindTh)
				return sFind;
			nFindTh = sFind.Find(_T("+mn"));
			if (0 == nFindTh)
				return sFind;

			if (_T("") == sFind)
			{
				sFind = _T("Arial");
			}

			std::map<CString, CString>::iterator pPair = m_mapPicks.find(sFind);
			if (pPair != m_mapPicks.end())
				return pPair->second;

			CString sInputSave = sFind;

			CFontSelectFormat oFontSelectFormat;
			oFontSelectFormat.wsName = new std::wstring;
			*oFontSelectFormat.wsName = std::wstring(sFind.GetString());
			//oFontSelectFormat.
			CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
			CString sRes = _T("Arial");
			if(NULL != pFontInfo)
			{
				sRes = CString(pFontInfo->m_wsFontName.c_str());

#ifdef _WIN32
                if (m_bIsEmbeddedFonts)
					m_oEmbeddedFonts.CheckFont(sRes, m_pFontManager);
#endif
				m_mapPicks.insert(std::pair<CString,CString>(sInputSave, sRes));
				
			}
			return sRes;
		}

		template<typename TTextFont>
		CString GetTypefacePick(TTextFont& textFont)
		{
			return GetTypefacePickByName(textFont.typeface);			
		}
	};
}

class COfficeFontPicker 	
{
private:
	NSFontCutter::CFontDstManager m_oPicker;

public:
	
	HRESULT Init(const CString& bsFontsDirectory)
	{
		m_oPicker.Init(bsFontsDirectory);
		return S_OK;
	}
	
	CFontManager* get_FontManager()
	{
		return m_oPicker.m_pFontManager;
	}

	HRESULT SetEmbeddedFontsDirectory(const CString& bsFontsDirectory)
	{
#ifdef _WIN32
        m_oPicker.m_oEmbeddedFonts.m_strEmbeddedFontsFolder = (CString)bsFontsDirectory;
#endif
		return S_OK;
	}
	HRESULT SetEmbeddedFontsParam(LONG lParam)
	{
		switch (lParam)
		{
		case 0:
			m_oPicker.m_bIsEmbeddedFonts = FALSE;
			break;
		case 1:
			m_oPicker.m_bIsEmbeddedFonts = TRUE;
			break;
		default:
			break;
		}
		return S_OK;
	}

	HRESULT CheckString(const CString& bsText)
	{
#ifdef _WIN32
        m_oPicker.m_oEmbeddedFonts.CheckString((CString)bsText);
#endif
		return S_OK;
	}
	HRESULT CheckFont(const CString& bsFontName)
	{
#ifdef _WIN32
        m_oPicker.m_oEmbeddedFonts.CheckFont((CString)bsFontName, m_oPicker.m_pFontManager);
#endif
		return S_OK;
	}

	HRESULT PickFont(LONG lParamType, const CString& bsParams, CString* pDstName)
	{
		if (NULL == pDstName)
			return S_FALSE;

		if (0 == lParamType)
		{
			CString strResult = m_oPicker.GetTypefacePickByName((CString)bsParams);
            *pDstName = strResult;//strResult.AllocSysString();
			return S_OK;
		}
		// not impl
		return S_OK;
	}

	//HRESULT GetBinaryData(LONG lType, SAFEARRAY** ppBinaryArray);
	HRESULT GetBinaryData(LONG lType, BYTE** ppyArray, size_t& szCount);

	NSFontCutter::CFontDstManager* GetNativePicker()
	{
		return &m_oPicker;
	}
#ifdef _WIN32
    NSFontCutter::CEmbeddedFontsManager* GetNativeCutter()
	{
		return &m_oPicker.m_oEmbeddedFonts;
	}
#endif
};

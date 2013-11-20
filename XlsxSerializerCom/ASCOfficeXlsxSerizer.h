#pragma once
#include "resource.h" 
#include "../Common/OfficeFileTemplate.h"
#include "Reader/BinaryWriter.h"
#include "Reader/FontProcessor.h"
#include "Writer/BinaryReader.h"
[
	object,
	uuid("87476A4D-6A42-44e9-A947-42B8E8613070"),
	dual,	helpstring("IAVSOfficeXlsxSerizer Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeXlsxSerizer : public IAVSOfficeFileTemplate
{
	[id(20)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(30)] HRESULT LoadChart([in] BSTR bsFilename, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(31)] HRESULT SaveChart([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsFilename, [in] BSTR bsContentTypePath, [out] BSTR* bsContentTypeElement);
	[id(32)] HRESULT SetDrawingConverter([in] IUnknown* pDocument);

	[id(120)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(130)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[
	coclass,
	default(IAVSOfficeXlsxSerizer),
	event_receiver(native),
	threading(apartment),
	vi_progid("ASCOfficeXlsxSerizer.AVSDocume"),
	progid("ASCOfficeXlsxSerizer.AVSDocu.1"),
	version(1.0),
	uuid("2A6514F2-0C01-4759-89CB-68C38669337D"),
	helpstring("ASCOfficeXlsxSerizer Class")
]
class ATL_NO_VTABLE CAVSOfficeXlsxSerizer : public IAVSOfficeXlsxSerizer
{
private:
	CString m_strEmbeddedFontsDirectory;
	PPTXFile::IAVSOfficeDrawingConverter* m_pExternalDrawingConverter;
public:
	CAVSOfficeXlsxSerizer()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_pExternalDrawingConverter = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE(m_pExternalDrawingConverter);
	}
public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter;
		CoCreateInstance(__uuidof(PPTXFile::CAVSOfficeDrawingConverter), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IAVSOfficeDrawingConverter), (void**)(&pOfficeDrawingConverter));

		CString sMediaDir = sDstPath;
		sMediaDir.Append(_T("\\xl\\media"));
		BSTR bstrMediaDir = sMediaDir.AllocSysString();
		pOfficeDrawingConverter->SetMediaDstPath(bstrMediaDir);
		SysFreeString(bstrMediaDir);

		//папка с бинарников
		TCHAR tFolder[256];
		TCHAR tDrive[256];
		_tsplitpath( sSrcFileName, tDrive, tFolder, NULL, NULL );
		CString sFolder = CString(tFolder);
		CString sDrive = CString(tDrive);
		CString sFileInDir = sDrive + sFolder;

		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = sFileInDir.AllocSysString();
		pOfficeDrawingConverter->SetAdditionalParam(L"SourceFileDir", var);
		RELEASESYSSTRING(var.bstrVal);

		//—оздаем temp тему, читаем ее и записываем в документ
		CString sTempTheme = CreateTheme();
		BinXlsxRW::BinaryFileReader oBinaryFileReader;
		oBinaryFileReader.ReadFile(sSrcFileName, sDstPath, sTempTheme, pOfficeDrawingConverter);
		::DeleteFile(sTempTheme);
		RELEASEINTERFACE(pOfficeDrawingConverter);
		return S_OK;
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		CString sFontDir = m_oFontProcessor.getFontDir();

		PPTXFile::IOfficeFontPicker* pFontPicker = NULL;
		CoCreateInstance(__uuidof(PPTXFile::COfficeFontPicker), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IOfficeFontPicker), (void**)(&pFontPicker));
		BSTR bstrFontDir1 = sFontDir.AllocSysString();
		pFontPicker->Init(bstrFontDir1);
		SysFreeString(bstrFontDir1);
		NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager = NULL;
		if(false == m_strEmbeddedFontsDirectory.IsEmpty())
		{
			CreateDirectoryW(m_strEmbeddedFontsDirectory, NULL);

			BSTR bstrEmbeddedFontsDirectory = m_strEmbeddedFontsDirectory.AllocSysString();
			pFontPicker->SetEmbeddedFontsDirectory(bstrEmbeddedFontsDirectory);
			SysFreeString(bstrEmbeddedFontsDirectory);

			VARIANT vt;
			pFontPicker->GetAdditionalParam(_T("NativeCutter"), &vt);
			pEmbeddedFontsManager = (NSFontCutter::CEmbeddedFontsManager*)vt.pvRecord;

			AVSGraphics::IAVSFontManager* pFontManager = m_oFontProcessor.getFontManager();
			//добавим мега шрифт
			pEmbeddedFontsManager->CheckFont(_T("Wingdings 3"), pFontManager);
			pEmbeddedFontsManager->CheckFont(_T("Arial"), pFontManager);
			//pEmbeddedFontsManager добавл€ютс€ все цифры
			//дл€ заголовков
			pEmbeddedFontsManager->CheckFont(_T("Calibri"), pFontManager);
			pEmbeddedFontsManager->CheckString(CString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));

			//дополнение дл€ ошибок "#NULL!", "#DIV/0!"...
			pEmbeddedFontsManager->CheckString(CString(_T("#!/?")));
			//дополнение дл€ num форматов по умолчанию с id от 0 до 49
			pEmbeddedFontsManager->CheckString(CString(_T(".%E+-():")));
		}

		PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter;
		CoCreateInstance(__uuidof(PPTXFile::CAVSOfficeDrawingConverter), NULL, CLSCTX_ALL, __uuidof(PPTXFile::IAVSOfficeDrawingConverter), (void**)(&pOfficeDrawingConverter));

		BSTR bstrFontDir = sFontDir.AllocSysString();
		pOfficeDrawingConverter->SetFontDir(bstrFontDir);
		SysFreeString(bstrFontDir);
		VARIANT vt;
		vt.vt = VT_UNKNOWN;
		vt.punkVal = pFontPicker;
		pOfficeDrawingConverter->SetAdditionalParam(_T("FontPicker"), vt);

		BinXlsxRW::BinaryFileWriter oBinaryFileWriter(m_oFontProcessor);
		oBinaryFileWriter.Open(CString(sSrcPath), CString(sDstFileName), pEmbeddedFontsManager, pOfficeDrawingConverter);

		RELEASEINTERFACE(pFontPicker);
		RELEASEINTERFACE(pOfficeDrawingConverter);
		return S_OK;
	}
	STDMETHOD(LoadChart)(BSTR bsFilename, SAFEARRAY** ppBinary)
	{
		HRESULT hRes = S_FALSE;
		*ppBinary = NULL;
		CString sChartPath(bsFilename);
		OOX::Spreadsheet::CChartSpace oChart(sChartPath);
		if(oChart.m_oChart.IsInit() && oChart.m_oChart->isValid() && NULL != m_pExternalDrawingConverter)
		{
			long nGrowSize = 1 * 1024 * 1024;//1мб
			Streams::CBuffer oBuffer;
			oBuffer.Create(nGrowSize, nGrowSize);
			Streams::CBufferedStream oBufferedStream;
			oBufferedStream.SetBuffer(&oBuffer);
			BinXlsxRW::BinaryCommonWriter oBcw(oBufferedStream);

			BSTR bstrChartPath = sChartPath.AllocSysString();
			m_pExternalDrawingConverter->SetRelsPath(bstrChartPath);
			SysFreeString(bstrChartPath);

			BinXlsxRW::BinaryChartWriter oBinaryChartWriter(oBufferedStream, m_pExternalDrawingConverter);	
			oBinaryChartWriter.Write(oChart);

			ULONG lBinarySize = oBufferedStream.GetPosition();
			SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, lBinarySize);
			
			BYTE* pDataD = (BYTE*)pArray->pvData;
			BYTE* pDataS = oBufferedStream.GetBuffer();
			memcpy(pDataD, pDataS, lBinarySize);

			*ppBinary = pArray;
			hRes = S_OK;
		}
		return hRes;
	}
	STDMETHOD(SaveChart)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsFilename, BSTR bsContentTypePath, BSTR* bsContentTypeElement)
	{
		HRESULT hRes = S_FALSE;
		*bsContentTypeElement = NULL;
		if(NULL != m_pExternalDrawingConverter)
		{
			Streams::CBuffer oBuffer;
			Streams::CBufferedStream oBufferedStream;
			oBufferedStream.SetBuffer(&oBuffer);
			oBufferedStream.Create((BYTE*)pBinaryObj->pvData + lStart, lLength);

			m_pExternalDrawingConverter->SetDstContentRels();

			OOX::Spreadsheet::CChartSpace oChartSpace;
			oChartSpace.m_oChart.Init();
			BinXlsxRW::BinaryChartReader oBinaryChartReader(oBufferedStream, pBinaryObj, m_pExternalDrawingConverter);
			oBinaryChartReader.ReadChartOut(lLength, &oChartSpace);

			if(oChartSpace.isValid())
			{
				TCHAR tDrive[256];
				TCHAR tFolder[256];
				TCHAR tFilename[256];
				TCHAR tExt[256];
				_tsplitpath( bsFilename, tDrive, tFolder, tFilename, tExt );
				CString sDrive(tDrive);
				CString sFolder(tFolder);
				CString sFilename(tFilename);
				CString sExt(tExt);

				CString sRelsDir = sDrive + sFolder;
				sRelsDir.Append(_T("_rels"));
				DWORD dwFileAttr = ::GetFileAttributes( sRelsDir );
				if( dwFileAttr == INVALID_FILE_ATTRIBUTES )
					OOX::CSystemUtility::CreateDirectories(sRelsDir);

				oChartSpace.write2(CString(bsFilename));

				CString sRelsPath;
				sRelsPath.Format(_T("%s\\%s.rels"), sRelsDir, sFilename + sExt);

				BSTR bstrRelsPath = sRelsPath.AllocSysString();
				m_pExternalDrawingConverter->SaveDstContentRels(bstrRelsPath);
				SysFreeString(bstrRelsPath);

				CString sContentTypePath(bsContentTypePath);
				sContentTypePath.Append(sFilename);
				sContentTypePath.Append(sExt);

				CString sContentTypeXml;
				sContentTypeXml.Format(_T("<Override PartName=\"%s\" ContentType=\"application/vnd.openxmlformats-officedocument.drawingml.chart+xml\"/>"), sContentTypePath);
				*bsContentTypeElement = sContentTypeXml.AllocSysString();
				hRes = S_OK;
			}
		}
		return hRes;
	}
	STDMETHOD(SetDrawingConverter)(IUnknown* pDocument)
	{
		RELEASEINTERFACE(m_pExternalDrawingConverter);
		pDocument->QueryInterface(__uuidof(PPTXFile::IAVSOfficeDrawingConverter), (void**)&m_pExternalDrawingConverter);
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsMediaDir)
	{
		m_oFontProcessor.setFontDir(bsMediaDir);
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_strEmbeddedFontsDirectory = ParamValue.bstrVal;
			return S_OK;
		}		
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
private:
	CString CreateTheme()
	{
		HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

		CString sThemePath;
		char sTempPath[MAX_PATH], sTempFile[MAX_PATH];
		if ( 0 == GetTempPathA( MAX_PATH, sTempPath ) )
			return sThemePath;

		if ( 0 == GetTempFileNameA( sTempPath, NULL, 0, sTempFile ) )
			return sThemePath;

		sThemePath = sTempFile;
		LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_XLSX_THEME), _T("XLSXSER"), sThemePath);
		return sThemePath;
	}
	void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
	{
		HRSRC hrRes = FindResource(hInst, sResName, sResType);
		if (!hrRes)
			return;

		HGLOBAL hGlobal = LoadResource(hInst, hrRes);
		DWORD sz = SizeofResource(hInst, hrRes);
		void* ptrRes = LockResource(hGlobal);

		CFile oFile;
		oFile.CreateFile(strDstFile);
		oFile.WriteFile(ptrRes, sz);

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}	
private:
	BinXlsxRW::FontProcessor m_oFontProcessor;
};
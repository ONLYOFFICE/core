// CASCPresentationEditor.h : Declaration of the CASCOfficeViewer
#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "ASCPresentationEditorDef.h"


#include "PPTXWriter\Converter.h"
#include "PPTStorage.h"
using namespace NSPresentationEditor;

// IAVSPresentationEditor
[object, uuid("7EE5C5A5-B0EF-440e-895D-949C1439A0C2"), dual, pointer_default(unique)]
__interface IAVSPresentationEditor: IDispatch
{
	[id(101), propget]	HRESULT TempDirectory([out, retval] BSTR* bsTempDir);
	[id(101), propput]	HRESULT TempDirectory([in] BSTR bsTempDir);

	[id(102), propget]	HRESULT EditorXml([out, retval] BSTR* bsXml);

	[id(103), propget]	HRESULT Status([out, retval] LONG* lStatus);

	[id(200)]			HRESULT OpenFile([in] BSTR bsFileName);
	[id(201)]			HRESULT CloseFile([in] VARIANT_BOOL vbAttack);
	[id(202)]			HRESULT OpenXmlFile([in] BSTR bsFileName);

	[id(301), propget]	HRESULT TempFileName([out, retval] BSTR* bsFileName);
	[id(301), propput]	HRESULT TempFileName([in] BSTR bsFileName);

	[id(400)]			HRESULT Save([in] LONG lFileType, [in] BSTR bsFilePath);

	[id(500)]			HRESULT ConvertToPPTY([in] BSTR bsFileName, [in] BSTR bsDstFileName);
	[id(500 + 2)]		HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(500 + 3)]		HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(500 + 4)]		HRESULT SetUseSystemFonts([in] VARIANT_BOOL useSystemFonts);

	[id(2000)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[dispinterface, uuid("34EC371F-3F73-4a40-B8A6-FD21B9C3E789")]
__interface _IAVSPresentationEditorEvents
{
};

// CAVSPresentationEditor
[coclass, uuid("29759D68-916E-4317-BC8B-35793F7448B3"), event_source(com), threading(apartment), vi_progid("AVSPptEditor.PptEditor"), progid("AVSPptEditor.PptEditor.1"), version(1.0)]
class ATL_NO_VTABLE CAVSPresentationEditor:	
	public IAVSPresentationEditor 
{
protected:
	ASCGraphics::IASCWinFonts*		m_pWinFonts;

	LONG m_lFileType;
	PPTFile::IAVSOfficePPTFile*		m_pPPTFile;
	PPTXFile::IAVSOfficePPTXFile*	m_pPPTXFile;

	SPPTStatusStorage   m_oSS;
	
	CStatusStorage*		m_pStatusStorage;
	CInfoStorage*		m_pInfoStorage;

	CString				m_strTempDirectory;
	CString				m_strTempDirectoryFile;
	CString				m_strTempFileName;

	CString				m_strEditorXml;
	CString				m_strFileName;

	LONG				m_lStatus;
	HANDLE				m_hSynchMutex;

	CDocument			m_oDocument;

	// ppty
	CString				m_strFontDirectory;
	CString				m_strMediaDirectory;
	BOOL				m_bIsUseSystemFonts;
	CString				m_strEmbeddedFontsDirectory;

#ifdef USE_ODF_FILE
	ODFFile::IOfficeOdfFile* m_pODFFile;
#endif

public:
	__event __interface _IAVSPresentationEditorEvents;

	CAVSPresentationEditor()
	{
		m_hSynchMutex = CreateMutex(NULL, FALSE, NULL);

		m_pWinFonts = NULL;
		CoCreateInstance(ASCGraphics::CLSID_CASCWinFonts, NULL, CLSCTX_ALL, ASCGraphics::IID_IASCWinFonts, (void**)&m_pWinFonts);
		
		m_pPPTFile	= NULL;
		m_pPPTXFile = NULL;
		m_lFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;

		m_lStatus = NULLMODE;

		m_pStatusStorage	= NULL;
		m_pInfoStorage		= NULL;	

		m_strEditorXml		= _T("");
		m_strTempDirectory	= _T("");
		m_strTempDirectoryFile = _T("");
		m_strFileName		= _T("");

		m_strFontDirectory = _T("");
		m_strMediaDirectory = _T("");
		m_bIsUseSystemFonts = FALSE;
		m_strEmbeddedFontsDirectory = _T("");

		m_pODFFile = NULL;
	}

	~CAVSPresentationEditor()
	{
		CloseFile(VARIANT_TRUE);
		RELEASEINTERFACE(m_pWinFonts);
		RELEASEINTERFACE(m_pODFFile);
	}

public:

	STDMETHOD(get_Status)(LONG* pVal)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		*pVal = m_lStatus;
		return S_OK;
	}
	STDMETHOD(get_EditorXml)(BSTR* pbstrVal)
	{
		if (NULL == pbstrVal)
			return FALSE;

		*pbstrVal = m_strEditorXml.AllocSysString();
		return S_OK;
	}

	STDMETHOD(OpenFile)(BSTR fileName)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
	
		CloseFile(VARIANT_TRUE);
		m_lFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		
		m_strFileName = (CString)fileName;
		
		RELEASEOBJECT(m_pStatusStorage);
		m_pStatusStorage = new CStatusStorage(m_strFileName);

		do
		{
			bool bGetMaster = false;
			m_pStatusStorage->GetStatus( &bGetMaster, &m_oSS );
			if ( STIF_BROKEN == m_oSS.m_sStatus || STIF_ERROR == m_oSS.m_sStatus )
			{
				InitTempDirectory();
				
				HRESULT hRes = S_FALSE;
				BSTR bstrTemp = m_strTempDirectoryFile.AllocSysString();
				
				if (SUCCEEDED(CoCreateInstance(PPTFile::CLSID_CAVSOfficePPTFile, NULL, CLSCTX_ALL, PPTFile::IID_IAVSOfficePPTFile, (void**)&m_pPPTFile)))
				{
					m_pPPTFile->put_TempDirectory(bstrTemp);
					
					hRes = m_pPPTFile->LoadFromFile(fileName, bstrTemp, L"");
					if (S_OK == hRes)
					{
						m_lFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
						m_lStatus = READMODE;

						VARIANT var;
						var.vt		= VT_BSTR;
						var.bstrVal = NULL;

						m_pPPTFile->GetAdditionalParam(L"EditorXml", &var);

						m_strEditorXml = (CString)var.bstrVal;

						SysFreeString(var.bstrVal);
					}

					RELEASEINTERFACE(m_pPPTFile);
				}
				if (S_OK != hRes)
				{
					if (SUCCEEDED(CoCreateInstance(PPTXFile::CLSID_CAVSOfficePPTXFile, NULL, CLSCTX_ALL, PPTXFile::IID_IAVSOfficePPTXFile, (void**)&m_pPPTXFile)))
					{
						m_pPPTXFile->put_TempDirectory(bstrTemp);
					
						hRes = m_pPPTXFile->LoadFromFile(fileName, bstrTemp, L"");
						if (S_OK == hRes)
						{
							m_lFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
							m_lStatus = READMODE;
							
							VARIANT var;
							var.vt		= VT_BSTR;
							var.bstrVal = NULL;

							m_pPPTXFile->GetAdditionalParam(L"EditorXml", &var);

							m_strEditorXml = (CString)var.bstrVal;

							SysFreeString(var.bstrVal);
						}

						RELEASEINTERFACE(m_pPPTXFile);
					}
				}
				if (S_OK != hRes)
				{
				}

				SysFreeString(bstrTemp);

				if (S_OK == hRes)
				{
					m_oSS.m_sStatus				= STIF_AVAILABLE;
					m_oSS.m_nProgress			= 100;

					SPPTInfoStorage oInfo;
					oInfo.m_strTempDirectory	= m_strTempDirectoryFile;
					oInfo.m_strXml				= m_strEditorXml;

					m_oDocument.LoadFromXML(m_strEditorXml);

					LONG64 lLen = oInfo.GetLength();

					oInfo.m_lCount = 1;

					m_oSS.m_lLength = lLen;

					RELEASEOBJECT(m_pInfoStorage);
					m_pInfoStorage = new CInfoStorage(m_strFileName, lLen);

					m_pInfoStorage->WriteStruct(&oInfo);
					m_pStatusStorage->WriteStruct( &m_oSS );

					/*
					для тестового примера
					XmlUtils::CXmlNode oNode;
					oNode.FromXmlString(m_strEditorXml);
					m_oDocument.ReadFromXml(oNode);
					m_oDocument.CalculateEditor(m_oDocument.m_oInfo, true);
					*/
				}
				else
				{
					CloseFile(VARIANT_TRUE);
				}
				return hRes;
			}
			else if ( STIF_CREATING == m_oSS.m_sStatus )
			{
				Sleep ( 100 );
			}
			else if ( STIF_AVAILABLE == m_oSS.m_sStatus )
			{
				m_lFileType		= AVS_OFFICESTUDIO_FILE_PRESENTATION;
				m_lStatus		= READMODE;
				
				RELEASEOBJECT(m_pInfoStorage);
				m_pInfoStorage = new CInfoStorage(m_strFileName, m_oSS.m_lLength);

				SPPTInfoStorage oInfo;
				m_pInfoStorage->ReadStruct(&oInfo);

				oInfo.m_lCount++;
				m_pInfoStorage->WriteCount(&oInfo);

				m_strTempDirectoryFile	= oInfo.m_strTempDirectory;
				m_strEditorXml			= oInfo.m_strXml;				
			
				return S_OK;
			}

		}
		while ( STIF_CREATING == m_oSS.m_sStatus );
		return S_FALSE;
	}
	STDMETHOD(CloseFile)(VARIANT_BOOL vbAttack)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);

		if (_T("") != m_strFileName && NULL != m_pInfoStorage)
		{	
			SPPTInfoStorage oInfo;
			m_pInfoStorage->ReadStruct(&oInfo);

			oInfo.m_lCount = max(0, oInfo.m_lCount - 1);
			m_pInfoStorage->WriteCount(&oInfo);
			
			if (0 == oInfo.m_lCount && (VARIANT_TRUE == vbAttack))
			{
				DeleteTempDirectory();
			}
			if (VARIANT_FALSE == vbAttack)
			{
				CDirectory::SaveToFile(m_strTempDirectoryFile + _T("\\Editor.xml"), m_strEditorXml);
			}
		}
		else if (_T("") != m_strFileName)
		{
			if (VARIANT_TRUE == vbAttack)
			{
				DeleteTempDirectory();
			}
		}

		m_strFileName	= _T("");
		m_strEditorXml	= _T("");

		m_pODFFile = NULL;

		RELEASEOBJECT(m_pInfoStorage);
		RELEASEOBJECT(m_pStatusStorage);

		RELEASEINTERFACE(m_pPPTFile);
		RELEASEINTERFACE(m_pPPTXFile);

		m_lFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		m_lStatus = NULLMODE;
		return S_OK;
	}

	STDMETHOD(OpenXmlFile)(BSTR fileName)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		CloseFile(VARIANT_TRUE);

		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlFile2((CString)fileName))
		{
			m_oDocument.ReadFromXml(oNode);
			m_oDocument.CalculateEditor(m_oDocument.m_oInfo, true);
			return S_OK;
		}

		return S_FALSE;
	}

	STDMETHOD(get_DrawingXml)(BSTR* pbstrXml)
	{
		if (NULL == pbstrXml)
			return S_FALSE;

		*pbstrXml = m_strEditorXml.AllocSysString();
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)(BSTR* pbstrDir)
	{
		if (NULL == pbstrDir)
			return S_FALSE;

		*pbstrDir = m_strTempDirectory.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_TempDirectory)(BSTR bstrDir)
	{
		m_strTempDirectory = (CStringW)bstrDir;
		return S_OK;
	}
	STDMETHOD(put_TempFileName)(BSTR bstrFile)
	{
		m_strTempFileName = (CString)bstrFile;
		return S_OK;
	}
	STDMETHOD(get_TempFileName)(BSTR* bstrFile)
	{
		if (NULL != bstrFile)
			*bstrFile = m_strTempFileName.AllocSysString();
		
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
	STDMETHOD(Save)(LONG lFileType, BSTR bsFilePath)
	{
		switch (lFileType)
		{
		case 0:
			{
				// xml
				CString strXml = m_oDocument.ToXmlEditor2();
				CFile oFile;
				oFile.CreateFile((CString)bsFilePath);
				oFile.WriteStringUTF8(strXml);
				oFile.CloseFile();
				break;
			}
		case 1:
			{
#ifdef USE_ODF_FILE
				if (NULL != m_pODFFile)
				{
					// здесь только архивация и удаление temp
					COfficeUtils* pOfficeUtils = new COfficeUtils(NULL);

					// архивируем
					BSTR bstrSrcPath = m_strTempDirectoryFile.AllocSysString();

					HRESULT hr = pOfficeUtils->CompressFileOrDirectory( bstrSrcPath, bsFilePath, -1 );
					SysFreeString(bstrSrcPath);
					RELEASEOBJECT(pOfficeUtils);
					return S_OK;
				}
#endif

				// пптх
				CPPTXWriter oWriter;

				oWriter.m_strTempDirectory		= m_strTempDirectory;
				oWriter.m_strDstFileName		= (CString)bsFilePath;

				oWriter.CreateFile(&m_oDocument);
				oWriter.CloseFile();
				break;
			}	
		default:
			break;
		}
		
		return S_OK;
	}

	STDMETHOD(ConvertToPPTY)(BSTR bsSrc, BSTR bsDst)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
	
		CloseFile(VARIANT_TRUE);
		m_lFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		
		m_strFileName = (CString)bsSrc;
		
		int len_temp = m_strTempDirectory.GetLength();
		if (len_temp != 0)
		{
			CString sLast = m_strTempDirectory.Mid(len_temp - 1);
			if (sLast != _T("\\"))
				m_strTempDirectory += _T("\\");
		}

		InitTempDirectory();
		
		HRESULT hRes = S_FALSE;
		BSTR bstrTemp = m_strTempDirectoryFile.AllocSysString();		
		
		if (SUCCEEDED(CoCreateInstance(PPTFile::CLSID_CAVSOfficePPTFile, NULL, CLSCTX_ALL, PPTFile::IID_IAVSOfficePPTFile, (void**)&m_pPPTFile)))
		{
			m_pPPTFile->put_TempDirectory(bstrTemp);
			
			hRes = m_pPPTFile->LoadFromFile(bsSrc, bstrTemp, L"");
			if (S_OK == hRes)
			{
				m_lFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
				m_lStatus = READMODE;

				VARIANT var;
				var.vt		= VT_BSTR;
				var.bstrVal = NULL;

				m_pPPTFile->GetAdditionalParam(L"EditorXml", &var);

				m_strEditorXml = (CString)var.bstrVal;

				SysFreeString(var.bstrVal);

				m_oDocument.LoadFromXML(m_strEditorXml);
				CPPTXWriter oWriter;

				CString sFolderT = m_strTempDirectory + _T("WritePPTX\\");

				oWriter.m_strTempDirectory		= sFolderT;
				oWriter.m_strDstFileName		= (CString)bsDst;

				oWriter.CreateFile(&m_oDocument);

				PPTXFile::IAVSOfficePPTXFile2* pPPTX2 = NULL;
				CoCreateInstance(PPTXFile::CLSID_CAVSOfficePPTXFile, NULL, CLSCTX_ALL, PPTXFile::IID_IAVSOfficePPTXFile2, (void**)&pPPTX2);

				BSTR bs1 = sFolderT.AllocSysString();

				BSTR bsMediaDir = m_strMediaDirectory.AllocSysString();
				BSTR bsFontDir = m_strFontDirectory.AllocSysString();
				VARIANT_BOOL vbSystem = (m_bIsUseSystemFonts == TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

				pPPTX2->SetMediaDir(bsMediaDir);
				pPPTX2->SetFontDir(bsFontDir);
				pPPTX2->SetUseSystemFonts(vbSystem);

				if (_T("") != m_strEmbeddedFontsDirectory)
				{	
					PPTXFile::IAVSOfficePPTXFile* pPPTX1 = NULL;
					pPPTX2->QueryInterface(PPTXFile::IID_IAVSOfficePPTXFile, (void**)&pPPTX1);

					if (NULL != pPPTX1)
					{
						VARIANT var;
						var.vt = VT_BSTR;
						var.bstrVal = m_strEmbeddedFontsDirectory.AllocSysString();

						pPPTX1->SetAdditionalParam(L"EmbeddedFontsDirectory", var);

						SysFreeString(var.bstrVal);
						RELEASEINTERFACE(pPPTX1);
					}
				}

				SysFreeString(bsMediaDir);
				SysFreeString(bsFontDir);
				
				pPPTX2->OpenDirectoryToPPTY(bs1, bsDst);
				SysFreeString(bs1);

				RELEASEINTERFACE(pPPTX2);

				oWriter.CloseFile2();
			}

			RELEASEINTERFACE(m_pPPTFile);
		}
		if (S_OK != hRes)
		{
			if (SUCCEEDED(CoCreateInstance(PPTXFile::CLSID_CAVSOfficePPTXFile, NULL, CLSCTX_ALL, PPTXFile::IID_IAVSOfficePPTXFile, (void**)&m_pPPTXFile)))
			{
				m_pPPTXFile->put_TempDirectory(bstrTemp);
			
				PPTXFile::IAVSOfficePPTXFile2* pPPTX2 = NULL;
				m_pPPTXFile->QueryInterface(PPTXFile::IID_IAVSOfficePPTXFile2, (void**)&pPPTX2);
				
				BSTR bsMediaDir = m_strMediaDirectory.AllocSysString();
				BSTR bsFontDir = m_strFontDirectory.AllocSysString();
				VARIANT_BOOL vbSystem = (m_bIsUseSystemFonts == TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

				pPPTX2->SetMediaDir(bsMediaDir);
				pPPTX2->SetFontDir(bsFontDir);
				pPPTX2->SetUseSystemFonts(vbSystem);

				SysFreeString(bsMediaDir);
				SysFreeString(bsFontDir);

				if (_T("") != m_strEmbeddedFontsDirectory)
				{	
					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = m_strEmbeddedFontsDirectory.AllocSysString();

					m_pPPTXFile->SetAdditionalParam(L"EmbeddedFontsDirectory", var);

					SysFreeString(var.bstrVal);
				}

				hRes = pPPTX2->OpenFileToPPTY(bsSrc, bsDst);

				RELEASEINTERFACE(pPPTX2);
				if (S_OK == hRes)
				{
					m_lFileType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
					m_lStatus = READMODE;
				}

				RELEASEINTERFACE(m_pPPTXFile);
			}
		}
		if (S_OK != hRes)
		{
			ODFFile::IOfficeOdfFile* pODPFile = NULL;
			CoCreateInstance(ODFFile::CLSID_COfficeOdfFile, NULL, CLSCTX_ALL, ODFFile::IID_IOfficeOdfFile, (void**)&pODPFile);

			CString strOptions = _T("<Options><onlyPresentation></onlyPresentation></Options>");
			BSTR bsOptions = strOptions.AllocSysString();

			CString strTemp1 = m_strTempDirectoryFile + "\\";
			CreateDirectory(strTemp1, NULL);
			BSTR bsTemp1 = strTemp1.AllocSysString();
			HRESULT hRes = pODPFile->LoadFromFile(bsSrc, bsTemp1, bsOptions);
			SysFreeString(bsOptions);

			RELEASEINTERFACE(pODPFile);

			PPTXFile::IAVSOfficePPTXFile2* pPPTX2 = NULL;
			CoCreateInstance(PPTXFile::CLSID_CAVSOfficePPTXFile, NULL, CLSCTX_ALL, PPTXFile::IID_IAVSOfficePPTXFile2, (void**)&pPPTX2);

			BSTR bsMediaDir = m_strMediaDirectory.AllocSysString();
			BSTR bsFontDir = m_strFontDirectory.AllocSysString();
			VARIANT_BOOL vbSystem = (m_bIsUseSystemFonts == TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

			pPPTX2->SetMediaDir(bsMediaDir);
			pPPTX2->SetFontDir(bsFontDir);
			pPPTX2->SetUseSystemFonts(vbSystem);

			SysFreeString(bsMediaDir);
			SysFreeString(bsFontDir);

			if (_T("") != m_strEmbeddedFontsDirectory)
			{	
				PPTXFile::IAVSOfficePPTXFile* pPPTX1 = NULL;
				pPPTX2->QueryInterface(PPTXFile::IID_IAVSOfficePPTXFile, (void**)&pPPTX1);

				if (NULL != pPPTX1)
				{
					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = m_strEmbeddedFontsDirectory.AllocSysString();

					pPPTX1->SetAdditionalParam(L"EmbeddedFontsDirectory", var);

					SysFreeString(var.bstrVal);
					RELEASEINTERFACE(pPPTX1);
				}
			}

			pPPTX2->OpenDirectoryToPPTY(bsTemp1, bsDst);
			SysFreeString(bsTemp1);
			RELEASEINTERFACE(pPPTX2);
		}

		SysFreeString(bstrTemp);

		CloseFile(VARIANT_TRUE);
		return hRes;
	}

	STDMETHOD(SetMediaDir)(BSTR bsMediaDir) 
	{
		m_strMediaDirectory = bsMediaDir;
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		m_strFontDirectory = bsFontDir;
		return S_OK;
	}
	STDMETHOD(SetUseSystemFonts)(VARIANT_BOOL useSystemFonts) 
	{
		m_bIsUseSystemFonts = (VARIANT_TRUE == useSystemFonts);
		return S_OK;
	}

protected:
	void InitTempDirectory()
	{
		if (_T("") == m_strTempDirectory)
		{
			CStringW sTempPath;
			GetTempPathW(4096, sTempPath.GetBuffer(4096));
			sTempPath.ReleaseBuffer();

			m_strTempDirectory = sTempPath;
		}

		CreateDirectoryW(m_strTempDirectory, NULL);

		if (_T("") != m_strTempFileName)
		{
			m_strTempDirectoryFile = m_strTempDirectory + m_strTempFileName;

			CreateDirectoryW(m_strTempDirectoryFile, NULL);
			return;
		}
		
		CStringW strSrcFileName = _T("AVSPresentationExtraData");

		CStringW sFileMask = m_strTempDirectory + L"*";
		
		CStringW strMaskTemp = (L"Temp_" + strSrcFileName);
		int nResultNumber = -1;	

		WIN32_FIND_DATAW winFileData;
		HANDLE hFile;

		hFile = FindFirstFileW(sFileMask, &winFileData);
		if (hFile != INVALID_HANDLE_VALUE)
		{ 
			CStringW strFilePath = sFileMask + (CStringW)winFileData.cFileName;
			DWORD dwAttr = GetFileAttributesW(strFilePath);
			if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
			{
				int nNum = GetTempFileNum(strMaskTemp, winFileData.cFileName);
				nResultNumber = max(nNum, nResultNumber);
			}

			while (FALSE != FindNextFileW(hFile, &winFileData))
			{
				CStringW strFilePathNext = sFileMask + (CStringW)winFileData.cFileName;
				DWORD dwAttrNext = GetFileAttributesW(strFilePathNext);
				if (dwAttrNext & FILE_ATTRIBUTE_DIRECTORY)
				{
					int nNumNext = GetTempFileNum(strMaskTemp, winFileData.cFileName);
					nResultNumber = max(nNumNext, nResultNumber);
				}
			}

			FindClose(hFile);
		}

		++nResultNumber;

		if (0 == nResultNumber)
		{
			m_strTempDirectoryFile = m_strTempDirectory + strMaskTemp;
		}
		else
		{
			CString strDig = _T("");
			strDig.Format(_T("%d"), nResultNumber);
			m_strTempDirectoryFile = m_strTempDirectory + strMaskTemp + strDig;
		}

		if (!CreateDirectoryW(m_strTempDirectoryFile, NULL))
		{
			m_strTempDirectoryFile = _T("");
		}
	}

	void DeleteTempDirectory()
	{
		if ( _T("") == m_strTempDirectoryFile )
				return;
		
		SHFILEOPSTRUCTW shfos;
		ZeroMemory(&shfos, sizeof(shfos));
		shfos.wFunc = FO_DELETE;
		CStringW _local = m_strTempDirectoryFile + L"*.*";
		_local.AppendChar(0);
		_local.AppendChar(0);
		shfos.pFrom = _local.GetString();
		shfos.fFlags = FOF_SILENT + FOF_NOCONFIRMATION;

		SHFileOperationW(&shfos);
		RemoveDirectoryW(m_strTempDirectory);

		m_strTempDirectoryFile = _T("");
	}

	int GetTempFileNum(CStringW strMask, CStringW strDirPath)
	{
		int nIndex = strDirPath.Find(strMask);
		if (0 != nIndex)
			return -1;

		CStringW strNum = strDirPath.Mid(strMask.GetLength());

		int nLen = strNum.GetLength();
		int nResult = 0;
		int nIndexNum = 0;

		while (nLen > 0)
		{
			int nDig = XmlUtils::GetDigit(strNum[nIndexNum]);

			if (-1 == nDig)
				return -1;

			nResult *= 10;
			nResult += nDig;
			
			++nIndexNum;
			--nLen;
		}

		return nResult;
	}
};

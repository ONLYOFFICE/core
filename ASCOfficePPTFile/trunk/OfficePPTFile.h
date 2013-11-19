// OfficePPTFile3.h : Declaration of the CPPTFile3
#pragma once
#include "resource.h"       // main symbols

#include "AVSUtils.h"
#include "..\..\Common\OfficeFileTemplate.h"
#include "..\..\Common\RSA\XMLEncoder.h"

#include "VideoFileErrorDescription.h"

#include "Reader\PPTFileReader.h"

// IAVSOfficePPTFile
[object, uuid("01AB4BA0-7AE9-4539-8B12-608233D307BC"), dual, pointer_default(unique)]
__interface IAVSOfficePPTFile :  IAVSOfficeFileTemplate
{
	[id(100 + 0), propget] HRESULT DrawingXml( [out, retval] BSTR* pbstrXml );
    [id(100 + 1), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
    [id(100 + 1), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
	
	[id(1000 + 0)] HRESULT Is_PPTFile([in] BSTR fileName, [out, retval] VARIANT_BOOL* Result);
	
	[id(1000 + 1)] HRESULT GetDVDXml([out, retval] BSTR* bstrPTTXml);
	[id(1000 + 2)] HRESULT GetBluRayXml([out, retval] BSTR* bstrDVDXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IAVSOfficePPTFileEvents
[dispinterface,	uuid("DF3BE363-8404-4173-830A-984BF11790F5")]
__interface _IAVSOfficePPTFileEvents
{
	[id(1)] HRESULT ProcessVideoFile([in] LONG ID, [in] LONG Percent, [in, out, ref] SHORT* Cancel);
};	

// CAVSOfficePPTFile
[coclass, uuid("FC1AD867-FFA6-454a-82A0-1538465A55EB"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTFile.OfficePPTFile"), progid("AVSOfficePPTFile.OfficePPTFile.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficePPTFile : 
	public IAVSOfficePPTFile, 
	public CAVSATLError
{
public:
	CAVSOfficePPTFile() : 
	  CAVSATLError(__uuidof(CAVSOfficePPTFile), __uuidof(IAVSOfficePPTFile)), m_hSynchMutex(NULL),
	  m_arUserSources()
	{
		m_strTempDirectory = L"";
	}

	__event __interface _IAVSOfficePPTFileEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		m_hSynchMutex = CreateMutex(NULL, FALSE, NULL);

		m_pReader = NULL;
		m_Status = NULLMODE;

		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEHANDLE ( m_hSynchMutex );
		CloseFile();
	}

private:

	HANDLE m_hSynchMutex;
	LONG m_Status;

	CPPTFileReader* m_pReader;

	CAtlArray<CString> m_arUserSources;
	CString	m_strEditorXml;

	CStringW m_strTempDirectory;

	CXMLEncoder m_oEncoder;
	
public:
	STDMETHOD(get_DrawingXml)(BSTR* pbstrXml)
	{
		if (NULL == pbstrXml)
			return S_FALSE;
		
		if (1 == m_arUserSources.GetCount())
		{
			CString strXml = m_arUserSources[0];

#ifdef _DEBUG
			CFile oFile;
			oFile.CreateFile(_T("C:\\PPT_drawing.xml"));
			oFile.WriteStringUTF8(strXml);
			oFile.CloseFile();
			//XmlUtils::CXmlWriter oWriter;
			//oWriter.WriteString(strXml);
			//oWriter.SaveToFile(_T("C:\\PPT_drawing.xml"), TRUE);

			oFile.CreateFile(_T("C:\\ppt_editor.xml"));
			oFile.WriteStringUTF8(m_strEditorXml);
			oFile.CloseFile();
			//XmlUtils::CXmlWriter oWriter2;
			//oWriter2.WriteString(m_strEditorXml);
			//oWriter2.SaveToFile(_T("C:\\PPT_editor.xml"), TRUE);
#endif

			strXml = m_oEncoder.EncryptXML(strXml);
			*pbstrXml = strXml.AllocSysString();
			return S_OK;
		}
		
		CString strXml = _T("");
		for (size_t nIndex = 0; nIndex < m_arUserSources.GetCount(); ++nIndex)
		{
			strXml += m_arUserSources[nIndex];
		}

		strXml = _T("<Tracks>") + strXml + _T("</Tracks>");

#ifdef _DEBUG
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteString(strXml);
		oWriter.SaveToFile(_T("C:\\PPT_drawing.xml"), TRUE);
#endif

		CStringA strA = m_oEncoder.EncryptXML(strXml);

		*pbstrXml = strA.AllocSysString();

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
	STDMETHOD(Is_PPTFile)(BSTR fileName, VARIANT_BOOL* Result)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		return S_OK;
	}

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);

		m_arUserSources.RemoveAll();
		
		m_strTempDirectory = (CStringW)sDstPath;

		BOOL bRes = OpenFile(sSrcFileName);
		if (!bRes)
		{
			CloseFile();
			m_Status = NULLMODE;
			return S_FALSE;
		}

		for (size_t nIndex = 0; nIndex < m_pReader->m_oDocumentInfo.m_arUsers.GetCount(); ++nIndex)
		{
			CPPTUserInfo* pUser = m_pReader->m_oDocumentInfo.m_arUsers[nIndex];
			
			m_arUserSources.Add();

			pUser->CalculateEditor(pUser->m_oInfo);
			m_arUserSources[nIndex] = pUser->ToXmlVideoSource2();

			/*
			#ifdef _DEBUG
			if (0 == nIndex)
			{
				m_strEditorXml = pUser->ToXmlEditor2();
				if (TRUE)
					pUser->SaveThemeThumbnail(m_strTempDirectory, 180, 135, false);
			}
			#endif
			*/
		}

		return S_OK;
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		return S_OK;
	}
	
	// для конвертации с поддержкой интерактива...	
	STDMETHOD(GetDVDXml)(BSTR* bstrDVDXml)
	{
		return S_OK;
	}
	STDMETHOD(GetBluRayXml)(BSTR* bstrBluRayXml)
	{
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
			ParamValue->punkVal = m_oEncoder.GetBlowfishKey();
		}		
		else if (_T("EditorXml") == sParamName)
		{		
			m_strEditorXml = _T("");
			if (0 < m_pReader->m_oDocumentInfo.m_arUsers.GetCount())
			{
				m_strEditorXml = m_pReader->m_oDocumentInfo.m_arUsers[0]->ToXmlEditor2();
				if (FALSE)
					m_pReader->m_oDocumentInfo.m_arUsers[0]->SaveThemeThumbnail(m_strTempDirectory, 180, 135, false);
			}
			ParamValue->bstrVal = m_strEditorXml.AllocSysString();

#ifdef AVS_PPT_LOG_FILE
			CDirectory::SaveToFile(_T("C:\\editor.xml"), m_strEditorXml);
#endif
		}
		return S_OK;
	}

private:
	BOOL OpenFile(BSTR bstrFileName)
	{
		CloseFile();
		
		HRESULT hr; 
		IStorage *pStgFrom = NULL; 

		hr = StgOpenStorage(bstrFileName, NULL, STGM_READ | STGM_DIRECT | STGM_SHARE_DENY_WRITE, NULL, 0, &pStgFrom); 
		if (FAILED(hr)) 
		{ 
			RELEASEINTERFACE(pStgFrom);
			return FALSE;     
		} 

		RELEASEOBJECT(m_pReader);
		m_pReader = new CPPTFileReader(pStgFrom, m_strTempDirectory);
		m_pReader->m_oDocumentInfo.m_strFileDirectory = GetDirectory(CString(bstrFileName));
		if	(!m_pReader->IsPowerPoint()) 
		{ 
			RELEASEINTERFACE(pStgFrom);
			RELEASEOBJECT(m_pReader);
			return FALSE; 
		} 

		m_pReader->ReadPersistDirectory(); 
		m_pReader->ReadSlideList();

		RELEASEINTERFACE(pStgFrom);

		m_Status = READMODE;

		return TRUE;
	}
	BOOL CloseFile()
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		m_Status = NULLMODE;
		RELEASEOBJECT(m_pReader);
		return S_OK;
	}

	CString GetDirectory(CString strFileName)
	{
		int nIndex = strFileName.ReverseFind(TCHAR('\\'));
		if (-1 != nIndex)
		{
			return strFileName.Mid(0, nIndex + 1);
		}
		return strFileName;
	}
};


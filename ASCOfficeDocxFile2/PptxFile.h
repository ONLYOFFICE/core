// CAVSOfficePPTXFile.h : Declaration of the CAVSOfficePPTXFile

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "../Common/OfficeFileTemplate.h"
#include "../ASCOfficePPTXFile/ASCOfficePPTXFile.h"

#include "../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

void comLoadTheme(void* pArg, int nIndex, CString& strDstFolder);
bool comExtractFile(void* pArg, CString& sFile, CString& sDir);
bool comCompressFile(void* pArg, CString& sDir, CString& sFile);
bool comProgress(void* pArg, long ID, long Percent);

// IAVSOfficePPTXFile
[object, uuid("096DBAB4-3E54-4e14-99F4-B57D97081C61"), dual, pointer_default(unique)]
__interface IAVSOfficePPTXFile : IAVSOfficeFileTemplate
{
//	[id(3), helpstring("method SaveAsDrawingXML")] HRESULT SaveAsDrawingXML([in] BSTR sSrcFileName, [in] BSTR sDestPath, [in] BSTR sDestXMLFileName);

	[propget, id(4), helpstring("property TempDirectory")] HRESULT TempDirectory([out, retval] BSTR* pVal);
	[propput, id(4), helpstring("property TempDirectory")] HRESULT TempDirectory([in] BSTR newVal);
	[id(5), helpstring("method GetDVDXml")] HRESULT GetDVDXml([out,retval] BSTR* pbstrPTTXml);
	[id(6), helpstring("method GetBluRayXml")] HRESULT GetBluRayXml([out,retval] BSTR* pbstrDVDXml);
	[propget, id(7), helpstring("property DrawingXml")] HRESULT DrawingXml([out, retval] BSTR* pVal);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// IAVSOfficePPTXFile2
[object, uuid("68603ED6-0BB4-4330-9A80-D06B855D6DAA"), dual, pointer_default(unique)]
__interface IAVSOfficePPTXFile2 : IDispatch
{
	[id(10000 + 0)] HRESULT OpenFileToPPTY([in] BSTR bsInputDir, [in] BSTR bsFileDst);
	[id(10000 + 1)] HRESULT OpenDirectoryToPPTY([in] BSTR bsInputDir, [in] BSTR bsFileDst);

	[id(10000 + 2)] HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(10000 + 3)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(10000 + 4)] HRESULT SetUseSystemFonts([in] VARIANT_BOOL useSystemFonts);
	[id(10000 + 5)] HRESULT ConvertPPTYToPPTX([in] BSTR bsInputFile, [in] BSTR bsFileDst);

	[id(10000 + 6)] HRESULT SetThemesDir([in] BSTR bsThemesPath);
};

// CAVSOfficePPTXFile
[coclass, uuid("A3B9F270-175E-4982-B302-55ED6DF6361E"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTXFile.OfficePPTXFile"), progid("AVSOfficePPTXFile.OfficePPTXFile.1"), version(1.0), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSOfficePPTXFile 
	:	public IAVSOfficePPTXFile
	,	public IAVSOfficePPTXFile2
{
private:
	CPPTXFile m_oCPPTXFile;
public:

	__event __interface _IAVSOfficeFileTemplateEvents2;
	CAVSOfficePPTXFile() : m_oCPPTXFile(comLoadTheme, comExtractFile, comCompressFile, comProgress, this)
	{
	}

	~CAVSOfficePPTXFile()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		return m_oCPPTXFile.LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
	}
public:
	HRESULT SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		return m_oCPPTXFile.SaveToFile(sDstFileName, sSrcPath, sXMLOptions);
	}

public:
	STDMETHOD(get_TempDirectory)(BSTR* pVal)
	{
		return m_oCPPTXFile.get_TempDirectory(pVal);
	}
	STDMETHOD(put_TempDirectory)(BSTR newVal)
	{
		return m_oCPPTXFile.put_TempDirectory(newVal);
	}
public:
	STDMETHOD(GetDVDXml)(BSTR* pbstrPTTXml)
	{
		return m_oCPPTXFile.GetDVDXml(pbstrPTTXml);
	}
	STDMETHOD(GetBluRayXml)(BSTR* pbstrDVDXml)
	{
		return m_oCPPTXFile.GetBluRayXml(pbstrDVDXml);
	}
public:
	STDMETHOD(get_DrawingXml)(BSTR* pVal)
	{
		return m_oCPPTXFile.get_DrawingXml(pVal);
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return m_oCPPTXFile.SetAdditionalParam(ParamName, ParamValue);
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return m_oCPPTXFile.GetAdditionalParam(ParamName, ParamValue);
	}

	// to PPTY
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir)
	{
		return m_oCPPTXFile.SetMediaDir(bsMediaDir);
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		return m_oCPPTXFile.SetFontDir(bsFontDir);
	}
	STDMETHOD(SetThemesDir)(BSTR bsDir)
	{
		return m_oCPPTXFile.SetThemesDir(bsDir);
	}
	STDMETHOD(SetUseSystemFonts)(VARIANT_BOOL useSystemFonts)
	{
		return m_oCPPTXFile.SetUseSystemFonts(useSystemFonts);
	}
	STDMETHOD(OpenFileToPPTY)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.OpenFileToPPTY(bsInput, bsOutput);
	}
	STDMETHOD(OpenDirectoryToPPTY)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.OpenDirectoryToPPTY(bsInput, bsOutput);
	}
	STDMETHOD(ConvertPPTYToPPTX)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.ConvertPPTYToPPTX(bsInput, bsOutput);
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
};

void comLoadTheme(void* pArg, int nIndex, CString& strDstFolder)
{
	CAVSOfficePPTXFile* pCAVSOfficePPTXFile = static_cast<CAVSOfficePPTXFile*>(pArg);
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

	CString strThemeNotes = _T("");
	strThemeNotes.Format(_T("\\ppt\\theme\\theme%d.xml"), nIndex);
	pCAVSOfficePPTXFile->LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESTHEME), _T("PPTXW"), strDstFolder + strThemeNotes);

	CDirectory::CreateDirectory(strDstFolder + _T("\\ppt\\notesMasters"));
	pCAVSOfficePPTXFile->LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_XML_NOTESMASTER), _T("PPTXW"), strDstFolder + _T("\\ppt\\notesMasters\\notesMaster1.xml"));

	OOX::CPath pathNotesMasterRels = strDstFolder + _T("\\ppt\\notesMasters\\_rels");
	CDirectory::CreateDirectory(pathNotesMasterRels.GetPath());
	
	CString strThemeNotesNum = _T("");
	strThemeNotesNum.Format(_T("%d"), nIndex);
	CString strVal = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
						<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
						<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme") + strThemeNotesNum + _T(".xml\"/></Relationships>");
	CFile oFileRels;
	oFileRels.CreateFile(pathNotesMasterRels.GetPath() + FILE_SEPARATOR_STR + _T("notesMaster1.xml.rels"));
	oFileRels.WriteStringUTF8(strVal);
	oFileRels.CloseFile();
}
bool comExtractFile(void* pArg, CString& sFile, CString& sDir)
{
	COfficeUtils oCOfficeUtils(NULL);
	return S_OK == oCOfficeUtils.ExtractToDirectory(string2std_string(sFile), string2std_string(sDir), NULL, 0) ? true : false;
}
bool comCompressFile(void* pArg, CString& sDir, CString& sFile)
{
	COfficeUtils oCOfficeUtils(NULL);
	return S_OK == oCOfficeUtils.CompressFileOrDirectory(string2std_string(sDir), string2std_string(sFile), -1) ? true : false;
}
bool comProgress(void* pArg, long ID, long Percent)
{
	CAVSOfficePPTXFile* pCAVSOfficePPTXFile = static_cast<CAVSOfficePPTXFile*>(pArg);
	SHORT res = 0;
	pCAVSOfficePPTXFile->OnProgressEx(ID, Percent, &res);
	return (res != 0);
}
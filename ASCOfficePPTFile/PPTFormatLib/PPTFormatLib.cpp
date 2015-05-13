
#include "PPTFormatLib.h"


#include "Reader/PPTFileReader.h"
#include "../../../ASCPresentationEditor/PPTXWriter/Converter.h"

#include "../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#ifndef READMODE
    #define READMODE 1
#endif

#ifndef NULLMODE
    #define NULLMODE 0
#endif

COfficePPTFile::~COfficePPTFile()
{
    CloseFile();
}

bool COfficePPTFile::OpenFile(std::wstring sFileName)
{
	CloseFile();
	
    POLE::Storage *pStgFrom = new POLE::Storage(sFileName.c_str());
	if (pStgFrom == NULL) return FALSE;
	
	pStgFrom->open(false,false);

	m_pReader = new CPPTFileReader(pStgFrom, m_strTempDirectory);
    ((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_strFileDirectory = GetDirectory(sFileName.c_str());
	
	if	(!((CPPTFileReader*)m_pReader)->IsPowerPoint()) 
	{ 
		CPPTFileReader* r = (CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
		m_pReader = NULL;
		return FALSE; 
	} 

	((CPPTFileReader*)m_pReader)->ReadPersistDirectory(); 
	((CPPTFileReader*)m_pReader)->ReadSlideList();

	m_Status = READMODE;

	return TRUE;
}
bool COfficePPTFile::CloseFile()
{
	m_Status = NULLMODE;

	CPPTFileReader* r = (CPPTFileReader*)m_pReader;	RELEASEOBJECT(r);
	m_pReader = NULL;
	return S_OK;
}
HRESULT COfficePPTFile::LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath)
{
    if (m_strTempDirectory.length() < 1)
    {
        m_strTempDirectory = FileSystem::Directory::GetTempPath().GetBuffer();
    }

    bool bRes = OpenFile(sSrcFileName);
	if (!bRes)
    {
		CloseFile();
		m_Status = NULLMODE;
		return S_FALSE;
	}
	m_strEditorXml = _T("");

	if (0 < ((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers.size())
	{
		m_strEditorXml = ((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers[0]->ToXmlEditor2();
	}
//////////////////////////////////////

	if (m_strEditorXml.GetLength() <1) return S_FALSE;

    if (sDstPath.length() > 0)
	{	

		NSPresentationEditor::CDocument		oPresentationEditor;
		NSPresentationEditor::CPPTXWriter	oPPTXWriter;
		
		oPresentationEditor.LoadFromXML(m_strEditorXml);
		
        oPPTXWriter.m_strTempDirectory = std_string2string(sDstPath);
		
		
		oPPTXWriter.CreateFile(&oPresentationEditor);	
		oPPTXWriter.CloseFile();
		
	}

	//CloseFile();	нельзя тута- поскольку в Win32 используется PresentationEditor для конвертации xml -> pptx

	return S_OK;
}

HRESULT COfficePPTFile::GetAdditionalParam (CString sParamName, VARIANT* ParamValue)
{
	if (NULL == ParamValue)
		return S_FALSE;

	if (_T("EditorXml") == sParamName)
	{		
#if defined(_WIN32) || defined (_WIN64)
        ParamValue->bstrVal = m_strEditorXml.AllocSysString();
#else
        ParamValue->bstrVal = m_strEditorXml;
#endif
	}
	return S_OK;
}

CString COfficePPTFile::GetDirectory(CString strFileName)
{
	int nIndex = strFileName.ReverseFind(FILE_SEPARATOR_CHAR);
	if (-1 != nIndex)
	{
		return strFileName.Mid(0, nIndex + 1);
	}
	return strFileName;
}

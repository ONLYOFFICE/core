
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

	m_pReader = new CPPTFileReader(pStgFrom, std_string2string(m_strTempDirectory));
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
	if (0 < ((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers.size())
	{
		NSPresentationEditor::CPPTXWriter	oPPTXWriter;
        oPPTXWriter.m_strTempDirectory = std_string2string(sDstPath);
		
		
		oPPTXWriter.CreateFile(((CPPTFileReader*)m_pReader)->m_oDocumentInfo.m_arUsers[0]);	
		oPPTXWriter.CloseFile();

	}
	return S_OK;
}

HRESULT COfficePPTFile::GetAdditionalParam (CString sParamName, VARIANT* ParamValue)
{
	if (NULL == ParamValue)
		return S_FALSE;

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

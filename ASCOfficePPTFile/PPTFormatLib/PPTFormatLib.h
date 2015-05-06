#pragma once

#if defined(_WIN32) || defined (_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../../DesktopEditor/common/ASCVariant.h"
    #include "../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include <vector>

class COfficePPTFile 
{
public:
    COfficePPTFile()
	{
		m_strTempDirectory = L"";
		m_pReader = NULL;
		m_Status = 0;

	}

    ~COfficePPTFile();

private:

	LONG m_Status;

	void* m_pReader;

	std::vector<CString> m_arUserSources;
	CString	m_strEditorXml;

	CString m_strTempDirectory;
	
public:

	HRESULT put_TempDirectory(CString sDir)
	{
		m_strTempDirectory = sDir;
		return S_OK;
	}
	
	CString get_TempDirectory()
	{
		return m_strTempDirectory;
	}

	HRESULT LoadFromFile(CString sSrcFileName, CString sDstPath, CString sXMLOptions);


	HRESULT GetAdditionalParam (CString sParamName, VARIANT* ParamValue);
private:
    bool OpenFile(CString sFileName);

    bool CloseFile();

	CString GetDirectory(CString strFileName);
};


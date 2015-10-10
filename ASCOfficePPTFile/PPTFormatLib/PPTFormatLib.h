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

	int m_Status;

	void* m_pReader;

	std::vector<CString> m_arUserSources;

    std::wstring m_strTempDirectory;
	
public:

    HRESULT put_TempDirectory(std::wstring sDir)
	{
		m_strTempDirectory = sDir;
		return S_OK;
	}
	
    std::wstring get_TempDirectory()
	{
		return m_strTempDirectory;
	}

    HRESULT LoadFromFile(std::wstring sSrcFileName, std::wstring sDstPath);


	HRESULT GetAdditionalParam (CString sParamName, VARIANT* ParamValue);
private:
    bool OpenFile(std::wstring sFileName);

    bool CloseFile();

	CString GetDirectory(CString strFileName);
};


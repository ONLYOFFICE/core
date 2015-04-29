#ifndef DOC_FORMAT_LIB
#define DOC_FORMAT_LIB

#include <string>

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
#endif

struct ProgressCallback;

class COfficeDocFile 
{
public:
	COfficeDocFile()
	{
		m_strTempDirectory = L"";

	}

	virtual ~COfficeDocFile()
	{
	}

private:

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

    HRESULT LoadFromFile(std::wstring sSrcFileName, std::wstring sDstFileName, ProgressCallback *ffCallBack = NULL);
    HRESULT SaveToFile(std::wstring sDstFileName, std::wstring sSrcFileName, ProgressCallback *ffCallBack = NULL);

};

#endif //DOC_FORMAT_LIB

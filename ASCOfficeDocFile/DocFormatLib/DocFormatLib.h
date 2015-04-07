#ifndef DOC_FORMAT_LIB
#define DOC_FORMAT_LIB

#if defined(_WIN32) || defined(_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../../DesktopEditor/common/ASCVariant.h"
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
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

	HRESULT LoadFromFile(CString sSrcFileName, CString sDstFileName, CString sXMLOptions, ProgressCallback *ffCallBack = NULL);
	HRESULT SaveToFile(CString sDstFileName, CString sSrcFileName, CString sXMLOptions, ProgressCallback *ffCallBack =NULL);

};

#endif //DOC_FORMAT_LIB

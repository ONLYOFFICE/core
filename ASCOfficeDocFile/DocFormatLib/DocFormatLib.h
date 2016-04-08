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
	}

	virtual ~COfficeDocFile()
	{
	}

    HRESULT LoadFromFile(std::wstring sSrcFileName, std::wstring sDstFileName, ProgressCallback *ffCallBack = NULL);
    HRESULT SaveToFile(std::wstring sDstFileName, std::wstring sSrcFileName, ProgressCallback *ffCallBack = NULL);

};

#endif //DOC_FORMAT_LIB

#pragma once

#include "TxtXmlEvent.h"

#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
	#include <atlstr.h>
#else
    #include "../../../DesktopEditor/common/ASCVariant.h"
#endif

namespace Writers
{
	class FileWriter;
}

class CTxtXmlFile : public TxtXml::ITxtXmlEvent
{
public:
	virtual bool Progress(long ID, long Percent);

    HRESULT txt_LoadFromFile(const std::wstring & sSrcFileName, const std::wstring & sDstPath, const std::wstring & sXMLOptions);
    HRESULT txt_SaveToFile	(const std::wstring & sDstFileName, const std::wstring & sSrcPath, const std::wstring & sXMLOptions);

	//HRESULT xml_LoadFromFile(CString sSrcFileName, CString sDstPath, CString sXMLOptions);
	//HRESULT xml_SaveToFile	(CString sDstFileName, CString sSrcPath, CString sXMLOptions);
	
	CTxtXmlFile();

private:
	void CreateDocxEmpty(CString path, Writers::FileWriter * DocxWriter) ;

};

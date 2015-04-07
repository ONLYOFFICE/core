#pragma once

#include "TxtXmlEvent.h"
#include "../../../Common/DocxFormat/Source/XML/stringcommon.h"

namespace Writers
{
	class FileWriter;
}

class CTxtXmlFile : public TxtXml::ITxtXmlEvent
{
public:
	virtual bool Progress(long ID, long Percent);

	HRESULT txt_LoadFromFile(CString sSrcFileName, CString sDstPath, CString sXMLOptions);
	HRESULT txt_SaveToFile	(CString sDstFileName, CString sSrcPath, CString sXMLOptions);

	//HRESULT xml_LoadFromFile(CString sSrcFileName, CString sDstPath, CString sXMLOptions);
	//HRESULT xml_SaveToFile	(CString sDstFileName, CString sSrcPath, CString sXMLOptions);
	
	CTxtXmlFile();

private:
	void CreateDocxEmpty(CString path, Writers::FileWriter * DocxWriter) ;

};
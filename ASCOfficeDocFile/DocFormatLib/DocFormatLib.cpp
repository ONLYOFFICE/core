#include "DocFormatLib.h"

#include "../DocDocxConverter/Converter.h"
//#include "../DocxDocConverter/FileConverter.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

HRESULT COfficeDocFile::LoadFromFile(std::wstring bsDocFile, std::wstring bsDocxDirectory, ProgressCallback *ffCallBack )
{
	HRESULT hr = S_FALSE;

	DocFileFormat::Converter docToDocx;
    hr= docToDocx.LoadAndConvert(std_string2string(bsDocFile), std_string2string(bsDocxDirectory), ffCallBack);

	return hr;
}

HRESULT COfficeDocFile::SaveToFile (std::wstring sDstFileName, std::wstring sSrcFileName, ProgressCallback *ffCallBack )
{
    return S_OK;
}

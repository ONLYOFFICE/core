#include "DocFormatLib.h"

#include "../DocDocxConverter/Converter.h"
//#include "../DocxDocConverter/FileConverter.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

HRESULT COfficeDocFile::LoadFromFile(CString bsDocFile, CString bsDocxDirectory, CString bsXMLOptions, ProgressCallback *ffCallBack ) 
{
	HRESULT hr = S_FALSE;

	DocFileFormat::Converter docToDocx;
	hr= docToDocx.LoadAndConvert(bsDocFile, bsDocxDirectory, ffCallBack);

	return hr;
}

HRESULT COfficeDocFile::SaveToFile (CString sDstFileName, CString sSrcFileName, CString sXMLOptions, ProgressCallback *ffCallBack )
{
	return S_OK;
}

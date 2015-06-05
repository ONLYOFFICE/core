#include "ConvertOO2OOX.h"

#include "../../Common/OfficeFileErrorDescription.h"

#include "../src/docx/docx_package.h"
#include "../src/docx/xlsx_package.h"
#include "../src/docx/pptx_package.h"

#include "../src/docx/docx_conversion_context.h"
#include "../src/docx/pptx_conversion_context.h"
#include "../src/docx/xlsxconversioncontext.h"

#include "../include/cpdoccore/odf/odf_document.h"


HRESULT ConvertOds2Xlsx(cpdoccore::odf::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::xlsx_document outputXlsx;
	cpdoccore::oox::xlsx_conversion_context conversionContext(&outputXlsx, &inputOdf);
   
	conversionContext.set_font_directory(fontsPath);
	
	if (inputOdf.xlsx_convert(conversionContext) == false) return S_FALSE;
    
	outputXlsx.write(dstPath);
    return S_OK;
}
HRESULT ConvertOdt2Docx(cpdoccore::odf::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::docx_document	outputDocx;
    cpdoccore::oox::docx_conversion_context conversionContext(&outputDocx, &inputOdf);
   
	conversionContext.set_font_directory(fontsPath);

	if (inputOdf.docx_convert(conversionContext) == false) return S_FALSE;
		
    outputDocx.write(dstPath);
		
    return S_OK;
}
HRESULT ConvertOdp2Pptx(cpdoccore::odf::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::pptx_document	outputPptx;
    cpdoccore::oox::pptx_conversion_context conversionContext(&outputPptx, &inputOdf);
	
	conversionContext.set_font_directory(fontsPath);
    
	if (inputOdf.pptx_convert(conversionContext) == false) return S_FALSE;
    outputPptx.write(dstPath);

    return S_OK;
}
HRESULT ConvertOO2OOX(const std::wstring & srcPath, const std::wstring & dstPath, const std::wstring & fontsPath, bool bOnlyPresentation, const ProgressCallback* CallBack)
{
	HRESULT hr = S_OK;

	try 
    {
		cpdoccore::odf::odf_document inputOdf(srcPath, CallBack);
		
		int type = inputOdf.get_office_mime_type();
		bool encrypted = inputOdf.get_encrypted();

		if (encrypted) return AVS_ERROR_DRM;

		//if (type<1)
		//{
		//	if (ext == L".odt")type = 1;
		//	if (ext == L".ods")type = 2;
		//	if (ext == L".odp")type = 3;

		//}

		if (bOnlyPresentation && type != 3)return AVS_ERROR_UNEXPECTED;

		switch (type)
		{
		case 1:
			hr = ConvertOdt2Docx(inputOdf,dstPath, fontsPath);
			break;
		case 2:
			hr = ConvertOds2Xlsx(inputOdf,dstPath, fontsPath);
			break;
		case 3:
			hr = ConvertOdp2Pptx(inputOdf,dstPath, fontsPath);
			break;
		}
		if (hr == S_OK)
		{
			hr = S_OK;
			inputOdf.UpdateProgress(1000000);
		}

	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}
	return hr;

}
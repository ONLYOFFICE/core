#include "stdafx.h"

#include "ConvertOO2OOX.h"

#include <cpdoccore/../../src/docx/docx_package.h>
#include <cpdoccore/../../src/docx/xlsx_package.h>
#include <cpdoccore/../../src/docx/pptx_package.h>

#include <cpdoccore/../../src/docx/docx_conversion_context.h>
#include <cpdoccore/../../src/docx/pptx_conversion_context.h>
#include <cpdoccore/../../src/docx/xlsxconversioncontext.h>

#include <cpdoccore/odf/odf_document.h>

HRESULT ConvertOds2Xlsx(cpdoccore::odf::odf_document & inputDoc, const std::wstring & dstPath)
{
    cpdoccore::oox::package::xlsx_document xlsx;
    cpdoccore::oox::xlsx_conversion_context conversionContext(&xlsx, &inputDoc);
    inputDoc.xlsx_convert(conversionContext);
    xlsx.write(dstPath);

    return S_OK;
}
HRESULT ConvertOdt2Docx(cpdoccore::odf::odf_document & inputDoc, const std::wstring & dstPath)
{
    cpdoccore::oox::package::docx_document docx;
    cpdoccore::oox::docx_conversion_context conversionContext(&docx, &inputDoc);
    inputDoc.docx_convert(conversionContext);
    docx.write(dstPath);

    return S_OK;
}
HRESULT ConvertOdp2Pptx(cpdoccore::odf::odf_document & inputDoc, const std::wstring & dstPath)
{
    cpdoccore::oox::package::pptx_document pptx;
    cpdoccore::oox::pptx_conversion_context conversionContext(&pptx, &inputDoc);
    inputDoc.pptx_convert(conversionContext);
    pptx.write(dstPath);

    return S_OK;
}
HRESULT ConvertOO2OOX(const std::wstring &ext, const std::wstring & srcPath, const std::wstring & dstPath)
{
	HRESULT hr = S_OK;

	try 
    {
		cpdoccore::odf::odf_document inputDoc(srcPath);

		int type = inputDoc.get_office_mime_type();

		if (type<1)
		{
			if (ext == L".odt")type = 1;
			if (ext == L".ods")type = 2;
			if (ext == L".odp")type = 3;

		}
		switch (type)
		{
		case 1:
			hr = ConvertOdt2Docx(inputDoc,dstPath);
			break;
		case 2:
			hr = ConvertOds2Xlsx(inputDoc,dstPath);
			break;
		case 3:
			hr = ConvertOdp2Pptx(inputDoc,dstPath);
			break;

		}

	}
	catch(...)
	{
		return E_FAIL;
	}
	return hr;

}
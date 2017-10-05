/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "ConvertOO2OOX.h"

#include "../../Common/OfficeFileErrorDescription.h"

#include "../src/docx/docx_package.h"
#include "../src/docx/xlsx_package.h"
#include "../src/docx/pptx_package.h"

#include "../src/docx/docx_conversion_context.h"
#include "../src/docx/pptx_conversion_context.h"
#include "../src/docx/xlsxconversioncontext.h"

#include "../include/cpdoccore/odf/odf_document.h"


HRESULT ConvertOds2Xlsx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::xlsx_document outputXlsx;
	cpdoccore::oox::xlsx_conversion_context conversionContext( &inputOdf);
   
	conversionContext.set_output_document	(&outputXlsx);
	conversionContext.set_font_directory	(fontsPath);
	
	if (inputOdf.xlsx_convert(conversionContext) == false) return S_FALSE;
    
	outputXlsx.write(dstPath);
    return S_OK;
}
HRESULT ConvertOdt2Docx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::docx_document	outputDocx;
    cpdoccore::oox::docx_conversion_context conversionContext(&inputOdf);
   
	conversionContext.set_output_document	(&outputDocx);
	conversionContext.set_font_directory	(fontsPath);

	if (inputOdf.docx_convert(conversionContext) == false) return S_FALSE;
		
    outputDocx.write(dstPath);
		
    return S_OK;
}
HRESULT ConvertOdp2Pptx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::pptx_document	outputPptx;
    cpdoccore::oox::pptx_conversion_context conversionContext(&inputOdf);
	
	conversionContext.set_output_document	(&outputPptx);
	conversionContext.set_font_directory	(fontsPath);
    
	if (inputOdf.pptx_convert(conversionContext) == false) return S_FALSE;
    outputPptx.write(dstPath);

    return S_OK;
}
HRESULT ConvertOO2OOX(const std::wstring & srcPath, const std::wstring & dstPath, const std::wstring & fontsPath, bool bOnlyPresentation, const ProgressCallback* CallBack)
{
	HRESULT hr = S_OK;

	try 
    {
		cpdoccore::odf_reader::odf_document inputOdf(srcPath, CallBack);
		
		int type = inputOdf.get_office_mime_type();
		bool encrypted = inputOdf.get_encrypted();

		if (encrypted) return AVS_ERROR_DRM;

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
/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "ConvertOO2OOX.h"

#include "../../Common/OfficeFileErrorDescription.h"

#include "docx_package.h"
#include "xlsx_package.h"
#include "pptx_package.h"

#include "docx_conversion_context.h"
#include "pptx_conversion_context.h"
#include "xlsxconversioncontext.h"

#include "../Format/odf_document.h"

_UINT32 ConvertOds2Xlsx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::xlsx_document outputXlsx;
	cpdoccore::oox::xlsx_conversion_context conversionContext( &inputOdf);
   
	conversionContext.set_output_document	(&outputXlsx);
	conversionContext.set_font_directory	(fontsPath);
	
	if (inputOdf.xlsx_convert(conversionContext) == false) return AVS_ERROR_FILEFORMAT;
    
	outputXlsx.write(dstPath);
    return 0;
}
_UINT32 ConvertOdt2Docx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::docx_document	outputDocx;
    cpdoccore::oox::docx_conversion_context conversionContext(&inputOdf);
   
	conversionContext.set_output_document	(&outputDocx);
	conversionContext.set_font_directory	(fontsPath);

	if (inputOdf.docx_convert(conversionContext) == false) return AVS_ERROR_FILEFORMAT;
		
    outputDocx.write(dstPath);
		
    return 0;
}
_UINT32 ConvertOdp2Pptx(cpdoccore::odf_reader::odf_document & inputOdf, const std::wstring & dstPath, const std::wstring & fontsPath)
{
    cpdoccore::oox::package::pptx_document	outputPptx;
    cpdoccore::oox::pptx_conversion_context conversionContext(&inputOdf);
	
	conversionContext.set_output_document	(&outputPptx);
	conversionContext.set_font_directory	(fontsPath);
    
	if (inputOdf.pptx_convert(conversionContext) == false) return AVS_ERROR_FILEFORMAT;
    outputPptx.write(dstPath);

    return 0;
}
_UINT32 ConvertODF2OOXml(const std::wstring & srcPath, const std::wstring & dstPath, const std::wstring & fontsPath, const std::wstring & tempPath, const std::wstring & password)
{
	_UINT32 nResult = 0;

	_CP_LOG << L"start odf to ..." << std::endl;
	try
    {
        cpdoccore::odf_reader::odf_document inputOdf(srcPath, tempPath, password);
		
		int type		= inputOdf.get_office_mime_type();
		bool bEncrypted	= inputOdf.get_encrypted();
		bool bError		= inputOdf.get_error();

		if (bError)
		{
			if (bEncrypted)
			{
				if (password.empty())	return AVS_ERROR_DRM;
				else					return AVS_ERROR_PASSWORD;
			}
			else
			{
				return AVS_ERROR_FILEFORMAT;
			}
		}

		switch (type)
		{
		case 1:
		case 4:
			nResult = ConvertOdt2Docx(inputOdf, dstPath, fontsPath);
			break;
		case 2:
		case 5:
			nResult = ConvertOds2Xlsx(inputOdf, dstPath, fontsPath);
			break;
		case 3:
		case 6:
		case 7:
			nResult = ConvertOdp2Pptx(inputOdf, dstPath, fontsPath);
			break;
		}
	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}
	_CP_LOG << L"... finish" << std::endl;
	return nResult;

}

_UINT32 ConvertOTF2ODF(const std::wstring & srcPath)
{
	_UINT32 nResult = 0;
	
	std::wstring manifest_xml	= srcPath + FILE_SEPARATOR_STR + L"META-INF" + FILE_SEPARATOR_STR + L"manifest.xml";
	std::wstring mimetype_xml	= srcPath + FILE_SEPARATOR_STR + L"mimetype";

	std::wstring manifest_content_file_;
	NSFile::CFileBinary::ReadAllTextUtf8(manifest_xml, manifest_content_file_); 

	if (!manifest_content_file_.empty())
	{
		size_t nPos = manifest_content_file_.find(L"-template");
		if (std::wstring::npos != nPos)
		{
			XmlUtils::replace_all(manifest_content_file_, L"-template", L"");
			NSFile::CFileBinary::SaveToFile(manifest_xml, manifest_content_file_); 
		}
	}

	std::wstring mimetype_content_file_;
	NSFile::CFileBinary::ReadAllTextUtf8(mimetype_xml, mimetype_content_file_); 

	if (!mimetype_content_file_.empty())
	{
		size_t nPos = mimetype_content_file_.find(L"-template");
		if (std::wstring::npos != nPos)
		{
			mimetype_content_file_ = mimetype_content_file_.substr(0, nPos);
		}
		NSFile::CFileBinary::SaveToFile(mimetype_xml, mimetype_content_file_); 
	}

	return nResult;
}

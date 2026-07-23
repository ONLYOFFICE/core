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

#include "office_binary_data.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../../Common/OfficeFileFormatChecker.h"

namespace cpdoccore { 
namespace odf_reader {

// office:binary-data
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_binary_data::ns = L"office";
const wchar_t * office_binary_data::name = L"binary-data";

std::wostream & office_binary_data::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}

void office_binary_data::add_attributes( const xml::attributes_wc_ptr & Attributes )
{    
}

void office_binary_data::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
}

void office_binary_data::add_text(const std::wstring & Text)
{
	base64Binary_ = std::string(Text.begin(), Text.end());
}

std::wstring office_binary_data::write_to(const std::wstring & path)
{
	std::wstring result;

	int nLength = 0;
	unsigned char *pData = NULL;

	NSFile::CBase64Converter::Decode(base64Binary_.c_str(), base64Binary_.length(), pData, nLength);
	if (pData)
	{
		CImageFileFormatChecker image_checker;
		std::wstring sExt = image_checker.DetectFormatByData(pData, nLength);

		if (sExt.empty())
		{
			std::wstring documentID;
			COfficeFileFormatChecker office_checker;
			
			if (office_checker.isPdfFormatFile(pData, nLength, documentID))
			{
				type_binary_data = 20; // oox::_rels_type = typePDF;
				sExt = L"pdf";
			}
		}

		NSFile::CFileBinary file;

		std::wstring bin_file = file.CreateTempFileWithUniqueName(path + FILE_SEPARATOR_STR, sExt);
		if (file.CreateFileW(bin_file))
		{
			file.WriteFile(pData, nLength);
			file.CloseFile();

			int pos = bin_file.rfind(FILE_SEPARATOR_STR);
			result = bin_file.substr(pos + 1);
		}
		delete []pData; pData = NULL;
	}
	return result;
}

}
}

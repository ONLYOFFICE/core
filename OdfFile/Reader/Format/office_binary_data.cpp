﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
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

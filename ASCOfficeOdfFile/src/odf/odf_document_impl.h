/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once

#include <string>
#include "odf_content_xml.h"
#include <cpdoccore/odf/odf_document.h>

namespace cpdoccore { 

namespace oox {
class docx_conversion_context;
class xlsx_conversion_context;
}

namespace odf_reader {

class odf_read_context;
typedef shared_ptr<odf_read_context>::Type odf_read_context_ptr;

class content_xml_t;
typedef shared_ptr<content_xml_t>::Type content_xml_t_ptr;

class odf_document::Impl
{
public:
    Impl(const std::wstring & Folder, const ProgressCallback* CallBack);
    odf_read_context & odf_context();
   
	bool docx_convert(oox::docx_conversion_context & Context);
    bool xlsx_convert(oox::xlsx_conversion_context & Context);
    bool pptx_convert(oox::pptx_conversion_context & Context);

    const std::wstring & get_folder() const { return base_folder_; }
   
	const	office_element * get_content() const;
		office_element * get_content();

	long get_office_mime_type() {return office_mime_type_;}

	bool get_encrypted(){return encrypted;}

	bool UpdateProgress(long Complete);
    
private:
	const ProgressCallback* pCallBack;
	short bUserStopConvert;
   
	odf_read_context_ptr context_;
    
	void parse_styles();
    void parse_fonts();
	void parse_manifests();
    void parse_settings();

private:
    content_xml_t_ptr content_xml_;
    content_xml_t_ptr styles_xml_;
    content_xml_t_ptr meta_xml_;
    content_xml_t_ptr settings_xml_;
    content_xml_t_ptr manifest_xml_;

private:
    std::wstring base_folder_;
	long office_mime_type_;
	bool encrypted;
            
};

}
}


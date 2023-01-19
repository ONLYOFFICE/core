/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#include <string>
#include "odf_content_xml.h"
#include "odf_document.h"

namespace cpdoccore 
{ 
	namespace xml 
	{
		class sax;
	}
	namespace oox 
	{
		class docx_conversion_context;
		class xlsx_conversion_context;
	}
	namespace odf_reader 
{

class odf_read_context;
typedef shared_ptr<odf_read_context>::Type odf_read_context_ptr;

class content_xml_t;
typedef shared_ptr<content_xml_t>::Type content_xml_t_ptr;

class odf_document::Impl
{
public:
    Impl(const std::wstring & SrcPath, const std::wstring & TempPath, const std::wstring & Password);	
	Impl(xml::sax * Reader, const std::wstring & TempPath);

	virtual ~Impl();

    odf_read_context & odf_context();
   
	static content_xml_t_ptr read_file_content(const std::wstring & Path);
	content_xml_t_ptr read_file_content(xml::sax * reader_owner);

	bool docx_convert(oox::docx_conversion_context & Context);
    bool xlsx_convert(oox::xlsx_conversion_context & Context);
    bool pptx_convert(oox::pptx_conversion_context & Context);

    const std::wstring & get_folder() const;
    const std::wstring & get_temp_folder() const;
   
	const office_element * get_content() const;
		  office_element * get_content();

	int get_office_mime_type() {return office_mime_type_;}

	bool get_encrypted()		{return (false == map_encryptions_.empty());}
	bool get_encrypted_extra()	{return (false == map_encryptions_extra_.empty());}
	
	bool get_error() {return bError;}
    
private:
    bool bError = false;
   
	odf_read_context_ptr context_;
    
	void parse_styles	(office_element *element);
    void parse_fonts	(office_element *elemen);
	void parse_manifests(office_element *element);
    void parse_settings	(office_element *element);
    void parse_meta		(office_element *element);

	bool decrypt_folder (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath);
	bool decrypt_file (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath, office_element_ptr data, int size );

	std::string read_binary(const std::wstring & Path);

    content_xml_t_ptr	content_xml_;
    content_xml_t_ptr	styles_xml_;
    content_xml_t_ptr	meta_xml_;
    content_xml_t_ptr	settings_xml_;
	content_xml_t_ptr	manifest_xml_;

	std::string			jsaProject_bin_;
    
	std::wstring mimetype_content_file_;

    std::wstring base_folder_;
    std::wstring tmp_folder_;
    std::wstring tmp_folder_original_;

	int office_mime_type_;

	int GetMimetype(std::wstring value);

	std::map<std::wstring, std::pair<office_element_ptr, int>> map_encryptions_;
 	std::map<std::wstring, std::pair<office_element_ptr, int>> map_encryptions_extra_;           
};

}
}


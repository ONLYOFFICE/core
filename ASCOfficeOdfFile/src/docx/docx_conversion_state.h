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
#ifndef _CPDOCCORE_DOCX_CONVERSIONSTATE_H_
#define _CPDOCCORE_DOCX_CONVERSIONSTATE_H_

namespace cpdoccore {
namespace oox {

struct conversion_state
{
    const odf_reader::style_instance * current_processed_style_;
    std::list< const odf_reader::style_text_properties * > text_properties_stack_;
    bool page_break_after_;
    bool in_automatic_style_;
    std::wstring current_page_properties_;
    bool next_dump_page_properties_;
    std::wstring text_list_style_name_;
    std::list<std::wstring> list_style_stack_;   
    bool first_element_list_item_;
    bool in_paragraph_;
    std::list<odf_reader::office_element *> delayed_elements_;
    odf_reader::office_element * section_properties_in_table_;
    std::wstring current_master_page_name_;
    bool rtl_; // right-to-left
};

}
}

#endif

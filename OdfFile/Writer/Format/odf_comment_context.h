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
#include <vector>

#include <CPOptional.h>

#include "office_elements_create.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer
{

class odf_conversion_context;
class odf_style_context;

class odf_comment_context
{
public:
	odf_comment_context(odf_conversion_context *odf_context);
    ~odf_comment_context();

	void set_styles_context(odf_style_context*  styles_context);

	int find_by_id(int oox_id);
	
	bool is_started();// for content

	void start_comment	(office_element_ptr & elm, int oox_id);
	void end_comment	(office_element_ptr & elm, int oox_id);

	void start_comment_content	();
	void end_comment_content	();

	void set_author		(std::wstring author);
	void set_initials	(std::wstring author);
	void set_date		(std::wstring author);
	
	void set_position (double x, double y);

private:
	std::wstring find_name_by_id(int oox_id);

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}
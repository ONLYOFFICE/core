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

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/chartsymbol.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/chartinterpolation.h"
#include "../../DataTypes/chartsolidtype.h"
#include "../../DataTypes/chartlabelarrangement.h"
#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/chartdatalabelnumber.h"
#include "../../DataTypes/charterrorcategory.h"
#include "../../DataTypes/chartseriessource.h"
#include "../../DataTypes/chartregressiontype.h"
#include "../../DataTypes/chartlabelposition.h"
#include "../../DataTypes/direction.h"

namespace cpdoccore { 
namespace odf_reader {

	class chart_format_properties : public std::vector<_property>
	{
	public:
		void add_attributes(const xml::attributes_wc_ptr & Attributes);
		void apply_from(const chart_format_properties & Other);

		odf_types::common_rotation_angle_attlist  common_rotation_angle_attlist_;
	};

// style:chart-properties
class style_chart_properties : public office_element_impl<style_chart_properties>//стили чарта не наследуются
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleChartProperties;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
 
public:
	chart_format_properties content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_chart_properties);

}
}

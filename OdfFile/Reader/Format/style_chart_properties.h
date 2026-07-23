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
	typedef boost::shared_ptr<chart_format_properties> chart_format_properties_ptr;

	template <class T>
	bool GetProperty(chart_format_properties_ptr & prop, const std::wstring Name, T & Val)
	{
		return prop ? GetProperty<T>(*prop, Name, Val) : false;
	}
// style:chart-properties
class style_chart_properties : public office_element_impl<style_chart_properties>//chart styles are not inherited
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

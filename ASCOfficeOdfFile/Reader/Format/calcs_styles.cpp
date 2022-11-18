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

#include "calcs_styles.h"
#include <algorithm>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

text_format_properties_content_ptr calc_text_properties_content(const std::vector<const style_text_properties*> & textProps)
{
	if (textProps.empty()) return text_format_properties_content_ptr();

	text_format_properties_content_ptr result = boost::make_shared<text_format_properties_content>();
 	
	for (size_t i = 0; i < textProps.size(); i++)
    {
        if (textProps[i])
            result->apply_from(textProps[i]->content_);
    }
    return result;
}

text_format_properties_content_ptr calc_text_properties_content(const style_instance * styleInstance)
{
    std::vector<const style_text_properties*> textProps;

    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_text_properties * textProp = content->get_style_text_properties())
			{
                textProps.insert(textProps.begin(), textProp);
			}
		
        styleInstance = styleInstance->parent();
    }
    return calc_text_properties_content(textProps);
}

text_format_properties_content_ptr calc_text_properties_content(const std::vector<const style_instance *> & styleInstances)
{
	if (styleInstances.empty()) return text_format_properties_content_ptr();

	text_format_properties_content_ptr result = boost::make_shared<text_format_properties_content>();

 	for (size_t i = 0; i < styleInstances.size(); i++)
    {
		text_format_properties_content_ptr props = calc_text_properties_content(styleInstances[i]);
		if (props)
		{
			result->apply_from(*props.get());
		}
    }
    return result;
}

//////////////
graphic_format_properties calc_graphic_properties_content(const std::vector<const graphic_format_properties*> & graphicProps)
{
    graphic_format_properties result;
 	for (size_t i = 0; i < graphicProps.size(); i++)
    {
        if (graphicProps[i])
			result.apply_from(graphicProps[i]);
    }
    return result;
}

graphic_format_properties calc_graphic_properties_content(const style_instance * styleInstance)
{
    std::vector<const graphic_format_properties*> graphicProps;
    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const graphic_format_properties * graphicProp = content->get_graphic_properties())
			{
                graphicProps.insert(graphicProps.begin(), graphicProp);
			}
		
        styleInstance = styleInstance->parent();
	}
    return calc_graphic_properties_content(graphicProps);
}

graphic_format_properties calc_graphic_properties_content(const std::vector<const style_instance *> & styleInstances)
{
    graphic_format_properties result;

 	for (size_t i = 0; i < styleInstances.size(); i++)
	{
		graphic_format_properties f = calc_graphic_properties_content(styleInstances[i]);
        result.apply_from(&f);
    }
    return result;
}

////

paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_paragraph_properties*> & parProps)
{
    paragraph_format_properties result;

	for (size_t i = 0; i < parProps.size(); i++)
    {
        if (parProps[i])
            result.apply_from(parProps[i]->content_);
    }
    return result;
}

paragraph_format_properties calc_paragraph_properties_content(const style_instance * styleInstance)
{
    std::vector<const style_paragraph_properties*> parProps;
    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_paragraph_properties * parProp = content->get_style_paragraph_properties())
			{
                parProps.insert(parProps.begin(), parProp);
			}
        styleInstance = styleInstance->parent();
    }
    return calc_paragraph_properties_content(parProps);
}

paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_instance *> & styleInstances)
{
    paragraph_format_properties result;
	for (size_t i = 0; i < styleInstances.size(); i++)
    {
        result.apply_from(calc_paragraph_properties_content(styleInstances[i]));
    }
    return result;
}
_CP_OPT(int) calc_outline_level(_CP_OPT(int) value, const style_instance * styleInstance)
{
	if (value) return value;

	while (styleInstance)
    {
        if (styleInstance->outline_level())
		{
			return styleInstance->outline_level();
		}

        styleInstance = styleInstance->parent();
    }

	return boost::none;
}

void calc_tab_stops(const style_instance * styleInstance, oox::tabs_context & context)
{
    std::vector<const style_paragraph_properties*> parProps;
    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_paragraph_properties * parProp = content->get_style_paragraph_properties())
			{
                parProps.insert(parProps.begin(), parProp);
			}
        styleInstance = styleInstance->parent();
    }
	double margin_left = 0;

	for (size_t i = 0; i < parProps.size(); i++)
	{
		if (parProps[i]->content_.fo_margin_left_)
			margin_left = 20.0 * parProps[i]->content_.fo_margin_left_->get_length().get_value_unit(odf_types::length::pt);
		
		if (parProps[i]->content_.style_tab_stops_)
		{
			style_tab_stops *tab_stops = dynamic_cast<style_tab_stops*>(parProps[i]->content_.style_tab_stops_.get());
			context.reset();
			for (size_t j = 0; j < tab_stops->content_.size(); j++)
			{
				context.add(tab_stops->content_[j], margin_left);
			}
		}
	}
}


//////

style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_table_cell_properties*> & props)
{
    style_table_cell_properties_attlist result;
	for (size_t i = 0; i < props.size(); i++)
    {
        if (props[i])
			result.apply_from(props[i]->attlist_);
    }
    return result;
}

style_table_cell_properties_attlist calc_table_cell_properties(const style_instance * styleInstance)
{
    std::vector<const style_table_cell_properties*> props;
    while (styleInstance)
    {
        if (style_content * content = styleInstance->content())
            if (style_table_cell_properties * prop = content->get_style_table_cell_properties())
			{
                props.insert(props.begin(), prop);
			}
        styleInstance = styleInstance->parent();
    }
    return calc_table_cell_properties(props);        
}

style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_instance *> & styleInstances)
{
    style_table_cell_properties_attlist result;
	for (size_t i = 0; i < styleInstances.size(); i++)
    {
        result.apply_from(calc_table_cell_properties(styleInstances[i]));
    }
    return result;
}

}
}

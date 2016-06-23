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

#include "calcs_styles.h"
#include <boost/foreach.hpp>
#include <vector>
#include <algorithm>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

text_format_properties_content calc_text_properties_content(const std::vector<const style_text_properties*> & textProps)
{
    text_format_properties_content result;
    BOOST_FOREACH(const style_text_properties* v, textProps)
    {
        if (v)
            result.apply_from(v->content());
    }
    return result;
}

text_format_properties_content calc_text_properties_content(const style_instance * styleInstance)
{
    std::vector<const style_text_properties*> textProps;

    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_text_properties * textProp = content->get_style_text_properties())
                textProps.push_back(textProp);
		
        styleInstance = styleInstance->parent();
    }
    reverse(textProps.begin(), textProps.end());
    return calc_text_properties_content(textProps);
}

text_format_properties_content calc_text_properties_content(const std::vector<const style_instance *> & styleInstances)
{
    text_format_properties_content result;
    BOOST_FOREACH(const style_instance * inst, styleInstances)
    {
        result.apply_from(calc_text_properties_content(inst));
    }
    return result;
}

//////////////
graphic_format_properties calc_graphic_properties_content(const std::vector<const style_graphic_properties*> & graphicProps)
{
    graphic_format_properties result;
    BOOST_FOREACH(const style_graphic_properties* v, graphicProps)
    {
        if (v)
            result.apply_from(v->content());
    }
    return result;
}

graphic_format_properties calc_graphic_properties_content(const style_instance * styleInstance)
{
    std::vector<const style_graphic_properties*> graphicProps;
    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_graphic_properties * graphicProp = content->get_style_graphic_properties())
                graphicProps.push_back(graphicProp);
		
        styleInstance = styleInstance->parent();
	}
    reverse(graphicProps.begin(), graphicProps.end());
    return calc_graphic_properties_content(graphicProps);
}

graphic_format_properties calc_graphic_properties_content(const std::vector<const style_instance *> & styleInstances)
{
    graphic_format_properties result;
    BOOST_FOREACH(const style_instance * inst, styleInstances)
    {
        result.apply_from(calc_graphic_properties_content(inst));
    }
    return result;
}

////

paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_paragraph_properties*> & parProps)
{
    paragraph_format_properties result;
    BOOST_FOREACH(const style_paragraph_properties* v, parProps)
    {
        if (v)
            result.apply_from(v->content());
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
                parProps.push_back(parProp);
        styleInstance = styleInstance->parent();
    }
    reverse(parProps.begin(), parProps.end());
    return calc_paragraph_properties_content(parProps);
}

paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_instance *> & styleInstances)
{
    paragraph_format_properties result;
    BOOST_FOREACH(const style_instance * inst, styleInstances)
    {
        result.apply_from(calc_paragraph_properties_content(inst));
    }
    return result;
}

//////

style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_table_cell_properties*> & props)
{
    style_table_cell_properties_attlist result;
    BOOST_FOREACH(const style_table_cell_properties* v, props)
    {
        if (v)
            result.apply_from(v->style_table_cell_properties_attlist_);
    }
    return result;
}

style_table_cell_properties_attlist calc_table_cell_properties(const style_instance * styleInstance)
{
    std::vector<const style_table_cell_properties*> props;
    while (styleInstance)
    {
        if (const style_content * content = styleInstance->content())
            if (const style_table_cell_properties * prop = content->get_style_table_cell_properties())
                props.push_back(prop);
        styleInstance = styleInstance->parent();
    }
    reverse(props.begin(), props.end());
    return calc_table_cell_properties(props);        
}

style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_instance *> & styleInstances)
{
    style_table_cell_properties_attlist result;
    BOOST_FOREACH(const style_instance * inst, styleInstances)
    {
        result.apply_from(calc_table_cell_properties(inst));
    }
    return result;
}

}
}

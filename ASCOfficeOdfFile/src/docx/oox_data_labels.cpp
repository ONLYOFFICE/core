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


#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_data_labels.h"
#include "oox_chart_shape.h"

#include <boost/foreach.hpp>

namespace cpdoccore {
namespace oox {

oox_data_labels::oox_data_labels()//подписи на значениях
{
	showBubbleSize_		= false;
	showCatName_		= false;
	showLeaderLines_	= false;
	showLegendKey_		= false;
	showPercent_		= false;
	showSerName_		= false;
	showVal_			= false;
}

void oox_data_labels::set_common_dLbl ( std::vector<odf_reader::_property> & text_properties)
{
	textPr_ = text_properties;
}

void oox_data_labels::add_dLbl(int ind, std::vector<odf_reader::_property> & text_properties)
{
	dLbls_.insert(std::pair<int, std::vector<odf_reader::_property>>(ind, text_properties));
}

void oox_data_labels::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:dLbls")
        {
			oox_serialize_default_text(CP_XML_STREAM(), textPr_);
	
			for (std::map<int, std::vector<odf_reader::_property>>::iterator it = dLbls_.begin(); it != dLbls_.end(); it++)
			{
				CP_XML_NODE(L"c:dLbl")
				{
					CP_XML_NODE(L"c:idx")
					{
						CP_XML_ATTR(L"val", it->first);
					}
					oox_serialize_default_text(CP_XML_STREAM(), it->second);

					CP_XML_NODE(L"c:showLegendKey")
					{
						CP_XML_ATTR(L"val", showLegendKey_);
					}
					CP_XML_NODE(L"c:showVal")
					{
						CP_XML_ATTR(L"val", showVal_);
					}
					CP_XML_NODE(L"c:showCatName")
					{
						CP_XML_ATTR(L"val", showCatName_);
					}
					CP_XML_NODE(L"c:showSerName")
					{
						CP_XML_ATTR(L"val", showSerName_);
					}
					CP_XML_NODE(L"c:showPercent")
					{
						CP_XML_ATTR(L"val", showPercent_);
					}
					CP_XML_NODE(L"c:showBubbleSize")
					{
						CP_XML_ATTR(L"val", showBubbleSize_);
					}
				}
			}

			CP_XML_NODE(L"c:showLegendKey")
			{
				CP_XML_ATTR(L"val", showLegendKey_);
			}
			CP_XML_NODE(L"c:showVal")
			{
				CP_XML_ATTR(L"val", showVal_);
			}
			CP_XML_NODE(L"c:showCatName")
			{
				CP_XML_ATTR(L"val", showCatName_);
			}
			CP_XML_NODE(L"c:showSerName")
			{
				CP_XML_ATTR(L"val", showSerName_);
			}
			CP_XML_NODE(L"c:showPercent")
			{
				CP_XML_ATTR(L"val", showPercent_);
			}
			CP_XML_NODE(L"c:showBubbleSize")
			{
				CP_XML_ATTR(L"val", showBubbleSize_);
			}
		}

    }
}
}
}

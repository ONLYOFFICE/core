

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

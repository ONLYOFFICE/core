#include "../odf/precompiled_cpodf.h"

#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_data_labels.h"
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace oox {

oox_data_labels::oox_data_labels()//подписи на значени€х
{
	//пол€ какие писать у значений
	showBubbleSize_ = false;
	showCatName_ = false;
	showLeaderLines_ = false;
	showLegendKey_ = false;
	showPercent_ = false;
	showSerName_ = false;
	showVal_ = false;
}
void oox_data_labels::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:dLbls")
        {
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

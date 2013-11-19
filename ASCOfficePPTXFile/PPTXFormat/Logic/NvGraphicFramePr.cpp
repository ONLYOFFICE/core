#include "NvGraphicFramePr.h"

namespace PPTX
{
	namespace Logic
	{

		NvGraphicFramePr::NvGraphicFramePr()
		{
		}


		NvGraphicFramePr::~NvGraphicFramePr()
		{
		}
	

		NvGraphicFramePr::NvGraphicFramePr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const NvGraphicFramePr& NvGraphicFramePr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		

		

	} // namespace Logic
} // namespace PPTX
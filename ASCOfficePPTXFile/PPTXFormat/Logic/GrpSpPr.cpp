#include "GrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		GrpSpPr::GrpSpPr()
		{
		}


		GrpSpPr::~GrpSpPr()
		{
		}
	

		GrpSpPr::GrpSpPr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const GrpSpPr& GrpSpPr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		
		

	} // namespace Logic
} // namespace PPTX
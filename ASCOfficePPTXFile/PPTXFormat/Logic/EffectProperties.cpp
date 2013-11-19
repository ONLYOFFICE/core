#include "EffectProperties.h"
#include "EffectLst.h"
#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{

		EffectProperties::EffectProperties()
		{
		}


		EffectProperties::~EffectProperties()
		{
		}
	

		EffectProperties::EffectProperties(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const EffectProperties& EffectProperties::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		

	} // namespace Logic
} // namespace PPTX
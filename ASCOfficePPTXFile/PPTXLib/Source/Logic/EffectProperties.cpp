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
	

		EffectProperties::EffectProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectProperties& EffectProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EffectProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "effectLst")
				List.reset(new Logic::EffectLst(node));
			else if(element->XName == "effectDag")
				List.reset(new Logic::EffectDag(node));
			else List.reset();
		}

		void EffectProperties::GetEffectListFrom(const XML::XElement& element){
			if(element.element("effectLst").exist())
				List.reset(new Logic::EffectLst(element.element("effectLst")));
			else if(element.element("effectDag").exist())
				List.reset(new Logic::EffectDag(element.element("effectDag")));
			else List.reset();
		}

		const XML::XNode EffectProperties::toXML() const
		{
			return XML::Write(List);
		}

	} // namespace Logic
} // namespace PPTX
#include "AttrName.h"

namespace PPTX
{
	namespace Logic
	{

		AttrName::AttrName()
		{
		}

		AttrName::~AttrName()
		{
		}

		AttrName::AttrName(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AttrName& AttrName::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AttrName::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			text = element.text();
		}

		const XML::XNode AttrName::toXML() const
		{
			return XML::XElement(ns.p + "attrName",
					XML::XText(text)
				);
		}
	} // namespace Logic
} // namespace PPTX
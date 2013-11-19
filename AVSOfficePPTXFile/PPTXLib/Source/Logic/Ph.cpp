#include "Ph.h"

namespace PPTX
{
	namespace Logic
	{
		Ph::Ph()
		{
		}

		Ph::~Ph()
		{
		}

		Ph::Ph(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Ph& Ph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Ph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			hasCustomPrompt = element.attribute("hasCustomPrompt").value();
			idx = element.attribute("idx").value();
			orient = element.attribute("orient").value();
			sz = element.attribute("sz").value();
			type = element.attribute("type").value();
		}

		const XML::XNode Ph::toXML() const
		{
			return XML::XElement(ns.p + "ph",
				XML::XAttribute("type", type) +
				XML::XAttribute("orient", orient) +
				XML::XAttribute("sz", sz) +
				XML::XAttribute("idx", idx) +
				XML::XAttribute("hasCustomPrompt", hasCustomPrompt)
			);
		}

	} // namespace Logic
} // namespace PPTX
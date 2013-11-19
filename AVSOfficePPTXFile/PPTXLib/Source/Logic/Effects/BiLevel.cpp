#include "BiLevel.h"

namespace PPTX
{
	namespace Logic
	{

		BiLevel::BiLevel()
		{
		}

		BiLevel::~BiLevel()
		{
		}

		BiLevel::BiLevel(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BiLevel& BiLevel::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BiLevel::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			thresh = element.attribute("thresh").value();
		}

		const XML::XNode BiLevel::toXML() const
		{
			return XML::XElement(ns.a + "biLevel",
				XML::XAttribute("thresh", thresh)
				);
		}
	} // namespace Logic
} // namespace PPTX
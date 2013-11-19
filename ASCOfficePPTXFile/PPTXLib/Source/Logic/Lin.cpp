#include "Lin.h"

namespace PPTX
{
	namespace Logic
	{

		Lin::Lin()
		{
		}


		Lin::~Lin()
		{
		}


		Lin::Lin(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Lin& Lin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Lin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			ang = element.attribute("ang").value();
			scaled = element.attribute("scaled").value();
		}


		const XML::XNode Lin::toXML() const
		{
			return XML::XElement(ns.a + "lin",
					XML::XAttribute("ang", ang) +
					XML::XAttribute("scaled", scaled)
				);
		}

	} // namespace Logic
} // namespace PPTX
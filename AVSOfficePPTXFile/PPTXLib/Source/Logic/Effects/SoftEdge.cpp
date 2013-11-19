#include "SoftEdge.h"

namespace PPTX
{
	namespace Logic
	{

		SoftEdge::SoftEdge()
		{
		}

		SoftEdge::~SoftEdge()
		{
		}

		SoftEdge::SoftEdge(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SoftEdge& SoftEdge::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SoftEdge::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			rad = element.attribute("rad").value();
		}


		const XML::XNode SoftEdge::toXML() const
		{
			return XML::XElement(ns.a + "softEdge",
					XML::XAttribute("rad", rad)
				);
		}
	} // namespace Logic
} // namespace PPTX
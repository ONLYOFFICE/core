#include "AlphaBiLevel.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaBiLevel::AlphaBiLevel()
		{
		}

		AlphaBiLevel::~AlphaBiLevel()
		{
		}

		AlphaBiLevel::AlphaBiLevel(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaBiLevel& AlphaBiLevel::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaBiLevel::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			thresh = element.attribute("thresh").value();
		}

		const XML::XNode AlphaBiLevel::toXML() const
		{
			return XML::XElement(ns.a + "alphaBiLevel",
				XML::XAttribute("thresh", thresh)
				);
		}
	} // namespace Logic
} // namespace PPTX
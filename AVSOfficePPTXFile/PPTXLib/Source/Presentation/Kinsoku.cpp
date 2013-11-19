#include "Kinsoku.h"

namespace PPTX
{
	namespace nsPresentation
	{
		Kinsoku::Kinsoku()
		{
		}

		Kinsoku::~Kinsoku()
		{
		}

		Kinsoku::Kinsoku(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Kinsoku& Kinsoku::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Kinsoku::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			invalEndChars = element.attribute("invalEndChars").value();
			invalStChars = element.attribute("invalStChars").value();
			lang = element.attribute("lang").value();
		}

		const XML::XNode Kinsoku::toXML() const
		{
			return	XML::XElement(ns.p + "kinsoku",
				XML::XAttribute("invalEndChars", invalEndChars) +
				XML::XAttribute("invalStChars", invalStChars) +
				XML::XAttribute("lang", lang)
			);
		}
	} // namespace nsPresentation
} // namespace PPTX
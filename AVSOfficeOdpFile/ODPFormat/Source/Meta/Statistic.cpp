#include "Statistic.h"

namespace Odp
{
	namespace Meta
	{
		Statistic::Statistic()
		{
		}
		
		Statistic::~Statistic()
		{
		}

		Statistic::Statistic(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Statistic& Statistic::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Statistic::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			TableCount	=	element.attribute("table-count").value();
			ImageCount	=	element.attribute("image-count").value();
			ObjectCount =	element.attribute("object-count").value();
			PageCount		=	element.attribute("page-count").value();
			ParagraphCount	=	element.attribute("paragraph-count").value();
			WordCount				=	element.attribute("word-count").value();
			CharacterCount	=	element.attribute("character-count").value();
		}

		const XML::XNode Statistic::toXML() const
		{
			return
				XML::XElement(ns.meta + "document-statistic",
					XML::XAttribute(ns.meta + "table-count", TableCount) +
					XML::XAttribute(ns.meta + "image-count", ImageCount) +
					XML::XAttribute(ns.meta + "object-count", ObjectCount) +
					XML::XAttribute(ns.meta + "page-count", PageCount) +
					XML::XAttribute(ns.meta + "paragraph-count", ParagraphCount) +
					XML::XAttribute(ns.meta + "word-count", WordCount) +
					XML::XAttribute(ns.meta + "character-count", CharacterCount)
				);
		}
	} // namespace Meta
} // namespace Odp

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Hyperlink.h"


namespace OOX
{
	namespace Logic
	{

		Hyperlink::Hyperlink()
		{
		}


		Hyperlink::~Hyperlink()
		{
		}


		Hyperlink::Hyperlink(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Hyperlink& Hyperlink::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void Hyperlink::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.attribute("id").exist())
				rId		= element.attribute("id").value().ToString();
			Anchor  = element.attribute("anchor").value();
			History = element.attribute("history").value();
			XML::Fill(Runs, element, "r");
		}


		const XML::XNode Hyperlink::toXML() const
		{
			return
				XML::XElement(ns.w + "hyperlink",
					XML::XAttribute(ns.r + "id", rId) +
					XML::XAttribute(ns.w + "anchor", Anchor) +
					XML::XAttribute(ns.w + "history", History) +
					XML::Write(Runs)
				);
		}


		const std::string Hyperlink::toTxt() const
		{
			std::string text;
			BOOST_FOREACH(const Run& run, *Runs)
				text += run.toTxt();
			return text;
		}

	} // namespace Logic
} // namespace OOX
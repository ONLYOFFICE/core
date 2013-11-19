
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Section.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			Section::Section()
			{
			}


			Section::~Section()
			{
			}


			Section::Section(const XML::XNode& node)
			{
				fromXML(node);
			}


			const Section& Section::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void Section::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);
				Columns = element.element("columns");
				Editable = element.attribute("editable").value();
				DontBalanceTextColumns = element.attribute("dont-balance-text-columns").value();
				if(element.attribute("margin-left").exist())
					MarginLeft = element.attribute("margin-left").value().ToString();
				if(element.attribute("margin-right").exist())
					MarginRight = element.attribute("margin-right").value().ToString();
				BackgroundColor = element.attribute("background-color").value();
				Image = element.element("background-image");
			}


			const XML::XNode Section::toXML() const
			{
				return 
					XML::XElement(ns.style + "section-properties",
						XML::XAttribute(ns.style + "editable", Editable) + 
						XML::XAttribute(ns.text + "dont-balance-text-columns", DontBalanceTextColumns) +
						XML::XAttribute(ns.fo + "margin-left", MarginLeft) +
						XML::XAttribute(ns.fo + "margin-right", MarginRight) +
						XML::XAttribute(ns.fo + "background-color", BackgroundColor) +
						XML::Write(Columns) +
						XML::Write(Image)
					);
			}


			const bool Section::isSimple() const
			{
				return true;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt
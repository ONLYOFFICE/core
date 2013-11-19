
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Columns.h"


namespace Odt
{
	namespace Styles
	{

		Columns::Columns()
		{
		}


		Columns::~Columns()
		{
		}


		Columns::Columns(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Columns& Columns::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Columns::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Count			= element.attribute("column-count").value();
			Separator	= element.element("column-sep");
			if(element.attribute("column-gap").exist())
				Gap = element.attribute("column-gap").value().ToString();
			Fill(Items, element, "column");
		}


		const XML::XNode Columns::toXML() const
		{
			return 
				XML::XElement(ns.style + "columns",
					XML::XAttribute(ns.fo + "column-count", Count) +
					XML::XAttribute(ns.fo + "column-gap", Gap) +
					Write(Separator) +
					Write(Items)
				);
		}

	} // namespace Styles
} // namespace Odt
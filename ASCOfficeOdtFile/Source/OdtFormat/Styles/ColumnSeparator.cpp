
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ColumnSeparator.h"


namespace Odt
{
	namespace Styles
	{

		ColumnSeparator::ColumnSeparator()
		{
		}


		ColumnSeparator::~ColumnSeparator()
		{
		}


		ColumnSeparator::ColumnSeparator(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ColumnSeparator& ColumnSeparator::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ColumnSeparator::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width = element.attribute("width").value().ToString();
			Color = element.attribute("color").value();
			Height = element.attribute("height").value();
		}


		const XML::XNode ColumnSeparator::toXML() const
		{
			return 
				XML::XElement(ns.style + "column-sep",
					XML::XAttribute(ns.style + "width", Width) + 
					XML::XAttribute(ns.style + "color", Color) +
					XML::XAttribute(ns.style + "height", Height)
				);
		}

	} // namespace Styles
} // namespace Odt
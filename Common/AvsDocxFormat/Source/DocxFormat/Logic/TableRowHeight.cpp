
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableRowHeight.h" 


namespace OOX
{
	namespace Logic
	{

		TableRowHeight::TableRowHeight()
		{
		}


		TableRowHeight::~TableRowHeight()
		{
		}
	

		TableRowHeight::TableRowHeight(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableRowHeight& TableRowHeight::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableRowHeight::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Height = element.attribute("val").value().ToString();
			Type	= element.attribute("hRule").value();
		}


		const XML::XNode TableRowHeight::toXML() const
		{
			return 
				XML::XElement(ns.w + "trHeight",
					XML::XAttribute(ns.w + "val", Height) + 
					XML::XAttribute(ns.w + "hRule", Type)
				);
		}

	} // namespace Logic
} // namespace OOX

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableRowProperties.h" 


namespace OOX
{
	namespace Logic
	{

		TableRowProperties::TableRowProperties()
		{
		}


		TableRowProperties::~TableRowProperties()
		{
		}
	

		TableRowProperties::TableRowProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableRowProperties& TableRowProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableRowProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Height	= element.element("trHeight");
			
		}


		const XML::XNode TableRowProperties::toXML() const
		{
			return
				XML::XElement(ns.w + "trPr", 
					XML::Write(Height)
				);
		}

	} // namespace Logic
} // namespace OOX
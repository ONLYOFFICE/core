
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableWidth.h" 


namespace OOX
{
	namespace Logic
	{

		TableWidth::TableWidth()
		{
		}


		TableWidth::~TableWidth()
		{
		}
	

		TableWidth::TableWidth(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableWidth& TableWidth::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableWidth::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width	= element.attribute("w").value().ToString();
			Type	= element.attribute("type").value();
		}


		const XML::XNode TableWidth::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
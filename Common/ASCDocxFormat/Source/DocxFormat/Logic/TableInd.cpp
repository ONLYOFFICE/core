
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableInd.h"


namespace OOX
{
	namespace Logic
	{

		TableInd::TableInd()
		{
		}


		TableInd::~TableInd()
		{
		}
	

		TableInd::TableInd(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableInd& TableInd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableInd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width	=	element.attribute("w").value();
			Type	=	element.attribute("type").value();
		}


		const XML::XNode TableInd::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "GridColumn.h" 

namespace OOX
{
	namespace Logic
	{
		GridColumn::GridColumn()
		{

		}

		GridColumn::~GridColumn()
		{

		}	

		GridColumn::GridColumn(const XML::XNode& node)
		{
			fromXML(node);
		}

		const GridColumn& GridColumn::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void GridColumn::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width	=	element.attribute("w").value().ToString();
		}

		const XML::XNode GridColumn::toXML() const
		{
			return XML::XElement(ns.w + "gridCol", XML::XAttribute(ns.w + "w", Width));
		}

	} // namespace Logic
} // namespace OOX
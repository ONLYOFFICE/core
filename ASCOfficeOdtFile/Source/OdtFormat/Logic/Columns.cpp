
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Columns.h"


namespace Odt
{
	namespace Logic
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
			ColumnCount	=	element.attribute("column-count").value();
			ColumnGap		=	element.attribute("column-gap").value().ToString();
		}


		const XML::XNode Columns::toXML() const
		{
			return
				XML::XElement(ns.style + "columns",
					XML::XAttribute(ns.fo + "column-count", ColumnCount) +
					XML::XAttribute(ns.fo + "column-gap", ColumnGap)
				);
		}

	} // namespace Logic
} // namespace Odt
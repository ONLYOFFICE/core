#include "TableCol.h"

namespace PPTX
{
	namespace Logic
	{
		TableCol::TableCol()
		{
		}

		TableCol::~TableCol()
		{
		}

		TableCol::TableCol(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableCol& TableCol::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCol::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width = element.attribute("w").value();
		}

		const XML::XNode TableCol::toXML() const
		{
			return XML::XElement(ns.a + "gridCol",
					XML::XAttribute("w", Width)
				);
		}
	} // namespace Logic
} // namespace PPTX
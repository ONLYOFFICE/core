#include "TableRow.h"

namespace PPTX
{
	namespace Logic
	{
		TableRow::TableRow()
		{
		}

		TableRow::~TableRow()
		{
		}

		TableRow::TableRow(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableRow& TableRow::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableRow::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Height = element.attribute("h").value();
			XML::Fill(Cells, element, "tc");
		}

		const XML::XNode TableRow::toXML() const
		{
			return XML::XElement(ns.a + "tr",
					XML::XAttribute("h", Height) +
					XML::Write(Cells)
				);
		}

		void TableRow::FillParentPointersForChilds()
		{
			for(std::vector<TableCell>::iterator i = Cells->begin(); i != Cells->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
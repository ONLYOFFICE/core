#include "Table.h"

namespace PPTX
{
	namespace Logic
	{
		Table::Table()
		{
		}

		Table::~Table()
		{
		}

		Table::Table(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Table& Table::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Table::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(TableCols, element.element("tblGrid"), "gridCol");
			XML::Fill(TableRows, element, "tr");
			TableProperties = element.element("tblPr");

			FillParentPointersForChilds();
		}

		const XML::XNode Table::toXML() const
		{
			return XML::XElement(ns.a + "tbl",
					XML::Write(TableProperties) +
					XML::XElement(ns.a + "tblGrid", XML::Write(TableCols)) +
					XML::Write(TableRows)
				);
		}

		void Table::FillParentPointersForChilds()
		{
			if(TableProperties.is_init())
				TableProperties->SetParentPointer(*this);
			for(std::vector<TableRow>::iterator i = TableRows->begin(); i != TableRows->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
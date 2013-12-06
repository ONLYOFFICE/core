
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableRow.h"
#include "Exception/log_runtime_error.h"

namespace OOX
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
			
			Properties	= element.element("trPr");
			int numCol = 0;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				XML::XElement element(*i);
				if (element.XName == "tc")
				{
					TableCell cell(*i);
					cell.NumCol = numCol;
					Cells->push_back(cell);

					int count = cell.Properties->GridSpan.get_value_or(1);

					// TODO, убрать цикл, поставить вместо арифметическое действие
					while(count-- > 0)
						numCol++;
				}
				else if (element.XName == "sdt")
				{
					if(element.element("sdtContent").exist())
					{
						if(element.element("sdtContent").element("tc").exist())
						{
							//TableCell cell(*i);
							TableCell cell(element.element("sdtContent").element("tc"));
							cell.NumCol = numCol;
							Cells->push_back(cell);

							int count = cell.Properties->GridSpan.get_value_or(1);

							// TODO, убрать цикл, поставить вместо арифметическое действие
							while(count-- > 0)
								numCol++;
						}
					}
				}
			}
		}

		const XML::XNode TableRow::toXML() const
		{
			return XML::XElement(ns.w + "tr", XML::Write(Properties) + XML::Write(Cells));
		}

		const TableCell TableRow::getCellByNumCol(const int num) const
		{
			const std::vector<TableCell>& runs = Cells.get();

			for (std::vector<TableCell>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
			{
				if ((*iter).NumCol == num)
					return (*iter);
			}

			throw log_runtime_error("bad num column");
		}

		const bool TableRow::isCellByNumCol(const int num) const
		{
			const std::vector<TableCell>& runs = Cells.get();

			for (std::vector<TableCell>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
			{
				if ((*iter).NumCol == num)
					return true;
			}
			
			return false;
		}

	} // namespace Logic
} // namespace OOX
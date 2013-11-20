
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Table.h"

namespace OOX
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
			
			tblPr	=	element.element("tblPr");
			tblGrid	=	element.element("tblGrid");

			XML::Fill(Rows, element, "tr");
			SetCountsVMerged();
		}

		const XML::XNode Table::toXML() const
		{
			return XML::XElement(ns.w + "tbl",	XML::Write(tblPr) +	XML::Write(tblGrid) + XML::Write(Rows));
		}

		void Table::SetCountsVMerged()
		{
			for (size_t j = 0; j != Rows->size(); ++j)
			{
				for (size_t i = 0; i !=  Rows->at(j).Cells->size(); ++i)
				{
					if (Rows->at(j).Cells->at(i).isFirstFromVMerged())
					{
						TableCell cell = Rows->at(j).Cells->at(i);
						for (size_t k = j + 1; k !=  Rows->size(); ++k)
						{
							if (Rows->at(k).isCellByNumCol(cell.NumCol) && Rows->at(k).getCellByNumCol(cell.NumCol).isFromVMergedButNotFirst())
								++(*cell.Properties->CountVMerge);
							else
								break;
						}
						Rows->at(j).Cells->at(i) = cell;
					}
				}
			}
		}

	} // namespace Logic
} // namespace OOX
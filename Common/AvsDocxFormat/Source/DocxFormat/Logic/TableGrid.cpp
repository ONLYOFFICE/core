
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableGrid.h" 

namespace OOX
{
	namespace Logic
	{
		TableGrid::TableGrid()
		{

		}

		TableGrid::~TableGrid()
		{

		}

		TableGrid::TableGrid(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableGrid& TableGrid::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableGrid::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(Columns, element);
		}

		const XML::XNode TableGrid::toXML() const
		{
			return XML::XElement(ns.w + "tblGrid", XML::Write(Columns));
		}

	} // namespace Logic
} // namespace OOX
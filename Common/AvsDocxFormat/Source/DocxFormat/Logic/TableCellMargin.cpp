
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCellMargin.h"


namespace OOX
{
	namespace Logic
	{

		TableCellMargin::TableCellMargin()
		{
		}


		TableCellMargin::~TableCellMargin()
		{
		}
	

		TableCellMargin::TableCellMargin(const XML::XNode& node)
		{
			fromXML(node);
		}


		TableCellMargin::TableCellMargin(const std::string& name, const CellMarginItem& margin)
			: Name(name),
			  Margin(margin)
		{
		}


		const TableCellMargin& TableCellMargin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCellMargin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name							 = element.XName->Name; 
			Margin->W		 = element.attribute("w").value();
			Margin->Type	 = element.attribute("type").value();
		}


		const XML::XNode TableCellMargin::toXML() const
		{
			return 
				XML::XElement(ns.w + Name.ToString(),
					XML::XAttribute(ns.w + "w", Margin->W) +
					XML::XAttribute(ns.w + "type", Margin->Type)
				);
		}

	} // namespace Logic
} // namespace OOX
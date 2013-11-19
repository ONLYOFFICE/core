#include "TableCell.h"
#include "../../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		TableCell::TableCell()
		{
		}

		TableCell::~TableCell()
		{
		}

		TableCell::TableCell(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableCell& TableCell::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCell::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			TxBody = element.element("txBody");
			CellProperties = element.element("tcPr");
			RowSpan = element.attribute("rowSpan").value();
			GridSpan = element.attribute("gridSpan").value();
			HMerge = element.attribute("hMerge").value();
			VMerge = element.attribute("vMerge").value();
			Id = element.attribute("id").value();

			FillParentPointersForChilds();
		}

		const XML::XNode TableCell::toXML() const
		{
			return XML::XElement(ns.a + "tc",
				XML::Write(TxBody) +
				XML::Write(CellProperties) +
				XML::XAttribute("rowSpan", RowSpan) +
				XML::XAttribute("gridSpan", GridSpan) +
				XML::XAttribute("hMerge", HMerge) +
				XML::XAttribute("vMerge", VMerge) +
				XML::XAttribute("id", Id)
				);
		}

		void TableCell::FillParentPointersForChilds()
		{
			if(TxBody.is_init())
				TxBody->SetParentPointer(*this);
			if(CellProperties.is_init())
				CellProperties->SetParentPointer(*this);
		}

		void TableCell::GetShapeProperties(ShapeProperties& props)const
		{
			if(parentFileIs<Slide>())
			{
				parentFileAs<Slide>().FillShapeProperties(props, "table-cell");
				props.FillFromTextBody(TxBody);

				//props.SetParentFilePointer(parentFile);
			}
		}

	} // namespace Logic
} // namespace PPTX
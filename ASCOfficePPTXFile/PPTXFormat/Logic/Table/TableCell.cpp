//#include "./stdafx.h"

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

		TableCell::TableCell(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const TableCell& TableCell::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCell::fromXML(XmlUtils::CXmlNode& node)
		{
            txBody			= node.ReadNodeNoNS(_T("txBody"));
			CellProperties	= node.ReadNode(_T("a:tcPr"));
			node.ReadAttributeBase(L"rowSpan", RowSpan);
			node.ReadAttributeBase(L"gridSpan", GridSpan);
			node.ReadAttributeBase(L"hMerge", HMerge);
			node.ReadAttributeBase(L"vMerge", VMerge);
			node.ReadAttributeBase(L"id", Id);

			FillParentPointersForChilds();
		}

		CString TableCell::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("rowSpan"), RowSpan);
			oAttr.Write(_T("gridSpan"), GridSpan);
			oAttr.Write(_T("hMerge"), HMerge);
			oAttr.Write(_T("vMerge"), VMerge);
			oAttr.Write(_T("id"), Id);

			XmlUtils::CNodeValue oValue;
            oValue.WriteNullable(txBody);
			oValue.WriteNullable(CellProperties);

			return XmlUtils::CreateNode(_T("a:tc"), oAttr, oValue);
		}

		void TableCell::FillParentPointersForChilds()
		{
            if(txBody.IsInit())
                txBody->SetParentPointer(this);
			if(CellProperties.IsInit())
				CellProperties->SetParentPointer(this);
		}

		void TableCell::GetShapeProperties(ShapeProperties& props)const
		{
			if(parentFileIs<Slide>())
			{
				parentFileAs<Slide>().FillShapeProperties(props, _T("table-cell"));
                props.FillFromTextBody(txBody);

				//props.SetParentFilePointer(parentFile);
			}
		}

	} // namespace Logic
} // namespace PPTX

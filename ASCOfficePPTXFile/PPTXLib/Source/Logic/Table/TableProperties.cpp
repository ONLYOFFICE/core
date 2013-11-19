#include "TableProperties.h"

namespace PPTX
{
	namespace Logic
	{
		TableProperties::TableProperties()
		{
		}

		TableProperties::~TableProperties()
		{
		}

		TableProperties::TableProperties(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableProperties& TableProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill.GetFillFrom(element);
			Effects.GetEffectListFrom(element);
			if(element.element("tableStyleId").exist())
				TableStyleId = element.element("tableStyleId").text();

			Rtl = element.attribute("rtl").value();
			FirstRow = element.attribute("firstRow").value();
			FirstCol = element.attribute("firstCol").value();
			LastRow = element.attribute("lastRow").value();
			LastCol = element.attribute("lastCol").value();
			BandRow = element.attribute("bandRow").value();
			BandCol = element.attribute("bandCol").value();

			FillParentPointersForChilds();
		}

		const XML::XNode TableProperties::toXML() const
		{
			return XML::XElement(ns.a + "tblPr",
					XML::Write(Fill) +
					XML::Write(Effects) +
					XML::WriteIf(XML::XElement(ns.a + "tableStyleId", XML::XText(TableStyleId)), TableStyleId.is_init()) +
					XML::XAttribute("rtl", Rtl) +
					XML::XAttribute("firstRow", FirstRow) +
					XML::XAttribute("firstCol", FirstCol) +
					XML::XAttribute("lastRow", LastRow) +
					XML::XAttribute("lastCol", LastCol) +
					XML::XAttribute("bandRow", BandRow) +
					XML::XAttribute("bandCol", BandCol)
				);
		}

		void TableProperties::FillParentPointersForChilds()
		{
			if(Fill.is_init())
				Fill.SetParentPointer(*this);
			if(Effects.is_init())
				Effects.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
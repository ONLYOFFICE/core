#include "TableCellProperties.h"

namespace PPTX
{
	namespace Logic
	{
		TableCellProperties::TableCellProperties()
		{
		}

		TableCellProperties::~TableCellProperties()
		{
		}

		TableCellProperties::TableCellProperties(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableCellProperties& TableCellProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCellProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			LnL = element.element("lnL");
			LnR = element.element("lnR");
			LnT = element.element("lnT");
			LnB = element.element("lnB");
			LnTlToBr = element.element("lnTlToBr");
			LnBlToTr = element.element("lnBlToTr");
			Cell3D = element.element("cell3D");
			Fill.GetFillFrom(element);
//			<xsd:element name="headers" type="CT_Headers" minOccurs="0"/> 

			MarL = element.attribute("marL").value();
			MarR = element.attribute("marR").value();
			MarT = element.attribute("marT").value();
			MarB = element.attribute("marB").value();
			Vert = element.attribute("vert").value();
			Anchor = element.attribute("anchor").value();
			AnchorCtr = element.attribute("anchorCtr").value();
			HorzOverflow = element.attribute("horzOverflow").value();

			FillParentPointersForChilds();
		}

		const XML::XNode TableCellProperties::toXML() const
		{
			return XML::XElement(ns.a + "tcPr",
				XML::Write(LnL) +
				XML::Write(LnR) +
				XML::Write(LnT) +
				XML::Write(LnB) +
				XML::Write(LnTlToBr) +
				XML::Write(LnBlToTr) +
				XML::Write(LnL) +
				XML::Write(Cell3D) +
				XML::Write(Fill) +
				XML::XAttribute("marL", MarL) +
				XML::XAttribute("marR", MarR) +
				XML::XAttribute("marT", MarT) +
				XML::XAttribute("marB", MarB) +
				XML::XAttribute("vert", Vert) +
				XML::XAttribute("anchor", Anchor) +
				XML::XAttribute("anchorCtr", AnchorCtr) +
				XML::XAttribute("horzOverflow", HorzOverflow)
				);
		}

		void TableCellProperties::FillParentPointersForChilds()
		{
			if(LnL.is_init())
				LnL->SetParentPointer(*this);
			if(LnR.is_init())
				LnR->SetParentPointer(*this);
			if(LnT.is_init())
				LnT->SetParentPointer(*this);
			if(LnB.is_init())
				LnB->SetParentPointer(*this);
			if(LnTlToBr.is_init())
				LnTlToBr->SetParentPointer(*this);
			if(LnBlToTr.is_init())
				LnBlToTr->SetParentPointer(*this);

			if(Cell3D.is_init())
				Cell3D->SetParentPointer(*this);
			if(Fill.is_init())
				Fill.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
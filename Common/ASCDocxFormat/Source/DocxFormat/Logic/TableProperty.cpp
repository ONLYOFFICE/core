
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableProperty.h"

namespace OOX
{
	namespace Logic
	{
		TableProperty::TableProperty()
		{
			tblStyleRowBandSize		=	1;
			tblStyleColBandSize		=	1;

			firstRow				=	0;
			lastRow					=	0;
			firstColumn				=	0;
			lastColumn				=	0;
			noHBand					=	0;
			noVBand					=	0;
		}

		TableProperty::~TableProperty()
		{

		}	

		TableProperty::TableProperty(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableProperty& TableProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Style					=	element.element("tblStyle").attribute("val").value();
			Width					=	element.element("tblW");
			Look					=	element.element("tblLook").attribute("val").value();
			Ind						=	element.element("tblInd");
			CellMar					=	element.element("tblCellMar");
			tblBorders				=	element.element("tblBorders");
			Layout					=	element.element("tblLayout").attribute("type").value();

			if (element.element("tblStyleRowBandSize").exist())
				tblStyleRowBandSize	=	element.element("tblStyleRowBandSize").attribute("val").value().ToInt();
			if (element.element("tblStyleColBandSize").exist())
				tblStyleColBandSize	=	element.element("tblStyleColBandSize").attribute("val").value().ToInt();

			if (element.element("tblLook").exist())
			{
				firstRow			=	element.element("tblLook").attribute("firstRow").value().ToInt();
				lastRow				=	element.element("tblLook").attribute("lastRow").value().ToInt();
				firstColumn			=	element.element("tblLook").attribute("firstColumn").value().ToInt();
				lastColumn			=	element.element("tblLook").attribute("lastColumn").value().ToInt();
				noHBand				=	element.element("tblLook").attribute("noHBand").value().ToInt();
				noVBand				=	element.element("tblLook").attribute("noVBand").value().ToInt();

				if (element.element("tblLook").attribute("val").exist())
				{
					std::string str	=	element.element("tblLook").attribute("val").value().ToString();

					int nVal		=	0;
					if (str.length())
					{
						nVal		=	HexString2Int (str);
					}
					
					firstRow		=	(0 != (nVal & 0x0020));
					lastRow			=	(0 != (nVal & 0x0040));
					firstColumn		=	(0 != (nVal & 0x0080));
					lastColumn		=	(0 != (nVal & 0x0100));
					noHBand			=	(0 != (nVal & 0x0200));
					noVBand			=	(0 != (nVal & 0x0400));
				}
			}
		}

		const XML::XNode TableProperty::toXML() const
		{
			return XML::XElement(ns.w + "tblPr",
				XML::Write(ns.w + "tblStyle", ns.w + "val", Style) + 
				XML::Write(Width) + 
				XML::Write(ns.w + "tblLayout", ns.w + "type", Layout) +
				XML::Write(ns.w + "tblLook", ns.w + "val", Look) +
				XML::Write(Ind) +
				XML::Write(CellMar) +
				XML::Write(tblBorders));
		}

	} // namespace Logic
} // namespace OOX
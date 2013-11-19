
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableCell.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
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
				VerticalAlign	= element.attribute("vertical-align").value();
				Padding			= element.attribute("padding").value();
				PaddingLeft		= element.attribute("padding-left").value();
				PaddingRight	= element.attribute("padding-right").value();
				PaddingTop		= element.attribute("padding-top").value();
				PaddingBottom	= element.attribute("padding-bottom").value();
				Border			= element.attribute("border").value();
				BorderLeft		= element.attribute("border-left").value();
				BorderRight		= element.attribute("border-right").value();
				BorderTop		= element.attribute("border-top").value();
				BorderBottom	= element.attribute("border-bottom").value();
				BorderLineWidthLeft		= element.attribute("border-line-width-left").value();
				BorderLineWidthRight	= element.attribute("border-line-width-right").value();
				BorderLineWidthTop		= element.attribute("border-line-width-top").value();
				BorderLineWidthBottom	= element.attribute("border-line-width-bottom").value();
				BackgroundColor			= element.attribute("background-color").value();
			}


			const XML::XNode TableCell::toXML() const
			{
				return 
					XML::XElement(ns.style + "table-cell-properties",
						XML::XAttribute(ns.style + "vertical-align", VerticalAlign) +
						XML::XAttribute(ns.fo + "padding", Padding) + 
						XML::XAttribute(ns.fo + "padding-left", PaddingLeft) + 
						XML::XAttribute(ns.fo + "padding-right", PaddingRight) + 
						XML::XAttribute(ns.fo + "padding-top", PaddingTop) + 
						XML::XAttribute(ns.fo + "padding-bottom", PaddingBottom) + 
						XML::XAttribute(ns.fo + "border", Border) +
						XML::XAttribute(ns.fo + "border-left", BorderLeft) +
						XML::XAttribute(ns.fo + "border-right", BorderRight) +
						XML::XAttribute(ns.fo + "border-top", BorderTop) + 
						XML::XAttribute(ns.fo + "border-bottom", BorderBottom) +
						XML::XAttribute(ns.fo + "border-line-width-left", BorderLineWidthLeft) +
						XML::XAttribute(ns.fo + "border-line-width-right", BorderLineWidthRight) +
						XML::XAttribute(ns.fo + "border-ine-width-top", BorderLineWidthTop) +
						XML::XAttribute(ns.fo + "border-line-width-bottom", BorderLineWidthBottom) +
						XML::XAttribute(ns.fo + "background-color", BackgroundColor)
					);
			}


			const TableCell TableCell::merge(const TableCell& prev, const TableCell& current)
			{
				TableCell properties;
				properties.VerticalAlign			= ::merge(prev.VerticalAlign, current.VerticalAlign);
				properties.Padding					= ::merge(prev.Padding, current.Padding);
				properties.PaddingLeft				= ::merge(prev.PaddingLeft, current.PaddingLeft);
				properties.PaddingRight				= ::merge(prev.PaddingRight, current.PaddingRight);
				properties.PaddingTop				= ::merge(prev.PaddingTop, current.PaddingTop);
				properties.PaddingBottom			= ::merge(prev.PaddingBottom, current.PaddingBottom);
				properties.Border					= ::merge(prev.Border, current.Border);
				properties.BorderLeft				= ::merge(prev.BorderLeft, current.BorderLeft);
				properties.BorderRight				= ::merge(prev.BorderRight, current.BorderRight);
				properties.BorderTop				= ::merge(prev.BorderTop, current.BorderTop);
				properties.BorderBottom				= ::merge(prev.BorderBottom, current.BorderBottom);
				properties.BorderLineWidthLeft		= ::merge(prev.BorderLineWidthLeft, current.BorderLineWidthLeft);
				properties.BorderLineWidthRight		= ::merge(prev.BorderLineWidthRight, current.BorderLineWidthRight);
				properties.BorderLineWidthTop		= ::merge(prev.BorderLineWidthTop, current.BorderLineWidthTop);
				properties.BorderLineWidthBottom	= ::merge(prev.BorderLineWidthBottom, current.BorderLineWidthBottom);
				return properties;
			}


			const bool TableCell::isSimple() const
			{
				if (Border.is_init() || BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init() || BackgroundColor.is_init())
					return false;
				return true;
			}


			const bool TableCell::onlyBorder() const
			{
				return !(BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init());
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt
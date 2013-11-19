
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Table.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
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
				BorderModel = element.attribute("border-model").value();
				if(element.attribute("width").exist())
					Width		= element.attribute("width").value().ToString();

				Align		= element.attribute("align").value();
				PageNumber	= element.attribute("page-number").value();

				if(element.attribute("margin-left").exist())
					MarginLeft	= element.attribute("margin-left").value().ToString();
				if(element.attribute("margin-right").exist())
					MarginRight	= element.attribute("margin-right").value().ToString();

				KeepWithNext= element.attribute("keep-with-next").value();
				WritingMode = element.attribute("writing-mode").value();
			}


			const XML::XNode Table::toXML() const
			{
				return 
					XML::XElement(ns.style + "table-properties",
						XML::XAttribute(ns.table + "border-model", BorderModel) + 
					  XML::XAttribute(ns.style + "width", Width) + 
					  XML::XAttribute(ns.table + "align", Align) +
						XML::XAttribute(ns.style + "page-number", PageNumber) +
					  XML::XAttribute(ns.fo + "margin-left", MarginLeft) +
					  XML::XAttribute(ns.fo + "margin-right", MarginRight) +
						XML::XAttribute(ns.fo + "keep-with-next", KeepWithNext) +
						XML::XAttribute(ns.style + "writing-mode", WritingMode)
					);
			}


			const bool Table::isSimple() const
			{
				if (BorderModel.is_init() || Width.is_init() || Align.is_init() || 
						MarginLeft.is_init() || MarginRight.is_init() || WritingMode.is_init())
					return false;
				return true;
			}


			const Table Table::merge(const Table& prev, const Table& current)
			{
				Table properties;
				properties.BorderModel	= ::merge(prev.BorderModel, current.BorderModel);
				properties.Width		= ::merge(prev.Width, current.Width);
				properties.Align		= ::merge(prev.Align, current.Align);
				properties.PageNumber	= ::merge(prev.PageNumber, current.PageNumber);
				properties.MarginLeft	= ::merge(prev.MarginLeft, current.MarginLeft);
				properties.MarginRight	= ::merge(prev.MarginRight, current.MarginRight);
				properties.KeepWithNext = ::merge(prev.KeepWithNext, current.KeepWithNext);
				properties.WritingMode	= ::merge(prev.WritingMode, current.WritingMode);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt
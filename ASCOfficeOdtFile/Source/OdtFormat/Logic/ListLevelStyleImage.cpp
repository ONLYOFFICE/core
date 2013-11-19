
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevelStyleImage.h"


namespace Odt
{
	namespace Logic
	{

		ListLevelStyleImage::ListLevelStyleImage()
		{
		}


		ListLevelStyleImage::~ListLevelStyleImage()
		{
		}


		ListLevelStyleImage::ListLevelStyleImage(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListLevelStyleImage& ListLevelStyleImage::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
		

		void ListLevelStyleImage::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Level		= element.attribute("level").value();
			Style		=	element.attribute("style-name").value();
			Properties->Graphic		=	element.element("graphic-properties");
			Properties->ListLevel = element.element("list-level-properties");
			Properties->Paragraph	=	element.element("paragraph-properties");
			Properties->Text			=	element.element("text-properties");
			Properties->Table			=	element.element("table-properties");
			Properties->TableRow	=	element.element("table-row-properties");
			Properties->TableCell	=	element.element("table-cell-properties");
			Properties->TableColumn	=	element.element("table-column-properties");
			
			Href = element.attribute("href").value();
			Type = element.attribute("type").value();
			Show = element.attribute("show").value();
			Actuate = element.attribute("actuate").value();
		}


		const XML::XNode ListLevelStyleImage::toXML() const
		{
			return
				XML::XElement(ns.text + "list-level-style-image",
					XML::XAttribute(ns.text + "level", Level) +
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.xlink + "href", Href) +
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "show", Show) +
					XML::XAttribute(ns.xlink + "actuate", Actuate) +
					XML::Write(Properties->Graphic) +
					XML::Write(Properties->ListLevel) +
					XML::Write(Properties->Paragraph) +
					XML::Write(Properties->TableCell) +
					XML::Write(Properties->TableColumn) +
					XML::Write(Properties->Table) +
					XML::Write(Properties->TableRow) +
					XML::Write(Properties->Text)
				);
		}

	} // namespace Logic
} // namespace Odt
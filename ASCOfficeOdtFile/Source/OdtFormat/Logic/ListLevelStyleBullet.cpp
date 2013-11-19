
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevelStyleBullet.h"


namespace Odt
{
	namespace Logic
	{

		ListLevelStyleBullet::ListLevelStyleBullet()
		{
		}


		ListLevelStyleBullet::~ListLevelStyleBullet()
		{
		}


		ListLevelStyleBullet::ListLevelStyleBullet(const XML::XNode& node)
		{
			fromXML(node);
		}

	
		const ListLevelStyleBullet& ListLevelStyleBullet::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ListLevelStyleBullet::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Level		= element.attribute("level").value();
			Style		= element.attribute("style-name").value();			
			Bullet	= element.attribute("bullet-char").value();
			Properties->Graphic		=	element.element("graphic-properties");
			Properties->ListLevel = element.element("list-level-properties");
			Properties->Paragraph	=	element.element("paragraph-properties");
			Properties->Text			=	element.element("text-properties");
			Properties->Table			=	element.element("table-properties");
			Properties->TableRow	=	element.element("table-row-properties");
			Properties->TableCell	=	element.element("table-cell-properties");
			Properties->TableColumn	=	element.element("table-column-properties");
		}


		const XML::XNode ListLevelStyleBullet::toXML() const
		{
			return
				XML::XElement(ns.text + "list-level-style-bullet", 
					XML::XAttribute(ns.text + "level", Level) +
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.text + "bullet-char", Bullet) +
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
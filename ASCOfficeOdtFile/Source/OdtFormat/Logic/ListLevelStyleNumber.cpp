
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevelStyleNumber.h"


namespace Odt
{
	namespace Logic
	{

		ListLevelStyleNumber::ListLevelStyleNumber()
		{
		}


		ListLevelStyleNumber::~ListLevelStyleNumber()
		{
		}


		ListLevelStyleNumber::ListLevelStyleNumber(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListLevelStyleNumber& ListLevelStyleNumber::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
		

		void ListLevelStyleNumber::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Level		= element.attribute("level").value();
			Style		=	element.attribute("style-name").value();
			Suffix	=	element.attribute("num-suffix").value();
			Prefix	= element.attribute("num-prefix").value();
			NumFormat	= element.attribute("num-format").value();
			NumLetterSync = element.attribute("num-letter-sync").value();
			StartValue = element.attribute("start-value").value();
			Properties->Graphic		=	element.element("graphic-properties");
			Properties->ListLevel = element.element("list-level-properties");
			Properties->Paragraph	=	element.element("paragraph-properties");
			Properties->Text			=	element.element("text-properties");
			Properties->Table			=	element.element("table-properties");
			Properties->TableRow	=	element.element("table-row-properties");
			Properties->TableCell	=	element.element("table-cell-properties");
			Properties->TableColumn	=	element.element("table-column-properties");
		}


		const XML::XNode ListLevelStyleNumber::toXML() const
		{
			return
				XML::XElement(ns.text + "list-level-style-number",
					XML::XAttribute(ns.text + "level", Level) +
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.style + "num-prefix", Prefix) +
					XML::XAttribute(ns.style + "num-suffix", Suffix) +
					XML::XAttribute(ns.style + "num-format", NumFormat) +
					XML::XAttribute(ns.style + "num-letter-sync", NumLetterSync) +
					XML::XAttribute(ns.style + "start-value", StartValue) +
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
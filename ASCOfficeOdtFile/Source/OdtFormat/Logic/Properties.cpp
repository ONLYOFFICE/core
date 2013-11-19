
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Properties.h"


namespace Odt
{
	namespace Logic
	{

		Properties::Properties()
		{
		}


		Properties::~Properties()
		{
		}


		Properties::Properties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Properties& Properties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Properties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Graphic			=	element.element("graphic-properties");
			Paragraph		=	element.element("paragraph-properties");
			Section			= element.element("section-properties");
			Text				=	element.element("text-properties");
			Table				=	element.element("table-properties");
			TableRow		=	element.element("table-row-properties");
			TableCell		=	element.element("table-cell-properties");
			TableColumn	=	element.element("table-column-properties");
			DrawingPage = element.element("drawing-page-properties");
		}


		const XML::XNode Properties::toXML() const
		{
			return
				XML::XContainer(
					XML::Write(Graphic) +
					XML::Write(Paragraph) +
					XML::Write(Section) + 
					XML::Write(TableCell) +
					XML::Write(TableColumn) +
					XML::Write(Table) +
					XML::Write(TableRow) +
					XML::Write(Text) +
					XML::Write(DrawingPage)
				);
		}


		const bool Properties::isSimple() const
		{
			if (Graphic.is_init() && !Graphic->isSimple())
				return false;
			if (ListLevel.is_init() && !ListLevel->isSimple())
				return false;
			if (Paragraph.is_init() && !Paragraph->isSimple())
				return false;
			if (Section.is_init() && !Section->isSimple())
				return false;
			if (TableCell.is_init() && !TableCell->isSimple())
				return false;
			if (TableColumn.is_init() && !TableColumn->isSimple())
				return false;
			if (Table.is_init() && !Table->isSimple())
				return false;
			if (TableRow.is_init() && !TableRow->isSimple())
				return false;
			if (Text.is_init() && !Text->isSimple())
				return false;
			if (DrawingPage.is_init() && !DrawingPage->isSimple())
				return false;
			return true;
		}


		const Properties Properties::merge(const Properties& prev, const Properties& current)
		{
			Properties properties;
			properties.Graphic		= merge(prev.Graphic, current.Graphic);
			properties.ListLevel	= merge(prev.ListLevel, current.ListLevel);
			properties.Paragraph	= merge(prev.Paragraph, current.Paragraph);
			//properties.Section		= merge(prev.Section, current.Section);
			properties.TableCell	= merge(prev.TableCell, current.TableCell);
			properties.TableColumn	= merge(prev.TableColumn, current.TableColumn);
			properties.Table		= merge(prev.Table, current.Table);
			properties.TableRow		= merge(prev.TableRow, current.TableRow);
			properties.Text			= merge(prev.Text, current.Text);
			properties.DrawingPage	= merge(prev.DrawingPage, current.DrawingPage);
			return properties;
		}

	} // namespace Logic
} // namespace Odt
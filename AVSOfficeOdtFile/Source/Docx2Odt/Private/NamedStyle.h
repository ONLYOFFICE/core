#pragma once
#ifndef DOCX_2_ODT_NAMED_STYLE_INCLUDE_H_
#define DOCX_2_ODT_NAMED_STYLE_INCLUDE_H_

#include "Base.h"
#include <string>


namespace Docx2Odt
{
	namespace Private
	{
		class NamedStyle : public Base
		{
		public:
			NamedStyle(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Logic::NamedStyle paragraphProperty2style(const OOX::Logic::ParagraphProperty& ooxProperty) const;
			const Odt::Logic::NamedStyle runProperty2style(const OOX::Logic::RunProperty& ooxProperty/*, const OOX::Theme::File& theme*/) const;
			//const Odt::Logic::NamedStyle runProperty2style(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Properties& odtProperties, const OOX::Theme::File& theme) const;
			const Odt::Logic::NamedStyle table2style(const OOX::Logic::Table& ooxTable) const;
			const Odt::Logic::NamedStyle tableColumnProperty2style(const OOX::Logic::GridColumn& gridColumn) const;
			const Odt::Logic::NamedStyle tableRowProperty2style(const OOX::Logic::TableRowProperties& ooxProperty) const;
			const Odt::Logic::NamedStyle tableCellProperty2style(const OOX::Logic::TableCellProperties& ooxProperty) const;
			const Odt::Logic::NamedStyle drawing2style(const OOX::Logic::Drawing& ooxDrawing) const;

			void addStyle2Styles(const Odt::Logic::NamedStyle& style);
			void addStyle2Content(const Odt::Logic::NamedStyle& style);

			const nullable<Odt::Logic::NamedStyle> createRunStyle(const OOX::Logic::Run& ooxRun, Odt::Content::Paragraph& odtParagraph/*, const OOX::Theme::File& theme*/) const;
			const Odt::Logic::NamedStyle createEmptyParagraphStyle() const; 
		private:
			const std::string createName(const std::string& prefix) const;
			static void SetUnderType(Odt::Logic::Property::Text& odtProperty, const std::string& type);

		public:
			static const Odt::Logic::Property::Paragraph ppr2ppr(const OOX::Logic::ParagraphProperty& ooxProperty);
			/*static*/ const Odt::Logic::Property::Text rpr2tpr(const OOX::Logic::RunProperty& ooxProperty/*, const OOX::Theme::File& theme*/)const;
			//static const Odt::Logic::Property::Text rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Property::Text& odtParagraphProperty, const OOX::Theme::File& theme);
			static const Odt::Logic::Property::Table table2tablePr(const OOX::Logic::Table& ooxTable);
			static const Odt::Logic::Property::TableColumn gridColumn2tableColumnPr(const OOX::Logic::GridColumn& gridColumn);
			static const Odt::Logic::Property::TableRow tableRowPr2tableRowPr(const OOX::Logic::TableRowProperties& ooxProperty);
			static const Odt::Logic::Property::TableCell tableCellPr2tableCellPr(const OOX::Logic::TableCellProperties& ooxProperty);
			static const Odt::Logic::Property::Graphic drawing2graphicProperty(const OOX::Logic::Drawing& ooxDrawing);

		public:
			static void ind2ppr(const OOX::Logic::Ind& ooxInd, Odt::Logic::Property::Paragraph& odtProperty);
			static void space2ppr(const OOX::Logic::Spacing& ooxSpacing, Odt::Logic::Property::Paragraph& odtProperty);
			static void borders2borders(const OOX::Logic::TableCellBorders& ooxBorders, Odt::Logic::Property::TableCell& odtProperty); 
			static void borders2borders(const OOX::Logic::ParagraphBorder& ooxBorders, Odt::Logic::Property::Paragraph& odtProperty);
			static void borders2borders(const OOX::Logic::PageBorders& ooxBorders, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties);
			static const Odt::Logic::Border border2border(const OOX::Logic::BorderItem& ooxBorder);
			static const Odt::Logic::BackgroundColor shading2color(const OOX::Logic::Shading& shading);

		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_NAMED_STYLE_INCLUDE_H_
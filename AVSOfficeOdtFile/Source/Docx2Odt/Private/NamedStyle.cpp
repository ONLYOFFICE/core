
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "NamedStyle.h"
#include "ToString.h"


namespace Docx2Odt
{
	namespace Private
	{

		NamedStyle::NamedStyle(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Logic::NamedStyle NamedStyle::paragraphProperty2style(const OOX::Logic::ParagraphProperty& ooxProperty) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("P");
			odtStyle.Family = "paragraph";

			if (ooxProperty.PStyle.is_init())
				odtStyle.ParentName = ooxProperty.PStyle;
//			else
//				odtStyle.ParentName = "Standard";

			odtStyle.Properties->Paragraph = ppr2ppr(ooxProperty);

			/*if (ooxProperty.RunProperty.is_init() && !ooxProperty.RunProperty->isSimple())
				odtStyle.Properties->Text = rpr2tpr(ooxProperty.RunProperty);*/


			// TODO перепроверить. Можно ли сделать конвертацию параграфа без конвертации текста
//			if (ooxProperty.RunProperty.is_init() /*&& odtStyle.ParentName == "Standard"*/)
//			{
//				odtStyle.Properties->Text = rpr2tpr(ooxProperty.RunProperty);
//			}

			return odtStyle;
		}


		const Odt::Logic::Property::Paragraph NamedStyle::ppr2ppr(const OOX::Logic::ParagraphProperty& ooxProperty)
		{
			Odt::Logic::Property::Paragraph odtProperty;
			odtProperty.Align		= ooxProperty.Align;
			//if(ooxProperty.Shading.is_init())
			//	odtProperty.Background	= shading2color(ooxProperty.Shading);
	
			if(ooxProperty.Ind.is_init())
				ind2ppr(ooxProperty.Ind, odtProperty);

			if(ooxProperty.Spacing.is_init())
				space2ppr(ooxProperty.Spacing, odtProperty);

			if(ooxProperty.ParagraphBorder.is_init())
				borders2borders(ooxProperty.ParagraphBorder, odtProperty);

			if(*ooxProperty.KeepLines)
				odtProperty.KeepTogether = "always";
			if(*ooxProperty.KeepNext)
				odtProperty.KeepWithNext = "always";
			if(*ooxProperty.SuppressLineNumbers)
			{
				odtProperty.NumberLines = false;
				odtProperty.LineNumber = 0;
			}

			return odtProperty;
		}


		void NamedStyle::ind2ppr(const OOX::Logic::Ind& ooxInd, Odt::Logic::Property::Paragraph& odtProperty)
		{
			if (ooxInd.Left.is_init())
			{
				odtProperty.MarginLeft = ooxInd.Left;
				odtProperty.MarginLeft->Type = UniversalUnit::Cm;
			}
			if (ooxInd.Right.is_init())
			{
				odtProperty.MarginRight = ooxInd.Right;
				odtProperty.MarginRight->Type = UniversalUnit::Cm;
			}
			if (ooxInd.FirstLine.is_init())
			{
				odtProperty.TextIndent = ooxInd.FirstLine;
				odtProperty.TextIndent->Type = UniversalUnit::Cm;
			}
		}


		void NamedStyle::space2ppr(const OOX::Logic::Spacing& ooxSpacing, Odt::Logic::Property::Paragraph& odtProperty)
		{
			if (!ooxSpacing.BeforeAutospacing.is_init() || ooxSpacing.BeforeAutospacing == false)
			{
				if(ooxSpacing.Before.is_init())
				{
					odtProperty.MarginTop	= ooxSpacing.Before;
					odtProperty.MarginTop->Type = UniversalUnit::Cm;
				}
			}
			if (!ooxSpacing.AfterAutospacing.is_init() || ooxSpacing.AfterAutospacing == false)
			{
				if(ooxSpacing.After.is_init())
				{
					odtProperty.MarginBottom = ooxSpacing.After;
					odtProperty.MarginBottom->Type = UniversalUnit::Cm;
				}
			}
			if(ooxSpacing.LineRule.is_init())
			{
				if (ooxSpacing.LineRule == "auto")// || ooxSpacing.LineRule == "atLeast" || ooxSpacing.LineRule == "exact")
				{	
					//odtProperty.LineHeightAtLeast = ooxSpacing.Line;
					//odtProperty.LineHeightAtLeast->Type = UniversalUnit::Cm;
					odtProperty.LineHeight = UniversalUnit(ooxSpacing.Line->value(UniversalUnit::Dx) / 2.4, UniversalUnit::Percent);
				}
				else
				{
					odtProperty.LineHeight = ooxSpacing.Line;
					odtProperty.LineHeight->Type = UniversalUnit::Cm;
				}
			}
		}


		const Odt::Logic::Property::Text NamedStyle::rpr2tpr(const OOX::Logic::RunProperty& ooxProperty/*, const OOX::Theme::File& theme*/)const
		{
			Odt::Logic::Property::Text odtProperty;
			odtProperty.Bold		= ooxProperty.Bold;
			odtProperty.Italic		= ooxProperty.Italic;
//			odtProperty.Under		= ooxProperty.Under;
			if(ooxProperty.Under.get_value_or(false))
			{
				odtProperty.UnderlineColor = "font-color";
				odtProperty.UnderlineWidth = "auto";
				odtProperty.UnderlineType = "single";
				SetUnderType(odtProperty, ooxProperty.UnderType.get_value_or("single"));
				if(odtProperty.UnderlineType.get_value_or("single") == "single")
					odtProperty.UnderlineType.reset();
			}
			if(ooxProperty.Shading.is_init())
				odtProperty.Background	= shading2color(ooxProperty.Shading);

			if (ooxProperty.Strike.get_value_or(false))
				odtProperty.LineThroughStyle = "solid";

			if (ooxProperty.DStrike.get_value_or(false))
			{
				odtProperty.LineThroughStyle = "solid";
				odtProperty.LineThroughType = "double";
			}

			if(ooxProperty.rFonts.is_init())
			{
				OOX::Theme::File theme;
				if (m_docxFileContainer.exist<OOX::Theme::File>())
					theme = m_docxFileContainer.find<OOX::Theme::File>();

				std::string fontType = ooxProperty.rFonts->fontTypeHAnsi();
				if (fontType == "fontName")
					odtProperty.FontName = *ooxProperty.rFonts->hAnsi;
				else if (fontType == "minor")
					odtProperty.FontName = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontName = theme.GetMajorFont();

				fontType = ooxProperty.rFonts->fontTypeAscii();
				if (fontType == "fontName")
					odtProperty.FontName = *ooxProperty.rFonts->ascii;
				else if (fontType == "minor")
					odtProperty.FontName = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontName = theme.GetMajorFont();

				fontType = ooxProperty.rFonts->fontTypeEA();
				if (fontType == "fontName")
					odtProperty.FontNameAsian = *ooxProperty.rFonts->EastAsia;
				else if (fontType == "minor")
					odtProperty.FontNameAsian = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontNameAsian = theme.GetMajorFont();

				fontType = ooxProperty.rFonts->fontTypeCS();
				if (fontType == "fontName")
					odtProperty.FontNameComplex = *ooxProperty.rFonts->Cs;
				else if (fontType == "minor")
					odtProperty.FontNameComplex = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontNameComplex = theme.GetMajorFont();
			}

			if (ooxProperty.Caps.get_value_or(false))
				odtProperty.TextTransform = "uppercase";
			if (ooxProperty.SmallCaps.get_value_or(false))
				odtProperty.FontVariant = "small-caps";
			if(ooxProperty.Emboss.get_value_or(false))
				odtProperty.Relief = "embossed";
			if(ooxProperty.Imprint.get_value_or(false))
				odtProperty.Relief = "engraved";
			if(ooxProperty.Vanish.get_value_or(false))
				odtProperty.Display = "none";
			if(ooxProperty.Shadow.get_value_or(false))
				odtProperty.Shadow = "1pt 1pt";
			if(ooxProperty.Outline.get_value_or(false))
				odtProperty.Outline = true;


			if (ooxProperty.FontSize.is_init())
				odtProperty.FontSize = ooxProperty.FontSize.get() / 2;
			
            if (ooxProperty.FontColor.is_init())
            {
                if (!ooxProperty.FontColor->isAuto())
                    odtProperty.FontColor	= ooxProperty.FontColor;
                else
                    odtProperty.UseWindowFontColor = true;
            }

			odtProperty.Index		= ooxProperty.Index;

			if (ooxProperty.Scale.is_init())
				odtProperty.TextScale = ooxProperty.Scale;

			if (ooxProperty.Spacing.is_init())
				odtProperty.LetterSpacing = *ooxProperty.Spacing;

			return odtProperty;
		}

/*
		// TODO odtParagraphProperty не используется
		const Odt::Logic::Property::Text NamedStyle::rpr2tpr(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Property::Text& odtParagraphProperty, const OOX::Theme::File& theme)
		{
			Odt::Logic::Property::Text odtProperty;

			odtProperty.Bold	= ooxProperty.Bold;
			odtProperty.Italic	= ooxProperty.Italic;
			odtProperty.Under	= ooxProperty.Under;

			if (ooxProperty.Strike.is_init() && ooxProperty.Strike == true)
				odtProperty.LineThroughStyle = true;

			if (ooxProperty.DStrike.is_init() && ooxProperty.DStrike ==true)
			{
				odtProperty.LineThroughStyle = true;
				odtProperty.LineThroughType = "double";
			}

			if(ooxProperty.RFonts.is_init())
			{
				std::string fontType = ooxProperty.RFonts->fontType();
				if (fontType == "fontName")
					odtProperty.FontName = ooxProperty.RFonts->fontName();
				else if (fontType == "minor")
					odtProperty.FontName = theme.GetMinorFont();
				else if (fontType == "major")
					odtProperty.FontName = theme.GetMajorFont();
			}

			if (ooxProperty.Caps.get_value_or(false))
				odtProperty.TextTransform = "uppercase";
			if (ooxProperty.SmallCaps.get_value_or(false))
				odtProperty.FontVariant = "small-caps";
			if(ooxProperty.Emboss.get_value_or(false))
				odtProperty.Relief = "embossed";
			if(ooxProperty.Imprint.get_value_or(false))
				odtProperty.Relief = "engraved";
			if(ooxProperty.Vanish.get_value_or(false))
				odtProperty.Display = "none";
			if(ooxProperty.Shadow.get_value_or(false))
				odtProperty.Shadow = "1pt 1pt";
			if(ooxProperty.Outline.get_value_or(false))
				odtProperty.Outline = true;

			if (ooxProperty.FontSize.is_init())
				odtProperty.FontSize = ooxProperty.FontSize.get() / 2;
			odtProperty.FontColor	= ooxProperty.FontColor;
			odtProperty.Background	= ooxProperty.Background;
			odtProperty.Index		= ooxProperty.Index;

			if (ooxProperty.Scale.is_init())
				odtProperty.TextScale = ooxProperty.Scale;

			if (ooxProperty.Spacing.is_init())
				odtProperty.LetterSpacing = *ooxProperty.Spacing;

			return odtProperty;
		}
*/

		void NamedStyle::addStyle2Styles(const Odt::Logic::NamedStyle& style)
		{
			m_odtFolder.Styles->Styles->NamedStyles->push_back(style);
		}


		void NamedStyle::addStyle2Content(const Odt::Logic::NamedStyle& style)
		{
			m_odtFolder.Content->NamedStyles->push_back(style);
		}


		const std::string NamedStyle::createName(const std::string& prefix) const
		{
			const size_t stylesStyles		= m_odtFolder.Styles->Styles->NamedStyles->size();
			const size_t contentStyles	= m_odtFolder.Content->NamedStyles->size();
			return prefix + ToString(stylesStyles + contentStyles);
		}


		// TODO odtParagraph не используется
		const nullable<Odt::Logic::NamedStyle> NamedStyle::createRunStyle(const OOX::Logic::Run& ooxRun, Odt::Content::Paragraph& odtParagraph/*, const OOX::Theme::File& theme*/) const
		{
			nullable<Odt::Logic::NamedStyle> odtStyle;

			if (odtParagraph.Style == "Standard")
			{
				if (ooxRun.Property.is_init() && !ooxRun.Property->isSimple())
				{
					odtStyle = runProperty2style(ooxRun.Property/*, theme*/);
				}
			}
			else
			{
				if(ooxRun.Property.is_init() && !ooxRun.Property->isSimple())
				{
//					Odt::Logic::Properties odtParagraphProperties = m_odtFolder.properties(odtParagraphStyleName);
//					Run converter(m_odtFolder, m_docxFolder);
//					odtRunStyle = converter.rpr2style(ooxRun.Property, odtParagraphProperties);
				}

//				if (getDisplayStyleName(odtParagraphStyleName) == "Standard")
//				{
					// TODO исправить после реализации стилей
//					Odt::Logic::Properties odtParagraphProperties = m_odtFolder.properties(odtParagraphStyleName);
//					Run converter(m_odtFolder, m_docxFolder);
//					odtRunStyle = converter.rpr2style(ooxRun.Property.get_value_or_default(), odtParagraphProperties);
//				}
//				else if(ooxRun.Property.is_init() && !ooxRun.Property->isSimple())
//				{
					// TODO исправить после реализации стилей
//					Odt::Logic::Properties odtParagraphProperties = m_odtFolder.properties(odtParagraphStyleName);
//					Run converter(m_odtFolder, m_docxFolder);
//					odtRunStyle = converter.rpr2style(ooxRun.Property, odtParagraphProperties);
//				}
			}
			return odtStyle;
		}


		const Odt::Logic::NamedStyle NamedStyle::runProperty2style(const OOX::Logic::RunProperty& ooxProperty/*, const OOX::Theme::File& theme*/) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("T");
			odtStyle.Family	= "text";
			if (ooxProperty.RStyle.is_init())
				odtStyle.ParentName = ooxProperty.RStyle;
			odtStyle.Properties->Text = rpr2tpr(ooxProperty/*, theme*/);
			return odtStyle;
		}

/*
		// TODO odtProperty не используется
		const Odt::Logic::NamedStyle NamedStyle::runProperty2style(const OOX::Logic::RunProperty& ooxProperty, const Odt::Logic::Properties& odtProperties, const OOX::Theme::File& theme) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("T");
			odtStyle.Family	= "text";
			odtStyle.Properties->Text = rpr2tpr(ooxProperty, odtProperties.Text, theme);
			return odtStyle;
		}
*/

		const Odt::Logic::NamedStyle NamedStyle::table2style(const OOX::Logic::Table& ooxTable) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("Table");
			odtStyle.Family	= "table";
			if (ooxTable.tblPr->Style.is_init())
				odtStyle.ParentName = ooxTable.tblPr->Style;
			odtStyle.Properties->Table = table2tablePr(ooxTable);
			return odtStyle;
		}


		const Odt::Logic::NamedStyle NamedStyle::tableColumnProperty2style(const OOX::Logic::GridColumn& gridColumn) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("Table-column");
			odtStyle.Family	= "table-column";
			odtStyle.Properties->TableColumn = gridColumn2tableColumnPr(gridColumn);
			return odtStyle;
		}


		const Odt::Logic::NamedStyle NamedStyle::tableRowProperty2style(const OOX::Logic::TableRowProperties& ooxProperty) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("Table-row");
			odtStyle.Family	= "table-row";
			odtStyle.Properties->TableRow = tableRowPr2tableRowPr(ooxProperty);
			return odtStyle;
		}


		const Odt::Logic::NamedStyle NamedStyle::tableCellProperty2style(const OOX::Logic::TableCellProperties& ooxProperty) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("Table-cell");
			odtStyle.Family	= "table-cell";
			odtStyle.Properties->TableCell = tableCellPr2tableCellPr(ooxProperty);
			return odtStyle;
		}


		const Odt::Logic::Property::Table NamedStyle::table2tablePr(const OOX::Logic::Table& ooxTable)
		{
			Odt::Logic::Property::Table odtProperty;
			odtProperty.Align = "margins";
			if(ooxTable.tblPr->Ind.is_init())
			{
				odtProperty.MarginLeft = UniversalUnit(*ooxTable.tblPr->Ind->Width, UniversalUnit::Dx);
				odtProperty.MarginLeft->Type = UniversalUnit::Cm;
			}

			Unit<int, Dx> width = 0;
			BOOST_FOREACH(const OOX::Logic::GridColumn& column, *ooxTable.tblGrid->Columns)
			{
				width += *column.Width;
			}
			odtProperty.Width = width;
			return odtProperty;
		}


		const Odt::Logic::Property::TableColumn NamedStyle::gridColumn2tableColumnPr(const OOX::Logic::GridColumn& gridColumn)
		{
			Odt::Logic::Property::TableColumn odtProperty;
			odtProperty.Width = *gridColumn.Width; 
			return odtProperty;
		}


		const Odt::Logic::Property::TableRow NamedStyle::tableRowPr2tableRowPr(const OOX::Logic::TableRowProperties& ooxProperty)
		{
			Odt::Logic::Property::TableRow odtProperty;
			odtProperty.MinHeight = ooxProperty.Height->Height.get();
			odtProperty.KeepTogether = "true";
			odtProperty.FoKeepTogether = "auto";

			return odtProperty;
		}


		const Odt::Logic::Property::TableCell NamedStyle::tableCellPr2tableCellPr(const OOX::Logic::TableCellProperties& ooxProperty)
		{
			Odt::Logic::Property::TableCell odtProperty;
			if (ooxProperty.tblBorders.is_init())
				borders2borders(ooxProperty.tblBorders, odtProperty);
			if (ooxProperty.Background.is_init())
                odtProperty.BackgroundColor = std::string("#") + ooxProperty.Background->ToString();
			return odtProperty;
		}


		void NamedStyle::borders2borders(const OOX::Logic::TableCellBorders& ooxBorders, Odt::Logic::Property::TableCell& odtProperty)
		{
			if (ooxBorders.bottom.is_init())
				odtProperty.BorderBottom = border2border(ooxBorders.bottom->Bdr);
			if (ooxBorders.top.is_init())
				odtProperty.BorderTop	 = border2border(ooxBorders.top->Bdr);
			if (ooxBorders.left.is_init())
				odtProperty.BorderLeft	 = border2border(ooxBorders.left->Bdr);
			if (ooxBorders.right.is_init())
				odtProperty.BorderRight  = border2border(ooxBorders.right->Bdr);
		}


		void NamedStyle::borders2borders(const OOX::Logic::ParagraphBorder& ooxBorders, Odt::Logic::Property::Paragraph& odtProperty)
		{
			if (ooxBorders.Bottom.is_init())
				odtProperty.BorderBottom = border2border(ooxBorders.Bottom->Bdr);
			if (ooxBorders.Top.is_init())
				odtProperty.BorderTop	 = border2border(ooxBorders.Top->Bdr);
			if (ooxBorders.Left.is_init())
				odtProperty.BorderLeft	 = border2border(ooxBorders.Left->Bdr);
			if (ooxBorders.Right.is_init())
				odtProperty.BorderRight  = border2border(ooxBorders.Right->Bdr);
		}


		void NamedStyle::borders2borders(const OOX::Logic::PageBorders& ooxBorders, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties)
		{
			if (ooxBorders.Bottom.is_init())
				odtPageLayoutProperties.BorderBottom = border2border(ooxBorders.Bottom->Bdr);
			if (ooxBorders.Top.is_init())
				odtPageLayoutProperties.BorderTop	 = border2border(ooxBorders.Top->Bdr);
			if (ooxBorders.Left.is_init())
				odtPageLayoutProperties.BorderLeft	 = border2border(ooxBorders.Left->Bdr);
			if (ooxBorders.Right.is_init())
				odtPageLayoutProperties.BorderRight  = border2border(ooxBorders.Right->Bdr);
		}


		const Odt::Logic::Border NamedStyle::border2border(const OOX::Logic::BorderItem& ooxBorder)
		{
			Odt::Logic::Border odtBorder;

			odtBorder.Color		 = ooxBorder.Color.get_value_or(Odt::Logic::Color("#000000"));
			odtBorder.BorderType = "solid";
			odtBorder.Width		 = ooxBorder.Sz.get_value_or(0);

			return odtBorder;
		}

		const Odt::Logic::NamedStyle NamedStyle::createEmptyParagraphStyle() const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("P");
			odtStyle.Family = "paragraph";
			
			return odtStyle;
		}


		const Odt::Logic::NamedStyle NamedStyle::drawing2style(const OOX::Logic::Drawing& ooxDrawing) const
		{
			Odt::Logic::NamedStyle odtStyle;
			odtStyle.Name = createName("fr");
			odtStyle.Family = "graphic";

			odtStyle.Properties->Graphic = drawing2graphicProperty(ooxDrawing);

			return odtStyle;
		}


		const Odt::Logic::Property::Graphic NamedStyle::drawing2graphicProperty(const OOX::Logic::Drawing& ooxDrawing)
		{
			Odt::Logic::Property::Graphic odtGraphicProperty;
			if((*ooxDrawing.Inline->Name == "inline") || (ooxDrawing.Inline->positionVRelativeFrom.get_value_or("paragraph") != "paragraph"))
			{
				odtGraphicProperty.VerticalPos = "top";
				odtGraphicProperty.VerticalRel = "baseline";
			}
			return odtGraphicProperty;
		}


		void NamedStyle::SetUnderType(Odt::Logic::Property::Text& odtProperty, const std::string& type)
		{
			if((type == "single") || (type == "double"))
			{
				odtProperty.UnderlineType = type;
				odtProperty.UnderlineStyle = "solid";
			}
			else if((type == "dash") || (type == "dotted") || (type == "wave"))
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = type;
			}
			else if(type == "wavyDouble")
			{
				odtProperty.UnderlineType = "double";
				odtProperty.UnderlineStyle = "wave";
			}
			else if(type == "dashLong")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "long-dash";
			}
			else if(type == "dotDash")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dot-dash";
			}
			else if(type == "dotDotDash")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dot-dot-dash";
			}
			else if(type == "wavyHeavy")
			{
				odtProperty.UnderlineStyle = "wave";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "dashLongHeavy")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "long-dash";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "dashDotHeavy")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dot-dash";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "dashDotDotHeavy")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dot-dot-dash";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "dashedHeavy")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dash";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "dottedHeavy")
			{
				//odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "dotted";
				odtProperty.UnderlineWidth = "bold";
			}
			else if(type == "thick")
			{
				odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "solid";
				odtProperty.UnderlineWidth = "thick";
			}
			else if(type == "words")
			{
				odtProperty.UnderlineType = "single";
				odtProperty.UnderlineStyle = "solid";
			}
			else
			{
				odtProperty.UnderlineType.reset();
				odtProperty.UnderlineStyle.reset();
				odtProperty.UnderlineWidth.reset();
				odtProperty.UnderlineColor.reset();
			}
		}


		const Odt::Logic::BackgroundColor NamedStyle::shading2color(const OOX::Logic::Shading& shading)
		{
			Odt::Logic::BackgroundColor color;
			if (shading.fill.is_init() && shading.fill != "auto")
				color =  shading.fill;
			else 
				color = "000000";
			return color;
		}
	} // namespace Private
} // namespace Docx2Odt
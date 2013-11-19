
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Document.h"
#include "FileContainer.h"
#include "Private/TextItem.h"
#include "Private/NamedStyle.h"


namespace Docx2Odt
{

	void Document::convert(const OOX::Document& ooxDocument, Odt::Folder& odtFolder)
	{
		const FileContainer converter;
		converter.convert(ooxDocument, odtFolder);

		sectorPropertyAndBG2PageLayoutProperties(ooxDocument, *odtFolder.Styles->AutomaticStyles->PageLayouts->front().Properties);
		sectorProperty2NotesProperties(ooxDocument.SectorProperty, *odtFolder.Styles->Styles->NotesConfigurations);
		
		sectorProperty2HeadersAndFooters(ooxDocument, odtFolder);


		Private::TextItem itemConverter(odtFolder, ooxDocument);
		itemConverter.converterProperties.hasBukvitza = false;
		itemConverter.converterProperties.Distance = UniversalUnit();
		itemConverter.converterProperties.Lines = 0;
		itemConverter.convert(ooxDocument.Items, *odtFolder.Content->Body->Items);
	}


	void Document::sectorProperty2NotesProperties(const OOX::Logic::SectorProperty& ooxSectorProperty, std::vector<Odt::Styles::NotesConfiguration>& notesPr)
	{
		if (!notesPr.empty())
		{
			BOOST_FOREACH(Odt::Styles::NotesConfiguration& noteStyle, notesPr)
			{
				if (noteStyle.NoteClass == "footnote" && ooxSectorProperty.FootNoteProperty.is_init())
					convertNoteStyle(ooxSectorProperty.FootNoteProperty, noteStyle);
				else if (noteStyle.NoteClass == "endnote" && ooxSectorProperty.EndNoteProperty.is_init())
					convertNoteStyle(ooxSectorProperty.EndNoteProperty, noteStyle);
			}
		}
	}

	void Document::convertNoteStyle(const OOX::Logic::NoteProperty& notePr, Odt::Styles::NotesConfiguration& noteStyle)
	{
		noteStyle.NumFormat = *notePr.NumFormat;
		noteStyle.StartValue = *notePr.NumStart - 1;
	}


	void Document::sectorProperty2HeadersAndFooters(const OOX::Document& ooxDocument, Odt::Folder& odtFolder)
	{
		OOX::Settings::File settings;
        bool isOddAndEven;
        try 
        {
            settings = ooxDocument.find<OOX::Settings::File>();
            isOddAndEven = settings.EvenAndOddHeaders;
        }
        catch(...)
        {
            isOddAndEven = false;        
        }		

		Odt::Styles::MasterPage& masterPage = odtFolder.Styles->MasterStyles->find("Standard");
		BOOST_FOREACH(const OOX::Logic::HeaderReference& head, *ooxDocument.SectorProperty->Headers)
		{
			if (head.Type == "default")
			{
				OOX::Header header = dynamic_cast<OOX::Header&>(*ooxDocument[head.rId]);
				Private::TextItem itemConverter(odtFolder, header);
				itemConverter.converterProperties.setDestStyleIsContent(false);
				itemConverter.convert(header.Items, *masterPage.Header);
			}
			else if (head.Type == "even" && isOddAndEven == true)
			{
				OOX::Header headerLeft = dynamic_cast<OOX::Header&>(*ooxDocument[head.rId]);
				Private::TextItem itemConverter(odtFolder, headerLeft);				
				itemConverter.converterProperties.setDestStyleIsContent(false);
				itemConverter.convert(headerLeft.Items, *masterPage.HeaderLeft);
			}
		}
		BOOST_FOREACH(const OOX::Logic::FooterReference& foot, *ooxDocument.SectorProperty->Footers)
		{
			if (foot.Type == "default")
			{
				OOX::Footer footer = dynamic_cast<OOX::Footer&>(*ooxDocument[foot.rId]);
				Private::TextItem itemConverter(odtFolder, footer);				
				itemConverter.converterProperties.setDestStyleIsContent(false);
				itemConverter.convert(footer.Items, *masterPage.Footer);
			}
			else if (foot.Type == "even" && isOddAndEven == true)
			{
				OOX::Footer footerLeft = dynamic_cast<OOX::Footer&>(*ooxDocument[foot.rId]);
				Private::TextItem itemConverter(odtFolder, footerLeft);
				itemConverter.converterProperties.setDestStyleIsContent(false);
				itemConverter.convert(footerLeft.Items, *masterPage.FooterLeft);
			}
		}
	}

	//void Document::HeadOrFoot2HeadOrFoot()


	void Document::sectorPropertyAndBG2PageLayoutProperties(const OOX::Document& ooxDocument, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties)
	{
		pageSize2PageLayoutProperties(ooxDocument.SectorProperty->PageSize, odtPageLayoutProperties);
		pageMargin2PageLayoutProperties(ooxDocument.SectorProperty->PageMargin, odtPageLayoutProperties);

		if (ooxDocument.SectorProperty->PageBorders.is_init())
			Private::NamedStyle::borders2borders(ooxDocument.SectorProperty->PageBorders, odtPageLayoutProperties);

		if (ooxDocument.SectorProperty->Columns.is_init())
			odtPageLayoutProperties.Columns = columns2columns(ooxDocument.SectorProperty->Columns);

		if (ooxDocument.Background.is_init())
			odtPageLayoutProperties.BackgroundColor = ooxDocument.Background->Color;

	}


	void Document::pageSize2PageLayoutProperties(const OOX::Logic::PageSize& ooxPageSize, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties)
	{
		odtPageLayoutProperties.PageWidth		= UniversalUnit(ooxPageSize.Width.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.PageWidth->Type = UniversalUnit::Inch;

		odtPageLayoutProperties.PageHeight		= UniversalUnit(ooxPageSize.Height.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.PageHeight->Type = UniversalUnit::Inch;

		odtPageLayoutProperties.PrintOrientation = ooxPageSize.Orient.get_value_or("portrait");
	}
	
	
	void Document::pageMargin2PageLayoutProperties(const OOX::Logic::PageMargin& ooxPageMargin, Odt::Styles::PageLayoutProperties& odtPageLayoutProperties)
	{
		odtPageLayoutProperties.MarginBottom = UniversalUnit(ooxPageMargin.Bottom.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.MarginBottom->Type = UniversalUnit::Inch;

		odtPageLayoutProperties.MarginTop = UniversalUnit(ooxPageMargin.Top.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.MarginTop->Type = UniversalUnit::Inch;

		odtPageLayoutProperties.MarginLeft = UniversalUnit(ooxPageMargin.Left.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.MarginLeft->Type = UniversalUnit::Inch;

		odtPageLayoutProperties.MarginRight = UniversalUnit(ooxPageMargin.Right.get(), UniversalUnit::Dx);
		odtPageLayoutProperties.MarginRight->Type = UniversalUnit::Inch;
	}


	const Odt::Styles::Columns Document::columns2columns(const OOX::Logic::Columns& ooxColumns)
	{
		Odt::Styles::Columns odtColumns;
		odtColumns.Count = ooxColumns.Num.get_value_or(1);
		odtColumns.Gap = 0;
		odtColumns.Items->clear();

		if (!ooxColumns.Items->empty() && ooxColumns.Space.is_init())
			odtColumns.Gap = *ooxColumns.Space;			//TODO исправить
		else if (ooxColumns.Space.is_init()) 
			odtColumns.Gap = *ooxColumns.Space;

		return odtColumns;
	}	

} // namespace Docx2Odt
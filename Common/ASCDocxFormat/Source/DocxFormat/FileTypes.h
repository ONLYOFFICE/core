#pragma once
#ifndef OOX_FILE_TYPES_INCLUDE_H_
#define OOX_FILE_TYPES_INCLUDE_H_

#include "FileType.h"


namespace OOX
{
	namespace FileTypes
	{
		const FileType App(L"docProps", L"app.xml",
												"application/vnd.openxmlformats-officedocument.extended-properties+xml",
												"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");

		const FileType Core(L"docProps", L"core.xml",
												"application/vnd.openxmlformats-package.core-properties+xml",
												"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");

		const FileType Document(L"word", L"document.xml",
														"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml",
														"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType Theme(L"theme", L"theme.xml",
													"application/vnd.openxmlformats-officedocument.theme+xml",
													"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme");

		const FileType Setting(L"", L"settings.xml",
														"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",
														"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings");

		const FileType FontTable(L"", L"fontTable.xml",
															"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable");

		const FileType Style(L"", L"styles.xml",
													"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",
													"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");

		const FileType Item(L"customXml", L"item.xml",
													"WARNING not implement",
													"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml");

		const FileType FootNote(L"", L"footnotes.xml",
														"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml",
														"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes");

		const FileType EndNote(L"", L"endnotes.xml",
														"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml",
														"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes");

		const FileType WebSetting(L"", L"webSettings.xml",
															"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings");

		const FileType Header(L"", L"header.xml",
															"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header");

		const FileType Footer(L"", L"footer.xml", 
															"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer");

		const FileType Numbering(L"", L"numbering.xml", 
															"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering");

		const FileType ItemProp(L"customXml", L"itemProps.xml",
															"application/vnd.openxmlformats-officedocument.customXmlProperties+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps");

		const FileType HyperLink(L"", L"", 
															"", 
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink");

		const FileType ExternalImage(L"", L"", 
															"", 
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");

		const FileType ExternalAudio(L"", L"", 
															"", 
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio");

		const FileType ExternalVideo(L"", L"", 
															"", 
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video");

		const FileType Image(L"media", L"image", 
												"", 
												"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");

		const FileType Audio(L"media", L"audio", 
												"", 
												"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio");

		const FileType Video(L"media", L"video", 
												"", 
												"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video");

		const FileType Data(L"diagrams", L"data.xml",
															"application/vnd.openxmlformats-officedocument.drawingml.diagramData+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramData");

		const FileType Layout(L"diagrams", L"layout.xml",
															"application/vnd.openxmlformats-officedocument.drawingml.diagramLayout+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramLayout");

		const FileType Colors(L"diagrams", L"colors.xml",
															"application/vnd.openxmlformats-officedocument.drawingml.diagramColors+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramColors");

		const FileType QuickStyle(L"diagrams", L"quickStyle.xml",
															"application/vnd.openxmlformats-officedocument.drawingml.diagramStyle+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramQuickStyle");

		const FileType Chart(L"charts", L"chart.xml",
															"application/vnd.openxmlformats-officedocument.drawingml.chart+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart");
		

		const FileType MicrosoftOfficeExcelWorksheet(L"embeddings", L"Microsoft_Office_Excel_Worksheet.xlsx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcel_97_2003_Worksheet(L"embeddings", L"Microsoft_Office_Excel_97-2003_Worksheet.xls",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficeExcelBinaryWorksheet(L"embeddings", L"Microsoft_Office_Excel_Binary_Worksheet.xlsb",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcelMacro_EnabledWorksheet(L"embeddings", L"Microsoft_Office_Excel_Macro-Enabled_Worksheet.xlsm",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcelChart(L"embeddings", L"Microsoft_Office_Excel_Chart.xlsx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");


		const FileType MicrosoftOfficeWordDocument(L"embeddings", L"Microsoft_Office_Word_Document.docx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeWord_97_2003_Document(L"embeddings", L"Microsoft_Office_Word_97_-_2003_Document.doc",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficeWordMacro_EnabledDocument(L"embeddings", L"Microsoft_Office_Word_Macro-Enabled_Document.docm",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");


		const FileType MicrosoftOfficePowerPointPresentation(L"embeddings", L"Microsoft_Office_PowerPoint_Presentation.pptx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficePowerPoint_97_2003_Presentation(L"embeddings", L"Microsoft_Office_PowerPoint_97-2003_Presentation.xlsx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficePowerPointMacro_EnabledPresentation(L"embeddings", L"Microsoft_Office_PowerPoint_Macro-Enabled_Presentation.pptm",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");
		

		const FileType MicrosoftOfficePowerPointSlide(L"embeddings", L"Microsoft_Office_PowerPoint_Slide.sldx",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficePowerPointMacro_EnabledSlide(L"embeddings", L"Microsoft_Office_PowerPoint_Macro-Enabled_Slide.sldm",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");
				
		
		const FileType OleObject(L"embeddings", L"oleObject.bin",
															"",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");
		
		const FileType Glossary(L"glossary", L"document.xml",
															"application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument");

		const FileType Slide(L"", L"slide.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.slide+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide");

		const FileType SlideLayout(L"", L"slideLayout.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout");

		const FileType SlideMaster(L"", L"slideMaster.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster");

		const FileType NotesSlide(L"", L"notesSlide.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.notesSlide+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide");

		const FileType NotesMaster(L"", L"notesMaster.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster");

		const FileType HandoutMaster(L"", L"handoutMaster.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.handoutMaster+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster");

		const FileType Presentation(L"ppt", L"presentation.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType PresProps(L"", L"presProps.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.presProps+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps");

		const FileType TableStyles(L"", L"tableStyles.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles");

		const FileType ViewProps(L"", L"viewProps.xml",
															"application/vnd.openxmlformats-officedocument.presentationml.viewProps+xml",
															"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps");

		const FileType ThemePPTX(L"", L"theme.xml",
													"application/vnd.openxmlformats-officedocument.theme+xml",
													"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme");

		const FileType VmlDrawing(L"", L"vmlDrawing.vml",
													"",
													"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing");

		const FileType Unknow(L"", L"", "", "");

	} // namespace FileTypes
} // namespace OOX

#endif // OOX_FILE_TYPES_INCLUDE_H_
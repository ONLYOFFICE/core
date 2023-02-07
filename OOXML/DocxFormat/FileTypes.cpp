/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "FileTypes.h"

namespace OOX
{
	namespace FileTypes
	{
		const FileType App				(L"docProps", L"app.xml",
												L"application/vnd.openxmlformats-officedocument.extended-properties+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");

		const FileType Core				(L"docProps", L"core.xml",
												L"application/vnd.openxmlformats-package.core-properties+xml",
												L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");

		const FileType CustomProperties	(L"docProps", L"custom.xml",
												L"application/vnd.openxmlformats-officedocument.custom-properties+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties");

		const FileType Document			(L"word", L"document.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType DocumentMacro	(L"word", L"document.xml",
												L"application/vnd.ms-word.document.macroEnabled.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

		const FileType DocumentFlat		(L"", L"", L"", L"");

		const FileType Theme			(L"theme", L"theme.xml",
												L"application/vnd.openxmlformats-officedocument.theme+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme", 
												L"theme", true);
		 
		const FileType ThemeOverride	(L"themeOverride", L"themeOverride.xml",
												L"application/vnd.openxmlformats-officedocument.theme+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/themeOverride", 
												L"themeOverride", true);

		const FileType Setting			(L"", L"settings.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings");

		const FileType FontTable		(L"", L"fontTable.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable");

		const FileType Style			(L"", L"styles.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");

		const FileType Bibliography		(L"customXml", L"item.xml",
												L"WARNING not implement",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/bibliography");

		const FileType FootNote			(L"", L"footnotes.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes");

		const FileType EndNote			(L"", L"endnotes.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes");

		const FileType WebSetting		(L"", L"webSettings.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings");

		const FileType Header			(L"", L"header.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.header+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header", L"header", true);

		const FileType Footer			(L"", L"footer.xml", 
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.footer+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", L"footer", true);

		const FileType Numbering		(L"", L"numbering.xml", 
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering");

		const FileType Comments			(L"", L"comments.xml", 
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments");

		const FileType DocumentComments		(L"", L"commentsDocument.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.comments+xml",
												L"http://schemas.onlyoffice.com/commentsDocument");

		const FileType CommentsExt		(L"", L"commentsExtended.xml", 
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/commentsExtended");

		const FileType DocumentCommentsExt		(L"", L"commentsExtendedDocument.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtended+xml",
												L"http://schemas.onlyoffice.com/commentsExtendedDocument");

		const FileType CommentsExtensible		(L"", L"commentsExtensible.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtensible+xml",
												L"http://schemas.microsoft.com/office/2018/08/relationships/commentsExtensible");

		const FileType DocumentCommentsExtensible (L"", L"commentsExtensibleDocument.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtensible+xml",
												L"http://schemas.onlyoffice.com/commentsExtensibleDocument");

		const FileType CommentsIds		(L"", L"commentsIds.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsIds+xml",
												L"http://schemas.microsoft.com/office/2016/09/relationships/commentsIds");

		const FileType DocumentCommentsIds		(L"", L"commentsIdsDocument.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsIds+xml",
												L"http://schemas.onlyoffice.com/commentsIdsDocument");

		const FileType CommentsUserData		(L"", L"commentsUserData.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.commentsExtensible+xml",
												L"http://schemas.onlyoffice.com/commentsUserData");

		const FileType People			(L"", L"people.xml", 
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.people+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/people");

		const FileType DocumentPeople			(L"", L"peopleDocument.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.people+xml",
												L"http://schemas.onlyoffice.com/peopleDocument");

		const FileType CustomXml		(L"customXml", L"item.xml",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml",
												L"customXml/item", true, true);

		const FileType CustomXmlProps	(L"", L"itemProps.xml",
												L"application/vnd.openxmlformats-officedocument.customXmlProperties+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps",
												L"customXml/itemProps", true, true);

		const FileType HyperLink		(L"", L"", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink");

		const FileType Image			(L"media", L"image", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image", L"image", true);

		const FileType Audio			(L"media", L"audio", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio", L"audio", true);

		const FileType Video			(L"media", L"video", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/video", L"video", true);

		const FileType Media			(L"media", L"media", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/media", L"media", true);

		const FileType DiagramData		(L"diagrams", L"data.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramData+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramData", 
												L"diagrams/data", true, true);

		const FileType DiagramQuickStyle(L"diagrams", L"quickStyle.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramQuickStyle+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramQuickStyle",
												L"diagrams/quickStyle", true, true);

		const FileType DiagramLayout	(L"diagrams", L"layout.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramLayout+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramLayout",
												L"diagrams/layout", true, true);

		const FileType DiagramColors	(L"diagrams", L"colors.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramColors+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramColors",
												L"diagrams/colors", true, true);

		const FileType DiagramDrawing	(L"diagrams", L"drawing.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramDrawing+xml",
												L"http://schemas.microsoft.com/office/2007/relationships/diagramDrawing", 
												L"diagrams/drawing", true, true);

		const FileType ChartDrawing			(L"drawings", L"drawing.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.chartshapes+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartUserShapes", 
												L"drawings/drawing", true, true);		

		const FileType Layout				(L"diagrams", L"layout.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramLayout+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramLayout", 
												L"diagrams/layout", true, true);

		const FileType Colors				(L"diagrams", L"colors.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramColors+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramColors", 
												L"diagrams/colors", true, true);

		const FileType QuickStyle			(L"diagrams", L"quickStyle.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.diagramStyle+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramQuickStyle",
												L"diagrams/quickStyle", true, true);

		const FileType Chart				(L"charts", L"chart.xml",
												L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart", 
												L"charts/chart", true, true);
		
		const FileType ChartEx				(L"charts", L"chartEx.xml",
												L"application/vnd.ms-office.chartex+xml",
												L"http://schemas.microsoft.com/office/2014/relationships/chartEx", 
												L"charts/chartEx", true, true);

		const FileType ChartStyle			(L"", L"style.xml",
												L"application/vnd.ms-office.chartstyle+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/chartStyle", 
												L"charts/style", true, true);

		const FileType ChartColors			(L"", L"colors.xml",
												L"application/vnd.ms-office.chartcolorstyle+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/chartColorStyle", 
												L"charts/colors", true, true);

		const FileType ActiveX_xml			(L"activeX", L"activeX.xml",
												L"application/vnd.ms-office.activeX+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/control",
												L"activeX/activeX_xml", true, true);
		
		const FileType ActiveX_bin			(L"", L"activeX.bin",
												L"application/vnd.ms-office.activeX",
												L"http://schemas.microsoft.com/office/2006/relationships/activeXControlBinary",
												L"activeX/activeX_bin", true, true);

		const FileType VbaProject			(L"", L"vbaProject.bin",
												L"application/vnd.ms-office.vbaProject",
												L"http://schemas.microsoft.com/office/2006/relationships/vbaProject");

		const FileType VbaData				(L"", L"vbaData.xml",
												L"application/vnd.ms-word.vbaData+xml",
												L"http://schemas.microsoft.com/office/2006/relationships/wordVbaData");

		const FileType JsaProject			(L"", L"jsaProject.bin",
												L"",
												L"http://schemas.onlyoffice.com/jsaProject");

		const FileType ImportedExternalContent(L"embeddings", L"",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/aFChunk");

		const FileType MicrosoftOfficeUnknown(L"embeddings", L"",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcelWorksheet(L"embeddings", L"Microsoft_Office_Excel_Worksheet.xlsx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcel_97_2003_Worksheet(L"embeddings", L"Microsoft_Office_Excel_97-2003_Worksheet.xls",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficeExcelBinaryWorksheet(L"embeddings", L"Microsoft_Office_Excel_Binary_Worksheet.xlsb",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcelMacro_EnabledWorksheet
											(L"embeddings", L"Microsoft_Office_Excel_Macro-Enabled_Worksheet.xlsm",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeExcelChart
											(L"embeddings", L"Microsoft_Office_Excel_Chart.xlsx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");


		const FileType MicrosoftOfficeWordDocument
											(L"embeddings", L"Microsoft_Office_Word_Document.docx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficeWord_97_2003_Document(L"embeddings", L"Microsoft_Office_Word_97_-_2003_Document.doc",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficeWordMacro_EnabledDocument
											(L"embeddings", L"Microsoft_Office_Word_Macro-Enabled_Document.docm",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");


		const FileType MicrosoftOfficePowerPointPresentation(L"embeddings", L"Microsoft_Office_PowerPoint_Presentation.pptx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficePowerPoint_97_2003_Presentation
											(L"embeddings", L"Microsoft_Office_PowerPoint_97-2003_Presentation.xlsx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");

		const FileType MicrosoftOfficePowerPointMacro_EnabledPresentation
											(L"embeddings", L"Microsoft_Office_PowerPoint_Macro-Enabled_Presentation.pptm",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");
		

		const FileType MicrosoftOfficePowerPointSlide
											(L"embeddings", L"Microsoft_Office_PowerPoint_Slide.sldx",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType MicrosoftOfficePowerPointMacro_EnabledSlide
											(L"embeddings", L"Microsoft_Office_PowerPoint_Macro-Enabled_Slide.sldm",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package");

		const FileType OleObject			(L"embeddings", L"oleObject.bin",
												L"",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject");
		
		const FileType VmlDrawing			(L"drawings", L"vmlDrawing.vml",
												L"application/vnd.openxmlformats-officedocument.vmlDrawing",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing", 
												L"drawings/vmlDrawing", true, true);

		const FileType ExternalLinkPath		(L"", L"", 
												L"", 
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLinkPath");

		const FileType ExternalLinkLibrary	(L"", L"", 
												L"", 
												L"http://schemas.microsoft.com/office/2006/relationships/xlExternalLinkPath/xlLibrary");
		
		const FileType ExternalLinkPathMissing(L"", L"", 
												L"", 
												L"http://schemas.microsoft.com/office/2006/relationships/xlExternalLinkPath/xlPathMissing");

		const FileType ExternalLinkPathStartup(L"", L"", 
												L"", 
												L"http://schemas.microsoft.com/office/2006/relationships/xlExternalLinkPath/xlStartup");

		const FileType ExternalLinkPathAlternateStartup(L"", L"", 
												L"", 
												L"http://schemas.microsoft.com/office/2006/relationships/xlExternalLinkPath/xlAlternateStartup");

		const FileType LegacyDiagramText	(L"", L"legacyDiagramText.bin",
												L"",
												L"http://schemas.microsoft.com/office/2006/relationships/legacyDiagramText");

		const FileType GlossaryDocument		(L"glossary", L"document.xml",
												L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.glossary+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument");

		const FileType Unknow				(L"", L"", L"", L"");

	} // namespace FileTypes
} // namespace OOX


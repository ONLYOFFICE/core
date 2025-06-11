#include "Converter2OOXML.h"

#include <cfloat>

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../DesktopEditor/graphics/pro/Graphics.h"

#include "../Paragraph/ParaText.h"
#include "../Paragraph/CtrlTable.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlShapeArc.h"
#include "../Paragraph/CtrlShapePolygon.h"
#include "../Paragraph/CtrlShapeCurve.h"

#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordCharShape.h"

#include "Transform.h"

#define PARA_SPACING_SCALE 0.85

namespace HWP
{

enum class EShapeObjectType
{
	Arc,
	Curve,
	Ellipse,
	Line,
	Ole,
	Picture,
	Polygon,
	Rectangle,
	Unknown
};

EShapeObjectType GetShapeObjectType(const std::wstring& wsID)
{
	if (L"cra$" == wsID)
		return EShapeObjectType::Arc;
	if (L"ruc$" == wsID)
		return EShapeObjectType::Curve;
	if (L"lle$" == wsID)
		return EShapeObjectType::Ellipse;
	if (L"nil$" == wsID || L"loc$" == wsID)
		return EShapeObjectType::Line;
	if (L"elo$" == wsID)
		return EShapeObjectType::Ole;
	if (L"cip$" == wsID)
		return EShapeObjectType::Picture;
	if (L"lop$" == wsID)
		return EShapeObjectType::Polygon;
	if (L"cer$" == wsID)
		return EShapeObjectType::Rectangle;

	return EShapeObjectType::Unknown;
}

CConverter2OOXML::CConverter2OOXML()
	: m_pContext(nullptr), m_ushShapeCount(0), m_ushPageCount(1), m_ushTableCount(0),
	  m_ushEquationCount(0), m_ushBookmarkCount(0)
{}

CConverter2OOXML::~CConverter2OOXML()
{}

void CConverter2OOXML::SetContext(CWriterContext* pContext)
{
	m_pContext = pContext;
}

void CConverter2OOXML::SetTempDirectory(const HWP_STRING& sTempDirectory)
{
	m_sTempDirectory = sTempDirectory;
	m_oOleConverter.SetTempDir(m_sTempDirectory);
}

void CConverter2OOXML::CreateEmptyFiles()
{
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/_rels");
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/docProps");
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/word");
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/word/_rels");
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/word/media");
	NSDirectory::CreateDirectory(m_sTempDirectory + L"/word/theme");

	// theme1.xml
	std::wstring wsTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:majorFont><a:minorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
	NSFile::CFileBinary oThemeWriter;
	if (oThemeWriter.CreateFileW(m_sTempDirectory + L"/word/theme/theme1.xml"))
	{
		oThemeWriter.WriteStringUTF8(wsTheme);
		oThemeWriter.CloseFile();
	}

	// app.xml
	std::wstring wsApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
	if (wsApplication.empty())
		wsApplication = NSSystemUtils::gc_EnvApplicationNameDefault;

	std::wstring wsApp = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
	wsApp += wsApplication;
	wsApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
	NSFile::CFileBinary oAppWriter;
	if (oAppWriter.CreateFileW(m_sTempDirectory + L"/docProps/app.xml"))
	{
		oAppWriter.WriteStringUTF8(wsApp);
		oAppWriter.CloseFile();
	}

	// .rels
	std::wstring wsRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/><Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/></Relationships>";
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(m_sTempDirectory + L"/_rels/.rels"))
	{
		oRelsWriter.WriteStringUTF8(wsRels);
		oRelsWriter.CloseFile();
	}

	// fontTable.xml
	std::wstring wsFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
	NSFile::CFileBinary oFontTableWriter;
	if (oFontTableWriter.CreateFileW(m_sTempDirectory + L"/word/fontTable.xml"))
	{
		oFontTableWriter.WriteStringUTF8(wsFontTable);
		oFontTableWriter.CloseFile();
	}

	// settings.xml
	std::wstring wsSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
	NSFile::CFileBinary oSettingsWriter;
	if (oSettingsWriter.CreateFileW(m_sTempDirectory + L"/word/settings.xml"))
	{
		oSettingsWriter.WriteStringUTF8(wsSettings);
		oSettingsWriter.CloseFile();
	}

	// core.xml
	std::wstring wsCore = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
	wsCore += L"<cp:lastModifiedBy/></cp:coreProperties>";
	NSFile::CFileBinary oCoreWriter;
	if (oCoreWriter.CreateFileW(m_sTempDirectory + L"/docProps/core.xml"))
	{
		oCoreWriter.WriteStringUTF8(wsCore);
		oCoreWriter.CloseFile();
	}

	FillDefaultData();
}

void CConverter2OOXML::FillDefaultData()
{
	// Заполняем начало файлов
	AddRelationship(L"styles", L"styles.xml");
	AddRelationship(L"settings", L"settings.xml");
	AddRelationship(L"webSettings", L"webSettings.xml");
	AddRelationship(L"fontTable", L"fontTable.xml");
	AddRelationship(L"theme", L"theme/theme1.xml");

	AddContentType(L"document.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
	AddContentType(L"styles.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
	AddContentType(L"settings.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");
	AddContentType(L"webSettings.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml");
	AddContentType(L"fontTable.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
	AddContentType(L"theme/theme1.xml", L"application/vnd.openxmlformats-officedocument.theme+xml");

	m_oNoteXmlRels.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
	m_oDocXml     .WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>");
	m_oWebSettings.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/>");
}

void CConverter2OOXML::Close()
{
	// Дописываем концы файлов
	m_oDocXml.WriteString(L"</w:body></w:document>");

	NSFile::CFileBinary oDocumentWriter;
	if (oDocumentWriter.CreateFileW(m_sTempDirectory + L"/word/document.xml"))
	{
		oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
		oDocumentWriter.CloseFile();
	}

	if (m_oFootnoteConverter.SaveToFile(m_sTempDirectory + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR))
	{
		AddRelationship(L"footnotes", L"footnotes.xml");
		AddRelationship(L"endnotes", L"endnotes.xml");

		AddContentType(L"footnotes.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml");
		AddContentType(L"endnotes.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.endnotes+xml");
	}

	// styles.xml
	m_oStyleConverter.SaveToFile(m_sTempDirectory + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR);

	// numbering.xml
	if (m_oNumberingConverter.SaveToFile(m_sTempDirectory + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR))
	{
		AddRelationship(L"numbering", L"numbering.xml");

		AddContentType(L"numbering.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml");
	}

	// webSettings.xml
	m_oWebSettings.WriteString(L"</w:webSettings>");
	NSFile::CFileBinary oWebSettingsWriter;
	if (oWebSettingsWriter.CreateFileW(m_sTempDirectory + L"/word/webSettings.xml"))
	{
		oWebSettingsWriter.WriteStringUTF8(m_oWebSettings.GetData());
		oWebSettingsWriter.CloseFile();
	}

	// [Content_Types].xml
	NSFile::CFileBinary oContentTypeWriter;
	if (oContentTypeWriter.CreateFileW(m_sTempDirectory + L"/[Content_Types].xml"))
	{
		oContentTypeWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");
		oContentTypeWriter.WriteStringUTF8(L"<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>");

		for (const TContentType& oContentType : m_arDefaultContentType)
			oContentTypeWriter.WriteStringUTF8(L"<Default Extension=\"" + oContentType.m_wsName + L"\" ContentType=\"" + oContentType.m_wsType + L"\"/>");

		for (const TContentType& oContentType : m_arContentTypes)
			oContentTypeWriter.WriteStringUTF8(L"<Override PartName=\"/word/" + oContentType.m_wsName + L"\" ContentType=\"" + oContentType.m_wsType + L"\"/>");

		oContentTypeWriter.WriteStringUTF8(L"<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/>");
		oContentTypeWriter.WriteStringUTF8(L"<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/>");

		oContentTypeWriter.WriteStringUTF8(L"</Types>");
		oContentTypeWriter.CloseFile();
	}

	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(m_sTempDirectory + L"/word/_rels/document.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

		for (const TRelationship& oRelationship : m_arRelationships)
		{
			oRelsWriter.WriteStringUTF8(L"<Relationship Id=\"" + oRelationship.m_wsID + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/" + oRelationship.m_wsType + L"\" Target=\"" + oRelationship.m_wsTarget + L'\"');

			if (L"hyperlink" == oRelationship.m_wsType)
				oRelsWriter.WriteStringUTF8(L" TargetMode=\"External\"/>");
			else
				oRelsWriter.WriteStringUTF8(L"/>");
		}

		oRelsWriter.WriteStringUTF8(L"</Relationships>");

		oRelsWriter.CloseFile();
	}
}

void CConverter2OOXML::Convert()
{
	if (nullptr == m_pContext)
		return;

	TConversionState oState;

	std::vector<const CHWPSection*> arSetcions{m_pContext->GetSections()};

	for (const CHWPSection* pSection : arSetcions)
	{
		const bool bIsLastSection = pSection == arSetcions.back();

		for (const CHWPPargraph *pPara : pSection->GetParagraphs())
			WriteParagraph(pPara, m_oDocXml, oState);

		if (!bIsLastSection)
			m_oDocXml.WriteString(L"<w:p><w:pPr>");

		WriteSectionSettings(oState);

		if (!bIsLastSection)
			m_oDocXml.WriteString(L"</w:pPr></w:p>");

		++oState.m_ushSecdIndex;
	}
}

bool CConverter2OOXML::IsRasterFormat(const HWP_STRING& sFormat)
{
	return L"png" == sFormat || L"jpg" == sFormat || L"jpeg" == sFormat || L"bmp" == sFormat;
}

void CConverter2OOXML::WriteCharacter(const CCtrlCharacter* pCharacter, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pCharacter)
		return;

	switch (pCharacter->GetType())
	{
		case ECtrlCharType::PARAGRAPH_BREAK:
		{
			if (!oState.m_bOpenedP)
				WriteEmptyParagraph(shParaShapeID, shParaStyleID, pCharacter->GetCharShapeId(), oBuilder, oState);
			else
				CloseParagraph(oBuilder, oState);

			break;
		}
		case ECtrlCharType::LINE_BREAK:
		{
			oState.m_eBreakType = TConversionState::EBreakType::TextWrapping;
			break;
		}
		case ECtrlCharType::HARD_HYPHEN:
		case ECtrlCharType::HARD_SPACE:
			break;
	}
}

void CConverter2OOXML::WriteShape(const CCtrlGeneralShape* pShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pShape || oState.m_bInTextBox)
		return;

	switch (pShape->GetShapeType())
	{
		case EShapeType::Arc:
		case EShapeType::Ellipse:
		case EShapeType::Rect:
		case EShapeType::Line:
		case EShapeType::Polygon:
		case EShapeType::Curve:
		{
			WriteGeometryShape(pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::Pic:
		{
			WritePicture((const CCtrlShapePic*)pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::EqEdit:
		{
			WriteEqEditShape((const CCtrlEqEdit*)pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::Ole:
		{
			WriteOleShape((const CCtrlShapeOle*)pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::Video:
		{
			WriteVideo((const CCtrlShapeVideo*)pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::Container:
		{
			WriteContainer((const CCtrlContainer*)pShape, shParaShapeID, shParaStyleID, oBuilder, oState);
			break;
		}
		case EShapeType::GeneralShape:
		case EShapeType::ConnectLine:
		case EShapeType::TextArt:
			break;
	}
}

void CConverter2OOXML::WriteNote(const CCtrlNote* pNote, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	oBuilder.WriteString(L"<w:r>");

	CRunnerStyle oRunnerStyle;
	oRunnerStyle.SetVerticalAlign(EVerticalAlignRun::Superscript);

	WriteRunnerStyle(oState.m_ushLastCharShapeId, oBuilder, oState, oRunnerStyle);

	oBuilder.WriteString(m_oFootnoteConverter.CreateNote((const CCtrlNote*)pNote, *this));

	oBuilder.WriteString(L"</w:r>");
}

void CConverter2OOXML::WriteField(const CCtrlField* pShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pShape)
		return;

	switch (pShape->GetType())
	{
		case EFieldType::Hyperlink:
		{
			HWP_STRING wsHref = pShape->GetStringParam(L"Path");

			if(wsHref.empty())
			{
				HWP_STRING sCommand = pShape->GetStringParam(L"Command");

				if (sCommand.empty())
					sCommand = pShape->GetCommand();

				sCommand = sCommand.substr(0, sCommand.find(L';'));

				size_t unFound = sCommand.find(L'\\');

				while (HWP_STRING::npos != unFound)
				{
					sCommand.erase(unFound, 1);
					unFound = sCommand.find(L'\\', unFound);
				}

				wsHref = sCommand;
			}

			if (wsHref.empty())
				break;

			const HWP_STRING wsID = AddRelationship(L"hyperlink", wsHref, &oState);

			OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);
			oBuilder.WriteString(L"<w:hyperlink r:id=\"" + wsID + L"\">");

			oState.m_mOpenField.insert(std::make_pair(pShape->GetInstanceID(), pShape));

			break;
		}
		case EFieldType::HyperlinkClosing:
		{
			oBuilder.WriteString(L"</w:hyperlink>");
			break;
		}
		case EFieldType::Bookmark:
		{
			oBuilder.WriteString(L"<w:bookmarkStart w:id=\"" + std::to_wstring(m_ushBookmarkCount) + L"\" w:name=\"");
			oBuilder.WriteEncodeXmlString(pShape->GetStringParam(L"bookmarkname"));
			oBuilder.WriteString(L"\"/>");

			oState.m_arOpenedBookmarks.push(m_ushBookmarkCount++);
			oState.m_mOpenField.insert(std::make_pair(pShape->GetInstanceID(), pShape));

			break;
		}
		case EFieldType::BookmarkClosing:
		{
			if (oState.m_arOpenedBookmarks.empty())
				break;

			oBuilder.WriteString(L"<w:bookmarkEnd w:id=\"" + std::to_wstring(oState.m_arOpenedBookmarks.top()) + L"\"/>");
			oState.m_arOpenedBookmarks.pop();
			break;
		}
		//TODO:: как-будто хочется определить тип закрывающей field на этапе парса hwpx
		case EFieldType::Unknown:
		{
			std::map<unsigned int, const CCtrlField*>::const_iterator itFound = oState.m_mOpenField.find(pShape->GetInstanceID());

			if (oState.m_mOpenField.cend() != itFound)
			{
				switch (itFound->second->GetType())
				{
					case EFieldType::Hyperlink:
					{
						oBuilder.WriteString(L"</w:hyperlink>");
						break;
					}
					case EFieldType::Bookmark:
					{
						if (oState.m_arOpenedBookmarks.empty())
							break;

						oBuilder.WriteString(L"<w:bookmarkEnd w:id=\"" + std::to_wstring(oState.m_arOpenedBookmarks.top()) + L"\"/>");
						oState.m_arOpenedBookmarks.pop();
						break;
					}
					default:
						break;
				}
			}

			break;
		}
	}
}

void CConverter2OOXML::WriteCaption(const CCtrlCommon* pCtrlCommon, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pCtrlCommon || !pCtrlCommon->HaveCaption())
		return;

	for (const CCapParagraph* pCapParagraph : pCtrlCommon->GetCaptionParas())
		WriteParagraph(pCapParagraph, oBuilder, oState);
}

void CConverter2OOXML::WriteEmptyParagraph(short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (oState.m_bOpenedP || oState.m_bLastEmptyNode)
		return;

	oBuilder.WriteString(L"<w:p>");
	oBuilder.WriteString(L"<w:pPr>");

	WriteParaShapeProperties(shParaShapeID, shParaStyleID, oBuilder, oState);
	WriteRunnerStyle(shCharShapeID, oBuilder, oState);

	oBuilder.WriteString(L"</w:pPr>");
	oBuilder.WriteString(L"</w:p>");

	oState.m_bLastEmptyNode = true;
}

void CConverter2OOXML::WriteParagraph(const CHWPPargraph* pParagraph, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pParagraph)
		return;

	CloseParagraph(oBuilder, oState);

	if (0 < pParagraph->GetBreakType())
	{
		if ((0x04 == (pParagraph->GetBreakType() & 0x04)) && (0 < oState.m_unParaIndex))
		{
			oState.m_eBreakType = TConversionState::EBreakType::Page;
			// oBuilder.WriteString(L"<w:r><w:br w:type=\"page\"/></w:r>");
			++m_ushPageCount;
		}
		else if (0x08 == (pParagraph->GetBreakType() & 0x08))
			oState.m_eBreakType = TConversionState::EBreakType::Column;
			// oBuilder.WriteString(L"<w:r><w:br w:type=\"column\"/></w:r>");
	}

	++oState.m_unParaIndex;

	for (const CCtrl* pCtrl : pParagraph->GetCtrls())
	{
		switch (pCtrl->GetCtrlType())
		{
			case ECtrlObjectType::ParaText:
			{
				WriteText((const CParaText*)pCtrl, pParagraph->GetRangeTags(), pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oState);
				// WriteText(((const CParaText*)pCtrl)->GetText(), pParagraph->GetShapeID(), ((const CParaText*)pCtrl)->GetCharShapeID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::Character:
			{
				WriteCharacter((const CCtrlCharacter*)pCtrl, pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::Shape:
			{
				WriteShape((const CCtrlGeneralShape*)pCtrl, pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::Table:
			{
				WriteTable((const CCtrlTable*)pCtrl, pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::Note:
			{
				WriteNote((const CCtrlNote*)pCtrl, pParagraph->GetShapeID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::SectionDef:
			{
				oState.m_pSectionDef = (const CCtrlSectionDef*)pCtrl;
				break;
			}
			case ECtrlObjectType::ColumnDef:
			{
				oState.m_pColumnDef  = (const CCtrlColumnDef*)(pCtrl);
				break;
			}
			case ECtrlObjectType::AutoNumber:
			{
				WriteAutoNumber((const CCtrlAutoNumber*)pCtrl, pParagraph->GetShapeID(), pParagraph->GetStyleID(), oState.m_ushLastCharShapeId, oBuilder, oState);
				break;
			}
			case ECtrlObjectType::Field:
			{
				WriteField((const CCtrlField*)pCtrl, pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oState);
				break;
			}
			case ECtrlObjectType::HeadFoot:
			{
				if (EHanType::HWPX == m_pContext->GetType())
					oState.m_arCtrlsHeadFoot.push_back((const CCtrlHeadFoot*)pCtrl);
				break;
			}
			case ECtrlObjectType::PageNumPos:
			{
				oState.m_pPageNum = dynamic_cast<const CCtrlPageNumPos*>(pCtrl);
				break;
			}
			case ECtrlObjectType::NewNumber:
			{
				oState.m_pNewNumber = dynamic_cast<const CCtrlNewNumber*>(pCtrl);
				break;
			}
			default:
				break;
		}
	}

	CloseParagraph(oBuilder, oState);
	oState.m_bLastEmptyNode = false;
}

void CConverter2OOXML::WriteParagraphProperties(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	oBuilder.WriteString(L"<w:pPr>");

	WriteParaShapeProperties(shParaShapeID, shParaStyleID, oBuilder, oState);

	oBuilder.WriteString(L"</w:pPr>");
}

void CConverter2OOXML::WriteParaShapeProperties(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == m_pContext)
		return;

	const std::wstring wsStyleId = m_oStyleConverter.CreateStyle(shParaShapeID, shParaStyleID, *m_pContext, oState);

	if (!wsStyleId.empty())
	{
		oBuilder.WriteString(L"<w:pStyle w:val=\"");
		oBuilder.WriteEncodeXmlString(wsStyleId);
		oBuilder.WriteString(L"\"/>");
	}

	if (oState.m_bInTable)
		oBuilder.WriteString(L"<w:wordWrap w:val=\"1\"/>");

	if (m_oStyleConverter.GetLastParaShapeId() != shParaShapeID)
		m_oStyleConverter.WriteDifferenceParagraphStyles(m_oStyleConverter.GetLastParaShapeId(), shParaShapeID, *m_pContext, oBuilder);

	const CHWPRecordParaShape* pParaShape= dynamic_cast<const CHWPRecordParaShape*>(m_pContext->GetParaShape(shParaShapeID));

	if (nullptr == pParaShape)
		return;

	switch (pParaShape->GetHeadingType())
	{
		case EHeadingType::NUMBER:
		case EHeadingType::BULLET:
		{
			const int nNumId = m_oNumberingConverter.CreateNumbering(dynamic_cast<const CHWPRecordNumbering*>(m_pContext->GetNumbering(pParaShape->GetHeadingIdRef())), pParaShape->GetHeadingType(), *this);

			if (0 == nNumId)
				break;

			oBuilder.WriteString(L"<w:numPr>");

			oBuilder.WriteString(L"<w:ilvl w:val=\"" + std::to_wstring((int)pParaShape->GetHeadingLevel()) + L"\"/>");
			oBuilder.WriteString(L"<w:numId w:val=\"" + std::to_wstring(nNumId) + L"\"/>");

			oBuilder.WriteString(L"</w:numPr>");

			break;
		}
		case EHeadingType::OUTLINE:
		case EHeadingType::NONE:
			break;
	}
}

void CConverter2OOXML::WriteTable(const CCtrlTable* pTable, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pTable || pTable->Empty())
		return;

	CloseParagraph(oBuilder, oState);

	++m_ushTableCount;

	oBuilder.WriteString(L"<w:tbl>");

	oState.m_bInTable = true;
	oState.m_bLastEmptyNode = false;

	WriteTableProperties(pTable, shParaShapeID, shParaStyleID, oBuilder, oState);

	std::vector<std::vector<std::pair<ECellCreator, const CTblCell*>>> m_arrCells(pTable->GetRows());

	for (unsigned int unRowIndex = 0; unRowIndex < pTable->GetRows(); ++unRowIndex)
	{
		m_arrCells[unRowIndex].resize(pTable->GetCols());

		for (unsigned int unColIndex  = 0; unColIndex < pTable->GetCols(); ++unColIndex)
			m_arrCells[unRowIndex][unColIndex] = std::make_pair(ECellCreator::NOT_CREATED, nullptr);
	}

	for (unsigned int unCellIndex = 0; unCellIndex < pTable->GetCountCells(); ++unCellIndex)
	{
		const CTblCell* pCell = pTable->GetCell(unCellIndex);

		if (nullptr == pCell || pCell->GetRowAddr() >= pTable->GetRows() || pCell->GetColAddr() >= pTable->GetCols())
			continue;

		m_arrCells[pCell->GetRowAddr()][pCell->GetColAddr()] = std::make_pair(ECellCreator::FILE, pCell);
	}

	for (unsigned int unRowIndex = 0; unRowIndex < pTable->GetRows(); ++unRowIndex)
	{
		for (unsigned int unColIndex  = 0; unColIndex < pTable->GetCols(); ++unColIndex)
		{
			std::pair<ECellCreator, const CTblCell*> oValue = m_arrCells[unRowIndex][unColIndex];

			if (ECellCreator::NOT_CREATED == oValue.first)
				continue;

			if (ECellCreator::FILE == oValue.first)
			{
				if (1 != oValue.second->GetRowSpan())
				{
					for (unsigned int unIndex = 1; unIndex < oValue.second->GetRowSpan() && unRowIndex + unIndex < pTable->GetRows(); ++unIndex)
						m_arrCells[unRowIndex + unIndex][unColIndex] = std::make_pair(ECellCreator::EMPTY, oValue.second);
				}
			}

			unColIndex += oValue.second->GetColSpan() - 1;
		}
	}

	//TODO:: в случаях, когда есть пустые столбцы необходимо добавить возможность удаления данных столбцов
	// Например для матрицы 3x2, у которой значения есть только в 2x2, необходимо удалить последний столбец

	for (unsigned int unRowIndex = 0; unRowIndex < pTable->GetRows(); ++unRowIndex)
	{
		oBuilder.WriteString(L"<w:tr>");

		int nHeight = 0;

		for (unsigned int unColIndex  = 0; unColIndex < pTable->GetCols(); ++unColIndex)
		{
			std::pair<ECellCreator, const CTblCell*> oValue = m_arrCells[unRowIndex][unColIndex];
			if (ECellCreator::FILE == oValue.first && 1 == oValue.second->GetRowSpan())
				nHeight = (std::max)(nHeight, oValue.second->GetHeight());
		}

		oBuilder.WriteString(L"<w:trPr>");
		oBuilder.WriteString(L"<w:trHeight w:val=\"" + std::to_wstring(Transform::HWPUINT2Twips(nHeight)) + L"\"/>");
		oBuilder.WriteString(L"</w:trPr>");

		for (unsigned int unColIndex  = 0; unColIndex < pTable->GetCols(); ++unColIndex)
		{
			std::pair<ECellCreator, const CTblCell*> oValue = m_arrCells[unRowIndex][unColIndex];

			WriteCell(oValue.second, oBuilder, oState, oValue.first);
		}

		oBuilder.WriteString(L"</w:tr>");
	}

	oBuilder.WriteString(L"</w:tbl>");

	oState.m_bInTable = false;
}

void CConverter2OOXML::WriteTableProperties(const CCtrlTable* pTable, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pTable)
		return;

	oBuilder.WriteString(L"<w:tblPr>");

	// TODO:: сделать вычисление
	oBuilder.WriteString(L"<w:tblW w:w=\"0\" w:type=\"auto\"/>");

	if (0 != pTable->GetInLSpace() || 0 != pTable->GetInTSpace() ||
	    0 != pTable->GetInRSpace() || 0 != pTable->GetInBSpace())
	{
		oBuilder.WriteString(L"<w:tblCellMar>");
		oBuilder.WriteString(L"<w:top w:w=\""    + std::to_wstring(Transform::HWPUINT2Twips(pTable->GetInTSpace())) + L"\" w:type=\"dxa\"/>");
		oBuilder.WriteString(L"<w:left w:w=\""   + std::to_wstring(Transform::HWPUINT2Twips(pTable->GetInLSpace())) + L"\" w:type=\"dxa\"/>");
		oBuilder.WriteString(L"<w:bottom w:w=\"" + std::to_wstring(Transform::HWPUINT2Twips(pTable->GetInBSpace())) + L"\" w:type=\"dxa\"/>");
		oBuilder.WriteString(L"<w:right w:w=\""  + std::to_wstring(Transform::HWPUINT2Twips(pTable->GetInRSpace())) + L"\" w:type=\"dxa\"/>");
		oBuilder.WriteString(L"</w:tblCellMar>");
	}

	WriteParaShapeProperties(shParaShapeID, shParaStyleID, oBuilder, oState);

	// const CHWPRecordBorderFill* pBorderFill = nullptr;

	// if (nullptr != m_pContext)
	// 	pBorderFill = dynamic_cast<const CHWPRecordBorderFill*>(m_pContext->GetBorderFill(pTable->GetBorderFillID()));

	// if (nullptr == pBorderFill)
	// {
	// 	oBuilder.WriteString(L"</w:tblPr>");
	// 	return;
	// }

	oBuilder.WriteString(L"</w:tblPr>");
}

void CConverter2OOXML::WriteCell(const CTblCell* pCell, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, ECellCreator eCellCreator)
{
	if (nullptr == pCell)
		return;

	oBuilder.WriteString(L"<w:tc>");

	oBuilder.WriteString(L"<w:tcPr>");
	oBuilder.WriteString(L"<w:tcW w:w=\"" + std::to_wstring(Transform::HWPUINT2Twips(pCell->GetWidth())) + L"\" w:type=\"dxa\"/>");

	if (1 != pCell->GetColSpan())
		oBuilder.WriteString(L"<w:gridSpan w:val=\"" + std::to_wstring(pCell->GetColSpan()) + L"\"/>");

	if (1 != pCell->GetRowSpan())
		oBuilder.WriteString(L"<w:vMerge w:val=\"" + HWP_STRING(((ECellCreator::FILE == eCellCreator) ? L"restart" : L"continue")) + L"\"/>");

	WriteCellProperties(pCell->GetBorderFillID(), oBuilder);

	switch(pCell->GetVertAlign())
	{
		case EVertAlign::TOP: oBuilder.WriteString(L"<w:vAlign w:val=\"top\"/>"); break;
		case EVertAlign::CENTER: oBuilder.WriteString(L"<w:vAlign w:val=\"center\"/>"); break;
		case EVertAlign::BOTTOM: oBuilder.WriteString(L"<w:vAlign w:val=\"bottom\"/>"); break;
		case EVertAlign::INSIDE:
		case EVertAlign::OUTSIDE:
			break;
	}

	oBuilder.WriteString(L"</w:tcPr>");

	if (ECellCreator::FILE == eCellCreator && !pCell->GetParagraphs().empty())
	{
		for (const CHWPPargraph* pParagraph : pCell->GetParagraphs())
		{
			NSStringUtils::CStringBuilder oCellBuilder;
			TConversionState oCellState;
			oCellState.m_pRelationships = oState.m_pRelationships;

			WriteParagraph(pParagraph, oCellBuilder, oCellState);

			if (0 == oCellBuilder.GetCurSize())
			{
				OpenParagraph(pParagraph->GetShapeID(), pParagraph->GetStyleID(), oBuilder, oCellState);
				CloseParagraph(oBuilder, oCellState);
			}
			else
				oBuilder.Write(oCellBuilder);
		}
	}
	else
		oBuilder.WriteString(L"<w:p><w:r></w:r></w:p>");

	oBuilder.WriteString(L"</w:tc>");
}

void CConverter2OOXML::WriteCellProperties(short shBorderFillID, NSStringUtils::CStringBuilder& oBuilder)
{
	const CHWPRecordBorderFill* pBorderFill = nullptr;

	if (nullptr != m_pContext)
		pBorderFill = dynamic_cast<const CHWPRecordBorderFill*>(m_pContext->GetBorderFill(shBorderFillID));

	if (nullptr == pBorderFill)
		return;

	if(nullptr != pBorderFill->GetFill())
	{
		const CFill* pFill{pBorderFill->GetFill()};

		if (pFill->ColorFill())
			oBuilder.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + Transform::IntColorToHEX(pFill->GetFaceColor()) + L"\"/>");
		// In OOXML, there is no gradient support for cell fills, so we fill in the first color of the gradient.
		else if (pFill->GradFill() && 0 != pFill->GetGradColorNum())
			oBuilder.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + Transform::IntColorToHEX(pFill->GetGradColors().front()) + L"\"/>");
	}

	oBuilder.WriteString(L"<w:tcBorders>");

	WriteBorder(pBorderFill->GetTopBorder(), L"top", oBuilder);
	WriteBorder(pBorderFill->GetLeftBorder(), L"left", oBuilder);
	WriteBorder(pBorderFill->GetBottomBorder(), L"bottom", oBuilder);
	WriteBorder(pBorderFill->GetRightBorder(), L"right", oBuilder);

	oBuilder.WriteString(L"</w:tcBorders>");
}

void CConverter2OOXML::WriteBorder(const TBorder& oBorder, const HWP_STRING& sBorderName, NSStringUtils::CStringBuilder& oBuilder)
{
	if (sBorderName.empty())
		return;

	HWP_STRING sType;

	//TODO:: проверить стиль линий
	switch(oBorder.m_eStyle)
	{
		case ELineStyle2::NONE: oBuilder.WriteString(L"<w:" + sBorderName + L" w:val=\"none\"/>"); return;
		case ELineStyle2::SOLID: sType = L"single"; break;
		case ELineStyle2::DASH: sType = L"dashed"; break;
		case ELineStyle2::DOT: sType = L"dotted"; break;
		case ELineStyle2::DASH_DOT: sType = L"dotDash"; break;
		case ELineStyle2::DASH_DOT_DOT: sType = L"dotDotDash"; break;
		case ELineStyle2::LONG_DASH: sType = L"dashed"; break;
		case ELineStyle2::CIRCLE: sType = L"dashed"; break;
		case ELineStyle2::DOUBLE_SLIM: sType = L"double"; break;
		case ELineStyle2::SLIM_THICK: sType = L"thinThickSmallGap"; break;
		case ELineStyle2::THICK_SLIM: sType = L"thickThinSmallGap"; break;
		case ELineStyle2::SLIM_THICK_SLIM: sType = L"thinThickThinSmallGap"; break;
	}

	oBuilder.WriteString(L"<w:" + sBorderName + L" w:val=\"" + sType + L"\" w:sz=\"" + std::to_wstring(Transform::LineWidth2Pt((short)oBorder.m_chWidth)) + L"\" w:color=\"" + Transform::IntColorToHEX(oBorder.m_nColor) + L"\"/>");
}

VECTOR<TPoint> ArcToBezier(const TPoint& oStart, const TPoint& oEnd, const TPoint& oCenter)
{
	const double dRadiusX = std::abs(oStart.m_nX - oCenter.m_nX);
	const double dRadiusY = std::abs(oStart.m_nY - oCenter.m_nY);

	// Вычисление углов
	double dStartAngle = std::atan2(oStart.m_nY - oCenter.m_nY, oStart.m_nX - oCenter.m_nX);
	double dEndAngle   = std::atan2(oEnd.m_nY   - oCenter.m_nY, oEnd.m_nX   - oCenter.m_nX);

	if (dEndAngle < dStartAngle)
		std::swap(dStartAngle, dEndAngle);

	TPoint oControl1{static_cast<int>(oStart.m_nX + dRadiusX * std::cos(dStartAngle)), static_cast<int>(oStart.m_nY + dRadiusY * std::sin(dStartAngle))};
	TPoint oControl2{static_cast<int>(oEnd.m_nX   - dRadiusX * std::cos(dEndAngle  )), static_cast<int>(oEnd.m_nY   + dRadiusY * std::sin(dEndAngle  ))};

	return {oStart, oControl1, oControl2, oEnd};
}

void CConverter2OOXML::WriteGeometryShape(const CCtrlGeneralShape* pGeneralShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pGeneralShape)
		return;

	EShapeObjectType eShapeType = GetShapeObjectType(pGeneralShape->GetID());

	if (EShapeObjectType::Unknown == eShapeType)
		return;

	++m_ushShapeCount;

	WriteCaption((const CCtrlCommon*)pGeneralShape, oBuilder, oState);

	OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);

	const int nWidth =  Transform::HWPUINT2OOXML(pGeneralShape->GetWidth());
	const int nHeight = Transform::HWPUINT2OOXML(pGeneralShape->GetHeight());

	const std::wstring wsWidth  = std::to_wstring(nWidth);
	const std::wstring wsHeight = std::to_wstring(nHeight);

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");
	oBuilder.WriteString(L"<mc:AlternateContent><mc:Choice Requires=\"wps\">");

	OpenDrawingNode(pGeneralShape, oBuilder);

	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
	oBuilder.WriteString(L"<wps:wsp><wps:cNvSpPr/><wps:spPr><a:xfrm>");
	oBuilder.WriteString(L"<a:off x=\"0\" y=\"0\"/>");
	oBuilder.WriteString(L"<a:ext cx=\"" + wsWidth + L"\" cy=\"" + wsHeight + L"\"/>");
	oBuilder.WriteString(L"</a:xfrm>");

	switch (eShapeType)
	{
		case EShapeObjectType::Arc:
		{
			const CCtrlShapeArc *pShapeArc = (const CCtrlShapeArc*)pGeneralShape;
			VECTOR<TPoint> arBezierPoints{ArcToBezier({Transform::HWPUINT2OOXML(pShapeArc->GetFirstPoint() .m_nX), Transform::HWPUINT2OOXML(pShapeArc->GetFirstPoint() .m_nY)},
			                                          {Transform::HWPUINT2OOXML(pShapeArc->GetSecondPoint().m_nX), Transform::HWPUINT2OOXML(pShapeArc->GetSecondPoint().m_nY)},
			                                          {Transform::HWPUINT2OOXML(pShapeArc->GetCenterPoint().m_nX), Transform::HWPUINT2OOXML(pShapeArc->GetCenterPoint().m_nY)})};

			oBuilder.WriteString(L"<a:custGeom><a:pathLst><a:path>");
			oBuilder.WriteString(L"<a:moveTo><a:pt x=\"" + std::to_wstring(arBezierPoints[0].m_nX) + L"\" y=\"" + std::to_wstring(arBezierPoints[0].m_nY) + L"\"/></a:moveTo>");

			oBuilder.WriteString(L"<a:cubicBezTo>");
			oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(arBezierPoints[1].m_nX) + L"\" y=\"" + std::to_wstring(arBezierPoints[1].m_nY) + L"\"/>");
			oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(arBezierPoints[2].m_nX) + L"\" y=\"" + std::to_wstring(arBezierPoints[2].m_nY) + L"\"/>");
			oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(arBezierPoints[3].m_nX) + L"\" y=\"" + std::to_wstring(arBezierPoints[3].m_nY) + L"\"/>");
			oBuilder.WriteString(L"</a:cubicBezTo>");

			oBuilder.WriteString(L"</a:path></a:pathLst></a:custGeom>");

			break;
		}
		case EShapeObjectType::Ellipse:
		{
			oBuilder.WriteString(L"<a:prstGeom prst=\"ellipse\"><a:avLst/></a:prstGeom>");
			break;
		}
		case EShapeObjectType::Line:
		{
			oBuilder.WriteString(L"<a:prstGeom prst=\"line\"><a:avLst/></a:prstGeom>");
			break;
		}

		case EShapeObjectType::Rectangle:
		{
			oBuilder.WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			break;
		}
		case EShapeObjectType::Polygon:
		{
			VECTOR<TPoint> arPoints{((const CCtrlShapePolygon*)pGeneralShape)->GetPoints()};

			if (2 > arPoints.size())
				break;

			oBuilder.WriteString(L"<a:custGeom><a:pathLst><a:path>");
			oBuilder.WriteString(L"<a:moveTo><a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[0].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[0].m_nY)) + L"\"/></a:moveTo>");

			for (unsigned short ushIndex = 1; ushIndex < arPoints.size(); ++ushIndex)
				oBuilder.WriteString(L"<a:lnTo><a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex].m_nY)) + L"\"/></a:lnTo>");

			oBuilder.WriteString(L"</a:path></a:pathLst></a:custGeom>");
			break;
		}
		case EShapeObjectType::Curve:
		{
			VECTOR<TPoint> arPoints{((const CCtrlShapeCurve*)pGeneralShape)->GetPoints()};

			if (2 > arPoints.size())
				break;

			VECTOR<HWP_BYTE> arSegmentType{((const CCtrlShapeCurve*)pGeneralShape)->GetSegmentsType()};
			oBuilder.WriteString(L"<a:custGeom><a:pathLst><a:path>");
			oBuilder.WriteString(L"<a:moveTo><a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[0].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[0].m_nY)) + L"\"/></a:moveTo>");

			for (unsigned short ushIndex = 0; ushIndex < arSegmentType.size(); ++ushIndex)
			{
				if (0x01 == arSegmentType[ushIndex])
				{
					oBuilder.WriteString(L"<a:cubicBezTo>");
					oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 1].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 1].m_nY)) + L"\"/>");
					oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 2].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 2].m_nY)) + L"\"/>");
					oBuilder.WriteString(L"<a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 3].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 3].m_nY)) + L"\"/>");
					oBuilder.WriteString(L"</a:cubicBezTo>");
					ushIndex += 2;
				}
				else
					oBuilder.WriteString(L"<a:lnTo><a:pt x=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 1].m_nX)) + L"\" y=\"" + std::to_wstring(Transform::HWPUINT2OOXML(arPoints[ushIndex + 1].m_nY)) + L"\"/></a:lnTo>");
			}

			oBuilder.WriteString(L"</a:path></a:pathLst></a:custGeom>");
			break;
		}
		case EShapeObjectType::Ole:
		case EShapeObjectType::Picture:
		case EShapeObjectType::Unknown:
			break;
	}

	const CFill *pFill = pGeneralShape->GetFill();

	if (nullptr == pFill || pFill->NoneFill())
		oBuilder.WriteString(L"<a:noFill/>");
	else if (pFill->ColorFill())
		oBuilder.WriteString(L"<a:solidFill><a:srgbClr val=\"" + Transform::IntColorToHEX(pFill->GetFaceColor()) + L"\"/></a:solidFill>");
	else if (pFill->ImageFill())
	{
		const HWP_STRING sPictureId = SavePicture(pFill->GetBinItemID(), oState);

		if (!sPictureId.empty())
			oBuilder.WriteString(L"<a:blipFill><a:blip r:embed=\"" + sPictureId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></a:blipFill>");
		else
			oBuilder.WriteString(L"<a:noFill/>");
	}

	WriteLineSettings(pGeneralShape, oBuilder);

	oBuilder.WriteString(L"</wps:spPr>");

	unsigned int nCountParagraphs = pGeneralShape->GetCountParagraphs();

	if (0 < nCountParagraphs)
	{
		oBuilder.WriteString(L"<wps:txbx><w:txbxContent>");

		TConversionState oShapeState;
		oShapeState.m_bInTextBox = true;

		for (unsigned int unParaIndex = 0; unParaIndex < nCountParagraphs; ++unParaIndex)
			WriteParagraph(pGeneralShape->GetParagraphs(unParaIndex), oBuilder, oShapeState);

		oBuilder.WriteString(L"</w:txbxContent></wps:txbx>");
	}

	oBuilder.WriteString(L"<wps:bodyPr/>");

	oBuilder.WriteString(L"</wps:wsp></a:graphicData></a:graphic>");
	CloseDrawingNode(pGeneralShape, oBuilder);
	oBuilder.WriteString(L"</mc:Choice></mc:AlternateContent></w:r>");
}

void CConverter2OOXML::WriteEqEditShape(const CCtrlEqEdit* pEqEditShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	//TODO:: добавить конвертацию eqn формулы в ooxml
	++m_ushEquationCount;

	WriteCaption((const CCtrlCommon*)pEqEditShape, oBuilder, oState);

	OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);

	oBuilder.WriteString(L"<w:r>");

	oBuilder.WriteString(L"<w:t xml:space=\"preserve\">");
	oBuilder.WriteEncodeXmlString(pEqEditShape->GetEqn());
	oBuilder.WriteString(L"</w:t></w:r>");
}

void CConverter2OOXML::WriteOleShape(const CCtrlShapeOle* pOleShape, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	//TODO:: добавить конвертацию hwp ole -> ooxml chart
	//TODO:: необходимо добавить поддержку формата "Hwp Document File Formats - Charts" (для случаев, когда нет ooxml представления)
	// Пока можем вытащить лишь ooxml представление данных
	// Реализовать с использованием pole.h?

	if (nullptr == m_pContext)
		return;

	CHWPStream oBuffer;
	HWP_STRING sFileName;

	if (!m_pContext->GetBinBytes(pOleShape->GetBinDataID(), oBuffer, sFileName))
		return;

	m_oOleConverter.CreateChart(oBuffer);

	const unsigned int unChartIndex = m_oOleConverter.GetChartsCount();

	if (0 == unChartIndex)
		return;

	++m_ushShapeCount;

	WriteCaption((const CCtrlCommon*)pOleShape, oBuilder, oState);

	const std::wstring wsWidth  = std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetWidth()));
	const std::wstring wsHeight = std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetHeight()));
	const std::wstring wsRelID  = AddRelationship(L"chart", L"charts/chart" + std::to_wstring(unChartIndex) + L".xml", &oState);

	OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");

	OpenDrawingNode(pOleShape, oBuilder);

	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\">");
	oBuilder.WriteString(L"<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"" + wsRelID + L"\"/>");
	oBuilder.WriteString(L"</a:graphicData></a:graphic>");
	CloseDrawingNode(pOleShape, oBuilder);
	oBuilder.WriteString(L"</w:r>");

	AddContentType(L"charts/chart" + std::to_wstring(unChartIndex) + L".xml", L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml");
	AddContentType(L"charts/style" + std::to_wstring(unChartIndex) + L".xml", L"application/vnd.ms-office.chartstyle+xml");
	AddContentType(L"charts/colors" + std::to_wstring(unChartIndex) + L".xml", L"application/vnd.ms-office.chartcolorstyle+xml");
}

void CConverter2OOXML::WriteContainer(const CCtrlContainer* pContainer, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	for (const CCtrlGeneralShape* pGeneralShape : pContainer->GetShapes())
		WriteShape(pGeneralShape, shParaShapeID, shParaStyleID, oBuilder, oState);
}

void CConverter2OOXML::WriteSectionSettings(TConversionState& oState)
{
	m_oDocXml.WriteString(L"<w:sectPr>");

	if (nullptr != oState.m_pSectionDef)
	{
		std::vector<const CCtrlHeadFoot*> arCtrlsHeadFoot;

		if (EHanType::HWP == m_pContext->GetType())
			arCtrlsHeadFoot = oState.m_pSectionDef->GetHeaderFooters();
		else if (EHanType::HWPX == m_pContext->GetType())
		{
			arCtrlsHeadFoot = oState.m_arCtrlsHeadFoot;
			oState.m_arCtrlsHeadFoot.clear();
		}

		#define WRITE_ID(id)\
		if (!id.empty() && id.length() > 6)\
		{\
			const std::wstring wsType = id.substr(0, 6);\
			AddContentType(id, L"application/vnd.openxmlformats-officedocument.wordprocessingml." + wsType + L"+xml");\
			m_oDocXml.WriteString(L"<w:" + wsType + L"Reference w:type=\"default\" r:id=\"" + AddRelationship(wsType, id, &oState) + L"\"/>");\
		}

		if (nullptr != oState.m_pPageNum)
		{
			const std::wstring wsID = m_oFootnoteConverter.CreatePageNum(oState.m_pPageNum, *this);

			WRITE_ID(wsID);

			oState.m_pPageNum = nullptr;
		}

		for (const CCtrlHeadFoot* pCtrlHeadFoot : arCtrlsHeadFoot)
		{
			const std::wstring wsID = m_oFootnoteConverter.CreateHeadOrFoot((const CCtrlHeadFoot*)pCtrlHeadFoot, *this);

			WRITE_ID(wsID);
		}
	}

	if (nullptr != oState.m_pNewNumber && ENumType::PAGE == oState.m_pNewNumber->GetNumType())
		m_oDocXml.WriteString(L"<w:pgNumType w:start=\"" + std::to_wstring(oState.m_pNewNumber->GetNum()) + L"\"/>");

	const CPage *pPage = (nullptr != oState.m_pSectionDef) ? oState.m_pSectionDef->GetPage() : nullptr;

	if (nullptr == pPage)
	{
		//DEFAULT_VALUE
		m_oDocXml.WriteString(L"<w:pgSz w:w=\"11906\" w:h=\"16838\"/>");
		m_oDocXml.WriteString(L"<w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"708\" w:footer=\"708\" w:gutter=\"0\"/>");
	}
	else
	{
		m_oDocXml.WriteString(L"<w:pgSz w:w=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetWidth())) + L"\" w:h=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetHeight())) + L"\"/>");
		m_oDocXml.WriteString(L"<w:pgMar w:top=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginTop())) + L"\" w:right=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginRight())) + L"\" w:bottom=\"" +
		                        std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginBottom())) + L"\"  w:left=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginRight())) + L"\" w:header=\"" +
		                        std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginHeader())) + L"\"  w:footer=\"" + std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginFooter())) + L"\"  w:gutter=\"" +
		                        std::to_wstring(Transform::HWPUINT2Twips(pPage->GetMarginGutter())) + L"\"/>");
	}

	if (nullptr != oState.m_pColumnDef && 1 < oState.m_pColumnDef->GetColCount())
	{
		//TODO:: Добавить поддержку остальный свойств
		m_oDocXml.WriteString(L"<w:cols w:num=\"" + std::to_wstring(oState.m_pColumnDef->GetColCount()) + L"\"  w:space=\"454\"");

		if (ELineStyle2::NONE != oState.m_pColumnDef->GetColLineStyle())
			m_oDocXml.WriteString(L" w:sep=\"true\"");

		m_oDocXml.WriteString(L"/>");
	}
	else
		m_oDocXml.WriteString(L"<w:cols w:space=\"708\"/>");

	m_oDocXml.WriteString(L"</w:sectPr>");
}

void CConverter2OOXML::WritePicture(const CCtrlShapePic* pCtrlPic, short shParaShapeId, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pCtrlPic)
		return;

	HWP_STRING sPictureID = SavePicture(pCtrlPic->GetBinDataID(), oState);

	if (sPictureID.empty())
		return;

	++m_ushShapeCount;

	WriteCaption((const CCtrlCommon*)pCtrlPic, oBuilder, oState);

	OpenParagraph(shParaShapeId, shParaStyleID, oBuilder, oState);

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");

	int nWidth{pCtrlPic->GetImageRectWidth()}, nHeight{pCtrlPic->GetIMageRectHeight()};

	OpenDrawingNode(pCtrlPic, oBuilder, &nWidth, &nHeight);

	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + std::to_wstring(m_ushShapeCount) + L"\" name=\"Shape " + std::to_wstring(m_ushShapeCount) + L"\"/>");
	oBuilder.WriteString(L"<pic:cNvPicPr/></pic:nvPicPr>");
	oBuilder.WriteString(L"<pic:blipFill><a:blip r:embed=\"" + sPictureID + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
	oBuilder.WriteString(L"<pic:spPr bwMode=\"auto\">");

	oBuilder.WriteString(L"<a:xfrm");
	if (pCtrlPic->HorzFlip())
		oBuilder.WriteString(L" flipH=\"1\"");
	if (pCtrlPic->VertFlip())
		oBuilder.WriteString(L" flipV=\"1\"");

	oBuilder.WriteString(L"><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm>");
	oBuilder.WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/>");
	WriteBorderSettings(pCtrlPic, oBuilder);
	oBuilder.WriteString(L"</pic:spPr></pic:pic></a:graphicData></a:graphic>");
	CloseDrawingNode(pCtrlPic, oBuilder);
	oBuilder.WriteString(L"</w:r>");
}

void CConverter2OOXML::WriteVideo(const CCtrlShapeVideo* pCtrlVideo, short shParaShapeId, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pCtrlVideo || 1 != pCtrlVideo->GetVideoType())
		return;

	HWP_STRING sPictureID = SavePicture(pCtrlVideo->GetThumnailBinID(), oState);

	if (sPictureID.empty())
		return;

	++m_ushShapeCount;

	WriteCaption((const CCtrlCommon*)pCtrlVideo, oBuilder, oState);

	OpenParagraph(shParaShapeId, shParaStyleID, oBuilder, oState);

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");

	OpenDrawingNode(pCtrlVideo, oBuilder);

	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + std::to_wstring(m_ushShapeCount) + L"\" name=\"Video " + std::to_wstring(m_ushShapeCount) + L"\" descr=\"" + pCtrlVideo->GetDesc() + L"\">");
	oBuilder.WriteString(L"<a:hlinkClick r:id=\"" + AddRelationship(L"hyperlink", pCtrlVideo->GetWebUrl(), &oState) + L"\"/></pic:cNvPr>");
	oBuilder.WriteString(L"<pic:cNvPicPr/></pic:nvPicPr>");
	oBuilder.WriteString(L"<pic:blipFill><a:blip r:embed=\"" + sPictureID + L"\"><a:extLst>");
	oBuilder.WriteString(L"<a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext>");

	const int nIFrameWidth  = (int)((double)Transform::HWPUINT2OOXML(pCtrlVideo->GetWidth())  / 22860.);
	const int nIFrameHeight = (int)((double)Transform::HWPUINT2OOXML(pCtrlVideo->GetHeight()) / 22860.);

	oBuilder.WriteString(L"<a:ext uri=\"{C809E66F-F1BF-436E-b5F7-EEA9579F0CBA}\">");
	oBuilder.WriteString(L"<wp15:webVideoPr xmlns:wp15=\"http://schemas.microsoft.com/office/word/2012/wordprocessingDrawing\" embeddedHtml=\"");
	oBuilder.WriteEncodeXmlString(L"<iframe width=\"" + std::to_wstring(nIFrameWidth) + L"\" height=\"" + std::to_wstring(nIFrameHeight) + L"\"");
	oBuilder.WriteEncodeXmlString(L"src=\"" + pCtrlVideo->GetWebUrl() + L"\" frameborder=\"0\" allow=\"accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share\" referrerpolicy=\"strict-origin-when-cross-origin\" allowfullscreen=\"\" title=\"\" sandbox=\"allow-scripts allow-same-origin allow-popups\"></iframe>");
	oBuilder.WriteString(L"\" h=\"" + std::to_wstring(nIFrameWidth) + L"\" w=\"" + std::to_wstring(nIFrameHeight) + L"\"/></a:ext>");

	oBuilder.WriteString(L"</a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
	oBuilder.WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlVideo->GetFinalWidth())) + L"\" cy=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlVideo->GetFinalHeight())) + L"\"/></a:xfrm>");
	oBuilder.WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr></pic:pic></a:graphicData></a:graphic>");

	CloseDrawingNode(pCtrlVideo, oBuilder);
	oBuilder.WriteString(L"</w:r>");
}

bool CConverter2OOXML::SaveSVGFile(const HWP_STRING& sSVG, HWP_STRING& sFileName)
{
	if (sSVG.empty())
		return false;

	NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
	pFonts->Initialize();

	MetaFile::IMetaFile* pSvgReader = MetaFile::Create(pFonts);

	if (!pSvgReader->LoadFromString(sSVG))
	{
		RELEASEINTERFACE(pSvgReader);
		RELEASEINTERFACE(pFonts);
		return false;
	}

	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pSvgReader->get_FontManager());

	double dX, dY, dW, dH;
	pSvgReader->GetBounds(&dX, &dY, &dW, &dH);

	if (dW < 0) dW = -dW;
	if (dH < 0) dH = -dH;

	double dOneMaxSize = (double)1000.;

	if (dW > dH && dW > dOneMaxSize)
	{
		dH *= (dOneMaxSize / dW);
		dW = dOneMaxSize;
	}
	else if (dH > dW && dH > dOneMaxSize)
	{
		dW *= (dOneMaxSize / dH);
		dH = dOneMaxSize;
	}

	int nWidth  = static_cast<int>(dW + 0.5);
	int nHeight = static_cast<int>(dH + 0.5);

	double dWidth  = 25.4 * nWidth / 96;
	double dHeight = 25.4 * nHeight / 96;

	unsigned char* pBgraData = (unsigned char*)malloc(nWidth * nHeight * 4);
	if (!pBgraData)
	{
		double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

		nWidth = (int)(dKoef * nWidth);
		nHeight = (int)(dKoef * nHeight);

		dWidth  = 25.4 * nWidth / 96;
		dHeight = 25.4 * nHeight / 96;

		pBgraData = (unsigned char*)malloc(nWidth * nHeight * 4);
	}

	if (!pBgraData)
		return false;

	unsigned int alfa = 0xffffff;
	//дефолтный тон должен быть прозрачным, а не белым
	//memset(pBgraData, 0xff, nWidth * nHeight * 4);
	for (int i = 0; i < nWidth * nHeight; i++)
	{
		((unsigned int*)pBgraData)[i] = alfa;
	}

	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

	pGrRenderer->CreateFromBgraFrame(&oFrame);
	pGrRenderer->SetSwapRGB(false);
	pGrRenderer->put_Width(dWidth);
	pGrRenderer->put_Height(dHeight);

	pSvgReader->SetTempDirectory(m_sTempDirectory);
	pSvgReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

	sFileName = sFileName.substr(0, sFileName.find(L'.'));
	sFileName += L".png";

	oFrame.SaveFile(m_sTempDirectory + L"/word/media/" + sFileName, 4);
	oFrame.put_Data(NULL);

	RELEASEINTERFACE(pGrRenderer);

	if (pBgraData)
		free(pBgraData);

	RELEASEINTERFACE(pSvgReader);
	RELEASEINTERFACE(pFonts);

	return true;
}

HWP_STRING CConverter2OOXML::SavePicture(const HWP_STRING& sBinItemId, TConversionState& oState)
{
	if (nullptr == m_pContext)
		return HWP_STRING();

	//TODO:: добавить поддержку устновки размеров изображения из свойств шейпа
	CHWPStream oBuffer;
	HWP_STRING sFileName;

	if (!m_pContext->GetBinBytes(sBinItemId, oBuffer, sFileName))
		return HWP_STRING();

	oBuffer.MoveToStart();

	if (IsRasterFormat(NSFile::GetFileExtention(sFileName)))
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(m_sTempDirectory + L"/word/media/" + sFileName);
		if (!oFile.WriteFile((unsigned char*)oBuffer.GetCurPtr(), oBuffer.GetSize()))
		{
			oFile.CloseFile();
			return HWP_STRING();
		}
		oFile.CloseFile();
	}
	else if (L"svg" == NSFile::GetFileExtention(sFileName))
	{
		std::string sSVG(oBuffer.GetCurPtr(), oBuffer.GetSize());
		if (!SaveSVGFile(UTF8_TO_U(sSVG), sFileName))
			return HWP_STRING();
	}

	AddContentType(L"media/" + sFileName, L"image/" + NSFile::GetFileExtention(sFileName));
	return AddRelationship(L"image", L"media/" + sFileName, &oState);
}

void CConverter2OOXML::WriteRunnerStyle(short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CRunnerStyle& sExternStyles)
{
	if (nullptr == m_pContext)
		return;

	oBuilder.WriteString(L"<w:rPr>");

	if (m_oStyleConverter.GetLastCharShapeId() != shCharShapeID)
		m_oStyleConverter.WriteDifferenceRunnerStyles(m_oStyleConverter.GetLastCharShapeId(), shCharShapeID, *m_pContext, oBuilder);

	const CHWPRecordCharShape* pCharShape = dynamic_cast<const CHWPRecordCharShape*>(m_pContext->GetCharShape(shCharShapeID));

	if (nullptr != pCharShape)
	{
		WriteTextBorderStyle(pCharShape->GetBorderFillID(), oBuilder, oState);
		oState.m_ushLastCharShapeId = shCharShapeID;
	}

	m_oStyleConverter.WriteRunnerProperties(sExternStyles, oBuilder);

	oBuilder.WriteString(L"</w:rPr>");

	switch(oState.m_eBreakType)
	{
		case TConversionState::EBreakType::Page:
			oBuilder.WriteString(L"<w:br w:type=\"page\"/>"); break;
		case TConversionState::EBreakType::Column:
			oBuilder.WriteString(L"<w:br w:type=\"column\"/>"); break;
		case TConversionState::EBreakType::TextWrapping:
			oBuilder.WriteString(L"<w:br/>"); break;
		case TConversionState::EBreakType::None:
			break;
	}

	oState.m_eBreakType = TConversionState::EBreakType::None;
}

void CConverter2OOXML::WriteTextBorderStyle(short shBorderFillId, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	const CHWPRecordBorderFill* pBorderFill = dynamic_cast<const CHWPRecordBorderFill*>(m_pContext->GetBorderFill(shBorderFillId));

	if (nullptr == pBorderFill)
		return;

	TBorder oBorder{pBorderFill->GetLeftBorder()};

	if (ELineStyle2::NONE == oBorder.m_eStyle)
		return;

	WriteBorder(pBorderFill->GetLeftBorder(), L"bdr", oBuilder);
}

void CConverter2OOXML::OpenDrawingNode(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder, int* pWidth, int* pHeight)
{
	if (nullptr == pCtrlShape)
		return;

	oBuilder.WriteString(L"<w:drawing>");

	if (pCtrlShape->GetTreatAsChar())
	{
		oBuilder.WriteString(L"<wp:inline distT=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetTopOutMargin() / 10)) +
		                             L"\" distB=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetBottomOutMargin() / 10)) +
		                             L"\" distL=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetLeftOutMargin() / 10)) +
		                             L"\" distR=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetRightOutMargin() / 10)) +
		                     L"\">");

		WriteShapeExtent(pCtrlShape, oBuilder, pWidth, pHeight);
	}
	else
	{
		oBuilder.WriteString(L"<wp:anchor behindDoc=\"" + std::wstring((ETextWrap::BEHIND_TEXT == pCtrlShape->GetTextWrap() ? L"1" : L"0")) +
		                     L"\" relativeHeight=\"" + std::to_wstring(pCtrlShape->GetZOrder()) +
		                     L"\" distT=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetTopOutMargin() / 10)) +
		                     L"\" distB=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetBottomOutMargin() / 10)) +
		                     L"\" distL=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetLeftOutMargin() / 10)) +
		                     L"\" distR=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetRightOutMargin() / 10)) +
		                     L"\" simplePos=\"0\" locked=\"0\" layoutInCell=\"1\" allowOverlap=\"1\">");

		WriteShapePosition(pCtrlShape, oBuilder);
		WriteShapeExtent(pCtrlShape, oBuilder, pWidth, pHeight);
		WriteShapeWrapMode(pCtrlShape, oBuilder);
	}

	WriteShapeProperty(pCtrlShape, oBuilder);
}

void CConverter2OOXML::CloseDrawingNode(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlShape)
		return;

	if (pCtrlShape->GetTreatAsChar())
		oBuilder.WriteString(L"</wp:inline>");
	else
	{
		oBuilder.WriteString(L"<wp14:sizeRelH relativeFrom=\"page\"><wp14:pctWidth>0</wp14:pctWidth></wp14:sizeRelH><wp14:sizeRelV relativeFrom=\"page\"><wp14:pctHeight>0</wp14:pctHeight></wp14:sizeRelV>");
		oBuilder.WriteString(L"</wp:anchor>");
	}

	oBuilder.WriteString(L"</w:drawing>");
}

HWP_STRING GetVRelativeFrom(EVRelTo eRelTo)
{
	switch (eRelTo)
	{
		case EVRelTo::PARA:
			return L"paragraph";
		case EVRelTo::PAPER:
		case EVRelTo::PAGE:
			return L"page";
	}
}

HWP_STRING GetHRelativeFrom(EHRelTo eRelTo)
{
	switch (eRelTo)
	{
		case EHRelTo::PAPER:
		case EHRelTo::PAGE:
			return L"page";
		case EHRelTo::COLUMN:
			return L"column";
		case EHRelTo::PARA:
			return L"character";
	}
}

void CConverter2OOXML::WriteShapeProperty(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlShape)
		return;

	oBuilder.WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_ushShapeCount) + L"\" name=\"Shape " + std::to_wstring(m_ushShapeCount) + L"\" descr=\"");
	oBuilder.WriteEncodeXmlString(pCtrlShape->GetDesc());
	oBuilder.WriteString(L"\"/>");
	oBuilder.WriteString(L"<wp:cNvGraphicFramePr/>");
}

void CConverter2OOXML::WriteShapePosition(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlShape)
		return;

	oBuilder.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");

	oBuilder.WriteString(L"<wp:positionH relativeFrom=\"");

	if (0 == pCtrlShape->GetHorzOffset())
	{
		oBuilder.WriteString(L"margin\">");
		oBuilder.WriteString(L"<wp:align>left</wp:align>");
	}
	else
	{
		oBuilder.WriteString(GetHRelativeFrom(pCtrlShape->GetHorzRelTo()) + L"\">");
		oBuilder.WriteString(L"<wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetHorzOffset())) + L"</wp:posOffset>");
	}

	oBuilder.WriteString(L"</wp:positionH>");

	oBuilder.WriteString(L"<wp:positionV relativeFrom=\"" + GetVRelativeFrom(pCtrlShape->GetVertRelTo()) + L"\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlShape->GetVertOffset())) + L"</wp:posOffset></wp:positionV>");
}

void CConverter2OOXML::WriteShapeExtent(const CCtrlObjElement* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder, int* pWidth, int* pHeight)
{
	if (nullptr == pCtrlShape)
		return;

	double dScaleX = 1., dScaleY = 1.;

	const int nOrgWidth{pCtrlShape->GetOrgWidth()}, nOrgHeight{pCtrlShape->GetOrgHeight()};
	const int nCurWidth{pCtrlShape->GetCurWidth()}, nCurHeight{pCtrlShape->GetCurHeight()};

	if (0 != nCurWidth && 0 != nOrgWidth)
		dScaleX = (double)nCurWidth / (double)nOrgWidth;

	if (0 != nCurHeight && 0 != nOrgHeight)
		dScaleY = (double)nCurHeight / (double)nOrgHeight;

	if (0 != pCtrlShape->GetWidth() && 0 != nCurWidth)
		dScaleX *= (double)pCtrlShape->GetWidth() / (double)nCurWidth;

	if (0 != pCtrlShape->GetHeight() && 0 != nCurHeight)
		dScaleY *= (double)pCtrlShape->GetHeight() / (double)nCurHeight;

	int nFinalWidth {pCtrlShape->GetWidth() };
	int nFinalHeight{pCtrlShape->GetHeight()};

	if (0 == nFinalWidth)
	{
		if (nullptr != pWidth && 0 != *pWidth)
			nFinalWidth = *pWidth;
		else
			nFinalWidth = nCurWidth;
	}

	if (nullptr != pWidth && 0 != *pWidth)
		dScaleX *= (double)*pWidth / (double)nFinalWidth;

	if (0 == nFinalHeight)
	{
		if (nullptr != pHeight && 0 != *pHeight)
			nFinalHeight = *pHeight;
		else
			nFinalHeight = nCurHeight;
	}

	if (nullptr != pHeight && 0 != *pHeight)
		dScaleY *= (double)*pHeight / (double)nFinalHeight;

	TMatrix oFinalMatrix{pCtrlShape->GetFinalMatrix()};
	oFinalMatrix.ApplyToSize(dScaleX, dScaleY);

	nFinalWidth = ceil((double)nFinalWidth * dScaleX);
	nFinalHeight = ceil((double)nFinalHeight * dScaleY);

	nFinalWidth  -= (pCtrlShape->GetLeftInMargin() + pCtrlShape->GetRightInMargin());
	nFinalHeight -= (pCtrlShape->GetTopInMargin() + pCtrlShape->GetBottomInMargin());

	if (nullptr != pWidth)
		*pWidth = Transform::HWPUINT2OOXML(nFinalWidth);

	if (nullptr != pHeight)
		*pHeight = Transform::HWPUINT2OOXML(nFinalHeight);

	oBuilder.WriteString(L"<wp:extent cx=\"" + std::to_wstring(Transform::HWPUINT2OOXML(nFinalWidth)) + L"\" cy=\"" + std::to_wstring(Transform::HWPUINT2OOXML(nFinalHeight)) + L"\"/>");
	oBuilder.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
}

void CConverter2OOXML::WriteShapeWrapMode(const CCtrlCommon* pCtrlShape, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlShape)
		return;

	switch (pCtrlShape->GetTextWrap())
	{
		case ETextWrap::SQUARE:
		{
			oBuilder.WriteString(L"<wp:wrapSquare wrapText=\"");
			switch (pCtrlShape->GetTextFlow())
			{
				case 0x0: default: oBuilder.WriteString(L"bothSides"); break;
				case 0x1: oBuilder.WriteString(L"left"); break;
				case 0x2: oBuilder.WriteString(L"right"); break;
				case 0x3: oBuilder.WriteString(L"largest"); break;
			}
			oBuilder.WriteString(L"\"/>");
			break;
		}
		case ETextWrap::TOP_AND_BOTTOM:
		{
			oBuilder.WriteString(L"<wp:wrapTopAndBottom/>");
			break;
		}
		case ETextWrap::BEHIND_TEXT:
		case ETextWrap::IN_FRONT_OF_TEXT:
		{
			oBuilder.WriteString(L"<wp:wrapNone/>");
			break;
		}
	}
}

void CConverter2OOXML::OpenParagraph(short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (oState.m_bOpenedP)
		return;

	oBuilder.WriteString(L"<w:p>");
	oState.m_bOpenedP = true;
	oState.m_bLastEmptyNode = false;
	WriteParagraphProperties(shParaShapeID, shParaStyleID, oBuilder, oState);
}

void CConverter2OOXML::CloseParagraph(NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (!oState.m_bOpenedP)
		return;

	oBuilder.WriteString(L"</w:p>");
	oState.m_bOpenedP = false;
}

void CConverter2OOXML::WriteText(const CParaText* pParaText, const std::vector<TRangeTag>& arRangeTags, short shParaShapeID, short shParaStyleID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pParaText)
		return;

	if (arRangeTags.empty())
	{
		WriteText(pParaText->GetText(), shParaShapeID, shParaStyleID, pParaText->GetCharShapeID(), oBuilder, oState);
		return;
	}

	HWP_STRING wsText = pParaText->GetText();
	int nParaTextPosition = pParaText->GetStartIDx();
	int unStartText = 0;

	TColor *pHighlightColor = nullptr;

	for (size_t unTextPosition = 0; unTextPosition < wsText.length(); ++unTextPosition)
	{
		for (const TRangeTag& oRangeTag : arRangeTags)
		{
			if (unTextPosition + nParaTextPosition == oRangeTag.m_nStartPos)
			{
				WriteText(wsText.substr(unStartText, unTextPosition - unStartText), shParaShapeID, shParaStyleID, pParaText->GetCharShapeID(), oBuilder, oState);
				unStartText = unTextPosition;

				switch (oRangeTag.m_chType)
				{
					case 0x02: //highlight
					{
						pHighlightColor = new TColor{(unsigned char)oRangeTag.m_arData[0], (unsigned char)oRangeTag.m_arData[1], (unsigned char)oRangeTag.m_arData[2]};
						break;
					}
					default:
						break;
				}
			}
			else if (unTextPosition + nParaTextPosition == oRangeTag.m_nEndPos)
			{
				if (nullptr == pHighlightColor)
				{
					WriteText(wsText.substr(unStartText, unTextPosition - unStartText), shParaShapeID, shParaStyleID, pParaText->GetCharShapeID(), oBuilder, oState);
					unStartText = unTextPosition;
					continue;
				}

				CRunnerStyle oRunnerStyle;
				oRunnerStyle.SetHighlight(NormalizeHighlightColor(*pHighlightColor));

				WriteText(wsText.substr(unStartText, unTextPosition - unStartText), shParaShapeID, shParaStyleID, pParaText->GetCharShapeID(), oBuilder, oState, oRunnerStyle);
				unStartText = unTextPosition;

				switch (oRangeTag.m_chType)
				{
					case 0x02: //highlight
					{
						if (nullptr != pHighlightColor)
						{
							delete pHighlightColor;
							pHighlightColor = nullptr;
						}
						break;
					}
					default:
						break;
				}
			}
		}
	}

	if (unStartText < wsText.length())
		WriteText(wsText.substr(unStartText), shParaShapeID, shParaStyleID, pParaText->GetCharShapeID(), oBuilder, oState);
}

std::vector<std::wstring> SplitText(const std::wstring& wsText)
{
	if (wsText.empty())
		return std::vector<std::wstring>{L""};

	std::vector<std::wstring> arTexts;

	std::wstring wsCurrentText;
	bool bWasLetter = iswalpha(wsText[0]);
	wsCurrentText.push_back(wsText[0]);

	for (unsigned int unIndex = 1; unIndex < wsText.length(); ++unIndex)
	{
		bool bCurrentIsLetter = iswalnum(wsText[unIndex]);

		if (bCurrentIsLetter != bWasLetter)
		{
			arTexts.push_back(wsCurrentText);
			wsCurrentText.clear();
			bWasLetter = bCurrentIsLetter;
		}

		wsCurrentText.push_back(wsText[unIndex]);
	}

	if (!wsCurrentText.empty())
		arTexts.push_back(wsCurrentText);

	return arTexts;
}

void CConverter2OOXML::WriteText(const HWP_STRING& wsText, short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState, const CRunnerStyle& oExternalStyle)
{
	if (!oState.m_bOpenedP && wsText.empty())
	{
		WriteEmptyParagraph(shParaShapeID, shParaStyleID, shCharShapeID, oBuilder, oState);
		return;
	}

	OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);

	if (wsText.empty())
	{
		oBuilder.WriteString(L"<w:r>");
		WriteRunnerStyle(shCharShapeID, oBuilder, oState, oExternalStyle);
		oBuilder.WriteString(L"</w:r>");
		return;
	}

	oBuilder.WriteString(L"<w:r>");

	WriteRunnerStyle(shCharShapeID, oBuilder, oState, oExternalStyle);

	oBuilder.WriteString(L"<w:t");

	bool bNeedPreserve = (wsText.cend() != std::find_if(wsText.cbegin(), wsText.cend(), [](wchar_t wChar){ return iswspace(wChar); }));
	bool bNeedAddSpace = false;

	if (oState.m_bIsNote && !iswspace(wsText[0]))
		bNeedAddSpace = true;

	if (bNeedPreserve || bNeedAddSpace)
		oBuilder.WriteString(L" xml:space=\"preserve\">");
	else
		oBuilder.WriteString(L">");

	if (bNeedAddSpace)
		oBuilder.WriteString(L" ");

	oBuilder.WriteEncodeXmlString(wsText);
	oBuilder.WriteString(L"</w:t></w:r>");

	return;

	for (const std::wstring& wsTextElement : SplitText(wsText))
	{
		oBuilder.WriteString(L"<w:r>");

		WriteRunnerStyle(shCharShapeID, oBuilder, oState, oExternalStyle);

		if (!wsTextElement.empty())
		{
			oBuilder.WriteString(L"<w:t");

			if (!iswalnum(wsTextElement.front()))
				oBuilder.WriteString(L" xml:space=\"preserve\"");

			oBuilder.WriteString(L">");

			oBuilder.WriteEncodeXmlString(wsTextElement);
			oBuilder.WriteString(L"</w:t>");
		}

		oBuilder.WriteString(L"</w:r>");
	}
}

void CConverter2OOXML::WriteLineSettings(const CCtrlGeneralShape* pCtrlGeneralShape, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlGeneralShape)
		return;

	WriteLineSettings(pCtrlGeneralShape->GetLineStyle(), pCtrlGeneralShape->GetLineColor(), pCtrlGeneralShape->GetLineThick(), 1, oBuilder);
}

void CConverter2OOXML::WriteLineSettings(ELineStyle2 eLineStyle, int nColor, int nThick, HWP_BYTE nCompoundLineType, NSStringUtils::CStringBuilder& oBuilder)
{
	if (ELineStyle2::NONE == eLineStyle)
	{
		oBuilder.WriteString(L"<a:ln><a:noFill/></a:ln>");
		return;
	}

	if (0 == nThick)
		nThick = 100;

	oBuilder.WriteString(L"<a:ln");

	oBuilder.WriteString(L" w=\"" + std::to_wstring(Transform::HWPUINT2OOXML(nThick)) + L"\" cap=\"sq\"");

	switch (eLineStyle)
	{
		case ELineStyle2::DOUBLE_SLIM:
		{
			oBuilder.WriteString(L" cmpd=\"dbl\"");
			break;
		}
		case ELineStyle2::SLIM_THICK:
		{
			oBuilder.WriteString(L" cmpd=\"thickThin\"");
			break;
		}
		case ELineStyle2::THICK_SLIM:
		{
			oBuilder.WriteString(L" cmpd=\"thinThick\"");
			break;
		}
		case ELineStyle2::SLIM_THICK_SLIM:
		{
			oBuilder.WriteString(L" cmpd=\"tri\"");
			break;
		}
		default:
			break;
	}

	oBuilder.WriteString(L">");

	oBuilder.WriteString(L"<a:solidFill><a:srgbClr val=\"" + Transform::IntColorToHEX(nColor) + L"\"/></a:solidFill>");

	switch (eLineStyle)
	{
		case ELineStyle2::DASH:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"dash\"/>");
			break;
		}
		case ELineStyle2::DOT:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"dot\"/>");
			break;
		}
		case ELineStyle2::DASH_DOT:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"dashDot\"/>");
			break;
		}
		case ELineStyle2::DASH_DOT_DOT:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"sysDashDot\"/>");
			break;
		}
		case ELineStyle2::LONG_DASH:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"lgDash\"/>");
			break;
		}
		case ELineStyle2::CIRCLE:
		{
			oBuilder.WriteString(L"<a:prstDash val=\"dot\"/>");
			break;
		}
		default:
			break;
	}

	switch (nCompoundLineType)
	{
		case 0x00:
		{
			oBuilder.WriteString(L"<a:round/>");
			break;
		}
		case 0x01:
		{
			oBuilder.WriteString(L"<a:miter lim=\"800000\"/>");
			break;
		}
	}

	oBuilder.WriteString(L"</a:ln>");
}

void CConverter2OOXML::WriteBorderSettings(const CCtrlShapePic* pCtrlPic, NSStringUtils::CStringBuilder& oBuilder)
{
	if (nullptr == pCtrlPic || nullptr == m_pContext)
		return;

	if (EHanType::HWP == m_pContext->GetType())
		WriteLineSettings(pCtrlPic->GetBorderLineStyle(), pCtrlPic->GetBorderColor(), pCtrlPic->GetBorderThick(), pCtrlPic->GetBorderCompoundLineType(), oBuilder);
	else if (EHanType::HWPX == m_pContext->GetType())
		WriteLineSettings(pCtrlPic->GetLineStyle(), pCtrlPic->GetLineColor(), pCtrlPic->GetLineThick(), 1, oBuilder);
}

void CConverter2OOXML::WriteAutoNumber(const CCtrlAutoNumber* pAutoNumber, short shParaShapeID, short shParaStyleID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pAutoNumber)
		return;

	unsigned short ushValue = 0;
	HWP_STRING wsType;

	//TODO:: лучше перейти не на ручной подсчет, а на автоматический в word (но там есть свои проблемы)
	switch (pAutoNumber->GetNumType())
	{
		case ENumType::PAGE:
		{
			OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);
			oBuilder.WriteString(L"<w:fldSimple w:instr=\"PAGE \\* ARABIC\"><w:r><w:t>1</w:t></w:r></w:fldSimple>");
			return;
		}
		case ENumType::TOTAL_PAGE:
			ushValue = m_ushPageCount; break;
		case ENumType::FOOTNOTE:
		{
			OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);
			oBuilder.WriteString(L"<w:r>");

			CRunnerStyle oRunnerStyle;
			oRunnerStyle.SetVerticalAlign(EVerticalAlignRun::Superscript);
			WriteRunnerStyle(shCharShapeID, oBuilder, oState, oRunnerStyle);

			oBuilder.WriteString(L"<w:footnoteRef/></w:r>");
			return;
		}
		case ENumType::ENDNOTE:
		{
			OpenParagraph(shParaShapeID, shParaStyleID, oBuilder, oState);
			oBuilder.WriteString(L"<w:r>");

			CRunnerStyle oRunnerStyle;
			oRunnerStyle.SetVerticalAlign(EVerticalAlignRun::Superscript);
			WriteRunnerStyle(shCharShapeID, oBuilder, oState, oRunnerStyle);

			oBuilder.WriteString(L"<w:endnoteRef/></w:r>");
			return;
		}
		case ENumType::FIGURE:
		{
			wsType = L"Figure";
			ushValue = m_ushShapeCount; break;
		}
		case ENumType::TABLE:
		{
			wsType = L"Table";
			ushValue = m_ushTableCount; break;
		}
		case ENumType::EQUATION:
		{
			wsType = L"Equation";
			ushValue = m_ushEquationCount; break;
		}
	}

	if (0 == ushValue)
		return;

	if (!wsType.empty())
		oBuilder.WriteString(L"<w:fldSimple w:instr=\" SEQ " + wsType + L" \\* ARABIC \">");

	WriteText(std::to_wstring(ushValue), shParaShapeID, shParaStyleID, shCharShapeID, oBuilder, oState);

	if (!wsType.empty())
		oBuilder.WriteString(L"</w:fldSimple>");
}

HWP_STRING CConverter2OOXML::AddRelationship(const HWP_STRING& wsType, const HWP_STRING& wsTarget, TConversionState* pState)
{
	if (wsType.empty() || wsTarget.empty())
		return HWP_STRING();

	VECTOR<TRelationship> *pRelationships = nullptr;

	if (nullptr != pState && nullptr != pState->m_pRelationships)
		pRelationships = pState->m_pRelationships;
	else
		pRelationships = &m_arRelationships;

	VECTOR<TRelationship>::const_iterator itFound = std::find_if(pRelationships->cbegin(), pRelationships->cend(), [wsTarget](const TRelationship& oRelationship){ return wsTarget == oRelationship.m_wsTarget; });

	if (pRelationships->cend() != itFound)
		return itFound->m_wsID;

	if (L"hyperlink" == wsType)
	{
		NSStringUtils::CStringBuilder oBuilder;
		oBuilder.WriteEncodeXmlString(wsTarget);

		pRelationships->push_back({L"rId" + std::to_wstring(pRelationships->size() + 1), wsType, oBuilder.GetData()});
	}
	else
		pRelationships->push_back({L"rId" + std::to_wstring(pRelationships->size() + 1), wsType, wsTarget});

	return pRelationships->back().m_wsID;
}

void CConverter2OOXML::AddContentType(const HWP_STRING& wsName, const HWP_STRING& wsType)
{
	if (wsName.empty() || wsType.empty())
		return;

	VECTOR<TContentType>::const_iterator itFound = std::find_if(m_arContentTypes.cbegin(), m_arContentTypes.cend(), [wsName](const TContentType& oContentType){ return wsName == oContentType.m_wsName; });

	if (m_arContentTypes.cend() != itFound)
		return;

	AddDefaultContentType(NSFile::GetFileExtention(wsName));

	m_arContentTypes.push_back({wsName, wsType});
}

void CConverter2OOXML::AddDefaultContentType(const HWP_STRING& wsName)
{
	if (wsName.empty())
		return;

	VECTOR<TContentType>::const_iterator itFound = std::find_if(m_arDefaultContentType.cbegin(), m_arDefaultContentType.cend(), [wsName](const TContentType& oContentType){ return wsName == oContentType.m_wsName; });

	if (m_arDefaultContentType.cend() != itFound)
		return;

	if (IsRasterFormat(wsName))
		m_arDefaultContentType.push_back({wsName, L"image/" + wsName});
	else if (L"xml" == wsName)
		m_arDefaultContentType.push_back({wsName, L"application/xml"});
}

bool CConverter2OOXML::ConvertToFile(const HWP_STRING& sFilePath)
{
	if (nullptr == m_pContext || sFilePath.empty())
		return false;

	CreateEmptyFiles();
	Convert();
	Close();

	COfficeUtils oZip;
	oZip.CompressFileOrDirectory(m_sTempDirectory, sFilePath);

	return true;
}

bool CConverter2OOXML::ConvertToDir(const HWP_STRING& sDirectoryPath)
{
	if (nullptr == m_pContext || sDirectoryPath.empty())
		return false;

	HWP_STRING sCurrentTempDir{m_sTempDirectory};

	SetTempDirectory(sDirectoryPath);

	CreateEmptyFiles();
	Convert();
	Close();

	SetTempDirectory(sCurrentTempDir);

	return true;
}

HWP_STRING CConverter2OOXML::GetTempDirectory() const
{
	return m_sTempDirectory;
}
}

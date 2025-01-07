#include "Converter2OOXML.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../DesktopEditor/raster/Metafile/svg/CSvgFile.h"

#include "../Paragraph/ParaText.h"
#include "../Paragraph/CtrlCharacter.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlTable.h"
#include "../Paragraph/CtrlShapeRect.h"
#include "../Paragraph/CtrlShapeArc.h"
#include "../Paragraph/CtrlShapeLine.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlNote.h"

#include "../HWPElements/HWPRecordBinData.h"
#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordCharShape.h"

#include "Transform.h"

#include <sstream>
#include <iomanip>

#define DEFAULT_FONT_FAMILY std::wstring(L"Arial")
#define DEFAULT_FONT_SIZE 18
#define DEFAULT_LANGUAGE std::wstring(L"en")
#define PARA_SPACING_SCALE 0.85
#define SPACING_SCALE_MS_WORD 1.21

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
	: m_pHWPFile(nullptr), m_ushShapeCount(0), m_ushPageCount(1), m_ushTableCount(0), m_ushEquationCount(0)
{}

CConverter2OOXML::~CConverter2OOXML()
{}

void CConverter2OOXML::SetHWPFile(CHWPFile_Private* pHWPFile)
{
	m_pHWPFile = pHWPFile;
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

	m_oNoteXmlRels.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");
	m_oDocXml     .WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>");
	m_oWebSettings.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/>");

	m_oStylesXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">");
	m_oStylesXml.WriteString(L"<w:rPrDefault><w:rPr>");
	m_oStylesXml.WriteString(L"<w:rFonts w:ascii=\"" + DEFAULT_FONT_FAMILY + L"\" w:eastAsia=\"" + DEFAULT_FONT_FAMILY + L"\"  w:hAnsi=\"" + DEFAULT_FONT_FAMILY + L"\" w:cs=\"" + DEFAULT_FONT_FAMILY + L"\"/>");
	m_oStylesXml.WriteString(L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>");
	m_oStylesXml.WriteString(L"<w:lang w:val=\"" + DEFAULT_LANGUAGE + L"\" w:eastAsia=\"en-US\" w:bidi=\"ar-SA\"/>");
	m_oStylesXml.WriteString(L"</w:rPr></w:rPrDefault><w:pPrDefault/>");

	m_oStylesXml.WriteString(L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/><w:rPr><w:rFonts w:eastAsiaTheme=\"minorEastAsia\"/>");
	m_oStylesXml.WriteString(L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>");
	m_oStylesXml.WriteString(L"</w:rPr></w:style>");

	m_oStylesXml.WriteString(L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>");
	m_oStylesXml.WriteString(L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>");
	m_oStylesXml.WriteString(L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>");
	m_oStylesXml.WriteString(L"<w:style w:type=\"paragraph\" w:styleId=\"normal-web\"><w:name w:val=\"Normal (Web)\"/><w:basedOn w:val=\"normal\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:pPr><w:spacing w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"/></w:pPr></w:style>");

	m_oContentTypes.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">");
	m_oContentTypes.WriteString(L"<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>");
	m_oContentTypes.WriteString(L"<Default Extension=\"xml\" ContentType=\"application/xml\"/>");
	m_oContentTypes.WriteString(L"<Default Extension=\"png\" ContentType=\"image/png\"/>");
	m_oContentTypes.WriteString(L"<Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/>");
	m_oContentTypes.WriteString(L"<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/>");
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
	m_oStylesXml.WriteString(L"</w:styles>");
	NSFile::CFileBinary oStylesWriter;
	if (oStylesWriter.CreateFileW(m_sTempDirectory + L"/word/styles.xml"))
	{
		oStylesWriter.WriteStringUTF8(m_oStylesXml.GetData());
		oStylesWriter.CloseFile();
	}

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
	m_oContentTypes.WriteString(L"</Types>");
	NSFile::CFileBinary oContentTypeWriter;
	if (oContentTypeWriter.CreateFileW(m_sTempDirectory + L"/[Content_Types].xml"))
	{
		oContentTypeWriter.WriteStringUTF8(m_oContentTypes.GetData());
		oContentTypeWriter.CloseFile();
	}

	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(m_sTempDirectory + L"/word/_rels/document.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

		for (const TRelationship& oRelationship : m_arRelationships)
			oRelsWriter.WriteStringUTF8(L"<Relationship Id=\"" + oRelationship.m_wsID + L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/" + oRelationship.m_wsType + L"\" Target=\"" + oRelationship.m_wsTarget + L"\"/>");

		oRelsWriter.WriteStringUTF8(L"</Relationships>");

		oRelsWriter.CloseFile();
	}
}

void CConverter2OOXML::Convert()
{
	TConversionState oState;

	for (const CHWPSection* pSection : m_pHWPFile->GetSections())
	{
		const bool bIsLastSection = pSection == m_pHWPFile->GetSections().back();

		for (const CHWPPargraph *pPara : pSection->GetParagraphs())
		{
			WriteParagraph(pPara, m_oDocXml, oState);
		}

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
	return L"png" == sFormat || L"jpg" == sFormat || L"jpeg" == sFormat;
}

void CConverter2OOXML::WriteParagraph(const CHWPPargraph* pParagraph, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pParagraph)
		return;

	if (0 < pParagraph->GetBreakType())
	{
		if ((0x01 == (pParagraph->GetBreakType() & 0x01) ||
		    0x02 == (pParagraph->GetBreakType() & 0x02)) &&
		    0 < oState.m_ushSecdIndex)
		{
			if (oState.m_bOpenedP)
				oBuilder.WriteString(L"</w:p>");

			oBuilder.WriteString(L"<w:p><w:r><w:br w:type=\"page\"/></w:r>");

			oState.m_bOpenedP = true;
			++m_ushPageCount;
		}
		else if (0x08 == (pParagraph->GetBreakType() & 0x08))
		{
			if (oState.m_bOpenedP)
				oBuilder.WriteString(L"</w:p>");

			oBuilder.WriteString(L"<w:p><w:r><w:br w:type=\"column\"/></w:r>");

			oState.m_bOpenedP = true;
		}
	}

	VECTOR<std::wstring> arNoteRef;

	for (const CCtrl* pCtrl : pParagraph->GetCtrls())
	{
		if (nullptr != dynamic_cast<const CParaText*>(pCtrl))
		{
			std::wstring wsText = ((const CParaText*)pCtrl)->GetText();

			if (wsText.empty())
				wsText = L" ";

			WriteText(wsText, pParagraph->GetShapeID(), ((const CParaText*)pCtrl)->GetCharShapeID(), oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlCharacter*>(pCtrl))
		{
			const CCtrlCharacter *pCtrlCharacter = (const CCtrlCharacter*)pCtrl;

			switch (pCtrlCharacter->GetType())
			{
				case ECtrlCharType::PARAGRAPH_BREAK:
				{
					if (oState.m_bOpenedP)
					{
						oState.m_bOpenedP = false;
						oBuilder.WriteString(L"</w:p>");
					}
					oState.m_bNeedLineBreak = false;
					break;
				}
				case ECtrlCharType::LINE_BREAK:
				{
					oState.m_bNeedLineBreak = true;
					break;
				}
				case ECtrlCharType::HARD_HYPHEN:
				case ECtrlCharType::HARD_SPACE:
					break;
			}
		}
		else if (nullptr != dynamic_cast<const CCtrlShapePic*>(pCtrl))
		{
			WritePicture((const CCtrlShapePic*)pCtrl, oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlTable*>(pCtrl))
		{
			WriteTable((const CCtrlTable*)pCtrl, pParagraph->GetShapeID(), oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlShapeArc*>(pCtrl) ||
		         nullptr != dynamic_cast<const CCtrlShapeEllipse*>(pCtrl) ||
		         nullptr != dynamic_cast<const CCtrlShapeLine*>(pCtrl) ||
		         nullptr != dynamic_cast<const CCtrlShapeRect*>(pCtrl))
		{
			WriteGeometryShape((const CCtrlGeneralShape*)pCtrl, oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlEqEdit*>(pCtrl))
		{
			WriteEqEditShape((const CCtrlEqEdit*)pCtrl, oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlShapeOle*>(pCtrl))
		{
			WriteOleShape((const CCtrlShapeOle*)pCtrl, oBuilder, oState);
		}
		else if (nullptr != dynamic_cast<const CCtrlNote*>(pCtrl))
		{
			arNoteRef.push_back(m_oFootnoteConverter.CreateNote((const CCtrlNote*)pCtrl, *this));
		}
		else if (nullptr != dynamic_cast<const CCtrlSectionDef*>(pCtrl))
			oState.m_pSectionDef = (const CCtrlSectionDef*)pCtrl;
		else if (nullptr != dynamic_cast<const CCtrlColumnDef*>(pCtrl))
			oState.m_pColumnDef  = (const CCtrlColumnDef*)(pCtrl);
		else if (nullptr != dynamic_cast<const CCtrlAutoNumber*>(pCtrl))
		{
			unsigned short ushValue = 0;

			//TODO:: лучше перейти не на ручной подсчет, а на автоматический в word (но там есть свои проблемы)
			switch (((const CCtrlAutoNumber*)pCtrl)->GetNumType())
			{
				case ENumType::PAGE:
				case ENumType::TOTAL_PAGE:
					ushValue = m_ushPageCount; break;
				case ENumType::FOOTNOTE:
					ushValue = m_oFootnoteConverter.GetFootnoteCount(); break;
				case ENumType::ENDNOTE:
					ushValue = m_oFootnoteConverter.GetEndnoteCount(); break;
				case ENumType::FIGURE:
					ushValue = m_ushShapeCount; break;
				case ENumType::TABLE:
					ushValue = m_ushTableCount; break;
				case ENumType::EQUATION:
					ushValue = m_ushEquationCount; break;
				case ENumType::null:
					break;
			}

			if (0 == ushValue)
				continue;

			WriteText(std::to_wstring(ushValue), pParagraph->GetShapeID(), ((const CParaText*)pCtrl)->GetCharShapeID(), oBuilder, oState);
		}
		else
			continue;

		if (!arNoteRef.empty() && nullptr == dynamic_cast<const CCtrlNote*>(pCtrl) && oState.m_bOpenedP)
		{
			for (const std::wstring& wsNoteRef : arNoteRef)
				oBuilder.WriteString(L"<w:r>" + wsNoteRef + L"</w:r>");

			arNoteRef.clear();
		}
	}

	if (oState.m_bOpenedP)
	{
		if (!arNoteRef.empty())
			for (const std::wstring& wsNoteRef : arNoteRef)
				oBuilder.WriteString(L"<w:r>" + wsNoteRef + L"</w:r>");

		oBuilder.WriteString(L"</w:p>");
		oState.m_bOpenedP = false;
	}
}

void CConverter2OOXML::WriteParagraphProperties(short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	oBuilder.WriteString(L"<w:pPr>");

	WriteParaShapeProperties(shParaShapeID, oBuilder, oState);

	oBuilder.WriteString(L"</w:pPr>");
}

void CConverter2OOXML::WriteParaShapeProperties(short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	if (nullptr != m_pHWPFile)
		pDocInfo = m_pHWPFile->GetDocInfo();

	if (nullptr == m_pHWPFile)
		return;

	const CHWPRecordParaShape* pParaShape = dynamic_cast<const CHWPRecordParaShape*>(pDocInfo->GetParaShape(shParaShapeID));

	if (nullptr == pParaShape)
		return;

	oBuilder.WriteString(L"<w:keepNext w:val=\"" + HWP_STRING((pParaShape->KeepWithNext()) ? L"true" : L"false") + L"\"/>");

	switch(pParaShape->GetHorizantalAlign())
	{
		case EHorizontalAlign::JUSTIFY: oBuilder.WriteString(L"<w:jc w:val=\"both\"/>"); break;
		case EHorizontalAlign::LEFT: oBuilder.WriteString(L"<w:jc w:val=\"left\"/>"); break;
		case EHorizontalAlign::RIGHT: oBuilder.WriteString(L"<w:jc w:val=\"right\"/>"); break;
		case EHorizontalAlign::CENTER: oBuilder.WriteString(L"<w:jc w:val=\"center\"/>"); break;
		case EHorizontalAlign::DISTRIBUTE:
		case EHorizontalAlign::DISTRIBUTE_SPACE: oBuilder.WriteString(L"<w:jc w:val=\"distribute\"/>"); break;
	}

	switch(pParaShape->GetVerticalAlign())
	{
		case EVerticalAlign::BOTTOM: oBuilder.WriteString(L"<w:textAlignment w:val=\"bottom\"/>"); break;
		case EVerticalAlign::TOP: oBuilder.WriteString(L"<w:textAlignment w:val=\"top\"/>"); break;
		case EVerticalAlign::CENTER: oBuilder.WriteString(L"<w:textAlignment w:val=\"center\"/>"); break;
		case EVerticalAlign::BASELINE:
			break;
	}

	int nLineSpacing = 0;
	HWP_STRING sType = L"auto";

	switch(pParaShape->GetLineSpacingType())
	{
		case 0x0:
		{
			sType = L"auto";
			nLineSpacing = static_cast<int>(240. * (double)pParaShape->GetLineSpacing() / 100.);
			break;
		}
		case 0x01:
		{
			sType = L"exact";
			nLineSpacing = static_cast<int>((double)pParaShape->GetLineSpacing() / 2. * 20)/*0.352778*/; //(1pt=0.352778mm) //TODO:: проверить, как найдется пример
			break;
		}
		case 0x02:
		case 0x03:
		default:
		{
			sType = L"atLeast";
			nLineSpacing = static_cast<int>((double)pParaShape->GetLineSpacing());  //TODO:: проверить, как найдется пример
			break;
		}
	}

	oBuilder.WriteString(L"<w:spacing w:lineRule=\"" + sType + L"\" w:line=\"" + std::to_wstring(nLineSpacing) +
	                      L"\" w:before=\"" + std::to_wstring(static_cast<int>((double)pParaShape->GetMarginPrev() / 10.)) +
	                      L"\" w:after=\"" + std::to_wstring(static_cast<int>((double)pParaShape->GetMarginNext() / 10.)) + L"\"/>");

	switch (pParaShape->GetHeadingType())
	{
		case EHeadingType::NUMBER:
		case EHeadingType::BULLET:
		{
			const int nNumId = m_oNumberingConverter.CreateNumbering(dynamic_cast<const CHWPRecordNumbering*>(pDocInfo->GetNumbering(pParaShape->GetHeadingIdRef())), pParaShape->GetHeadingType());

			if (0 == nNumId)
				break;

			oBuilder.WriteString(L"<w:numPr>");

			oBuilder.WriteString(L"<w:ilvl w:val=\"0\"/>");
			oBuilder.WriteString(L"<w:numId w:val=\"" + std::to_wstring(nNumId) + L"\"/>");

			oBuilder.WriteString(L"</w:numPr>");

			break;
		}
		case EHeadingType::OUTLINE:
		case EHeadingType::NONE:
			break;
	}
}

void CConverter2OOXML::WriteTable(const CCtrlTable* pTable, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pTable || pTable->Empty())
		return;

	++m_ushTableCount;

	oBuilder.WriteString(L"<w:tbl>");

	WriteTableProperties(pTable, shParaShapeID, oBuilder, oState);

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
}

void CConverter2OOXML::WriteTableProperties(const CCtrlTable* pTable, short shParaShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pTable)
		return;

	oBuilder.WriteString(L"<w:tblPr>");

	oBuilder.WriteString(L"<w:tblW w:w=\"0\" w:type=\"auto\"/>");

	WriteParaShapeProperties(shParaShapeID, oBuilder, oState);

	const CHWPRecordBorderFill* pBorderFill = nullptr;

	if (nullptr != m_pHWPFile)
		pBorderFill = dynamic_cast<const CHWPRecordBorderFill*>(m_pHWPFile->GetDocInfo()->GetBorderFill(pTable->GetBorderFillID()));

	if (nullptr == pBorderFill)
	{
		oBuilder.WriteString(L"</w:tblPr>");
		return;
	}

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
			TConversionState oCellState;
			WriteParagraph(pParagraph, oBuilder, oCellState);
		}
	}
	else
		oBuilder.WriteString(L"<w:p><w:r></w:r></w:p>");

	oBuilder.WriteString(L"</w:tc>");
}

void CConverter2OOXML::WriteCellProperties(short shBorderFillID, NSStringUtils::CStringBuilder& oBuilder)
{
	const CHWPRecordBorderFill* pBorderFill = nullptr;

	if (nullptr != m_pHWPFile)
		pBorderFill = dynamic_cast<const CHWPRecordBorderFill*>(m_pHWPFile->GetDocInfo()->GetBorderFill(shBorderFillID));

	if (nullptr == pBorderFill)
		return;

	if (pBorderFill->GetFill()->ColorFill())
		oBuilder.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + Transform::IntColorToHEX(pBorderFill->GetFill()->GetFaceColor()) + L"\"/>");

	oBuilder.WriteString(L"<w:tcBorders>");

	WriteBorder(pBorderFill->GetTopBorder(), L"top", oBuilder);
	WriteBorder(pBorderFill->GetLeftBorder(), L"left", oBuilder);
	WriteBorder(pBorderFill->GetBottomBorder(), L"bottom", oBuilder);
	WriteBorder(pBorderFill->GetRightBorder(), L"right", oBuilder);

	oBuilder.WriteString(L"</w:tcBorders>");
}

void CConverter2OOXML::WriteBorder(const TBorder& oBorder, const HWP_STRING& sBorderName, NSStringUtils::CStringBuilder& oBuilder)
{
	if (0x00 == oBorder.m_chWidth || sBorderName.empty())
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
		case ELineStyle2::DOUBLE_SLIM: sType = L"thickThinMediumGap"; break;
		case ELineStyle2::SLIM_THICK: sType = L"thickThinMediumGap"; break;
		case ELineStyle2::THICK_SLIM: sType = L"thickThinMediumGap"; break;
		case ELineStyle2::SLIM_THICK_SLIM: sType = L"thickThinMediumGap"; break;
			break;
	}

	oBuilder.WriteString(L"<w:" + sBorderName + L" w:val=\"" + sType + L"\" w:sz=\"" + std::to_wstring(Transform::LineWidth2Pt((short)oBorder.m_chWidth)) + L"\" w:space=\"0\" w:color=\"" + Transform::IntColorToHEX(oBorder.m_nColor) + L"\"/>");
}

void CConverter2OOXML::WriteGeometryShape(const CCtrlGeneralShape* pGeneralShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (nullptr == pGeneralShape)
		return;

	EShapeObjectType eShapeType = GetShapeObjectType(pGeneralShape->GetID());

	if (EShapeObjectType::Unknown == eShapeType)
		return;

	const std::wstring wsWidth  = std::to_wstring(Transform::HWPUINT2OOXML(pGeneralShape->GetWidth()));
	const std::wstring wsHeight = std::to_wstring(Transform::HWPUINT2OOXML(pGeneralShape->GetHeight()));

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"<w:p>");

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");
	oBuilder.WriteString(L"<mc:AlternateContent><mc:Choice Requires=\"wps\">");
	oBuilder.WriteString(L"<w:drawing>");
	oBuilder.WriteString(L"<wp:anchor  behindDoc=\"0\" distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\" simplePos=\"0\" locked=\"0\" layoutInCell=\"0\" allowOverlap=\"1\" relativeHeight=\"2\">");
	oBuilder.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");

	if (0 != pGeneralShape->GetHorzOffset())
		oBuilder.WriteString(L"<wp:positionH relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pGeneralShape->GetHorzOffset())) + L"</wp:posOffset></wp:positionH>");

	if (0 != pGeneralShape->GetVertOffset())
		oBuilder.WriteString(L"<wp:positionV relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pGeneralShape->GetVertOffset())) + L"</wp:posOffset></wp:positionV>");

	oBuilder.WriteString(L"<wp:extent cx=\"" + wsWidth + L"\" cy=\"" + wsHeight + L"\"/>");
	oBuilder.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	oBuilder.WriteString(L"<wp:wrapNone/>"); //TODO:: добавить поддержку
	oBuilder.WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_ushShapeCount) + L"\" name=\"Shape " + std::to_wstring(++m_ushShapeCount) + L"\" descr=\"");
	oBuilder.WriteEncodeXmlString(pGeneralShape->GetDesc());
	oBuilder.WriteString(L"\"/><wp:cNvGraphicFramePr/>");
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
			oBuilder.WriteString(L"<a:prstGeom prst=\"arc\"><a:avLst>");
			oBuilder.WriteString(L"<a:gd name=\"adj1\" fmla=\"val 0\"/>");
			oBuilder.WriteString(L"<a:gd name=\"adj2\" fmla=\"val 5400000\"/>");
			oBuilder.WriteString(L"</a:avLst></a:prstGeom>");
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
		case EShapeObjectType::Curve:
		case EShapeObjectType::Ole:
		case EShapeObjectType::Picture:
		case EShapeObjectType::Polygon:
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
		const HWP_STRING sPictureId = SavePicture(pFill->GetBinItemID());

		if (!sPictureId.empty())
			oBuilder.WriteString(L"<a:blipFill><a:blip r:embed=\"" + sPictureId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></a:blipFill>");
		else
			oBuilder.WriteString(L"<a:noFill/>");
	}

	oBuilder.WriteString(L"<a:ln" + ((ELineStyle2::NONE != pGeneralShape->GetLineStyle()) ? (L" w=\"" + std::to_wstring(Transform::LineWidth2Pt(pGeneralShape->GetLineThick())) + L'\"') : std::wstring()) + L">");

	switch (pGeneralShape->GetLineStyle())
	{
		case ELineStyle2::NONE: oBuilder.WriteString(L"<a:noFill/>"); break;
		case ELineStyle2::SOLID: oBuilder.WriteString(L"<a:solidFill><a:srgbClr val=\"" + Transform::IntColorToHEX(pGeneralShape->GetLineColor()) + L"\"/></a:solidFill>"); break;
		case ELineStyle2::DASH:
		case ELineStyle2::DOT:
		case ELineStyle2::DASH_DOT:
		case ELineStyle2::DASH_DOT_DOT:
		case ELineStyle2::LONG_DASH:
		case ELineStyle2::CIRCLE:
		case ELineStyle2::DOUBLE_SLIM:
		case ELineStyle2::SLIM_THICK:
		case ELineStyle2::THICK_SLIM:
		case ELineStyle2::SLIM_THICK_SLIM:
			oBuilder.WriteString(L"<a:noFill/>"); break; //TODO:: добавить реализацию
			break;
	}
	oBuilder.WriteString(L"</a:ln>");

	oBuilder.WriteString(L"</wps:spPr>");

	unsigned int nCountParagraphs = pGeneralShape->GetCountParagraphs();

	if (0 < nCountParagraphs)
	{
		oBuilder.WriteString(L"<wps:txbx><w:txbxContent>");

		TConversionState oShapeState;

		for (unsigned int unParaIndex = 0; unParaIndex < nCountParagraphs; ++unParaIndex)
			WriteParagraph(pGeneralShape->GetParagraphs(unParaIndex), oBuilder, oShapeState);

		oBuilder.WriteString(L"</w:txbxContent></wps:txbx>");
	}

	oBuilder.WriteString(L"<wps:bodyPr/>");

	oBuilder.WriteString(L"</wps:wsp></a:graphicData></a:graphic></wp:anchor></w:drawing></mc:Choice></mc:AlternateContent></w:r>");

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"</w:p>");
}

void CConverter2OOXML::WriteEqEditShape(const CCtrlEqEdit* pEqEditShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	//TODO:: добавить конвертацию eqn формулы в ooxml
	++m_ushEquationCount;

	if (!oState.m_bOpenedP)
	{
		oBuilder.WriteString(L"<w:p>");
		oState.m_bOpenedP = true;
	}

	oBuilder.WriteString(L"<w:r>");

	oBuilder.WriteString(L"<w:t xml:space=\"preserve\">");
	oBuilder.WriteEncodeXmlString(pEqEditShape->GetEqn());
	oBuilder.WriteString(L"</w:t></w:r>");
}

void CConverter2OOXML::WriteOleShape(const CCtrlShapeOle* pOleShape, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	//TODO:: добавить конвертацию hwp ole -> ooxml chart
	//TODO:: необходимо добавить поддержку формата "Hwp Document File Formats - Charts" (для случаев, когда нет ooxml представления)
	// Пока можем вытащить лишь ooxml представление данных

	CHWPStream oBuffer;
	HWP_STRING sFormat;

	if (!GetBinBytes(pOleShape->GetBinDataID(), oBuffer, sFormat))
		return;

	m_oOleConverter.CreateChart(oBuffer);

	const unsigned int unChartIndex = m_oOleConverter.GetChartsCount();

	if (0 == unChartIndex)
		return;

	const std::wstring wsWidth  = std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetWidth()));
	const std::wstring wsHeight = std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetHeight()));

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"<w:p>");

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr>");
	oBuilder.WriteString(L"<w:drawing>");
	oBuilder.WriteString(L"<wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">");

	if (0 != pOleShape->GetHorzOffset())
		oBuilder.WriteString(L"<wp:positionH relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetHorzOffset())) + L"</wp:posOffset></wp:positionH>");

	if (0 != pOleShape->GetVertOffset())
		oBuilder.WriteString(L"<wp:positionV relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pOleShape->GetVertOffset())) + L"</wp:posOffset></wp:positionV>");

	const std::wstring wsRelID = AddRelationship(L"chart", L"charts/chart" + std::to_wstring(unChartIndex) + L".xml");

	oBuilder.WriteString(L"<wp:extent cx=\"" + wsWidth + L"\" cy=\"" + wsHeight + L"\"/>");
	oBuilder.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	oBuilder.WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_ushShapeCount) + L"\" name=\"Shape " + std::to_wstring(++m_ushShapeCount) + L"\" descr=\"");
	oBuilder.WriteEncodeXmlString(pOleShape->GetDesc());
	oBuilder.WriteString(L"\"/><wp:cNvGraphicFramePr/>");
	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\">");
	oBuilder.WriteString(L"<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:id=\"" + wsRelID + L"\"/>");
	oBuilder.WriteString(L"</a:graphicData></a:graphic></wp:inline></w:drawing></w:r>");

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"</w:p>");

	AddContentType(L"charts/chart" + std::to_wstring(unChartIndex), L"application/vnd.openxmlformats-officedocument.drawingml.chart+xml");
	AddContentType(L"charts/style" + std::to_wstring(unChartIndex), L"application/vnd.ms-office.chartstyle+xml");
	AddContentType(L"charts/colors" + std::to_wstring(unChartIndex), L"application/vnd.ms-office.chartcolorstyle+xml");
}

void CConverter2OOXML::WriteSectionSettings(TConversionState& oState)
{
	m_oDocXml.WriteString(L"<w:sectPr>");

	if (nullptr != oState.m_pSectionDef)
	{
		for (const CCtrlHeadFoot* pCtrlHeadFoot : oState.m_pSectionDef->GetHeaderFooters())
		{
			const std::wstring wsID = m_oFootnoteConverter.CreateHeadOrFoot((const CCtrlHeadFoot*)pCtrlHeadFoot, *this);

			if (!wsID.empty() && wsID.length() > 6)
			{
				const std::wstring wsType = wsID.substr(0, 6);

				AddContentType(wsID, L"application/vnd.openxmlformats-officedocument.wordprocessingml." + wsType + L"+xml");

				m_oDocXml.WriteString(L"<w:" + wsType + L"Reference w:type=\"default\" r:id=\"" + AddRelationship(wsType, wsID) + L"\"/>");
			}
		}
	}

	if (nullptr != oState.m_pColumnDef && 1 < oState.m_pColumnDef->GetColCount())
	{
		//TODO:: Добавить поддержку остальный свойств
		m_oDocXml.WriteString(L"<w:cols w:num=\"" + std::to_wstring(oState.m_pColumnDef->GetColCount()) + L"\"  w:space=\"454\" w:equalWidth=\"true\"");

		if (ELineStyle2::NONE != oState.m_pColumnDef->GetColLineStyle())
			m_oDocXml.WriteString(L" w:sep=\"true\"");

		m_oDocXml.WriteString(L"/>");
	}

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
	m_oDocXml.WriteString(L"<w:cols w:space=\"708\"/>");
	m_oDocXml.WriteString(L"<w:docGrid w:linePitch=\"360\"/>");
	m_oDocXml.WriteString(L"</w:sectPr>");
}

void CConverter2OOXML::WritePicture(const CCtrlShapePic* pCtrlPic, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState)
{
	if (nullptr == pCtrlPic)
		return;

	HWP_STRING sPictureID = SavePicture(pCtrlPic->GetBinDataID());

	if (sPictureID.empty())
		return;

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"<w:p>");

	oBuilder.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr><w:drawing>");
	oBuilder.WriteString(L"<wp:anchor  behindDoc=\"0\" distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\" simplePos=\"0\" locked=\"0\" layoutInCell=\"0\" allowOverlap=\"1\" relativeHeight=\"2\">");
	oBuilder.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");
	oBuilder.WriteString(L"<wp:positionH relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetHorzOffset())) + L"</wp:posOffset></wp:positionH>");
	oBuilder.WriteString(L"<wp:positionV relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetVertOffset())) + L"</wp:posOffset></wp:positionV>");
	oBuilder.WriteString(L"<wp:extent cx=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetCurWidth())) + L"\" cy=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetCurHeight())) + L"\"/>");
	oBuilder.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	oBuilder.WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
	oBuilder.WriteString(L"<wp:docPr id=\"" + pCtrlPic->GetBinDataID() + L"\" name=\"Picture" + pCtrlPic->GetBinDataID() + L"\"/>");
	oBuilder.WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
	oBuilder.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	oBuilder.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	oBuilder.WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + pCtrlPic->GetBinDataID() + L"\" name=\"Picture" + pCtrlPic->GetBinDataID() + L"\"/>");
	oBuilder.WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
	oBuilder.WriteString(L"<pic:blipFill><a:blip r:embed=\"" + sPictureID + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
	oBuilder.WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetCurWidth())) + L"\" cy=\"" + std::to_wstring(Transform::HWPUINT2OOXML(pCtrlPic->GetCurHeight())) + L"\"/></a:xfrm>");
	oBuilder.WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr></pic:pic></a:graphicData></a:graphic>");
	oBuilder.WriteString(L"<wp14:sizeRelH relativeFrom=\"page\"><wp14:pctWidth>0</wp14:pctWidth></wp14:sizeRelH><wp14:sizeRelV relativeFrom=\"page\"><wp14:pctHeight>0</wp14:pctHeight></wp14:sizeRelV>");
	oBuilder.WriteString(L"</wp:anchor></w:drawing></w:r>");

	if (!oState.m_bOpenedP)
		oBuilder.WriteString(L"</w:p>");
}

bool CConverter2OOXML::SaveSVGFile(const HWP_STRING& sSVG, const HWP_STRING& sIndex)
{
	if (sSVG.empty())
		return false;

	CSvgFile oSvgReader;

	NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
	NSFonts::IFontManager* pFontManager = pFonts->GenerateFontManager();
	NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();

	pFontCache->SetStreams(pFonts->GetStreams());
	pFontManager->SetOwnerCache(pFontCache);

	oSvgReader.SetFontManager(pFontManager);

	if (!oSvgReader.ReadFromWString(sSVG))
	{
		RELEASEINTERFACE(pFontManager);
		pFonts->Release();
		return false;
	}

	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pFontManager);

	double dX, dY, dW, dH;
	oSvgReader.GetBounds(dX, dY, dW, dH);

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

	oSvgReader.Draw(pGrRenderer, 0, 0, dWidth, dHeight);

	oFrame.SaveFile(m_sTempDirectory + L"/word/media/image" + sIndex + L".png", 4);
	oFrame.put_Data(NULL);

	RELEASEINTERFACE(pFontManager);
	RELEASEINTERFACE(pGrRenderer);

	if (pBgraData)
		free(pBgraData);

	pFonts->Release();

	return true;
}

HWP_STRING CConverter2OOXML::SavePicture(const HWP_STRING& sBinItemId)
{
	CHWPStream oBuffer;
	HWP_STRING sFormat;

	if (!GetBinBytes(sBinItemId, oBuffer, sFormat))
		return HWP_STRING();

	oBuffer.MoveToStart();

	if (IsRasterFormat(sFormat))
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(m_sTempDirectory + L"/word/media/image" + sBinItemId + L'.' + sFormat);
		if (!oFile.WriteFile((unsigned char*)oBuffer.GetCurPtr(), oBuffer.GetSize()))
		{
			oFile.CloseFile();
			return HWP_STRING();
		}
		oFile.CloseFile();
	}
	else if (L"svg" == sFormat)
	{
		std::string sSVG(oBuffer.GetCurPtr(), oBuffer.GetSize());
		if (!SaveSVGFile(UTF8_TO_U(sSVG), sBinItemId))
			return HWP_STRING();

		sFormat = L"png";
	}

	AddContentType(L"media/image" + sBinItemId + L'.' + sFormat, L"image/" + sFormat);

	return AddRelationship(L"image", L"media/image" + sBinItemId + L'.' + sFormat);
}

void CConverter2OOXML::WriteRunnerStyle(short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, const TConversionState& oState)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	if (nullptr != m_pHWPFile)
	{
		pDocInfo = m_pHWPFile->GetDocInfo();

		const CHWPRecordCharShape* pCharShape = dynamic_cast<const CHWPRecordCharShape*>(pDocInfo->GetCharShape(shCharShapeID));

		if (nullptr == pCharShape)
			return;

		oBuilder.WriteString(L"<w:rPr>");

		HWP_STRING sFontFamily = pCharShape->GetFontName(ELang::LATIN);
		HWP_STRING sFontFamilyAsian = pCharShape->GetFontName(ELang::HANGUL);

		if (sFontFamilyAsian.empty() && !sFontFamily.empty())
			sFontFamilyAsian = sFontFamily;
		else if (!sFontFamilyAsian.empty() && sFontFamily.empty())
			sFontFamily = sFontFamilyAsian;

		if (!sFontFamily.empty() && !sFontFamilyAsian.empty())
		{
			oBuilder.WriteString(L"<w:rFonts w:ascii=\"" + sFontFamily +
			                      L"\" w:hAnsi=\"" + sFontFamily +
			                      L"\" w:cs=\"" + sFontFamily +
			                      L"\" w:eastAsia=\"" + sFontFamilyAsian + L"\"/>");
		}

		if (pCharShape->Bold())
			oBuilder.WriteString(L"<w:b/><w:bCs/>");

		if (pCharShape->Italic())
			oBuilder.WriteString(L"<w:i/><w:iCs/>");

		const int nHeight = static_cast<int>(((double)(std::abs)(pCharShape->GetHeight()) * ((double)pCharShape->GetRelSize(ELang::LATIN) / 100.) / 100.) * 2.);

		oBuilder.WriteString(L"<w:sz w:val=\"" + std::to_wstring(nHeight) + L"\"/><w:szCs w:val=\"" + std::to_wstring(nHeight) + L"\"/>");

		oBuilder.WriteString(L"<w:color w:val=\"" + Transform::IntColorToHEX(pCharShape->GetTextColor()) + L"\"/>");

		if (pCharShape->Underline())
		{
			EUnderline eUnderlineType = pCharShape->GetUnderlineType();
			ELineStyle1 eUnderlineStyle = pCharShape->GetUnderlineStyle();

			if (EUnderline::BOTTOM == eUnderlineType)
			{
				oBuilder.WriteString(L"<w:u w:val=\"");

				switch (eUnderlineStyle)
				{
					case ELineStyle1::SOLID: oBuilder.WriteString(L"single"); break;
					case ELineStyle1::DASH: oBuilder.WriteString(L"dash"); break;
					case ELineStyle1::DOT: oBuilder.WriteString(L"dotted"); break;
					case ELineStyle1::DASH_DOT: oBuilder.WriteString(L"dotDash"); break;
					case ELineStyle1::DASH_DOT_DOT: oBuilder.WriteString(L"dotDotDash"); break;
					case ELineStyle1::LONG_DASH: oBuilder.WriteString(L"dotDash"); break;
					case ELineStyle1::CIRCLE: oBuilder.WriteString(L"dotted"); break;
					case ELineStyle1::DOUBLE_SLIM: oBuilder.WriteString(L"double"); break;
					case ELineStyle1::SLIM_THICK: oBuilder.WriteString(L"double"); break;
					case ELineStyle1::THICK_SLIM: oBuilder.WriteString(L"double"); break;
					case ELineStyle1::SLIM_THICK_SLIM: oBuilder.WriteString(L"double"); break;
					case ELineStyle1::WAVE: oBuilder.WriteString(L"wave"); break;
					case ELineStyle1::DOUBLE_WAVE: oBuilder.WriteString(L"wavyDouble"); break;
					case ELineStyle1::THICK_3D: oBuilder.WriteString(L"thick"); break;
					case ELineStyle1::THICK_3D_REVERS_LI: oBuilder.WriteString(L"thick"); break;
					case ELineStyle1::SOLID_3D: oBuilder.WriteString(L"thick"); break;
					case ELineStyle1::SOLID_3D_REVERS_LI: oBuilder.WriteString(L"thick"); break;
				}

				oBuilder.WriteString(L"\" w:color=\"" + Transform::IntColorToHEX(pCharShape->GetUnderlineColor()) + L"\"/>");
			}
			else if (EUnderline::CENTER == eUnderlineType)
			{
				if (eUnderlineStyle == ELineStyle1::DOUBLE_SLIM ||
				    eUnderlineStyle == ELineStyle1::DOUBLE_WAVE)
					oBuilder.WriteString(L"<w:dstrike/>");
				else
					oBuilder.WriteString(L"<w:strike/>");
			}
		}

		double dSpacing = ((double)pCharShape->GetHeight() / 100.) * ((double)pCharShape->GetSpacing(ELang::HANGUL) / 100) * 0.8 + 0.4;
		dSpacing *= 20; // pt to twips (20 = 1440 / 72)

		oBuilder.WriteString(L"<w:spacing w:val=\"" + std::to_wstring((int)std::round(dSpacing)) + L"\"/>");

		oBuilder.WriteString(L"</w:rPr>");
	}
}

void CConverter2OOXML::WriteText(const std::wstring& wsText, short shParaShapeID, short shCharShapeID, NSStringUtils::CStringBuilder& oBuilder, TConversionState& oState)
{
	if (!oState.m_bOpenedP)
	{
		oBuilder.WriteString(L"<w:p>");
		oState.m_bOpenedP = true;
		WriteParagraphProperties(shParaShapeID, oBuilder, oState);
	}

	oBuilder.WriteString(L"<w:r>");

	WriteRunnerStyle(shCharShapeID, oBuilder, oState);

	if (oState.m_bNeedLineBreak)
	{
		oBuilder.WriteString(L"<w:br/>");
		oState.m_bNeedLineBreak = false;
	}

	oBuilder.WriteString(L"<w:t xml:space=\"preserve\">");
	oBuilder.WriteEncodeXmlString(wsText);
	oBuilder.WriteString(L"</w:t></w:r>");
}

bool CConverter2OOXML::GetBinBytes(const HWP_STRING& sID, CHWPStream& oBuffer, HWP_STRING& sFormat)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	if (nullptr != m_pHWPFile)
	{
		pDocInfo = m_pHWPFile->GetDocInfo();

		const CHWPRecordBinData* pBinData = dynamic_cast<const CHWPRecordBinData*>(pDocInfo->GetBinData(sID));

		if (nullptr == pBinData)
			return false;

		if (EType::LINK == pBinData->GetType())
		{
			NSFile::CFileBinary oFile;
			unsigned char *pBuffer = nullptr;
			unsigned long ulSize = 0;

			oFile.ReadAllBytes(pBinData->GetPath(), &pBuffer, ulSize);
			oBuffer.SetStream((HWP_BYTE*)pBuffer, ulSize, false);
			sFormat = NSFile::GetFileExtention(pBinData->GetPath());
		}
		else
		{
			std::wostringstream oStringStream;
			oStringStream << L"BIN" << std::setw(4) << std::setfill(L'0') << std::hex << pBinData->GetBinDataID() << L"." << pBinData->GetFormat();
			sFormat = pBinData->GetFormat();

			return m_pHWPFile->GetChildStream(oStringStream.str(), pBinData->GetCompressed(), oBuffer);
		}
	}

	return true;
}

HWP_STRING CConverter2OOXML::AddRelationship(const HWP_STRING& wsType, const HWP_STRING& wsTarget)
{
	if (wsType.empty() || wsTarget.empty())
		return HWP_STRING();

	m_arRelationships.push_back({L"rId" + std::to_wstring(m_arRelationships.size() + 1), wsType, wsTarget});

	return m_arRelationships.back().m_wsID;
}

void CConverter2OOXML::AddContentType(const HWP_STRING& wsName, const HWP_STRING& wsType)
{
	if (wsName.empty() || wsType.empty())
		return;

	m_oContentTypes.WriteString(L"<Override PartName=\"/word/" + wsName + L"\" ContentType=\"" + wsType + L"\"/>");
}

bool CConverter2OOXML::ConvertToFile(const HWP_STRING& sFilePath)
{
	if (nullptr == m_pHWPFile || sFilePath.empty())
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
	if (nullptr == m_pHWPFile || sDirectoryPath.empty())
		return false;

	SetTempDirectory(sDirectoryPath);

	CreateEmptyFiles();
	Convert();
	Close();

	return true;
}

HWP_STRING CConverter2OOXML::GetTempDirectory() const
{
	return m_sTempDirectory;
}

TConversionState::TConversionState()
	: m_bOpenedP(false), m_bOpenedR(false), m_bNeedLineBreak(false), m_ushSecdIndex(0), m_pSectionDef(nullptr), m_pColumnDef(nullptr)
{}

}

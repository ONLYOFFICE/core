#include "Converter2OOXML.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../../DesktopEditor/raster/Metafile/svg/CSvgFile.h"

#include "../Paragraph/CtrlCharacter.h"
#include "../Paragraph/ParaText.h"
#include "../Paragraph/CtrlSectionDef.h"

#include "../HWPElements/HWPRecordBinData.h"
#include "../HWPElements/HWPRecordStyle.h"
#include "../HWPElements/HWPRecordParaShape.h"
#include "../HWPElements/HWPRecordCharShape.h"

#include "Transform.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#define DEFAULT_FONT_FAMILY std::wstring(L"Arial")
#define DEFAULT_FONT_SIZE 18
#define DEFAULT_LANGUAGE std::wstring(L"en")

namespace HWP
{
CConverter2OOXML::CConverter2OOXML()
	: m_pHWPFile(nullptr)
{}

CConverter2OOXML::~CConverter2OOXML()
{
	Clear();
}

void CConverter2OOXML::Clear()
{
	m_oStylesXml.Clear();
	m_oDocXmlRels.Clear();
	m_oNoteXmlRels.Clear();
	m_oDocXml.Clear();
	m_oNoteXml.Clear();
	m_oNumberXml.Clear();
	m_oWebSettings.Clear();
}

void CConverter2OOXML::SetHWPFile(CHWPFile_Private* pHWPFile)
{
	m_pHWPFile = pHWPFile;
}

void CConverter2OOXML::SetTempDirectory(const HWP_STRING& sTempDirectory)
{
	m_sTempDirectory = sTempDirectory;
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

	// [Content_Types].xml
	std::wstring wsContent = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"svg\" ContentType=\"image/svg+xml\"/><Default Extension=\"jfif\" ContentType=\"image/jpeg\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/footnotes.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/><Override PartName=\"/word/numbering.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\"/></Types>";
	NSFile::CFileBinary oContentWriter;
	if (oContentWriter.CreateFileW(m_sTempDirectory + L"/[Content_Types].xml"))
	{
		oContentWriter.WriteStringUTF8(wsContent);
		oContentWriter.CloseFile();
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
	std::wstring wsSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
	NSFile::CFileBinary oSettingsWriter;
	if (oSettingsWriter.CreateFileW(m_sTempDirectory + L"/word/settings.xml"))
	{
		oSettingsWriter.WriteStringUTF8(wsSettings);
		oSettingsWriter.CloseFile();
	}

	// numbering.xml
	// Маркированный список
	m_oNumberXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:numbering xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:abstractNum w:abstractNumId=\"0\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(183);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"720\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1440\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(167);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2160\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(183);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2880\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3600\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(167);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"4320\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(183);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5040\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5760\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
	m_oNumberXml.AddCharSafe(167);
	m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"6480\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl></w:abstractNum>";

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
	m_oDocXmlRels  += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
	m_oDocXmlRels  += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";
	m_oDocXmlRels  += L"<Relationship Id=\"rId7\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering\" Target=\"numbering.xml\"/>";
	m_oNoteXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
	m_oDocXml      += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";
	m_oNoteXml     += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
	m_oNoteXml     += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";
	m_oWebSettings += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/>";

	m_oStylesXml   += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">";
	m_oStylesXml += L"<w:rPrDefault><w:rPr>";
	m_oStylesXml += L"<w:rFonts w:ascii=\"" + DEFAULT_FONT_FAMILY + L"\" w:eastAsia=\"" + DEFAULT_FONT_FAMILY + L"\"  w:hAnsi=\"" + DEFAULT_FONT_FAMILY + L"\" w:cs=\"" + DEFAULT_FONT_FAMILY + L"\"/>";
	m_oStylesXml += L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>";
	m_oStylesXml += L"<w:lang w:val=\"" + DEFAULT_LANGUAGE + L"\" w:eastAsia=\"en-US\" w:bidi=\"ar-SA\"/>";
	m_oStylesXml += L"</w:rPr></w:rPrDefault><w:pPrDefault/>";

	m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/><w:rPr><w:rFonts w:eastAsiaTheme=\"minorEastAsia\"/>";
	m_oStylesXml += L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>";
	m_oStylesXml += L"</w:rPr></w:style>";

	m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>";
	m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>";
	m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>";
	m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal-web\"><w:name w:val=\"Normal (Web)\"/><w:basedOn w:val=\"normal\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:pPr><w:spacing w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"/></w:pPr></w:style>";
}

void CConverter2OOXML::Close()
{
	// Дописываем концы файлов
	m_oDocXmlRels.WriteString(L"</Relationships>");
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(m_sTempDirectory + L"/word/_rels/document.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
		oRelsWriter.CloseFile();
	}

	m_oNoteXmlRels.WriteString(L"</Relationships>");
	if (oRelsWriter.CreateFileW(m_sTempDirectory + L"/word/_rels/footnotes.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_oNoteXmlRels.GetData());
		oRelsWriter.CloseFile();
	}

	m_oDocXml.WriteString(L"</w:body></w:document>");

	NSFile::CFileBinary oDocumentWriter;
	if (oDocumentWriter.CreateFileW(m_sTempDirectory + L"/word/document.xml"))
	{
		oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
		oDocumentWriter.CloseFile();
	}

	m_oNoteXml.WriteString(L"</w:footnotes>");
	NSFile::CFileBinary oFootnotesWriter;
	if (oFootnotesWriter.CreateFileW(m_sTempDirectory + L"/word/footnotes.xml"))
	{
		oFootnotesWriter.WriteStringUTF8(m_oNoteXml.GetData());
		oFootnotesWriter.CloseFile();
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
	// Маркированный список
	m_oNumberXml.WriteString(L"<w:num w:numId=\"1\"><w:abstractNumId w:val=\"0\"/></w:num>");
	m_oNumberXml.WriteString(L"</w:numbering>");

	NSFile::CFileBinary oNumberingWriter;
	if (oNumberingWriter.CreateFileW(m_sTempDirectory + L"/word/numbering.xml"))
	{
		oNumberingWriter.WriteStringUTF8(m_oNumberXml.GetData());
		oNumberingWriter.CloseFile();
	}

	// webSettings.xml
	m_oWebSettings.WriteString(L"</w:webSettings>");
	NSFile::CFileBinary oWebSettingsWriter;
	if (oWebSettingsWriter.CreateFileW(m_sTempDirectory + L"/word/webSettings.xml"))
	{
		oWebSettingsWriter.WriteStringUTF8(m_oWebSettings.GetData());
		oWebSettingsWriter.CloseFile();
	}
}

void CConverter2OOXML::Convert()
{
	TConversionState oState;

	for (const CHWPSection* pSection : m_pHWPFile->GetSections())
	{
		const bool bIsLastSection = pSection == m_pHWPFile->GetSections().back();
		const CCtrlSectionDef *pCtrlSectionDef = nullptr;

		for (const CHWPPargraph *pPara : pSection->GetParagraphs())
		{
			bool bLineBreak = false;

			for (const CCtrl* pCtrl : pPara->GetCtrls())
			{
				if (nullptr != dynamic_cast<const CParaText*>(pCtrl))
				{
					const std::wstring wsText = ((const CParaText*)pCtrl)->GetText();

					if (!oState.m_bOpenedP)
					{
						m_oDocXml += L"<w:p>";
						oState.m_bOpenedP = true;
					}

					WriteParaStyle(pPara->GetStyleID(), pPara->GetShapeID(), ((const CParaText*)pCtrl)->GetCharShapeID(), oState);

					m_oDocXml += L"<w:r>";

					if (bLineBreak)
					{
						m_oDocXml += L"<w:br/>";
						bLineBreak = false;
					}

					m_oDocXml += L"<w:t>";
					m_oDocXml.WriteEncodeXmlString(wsText);
					m_oDocXml += L"</w:t></w:r>";
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
								m_oDocXml += L"</w:p>";
							}
							m_oDocXml += L"<w:p><w:r></w:r></w:p>";
							bLineBreak = false;
							break;
						}
						case ECtrlCharType::LINE_BREAK:
						{
							bLineBreak = true;
							break;
						}
						case ECtrlCharType::HARD_HYPHEN:
						case ECtrlCharType::HARD_SPACE:
							break;
					}
				}
				else if (nullptr != dynamic_cast<const CCtrlSectionDef*>(pCtrl))
				{
					pCtrlSectionDef = (const CCtrlSectionDef*)pCtrl;
				}
				else if (nullptr != dynamic_cast<const CCtrlShapePic*>(pCtrl))
				{
					WritePicture((const CCtrlShapePic*)pCtrl, oState);
				}
				else
					continue;
			}

			if (oState.m_bOpenedP)
			{
				m_oDocXml += L"</w:p>";
				oState.m_bOpenedP = false;
			}
		}

		if (!bIsLastSection)
			m_oDocXml += L"<w:p><w:pPr>";

		WriteSectionSettings((nullptr != pCtrlSectionDef) ? pCtrlSectionDef->GetPage() : nullptr);

		if (!bIsLastSection)
			m_oDocXml += L"</w:pPr></w:p>";
	}
}

bool CConverter2OOXML::IsRasterFormat(const HWP_STRING& sFormat)
{
	return L"png" == sFormat || L"jpg" == sFormat || L"jpeg" == sFormat;
}

void CConverter2OOXML::WriteSectionSettings(const CPage* pPage)
{
	m_oDocXml.WriteString(L"<w:sectPr>");
	if (nullptr == pPage)
	{
		//DEFAULT_VALUE
		m_oDocXml.WriteString(L"<w:pgSz w:w=\"11906\" w:h=\"16838\"/>");
		m_oDocXml.WriteString(L"<w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"708\" w:footer=\"708\" w:gutter=\"0\"/>");
	}
	else
	{
		m_oDocXml.WriteString(L"<w:pgSz w:w=\"" + std::to_wstring(pPage->GetWidth() / 5) + L"\" w:h=\"" + std::to_wstring(pPage->GetHeight() / 5) + L"\"/>");
		m_oDocXml.WriteString(L"<w:pgMar w:top=\"" + std::to_wstring(pPage->GetMarginTop() / 5) + L"\" w:right=\"" + std::to_wstring(pPage->GetMarginRight() / 5) + L"\" w:bottom=\"" +
		                        std::to_wstring(pPage->GetMarginBottom() / 5) + L"\"  w:left=\"" + std::to_wstring(pPage->GetMarginRight() / 5) + L"\" w:header=\"" +
		                        std::to_wstring(pPage->GetMarginHeader() / 5) + L"\"  w:footer=\"" + std::to_wstring(pPage->GetMarginFooter() / 5) + L"\"  w:gutter=\"" +
		                        std::to_wstring(pPage->GetMarginGutter() / 5) + L"\"/>");
	}
	m_oDocXml.WriteString(L"<w:cols w:space=\"708\"/>");
	m_oDocXml.WriteString(L"<w:docGrid w:linePitch=\"360\"/>");
	m_oDocXml.WriteString(L"</w:sectPr>");
}

void CConverter2OOXML::WritePicture(const CCtrlShapePic* pCtrlPic, const TConversionState& oState)
{
	if (nullptr == pCtrlPic)
		return;

	CHWPStream oBuffer;
	HWP_STRING sFormat;

	if (!GetBinBytes(pCtrlPic->GetBinDataID(), oBuffer, sFormat))
		return;

	oBuffer.MoveToStart();

	if (IsRasterFormat(sFormat))
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(m_sTempDirectory + L"/word/media/image" + pCtrlPic->GetBinDataID() + L'.' + sFormat);
		if (!oFile.WriteFile((unsigned char*)oBuffer.GetCurPtr(), oBuffer.GetSize()))
		{
			oFile.CloseFile();
			return;
		}
		oFile.CloseFile();
	}
	else if (L"svg" == sFormat)
	{
		std::string sSVG(oBuffer.GetCurPtr(), oBuffer.GetSize());
		if (!SaveSVGFile(UTF8_TO_U(sSVG), pCtrlPic->GetBinDataID()))
			return;

		sFormat = L"png";
	}

	m_oDocXmlRels.WriteString(L"<Relationship Id=\"Picture");
	m_oDocXmlRels.WriteString(pCtrlPic->GetBinDataID());
	m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image");
	m_oDocXmlRels.WriteEncodeXmlString(pCtrlPic->GetBinDataID() + L'.' + sFormat);
	m_oDocXmlRels.WriteString(L"\"/>");

	if (!oState.m_bOpenedP)
		m_oDocXml.WriteString(L"<w:p>");

	m_oDocXml.WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr><w:drawing>");
	m_oDocXml.WriteString(L"<wp:anchor  behindDoc=\"0\" distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\" simplePos=\"0\" locked=\"0\" layoutInCell=\"0\" allowOverlap=\"1\" relativeHeight=\"2\">");
	m_oDocXml.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");
	m_oDocXml.WriteString(L"<wp:positionH relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetHorzOffset())) + L"</wp:posOffset></wp:positionH>");
	m_oDocXml.WriteString(L"<wp:positionV relativeFrom=\"page\"><wp:posOffset>" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetVertOffset())) + L"</wp:posOffset></wp:positionV>");
	m_oDocXml.WriteString(L"<wp:extent cx=\"" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetCurWidth())) + L"\" cy=\"" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetCurHeight())) + L"\"/>");
	m_oDocXml.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	m_oDocXml.WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
	m_oDocXml.WriteString(L"<wp:docPr id=\"" + pCtrlPic->GetBinDataID() + L"\" name=\"Picture" + pCtrlPic->GetBinDataID() + L"\"/>");
	m_oDocXml.WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
	m_oDocXml.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
	m_oDocXml.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	m_oDocXml.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	m_oDocXml.WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + pCtrlPic->GetBinDataID() + L"\" name=\"Picture" + pCtrlPic->GetBinDataID() + L"\"/>");
	m_oDocXml.WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
	m_oDocXml.WriteString(L"<pic:blipFill><a:blip r:embed=\"Picture" + pCtrlPic->GetBinDataID() + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
	m_oDocXml.WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetCurWidth())) + L"\" cy=\"" + std::to_wstring(Transform::TranslateHWP2OOXML(pCtrlPic->GetCurHeight())) + L"\"/></a:xfrm>");
	m_oDocXml.WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr></pic:pic></a:graphicData></a:graphic>");
	m_oDocXml.WriteString(L"<wp14:sizeRelH relativeFrom=\"page\"><wp14:pctWidth>0</wp14:pctWidth></wp14:sizeRelH><wp14:sizeRelV relativeFrom=\"page\"><wp14:pctHeight>0</wp14:pctHeight></wp14:sizeRelV>");
	m_oDocXml.WriteString(L"</wp:anchor></w:drawing></w:r>");

	if (!oState.m_bOpenedP)
		m_oDocXml.WriteString(L"</w:p>");
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

void CConverter2OOXML::WriteParaStyle(short shStyleID, short shParaShapeID, short shCharShapeID, const TConversionState& oState)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	if (nullptr != m_pHWPFile)
	{
		pDocInfo = m_pHWPFile->GetDocInfo();

		const CHWPRecordStyle* pStyle = dynamic_cast<const CHWPRecordStyle*>(pDocInfo->GetStyle(shStyleID));
		const CHWPRecordParaShape* pParaShape = dynamic_cast<const CHWPRecordParaShape*>(pDocInfo->GetParaShape(shParaShapeID));
		const CHWPRecordCharShape* pCharShape = dynamic_cast<const CHWPRecordCharShape*>(pDocInfo->GetCharShape(shCharShapeID));
	}
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

bool CConverter2OOXML::ConvertTo(const HWP_STRING& sFilePath)
{
	if (nullptr == m_pHWPFile || sFilePath.empty())
		return false;

	Clear();
	CreateEmptyFiles();
	Convert();
	Close();

	COfficeUtils oZip;
	oZip.CompressFileOrDirectory(m_sTempDirectory, sFilePath);

	return false;
}

TConversionState::TConversionState()
	: m_bOpenedP(false), m_bOpenedR(false)
{

}

}

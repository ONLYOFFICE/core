#include "OOXMLWriter.h"

#include <cwctype>

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/SystemUtils.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include "../src/Languages.h"

namespace HTML
{
#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

#define MAX_STRING_BLOCK_SIZE (size_t)10485760

#define MAXCOLUMNSINTABLE 63
#define MAXROWSINTABLE    32767

#define DEFAULT_PAGE_WIDTH  12240 // Значение в Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Значение в Twips

#define DEFAULT_LANGUAGE std::wstring(L"en-US")
#define DEFAULT_FONT_FAMILY std::wstring(L"Times New Roman")
#define DEFAULT_FONT_SIZE 24

#define DEFAULT_IMAGE_WIDTH  304800
#define DEFAULT_IMAGE_HEIGHT 304800

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors);
inline bool ElementInHeader(const std::vector<NSCSS::CNode> arSelsectors);
inline std::wstring StandardizeHeaderId(const std::wstring& wsHeader);
inline int CalculateFontChange(const std::vector<NSCSS::CNode>& arSelectors);
inline UINT GetFontSizeLevel(UINT unFontSize);
inline UINT GetFontSizeByLevel(UINT unLevel);
inline void ReplaceSpaces(std::wstring& wsValue);

COOXMLWriter::COOXMLWriter(THTMLParameters* pHTMLParameters, NSCSS::CCssCalculator* pCSSCalculator)
	: m_pDstPath(nullptr), m_pTempDir(nullptr), m_pSrcPath(nullptr),
	  m_pBasePath(nullptr), m_pCorePath(nullptr), m_pStylesCalculator(pCSSCalculator),
	  m_pHTMLParameters(pHTMLParameters), m_nFootnoteId(1), m_nHyperlinkId(1), m_nListId(1),
	  m_nElementId(1), m_bBanUpdatePageData(false), m_bWasDivs(false), m_pFonts(nullptr)
{
	m_oPageData.SetWidth (DEFAULT_PAGE_WIDTH,  NSCSS::UnitMeasure::Twips, 0, true);
	m_oPageData.SetHeight(DEFAULT_PAGE_HEIGHT, NSCSS::UnitMeasure::Twips, 0, true);
	m_oPageData.SetMargin(1440,                NSCSS::UnitMeasure::Twips, 0, true);
	m_oPageData.SetFooter(720,                 NSCSS::UnitMeasure::Twips, 0, true);
	m_oPageData.SetHeader(720,                 NSCSS::UnitMeasure::Twips, 0, true);

	m_arStates.push(TState(nullptr));
	m_arStates.top().m_pCurrentDocument = &m_oDocXml;
}

void COOXMLWriter::SetSrcDirectory(const std::wstring& wsPath)
{
	m_pSrcPath = &wsPath;
}

void COOXMLWriter::SetDstDirectory(const std::wstring& wsPath)
{
	m_pDstPath = &wsPath;
}

void COOXMLWriter::SetTempDirectory(const std::wstring& wsPath)
{
	m_pTempDir = &wsPath;
}

void COOXMLWriter::SetBaseDirectory(const std::wstring& wsPath)
{
	m_pBasePath = &wsPath;
}

void COOXMLWriter::SetCoreDirectory(const std::wstring& wsPath)
{
	m_pCorePath = &wsPath;
}

void COOXMLWriter::Begin(const std::wstring& wsDst)
{
	// Создаем пустые папки
	NSDirectory::CreateDirectory(wsDst + L"/_rels");
	NSDirectory::CreateDirectory(wsDst + L"/docProps");
	NSDirectory::CreateDirectory(wsDst + L"/word");
	NSDirectory::CreateDirectory(wsDst + L"/word/_rels");
	NSDirectory::CreateDirectory(wsDst + L"/word/media");
	NSDirectory::CreateDirectory(wsDst + L"/word/theme");

	// theme1.xml
	std::wstring sTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:majorFont><a:minorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
	NSFile::CFileBinary oThemeWriter;
	if (oThemeWriter.CreateFileW(wsDst + L"/word/theme/theme1.xml"))
	{
		oThemeWriter.WriteStringUTF8(sTheme);
		oThemeWriter.CloseFile();
	}

	// app.xml
	std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
	if (sApplication.empty())
		sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
	#if defined(INTVER)
	const std::string sVersion = VALUE2STR(INTVER);
	#else
	const std::string sVersion{L"1.0"};
	#endif
	std::wstring sApp = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
	sApp += sApplication + L"/" + UTF8_TO_U(sVersion);
	sApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
	NSFile::CFileBinary oAppWriter;
	if (oAppWriter.CreateFileW(wsDst + L"/docProps/app.xml"))
	{
		oAppWriter.WriteStringUTF8(sApp);
		oAppWriter.CloseFile();
	}

	// .rels
	std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/><Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/></Relationships>";
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(wsDst + L"/_rels/.rels"))
	{
		oRelsWriter.WriteStringUTF8(sRels);
		oRelsWriter.CloseFile();
	}

	// [Content_Types].xml
	std::wstring sContent = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"svg\" ContentType=\"image/svg+xml\"/><Default Extension=\"jfif\" ContentType=\"image/jpeg\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/footnotes.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/><Override PartName=\"/word/numbering.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\"/></Types>";
	NSFile::CFileBinary oContentWriter;
	if (oContentWriter.CreateFileW(wsDst + L"/[Content_Types].xml"))
	{
		oContentWriter.WriteStringUTF8(sContent);
		oContentWriter.CloseFile();
	}

	// fontTable.xml
	std::wstring sFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
	NSFile::CFileBinary oFontTableWriter;
	if (oFontTableWriter.CreateFileW(wsDst + L"/word/fontTable.xml"))
	{
		oFontTableWriter.WriteStringUTF8(sFontTable);
		oFontTableWriter.CloseFile();
	}

	// settings.xml
	std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:displayBackgroundShape/><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
	NSFile::CFileBinary oSettingsWriter;
	if (oSettingsWriter.CreateFileW(wsDst + L"/word/settings.xml"))
	{
		oSettingsWriter.WriteStringUTF8(sSettings);
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

	std::wstring wsCurrentLanguage;

	// core.xml
	std::wstring sCore = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
	if(nullptr != m_pHTMLParameters)
	{
		if(!m_pHTMLParameters->m_sBookTitle.empty())
		{
			sCore += L"<dc:title>";
			sCore += EncodeXmlString(m_pHTMLParameters->m_sBookTitle);
			sCore += L"</dc:title>";
		}
		if(!m_pHTMLParameters->m_sAuthors.empty())
		{
			sCore += L"<dc:creator>";
			sCore += EncodeXmlString(m_pHTMLParameters->m_sAuthors);
			sCore += L"</dc:creator>";
		}
		if(!m_pHTMLParameters->m_sGenres.empty())
		{
			sCore += L"<dc:subject>";
			sCore += EncodeXmlString(m_pHTMLParameters->m_sGenres);
			sCore += L"</dc:subject>";
		}
		if(!m_pHTMLParameters->m_sDate.empty())
		{
			sCore += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">";
			sCore += EncodeXmlString(m_pHTMLParameters->m_sDate);
			sCore += L"</dcterms:created>";
		}
		if(!m_pHTMLParameters->m_sDescription.empty())
		{
			sCore += L"<dc:description>";
			sCore += EncodeXmlString(m_pHTMLParameters->m_sDescription);
			sCore += L"</dc:description>";
		}
		if (!m_pHTMLParameters->m_sLanguage.empty())
		{
			wsCurrentLanguage = IndentifyLanguage(m_pHTMLParameters->m_sLanguage);

			sCore += L"<dc:language>";
			sCore += wsCurrentLanguage;
			sCore += L"</dc:language>";
		}
	}
	sCore += L"<cp:lastModifiedBy/></cp:coreProperties>";
	NSFile::CFileBinary oCoreWriter;
	if (oCoreWriter.CreateFileW(wsDst + L"/docProps/core.xml"))
	{
		oCoreWriter.WriteStringUTF8(sCore);
		oCoreWriter.CloseFile();
	}

	// Начала файлов
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
	m_oStylesXml   += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">";
	m_oWebSettings += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/>";

	m_nElementId += 7;

	// docDefaults по умолчанию
	if(m_pHTMLParameters && !m_pHTMLParameters->m_sdocDefaults.empty())
		m_oStylesXml += m_pHTMLParameters->m_sdocDefaults;
	else
	{
		m_oStylesXml += L"<w:rPrDefault><w:rPr>";
		m_oStylesXml += L"<w:rFonts w:ascii=\"" + DEFAULT_FONT_FAMILY + L"\" w:eastAsia=\"" + DEFAULT_FONT_FAMILY + L"\"  w:hAnsi=\"" + DEFAULT_FONT_FAMILY + L"\" w:cs=\"" + DEFAULT_FONT_FAMILY + L"\"/>";
		m_oStylesXml += L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>";
		m_oStylesXml += L"<w:lang w:val=\"" + ((!wsCurrentLanguage.empty()) ? wsCurrentLanguage : DEFAULT_LANGUAGE) + L"\" w:eastAsia=\"en-US\" w:bidi=\"ar-SA\"/>";
		m_oStylesXml += L"</w:rPr></w:rPrDefault><w:pPrDefault/>";

//			m_oStylesXml += L"<w:pPrDefault><w:pPr><w:spacing w:after=\"200\" w:line=\"276\" w:lineRule=\"auto\"/></w:pPr></w:pPrDefault>";
	}

	// normal по умолчанию
	if(m_pHTMLParameters && !m_pHTMLParameters->m_sNormal.empty())
		m_oStylesXml += m_pHTMLParameters->m_sNormal;
	else
	{
		m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/><w:rPr><w:rFonts w:eastAsiaTheme=\"minorEastAsia\"/>";
		m_oStylesXml += L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>";
		m_oStylesXml += L"</w:rPr></w:style>";
	}

	// Маркированный список
	m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>";
	// Ссылки
	m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>";
	// Таблицы
//		m_oStylesXml += L"<w:style w:type=\"table\" w:default=\"1\" w:styleId=\"table-based\"><w:name w:val=\"Normal Table\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:tblPr><w:tblInd w:w=\"0\" w:type=\"dxa\"/><w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:left w:w=\"108\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:right w:w=\"108\" w:type=\"dxa\"/></w:tblCellMar></w:tblPr></w:style><w:style w:type=\"table\" w:styleId=\"table\"><w:name w:val=\"Table Grid\"/><w:basedOn w:val=\"table-based\"/><w:uiPriority w:val=\"59\"/><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:tblPr><w:tblBorders><w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/></w:tblBorders></w:tblPr></w:style>";
	// Сноски
	m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>";
	// Web стиль по-умолчанию
	m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal-web\"><w:name w:val=\"Normal (Web)\"/><w:basedOn w:val=\"normal\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:pPr><w:spacing w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"/></w:pPr></w:style>";
}

void COOXMLWriter::End(const std::wstring& wsDst)
{
	m_oDocXmlRels.WriteString(L"</Relationships>");
	NSFile::CFileBinary oRelsWriter;
	if (oRelsWriter.CreateFileW(wsDst + L"/word/_rels/document.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
		oRelsWriter.CloseFile();
	}

	for (const std::pair<std::wstring, std::wstring>& oFootnote : m_mFootnotes)
	{
		m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
		m_oNoteXml.WriteString(oFootnote.second);
		m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
		m_oNoteXml.WriteEncodeXmlString(oFootnote.first);
		m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");
	}

	m_oNoteXmlRels.WriteString(L"</Relationships>");
	if (oRelsWriter.CreateFileW(wsDst + L"/word/_rels/footnotes.xml.rels"))
	{
		oRelsWriter.WriteStringUTF8(m_oNoteXmlRels.GetData());
		oRelsWriter.CloseFile();
	}

	CloseP();

	m_oDocXml.WriteString(L"<w:sectPr w:rsidR=\"0007083F\" w:rsidRPr=\"0007083F\" w:rsidSect=\"0007612E\">");
	m_oDocXml.WriteString(L"<w:pgSz w:w=\"" + std::to_wstring(m_oPageData.GetWidth().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L" w:h=\"" + std::to_wstring(m_oPageData.GetHeight().ToInt(NSCSS::Twips)) + L"\"/>");
	m_oDocXml.WriteString(L"<w:pgMar w:top=\"" + std::to_wstring(m_oPageData.GetMargin().GetTop().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:right=\"" + std::to_wstring(m_oPageData.GetMargin().GetRight().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:bottom=\"" + std::to_wstring(m_oPageData.GetMargin().GetBottom().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:left=\"" + std::to_wstring(m_oPageData.GetMargin().GetLeft().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:header=\"" + std::to_wstring(m_oPageData.GetHeader().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:footer=\"" + std::to_wstring(m_oPageData.GetFooter().ToInt(NSCSS::Twips)) + L"\" ");
	m_oDocXml.WriteString(L"w:gutter=\"0\"/><w:cols w:space=\"720\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body></w:document>");

	NSFile::CFileBinary oDocumentWriter;
	if (oDocumentWriter.CreateFileW(wsDst + L"/word/document.xml"))
	{
		oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
		oDocumentWriter.CloseFile();
	}

	m_oNoteXml.WriteString(L"</w:footnotes>");
	NSFile::CFileBinary oFootnotesWriter;
	if (oFootnotesWriter.CreateFileW(wsDst + L"/word/footnotes.xml"))
	{
		oFootnotesWriter.WriteStringUTF8(m_oNoteXml.GetData());
		oFootnotesWriter.CloseFile();
	}

	// styles.xml
	m_oStylesXml.WriteString(L"</w:styles>");
	NSFile::CFileBinary oStylesWriter;
	if (oStylesWriter.CreateFileW(wsDst + L"/word/styles.xml"))
	{
		oStylesWriter.WriteStringUTF8(m_oStylesXml.GetData());
		oStylesWriter.CloseFile();
	}

	// numbering.xml
	// Маркированный список
	m_oNumberXml.WriteString(L"<w:num w:numId=\"1\"><w:abstractNumId w:val=\"0\"/></w:num>");
	// Нумерованный список
	for(int i = 1; i < m_nListId; i++)
	{
		m_oNumberXml.WriteString(L"<w:num w:numId=\"");
		m_oNumberXml.WriteString(std::to_wstring(i + 1));
		m_oNumberXml.WriteString(L"\"><w:abstractNumId w:val=\"");
		m_oNumberXml.WriteString(std::to_wstring(i));
		m_oNumberXml.WriteString(L"\"/></w:num>");
	}
	m_oNumberXml.WriteString(L"</w:numbering>");
	NSFile::CFileBinary oNumberingWriter;
	if (oNumberingWriter.CreateFileW(wsDst + L"/word/numbering.xml"))
	{
		oNumberingWriter.WriteStringUTF8(m_oNumberXml.GetData());
		oNumberingWriter.CloseFile();
	}

	// webSettings.xml
	if (m_bWasDivs)
		m_oWebSettings.WriteString(L"</w:divs>");

	m_oWebSettings.WriteString(L"</w:webSettings>");
	NSFile::CFileBinary oWebSettingsWriter;
	if (oWebSettingsWriter.CreateFileW(wsDst + L"/word/webSettings.xml"))
	{
		oWebSettingsWriter.WriteStringUTF8(m_oWebSettings.GetData());
		oWebSettingsWriter.CloseFile();
	}
}

bool COOXMLWriter::OpenP()
{
	if (m_arStates.top().m_bInP)
		return false;

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:p>");
	m_arStates.top().m_bInP = true;
	m_arStates.top().m_bWasPStyle = false;

	return true;
}

bool COOXMLWriter::OpenR()
{
	if (m_arStates.top().m_bInR)
		return false;

	OpenHyperlink();

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:r>");
	m_arStates.top().m_bInR = true;
	return true;
}

bool COOXMLWriter::OpenT()
{
	if (m_arStates.top().m_bInT)
		return false;

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:t xml:space=\"preserve\">");
	m_arStates.top().m_bInT = true;
	return true;
}

void COOXMLWriter::OpenHyperlink()
{
	if (m_arStates.top().m_bInHyperlink)
		return;

	if (!m_arStates.top().m_wsHref.empty())
	{
		if (m_arStates.top().m_bISCrossHyperlink)
			OpenCrossHyperlink(m_arStates.top().m_wsHref);
		else
			OpenExternalHyperlink(m_arStates.top().m_wsHref, m_arStates.top().m_wsTooltip);

		m_arStates.top().m_bInHyperlink = true;
	}
}

void COOXMLWriter::CloseP()
{
	m_arStates.top().m_bWasSpace = true;

	if (!m_arStates.top().m_bInP)
		return;

	CloseT();
	CloseR();
	CloseHyperlink();

	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:p>");
	m_arStates.top().m_bInP = false;
}

void COOXMLWriter::CloseR()
{
	if (!m_arStates.top().m_bInR)
		return;

	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:r>");
	m_arStates.top().m_bInR = false;
}

void COOXMLWriter::CloseT()
{
	if (!m_arStates.top().m_bInT)
		return;

	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:t>");
	m_arStates.top().m_bInT = false;
}

void COOXMLWriter::CloseHyperlink()
{
	if (!m_arStates.top().m_bInHyperlink)
		return;

	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:hyperlink>");
	m_arStates.top().m_bInHyperlink = false;

	// Сноска
	if (m_arStates.top().m_wsFootnote.empty())
		return;

	if (!m_arStates.top().m_bIsFootnote)
	{
		std::wstring sFootnoteID = std::to_wstring(m_nFootnoteId++);
		OpenR();
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
		m_arStates.top().m_pCurrentDocument->WriteString(sFootnoteID);
		m_arStates.top().m_pCurrentDocument->WriteString(L"\"/>");
		CloseR();
		m_mFootnotes.insert(std::make_pair(m_arStates.top().m_wsFootnote, sFootnoteID));
	}
	else
	{
		OpenR();
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/>");
		CloseR();
	}
}

void COOXMLWriter::BeginBlock()
{
	CloseP();

	SaveState();
	m_arStates.top().CreateNewCurrentDocument();
}

void COOXMLWriter::EndBlock(bool bAddBlock)
{
	if (m_arStates.size() == 1)
		return;

	CloseP();

	if (m_arStates.top().m_pCurrentDocument == &m_oDocXml)
	{
		RollBackState();
		return;
	}

	if (bAddBlock)
	{
		XmlString *pXmlString{m_arStates.top().m_pCurrentDocument};
		const bool bRemoveXmlString{m_arStates.top().m_bRemoveCurrentDocument};

		m_arStates.top().m_bRemoveCurrentDocument = false;
		RollBackState();

		WriteToStringBuilder(*pXmlString, *m_arStates.top().m_pCurrentDocument);

		if (bRemoveXmlString)
			delete pXmlString;
	}
	else
		RollBackState();
}

void COOXMLWriter::SetDataOutput(XmlString* pOutputData)
{
	SaveState();
	m_arStates.top().m_pCurrentDocument = pOutputData;
	m_arStates.top().m_bRemoveCurrentDocument = false;
}

void COOXMLWriter::RevertDataOutput()
{
	CloseP();
	RollBackState();
}

void COOXMLWriter::SaveState()
{
	m_arStates.push(TState(m_arStates.top().m_pCurrentDocument));
}

void COOXMLWriter::RollBackState()
{
	if(m_arStates.size() > 1)
		m_arStates.pop();
}

void COOXMLWriter::SetCurrentDocument(XmlString* pNewDocument)
{
	SaveState();

	m_arStates.top().m_pCurrentDocument = pNewDocument;
	m_arStates.top().m_bRemoveCurrentDocument = false;
}

void COOXMLWriter::Break(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (m_arStates.top().m_bInP)
	{
		OpenR();
		if(arSelectors.back().m_pCompiledStyle->m_oText.GetAlign() == L"both")
			m_arStates.top().m_pCurrentDocument->WriteString(L"<w:tab/>");
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:br/>");
		CloseR();
	}
	else
		WriteEmptyParagraph();

	m_arStates.top().m_bWasSpace = true;
}

void COOXMLWriter::SetHyperlinkData(const std::wstring& wsRef, const std::wstring& wsTooltip, bool bIsCross, const std::wstring& wsFootnote, bool bIsFootnote)
{
	CloseHyperlink();

	//TODO:: подумать как лучше сделать работу с гиперссылками
	m_arStates.top().m_wsHref = wsRef;
	m_arStates.top().m_wsTooltip = wsTooltip;
	m_arStates.top().m_bISCrossHyperlink = bIsCross;
	m_arStates.top().m_wsFootnote = wsFootnote;
	m_arStates.top().m_bIsFootnote = bIsFootnote;
}

void COOXMLWriter::ClearHyperlinkData()
{
	m_arStates.top().m_wsHref.clear();
	m_arStates.top().m_wsTooltip.clear();
	m_arStates.top().m_bISCrossHyperlink = false;
	m_arStates.top().m_wsFootnote.clear();
	m_arStates.top().m_bIsFootnote = false;
}

void COOXMLWriter::PageBreak()
{
	OpenP();
	GetCurrentDocument()->WriteString(L"<w:pPr><w:pageBreakBefore/></w:pPr>");
	CloseP();
}

void COOXMLWriter::OpenCrossHyperlink(const std::wstring& wsRef)
{
	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:hyperlink w:anchor=\"");
	const size_t nSharp = wsRef.find('#');

	std::wstring wsAnchorValue;

	if(nSharp == std::wstring::npos)
		wsAnchorValue = NSFile::GetFileName(wsRef);
	else
		wsAnchorValue = (wsRef.c_str() + nSharp + 1);

	if (!wsAnchorValue.empty())
		m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(AddAnchor(wsAnchorValue));

	m_arStates.top().m_pCurrentDocument->WriteString(L"\">");
}

void COOXMLWriter::OpenExternalHyperlink(const std::wstring& wsRef, const std::wstring& wsTooltip)
{
	XmlString& oRelationshipXml(m_oDocXmlRels);

	if (*&m_arStates.top().m_pCurrentDocument == &m_oNoteXml)
		oRelationshipXml = m_oNoteXmlRels;

	oRelationshipXml.WriteString(L"<Relationship Id=\"rHyp");
	oRelationshipXml.WriteString(std::to_wstring(m_nHyperlinkId));
	oRelationshipXml.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
	oRelationshipXml.WriteEncodeXmlString(wsRef);
	oRelationshipXml.WriteString(L"\" TargetMode=\"External\"/>");

	m_arStates.top().m_bInHyperlink = true;
	// Пишем в document.xml
	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:hyperlink w:tooltip=\"");
	m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(wsTooltip);
	m_arStates.top().m_pCurrentDocument->WriteString(L"\" r:id=\"rHyp");
	m_arStates.top().m_pCurrentDocument->WriteString(std::to_wstring(m_nHyperlinkId++));
	m_arStates.top().m_pCurrentDocument->WriteString(L"\">");
}

void COOXMLWriter::CloseCrossHyperlink(const std::vector<NSCSS::CNode>& arSelectors, std::wstring wsFootnote, const std::wstring& wsRef)
{
	if (!m_arStates.top().m_bInP)
		return;

	CloseExternalHyperlink();

	if (wsFootnote == L"href")
		wsFootnote = wsRef.substr(wsRef.find('#') + 1);

	bool bFootnote = false;
	if (arSelectors.size() > 1)
	{
		const NSCSS::CNode& oNode = arSelectors[arSelectors.size() - 2];
		bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
	}

	// Сноска
	if (!wsFootnote.empty())
	{
		if (!bFootnote)
		{
			std::wstring sFootnoteID = std::to_wstring(m_nFootnoteId++);
			OpenR();
			m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
			m_arStates.top().m_pCurrentDocument->WriteString(sFootnoteID);
			m_arStates.top().m_pCurrentDocument->WriteString(L"\"/>");
			CloseR();
			m_mFootnotes.insert(std::make_pair(wsFootnote, sFootnoteID));
		}
		else
		{
			OpenR();
			m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/>");
			CloseR();
		}
	}
}

void COOXMLWriter::CloseExternalHyperlink()
{
	if (!m_arStates.top().m_bInP && !m_arStates.top().m_bInHyperlink)
		return;

	CloseT();
	CloseR();
	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:hyperlink>");
	m_arStates.top().m_bInHyperlink = false;
}

std::wstring COOXMLWriter::WritePPr(const std::vector<NSCSS::CNode>& arSelectors)
{
	OpenP();

	if (m_arStates.top().m_bWasPStyle)
		return L"";

	std::wstring sPStyle = GetStyle(*arSelectors.back().m_pCompiledStyle, true);

	if (sPStyle.empty() && !ElementInTable(arSelectors))
		sPStyle = L"normal-web";

	std::wstring wsAnchor;

	for (std::vector<NSCSS::CNode>::const_reverse_iterator itNode{arSelectors.crbegin()}; itNode < arSelectors.crend(); ++itNode)
	{
		if (itNode->m_wsId.empty())
			continue;

		wsAnchor = itNode->m_wsId;
		break;
	}

	if (sPStyle.empty() && m_arDivId.empty() && wsAnchor.empty())
		return L"";

	m_arStates.top().m_pCurrentDocument->WriteNodeBegin(L"w:pPr");

	if (!sPStyle.empty())
	{
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:pStyle w:val=\"");
		m_arStates.top().m_pCurrentDocument->WriteString(sPStyle);
		m_arStates.top().m_pCurrentDocument->WriteString(L"\"/>");
	}

	int nLiLevel{-1};
	bool bNumberingLi{false};
	bool bInTable{false};

	for (const NSCSS::CNode& oNode : arSelectors)
	{
		if (L"ol" == oNode.m_wsName)
			bNumberingLi = true;
		else if (L"ul" == oNode.m_wsName)
			bNumberingLi = false;
		else if (L"table" ==  oNode.m_wsName)
		{
			bInTable = true;
			continue;
		}
		else
			continue;

		++nLiLevel;
	}

	if (nLiLevel >= 0)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(nLiLevel) + L"\"/><w:numId w:val=\"" +
		                                                 (!bNumberingLi ? L"1" : std::to_wstring(m_nListId)) + L"\"/></w:numPr>");

	if (!m_arDivId.empty() && !bInTable)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:divId w:val=\"" + m_arDivId.top() + L"\"/>");

	m_arStates.top().m_pCurrentDocument->WriteNodeEnd(L"w:pPr");

	m_arStates.top().m_bWasPStyle = true;

	if (!wsAnchor.empty())
		WriteEmptyBookmark(wsAnchor);

	return sPStyle;
}

std::wstring COOXMLWriter::WriteRPr(XmlString& oXml, const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_arStates.top().m_bInP)
		return L"";

	NSCSS::CCompiledStyle *pMainStyle{arSelectors.back().m_pCompiledStyle};

	const std::wstring sRStyle = GetStyle(*pMainStyle, false);

	std::wstring wsFontSize;

	const int nCalculatedFontChange{CalculateFontChange(arSelectors)};

	if (0 != nCalculatedFontChange)
	{
		int nFontSizeLevel{static_cast<int>((pMainStyle->m_oFont.Empty()) ? 3 : GetFontSizeLevel(pMainStyle->m_oFont.GetSize().ToInt(NSCSS::Point) * 2))};

		nFontSizeLevel += nCalculatedFontChange;

		const UINT unFontSize{GetFontSizeByLevel(nFontSizeLevel)};

		wsFontSize += L"<w:sz w:val=\"" + std::to_wstring(unFontSize) + L"\"/><w:szCs w:val=\"" + std::to_wstring(unFontSize) + L"\"/>";
	}

	const std::wstring wsTextMode{pMainStyle->m_oDisplay.GetVAlign().ToWString()};

	if (!sRStyle.empty() || (!wsTextMode.empty() && L"normal" != wsTextMode) || !wsFontSize.empty())
	{
		oXml.WriteString(L"<w:rPr>");
		if (!sRStyle.empty())
		{
			oXml.WriteString(L"<w:rStyle w:val=\"");
			oXml.WriteString(sRStyle);
			oXml.WriteString(L"\"/>");
		}

		if (L"sub" == wsTextMode)
			oXml.WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
		else if (L"super" == wsTextMode)
			oXml.WriteString(L"<w:vertAlign w:val=\"superscript\"/>");

		oXml.WriteString(wsFontSize);
		oXml.WriteString(L"</w:rPr>");
	}
	return sRStyle;
}

bool COOXMLWriter::WriteText(std::wstring wsText, const std::vector<NSCSS::CNode>& arSelectors)
{
	if (wsText.empty())
		return false;

	bool bBidirectional{false}, bPreformatted{false}, bQuotation{false},
	     bAddSpaces{true}, bMergedText{false}, bDeleted{false};

	for (const NSCSS::CNode& oNode : arSelectors)
	{
		if (L"bdo" == oNode.m_wsName && L"rtl" == oNode.GetAttributeValue(L"dir"))
			bBidirectional = true;
		else if (L"bdi" == oNode.m_wsName)
			bBidirectional = false;
		else if (L"pre" == oNode.m_wsName || L"nobr" == oNode.m_wsName || L"xmp" == oNode.m_wsName)
			bPreformatted = true;
		else if (L"q" == oNode.m_wsName)
			bQuotation = true;
		else if (L"span" == oNode.m_wsName)
			bAddSpaces = false;
		else if (L"del" == oNode.m_wsName)
			bDeleted = true;
	}

	const NSCSS::CCompiledStyle* pCompiledStyle{arSelectors.back().m_pCompiledStyle};

	if (!bPreformatted && nullptr != arSelectors.back().m_pCompiledStyle)
	{
		// TODO::поведение должно быть немного разное (реализовать)
		switch(pCompiledStyle->m_oDisplay.GetWhiteSpace().ToInt())
		{
			case NSCSS::NSProperties::EWhiteSpace::Pre:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Wrap:
			case NSCSS::NSProperties::EWhiteSpace::Pre_Line:
				bPreformatted = true;
			default:
				break;
		}
	}

	if (!bPreformatted && wsText.end() == std::find_if_not(wsText.begin(), wsText.end(), [](wchar_t wchChar){ return iswspace(wchChar) && 0xa0 != wchChar;}))
		return false;

	std::wstring wsHeaderId;

	if (ElementInHeader(arSelectors))
		wsHeaderId = StandardizeHeaderId(wsText);

	if(bBidirectional)
		std::reverse(wsText.begin(), wsText.end());

	const bool bInT = m_arStates.top().m_bInT;

	OpenP();

	WritePPr(arSelectors);

	std::wstring wsCrossId;

	if (!wsHeaderId.empty())
		WriteEmptyBookmark(wsHeaderId);

	if (bDeleted)
		GetCurrentDocument()->WriteString(L"<w:del w:author=\"Unknown\">");

	NSStringUtils::CStringBuilder oRPr;
	std::wstring sRStyle;

	if (OpenR())
	{
		sRStyle = WriteRPr(oRPr, arSelectors);
		WriteToStringBuilder(oRPr, *m_arStates.top().m_pCurrentDocument);
	}

	if (bQuotation)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t>");

	if (!bPreformatted && bAddSpaces && m_arStates.top().m_bInP && !m_arStates.top().m_bInR && !iswspace(wsText.front()) && !iswpunct(wsText.front()) && !m_arStates.top().m_bWasSpace)
	{
		if (pCompiledStyle->m_oDisplay.GetVAlign().Empty() || L"normal" == pCompiledStyle->m_oDisplay.GetVAlign().ToWString())
			WriteSpace();
	}

	if(bPreformatted)
	{
		size_t unBegin = 0, unEnd = wsText.find_first_of(L"\n\r\t");

		while (std::wstring::npos != unBegin)
		{
			if (OpenR())
				WriteToStringBuilder(oRPr, *m_arStates.top().m_pCurrentDocument);

			if (!bDeleted)
				OpenT();
			else
				GetCurrentDocument()->WriteString(L"<w:delText>");

			if (unEnd == std::wstring::npos)
			{
				m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(wsText.c_str() + unBegin, wsText.length() - unBegin);
				break;
			}

			if (unBegin != unEnd)
			{
				m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(wsText.c_str() + unBegin, unEnd - unBegin);
				CloseT();
			}

			if (L'\n' == wsText[unEnd])
				m_arStates.top().m_pCurrentDocument->WriteString(L"<w:br/>");
			else if (L'\t' == wsText[unEnd])
				m_arStates.top().m_pCurrentDocument->WriteString(L"<w:tab/>");

			unBegin = unEnd + 1;
			unEnd = wsText.find_first_of(L"\n\r\t", unBegin);
		}
	}
	else
	{
		ReplaceSpaces(wsText);

		if (!wsText.empty() && L'\t' == wsText[0])
		{
			m_arStates.top().m_pCurrentDocument->WriteString(L"<w:tab/>");
			wsText.erase(0, 1);
		}

		if (!wsText.empty() && std::iswspace(wsText.front()) && m_arStates.top().m_bWasSpace)
			wsText.erase(0, 1);

		if (!bDeleted)
			OpenT();
		else
			GetCurrentDocument()->WriteString(L"<w:delText>");

		if (bMergedText && !m_arStates.top().m_bWasSpace && bInT && !bPreformatted)
			m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(L" ");

		if (!wsText.empty())
		{
			m_arStates.top().m_bWasSpace = std::iswspace(wsText.back());
			m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(wsText);
		}
	}

	if (bQuotation)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t>");

	if (!bDeleted)
		CloseT();
	else
		GetCurrentDocument()->WriteString(L"</w:delText>");

	if (!bMergedText)
		CloseR();

	if (bDeleted)
		GetCurrentDocument()->WriteString(L"</w:del>");

	return true;
}

void COOXMLWriter::WriteEmptyParagraph(bool bVahish, bool bInP)
{
	if (!bInP)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:p><w:pPr>");

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/>");

	if (bVahish)
		m_arStates.top().m_pCurrentDocument->WriteString(L"<w:sz w:val=\"2\"/><w:szCs w:val=\"2\"/><w:vanish/>");

	m_arStates.top().m_pCurrentDocument->WriteString(L"</w:rPr>");

	if (!bInP)
		m_arStates.top().m_pCurrentDocument->WriteString(L"</w:pPr></w:p>");
}

void COOXMLWriter::WriteSpace()
{
	OpenR();
	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/></w:rPr><w:t xml:space=\"preserve\"> </w:t>");
	CloseR();
	m_arStates.top().m_bWasSpace = true;
}

void COOXMLWriter::WriteEmptyBookmark(const std::wstring& wsId)
{
	const std::wstring wsCrossId{WriteBookmark(wsId)};

	if (wsCrossId.empty())
		return;

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:bookmarkEnd w:id=\"");
	m_arStates.top().m_pCurrentDocument->WriteString(wsCrossId);
	m_arStates.top().m_pCurrentDocument->WriteString(L"\"/>");
}

std::wstring COOXMLWriter::AddLiteBookmark()
{
	return m_mBookmarks.insert(std::make_pair(L"Bookmark" + std::to_wstring(m_mBookmarks.size() + 1), m_mBookmarks.size() + 1)).first->first;
}

std::wstring COOXMLWriter::WriteBookmark(const std::wstring& wsId)
{
	const std::wstring sCrossId = std::to_wstring(m_mBookmarks.size() + 1);
	std::wstring sName;

	if (m_mBookmarks.end() != m_mBookmarks.find(wsId))
		sName = wsId + L"_" + std::to_wstring(++m_mBookmarks[wsId]);
	else
	{
		const anchors_map::const_iterator itFound{m_mAnchors.find(wsId)};

		if (m_mAnchors.end() != itFound)
			sName = itFound->second;
		else
			sName = AddAnchor(wsId);

		m_mBookmarks.insert({wsId, 1});
	}

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:bookmarkStart w:id=\"");
	m_arStates.top().m_pCurrentDocument->WriteString(sCrossId);
	m_arStates.top().m_pCurrentDocument->WriteString(L"\" w:name=\"");
	m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(sName);
	m_arStates.top().m_pCurrentDocument->WriteString(L"\"/>");

	return sCrossId;
}

std::wstring COOXMLWriter::AddAnchor(const std::wstring& wsAnchorValue)
{
	const anchors_map::iterator itFound = m_mAnchors.find(wsAnchorValue);

	if (m_mAnchors.end() != itFound)
		return itFound->second;

	const std::wstring wsAnchorId{L"anchor-" + std::to_wstring(m_mAnchors.size() + 1)};
	m_mAnchors[wsAnchorValue] = wsAnchorId;

	return wsAnchorId;
}

void COOXMLWriter::WriteImage(const TImageData& oImageData, const std::wstring& wsId)
{
	OpenR();

	XmlString* pCurrentDocument{m_arStates.top().m_pCurrentDocument};

	// Пишем в document.xml
	if (oImageData.ZeroSpaces())
	{
		pCurrentDocument->WriteString(L"<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"");
		pCurrentDocument->WriteString(std::to_wstring(oImageData.m_unWidth));
		pCurrentDocument->WriteString(L"\" cy=\"");
		pCurrentDocument->WriteString(std::to_wstring(oImageData.m_unHeight));
		pCurrentDocument->WriteString(L"\"/><wp:docPr id=\"");
		pCurrentDocument->WriteString(wsId);
		pCurrentDocument->WriteString(L"\" name=\"Picture " + wsId + L"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"");
		pCurrentDocument->WriteString(wsId);
		pCurrentDocument->WriteString(L"\" name=\"Picture " + wsId + L"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic");
		pCurrentDocument->WriteString(wsId);
		pCurrentDocument->WriteString(L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"");
		pCurrentDocument->WriteString(std::to_wstring(oImageData.m_unWidth));
		pCurrentDocument->WriteString(L"\" cy=\"");
		pCurrentDocument->WriteString(std::to_wstring(oImageData.m_unHeight));
		pCurrentDocument->WriteString(L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>");
	}
	else
	{
		pCurrentDocument->WriteString(L"<w:rPr><w:noProof/></w:rPr><w:drawing>");
		pCurrentDocument->WriteString(L"<wp:anchor distT=\"" + std::to_wstring(oImageData.m_nHSpace) + L"\" distB=\"" + std::to_wstring(oImageData.m_nHSpace) + L"\" distL=\"" + std::to_wstring(oImageData.m_nVSpace) + L"\" distR=\"" + std::to_wstring(oImageData.m_nVSpace) + L"\" simplePos=\"0\" relativeHeight=\"251658240\" behindDoc=\"0\" locked=\"0\" layoutInCell=\"1\" allowOverlap=\"0\">");
		pCurrentDocument->WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");
		pCurrentDocument->WriteString(L"<wp:positionH relativeFrom=\"column\"><wp:align>" + oImageData.m_wsAlign + L"</wp:align></wp:positionH>");
		pCurrentDocument->WriteString(L"<wp:positionV relativeFrom=\"line\"><wp:posOffset>0</wp:posOffset></wp:positionV>");
		pCurrentDocument->WriteString(L"<wp:extent cx=\"" + std::to_wstring(oImageData.m_unWidth) + L"\" cy=\"" + std::to_wstring(oImageData.m_unHeight) + L"\"/>");
		pCurrentDocument->WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
		pCurrentDocument->WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
		pCurrentDocument->WriteString(L"<wp:docPr id=\"" + wsId + L"\" name=\"Picture " + wsId + L"\"/>");
		pCurrentDocument->WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
		pCurrentDocument->WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
		pCurrentDocument->WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
		pCurrentDocument->WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
		pCurrentDocument->WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + wsId + L"\" name=\"Picture " + wsId + L"\"/>");
		pCurrentDocument->WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
		pCurrentDocument->WriteString(L"<pic:blipFill><a:blip r:link=\"rPic" + wsId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
		pCurrentDocument->WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(oImageData.m_unWidth) + L"\" cy=\"" + std::to_wstring(oImageData.m_unHeight) + L"\"/></a:xfrm>");
		pCurrentDocument->WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr></pic:pic></a:graphicData></a:graphic>");
		pCurrentDocument->WriteString(L"<wp14:sizeRelH relativeFrom=\"page\"><wp14:pctWidth>0</wp14:pctWidth></wp14:sizeRelH><wp14:sizeRelV relativeFrom=\"page\"><wp14:pctHeight>0</wp14:pctHeight></wp14:sizeRelV>");
		pCurrentDocument->WriteString(L"</wp:anchor></w:drawing>");
	}

	CloseR();
}

void COOXMLWriter::WriteAlternativeImage(const std::wstring& wsAlt, const std::wstring& wsSrc, const TImageData& oImageData)
{
	m_oDocXmlRels.WriteString(L"<Relationship Id=\"rId");
	m_oDocXmlRels.WriteString(std::to_wstring(m_nElementId));
	m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"");
	m_oDocXmlRels.WriteEncodeXmlString(wsSrc);
	m_oDocXmlRels.WriteString(L"\" TargetMode=\"External\"/>");

	const bool bOpenedP{OpenP()};

	WriteEmptyImage((0 != oImageData.m_unWidth) ? oImageData.m_unWidth : DEFAULT_IMAGE_WIDTH, (0 != oImageData.m_unHeight) ? oImageData.m_unHeight : DEFAULT_IMAGE_HEIGHT, L"", wsAlt);

	if (bOpenedP)
		CloseP();
}
void COOXMLWriter::WriteEmptyImage(int nWidth, int nHeight, const std::wstring& wsName, const std::wstring& wsDescr)
{
	OpenR();

	#define WRITE_ENCODE_ARGUMENT(argumentName, argumentValue) \
	m_arStates.top().m_pCurrentDocument->WriteString(L" " + std::wstring(argumentName) + L"=\""); \
	m_arStates.top().m_pCurrentDocument->WriteEncodeXmlString(argumentValue); \
	m_arStates.top().m_pCurrentDocument->WriteString(L"\"")

	m_arStates.top().m_pCurrentDocument->WriteString(L"<w:rPr><w:noProof/></w:rPr><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_nElementId - 7) + L"\"");
	WRITE_ENCODE_ARGUMENT(L"name", wsName);
	WRITE_ENCODE_ARGUMENT(L"descr", wsDescr);
	m_arStates.top().m_pCurrentDocument->WriteString(L"/>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"0\"");
	WRITE_ENCODE_ARGUMENT(L"name", wsName);
	WRITE_ENCODE_ARGUMENT(L"descr", wsDescr);
	m_arStates.top().m_pCurrentDocument->WriteString(L"/>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<pic:blipFill><a:blip r:link=\"rId" + std::to_wstring(m_nElementId++) + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr>");
	m_arStates.top().m_pCurrentDocument->WriteString(L"</pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>");

	CloseR();
}

void COOXMLWriter::WriteImageRels(const std::wstring& wsImageId, const std::wstring& wsImageName)
{
	m_oDocXmlRels.WriteString(L"<Relationship Id=\"rPic");
	m_oDocXmlRels.WriteString(wsImageId);
	m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/i");
	m_oDocXmlRels.WriteEncodeXmlString(wsImageName);
	m_oDocXmlRels.WriteString(L"\"/>");
}

std::wstring COOXMLWriter::GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bParagraphStyle)
{
	if (!((bParagraphStyle) ? m_oXmlStyle.WritePStyle(oStyle) : m_oXmlStyle.WriteRStyle(oStyle)))
		return std::wstring();

	m_oStylesXml.WriteString(m_oXmlStyle.GetStyle());
	return m_oXmlStyle.GetIdAndClear();
}

void COOXMLWriter::UpdatePageStyle(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (nullptr != m_pStylesCalculator)
		m_pStylesCalculator->CalculatePageStyle(m_oPageData, arSelectors);
}

void COOXMLWriter::SetBaseFont(const std::wstring& wsFontStyles)
{
	if (nullptr != m_pStylesCalculator)
		m_pStylesCalculator->AddStyles(wsFontStyles);
}

void COOXMLWriter::SetDivId(const std::wstring& wsDivId)
{
	m_arDivId.push(wsDivId);
	m_bWasDivs = true;
}

void COOXMLWriter::RollBackDivId()
{
	if (!m_arDivId.empty())
		m_arDivId.pop();
}

void COOXMLWriter::IncreaseListId()
{
	++m_nListId;
}

int COOXMLWriter::GetListId() const
{
	return m_nListId;
}

std::wstring COOXMLWriter::FindFootnote(const std::wstring& wsId)
{
	const std::map<std::wstring, std::wstring>::const_iterator itFound{m_mFootnotes.find(wsId)};
	return (itFound != m_mFootnotes.cend()) ? itFound->second : std::wstring();
}

void COOXMLWriter::OpenFootnote(const std::wstring& wsFootnoteID)
{
	m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
	m_oNoteXml.WriteString(wsFootnoteID);
	m_oNoteXml.WriteString(L"\">");
}

void COOXMLWriter::CloseFootnote()
{
	m_oNoteXml.WriteString(L"</w:footnote>");
}

XmlString& COOXMLWriter::GetStylesXml()
{
	return m_oStylesXml;
}

XmlString& COOXMLWriter::GetDocRelsXml()
{
	return m_oDocXmlRels;
}

XmlString& COOXMLWriter::GetNotesRelsXml()
{
	return m_oNoteXmlRels;
}

XmlString& COOXMLWriter::GetDocumentXml()
{
	return m_oDocXml;
}

XmlString& COOXMLWriter::GetNotesXml()
{
	return m_oNoteXml;
}

XmlString& COOXMLWriter::GetNumberingXml()
{
	return m_oNumberXml;
}

XmlString& COOXMLWriter::GetWebSettingsXml()
{
	return m_oWebSettings;
}

XmlString* COOXMLWriter::GetCurrentDocument() const
{
	return m_arStates.top().m_pCurrentDocument;
}

bool COOXMLWriter::SupportNestedTables() const
{
	return true;
}

const NSCSS::NSProperties::CPage* COOXMLWriter::GetPageData() const
{
	return &m_oPageData;
}

NSFonts::IApplicationFonts* COOXMLWriter::GetFonts()
{
	if (nullptr == m_pFonts)
	{
		m_pFonts = NSFonts::NSApplication::Create();

		if (NULL != m_pFonts)
			m_pFonts->Initialize();
	}

	return m_pFonts;
}

std::wstring COOXMLWriter::GetMediaDir() const
{
	return ((nullptr != m_pDstPath) ? *m_pDstPath : std::wstring()) + L"/word/media/";
}

std::wstring COOXMLWriter::GetTempDir() const
{
	return (nullptr != m_pTempDir) ? *m_pTempDir : std::wstring();
}

std::wstring COOXMLWriter::GetSrcPath() const
{
	return (nullptr != m_pSrcPath) ? *m_pSrcPath : std::wstring();
}

std::wstring COOXMLWriter::GetBasePath() const
{
	return (nullptr != m_pBasePath) ? *m_pBasePath : std::wstring();
}

std::wstring COOXMLWriter::GetCorePath() const
{
	return (nullptr != m_pCorePath) ? *m_pCorePath : std::wstring();
}

inline bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}

inline bool ElementInHeader(const std::vector<NSCSS::CNode> arSelsectors)
{
	for (const NSCSS::CNode& oNode : arSelsectors)
	{
		if (2 == oNode.m_wsName.length() && L'h' != oNode.m_wsName[0] &&
		    (L'1' == oNode.m_wsName[1] || L'2' == oNode.m_wsName[1] ||
		     L'3' == oNode.m_wsName[1] || L'4' == oNode.m_wsName[1] ||
		     L'5' == oNode.m_wsName[1] || L'6' == oNode.m_wsName[1]))
			return true;
	}

	return false;
}

std::wstring StandardizeHeaderId(const std::wstring& wsHeader)
{
	if (wsHeader.empty())
		return std::wstring();

	std::wstring result;
	result.reserve(wsHeader.size());

	// Флаг, указывающий, был ли предыдущий символ дефисом
	bool prevWasHyphen = false;
	bool inWhitespaceSequence = false;
	wchar_t lowerC;

	for (wchar_t c : wsHeader)
	{
		// Приведение к нижнему регистру
		lowerC = std::tolower(c);

		// Проверяем, является ли символ буквой или цифрой
		if (std::iswalnum(lowerC))
		{
			result.push_back(lowerC);
			prevWasHyphen = false;
			inWhitespaceSequence = false;
		}
		// Проверяем, является ли символ пробельным (пробел, табуляция и т.д.)
		else if (std::iswspace(lowerC))
		{
			// Заменяем последовательности пробельных символов на один дефис
			if (!inWhitespaceSequence && !result.empty())
			{
				result.push_back(L'-');
				inWhitespaceSequence = true;
			}
		}
		// Проверяем, является ли символ дефисом или подчеркиванием
		else if (c == L'-' || c == L'_')
		{
			// Добавляем дефис, если предыдущий символ не был дефисом
			if (!prevWasHyphen && !result.empty())
			{
				result.push_back(L'-');
				prevWasHyphen = true;
			}
			inWhitespaceSequence = false;
		}
		// Все остальные символы (знаки препинания) пропускаем
		// Но если это буква в Unicode, мы можем её обработать
		else if (std::iswalpha(lowerC))
		{
			// Для Unicode-символов, которые являются буквами
			result.push_back(lowerC);
			prevWasHyphen = false;
			inWhitespaceSequence = false;
		}
		// Остальные символы игнорируем
	}

	// Удаляем дефисы в начале и конце
	size_t start = 0;
	size_t end = result.length();

	while (start < end && result[start] == L'-')
		++start;

	while (end > start && result[end - 1] == L'-')
		--end;

	// Удаляем последовательные дефисы
	std::wstring finalResult;
	finalResult.reserve(end - start);

	bool lastWasHyphen = false;
	for (size_t i = start; i < end; i++)
	{
		if (result[i] == L'-')
		{
			if (!lastWasHyphen)
			{
				finalResult.push_back(L'-');
				lastWasHyphen = true;
			}
		}
		else
		{
			finalResult.push_back(result[i]);
			lastWasHyphen = false;
		}
	}

	return finalResult;
}

inline int CalculateFontChange(const std::vector<NSCSS::CNode>& arSelectors)
{
	int nFontChange = 0;

	for (const NSCSS::CNode& oNode : arSelectors)
	{
		if (L"big" == oNode.m_wsName)
			++nFontChange;
		else if (L"small" == oNode.m_wsName)
			--nFontChange;
	}

	return nFontChange;
}

inline UINT GetFontSizeLevel(UINT unFontSize)
{
	if (unFontSize <= 18)
		return 1;
	else if (unFontSize <= 22)
		return 2;
	else if (unFontSize <= 26)
		return 3;
	else if (unFontSize <= 30)
		return 4;
	else if (unFontSize <= 40)
		return 5;
	else if (unFontSize <= 59)
		return 6;
	else
		return 7;
}

inline UINT GetFontSizeByLevel(UINT unLevel)
{
	if (0 == unLevel)
		return 15;
	else if (unLevel > 7)
		return 72;

	switch (unLevel)
	{
		case 1: return 15;
		case 2: return 20;
		case 3: return 24;
		case 4: return 27;
		case 5: return 36;
		case 6: return 48;
		case 7: return 72;
	}
	return 24;
}

inline void ReplaceSpaces(std::wstring& wsValue)
{
	// boost::wregex oRegex(L"\\s+");
	// wsValue = boost::regex_replace(wsValue, oRegex, L" ");

	std::wstring::const_iterator itBegin = std::find_if(wsValue.cbegin(), wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	std::wstring::const_iterator itEnd;

	while (wsValue.cend() != itBegin)
	{
		itEnd = std::find_if(itBegin, wsValue.cend(), [](wchar_t wchValue){ return !std::iswspace(wchValue) || 0xa0 == wchValue; });

		wsValue.replace(itBegin, itEnd, L" ");

		itBegin = std::find_if(itBegin + 1, wsValue.cend(), [](wchar_t wchValue){ return std::iswspace(wchValue) && 0xa0 != wchValue; });
	}
}
}

#include "htmlfile2.h"

#include <string>
#include <cwctype>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>

#include "../Common/3dParty/html/htmltoxhtml.h"
#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../Common/Network/FileTransporter/include/FileTransporter.h"
#include "../DesktopEditor/common/Types.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/SystemUtils.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "htmlfile2.h"

#include <boost/regex.hpp>

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

std::wstring rStyle = L" a area b strong bdo bdi big br center cite dfn em i var code kbd samp tt del s font img ins u mark q rt sup small sub svg input basefont button label data object noscript output abbr time ruby progress hgroup meter span acronym ";

//struct CTree
//{
//    NSCSS::CNode m_oNode;
//    std::vector<CTree> m_arrChild;
//};

// Ячейка таблицы
struct CTc
{
    int i;
    int j;
    std::wstring sGridSpan = L"1";

    CTc(int _i, int _j, const std::wstring& sColspan) : i(_i), j(_j), sGridSpan(sColspan) {}

    bool operator==(const CTc& c2)
    {
        return (i == c2.i && j == c2.j && sGridSpan == c2.sGridSpan);
    }
};

// Настройки текста
struct CTextSettings
{
    bool bBdo; // Реверс текста
    bool bPre; // Сохранение форматирования (Сохранение пробелов, табуляций, переносов строк)
    int  nLi;  // Уровень списка
    std::wstring sRStyle; // w:rStyle
    std::wstring sPStyle; // w:pStyle

    CTextSettings(bool _bBdo, bool _bPre, int _nLi, const std::wstring& _sRStyle, const std::wstring& _sPStyle) :
        bBdo(_bBdo), bPre(_bPre), nLi(_nLi), sRStyle(_sRStyle), sPStyle(_sPStyle) {}

    CTextSettings(const CTextSettings& oTS) :
        bBdo(oTS.bBdo), bPre(oTS.bPre), nLi(oTS.nLi), sRStyle(oTS.sRStyle), sPStyle(oTS.sPStyle) {}
};

class CHtmlFile2_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
    NSCSS::CCssCalculator m_oStylesCalculator; // Css калькулятор
    NSCSS::CDocumentStyle m_oXmlStyle;         // Ooxml стиль

    std::wstring m_sTmp;  // Temp папка
    std::wstring m_sSrc;  // Директория источника
    std::wstring m_sDst;  // Директория назначения
    std::wstring m_sBase; // Полный базовый адрес

    NSCSS::CTree m_oTree; // Дерево body html-файла

private:
    int m_nImageId;     // ID картинки
    int m_nFootnoteId;  // ID сноски
    int m_nHyperlinkId; // ID ссылки
    int m_nCrossId;     // ID перекрестной ссылки
    int m_nNumberingId; // ID списка

    NSStringUtils::CStringBuilder m_oStylesXml;  // styles.xml
    NSStringUtils::CStringBuilder m_oDocXmlRels; // document.xml.rels
    NSStringUtils::CStringBuilder m_oDocXml;     // document.xml
    NSStringUtils::CStringBuilder m_oNoteXml;    // footnotes.xml
    NSStringUtils::CStringBuilder m_oNumberXml;  // numbering.xml

    bool m_bInP; // <w:p> открыт?
    bool m_bWasPStyle; // <w:pStyle> записан?
public:

    CHtmlFile2_Private() : m_nImageId(1), m_nFootnoteId(1), m_nHyperlinkId(1), m_nCrossId(1), m_nNumberingId(1), m_bInP(false), m_bWasPStyle(false)
    {
        //Установим размер исходного и нового окна для Css калькулятора (должны быть одинаковые единицы измерения (желательно пункты))
        //Это нужно для масштабирования некоторых значений
		m_oStylesCalculator.SetSizeSourceWindow(NSCSS::CSizeWindow(4940 * (1366 * (25.4 / m_oStylesCalculator.GetDpi())), 0));
        m_oStylesCalculator.SetSizeDeviceWindow(NSCSS::CSizeWindow(4940, 0));
    }

    ~CHtmlFile2_Private()
    {
        m_oLightReader     .Clear();
        m_oStylesCalculator.Clear();
        m_oXmlStyle        .Clear();
        m_oStylesXml       .Clear();
        m_oDocXmlRels      .Clear();
        m_oDocXml          .Clear();
        m_oNoteXml         .Clear();
        m_oNumberXml       .Clear();
    }

    // Проверяет наличие тэга html
    bool isHtml()
    {
        return (m_oLightReader.ReadNextNode() ? m_oLightReader.GetName() == L"html" : false);
    }

    // Создаёт основу docx
    void CreateDocxEmpty(CHtmlParams* oParams)
    {
        // Создаем пустые папки
        NSDirectory::CreateDirectory(m_sDst + L"/_rels");
        NSDirectory::CreateDirectory(m_sDst + L"/docProps");
        NSDirectory::CreateDirectory(m_sDst + L"/word");
        NSDirectory::CreateDirectory(m_sDst + L"/word/_rels");
        NSDirectory::CreateDirectory(m_sDst + L"/word/media");
        NSDirectory::CreateDirectory(m_sDst + L"/word/theme");

        // theme1.xml
        std::wstring sTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:majorFont><a:minorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
        NSFile::CFileBinary oThemeWriter;
        if (oThemeWriter.CreateFileW(m_sDst + L"/word/theme/theme1.xml"))
        {
            oThemeWriter.WriteStringUTF8(sTheme);
            oThemeWriter.CloseFile();
        }

        // app.xml
        std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
        if (sApplication.empty())
            sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
        #if defined(INTVER)
        std::string sVersion = VALUE2STR(INTVER);
        #endif
        std::wstring sApp = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
        sApp += sApplication + L"/" + UTF8_TO_U(sVersion);
        sApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
        NSFile::CFileBinary oAppWriter;
        if (oAppWriter.CreateFileW(m_sDst + L"/docProps/app.xml"))
        {
            oAppWriter.WriteStringUTF8(sApp);
            oAppWriter.CloseFile();
        }

        // .rels
        std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/><Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/></Relationships>";
        NSFile::CFileBinary oRelsWriter;
        if (oRelsWriter.CreateFileW(m_sDst + L"/_rels/.rels"))
        {
            oRelsWriter.WriteStringUTF8(sRels);
            oRelsWriter.CloseFile();
        }

        // [Content_Types].xml
        std::wstring sContent = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"svg\" ContentType=\"image/svg+xml\"/><Default Extension=\"jfif\" ContentType=\"image/jpeg\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/footnotes.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/><Override PartName=\"/word/numbering.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\"/></Types>";
        NSFile::CFileBinary oContentWriter;
        if (oContentWriter.CreateFileW(m_sDst + L"/[Content_Types].xml"))
        {
            oContentWriter.WriteStringUTF8(sContent);
            oContentWriter.CloseFile();
        }

        // footnotes.xml.rels
        NSFile::CFileBinary oFootRelsWriter;
        if (oFootRelsWriter.CreateFileW(m_sDst + L"/word/_rels/footnotes.xml.rels"))
        {
            oFootRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"></Relationships>");
            oFootRelsWriter.CloseFile();
        }

        // fontTable.xml
        std::wstring sFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Arial\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
        NSFile::CFileBinary oFontTableWriter;
        if (oFontTableWriter.CreateFileW(m_sDst + L"/word/fontTable.xml"))
        {
            oFontTableWriter.WriteStringUTF8(sFontTable);
            oFontTableWriter.CloseFile();
        }

        // settings.xml
        std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"14\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
        NSFile::CFileBinary oSettingsWriter;
        if (oSettingsWriter.CreateFileW(m_sDst + L"/word/settings.xml"))
        {
            oSettingsWriter.WriteStringUTF8(sSettings);
            oSettingsWriter.CloseFile();
        }

        // webSettings.xml
        std::wstring sWebSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/></w:webSettings>";
        NSFile::CFileBinary oWebWriter;
        if (oWebWriter.CreateFileW(m_sDst + L"/word/webSettings.xml"))
        {
            oWebWriter.WriteStringUTF8(sWebSettings);
            oWebWriter.CloseFile();
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
        std::wstring sCore = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
        if(oParams)
        {
            if(!oParams->m_sBookTitle.empty())
            {
                sCore += L"<dc:title>";
                sCore += oParams->m_sBookTitle;
                sCore += L"</dc:title>";
            }
            if(!oParams->m_sAuthors.empty())
            {
                sCore += L"<dc:creator>";
                sCore += oParams->m_sAuthors;
                sCore += L"</dc:creator>";
            }
            if(!oParams->m_sGenres.empty())
            {
                sCore += L"<dc:subject>";
                sCore += oParams->m_sGenres;
                sCore += L"</dc:subject>";
            }
            if(!oParams->m_sDate.empty())
            {
                sCore += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">";
                sCore += oParams->m_sDate;
                sCore += L"</dcterms:created>";
            }
            if(!oParams->m_sDescription.empty())
            {
                sCore += L"<dc:description>";
                sCore += oParams->m_sDescription;
                sCore += L"</dc:description>";
            }
        }
        sCore += L"<cp:lastModifiedBy/></cp:coreProperties>";
        NSFile::CFileBinary oCoreWriter;
        if (oCoreWriter.CreateFileW(m_sDst + L"/docProps/core.xml"))
        {
            oCoreWriter.WriteStringUTF8(sCore);
            oCoreWriter.CloseFile();
        }

        // Начала файлов
        m_oDocXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
        m_oDocXmlRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId7\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering\" Target=\"numbering.xml\"/>";
        m_oDocXml     += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";
        m_oNoteXml    += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
        m_oNoteXml    += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";
        m_oStylesXml  += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">";

        // docDefaults по умолчанию
        if(oParams && !oParams->m_sdocDefaults.empty())
            m_oStylesXml += oParams->m_sdocDefaults;
        else
            m_oStylesXml += L"<w:docDefaults><w:rPrDefault><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\" w:hint=\"default\"/><w:sz w:val=\"22\"/><w:szCs w:val=\"22\"/><w:lang w:val=\"ru-RU\" w:bidi=\"ar-SA\" w:eastAsia=\"en-US\"/></w:rPr></w:rPrDefault><w:pPrDefault><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"276\" w:after=\"200\"/></w:pPr></w:pPrDefault></w:docDefaults>";

        // normal по умолчанию
        if(oParams && !oParams->m_sNormal.empty())
            m_oStylesXml += oParams->m_sNormal;
        else
            m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/></w:style>";

        // Маркированный список
        m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>";
        // Ссылки
        m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>";
        // Таблицы
        m_oStylesXml += L"<w:style w:type=\"table\" w:default=\"1\" w:styleId=\"table-based\"><w:name w:val=\"Normal Table\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:tblPr><w:tblInd w:w=\"0\" w:type=\"dxa\"/><w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:left w:w=\"108\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:right w:w=\"108\" w:type=\"dxa\"/></w:tblCellMar></w:tblPr></w:style><w:style w:type=\"table\" w:styleId=\"table\"><w:name w:val=\"Table Grid\"/><w:basedOn w:val=\"table-based\"/><w:uiPriority w:val=\"59\"/><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:tblPr><w:tblBorders><w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/></w:tblBorders></w:tblPr></w:style>";
        // Сноски
        m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>";
    }

    // Читает файл
    void readSrc()
    {
        // Читаем html
        m_oLightReader.ReadNextNode();

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"head")
                readHead();
            else if(sName == L"body")
                readBody();
        }
    }

    // Дописывает концы docx
    void write()
    {
        m_oDocXmlRels.WriteString(L"</Relationships>");
        NSFile::CFileBinary oRelsWriter;
        if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/document.xml.rels"))
        {
            oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
            oRelsWriter.CloseFile();
        }

        if (m_bInP)
            m_oDocXml.WriteString(L"</w:p>");
        m_oDocXml.WriteString(L"<w:sectPr w:rsidR=\"0007083F\" w:rsidRPr=\"0007083F\" w:rsidSect=\"0007612E\"><w:pgSz w:w=\"12240\" w:h=\"15840\"/><w:pgMar w:gutter=\"0\" w:footer=\"720\" w:header=\"720\" w:left=\"720\" w:bottom=\"720\" w:right=\"720\" w:top=\"720\"/><w:cols w:space=\"720\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body></w:document>");
        NSFile::CFileBinary oDocumentWriter;
        if (oDocumentWriter.CreateFileW(m_sDst + L"/word/document.xml"))
        {
            oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
            oDocumentWriter.CloseFile();
        }

        m_oNoteXml.WriteString(L"</w:footnotes>");
        NSFile::CFileBinary oFootnotesWriter;
        if (oFootnotesWriter.CreateFileW(m_sDst + L"/word/footnotes.xml"))
        {
            oFootnotesWriter.WriteStringUTF8(m_oNoteXml.GetData());
            oFootnotesWriter.CloseFile();
        }

        // styles.xml
        m_oStylesXml.WriteString(L"</w:styles>");
        NSFile::CFileBinary oStylesWriter;
        if (oStylesWriter.CreateFileW(m_sDst + L"/word/styles.xml"))
        {
            oStylesWriter.WriteStringUTF8(m_oStylesXml.GetData());
            oStylesWriter.CloseFile();
        }

        // numbering.xml
        // Маркированный список
        m_oNumberXml.WriteString(L"<w:num w:numId=\"1\"><w:abstractNumId w:val=\"0\"/></w:num>");
        // Нумерованный список
        for(int i = 1; i < m_nNumberingId; i++)
        {
            m_oNumberXml.WriteString(L"<w:num w:numId=\"");
            m_oNumberXml.WriteString(std::to_wstring(i + 1));
            m_oNumberXml.WriteString(L"\"><w:abstractNumId w:val=\"");
            m_oNumberXml.WriteString(std::to_wstring(i));
            m_oNumberXml.WriteString(L"\"/></w:num>");
        }
        m_oNumberXml.WriteString(L"</w:numbering>");
        NSFile::CFileBinary oNumberingWriter;
        if (oNumberingWriter.CreateFileW(m_sDst + L"/word/numbering.xml"))
        {
            oNumberingWriter.WriteStringUTF8(m_oNumberXml.GetData());
            oNumberingWriter.CloseFile();
        }
    }

    // Конвертирует html в xhtml
    bool htmlXhtml(const std::wstring& sSrc)
    {
        BYTE* pData;
        DWORD nLength;
        if (!NSFile::CFileBinary::ReadAllBytes(sSrc, &pData, nLength))
            return false;

        std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
        bool bNeedConvert = true;
        if (nLength > 4)
        {
            if (pData[0] == 0xFF && pData[1] == 0xFE && !(pData[2] == 0x00 && pData[3] == 0x00))
                bNeedConvert = false;
            if (pData[0] == 0xFE && pData[1] == 0xFF)
                bNeedConvert = false;

            if (pData[0] == 0xFF && pData[1] == 0xFE && pData[2] == 0x00 && pData[3] == 0x00)
                bNeedConvert = false;
            if (pData[0] == 0 && pData[1] == 0 && pData[2] == 0xFE && pData[3] == 0xFF)
                bNeedConvert = false;
        }
        RELEASEARRAYOBJECTS(pData);

        size_t nFind = sFileContent.find("version=\"");
        if(nFind != std::string::npos)
        {
            nFind += 9;
            size_t nFindEnd = sFileContent.find("\"", nFind);
            if(nFindEnd != std::string::npos)
                sFileContent.replace(nFind, nFindEnd - nFind, "1.0");
        }
        /*
        std::wstring sRes = htmlToXhtml(sFileContent, bNeedConvert);
        NSFile::CFileBinary oWriter;
        if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
        {
            oWriter.WriteStringUTF8(sRes);
            oWriter.CloseFile();
        }
        */
        return m_oLightReader.FromString(htmlToXhtml(sFileContent, bNeedConvert));
    }

    // Конвертирует mht в xhtml
    bool mhtXhtml(const std::wstring& sSrc)
    {
        NSFile::CFileBinary file;
        if (!file.OpenFile(sSrc))
            return false;

        unsigned char* buffer = new unsigned char[4096];
        if (!buffer)
        {
            file.CloseFile();
            return false;
        }

        DWORD dwReadBytes = 0;
        file.ReadFile(buffer, 4096, dwReadBytes);
        file.CloseFile();
        std::string xml_string = XmlUtils::GetUtf8FromFileContent(buffer, dwReadBytes);

        bool bRes = false;
        if (std::string::npos != xml_string.find("Content-Type: multipart/related"))
        {
            BYTE* pData;
            DWORD nLength;
            if (!NSFile::CFileBinary::ReadAllBytes(sSrc, &pData, nLength))
                return false;

            std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
            RELEASEARRAYOBJECTS(pData);
            /*
            std::wstring sRes = mhtToXhtml(sFileContent);
            NSFile::CFileBinary oWriter;
            if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
            {
                oWriter.WriteStringUTF8(sRes);
                oWriter.CloseFile();
            }
            */
            bRes = m_oLightReader.FromString(mhtToXhtml(sFileContent));
        }
        else
            bRes = htmlXhtml(sSrc);

        RELEASEARRAYOBJECTS(buffer);
        return bRes;
    }

    // Читает стили
    void readStyle()
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();

            if(sName == L"body")
                readStyle2(m_oTree);
            else
            {
                // Стиль по ссылке
                if(sName == L"link")
                {
                    while(m_oLightReader.MoveToNextAttribute())
                    {
                        if(m_oLightReader.GetName() != L"href")
                            continue;
                        std::wstring sRef = m_oLightReader.GetText();
                        if(NSFile::GetFileExtention(sRef) != L"css")
                            continue;
                        std::wstring sFName = NSFile::GetFileName(sRef);
                        // Стиль в сети
                        if(sRef.substr(0, 4) == L"http")
                        {
                            sFName = m_sTmp + L'/' + sFName;
                            NSNetwork::NSFileTransport::CFileDownloader oDownloadStyle(sRef, false);
                            oDownloadStyle.SetFilePath(sFName);
                            if(oDownloadStyle.DownloadSync())
                            {
                                m_oStylesCalculator.AddStylesFromFile(sFName);
                                NSFile::CFileBinary::Remove(sFName);
                            }
                        }
                        else
                        {
                            m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sFName);
                            m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sRef);
                        }
                    }
                    m_oLightReader.MoveToElement();
                }
                // тэг style содержит стили для styles.xml
                else if(sName == L"style")
                    m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());
                else
                    readStyle();
            }
        }
    }

    void readStyle2(NSCSS::CTree& oTree)
    {
        std::wstring sName = m_oLightReader.GetName();
        // Стиль по ссылке
        if(sName == L"link")
        {
            while(m_oLightReader.MoveToNextAttribute())
            {
                if(m_oLightReader.GetName() != L"href")
                    continue;
                std::wstring sRef = m_oLightReader.GetText();
                if(NSFile::GetFileExtention(sRef) != L"css")
                    continue;
                std::wstring sFName = NSFile::GetFileName(sRef);
                // Стиль в сети
                if(sRef.substr(0, 4) == L"http")
                {
                    sFName = m_sTmp + L'/' + sFName;
                    NSNetwork::NSFileTransport::CFileDownloader oDownloadStyle(sRef, false);
                    oDownloadStyle.SetFilePath(sFName);
                    if(oDownloadStyle.DownloadSync())
                    {
                        m_oStylesCalculator.AddStylesFromFile(sFName);
                        NSFile::CFileBinary::Remove(sFName);
                    }
                }
                else
                {
                    m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sFName);
                    m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sRef);
                }
            }
            m_oLightReader.MoveToElement();
        }
        // тэг style содержит стили для styles.xml
        else if(sName == L"style")
            m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());

        oTree.m_oNode.m_sName = sName;
        // Стиль по атрибуту
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sNameA  = m_oLightReader.GetName();
            if(sNameA == L"class")
                oTree.m_oNode.m_sClass  = m_oLightReader.GetText();
            else if(sNameA == L"id")
                oTree.m_oNode.m_sId = m_oLightReader.GetText();
            else if(sNameA == L"style")
                oTree.m_oNode.m_sStyle += m_oLightReader.GetText();
            else
                oTree.m_oNode.m_mAttrs[sNameA] = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            if(!m_oLightReader.IsEmptyNode())
            {
                NSCSS::CTree oChildTree;
                readStyle2(oChildTree);
                oTree.m_arrChild.push_back(oChildTree);
            }
        }
    }

    void PageBreakBefore()
    {
        m_oDocXml.WriteString(L"<w:p><w:pPr><w:pageBreakBefore/></w:pPr></w:p>");
    }

private:

    std::wstring GetSubClass(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors)
    {
        NSCSS::CNode oNode;
        std::wstring sNote;
        oNode.m_sName = m_oLightReader.GetName();
        // Стиль по атрибуту
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName  = m_oLightReader.GetName();
            if(sName == L"class")
                oNode.m_sClass  = m_oLightReader.GetText();
            else if(sName == L"id")
            {
                oNode.m_sId = m_oLightReader.GetText();
                std::wstring sCrossId = std::to_wstring(m_nCrossId++);
                oXml->WriteString(L"<w:bookmarkStart w:id=\"");
                oXml->WriteString(sCrossId);
                oXml->WriteString(L"\" w:name=\"");
                oXml->WriteEncodeXmlString(oNode.m_sId);
                oXml->WriteString(L"\"/><w:bookmarkEnd w:id=\"");
                oXml->WriteString(sCrossId);
                oXml->WriteString(L"\"/>");
            }
            else if(sName == L"style")
                oNode.m_sStyle += m_oLightReader.GetText();
            else if(sName == L"title")
                sNote           = m_oLightReader.GetText();
            else
                oNode.m_mAttrs[sName] = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();
        sSelectors.push_back(oNode);
        return sNote;
    }

    std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bP)
    {
//        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
        bP ? m_oXmlStyle.WritePStyle(oStyle) : m_oXmlStyle.WriteRStyle(oStyle);
        m_oStylesXml.WriteString(m_oXmlStyle.GetStyle());
        return m_oXmlStyle.GetIdAndClear();
    }

    void readHead()
    {
        if(m_oLightReader.IsEmptyNode())
            return;
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            // Базовый адрес
            if(m_oLightReader.GetName() == L"base")
            {
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"href")
                        m_sBase = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();
            }
        }
    }

    void readBody()
    {
        std::vector<NSCSS::CNode> sSelectors;
        GetSubClass(&m_oDocXml, sSelectors);
        /*
        std::wstring sCrossId = std::to_wstring(m_nCrossId++);
        m_oDocXml.WriteString(L"<w:bookmarkStart w:id=\"");
        m_oDocXml.WriteString(sCrossId);
        m_oDocXml.WriteString(L"\" w:name=\"");
        m_oDocXml.WriteString(sFileName);
        m_oDocXml.WriteString(L"\"/><w:bookmarkEnd w:id=\"");
        m_oDocXml.WriteString(sCrossId);
        m_oDocXml.WriteString(L"\"/>");
        */

        readStream(&m_oDocXml, sSelectors, { false, false, -1, L"", L"" });
    }

    void readInside (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& sName)
    {
        if(sName == L"#text")
        {
            std::wstring sText = m_oLightReader.GetText();
            size_t find = sText.find_first_not_of(L" \n\t\r");
            if (find == std::wstring::npos)
                sText = L" ";
            else if(!(find == 1 && sText.front() == L' '))
                sText.erase(0, find);

            std::wstring sPStyle = wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r>");
            std::wstring sRStyle = wrR(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:t xml:space=\"preserve\">");

            std::wstring::iterator end;
            if(oTS.bBdo)
                std::reverse(sText.begin(), sText.end());
            if(oTS.bPre)
            {
                size_t nAfter = sText.find_first_of(L"\n\r");
                while(nAfter != std::wstring::npos)
                {
                    oXml->WriteEncodeXmlString(sText.c_str(), nAfter);
                    oXml->WriteString(L"</w:t></w:r></w:p><w:p>");
                    if(!sPStyle.empty())
                    {
                        oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");
                        oXml->WriteString(sPStyle);
                        oXml->WriteString(L"\"/>");
                        oXml->WriteString(oTS.sPStyle);
                        oXml->WriteString(L"</w:pPr>");
                    }
                    oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
                    oXml->WriteString(sRStyle);
                    oXml->WriteString(L"\"/>");
                    oXml->WriteString(oTS.sRStyle);
                    oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
                    sText.erase(0, nAfter + 1);
                    nAfter = sText.find_first_of(L"\n\r");
                }
                end = sText.end();
            }
            else
                end = std::unique(sText.begin(), sText.end(), [] (wchar_t l, wchar_t r) { return std::iswspace(l) && std::iswspace(r); });

            sText = std::wstring(sText.begin(), end);
            oXml->WriteEncodeXmlString(sText);
            oXml->WriteString(L"</w:t></w:r>");
            return;
        }

        std::wstring sNote = GetSubClass(oXml, sSelectors);

        // Ссылка
        // Область ссылки
        if(sName == L"a" || sName == L"area")
            readA(oXml, sSelectors, oTS, sNote);
        // Полужирный текст
        // Акцентированный текст
        else if(sName == L"b" || sName == L"strong")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:b/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Направление текста
        else if(sName == L"bdo")
        {
            std::wstring sDir;
            while(m_oLightReader.MoveToNextAttribute())
                if(m_oLightReader.GetName() == L"dir")
                    sDir = m_oLightReader.GetText();
            m_oLightReader.MoveToElement();

            CTextSettings oTSBdo(oTS);
            oTSBdo.bBdo = (sDir == L"rtl");
            readStream(oXml, sSelectors, oTSBdo);
        }
        // Отмена направления текста
        else if(sName == L"bdi")
        {
            CTextSettings oTSBdo(oTS);
            oTSBdo.bBdo = false;
            readStream(oXml, sSelectors, oTSBdo);
        }
        // Увеличивает размер шрифта
        else if(sName == L"big")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:sz w:val=\"26\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Перенос строки
        else if(sName == L"br")
        {
            wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r>");
            NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
            if(oStyle.m_pText.GetAlign() == L"both")
                oXml->WriteString(L"<w:tab/>");
            oXml->WriteString(L"<w:br/></w:r>");
        }
        else if(sName == L"center")
        {
            CTextSettings oTSP(oTS);
            oTSP.sPStyle += L"<w:jc w:val=\"center\"/>";
            readStream(oXml, sSelectors, oTSP);
        }
        // Цитата, обычно выделяется курсивом
        // Новый термин, обычно выделяется курсивом
        // Акцентированный текст
        // Курсивный текст
        // Переменная, обычно выделяется курсивом
        else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i" || sName == L"var")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:i/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Код
        // Моноширинный шрифт, например, Consolas
        // Результат скрипта
        else if(sName == L"code" || sName == L"kbd" || sName == L"samp" || sName == L"tt")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Зачеркнутый текст
        else if(sName == L"del" || sName == L"s")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:strike/>";
            readStream(oXml, sSelectors, oTSR);
        }
        else if(sName == L"font")
        {
            while(m_oLightReader.MoveToNextAttribute())
            {
                std::wstring sAName = m_oLightReader.GetName();
                if(sAName == L"color")
                    sSelectors.back().m_sStyle += L"; color: " + m_oLightReader.GetText();
                else if(sAName == L"face")
                    sSelectors.back().m_sStyle += L"; font-family: " + m_oLightReader.GetText();
                else if(sAName == L"size")
                {
                    int nSize = 3;
                    std::wstring sSize = m_oLightReader.GetText();
                    if(!sSize.empty())
                    {
                        if(sSize.front() == L'+')
                            nSize += std::stoi(sSize.substr(1));
                        else if(sSize.front() == L'-')
                            nSize -= std::stoi(sSize.substr(1));
                        else
                            nSize = std::stoi(sSize);
                    }
                    sSize = nSize >= 1 && nSize <= 7 ? std::to_wstring(10 + nSize * 5) : L"22";
                    sSelectors.back().m_sStyle += L"; font-size: " + sSize;
                }
            }
            m_oLightReader.MoveToElement();
            readStream(oXml, sSelectors, oTS);
        }
        // Картинки
        else if(sName == L"img")
            readImage(oXml, sSelectors, oTS);
        // Подчеркнутый
        else if(sName == L"ins" || sName == L"u")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:u w:val=\"single\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Выделенный текст, обычно выделяется желтым
        else if(sName == L"mark")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:highlight w:val=\"yellow\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Цитата, выделенная кавычками, обычно выделяется курсивом
        else if(sName == L"q")
        {
            wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r>");
            std::wstring sRStyle = wrR(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t></w:r>");

            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:i/>";
            readStream(oXml, sSelectors, oTSR);

            wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
            oXml->WriteString(sRStyle);
            oXml->WriteString(L"\"/>");
            oXml->WriteString(oTS.sRStyle);
            oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">&quot;</w:t></w:r>");
        }
        // Текст верхнего регистра
        else if(sName == L"rt" || sName == L"sup")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:vertAlign w:val=\"superscript\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Уменьшает размер шрифта
        else if(sName == L"small")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:sz w:val=\"18\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Текст нижнего регистра
        else if(sName == L"sub")
        {
            CTextSettings oTSR(oTS);
            oTSR.sRStyle += L"<w:vertAlign w:val=\"subscript\"/>";
            readStream(oXml, sSelectors, oTSR);
        }
        // Векторная картинка
        else if(sName == L"svg" || (sName.length() > 3 && sName.compare(sName.length() - 3, 3, L"svg") == 0))
        {
            wrP(oXml, sSelectors, oTS);
            readSVG(oXml);
        }
        else if(sName == L"input")
            readInput(oXml, sSelectors, oTS);
        // Игнорируются тэги выполняющие скрипт
        else if(sName == L"template" || sName == L"canvas" || sName == L"video" || sName == L"math" || sName == L"rp"    ||
                sName == L"command"  || sName == L"iframe" || sName == L"embed" || sName == L"wbr"  || sName == L"audio" ||
                sName == L"bgsound"  || sName == L"applet" || sName == L"blink" || sName == L"keygen"|| sName == L"script" ||
                sName == L"comment"  || sName == L"title"  || sName == L"style")
        {
            sSelectors.pop_back();
            return;
        }
        // Без нового абзаца
        else if(sName == L"basefont" || sName == L"button" || sName == L"label" || sName == L"data" || sName == L"object" ||
                sName == L"noscript" || sName == L"output" || sName == L"abbr"  || sName == L"time" || sName == L"ruby"   ||
                sName == L"progress" || sName == L"hgroup" || sName == L"meter" || sName == L"span" ||
                sName == L"acronym")
            readStream(oXml, sSelectors, oTS);
        // С нового абзаца
        else
        {
            if (m_bInP)
            {
                for (const NSCSS::CNode& item : sSelectors)
                    if (item.m_sName == L"a")
                        oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p>");
                m_bInP = false;
            }

            // Адрес
            if(sName == L"address")
            {
                CTextSettings oTSR(oTS);
                oTSR.sRStyle += L"<w:i/>";
                readStream(oXml, sSelectors, oTSR);
            }
            // Определение термина, отступ от левого края
            else if(sName == L"dd")
            {
                CTextSettings oTSP(oTS);
                oTSP.sPStyle += L"<w:ind w:left=\"567\"/>";
                readStream(oXml, sSelectors, oTSP);
            }
            // С нового абзаца
            else if(sName == L"article" || sName == L"header" || sName == L"div" || sName == L"blockquote" || sName == L"main" ||
                    sName == L"summary" || sName == L"footer" || sName == L"nav" || sName == L"figcaption" || sName == L"form" ||
                    sName == L"details" || sName == L"option" || sName == L"dt"  || sName == L"aside"      || sName == L"p"    ||
                    sName == L"section" || sName == L"figure" || sName == L"dl"  || sName == L"legend"     || sName == L"map"  ||
                    sName == L"dir" ||
                    sName == L"h1" || sName == L"h2" || sName == L"h3" || sName == L"h4" || sName == L"h5" || sName == L"h6")
                readStream(oXml, sSelectors, oTS);
            // Горизонтальная линия
            else if(sName == L"hr")
                oXml->WriteString(L"<w:p><w:pPr><w:pBdr><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr></w:p>");
            // Меню
            // Маркированный список
            else if(sName == L"menu" || sName == L"ul" || sName == L"select" || sName == L"datalist")
                readLi(oXml, sSelectors, oTS, true);
            // Нумерованный список
            else if(sName == L"ol")
                readLi(oXml, sSelectors, oTS, false);
            // Предварительно форматированный текст
            else if(sName == L"pre" || sName == L"xmp")
            {
                CTextSettings oTSPre(oTS);
                oTSPre.bPre = true;
                oTSPre.sRStyle += L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>";
                oTSPre.sPStyle += L"<w:spacing w:after=\"0\"/>";
                readStream(oXml, sSelectors, oTSPre);
            }
            // Таблицы
            else if(sName == L"table")
                readTable(oXml, sSelectors, oTS);
            // Текст с границами
            else if(sName == L"textarea" || sName == L"fieldset")
            {
                CTextSettings oTSP(oTS);
                oTSP.sPStyle += L"<w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr>";
                readStream(oXml, sSelectors, oTSP);
            }
            // Неизвестный тэг. Выделять ли его абзацем?
            else
                readStream(oXml, sSelectors, oTS);
            readNote(oXml, sSelectors, sNote);
            sNote = L"";

            if (m_bInP)
            {
                for (const NSCSS::CNode& item : sSelectors)
                    if (item.m_sName == L"a")
                        oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p>");
                m_bInP = false;
            }
        }
        readNote(oXml, sSelectors, sNote);
        sSelectors.pop_back();
    }

    bool readStream (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        int nDeath = m_oLightReader.GetDepth();
        if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
            return false;

        do
        {
            readInside(oXml, sSelectors, oTS, m_oLightReader.GetName());
        } while(m_oLightReader.ReadNextSiblingNode2(nDeath));
        return true;
    }

    void readTr     (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& sBorders)
    {
        std::vector<CTc> mTable;
        int nDeath = m_oLightReader.GetDepth();
        int i = 1; // Строка

        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            // tr - строки в таблице
            if(m_oLightReader.GetName() != L"tr")
                continue;
            int nTrDeath = m_oLightReader.GetDepth();
            if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode(nTrDeath))
                continue;

            int j = 1; // Столбец
            oXml->WriteString(L"<w:tr>");
            do
            {
                int nColspan = 1;
                int nRowspan = 1;
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"colspan")
                        nColspan = stoi(m_oLightReader.GetText());
                    else if(m_oLightReader.GetName() == L"rowspan")
                        nRowspan = stoi(m_oLightReader.GetText());
                }
                m_oLightReader.MoveToElement();

                // Вставляем ячейки до
                std::vector<CTc>::iterator it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                std::vector<CTc>::iterator it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                while(it1 != mTable.end() || it2 != mTable.end())
                {
                    oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:tcBorders>");
                    oXml->WriteString(!sBorders.empty() ? sBorders : L"<w:left w:val=\"none\" w:color=\"000000\"/><w:top w:val=\"none\" w:color=\"000000\"/><w:right w:val=\"none\" w:color=\"000000\"/><w:bottom w:val=\"none\" w:color=\"000000\"/>");
                    oXml->WriteString(L"</w:tcBorders><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
                    std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
                    oXml->WriteString(sCol);
                    oXml->WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
                    j += stoi(sCol);
                    it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                    it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                }

                GetSubClass(oXml, sSelectors);
                oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:tcBorders>");
                oXml->WriteString(!sBorders.empty() ? sBorders : L"<w:left w:val=\"none\" w:color=\"000000\"/><w:top w:val=\"none\" w:color=\"000000\"/><w:right w:val=\"none\" w:color=\"000000\"/><w:bottom w:val=\"none\" w:color=\"000000\"/>");
                oXml->WriteString(L"</w:tcBorders>");
                if(nRowspan != 1)
                {
                    oXml->WriteString(L"<w:vMerge w:val=\"restart\"/>");
                    std::wstring sColspan = std::to_wstring(nColspan);
                    if(nRowspan == 0)
                        mTable.push_back({0, j, sColspan});
                    else
                        for(int k = i + 1; k < i + nRowspan; k++)
                            mTable.push_back({k, j, sColspan});
                }

                NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle({sSelectors.back()}, true);
                NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle({sSelectors.back()}, false);

                NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

                int nWidth = oStyle.m_pDisplay.GetWidth();
                std::wstring wsType = L"dxa";

                //Если ширина указана в %, то используем тип dxa, если же в других ндтнтцах измерения, то в pct
            #if 1
                // проблема с regex в старом gcc (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52719)
                boost::wregex oWidthRegex(L"((width)+)[\\s]*:[\\s]*(.+%)");
                bool bIsWidthPct = boost::regex_search(sSelectors.back().m_sStyle, oWidthRegex);
            #else
                std::wregex oWidthRegex(L"((width)+)[\\s]*:[\\s]*(.+%)");
                bool bIsWidthPct = std::regex_search(sSelectors.back().m_sStyle.data(), oWidthRegex);
            #endif

                if (bIsWidthPct)
                    wsType = L"pct";
                else
                    nWidth *= 10;
                //-------------------------

                if (nWidth > 0)
                        oXml->WriteString(L"<w:tcW w:w=\"" + std::to_wstring(nWidth) + L"\" w:type=\"" + wsType + L"\"/>");
                else
                        oXml->WriteString(L"<w:tcW w:w=\"0\" w:type=\"auto\"/>");

                if(nColspan != 1)
                {
                        oXml->WriteString(L"<w:gridSpan w:val=\"");
                        oXml->WriteString(std::to_wstring(nColspan));
                        oXml->WriteString(L"\"/>");

                        j += nColspan - 1;
                }

                std::wstring wsVerticalAlign = oStyle.m_pDisplay.GetVerticalAlign();

                if (!wsVerticalAlign.empty())
                        oXml->WriteString(L"<w:vAlign w:val=\"" + wsVerticalAlign + L"\"/>");

                oXml->WriteString(L"<w:hideMark/></w:tcPr>");
                size_t nEmpty = oXml->GetCurSize();
                m_bWasPStyle = false;

                // Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
                if(m_oLightReader.GetName() == L"th")
                {                            
                    CTextSettings oTSR(oTS);
                    oTSR.sRStyle += L"<w:b/>";
                    readStream(oXml, sSelectors, oTSR);
                }
                // Читаем td. Ячейка таблицы. Выравнивание вправо
                else if(m_oLightReader.GetName() == L"td")
                    readStream(oXml, sSelectors, oTS);
                sSelectors.pop_back();
                if (m_bInP)
                {
                    wrP(oXml, sSelectors, oTS);
                    for (const NSCSS::CNode& item : sSelectors)
                        if (item.m_sName == L"a")
                            oXml->WriteString(L"</w:hyperlink>");
                    oXml->WriteString(L"<w:r></w:r></w:p>");
                    m_bInP = false;
                }
                else if (oXml->GetSubData(oXml->GetCurSize() - 6) != L"</w:p>")
                    oXml->WriteString(L"<w:p></w:p>");
                oXml->WriteString(L"</w:tc>");
                j++;

                // Вставляем ячейки после
                it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                while(it1 != mTable.end() || it2 != mTable.end())
                {
                    oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:tcBorders>");
                    oXml->WriteString(!sBorders.empty() ? sBorders : L"<w:left w:val=\"none\" w:color=\"000000\"/><w:top w:val=\"none\" w:color=\"000000\"/><w:right w:val=\"none\" w:color=\"000000\"/><w:bottom w:val=\"none\" w:color=\"000000\"/>");
                    oXml->WriteString(L"</w:tcBorders><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
                    std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
                    oXml->WriteString(sCol);
                    oXml->WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
                    j += stoi(sCol);
                    it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                    it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                }
            } while(m_oLightReader.ReadNextSiblingNode(nTrDeath));
            oXml->WriteString(L"</w:tr>");
            i++;
        }
    }

    void readTable  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        NSStringUtils::CStringBuilder oHead;
        NSStringUtils::CStringBuilder oBody;
        NSStringUtils::CStringBuilder oFoot;

        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, false);

        if (oXml->GetSubData(oXml->GetCurSize() - 6) != L"</w:p>")
            oXml->WriteString(L"<w:p><w:pPr><w:spacing w:beforeLines=\"0\" w:before=\"0\" w:afterLines=\"0\" w:after=\"0\"/><w:rPr><w:vanish/><w:sz w:val=\"2\"/><w:szCs w:val=\"2\"/></w:rPr></w:pPr></w:p>");

        // Начало таблицы
        std::wstring wsTable = L"<w:tbl><w:tblPr>";

        int nWidth = oStyle.m_pDisplay.GetWidth();
        std::wstring wsAlign = oStyle.m_pDisplay.GetAlign();

        if (0 < nWidth)
                wsTable += L"<w:tblW w:w=\"" + std::to_wstring(nWidth) + L"\" w:type=\"pct\"/>";
        else if (m_oStylesCalculator.GetSizeDeviceWindow().m_ushWidth != 0)
                wsTable += L"<w:tblW w:w=\"" + std::to_wstring(m_oStylesCalculator.GetSizeDeviceWindow().m_ushWidth) + L"\" w:type=\"pct\"/>";
        else
                wsTable += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

        if (wsAlign.empty())
        {
                NSCSS::CNode oLastNode = sSelectors.back();
                sSelectors.pop_back();

                NSCSS::CCompiledStyle oTempSettingsStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);

                wsAlign = oTempSettingsStyle.m_pText.GetAlign();

                if (wsAlign.empty())
                {
                        NSCSS::CCompiledStyle oTempStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, false);

                        wsAlign = oTempStyle.m_pText.GetAlign();
                }

                sSelectors.push_back(oLastNode);
        }

        if (!oStyle.m_pMargin.Empty() && (0 < oStyle.m_pMargin.GetTopSide() || 0 < oStyle.m_pMargin.GetBottomSide()))
        {
                wsTable += L"<w:tblCellMar>";

                if (0 < oStyle.m_pMargin.GetTopSide())
                        wsTable += L"<w:top w:w=\"" + std::to_wstring(static_cast<short int>(oStyle.m_pMargin.GetTopSide() * 10 + 0.5f)) + L"\" w:type=\"dxa\"/>";

//                if (0 < oStyle.m_pMargin.GetLeftSide())
//                        wsTable += L"<w:left w:w=\"" + std::to_wstring(static_cast<short int>(oStyle.m_pMargin.GetLeftSide() * 10 + 0.5f)) + L"\" w:type=\"dxa\"/>";

                if (0 < oStyle.m_pMargin.GetBottomSide())
                        wsTable += L"<w:bottom w:w=\"" + std::to_wstring(static_cast<short int>(oStyle.m_pMargin.GetBottomSide() * 10 + 0.5f)) + L"\" w:type=\"dxa\"/>";

//                if (0 < oStyle.m_pMargin.GetRightSide())
//                        wsTable += L"<w:right w:w=\"" + std::to_wstring(static_cast<short int>(oStyle.m_pMargin.GetRightSide() * 10 + 0.5f)) + L"\" w:type=\"dxa\"/>";

                wsTable += L"</w:tblCellMar>";
        }

        if (!wsAlign.empty())
                wsTable += L"<w:jc w:val=\"" + wsAlign + L"\"/>";

        wsTable += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
        wsTable += L"</w:tblPr>";

        // borders
        std::wstring sBorders;
        oStyle.m_pBorder.Unlock();
        if (oStyle.m_pBorder.Empty())
        {
            sBorders = L"<w:left w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:top w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:right w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:bottom w:val=\"none\" w:color=\"auto\" w:sz=\"4\" w:space=\"0\"/>";
        }
        else
        {
            if (oStyle.m_pBorder.EqualSides())
            {
                std::wstring sColor = oStyle.m_pBorder.GetColorBottomSide();
                std::wstring sSz    = oStyle.m_pBorder.GetWidthBottomSideW();
                std::wstring sStyle = oStyle.m_pBorder.GetStyleBottomSide();

                sBorders =  L"<w:top w:val=\""      + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>" +
                            L"<w:left w:val=\""     + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>" +
                            L"<w:bottom w:val=\""   + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>" +
                            L"<w:right w:val=\""    + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>" +
                            L"<w:insideH w:val=\""  + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>" +
                            L"<w:insideV w:val=\""  + sStyle + L"\" w:color=\"" + sColor + L"\" w:sz=\"" + sSz + L"\" w:space=\"0\"/>";
            }
            else
            {
                    wsTable.insert(35, oStyle.GetId());

                    std::wstring sColorLeftSide     = oStyle.m_pBorder.GetColorLeftSide();
                    std::wstring sSzLeftSide        = oStyle.m_pBorder.GetWidthLeftSideW();
                    std::wstring sStyleLeftSide     = oStyle.m_pBorder.GetStyleLeftSide();
                    std::wstring sColorTopSide      = oStyle.m_pBorder.GetColorTopSide();
                    std::wstring sSzTopSide         = oStyle.m_pBorder.GetWidthTopSideW();
                    std::wstring sStyleTopSide      = oStyle.m_pBorder.GetStyleLeftSide();
                    std::wstring sColorRightSide    = oStyle.m_pBorder.GetColorRightSide();
                    std::wstring sSzRightSide       = oStyle.m_pBorder.GetWidthRightSideW();
                    std::wstring sStyleRightSide    = oStyle.m_pBorder.GetStyleLeftSide();
                    std::wstring sColorBottomSide   = oStyle.m_pBorder.GetColorBottomSide();
                    std::wstring sSzBottomSide      = oStyle.m_pBorder.GetWidthBottomSideW();
                    std::wstring sStyleBottomSide   = oStyle.m_pBorder.GetStyleLeftSide();

                    sBorders =  L"<w:left w:val=\""     + sStyleLeftSide    + L"\" w:color=\"" + sColorLeftSide    + L"\" w:sz=\"" + sSzLeftSide   + L"\" w:space=\"0\"/>" +
                                L"<w:top w:val=\""      + sStyleTopSide     + L"\" w:color=\"" + sColorTopSide     + L"\" w:sz=\"" + sSzTopSide    + L"\" w:space=\"0\"/>" +
                                L"<w:right w:val=\""    + sStyleRightSide   + L"\" w:color=\"" + sColorRightSide   + L"\" w:sz=\"" + sSzRightSide  + L"\" w:space=\"0\"/>" +
                                L"<w:bottom w:val=\""   + sStyleBottomSide  + L"\" w:color=\"" + sColorBottomSide  + L"\" w:sz=\"" + sSzBottomSide + L"\" w:space=\"0\"/>";

            }
        }

        oXml->WriteString(wsTable);

        /*
        NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
        oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
        NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);
        m_oXmlStyle.WriteLitePStyle(oStyleSetting);
        std::wstring sPSettings = m_oXmlStyle.GetStyle();
        m_oXmlStyle.Clear();
        size_t nBdr = sPSettings.find(L"<w:pBdr>");
        if (nBdr != std::wstring::npos)
        {
            nBdr += 8;
            size_t nBdrEnd = sPSettings.find(L"</w:pBdr>", nBdr);
            if (nBdrEnd != std::wstring::npos)
            {
                sBorders = sPSettings.substr(nBdr, nBdrEnd - nBdr);
                size_t nSpace = sBorders.find(L"w:space=\"");
                while (nSpace != std::wstring::npos)
                {
                    nSpace += 9;
                    size_t nSpaceEnd = sBorders.find(L'\"', nSpace);
                    sBorders.replace(nSpace, nSpaceEnd - nSpace, L"0");
                    nSpace = sBorders.find(L"w:space=\"", nSpace);
                }
            }
        }
        */

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            GetSubClass(oXml, sSelectors);
            // Заголовок таблицы
            if(sName == L"caption")
            {
                size_t nHyp = 0;
                if (!m_bInP)
                {
                    oXml->WriteString(L"<w:p>");
                    for (const NSCSS::CNode& item : sSelectors)
                    {
                        if (item.m_sName == L"a")
                        {
                            oXml->WriteString(L"<w:hyperlink>");
                            nHyp++;
                        }
                    }
                    m_bInP = true;
                    m_bWasPStyle = false;
                }
                // Заголовок таблицы выравнивание посередине
                CTextSettings oTSP { oTS.bBdo, oTS.bPre, oTS.nLi, oTS.sRStyle, oTS.sPStyle + L"<w:jc w:val=\"center\"/>" };
                readStream(oXml, sSelectors, oTSP);
                if (m_bInP)
                {
                    for (size_t i = 0; i < nHyp; i++)
                        oXml->WriteString(L"</w:hyperlink>");
                    oXml->WriteString(L"</w:p>");
                    m_bInP = false;
                    m_bWasPStyle = false;
                }
            }
            if(sName == L"thead")
                readTr(&oHead, sSelectors, oTS, sBorders);
            else if(sName == L"tbody")
                readTr(&oBody, sSelectors, oTS, sBorders);
            else if(sName == L"tfoot")
                readTr(&oFoot, sSelectors, oTS, sBorders);
            sSelectors.pop_back();
        }

        // Конец таблицы
        oXml->WriteString(oHead.GetData());
        oXml->WriteString(oBody.GetData());
        oXml->WriteString(oFoot.GetData());
        oXml->WriteString(L"</w:tbl>");
    }

    void readInput  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        std::wstring sValue;
        std::wstring sAlt;
        std::wstring sType;
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"value")
                sValue = m_oLightReader.GetText();
            else if(sName == L"alt")
                sAlt = m_oLightReader.GetText();
            else if(sName == L"type")
                sType = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();
        if(sType == L"hidden")
            return;
        if(sValue.empty())
            sValue = sAlt;
        if(!sValue.empty())
        {
            wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r>");
            wrR(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:t xml:space=\"preserve\">");
            oXml->WriteEncodeXmlString(sValue + L' ');
            oXml->WriteString(L"</w:t></w:r>");
        }
        readStream(oXml, sSelectors, oTS);
    }

    void readLi     (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, bool bType)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        std::wstring sStart = L"1";
        while(m_oLightReader.MoveToNextAttribute())
            if(m_oLightReader.GetName() == L"start")
                sStart = m_oLightReader.GetText();
        m_oLightReader.MoveToElement();

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            GetSubClass(oXml, sSelectors);
            if(sName == L"optgroup")
            {
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() != L"label")
                        continue;
                    if (m_bInP)
                    {
                        for (const NSCSS::CNode& item : sSelectors)
                            if (item.m_sName == L"a")
                                oXml->WriteString(L"</w:hyperlink>");
                        oXml->WriteString(L"</w:p>");
                        m_bInP = false;
                    }
                    wrP(oXml, sSelectors, oTS);
                    oXml->WriteString(L"<w:r>");
                    wrR(oXml, sSelectors, oTS);
                    oXml->WriteString(L"<w:t xml:space=\"preserve\">");
                    oXml->WriteEncodeXmlString(m_oLightReader.GetText());
                    oXml->WriteString(L"</w:t></w:r>");
                }
                m_oLightReader.MoveToElement();
                readLi(oXml, sSelectors, oTS, true);
                continue;
            }
            if(sName != L"li" && sName != L"option")
            {
                readInside(oXml, sSelectors, oTS, sName);
                sSelectors.pop_back();
                continue;
            }

            while(m_oLightReader.MoveToNextAttribute())
                if(m_oLightReader.GetName() == L"value")
                    sStart = m_oLightReader.GetText();
            m_oLightReader.MoveToElement();

            if (m_bInP)
            {
                for (const NSCSS::CNode& item : sSelectors)
                    if (item.m_sName == L"a")
                        oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p>");
                m_bInP = false;
            }
            CTextSettings oTSLiP(oTS);
            oTSLiP.nLi++;
            oTSLiP.sPStyle += L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(oTSLiP.nLi) + L"\"/><w:numId w:val=\"" +
                    (bType ? L"1" : std::to_wstring(m_nNumberingId + 1)) + L"\"/></w:numPr>";
            readStream(oXml, sSelectors, oTSLiP);
            if (m_bInP)
            {
                for (const NSCSS::CNode& item : sSelectors)
                    if (item.m_sName == L"a")
                        oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p>");
                m_bInP = false;
            }
            sSelectors.pop_back();
        }
        // Нумерованный список
        if(!bType)
        {
            m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
            m_oNumberXml.WriteString(std::to_wstring(m_nNumberingId++));
            m_oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"");
            m_oNumberXml.WriteString(sStart);
            m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");
        }
    }

    void readA      (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, std::wstring& sNote)
    {
        std::wstring sRef;
        std::wstring sAlt;
        bool bCross = false;
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"href")
            {
                sRef = m_oLightReader.GetText();
                if(sRef.find('#') != std::wstring::npos)
                    bCross = true;
            }
            else if(sName == L"name")
            {
                std::wstring sCrossId = std::to_wstring(m_nCrossId++);
                oXml->WriteString(L"<w:bookmarkStart w:id=\"");
                oXml->WriteString(sCrossId);
                oXml->WriteString(L"\" w:name=\"");
                oXml->WriteString(m_oLightReader.GetText());
                oXml->WriteString(L"\"/><w:bookmarkEnd w:id=\"");
                oXml->WriteString(sCrossId);
                oXml->WriteString(L"\"/>");
            }
            else if(sName == L"alt")
                sAlt = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();
        if(sNote.empty())
            sNote = sRef;

        if (!m_bInP)
        {
            oXml->WriteString(L"<w:p>");
            for (size_t i = 0; i < sSelectors.size() - 1; i++)
                if (sSelectors[i].m_sName == L"a")
                    oXml->WriteString(L"<w:hyperlink>");
            m_bInP = true;
            m_bWasPStyle = false;
        }
        wrP(oXml, sSelectors, oTS);
        // Перекрестная ссылка внутри файла
        if(bCross)
        {
            oXml->WriteString(L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"");
            size_t nSharp = sRef.find('#');
            if(nSharp == std::wstring::npos)
                oXml->WriteString(NSFile::GetFileName(sRef));
            else
                oXml->WriteEncodeXmlString(sRef.c_str() + nSharp + 1);
        }
        // Внешняя ссылка
        else
        {
            // Пишем рельсы
            m_oDocXmlRels.WriteString(L"<Relationship Id=\"rHyp");
            m_oDocXmlRels.WriteString(std::to_wstring(m_nHyperlinkId));
            m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
            m_oDocXmlRels.WriteEncodeXmlString(sRef);
            m_oDocXmlRels.WriteString(L"\" TargetMode=\"External\"/>");

            // Пишем в document.xml
            oXml->WriteString(L"<w:hyperlink w:tooltip=\"");
            oXml->WriteEncodeXmlString(sNote);
            oXml->WriteString(L"\" r:id=\"rHyp");
            oXml->WriteString(std::to_wstring(m_nHyperlinkId++));
        }
        oXml->WriteString(L"\">");

        if(!readStream(oXml, sSelectors, oTS))
        {
            oXml->WriteString(L"<w:r>");
            wrR(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:t xml:space=\"preserve\">");
            oXml->WriteEncodeXmlString(sAlt);
            oXml->WriteString(L"</w:t></w:r>");
        }
        if (m_bInP)
            oXml->WriteString(L"</w:hyperlink>");
        sNote = L"";
    }

    void readImage  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        std::wstring sAlt = L"";
        bool bRes = false;
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"alt")
            {
                sAlt = m_oLightReader.GetText();
                continue;
            }
            else if(m_oLightReader.GetName() != L"src")
                continue;

            std::wstring sSrcM = m_oLightReader.GetText();
            std::wstring sImageName;
            std::wstring sImageId = std::to_wstring(m_nImageId);
            // Картинка Base64
            if(sSrcM.substr(0, 4) == L"data")
            {
                size_t nBase = sSrcM.find(L"/", 4);
                if(nBase == std::wstring::npos)
                    continue;
                nBase++;
                size_t nEndBase = sSrcM.find(L";", nBase);
                if(nEndBase == std::wstring::npos)
                    continue;
                std::wstring sType = sSrcM.substr(nBase, nEndBase - nBase);
                if(sType == L"octet-stream")
                    sType = L"jpg";
                sImageName = sImageId + L"." + sType;
                NSFile::CFileBinary oImageWriter;
                if(oImageWriter.CreateFileW(m_sDst + L"/word/media/i" + sImageName))
                {

                    nBase = sSrcM.find(L"base64", 4);
                    if(nBase == std::wstring::npos)
                        continue;
                    std::string sBase64 = m_oLightReader.GetTextA().substr(nBase + 7);
                    int nSrcLen = (int)sBase64.length();
                    int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
                    if(nDecodeLen == 0)
                        continue;
                    BYTE* pImageData = new BYTE[nDecodeLen];
                    if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                    {
                        oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
                        bRes = true;
                    }
                    RELEASEARRAYOBJECTS(pImageData);
                    oImageWriter.CloseFile();
                }
            }
            // Картинка в сети
            else if(sSrcM.substr(0, 4) == L"http" || !m_sBase.empty())
            {
                std::wstring sExtention = NSFile::GetFileExtention(sSrcM);
                std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
                if(sExtention != L"bmp" && sExtention != L"svg" && sExtention != L"jfif" && sExtention != L"wmf" && sExtention != L"gif" &&
                   sExtention != L"jpe" && sExtention != L"png" && sExtention != L"jpeg" && sExtention != L"jpg" )
                    continue;

                sImageName = NSFile::GetFileName(sSrcM);
                sImageName.erase(std::remove_if(sImageName.begin(), sImageName.end(), [] (wchar_t ch) { return std::iswspace(ch) || (ch == L'^'); }), sImageName.end());
                NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
                oDownloadImg.SetFilePath(m_sDst + L"/word/media/i" + sImageName);
                bRes = oDownloadImg.DownloadSync();
            }
            // Картинка по относительному пути
            else
            {
                sImageName = NSFile::GetFileName(sSrcM);
                sImageName.erase(std::remove_if(sImageName.begin(), sImageName.end(), [] (wchar_t ch) { return std::iswspace(ch) || (ch == L'^'); }), sImageName.end());

                std::wstring sExtention = NSFile::GetFileExtention(sSrcM);
                std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
                if(sExtention != L"bmp" && sExtention != L"svg" && sExtention != L"jfif" && sExtention != L"wmf" && sExtention != L"gif" &&
                   sExtention != L"jpe" && sExtention != L"png" && sExtention != L"jpeg" && sExtention != L"jpg" )
                    continue;

                bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + sSrcM, m_sDst + L"/word/media/i" + sImageName);
                if(!bRes)
                    bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + NSFile::GetFileName(sSrcM), m_sDst + L"/word/media/i" + sImageName);
                if(!bRes)
                    bRes = NSFile::CFileBinary::Copy(sSrcM, m_sDst + L"/word/media/i" + sImageName);
            }
            if(bRes)
            {
                wrP(oXml, sSelectors, oTS);
                bRes = ImageRels(oXml, sImageId, L"i" + sImageName);
            }
        }
        m_oLightReader.MoveToElement();

        if(!bRes)
        {
            wrP(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:r>");
            wrR(oXml, sSelectors, oTS);
            oXml->WriteString(L"<w:t xml:space=\"preserve\">");
            oXml->WriteEncodeXmlString(sAlt);
            oXml->WriteString(L"</w:t></w:r>");
        }
    }

    std::wstring wrP(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        if (!m_bInP)
        {
            oXml->WriteString(L"<w:p>");
            for (const NSCSS::CNode& item : sSelectors)
                if (item.m_sName == L"a")
                    oXml->WriteString(L"<w:hyperlink>");
            m_bInP = true;
            m_bWasPStyle = false;
        }
        if (m_bWasPStyle)
            return L"";
        oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");
        
        std::vector<std::pair<size_t, NSCSS::CNode>> temporary;
        size_t i = 0;
        while(i != sSelectors.size())
        {
            if(rStyle.find(L' ' + sSelectors[i].m_sName + L' ') != std::wstring::npos &&
               sSelectors[i].m_sClass.empty() && sSelectors[i].m_sId.empty() &&
               sSelectors[i].m_sStyle.empty() && sSelectors[i].m_mAttrs.empty())
            {
                temporary.push_back(std::make_pair(i, sSelectors[i]));
                sSelectors.erase(sSelectors.begin() + i);
            }
            else
                i++;
        }
        NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);

        NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

        std::wstring sPStyle = GetStyle(oStyle, true);

        m_oXmlStyle.WriteLitePStyle(oStyleSetting);
        std::wstring sPSettings = m_oXmlStyle.GetStyle();
        m_oXmlStyle.Clear();

        for(int i = temporary.size() - 1; i >= 0; i--)
            sSelectors.insert(sSelectors.begin() + temporary[i].first, temporary[i].second);

        // Если в таблице, то игнориуются Paragraph Borders
        bool bInTable = false;
        for (const NSCSS::CNode& item : sSelectors)
            if (item.m_sName == L"table")
                bInTable = true;
        if (bInTable)
        {
            size_t nBdr = sPSettings.find(L"<w:pBdr>");
            if (nBdr != std::wstring::npos)
            {
                size_t nBdrEnd = sPSettings.find(L"</w:pBdr>", nBdr);
                if (nBdrEnd != std::wstring::npos)
                    sPSettings.erase(nBdr, nBdrEnd + 9 - nBdr);
            }
        }

        oXml->WriteString(sPStyle);
        oXml->WriteString(L"\"/>");
        oXml->WriteString(oTS.sPStyle + L' ' + sPSettings);
        oXml->WriteString(L"</w:pPr>");
        m_bWasPStyle = true;
        return sPStyle;
    }

    std::wstring wrR(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
    {
        if (!m_bInP)
            return L"";

        NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);

        NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

        std::wstring sRStyle = GetStyle(oStyle, false);

        m_oXmlStyle.WriteLiteRStyle(oStyleSetting);
        const std::wstring sRSettings = m_oXmlStyle.GetStyle();
        m_oXmlStyle.Clear();

        oXml->WriteString(L"<w:rPr><w:rStyle w:val=\"");
        oXml->WriteString(sRStyle);
        oXml->WriteString(L"\"/>");

        oXml->WriteString(oTS.sRStyle + L' ' + sRSettings);
        oXml->WriteString(L"</w:rPr>");
        return sRStyle;
    }

    bool ImageRels  (NSStringUtils::CStringBuilder* oXml, const std::wstring& sImageId, const std::wstring& sImageName)
    {
        CBgraFrame oBgraFrame;
        if(!oBgraFrame.OpenFile(m_sDst + L"/word/media/" + sImageName))
            return false;

        m_nImageId++;
        // Прописать рельсы
        m_oDocXmlRels.WriteString(L"<Relationship Id=\"rPic");
        m_oDocXmlRels.WriteString(sImageId);
        m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/");
        m_oDocXmlRels.WriteString(sImageName);
        m_oDocXmlRels.WriteString(L"\"/>");

        // Получаем размеры картинки
        int nHy = oBgraFrame.get_Height();
        int nWx = oBgraFrame.get_Width();
        if(nWx > nHy)
        {
            int nW = nWx * 9525;
            nW = (nW > 7000000 ? 7000000 : nW);
            nHy = (int)((double)nHy * (double)nW / (double)nWx);
            nWx = nW;
        }
        else
        {
            int nH = nHy * 9525;
            nH = (nH > 8000000 ? 8000000 : nH);
            int nW = (int)((double)nWx * (double)nH / (double)nHy);
            if(nW > 7000000)
            {
                nW = 7000000;
                nHy = (int)((double)nHy * (double)nW / (double)nWx);
            }
            else
                nHy = nH;
            nWx = nW;
        }

        // Пишем в document.xml
        oXml->WriteString(L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"");
        oXml->WriteString(std::to_wstring(nWx));
        oXml->WriteString(L"\" cy=\"");
        oXml->WriteString(std::to_wstring(nHy));
        oXml->WriteString(L"\"/><wp:docPr id=\"");
        oXml->WriteString(sImageId);
        oXml->WriteString(L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"");
        oXml->WriteString(sImageId);
        oXml->WriteString(L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic");
        oXml->WriteString(sImageId);
        oXml->WriteString(L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"");
        oXml->WriteString(std::to_wstring(nWx));
        oXml->WriteString(L"\" cy=\"");
        oXml->WriteString(std::to_wstring(nHy));
        oXml->WriteString(L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>");
        return true;
    }

    void readNote   (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const std::wstring& sNote)
    {
        if(sNote.empty())
            return;
        if (!m_bInP)
        {
            oXml->WriteString(L"<w:p>");
            for (const NSCSS::CNode& item : sSelectors)
                if (item.m_sName == L"a")
                    oXml->WriteString(L"<w:hyperlink>");
            m_bInP = true;
            m_bWasPStyle = false;
        }
        oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
        oXml->WriteString(std::to_wstring(m_nFootnoteId));
        oXml->WriteString(L"\"/></w:r>");
        m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
        m_oNoteXml.WriteString(std::to_wstring(m_nFootnoteId++));
        m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
        m_oNoteXml.WriteEncodeXmlString(sNote);
        m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");
    }

    void readSVG    (NSStringUtils::CStringBuilder* oXml)
    {
        // Сохранить как .svg картинку
        NSStringUtils::CStringBuilder oSVG;
        oSVG.WriteString(L"<svg ");
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName.find(L"xmlns") != std::wstring::npos)
                continue;
            oSVG.WriteString(sName);
            oSVG.WriteString(L"=\"");
            oSVG.WriteString(m_oLightReader.GetText());
            oSVG.WriteString(L"\" ");
        }
        m_oLightReader.MoveToElement();
        oSVG.WriteString(L"xmlns=\"http://www.w3.org/2000/svg\">");

        std::wstring sSVG = m_oLightReader.GetInnerXml();
        size_t nRef = sSVG.find(L"image");
        while(nRef != std::wstring::npos)
        {
            size_t nRefBegin = sSVG.rfind(L'<', nRef);
            if (nRefBegin != std::wstring::npos)
            {
                if (sSVG[nRefBegin + 1] == L'/')
                    nRefBegin++;
                sSVG.erase(nRefBegin + 1, nRef - nRefBegin - 1);
                nRef = nRefBegin + 1;
            }

            size_t nRefEnd = sSVG.find(L'>', nRef);
            size_t nHRef = sSVG.find(L"href", nRef);
            if(nHRef == std::wstring::npos || nRefEnd == std::wstring::npos)
                break;
            nHRef += 6;
            if(nHRef > nRefEnd || sSVG.compare(nHRef, 4, L"http") == 0)
            {
                nRef = sSVG.find(L"image", nRef + 5);
                continue;
            }
            size_t nHRefLen = sSVG.find(L"\"", nHRef);
            if(nHRefLen == std::wstring::npos)
                break;
            std::wstring sImageName = sSVG.substr(nHRef, nHRefLen - nHRef);
            std::wstring sTIN(sImageName);
            sTIN.erase(std::remove_if(sTIN.begin(), sTIN.end(), [] (wchar_t ch) { return std::iswspace(ch) || (ch == L'^'); }), sTIN.end());
            sTIN = NSFile::GetFileName(sTIN);
            bool bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + sImageName, m_sDst + L"/word/media/" + sTIN);
            if(!bRes)
                bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + NSFile::GetFileName(sImageName), m_sDst + L"/word/media/" + sTIN);
            if(bRes)
                sSVG.replace(nHRef, nHRefLen - nHRef, sTIN);
            nRef = sSVG.find(L"image", nRef + 5);
        }

        oSVG.WriteString(sSVG);
        oSVG.WriteString(L"</svg>");

        std::wstring sImageId = std::to_wstring(m_nImageId);
        NSFile::CFileBinary oSVGWriter;
        std::wstring sImageFile = m_sDst + L"/word/media/" + sImageId + L".svg";
        if (oSVGWriter.CreateFileW(sImageFile))
        {
            oSVGWriter.WriteStringUTF8(oSVG.GetData());
            oSVGWriter.CloseFile();
        }

        // Конвертация из svg в png
        NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
        MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
        bool bLoad = pMetafile->LoadFromFile(sImageFile.data());
        if(bLoad)
        {
            std::wstring sPngFile = m_sDst + L"/word/media/" + sImageId + L".png";
            pMetafile->ConvertToRaster(sPngFile.data(), 4, 1000);
        }
        pMetafile->Release();
        pFonts->Release();

        ImageRels(oXml, sImageId, sImageId + L".png");
    }
};

CHtmlFile2::CHtmlFile2()
{
    m_internal = new CHtmlFile2_Private();
}

CHtmlFile2::~CHtmlFile2()
{
    RELEASEOBJECT(m_internal);
}

bool CHtmlFile2::IsHtmlFile(const std::wstring& sFile)
{
    // Конвертируем в xhtml
    if(!m_internal->htmlXhtml(sFile))
        return false;
    // Читаем html
    return m_internal->isHtml();
}

bool CHtmlFile2::IsMhtFile(const std::wstring& sFile)
{
    // Конвертируем в xhtml
    if(!m_internal->mhtXhtml(sFile))
        return false;
    // Читаем html
    return m_internal->isHtml();
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmp = sFolder;
}

HRESULT CHtmlFile2::OpenHtml(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    if(!m_internal->m_oLightReader.IsValid())
        if(!IsHtmlFile(sSrc))
            return S_FALSE;

    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    m_internal->readStyle();

    m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

    // Переходим в начало
    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    if(oParams && oParams->m_bNeedPageBreakBefore)
        m_internal->PageBreakBefore();
    m_internal->readSrc();
    m_internal->write();
    return S_OK;
}

HRESULT CHtmlFile2::OpenMht(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    if(!m_internal->m_oLightReader.IsValid())
        if(!IsMhtFile(sSrc))
            return S_FALSE;

    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    m_internal->readStyle();

    m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

    // Переходим в начало
    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    if(oParams && oParams->m_bNeedPageBreakBefore)
        m_internal->PageBreakBefore();
    m_internal->readSrc();
    m_internal->write();
    return S_OK;
}

HRESULT CHtmlFile2::OpenBatchHtml(const std::vector<std::wstring>& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    bool bFirst = true;

    for(const std::wstring& sS : sSrc)
    {
        #ifdef _DEBUG
        std::wcout << NSFile::GetFileName(sS) << std::endl;
        #endif

        m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sS);
        if(!IsHtmlFile(sS))
            continue;
        m_internal->readStyle();

        m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

        // Переходим в начало
        if(m_internal->m_oLightReader.MoveToStart())
        {
            if(oParams && oParams->m_bNeedPageBreakBefore && !bFirst)
                m_internal->PageBreakBefore();
            bFirst = false;
            m_internal->readSrc();
            m_internal->m_oLightReader.Clear();
            m_internal->m_sBase.clear();
        }
        m_internal->m_oStylesCalculator.Clear();
    }

    m_internal->write();
    return S_OK;
}

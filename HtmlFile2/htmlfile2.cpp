#include <string>
#include <cwctype>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "htmlfile2.h"
#include "../Common/3dParty/html/htmltoxhtml.h"
#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../Common/FileDownloader/FileDownloader.h"
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

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

// Ячейка таблицы
struct CTc
{
    int i;
    int j;
    std::wstring sGridSpan = L"1";

    CTc(int _i, int _j, std::wstring sColspan)
    {
        i = _i;
        j = _j;
        sGridSpan = sColspan;
    }
};

// Настройки текста
struct CTextSettings
{
    bool bBdo; // Реверс текста
    bool bPre; // Сохранение форматирования (Сохранение пробелов, табуляций, переносов строк)
    int  nLi;  // Уровень списка

    CTextSettings(bool _bBdo, bool _bPre, int _nLi)
    {
        bBdo = _bBdo;
        bPre = _bPre;
        nLi  = _nLi;
    }
};

bool operator==(const CTc& c1, const CTc& c2)
{
    return (c1.i == c2.i && c1.j == c2.j && c1.sGridSpan == c2.sGridSpan);
}

class CHtmlFile2_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
    NSCSS::CCssCalculator m_oStylesCalculator; // Css калькулятор
    NSCSS::CDocumentStyle m_oXmlStyle;         // Ooxml стиль

    std::wstring m_sTmp;  // Temp папка для конфертации html в xhtml
    std::wstring m_sSrc;  // Директория источника
    std::wstring m_sDst;  // Директория назначения
    std::wstring m_sBase; // Полный базовый адрес

    std::map<std::wstring, std::vector<std::wstring>> m_sSrcs; // Имена обрабатываемых файлов (имя файла, имя перекрестной ссылки)

private:
    int m_nImageId;     // ID картинки
    int m_nFootnoteId;  // ID сноски
    int m_nHyperlinkId; // ID ссылки
    int m_nStyleId;     // ID стиля
    int m_nCrossId;     // ID перекрестной ссылки
    int m_nNumberingId; // ID списка

    NSStringUtils::CStringBuilder m_oStylesXml;  // styles.xml
    NSStringUtils::CStringBuilder m_oDocXmlRels; // document.xml.rels
    NSStringUtils::CStringBuilder m_oDocXml;     // document.xml
    NSStringUtils::CStringBuilder m_oNoteXml;    // footnotes.xml
    NSStringUtils::CStringBuilder m_oNumberXml;  // numbering.xml

public:

    CHtmlFile2_Private()
    {
        m_nImageId     = 1;
        m_nFootnoteId  = 1;
        m_nHyperlinkId = 1;
        m_nStyleId     = 1;
        m_nCrossId     = 1;
        m_nNumberingId = 1;
    }

    ~CHtmlFile2_Private()
    {
        m_oLightReader     .Clear();
        m_oStylesCalculator.Clear();
        m_oXmlStyle        .Clear();
        m_sSrcs            .clear();
        m_oStylesXml       .Clear();
        m_oDocXmlRels      .Clear();
        m_oDocXml          .Clear();
        m_oNoteXml         .Clear();
        m_oNumberXml       .Clear();
    }

    // Проверяет наличие тэга html
    bool isHtml()
    {
        if(!m_oLightReader.ReadNextNode())
            return false;
        if(m_oLightReader.GetName() != L"html")
            return false;
        return true;
    }

    void CreateDocxEmpty(CHtmlParams* oParams)
    {
        // Создаем пустые папки
        std::wstring strDirectory = m_sDst;
        // rels
        std::wstring pathRels = strDirectory + L"/_rels";
        NSDirectory::CreateDirectory(pathRels);
        // docProps
        std::wstring pathDocProps = strDirectory + L"/docProps";
        NSDirectory::CreateDirectory(pathDocProps);
        // word
        std::wstring pathWord = strDirectory + L"/word";
        NSDirectory::CreateDirectory(pathWord);
        // documentRels
        std::wstring pathWordRels = pathWord + L"/_rels";
        NSDirectory::CreateDirectory(pathWordRels);
        // media
        std::wstring pathMedia = pathWord + L"/media";
        NSDirectory::CreateDirectory(pathMedia);
        // theme
        std::wstring pathTheme = pathWord + L"/theme";
        NSDirectory::CreateDirectory(pathTheme);

        // theme1.xml
        std::wstring sTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:majorFont><a:minorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
        NSFile::CFileBinary oThemeWriter;
        if (oThemeWriter.CreateFileW(pathTheme + L"/theme1.xml"))
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
        sApplication += L"/";
        sApplication += UTF8_TO_U(sVersion);
        std::wstring sApp = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
        sApp += sApplication;
        sApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
        NSFile::CFileBinary oAppWriter;
        if (oAppWriter.CreateFileW(pathDocProps + L"/app.xml"))
        {
            oAppWriter.WriteStringUTF8(sApp);
            oAppWriter.CloseFile();
        }

        // .rels
        std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/><Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/></Relationships>";
        NSFile::CFileBinary oRelsWriter;
        if (oRelsWriter.CreateFileW(pathRels + L"/.rels"))
        {
            oRelsWriter.WriteStringUTF8(sRels);
            oRelsWriter.CloseFile();
        }

        // [Content_Types].xml
        std::wstring sContent = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"svg\" ContentType=\"image/svg+xml\"/><Default Extension=\"jfif\" ContentType=\"image/jpeg\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/footnotes.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/><Override PartName=\"/word/numbering.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\"/></Types>";
        NSFile::CFileBinary oContentWriter;
        if (oContentWriter.CreateFileW(strDirectory + L"/[Content_Types].xml"))
        {
            oContentWriter.WriteStringUTF8(sContent);
            oContentWriter.CloseFile();
        }

        // footnotes.xml.rels
        NSFile::CFileBinary oFootRelsWriter;
        if (oFootRelsWriter.CreateFileW(pathWordRels + L"/footnotes.xml.rels"))
        {
            oFootRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"></Relationships>");
            oFootRelsWriter.CloseFile();
        }

        // fontTable.xml
        std::wstring sFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Arial\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
        NSFile::CFileBinary oFontTableWriter;
        if (oFontTableWriter.CreateFileW(pathWord + L"/fontTable.xml"))
        {
            oFontTableWriter.WriteStringUTF8(sFontTable);
            oFontTableWriter.CloseFile();
        }

        // settings.xml
        std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"14\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
        NSFile::CFileBinary oSettingsWriter;
        if (oSettingsWriter.CreateFileW(pathWord + L"/settings.xml"))
        {
            oSettingsWriter.WriteStringUTF8(sSettings);
            oSettingsWriter.CloseFile();
        }

        // webSettings.xml
        std::wstring sWebSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/></w:webSettings>";
        NSFile::CFileBinary oWebWriter;
        if (oWebWriter.CreateFileW(pathWord + L"/webSettings.xml"))
        {
            oWebWriter.WriteStringUTF8(sWebSettings);
            oWebWriter.CloseFile();
        }

        // numbering.xml
        NSStringUtils::CStringBuilder oNumbering;
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
        if(oParams != NULL)
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
        if (oCoreWriter.CreateFileW(pathDocProps + L"/core.xml"))
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

        m_oDocXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";

        m_oNoteXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
        m_oNoteXml += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

        m_oStylesXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:docDefaults><w:rPrDefault><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\" w:hint=\"default\"/><w:sz w:val=\"22\"/><w:szCs w:val=\"22\"/><w:lang w:val=\"ru-RU\" w:bidi=\"ar-SA\" w:eastAsia=\"en-US\"/></w:rPr></w:rPrDefault><w:pPrDefault><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"276\" w:after=\"200\"/></w:pPr></w:pPrDefault></w:docDefaults>";
        // m_oStylesXml += L"<w:latentStyles w:defLockedState=\"0\" w:defUIPriority=\"99\" w:defSemiHidden=\"1\" w:defUnhideWhenUsed=\"1\" w:defQFormat=\"0\" w:count=\"267\"><w:lsdException w:name=\"Normal\" w:semiHidden=\"0\" w:uiPriority=\"0\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 1\" w:semiHidden=\"0\" w:uiPriority=\"9\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 2\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 3\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 4\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 5\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 6\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 7\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 8\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 9\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"toc 1\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 2\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 3\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 4\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 5\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 6\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 7\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 8\" w:uiPriority=\"39\"/><w:lsdException w:name=\"toc 9\" w:uiPriority=\"39\"/><w:lsdException w:name=\"caption\" w:uiPriority=\"35\" w:qFormat=\"1\"/><w:lsdException w:name=\"Title\" w:semiHidden=\"0\" w:uiPriority=\"10\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Default Paragraph Font\" w:uiPriority=\"1\"/><w:lsdException w:name=\"Subtitle\" w:semiHidden=\"0\" w:uiPriority=\"11\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Strong\" w:semiHidden=\"0\" w:uiPriority=\"22\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Emphasis\" w:semiHidden=\"0\" w:uiPriority=\"20\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Table Grid\" w:semiHidden=\"0\" w:uiPriority=\"59\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Placeholder Text\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"No Spacing\" w:semiHidden=\"0\" w:uiPriority=\"1\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Light Shading\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Revision\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"List Paragraph\" w:semiHidden=\"0\" w:uiPriority=\"34\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Quote\" w:semiHidden=\"0\" w:uiPriority=\"29\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Quote\" w:semiHidden=\"0\" w:uiPriority=\"30\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Medium List 2 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 1\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/>";
        // m_oStylesXml += L"<w:lsdException w:name=\"Colorful List Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 2\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 3\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 4\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 5\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Shading Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"60\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light List Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"61\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Light Grid Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"62\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 1 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"63\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Shading 2 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"64\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 1 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"65\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium List 2 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"66\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 1 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"67\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 2 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"68\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Medium Grid 3 Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"69\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Dark List Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"70\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Shading Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"71\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful List Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"72\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Colorful Grid Accent 6\" w:semiHidden=\"0\" w:uiPriority=\"73\" w:unhideWhenUsed=\"0\"/><w:lsdException w:name=\"Subtle Emphasis\" w:semiHidden=\"0\" w:uiPriority=\"19\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Emphasis\" w:semiHidden=\"0\" w:uiPriority=\"21\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Subtle Reference\" w:semiHidden=\"0\" w:uiPriority=\"31\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Reference\" w:semiHidden=\"0\" w:uiPriority=\"32\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Book Title\" w:semiHidden=\"0\" w:uiPriority=\"33\" w:unhideWhenUsed=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"Bibliography\" w:uiPriority=\"37\"/><w:lsdException w:name=\"TOC Heading\" w:uiPriority=\"39\" w:qFormat=\"1\"/></w:latentStyles>";

        // Стили по умолчанию
        // Нормальный стиль, от которого базируются
        m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/></w:style>";
        // Маркированный список
        m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>";
        // Заголовки
        // m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"h1\"><w:name w:val=\"Heading 1\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h1-c\"/><w:qFormat/><w:rPr><w:b/><w:bCs/><w:color w:val=\"000000\" w:themeColor=\"text1\"/><w:sz w:val=\"48\"/><w:szCs w:val=\"48\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"480\"/><w:outlineLvl w:val=\"0\"/></w:pPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"h2\"><w:name w:val=\"Heading 2\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h2-c\"/><w:qFormat/><w:unhideWhenUsed/><w:rPr><w:b/><w:bCs/><w:color w:val=\"000000\" w:themeColor=\"text1\"/><w:sz w:val=\"40\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"200\"/><w:outlineLvl w:val=\"1\"/></w:pPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"h3\"><w:name w:val=\"Heading 3\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h3-c\"/><w:qFormat/><w:unhideWhenUsed/><w:rPr><w:b/><w:bCs/><w:i/><w:iCs/><w:color w:val=\"000000\" w:themeColor=\"text1\"/><w:sz w:val=\"36\"/><w:szCs w:val=\"36\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"200\"/><w:outlineLvl w:val=\"2\"/></w:pPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"h4\"><w:name w:val=\"Heading 4\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h4-c\"/><w:qFormat/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"232323\"/><w:sz w:val=\"32\"/><w:szCs w:val=\"32\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"200\"/><w:outlineLvl w:val=\"3\"/></w:pPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"h5\"><w:name w:val=\"Heading 5\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h5-c\"/><w:qFormat/><w:unhideWhenUsed/><w:rPr><w:b/><w:bCs/><w:color w:val=\"444444\"/><w:sz w:val=\"28\"/><w:szCs w:val=\"28\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"200\"/><w:outlineLvl w:val=\"4\"/></w:pPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"h6\"><w:name w:val=\"Heading 6\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"h6-c\"/><w:qFormat/><w:unhideWhenUsed/><w:rPr><w:i/><w:iCs/><w:color w:val=\"232323\"/><w:sz w:val=\"28\"/><w:szCs w:val=\"28\"/></w:rPr><w:pPr><w:keepLines/><w:keepNext/><w:spacing w:after=\"0\" w:before=\"200\"/><w:outlineLvl w:val=\"5\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"h1-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 1 character\"/><w:link w:val=\"h1\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:sz w:val=\"40\"/><w:szCs w:val=\"40\"/></w:rPr></w:style><w:style w:type=\"character\" w:styleId=\"h2-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 2 character\"/><w:link w:val=\"h2\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:sz w:val=\"34\"/></w:rPr></w:style><w:style w:type=\"character\" w:styleId=\"h3-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 3 character\"/><w:link w:val=\"h3\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:sz w:val=\"30\"/><w:szCs w:val=\"30\"/></w:rPr></w:style><w:style w:type=\"character\" w:styleId=\"h4-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 4 character\"/><w:link w:val=\"h4\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:b/><w:bCs/><w:sz w:val=\"26\"/><w:szCs w:val=\"26\"/></w:rPr></w:style><w:style w:type=\"character\" w:styleId=\"h5-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 5 character\"/><w:link w:val=\"h5\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:b/><w:bCs/><w:sz w:val=\"24\"/><w:szCs w:val=\"24\"/></w:rPr></w:style><w:style w:type=\"character\" w:styleId=\"h6-c\" w:customStyle=\"1\"><w:name w:val=\"Heading 6 character\"/><w:link w:val=\"h6\"/><w:uiPriority w:val=\"9\"/><w:rPr><w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/><w:b/><w:bCs/><w:sz w:val=\"22\"/><w:szCs w:val=\"22\"/></w:rPr></w:style>";
        // Текст: p, div
        // m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"p-c\" w:customStyle=\"1\"><w:name w:val=\"Paragraph_character\"/><w:link w:val=\"p\"/></w:style><w:style w:type=\"paragraph\" w:styleId=\"p\" w:customStyle=\"1\"><w:name w:val=\"Paragraph-p\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"p-c\"/><w:qFormat/><w:pPr><w:ind w:firstLine=\"567\"/><w:jc w:val=\"both\"/></w:pPr></w:style>";
        // m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"div-c\" w:customStyle=\"1\"><w:name w:val=\"Paragraph_character\"/><w:link w:val=\"div\"/></w:style><w:style w:type=\"paragraph\" w:styleId=\"div\" w:customStyle=\"1\"><w:name w:val=\"Paragraph-div\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"div-c\"/><w:qFormat/><w:pPr><w:ind w:firstLine=\"567\"/><w:jc w:val=\"both\"/></w:pPr></w:style>";
        // Ссылки
        m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0563C1\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>";
        // Таблицы
        m_oStylesXml += L"<w:style w:type=\"table\" w:default=\"1\" w:styleId=\"table-based\"><w:name w:val=\"Normal Table\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:tblPr><w:tblInd w:w=\"0\" w:type=\"dxa\"/><w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:left w:w=\"108\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:right w:w=\"108\" w:type=\"dxa\"/></w:tblCellMar></w:tblPr></w:style><w:style w:type=\"table\" w:styleId=\"table\"><w:name w:val=\"Table Grid\"/><w:basedOn w:val=\"table-based\"/><w:uiPriority w:val=\"59\"/><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:tblPr><w:tblBorders><w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/></w:tblBorders></w:tblPr></w:style>";
        // Сноски
        m_oStylesXml.WriteString(L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>");
    }

    bool readSrc(const std::wstring& sFileName)
    {
        // Читаем html
        if(!isHtml())
            return false;
        if(m_oLightReader.IsEmptyNode())
            return true;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"head")
                readHead();
            else if(sName == L"body")
                readBody(sFileName);
        }
        return true;
    }

    void write()
    {
        m_oDocXmlRels.WriteString(L"</Relationships>");
        NSFile::CFileBinary oRelsWriter;
        if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/document.xml.rels"))
        {
            oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
            oRelsWriter.CloseFile();
        }

        m_oDocXml.WriteString(L"<w:sectPr/></w:body></w:document>");
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

    void htmlXhtml(const std::wstring& sSrc)
    {
        NSFile::CFileBinary oXhtmlWriter;
        if (oXhtmlWriter.CreateFileW(m_sTmp + L"/res.xhtml"))
        {
            // htmlToXhtml возвращает текст файла в кодировке UTF-8
            oXhtmlWriter.WriteStringUTF8(htmlToXhtml(sSrc));
            oXhtmlWriter.CloseFile();
        }
    }

    void readStyle()
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Стиль по ссылке
            if(sName == L"link")
            {
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"href")
                    {
                        std::wstring sRef = m_oLightReader.GetText();
                        // Если это css файл, то поведение аналогично тэгу style
                        // Кроме функции получения стилей
                        std::wstring sType = NSFile::GetFileExtention(sRef);
                        if(sType == L"css")
                            m_oStylesCalculator.AddStylesFromFile(m_sSrc + L"/" + sRef);
                    }
                }
                m_oLightReader.MoveToElement();
            }
            // тэг style содержит стили для styles.xml
            else if(sName == L"style")
            {
                std::string sText = m_oLightReader.GetText2A();
                m_oStylesCalculator.AddStyles(sText);
            }
            readStyle();
        }
    }

private:

    std::vector<NSCSS::CNode> GetSubClass(const std::vector<NSCSS::CNode>& sSelectors, std::wstring& sNote)
    {
        std::vector<NSCSS::CNode> sSubClass(sSelectors);

        NSCSS::CNode oNode;
        oNode.m_sName = m_oLightReader.GetName();
        if(oNode.m_sName == L"#text")
            return sSubClass;
        // Стиль по атрибуту
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sAName = m_oLightReader.GetName();
            if(sAName == L"class")
                oNode.m_sClass = m_oLightReader.GetText();
            else if(sAName == L"id")
                oNode.m_sId    = m_oLightReader.GetText();
            else if(sAName == L"style")
                oNode.m_sStyle = m_oLightReader.GetText();
            else if(sAName == L"title")
                sNote          = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();

        sSubClass.push_back(oNode);
        return sSubClass;
    }

    std::wstring GetStyle(std::vector<NSCSS::CNode> sSelectors, bool bP)
    {
        NSCSS::CNode oChild = sSelectors.back();
        sSelectors.pop_back();
        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(oChild, sSelectors);
        if(bP)
            m_oXmlStyle.WritePStyle(oStyle);
        else
            m_oXmlStyle.WriteRStyle(oStyle);
        std::wstring sRes = m_oXmlStyle.GetId();
        std::wstring sStyle = m_oXmlStyle.GetStyle();
        m_oStylesXml.WriteString(sStyle);
        return sRes;
    }

    void readHead()
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Базовый адрес
            if(sName == L"base")
            {
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"href")
                        m_sBase = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();
            }
        }
    }

    void readBody(const std::wstring& sFileName)
    {
        bool bWasP = true;
        bool bWasPPr = false;
        std::vector<NSCSS::CNode> sSelectors;
        std::wstring sEmptyNote;
        sSelectors = GetSubClass(sSelectors, sEmptyNote);
        CTextSettings oTS { false, false, -1 };

        auto it = m_sSrcs.find(sFileName);
        if(it != m_sSrcs.end())
        {
            for(const std::wstring& sId : it->second)
            {
                std::wstring sCrossId = std::to_wstring(m_nCrossId++);
                m_oDocXml.WriteString(L"<w:bookmarkStart w:id=\"");
                m_oDocXml.WriteString(sCrossId);
                m_oDocXml.WriteString(L"\" w:name=\"");
                m_oDocXml.WriteString(sId);
                m_oDocXml.WriteString(L"\"/><w:bookmarkEnd w:id=\"");
                m_oDocXml.WriteString(sCrossId);
                m_oDocXml.WriteString(L"\"/>");
            }
        }

        m_oDocXml.WriteString(L"<w:p>");
        readStream(&m_oDocXml, sSelectors, L"", oTS, bWasP, bWasPPr);
        m_oDocXml.WriteString(L"</w:p>");
    }

    void readStream(NSStringUtils::CStringBuilder* oXml, const std::vector<NSCSS::CNode>& sSelectors, std::wstring sStyle, const CTextSettings& oTS, bool& bWasP, bool& bWasPPr)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode2(nDeath))
        {
            std::wstring sNote;
            std::vector<NSCSS::CNode> sSubClass = GetSubClass(sSelectors, sNote);
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"#text")
            {
                std::wstring sPStyle = L"";
                if(!bWasPPr)
                {
                    oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");
                    sPStyle = GetStyle(sSubClass, true);
                    oXml->WriteString(sPStyle);
                    oXml->WriteString(L"\"/></w:pPr>");
                    bWasPPr = true;
                }
                oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
                std::wstring sRStyle = GetStyle(sSubClass, false);
                oXml->WriteString(sRStyle);
                oXml->WriteString(L"\"/>");
                oXml->WriteString(sStyle);
                oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");

                std::wstring sText = m_oLightReader.GetText();
                auto end = sText.end();
                if(oTS.bBdo)
                    std::reverse(sText.begin(), sText.end());
                if(oTS.bPre)
                {
                    size_t nAfter = sText.find(L'\n');
                    while(nAfter != std::wstring::npos)
                    {
                        std::wstring sSubText = sText.substr(0, nAfter);
                        sText.erase(0, sSubText.length() + 1);
                        oXml->WriteEncodeXmlString(sSubText);
                        oXml->WriteString(L"</w:t></w:r></w:p><w:p><w:pPr><w:pStyle w:val=\"");
                        oXml->WriteString(sPStyle);
                        oXml->WriteString(L"\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"");
                        oXml->WriteString(sRStyle);
                        oXml->WriteString(L"\"/>");
                        oXml->WriteString(sStyle);
                        oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
                        nAfter = sText.find(L'\n');
                    }
                    end = sText.end();
                }
                else
                    end = std::unique(sText.begin(), sText.end(), [] (wchar_t l, wchar_t r) { return std::iswspace(l) && std::iswspace(r); });

                sText = std::wstring(sText.begin(), end);
                oXml->WriteEncodeXmlString(sText);
                oXml->WriteString(L"</w:t></w:r>");
                bWasP = false;
                continue;
            }

            // Ссылки
            // Области ссылки
            if(sName == L"a" || sName == L"area")
                readA(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
            // Полужирный текст
            // Акцентированный текст
            else if(sName == L"b" || sName == L"strong")
                readStream(oXml, sSubClass, sStyle + L"<w:b/>", oTS, bWasP, bWasPPr);
            // Направление текста
            else if(sName == L"bdo")
            {
                std::wstring sDir = L"";
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"dir")
                        sDir = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();

                CTextSettings oTSBdo { sDir == L"rtl", oTS.bPre, oTS.nLi };
                readStream(oXml, sSubClass, sStyle, oTSBdo, bWasP, bWasPPr);
            }
            // Отмена направления текста
            else if(sName == L"bdi")
            {
                CTextSettings oTSBdo { false, oTS.bPre, oTS.nLi };
                readStream(oXml, sSubClass, sStyle, oTSBdo, bWasP, bWasPPr);
            }
            // Увеличивает размер шрифта
            else if(sName == L"big")
                readStream(oXml, sSubClass, sStyle + L"<w:sz w:val=\"26\"/>", oTS, bWasP, bWasPPr);
            // Перенос строки
            else if(sName == L"br")
                oXml->WriteString(L"<w:r><w:br/></w:r>");
            // Цитата, обычно выделяется курсивом
            // Новый термин, обычно выделяется курсивом
            // Акцентированный текст
            // Курсивный текст
            // Переменная, обычно выделяется курсивом
            else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i" || sName == L"var")
                readStream(oXml, sSubClass, sStyle + L"<w:i/>", oTS, bWasP, bWasPPr);
            // Код
            // Моноширинный шрифт, например, Consolas
            // Результат скрипта
            else if(sName == L"code" || sName == L"kbd" || sName == L"samp")
                readStream(oXml, sSubClass, sStyle + L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>", oTS, bWasP, bWasPPr);
            // Зачеркнутый текст
            else if(sName == L"del" || sName == L"s")
                readStream(oXml, sSubClass, sStyle + L"<w:strike/>", oTS, bWasP, bWasPPr);
            // Картинки
            else if(sName == L"img")
            {
                readImage(oXml, sStyle);
                bWasP = false;
                bWasPPr = false;
            }
            // Подчеркнутый
            else if(sName == L"ins")
                readStream(oXml, sSubClass, sStyle + L"<w:u w:val=\"single\"/>", oTS, bWasP, bWasPPr);
            // Выделенный текст, обычно выделяется желтым
            else if(sName == L"mark")
                readStream(oXml, sSubClass, sStyle + L"<w:highlight w:val=\"yellow\"/>", oTS, bWasP, bWasPPr);
            // Цитата, выделенная кавычками, обычно выделяется курсивом
            else if(sName == L"q")
            {
                oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
                std::wstring sStyle = GetStyle(sSubClass, false);
                oXml->WriteString(sStyle);
                oXml->WriteString(L"\"/>");
                oXml->WriteString(sStyle);
                oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">&quot;</w:t></w:r>");

                readStream(oXml, sSubClass, sStyle + L"<w:i/>", oTS, bWasP, bWasPPr);

                oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
                oXml->WriteString(sStyle);
                oXml->WriteString(L"\"/>");
                oXml->WriteString(sStyle);
                oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">&quot;</w:t></w:r>");
                bWasP = false;
            }
            // Текст верхнего регистра
            else if(sName == L"rt" || sName == L"sup")
                readStream(oXml, sSubClass, sStyle + L"<w:vertAlign w:val=\"superscript\"/>", oTS, bWasP, bWasPPr);
            // Уменьшает размер шрифта
            else if(sName == L"small")
                readStream(oXml, sSubClass, sStyle + L"<w:sz w:val=\"18\"/>", oTS, bWasP, bWasPPr);
            // Текст нижнего регистра
            else if(sName == L"sub")
                readStream(oXml, sSubClass, sStyle + L"<w:vertAlign w:val=\"subscript\"/>", oTS, bWasP, bWasPPr);
            // Векторная картинка
            else if(sName == L"svg")
            {
                readSVG(oXml);
                bWasP = false;
            }
            // Игнорируются тэги выполняющие скрипт
            else if(sName == L"template" || sName == L"canvas" || sName == L"video" || sName == L"math" || sName == L"rp"    ||
                    sName == L"command"  || sName == L"iframe" || sName == L"embed" || sName == L"wbr"  || sName == L"audio" ||
                    sName == L"bgsound"  || sName == L"applet" || sName == L"blink" || sName == L"keygen"|| sName == L"script")
                continue;
            // Без нового абзаца
            else if(sName == L"datalist" || sName == L"button" || sName == L"label" || sName == L"data" || sName == L"object" ||
                    sName == L"noscript" || sName == L"output" || sName == L"input" || sName == L"time" || sName == L"ruby"   ||
                    sName == L"progress" || sName == L"hgroup" || sName == L"meter" || sName == L"span" || sName == L"font"   ||
                    sName == L"basefont" || sName == L"center" || sName == L"acronym"|| sName == L"abbr")
                readStream(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
            // С нового абзаца
            else
            {
                if(!bWasP)
                {
                    auto it = std::find_if(sSubClass.begin(), sSubClass.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                    if(it != sSubClass.end())
                        oXml->WriteString(L"</w:hyperlink>");
                    oXml->WriteString(L"</w:p><w:p>");
                    if(it != sSubClass.end())
                        oXml->WriteString(L"<w:hyperlink>");
                    bWasP = true;
                    bWasPPr = false;
                }
                // Адрес
                if(sName == L"address")
                    readStream(oXml, sSubClass, sStyle + L"<w:i/>", oTS, bWasP, bWasPPr);
                // С нового абзаца
                else if(sName == L"article" || sName == L"header" || sName == L"div" || sName == L"blockquote" || sName == L"main" ||
                        sName == L"summary" || sName == L"footer" || sName == L"nav" || sName == L"figcaption" || sName == L"form" ||
                        sName == L"details" || sName == L"option" || sName == L"dd"  || sName == L"fieldset"   || sName == L"p"    ||
                        sName == L"section" || sName == L"figure" || sName == L"dl"  || sName == L"legend"     || sName == L"map"  ||
                        sName == L"dt"      || sName == L"aside"  ||
                        sName == L"h1" || sName == L"h2" || sName == L"h3" || sName == L"h4" || sName == L"h5" || sName == L"h6")
                    readStream(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
                // Горизонтальная линия
                else if(sName == L"hr")
                {
                    oXml->WriteString(L"<w:pPr><w:pBdr><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr>");
                    bWasP = false;
                    bWasPPr = true;
                }
                // Меню
                // Маркированный список
                else if(sName == L"menu" || sName == L"ul" || sName == L"select")
                    readLi(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr, true);
                // Нумерованный список
                else if(sName == L"ol")
                    readLi(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr, false);
                // Предварительно форматированный текст
                else if(sName == L"pre")
                {
                    CTextSettings oTSPre { oTS.bBdo, true, oTS.nLi };
                    readStream(oXml, sSubClass, sStyle + L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>", oTSPre, bWasP, bWasPPr);
                }
                // Таблицы
                else if(sName == L"table")
                {
                    auto it = std::find_if(sSubClass.begin(), sSubClass.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                    if(it != sSubClass.end())
                        oXml->WriteString(L"</w:hyperlink>");
                    oXml->WriteString(L"</w:p>");
                    bWasP = false;
                    bWasPPr = false;
                    readTable(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
                    oXml->WriteString(L"<w:p>");
                    if(it != sSubClass.end())
                        oXml->WriteString(L"<w:hyperlink>");
                    bWasP = true;
                    bWasPPr = false;
                }
                // Текст с границами
                else if(sName == L"textarea")
                {
                    oXml->WriteString(L"<w:pPr><w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr>");
                    bWasPPr = true;
                    readStream(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
                }
                // Неизвестный тэг. Выделять ли его абзацем?
                else
                    readStream(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
                readNote(oXml, sNote);
                sNote = L"";
                if(!bWasP)
                {
                    auto it = std::find_if(sSubClass.begin(), sSubClass.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                    if(it != sSubClass.end())
                        oXml->WriteString(L"</w:hyperlink>");
                    oXml->WriteString(L"</w:p><w:p>");
                    if(it != sSubClass.end())
                        oXml->WriteString(L"<w:hyperlink>");
                    bWasP = true;
                    bWasPPr = false;
                }
            }
            readNote(oXml, sNote);
        }
    }

    int  readTr    (NSStringUtils::CStringBuilder* oXml, const std::vector<NSCSS::CNode>& sSelectors, std::wstring sStyle, const CTextSettings& oTS, bool& bWasP, bool& bWasPPr)
    {
        std::vector<CTc> mTable;
        int nGridCol = 0;
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
                auto it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                auto it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                while(it1 != mTable.end() || it2 != mTable.end())
                {
                    oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:vAlign w:val=\"center\"/><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
                    std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
                    oXml->WriteString(sCol);
                    oXml->WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
                    j += stoi(sCol);
                    it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                    it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                }

                oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:vAlign w:val=\"center\"/>");
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
                if(nColspan != 1)
                {
                    oXml->WriteString(L"<w:gridSpan w:val=\"");
                    oXml->WriteString(std::to_wstring(nColspan));
                    oXml->WriteString(L"\"/>");
                    j += nColspan - 1;
                }
                oXml->WriteString(L"</w:tcPr><w:p>");

                std::wstring sEmptyNote;
                std::vector<NSCSS::CNode> sSubClass = GetSubClass(sSelectors, sEmptyNote);
                // Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
                if(m_oLightReader.GetName() == L"th")
                {
                    oXml->WriteString(L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>");
                    bWasP = false;
                    bWasPPr = true;
                    readStream(oXml, sSubClass, sStyle + L"<w:b/>", oTS, bWasP, bWasPPr);
                }
                // Читаем td. Ячейка таблицы. Выравнивание вправо
                else if(m_oLightReader.GetName() == L"td")
                {
                    oXml->WriteString(L"<w:pPr><w:jc w:val=\"left\"/></w:pPr>");
                    bWasP = false;
                    bWasPPr = true;
                    readStream(oXml, sSubClass, sStyle, oTS, bWasP, bWasPPr);
                }
                oXml->WriteString(L"</w:p></w:tc>");
                j++;

                // Вставляем ячейки после
                it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                while(it1 != mTable.end() || it2 != mTable.end())
                {
                    oXml->WriteString(L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/><w:vAlign w:val=\"center\"/><w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
                    std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
                    oXml->WriteString(sCol);
                    oXml->WriteString(L"\"/></w:tcPr><w:p></w:p></w:tc>");
                    j += stoi(sCol);
                    it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
                    it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
                }
            } while(m_oLightReader.ReadNextSiblingNode(nTrDeath));
            oXml->WriteString(L"</w:tr>");
            if(--j > nGridCol)
                nGridCol = j;
            i++;
        }
        return nGridCol;
    }

    void readTable (NSStringUtils::CStringBuilder* oXml, const std::vector<NSCSS::CNode>& sSelectors, std::wstring sStyle, const CTextSettings& oTS, bool& bWasP, bool& bWasPPr)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        NSStringUtils::CStringBuilder oCaption;
        NSStringUtils::CStringBuilder oHead;
        NSStringUtils::CStringBuilder oBody;
        NSStringUtils::CStringBuilder oFoot;
        int nGridCol = 0;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            int n = 0;
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"caption")
            {
                oCaption.WriteString(L"<w:p><w:pPr><w:jc w:val=\"center\"/></w:pPr>");
                bWasP = false;
                bWasPPr = true;
                readStream(&oCaption, sSelectors, sStyle, oTS, bWasP, bWasPPr);
                oCaption.WriteString(L"</w:p>");
                bWasP = false;
                bWasPPr = false;
            }
            if(sName == L"thead")
                n = readTr(&oHead, sSelectors, sStyle, oTS, bWasP, bWasPPr);
            else if(sName == L"tbody")
                n = readTr(&oBody, sSelectors, sStyle, oTS, bWasP, bWasPPr);
            else if(sName == L"tfoot")
                n = readTr(&oFoot, sSelectors, sStyle, oTS, bWasP, bWasPPr);
            if(n > nGridCol)
                nGridCol = n;
        }

        // Заголовок таблицы
        oXml->WriteString(oCaption.GetData());
        // Начало таблицы
        oXml->WriteString(L"<w:tbl><w:tblPr><w:tblStyle w:val=\"table\"/><w:tblW w:w=\"0\" w:type=\"auto\"/></w:tblPr>"); // <w:tblLayout w:type=\"fixed\"/></w:tblPr>";
        // Размеры таблицы
        /*
        std::wstring sGridCol = L"";
        if(nGridCol != 0)
            sGridCol = std::to_wstring((int)(9570.0 / (double)nGridCol));
        *oXml += L"<w:tblGrid>";
        for(int i = 0; i < nGridCol; i++)
        {
            *oXml += L"<w:gridCol w:w=\"";
            *oXml += sGridCol;
            *oXml += L"\"/>";
        }
        *oXml += L"</w:tblGrid>";
        */
        // Конец таблицы
        oXml->WriteString(oHead.GetData());
        oXml->WriteString(oBody.GetData());
        oXml->WriteString(oFoot.GetData());
        oXml->WriteString(L"</w:tbl>");
        // Пустая строка после таблицы, чтобы следующий текст не приклеивался
        oXml->WriteString(L"<w:p></w:p>");
    }

    void readLi    (NSStringUtils::CStringBuilder* oXml, const std::vector<NSCSS::CNode>& sSelectors, std::wstring sStyle, const CTextSettings& oTS, bool& bWasP, bool& bWasPPr, bool bType)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        // Нумерованный список
        if(!bType)
        {
            m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
            m_oNumberXml.WriteString(std::to_wstring(m_nNumberingId++));
            m_oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"1\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"1\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"1\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"1\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");
        }

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"optgroup")
            {
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"label")
                    {
                        if(!bWasP)
                        {
                            auto it = std::find_if(sSelectors.begin(), sSelectors.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                            if(it != sSelectors.end())
                                oXml->WriteString(L"</w:hyperlink>");
                            oXml->WriteString(L"</w:p><w:p>");
                            if(it != sSelectors.end())
                                oXml->WriteString(L"<w:hyperlink>");
                            bWasP = true;
                            bWasPPr = false;
                        }

                        std::wstring sPStyle = L"";
                        if(!bWasPPr)
                        {
                            oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");
                            sPStyle = GetStyle(sSelectors, true);
                            oXml->WriteString(sPStyle);
                            oXml->WriteString(L"\"/></w:pPr>");
                            bWasPPr = true;
                        }
                        oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
                        std::wstring sRStyle = GetStyle(sSelectors, false);
                        oXml->WriteString(sRStyle);
                        oXml->WriteString(L"\"/>");
                        oXml->WriteString(sStyle);
                        oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
                        oXml->WriteEncodeXmlString(m_oLightReader.GetText());
                        oXml->WriteString(L"</w:t></w:r>");

                        auto it = std::find_if(sSelectors.begin(), sSelectors.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                        if(it != sSelectors.end())
                            oXml->WriteString(L"</w:hyperlink>");
                        oXml->WriteString(L"</w:p><w:p>");
                        if(it != sSelectors.end())
                            oXml->WriteString(L"<w:hyperlink>");
                        bWasP = true;
                        bWasPPr = false;
                    }
                }
                m_oLightReader.MoveToElement();
                readLi(oXml, sSelectors, sStyle, oTS, bWasP, bWasPPr, true);
                continue;
            }
            if(sName != L"li" && sName != L"option")
            {
                readStream(oXml, sSelectors, sStyle, oTS, bWasP, bWasPPr);
                continue;
            }
            if(!bWasP)
            {
                auto it = std::find_if(sSelectors.begin(), sSelectors.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                if(it != sSelectors.end())
                    oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p><w:p>");
                if(it != sSelectors.end())
                    oXml->WriteString(L"<w:hyperlink>");
                bWasP = true;
                bWasPPr = false;
            }
            CTextSettings oTSLi { oTS.bBdo, oTS.bPre, oTS.nLi + 1 };
            oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"li\"/><w:numPr><w:ilvl w:val=\"");
            oXml->WriteString(std::to_wstring(oTSLi.nLi));
            oXml->WriteString(L"\"/><w:numId w:val=\"");
            oXml->WriteString(bType ? L"1" : std::to_wstring(m_nNumberingId));
            oXml->WriteString(L"\"/></w:numPr></w:pPr>");
            bWasP = true;
            bWasPPr = true;
            readStream(oXml, sSelectors, sStyle, oTSLi, bWasP, bWasPPr);
            if(!bWasP)
            {
                auto it = std::find_if(sSelectors.begin(), sSelectors.end(), [](const NSCSS::CNode& item){ return item.m_sName == L"a"; });
                if(it != sSelectors.end())
                    oXml->WriteString(L"</w:hyperlink>");
                oXml->WriteString(L"</w:p><w:p>");
                if(it != sSelectors.end())
                    oXml->WriteString(L"<w:hyperlink>");
                bWasP = true;
                bWasPPr = false;
            }
        }
    }

    void readA     (NSStringUtils::CStringBuilder* oXml, const std::vector<NSCSS::CNode>& sSelectors, std::wstring sStyle, const CTextSettings& oTS, bool& bWasP, bool& bWasPPr)
    {
        std::wstring sRef;
        std::wstring sTitle;
        std::wstring sAlt;
        bool bCross = false;
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"href")
            {
                sRef = m_oLightReader.GetText();
                if(sRef.length() > 1)
                {
                    if(sRef.front() == L'#')
                    {
                        bCross = true;
                        sRef = sRef.substr(1);
                        continue;
                    }
                }

                size_t nSrc = sRef.rfind(L"/");
                if(nSrc == std::wstring::npos)
                    nSrc = 0;
                else
                    nSrc++;
                size_t nLen = sRef.rfind(L"html");
                if(nLen == std::wstring::npos)
                    continue;
                else
                    nLen += 4;
                std::wstring sFileName = sRef.substr(nSrc, nLen - nSrc);
                auto it = m_sSrcs.find(sFileName);
                if(it != m_sSrcs.end())
                {
                    bCross = true;
                    sRef = L"cHyp" + std::to_wstring(m_nHyperlinkId++);
                    it->second.push_back(sRef);
                }
            }
            else if(sName == L"title")
                sTitle = m_oLightReader.GetText();
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

        if(sTitle.empty())
            sTitle = sRef;

        // Перекрестная ссылка внутри файла
        if(bCross)
        {
            oXml->WriteString(L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"");
            oXml->WriteString(sRef);
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
            oXml->WriteEncodeXmlString(sTitle);
            oXml->WriteString(L"\" r:id=\"rHyp");
            oXml->WriteString(std::to_wstring(m_nHyperlinkId++));
        }
        oXml->WriteString(L"\">");
        bWasP = true;
        if(m_oLightReader.IsEmptyNode())
        {
            oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
            std::wstring sRStyle = GetStyle(sSelectors, false);
            oXml->WriteString(sRStyle);
            oXml->WriteString(L"\"/>");
            oXml->WriteString(sStyle + L"<w:rStyle w:val=\"a\"/>");
            oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
            oXml->WriteString(sAlt);
            oXml->WriteString(L"</w:t></w:r>");
        }
        else
            readStream(oXml, sSelectors, sStyle + L"<w:rStyle w:val=\"a\"/>", oTS, bWasP, bWasPPr);
        oXml->WriteString(L"</w:hyperlink>");
        bWasP = false;
    }

    void readNote(NSStringUtils::CStringBuilder* oXml, const std::wstring& sNote)
    {
        if(sNote.empty())
            return;
        oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
        oXml->WriteString(std::to_wstring(m_nFootnoteId));
        oXml->WriteString(L"\"/></w:r>");

        m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
        m_oNoteXml.WriteString(std::to_wstring(m_nFootnoteId++));
        m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
        m_oNoteXml.WriteEncodeXmlString(sNote);
        m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");
    }

    void readImage (NSStringUtils::CStringBuilder* oXml, const std::wstring& sStyle)
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
            std::wstring sImageName = L"";
            std::wstring sImageId = std::to_wstring(m_nImageId);
            size_t nLen = (sSrcM.length() > 4 ? 4 : 0);
            // Картинка Base64
            if(sSrcM.substr(0, nLen) == L"data")
            {
                size_t nBase = sSrcM.find(L"/", nLen);
                if(nBase == std::wstring::npos)
                    continue;
                nBase++;
                size_t nEndBase = sSrcM.find(L";", nBase);
                if(nEndBase == std::wstring::npos)
                    continue;
                std::wstring sType = sSrcM.substr(nBase, nEndBase - nBase);
                sImageName = sImageId + L"." + sType;
                NSFile::CFileBinary oImageWriter;
                if(oImageWriter.CreateFileW(m_sDst + L"/word/media/i" + sImageName))
                {

                    nBase = sSrcM.find(L"base64", nLen);
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
            else if(sSrcM.substr(0, nLen) == L"http" || !m_sBase.empty())
            {
                std::wstring sExtention = NSFile::GetFileExtention(sSrcM);
                if(sExtention != L"bmp" && sExtention != L"svg" && sExtention != L"jfif" && sExtention != L"wmf" && sExtention != L"gif" &&
                   sExtention != L"jpe" && sExtention != L"png" && sExtention != L"jpeg" && sExtention != L"jpg" )
                    continue;

                sImageName = NSFile::GetFileName(sSrcM);
                sImageName.erase(std::remove_if(sImageName.begin(), sImageName.end(), [] (wchar_t ch) { return std::iswspace(ch); }), sImageName.end());
                CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
                oDownloadImg.SetFilePath(m_sDst + L"/word/media/i" + sImageName);
                bRes = oDownloadImg.DownloadSync();
            }
            // Картинка по относительному пути
            else
            {
                sImageName = NSFile::GetFileName(sSrcM);
                sImageName.erase(std::remove_if(sImageName.begin(), sImageName.end(), [] (wchar_t ch) { return std::iswspace(ch); }), sImageName.end());

                std::wstring sExtention = NSFile::GetFileExtention(sSrcM);
                if(sExtention != L"bmp" && sExtention != L"svg" && sExtention != L"jfif" && sExtention != L"wmf" && sExtention != L"gif" &&
                   sExtention != L"jpe" && sExtention != L"png" && sExtention != L"jpeg" && sExtention != L"jpg" )
                    continue;

                bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + sSrcM, m_sDst + L"/word/media/i" + sImageName);
                if(!bRes)
                    bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + NSFile::GetFileName(sSrcM), m_sDst + L"/word/media/i" + sImageName);
            }

            if(bRes)
                ImageRels(oXml, sImageId, L"i" + sImageName);
        }
        m_oLightReader.MoveToElement();

        if(!bRes)
        {
            oXml->WriteString(L"<w:r><w:rPr>");
            oXml->WriteString(sStyle);
            oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
            oXml->WriteEncodeXmlString(sAlt);
            oXml->WriteString(L"</w:t></w:r>");
        }
    }

    void ImageRels (NSStringUtils::CStringBuilder* oXml, const std::wstring& sImageId, const std::wstring& sImageName)
    {
        m_nImageId++;
        // Прописать рельсы
        m_oDocXmlRels.WriteString(L"<Relationship Id=\"rPic");
        m_oDocXmlRels.WriteString(sImageId);
        m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/");
        m_oDocXmlRels.WriteString(sImageName);
        m_oDocXmlRels.WriteString(L"\"/>");

        // Получаем размеры картинки
        CBgraFrame oBgraFrame;
        oBgraFrame.OpenFile(m_sDst + L"/word/media/" + sImageName);
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
            nH = (nH > 9000000 ? 9000000 : nH);
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
    }

    void readSVG   (NSStringUtils::CStringBuilder* oXml)
    {
        // Сохранить как .svg картинку
        NSStringUtils::CStringBuilder oSVG;
        bool bNeedXmlns = true;
        oSVG.WriteString(L"<svg ");
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"xmlns")
                bNeedXmlns = false;
            oSVG.WriteString(m_oLightReader.GetName());
            oSVG.WriteString(L"=\"");
            oSVG.WriteString(m_oLightReader.GetText());
            oSVG.WriteString(L"\" ");
        }
        m_oLightReader.MoveToElement();
        if(bNeedXmlns)
            oSVG.WriteString(L"xmlns=\"http://www.w3.org/2000/svg\"");
        oSVG.WriteString(L">");

        std::wstring sSVG = m_oLightReader.GetInnerXml();
        size_t nRef = sSVG.find(L"image");
        while(nRef != std::wstring::npos)
        {
            size_t nHRef = sSVG.find(L"href", nRef);
            if(nHRef == std::wstring::npos)
                break;
            nHRef += 6;
            if(sSVG.compare(nHRef, 4, L"http") == 0)
            {
                nRef = sSVG.find(L"image", nRef + 5);
                continue;
            }
            size_t nHRefLen = sSVG.find(L"\"", nHRef);
            if(nHRefLen == std::wstring::npos)
                break;
            std::wstring sImageName = sSVG.substr(nHRef, nHRefLen - nHRef);
            bool bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + sImageName, m_sDst + L"/word/media/" + NSFile::GetFileName(sImageName));
            if(!bRes)
                bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + NSFile::GetFileName(sImageName), m_sDst + L"/word/media/" + NSFile::GetFileName(sImageName));
            if(bRes)
                sSVG.replace(nHRef, nHRefLen - nHRef, NSFile::GetFileName(sImageName));
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
    m_internal->htmlXhtml(sFile);

    // Открывает файл на проверку
    if (!m_internal->m_oLightReader.FromFile(m_internal->m_sTmp + L"/res.xhtml"))
        return false;
    // Читаем html
    if(!m_internal->isHtml())
        return false;
    return true;
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmp = sFolder;
}

HRESULT CHtmlFile2::Open(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    if(!m_internal->m_oLightReader.IsValid())
        if(!IsHtmlFile(sSrc))
            return S_FALSE;

    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);
    m_internal->readStyle();

    // Переходим в начало
    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    if(!m_internal->readSrc(NSFile::GetFileName(sSrc)))
        return S_FALSE;
    m_internal->write();
    NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
    return S_OK;
}

HRESULT CHtmlFile2::OpenBatch(const std::vector<std::wstring>& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    m_internal->m_sDst = sDst;
    m_internal->CreateDocxEmpty(oParams);

    for(const std::wstring& sS : sSrc)
        m_internal->m_sSrcs.insert(std::make_pair(NSFile::GetFileName(sS), std::vector<std::wstring>()));

    for(const std::wstring& sS : sSrc)
    {
        #ifdef _DEBUG
        std::wcout << NSFile::GetFileName(sS) << std::endl;
        #endif

        m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sS);
        if(!IsHtmlFile(sS))
            return S_FALSE;
        m_internal->readStyle();

        // Переходим в начало
        if(!m_internal->m_oLightReader.MoveToStart())
            return S_FALSE;

        if(!m_internal->readSrc(NSFile::GetFileName(sS)))
            return S_FALSE;

        NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
        m_internal->m_oLightReader.Clear();
        m_internal->m_oStylesCalculator.Clear();
        m_internal->m_sBase = L"";
    }

    m_internal->write();
    return S_OK;
}

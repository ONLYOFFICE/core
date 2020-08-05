#include "CDocxFile.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"

#include "document/CDocument.h"
#include "sdt/CSdt.h"
#include "hyperlink/CHyperlink.h"

CDocxFile::CDocxFile()
{
    m_oDocumentXml          = NULL;
    m_oContentTypeXml       = NULL;
    m_oFontTableXml         = NULL;
    m_oSettingsXml          = NULL;
    m_oWebSettingsXml       = NULL;
    m_oStylesXml            = NULL;
    m_oRelationshipsXml     = NULL;
    m_oDocRelationshipsXml  = NULL;
    m_oAppXml               = NULL;
    m_oCoreXml              = NULL;
}

CDocxFile::~CDocxFile()
{
    NSDirectory::DeleteDirectory(m_sTempDir);
}

bool CDocxFile::AddParagraph(std::wstring sText, bool bNewPAge)
{
    if (m_oDocumentXml == NULL)
        return false;

    m_oDocumentXml->AddParagraph(sText, bNewPAge);
}

bool CDocxFile::AddParagraph(CParagraph *oParagraph)
{
    if (m_oDocumentXml == NULL)
        return false;

    m_oDocumentXml->AddParagraph(oParagraph);
}

bool CDocxFile::AddBookToc(CBookToc *oBookToc)
{

    if (oBookToc == NULL)
        return false;

    CSdt *oSdt = new CSdt;
    oSdt->SetDefoult();
    for (int i = 0; i < oBookToc->GetCountToc(); i++)
        oSdt->AddHeader(oBookToc->GetTextAndRef(i), m_oDocumentXml, m_oDocRelationshipsXml);

    m_oDocumentXml->AddElement(oSdt);

    return true;
}

bool CDocxFile::AddRelationship(CElement *oRelationship)
{
    if (oRelationship == NULL)
        return false;

    m_oDocRelationshipsXml->GetXmlStructure()->AddChildren(oRelationship);
    return true;
}

bool CDocxFile::CreateRelsDir()
{
    m_sRelsDir = m_sTempDir + L"/_rels";
    NSDirectory::CreateDirectory(m_sRelsDir);

    CreateRelsXml();

    return true;
}

bool CDocxFile::CreateWordDir()
{
    m_sWordDir = m_sTempDir + L"/word";
    NSDirectory::CreateDirectory(m_sWordDir);

    CreateDocRelsXml();
    CreateThemeXml();
    CreateStylesXml();
    CreateWebSettingsXml();
    CreateSettingsXml();
    CreateFontTableXml();
    CreateDocumentXml();

    return true;
}

bool CDocxFile::CreateDocPropsDir()
{
    m_sDocPropsDir = m_sTempDir + L"/docProps";
    NSDirectory::CreateDirectory(m_sDocPropsDir);

    CreateCoreXml();
    CreateAppXml();

    return true;
}

bool CDocxFile::CreateTempFiles(std::wstring sPathFile, std::wstring sPathTempDir)
{
    m_sTempDir = sPathTempDir + L"/docx";

    NSDirectory::CreateDirectory(m_sTempDir);
    NSFile::CFileBinary oFileBinary;

    if (!CreateContentTypesXml())
        return false;

    if (!CreateRelsDir() || !CreateWordDir() || !CreateDocPropsDir())
        return false;

    return true;
}

bool CDocxFile::CreateDocumentXml()
{
    m_oDocumentXml = new CDocumentXml;
    m_oDocumentXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateFontTableXml()
{
    m_oFontTableXml = new CFontTableXml;
    m_oFontTableXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateContentTypesXml()
{
    m_oContentTypeXml = new CContentTypeXml;
    m_oContentTypeXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateSettingsXml()
{
    m_oSettingsXml = new CSettingsXml;
    m_oSettingsXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateWebSettingsXml()
{
    m_oWebSettingsXml = new CWebSettingsXml;
    m_oWebSettingsXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateRelsXml()
{
    m_oRelationshipsXml = new CRelationshipsXml;
    m_oRelationshipsXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateDocRelsXml()
{
    std::wstring sPathDir = m_sWordDir + L"/_rels";
    NSDirectory::CreateDirectory(sPathDir);

    m_oDocRelationshipsXml = new CDocRelationshipsXml;
    m_oDocRelationshipsXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateAppXml()
{
    m_oAppXml = new CAppXml;
    m_oAppXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateCoreXml()
{
    m_oCoreXml = new CCoreXml;
    m_oCoreXml->SetDefoult();

    return true;
}

bool CDocxFile::CreateThemeXml()
{
    std::wstring sPathThemeDir = m_sWordDir + L"/theme";
    NSDirectory::CreateDirectory(sPathThemeDir);
    std::wstring sPathFile = sPathThemeDir + L"/theme1.xml";
    std::string sXml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\"Тема Office\"><a:themeElements><a:clrScheme name=\"Стандартная\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Стандартная\"><a:majorFont><a:latin typeface=\"Calibri Light\" panose=\"020F0302020204030204\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/><a:font script=\"Jpan\" typeface=\"游ゴシック Light\"/><a:font script=\"Hang\" typeface=\"맑은 고딕\"/><a:font script=\"Hans\" typeface=\"等线 Light\"/><a:font script=\"Hant\" typeface=\"新細明體\"/><a:font script=\"Arab\" typeface=\"Times New Roman\"/><a:font script=\"Hebr\" typeface=\"Times New Roman\"/><a:font script=\"Thai\" typeface=\"Angsana New\"/><a:font script=\"Ethi\" typeface=\"Nyala\"/><a:font script=\"Beng\" typeface=\"Vrinda\"/><a:font script=\"Gujr\" typeface=\"Shruti\"/><a:font script=\"Khmr\" typeface=\"MoolBoran\"/><a:font script=\"Knda\" typeface=\"Tunga\"/><a:font script=\"Guru\" typeface=\"Raavi\"/><a:font script=\"Cans\" typeface=\"Euphemia\"/><a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/><a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/><a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/><a:font script=\"Thaa\" typeface=\"MV Boli\"/><a:font script=\"Deva\" typeface=\"Mangal\"/><a:font script=\"Telu\" typeface=\"Gautami\"/><a:font script=\"Taml\" typeface=\"Latha\"/><a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/><a:font script=\"Orya\" typeface=\"Kalinga\"/><a:font script=\"Mlym\" typeface=\"Kartika\"/><a:font script=\"Laoo\" typeface=\"DokChampa\"/><a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/><a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/><a:font script=\"Viet\" typeface=\"Times New Roman\"/><a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/><a:font script=\"Geor\" typeface=\"Sylfaen\"/></a:majorFont><a:minorFont><a:latin typeface=\"Calibri\" panose=\"020F0502020204030204\"/><a:ea typeface=\"\"/><a:cs typeface=\"\"/><a:font script=\"Jpan\" typeface=\"游明朝\"/><a:font script=\"Hang\" typeface=\"맑은 고딕\"/><a:font script=\"Hans\" typeface=\"等线\"/><a:font script=\"Hant\" typeface=\"新細明體\"/><a:font script=\"Arab\" typeface=\"Arial\"/><a:font script=\"Hebr\" typeface=\"Arial\"/><a:font script=\"Thai\" typeface=\"Cordia New\"/><a:font script=\"Ethi\" typeface=\"Nyala\"/><a:font script=\"Beng\" typeface=\"Vrinda\"/><a:font script=\"Gujr\" typeface=\"Shruti\"/><a:font script=\"Khmr\" typeface=\"DaunPenh\"/><a:font script=\"Knda\" typeface=\"Tunga\"/><a:font script=\"Guru\" typeface=\"Raavi\"/><a:font script=\"Cans\" typeface=\"Euphemia\"/><a:font script=\"Cher\" typeface=\"Plantagenet Cherokee\"/><a:font script=\"Yiii\" typeface=\"Microsoft Yi Baiti\"/><a:font script=\"Tibt\" typeface=\"Microsoft Himalaya\"/><a:font script=\"Thaa\" typeface=\"MV Boli\"/><a:font script=\"Deva\" typeface=\"Mangal\"/><a:font script=\"Telu\" typeface=\"Gautami\"/><a:font script=\"Taml\" typeface=\"Latha\"/><a:font script=\"Syrc\" typeface=\"Estrangelo Edessa\"/><a:font script=\"Orya\" typeface=\"Kalinga\"/><a:font script=\"Mlym\" typeface=\"Kartika\"/><a:font script=\"Laoo\" typeface=\"DokChampa\"/><a:font script=\"Sinh\" typeface=\"Iskoola Pota\"/><a:font script=\"Mong\" typeface=\"Mongolian Baiti\"/><a:font script=\"Viet\" typeface=\"Arial\"/><a:font script=\"Uigh\" typeface=\"Microsoft Uighur\"/><a:font script=\"Geor\" typeface=\"Sylfaen\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Стандартная\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:lumMod val=\"110000\"/><a:satMod val=\"105000\"/><a:tint val=\"67000\"/></a:schemeClr></a:gs><a:gs pos=\"50000\"><a:schemeClr val=\"phClr\"><a:lumMod val=\"105000\"/><a:satMod val=\"103000\"/><a:tint val=\"73000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:lumMod val=\"105000\"/><a:satMod val=\"109000\"/><a:tint val=\"81000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"5400000\" scaled=\"0\"/></a:gradFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:satMod val=\"103000\"/><a:lumMod val=\"102000\"/><a:tint val=\"94000\"/></a:schemeClr></a:gs><a:gs pos=\"50000\"><a:schemeClr val=\"phClr\"><a:satMod val=\"110000\"/><a:lumMod val=\"100000\"/><a:shade val=\"100000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:lumMod val=\"99000\"/><a:satMod val=\"120000\"/><a:shade val=\"78000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"5400000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/><a:miter lim=\"800000\"/></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/><a:miter lim=\"800000\"/></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:prstDash val=\"solid\"/><a:miter lim=\"800000\"/></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst/></a:effectStyle><a:effectStyle><a:effectLst/></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"57150\" dist=\"19050\" dir=\"5400000\" algn=\"ctr\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"63000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:solidFill><a:schemeClr val=\"phClr\"><a:tint val=\"95000\"/><a:satMod val=\"170000\"/></a:schemeClr></a:solidFill><a:gradFill rotWithShape=\"1\"><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"93000\"/><a:satMod val=\"150000\"/><a:shade val=\"98000\"/><a:lumMod val=\"102000\"/></a:schemeClr></a:gs><a:gs pos=\"50000\"><a:schemeClr val=\"phClr\"><a:tint val=\"98000\"/><a:satMod val=\"130000\"/><a:shade val=\"90000\"/><a:lumMod val=\"103000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"63000\"/><a:satMod val=\"120000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"5400000\" scaled=\"0\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/><a:extraClrSchemeLst/><a:extLst><a:ext uri=\"{05A4C25C-085E-4340-85A3-A5531E510DB2}\"><thm15:themeFamily xmlns:thm15=\"http://schemas.microsoft.com/office/thememl/2012/main\" name=\"Office Theme\" id=\"{62F939B6-93AF-4DB8-9C6B-D6C7DFDC589F}\" vid=\"{4A3C46E8-61CC-4603-A589-7422A47A8E4A}\"/></a:ext></a:extLst></a:theme>";
    std::wstring sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sXml.c_str(), (LONG)strlen((const char*)sXml.c_str()));

    XmlUtils::CXmlWriter oXmlWriter;
    oXmlWriter.WriteString(sTemp);
    if (!oXmlWriter.SaveToFile(sPathFile))
        return false;

    return true;
}

bool CDocxFile::CreateStylesXml()
{
    std::wstring sPathFile = m_sWordDir + L"/styles.xml";
    std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" mc:Ignorable=\"w14 w15 w16se\"><w:docDefaults><w:rPrDefault><w:rPr><w:rFonts w:asciiTheme=\"minorHAnsi\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsiTheme=\"minorHAnsi\" w:cstheme=\"minorBidi\"/><w:sz w:val=\"22\"/><w:szCs w:val=\"22\"/><w:lang w:val=\"ru-RU\" w:eastAsia=\"zh-TW\" w:bidi=\"ar-SA\"/></w:rPr></w:rPrDefault><w:pPrDefault><w:pPr><w:spacing w:after=\"160\" w:line=\"259\" w:lineRule=\"auto\"/></w:pPr></w:pPrDefault></w:docDefaults><w:latentStyles w:defLockedState=\"0\" w:defUIPriority=\"99\" w:defSemiHidden=\"0\" w:defUnhideWhenUsed=\"0\" w:defQFormat=\"0\" w:count=\"371\"><w:lsdException w:name=\"Normal\" w:uiPriority=\"0\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 1\" w:uiPriority=\"9\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 2\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 3\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 4\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 5\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 6\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 7\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 8\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"heading 9\" w:semiHidden=\"1\" w:uiPriority=\"9\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"index 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 6\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 7\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 8\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index 9\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 1\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 2\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 3\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 4\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 5\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 6\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 7\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 8\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toc 9\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Normal Indent\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"footnote text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"annotation text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"header\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"footer\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"index heading\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"caption\" w:semiHidden=\"1\" w:uiPriority=\"35\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"table of figures\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"envelope address\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"envelope return\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"footnote reference\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"annotation reference\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"line number\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"page number\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"endnote reference\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"endnote text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"table of authorities\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"macro\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"toa heading\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Bullet\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Number\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Bullet 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Bullet 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Bullet 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Bullet 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Number 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Number 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Number 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Number 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Title\" w:uiPriority=\"10\" w:qFormat=\"1\"/><w:lsdException w:name=\"Closing\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Signature\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Default Paragraph Font\" w:semiHidden=\"1\" w:uiPriority=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text Indent\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Continue\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/>"
                        "<w:lsdException w:name=\"List Continue 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Continue 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Continue 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"List Continue 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Message Header\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Subtitle\" w:uiPriority=\"11\" w:qFormat=\"1\"/><w:lsdException w:name=\"Salutation\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Date\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text First Indent\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text First Indent 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Note Heading\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text Indent 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Body Text Indent 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Block Text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Hyperlink\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"FollowedHyperlink\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Strong\" w:uiPriority=\"22\" w:qFormat=\"1\"/><w:lsdException w:name=\"Emphasis\" w:uiPriority=\"20\" w:qFormat=\"1\"/><w:lsdException w:name=\"Document Map\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Plain Text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"E-mail Signature\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Top of Form\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Bottom of Form\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Normal (Web)\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Acronym\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Address\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Cite\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Code\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Definition\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Keyboard\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Preformatted\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Sample\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Typewriter\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"HTML Variable\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Normal Table\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"annotation subject\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"No List\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Outline List 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Outline List 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Outline List 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Simple 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Simple 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Simple 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Classic 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Classic 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Classic 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Classic 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Colorful 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Colorful 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Colorful 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Columns 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Columns 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Columns 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Columns 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Columns 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 6\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 7\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Grid 8\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 4\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 5\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 6\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 7\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table List 8\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table 3D effects 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table 3D effects 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table 3D effects 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Contemporary\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Elegant\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Professional\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Subtle 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Subtle 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Web 1\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Web 2\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Table Web 3\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Balloon Text\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/>"
                        "<w:lsdException w:name=\"Table Grid\" w:uiPriority=\"39\"/><w:lsdException w:name=\"Table Theme\" w:semiHidden=\"1\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"Placeholder Text\" w:semiHidden=\"1\"/><w:lsdException w:name=\"No Spacing\" w:uiPriority=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"Light Shading\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 1\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 1\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 1\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 1\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 1\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 1\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Revision\" w:semiHidden=\"1\"/><w:lsdException w:name=\"List Paragraph\" w:uiPriority=\"34\" w:qFormat=\"1\"/><w:lsdException w:name=\"Quote\" w:uiPriority=\"29\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Quote\" w:uiPriority=\"30\" w:qFormat=\"1\"/><w:lsdException w:name=\"Medium List 2 Accent 1\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 1\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 1\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 1\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 1\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading Accent 1\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 1\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 1\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 2\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 2\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 2\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 2\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 2\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 2\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2 Accent 2\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 2\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 2\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 2\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 2\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading Accent 2\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 2\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 2\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 3\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 3\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 3\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 3\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 3\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 3\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2 Accent 3\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 3\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 3\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 3\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 3\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading Accent 3\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 3\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 3\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 4\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 4\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 4\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 4\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 4\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 4\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2 Accent 4\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 4\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 4\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 4\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 4\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading Accent 4\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 4\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 4\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 5\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 5\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 5\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 5\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 5\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 5\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2 Accent 5\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 5\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 5\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 5\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 5\" w:uiPriority=\"70\"/><w:lsdException w:name=\"Colorful Shading Accent 5\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 5\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 5\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Light Shading Accent 6\" w:uiPriority=\"60\"/><w:lsdException w:name=\"Light List Accent 6\" w:uiPriority=\"61\"/><w:lsdException w:name=\"Light Grid Accent 6\" w:uiPriority=\"62\"/><w:lsdException w:name=\"Medium Shading 1 Accent 6\" w:uiPriority=\"63\"/><w:lsdException w:name=\"Medium Shading 2 Accent 6\" w:uiPriority=\"64\"/><w:lsdException w:name=\"Medium List 1 Accent 6\" w:uiPriority=\"65\"/><w:lsdException w:name=\"Medium List 2 Accent 6\" w:uiPriority=\"66\"/><w:lsdException w:name=\"Medium Grid 1 Accent 6\" w:uiPriority=\"67\"/><w:lsdException w:name=\"Medium Grid 2 Accent 6\" w:uiPriority=\"68\"/><w:lsdException w:name=\"Medium Grid 3 Accent 6\" w:uiPriority=\"69\"/><w:lsdException w:name=\"Dark List Accent 6\" w:uiPriority=\"70\"/>"
                        "<w:lsdException w:name=\"Colorful Shading Accent 6\" w:uiPriority=\"71\"/><w:lsdException w:name=\"Colorful List Accent 6\" w:uiPriority=\"72\"/><w:lsdException w:name=\"Colorful Grid Accent 6\" w:uiPriority=\"73\"/><w:lsdException w:name=\"Subtle Emphasis\" w:uiPriority=\"19\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Emphasis\" w:uiPriority=\"21\" w:qFormat=\"1\"/><w:lsdException w:name=\"Subtle Reference\" w:uiPriority=\"31\" w:qFormat=\"1\"/><w:lsdException w:name=\"Intense Reference\" w:uiPriority=\"32\" w:qFormat=\"1\"/><w:lsdException w:name=\"Book Title\" w:uiPriority=\"33\" w:qFormat=\"1\"/><w:lsdException w:name=\"Bibliography\" w:semiHidden=\"1\" w:uiPriority=\"37\" w:unhideWhenUsed=\"1\"/><w:lsdException w:name=\"TOC Heading\" w:semiHidden=\"1\" w:uiPriority=\"39\" w:unhideWhenUsed=\"1\" w:qFormat=\"1\"/><w:lsdException w:name=\"Plain Table 1\" w:uiPriority=\"41\"/><w:lsdException w:name=\"Plain Table 2\" w:uiPriority=\"42\"/><w:lsdException w:name=\"Plain Table 3\" w:uiPriority=\"43\"/><w:lsdException w:name=\"Plain Table 4\" w:uiPriority=\"44\"/><w:lsdException w:name=\"Plain Table 5\" w:uiPriority=\"45\"/><w:lsdException w:name=\"Grid Table Light\" w:uiPriority=\"40\"/><w:lsdException w:name=\"Grid Table 1 Light\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 1\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 1\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 1\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 1\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 1\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 1\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 1\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 2\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 2\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 2\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 2\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 2\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 2\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 2\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 3\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 3\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 3\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 3\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 3\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 3\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 3\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 4\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 4\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 4\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 4\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 4\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 4\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 4\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 5\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 5\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 5\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 5\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 5\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 5\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 5\" w:uiPriority=\"52\"/><w:lsdException w:name=\"Grid Table 1 Light Accent 6\" w:uiPriority=\"46\"/><w:lsdException w:name=\"Grid Table 2 Accent 6\" w:uiPriority=\"47\"/><w:lsdException w:name=\"Grid Table 3 Accent 6\" w:uiPriority=\"48\"/><w:lsdException w:name=\"Grid Table 4 Accent 6\" w:uiPriority=\"49\"/><w:lsdException w:name=\"Grid Table 5 Dark Accent 6\" w:uiPriority=\"50\"/><w:lsdException w:name=\"Grid Table 6 Colorful Accent 6\" w:uiPriority=\"51\"/><w:lsdException w:name=\"Grid Table 7 Colorful Accent 6\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 1\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 1\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 1\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 1\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 1\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful Accent 1\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 1\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 2\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 2\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 2\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 2\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 2\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful Accent 2\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 2\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 3\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 3\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 3\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 3\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 3\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful Accent 3\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 3\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 4\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 4\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 4\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 4\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 4\" w:uiPriority=\"50\"/>"
                        "<w:lsdException w:name=\"List Table 6 Colorful Accent 4\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 4\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 5\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 5\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 5\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 5\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 5\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful Accent 5\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 5\" w:uiPriority=\"52\"/><w:lsdException w:name=\"List Table 1 Light Accent 6\" w:uiPriority=\"46\"/><w:lsdException w:name=\"List Table 2 Accent 6\" w:uiPriority=\"47\"/><w:lsdException w:name=\"List Table 3 Accent 6\" w:uiPriority=\"48\"/><w:lsdException w:name=\"List Table 4 Accent 6\" w:uiPriority=\"49\"/><w:lsdException w:name=\"List Table 5 Dark Accent 6\" w:uiPriority=\"50\"/><w:lsdException w:name=\"List Table 6 Colorful Accent 6\" w:uiPriority=\"51\"/><w:lsdException w:name=\"List Table 7 Colorful Accent 6\" w:uiPriority=\"52\"/></w:latentStyles><w:style w:type=\"paragraph\" w:default=\"1\" w:styleId=\"a\"><w:name w:val=\"Normal\"/><w:qFormat/></w:style><w:style w:type=\"character\" w:default=\"1\" w:styleId=\"a0\"><w:name w:val=\"Default Paragraph Font\"/><w:uiPriority w:val=\"1\"/><w:semiHidden/><w:unhideWhenUsed/></w:style><w:style w:type=\"table\" w:default=\"1\" w:styleId=\"a1\"><w:name w:val=\"Normal Table\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:tblPr><w:tblInd w:w=\"0\" w:type=\"dxa\"/><w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:left w:w=\"108\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:right w:w=\"108\" w:type=\"dxa\"/></w:tblCellMar></w:tblPr></w:style><w:style w:type=\"numbering\" w:default=\"1\" w:styleId=\"a2\"><w:name w:val=\"No List\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/></w:style></w:styles>";

    XmlUtils::CXmlWriter oXmlWriter;
    oXmlWriter.WriteString(sXml);
    if (!oXmlWriter.SaveToFile(sPathFile))
        return false;

    return true;
}

bool CDocxFile::SaveToFile()
{
    std::wstring sPathDocFile = m_sWordDir + L"/document.xml";
    std::wstring sPathAppFile = m_sDocPropsDir + L"/app.xml";
    std::wstring sPathCoreFile = m_sDocPropsDir + L"/core.xml";
    std::wstring sPathDocRelsFile = m_sWordDir + L"/_rels/document.xml.rels";
    std::wstring sPathRelsFile = m_sRelsDir + L"/.rels";
    std::wstring sPathWebSetFile = m_sWordDir + L"/webSettings.xml";
    std::wstring sPathConTypeFile = m_sTempDir + L"/[Content_Types].xml";
    std::wstring sPathFontTableFile = m_sWordDir + L"/fontTable.xml";
    std::wstring sParhSettingsFile = m_sWordDir + L"/settings.xml";


    if (!m_oDocumentXml->SaveToFile(sPathDocFile))
        return false;
    if (!m_oFontTableXml->SaveToFile(sPathFontTableFile))
        return false;
    if (!m_oContentTypeXml->SaveToFile(sPathConTypeFile))
        return false;
    if (!m_oSettingsXml->SaveToFile(sParhSettingsFile))
        return false;
    if (!m_oWebSettingsXml->SaveToFile(sPathWebSetFile))
        return false;
    if (!m_oRelationshipsXml->SaveToFile(sPathRelsFile))
        return false;
    if (!m_oDocRelationshipsXml->SaveToFile(sPathDocRelsFile))
        return false;
    if (!m_oAppXml->SaveToFile(sPathAppFile))
        return false;
    if (!m_oCoreXml->SaveToFile(sPathCoreFile))
        return false;

    return true;
}

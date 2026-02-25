#include "HTMLReader.h"

#include "../Common/Network/FileTransporter/include/FileTransporter.h"
#include "../DesktopEditor/common/Path.h"

#include "../Common/3dParty/html/htmltoxhtml.h"

#include "Common.h"

#include "Writers/OOXMLWriter.h"
#include "Tags/OOXMLTags.h"

#include "Writers/MDWriter.h"
#include "Tags/MDTags.h"

#include "../Common/3dParty/html/gumbo-parser/src/gumbo.h"
#include "src/StringFinder.h"

#include <boost/tuple/tuple.hpp>

#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/File.h"

namespace HTML
{
#define HTML_TAG(tag) GUMBO_TAG_##tag
#define ADD_TAG(strName, enumName) {strName, HTML_TAG(enumName)}
#define SKIP_TAG SCRIPT
#define UNKNOWN_TAG GumboTag::GUMBO_TAG_UNKNOWN
#define HtmlTag GumboTag

#define TAGS_COUNT 32

const static std::map<std::wstring, HtmlTag> m_HTML_TAGS
{
	ADD_TAG(L"a", A),
	ADD_TAG(L"abbr", ABBR),
	ADD_TAG(L"acronym", ACRONYM),
	ADD_TAG(L"address", ADDRESS),
	ADD_TAG(L"applet", APPLET),
	ADD_TAG(L"area", AREA),
	ADD_TAG(L"article", ARTICLE),
	ADD_TAG(L"aside", ASIDE),
	ADD_TAG(L"audio", AUDIO),
	ADD_TAG(L"b", B),
	ADD_TAG(L"base", BASE),
	ADD_TAG(L"basefont", BASEFONT),
	ADD_TAG(L"bdi", BDI),
	ADD_TAG(L"bdo", BDO),
	ADD_TAG(L"bgsound", BGSOUND),
	ADD_TAG(L"blockquote", BLOCKQUOTE),
	ADD_TAG(L"big", BIG),
	ADD_TAG(L"body", BODY),
	ADD_TAG(L"blink", BLINK),
	ADD_TAG(L"br", BR),
	ADD_TAG(L"button", BUTTON),
	ADD_TAG(L"canvas", CANVAS),
	ADD_TAG(L"caption", CAPTION),
	ADD_TAG(L"center", CENTER),
	ADD_TAG(L"cite", CITE),
	ADD_TAG(L"code", CODE),
	ADD_TAG(L"col", COL),
	ADD_TAG(L"colgroup", COLGROUP),
	ADD_TAG(L"command", SKIP_TAG), // Данного обозначения нет, но т.к.мы всё равно пропускаем, то делаем script
	ADD_TAG(L"comment", SKIP_TAG), // Данного обозначения нет, но т.к.мы всё равно пропускаем, то делаем script
	ADD_TAG(L"datalist", DATALIST),
	ADD_TAG(L"dd", DD),
	ADD_TAG(L"del", DEL),
	ADD_TAG(L"details", DETAILS),
	ADD_TAG(L"dfn", DFN),
	ADD_TAG(L"dir", DIR),
	ADD_TAG(L"div", DIV),
	ADD_TAG(L"dl", DL),
	ADD_TAG(L"dt", DT),
	ADD_TAG(L"em", EM),
	ADD_TAG(L"embed", EMBED),
	ADD_TAG(L"fieldset", FIELDSET),
	ADD_TAG(L"figcaption", FIGCAPTION),
	ADD_TAG(L"figure", FIGURE),
	ADD_TAG(L"font", FONT),
	ADD_TAG(L"form", FORM),
	ADD_TAG(L"footer", FOOTER),
	ADD_TAG(L"frame", FRAME),
	ADD_TAG(L"frameset", FRAMESET),
	ADD_TAG(L"h1", H1),
	ADD_TAG(L"h2", H2),
	ADD_TAG(L"h3", H3),
	ADD_TAG(L"h4", H4),
	ADD_TAG(L"h5", H5),
	ADD_TAG(L"h6", H6),
	ADD_TAG(L"head", HEAD),
	ADD_TAG(L"header", HEADER),
	ADD_TAG(L"hgroup", HGROUP),
	ADD_TAG(L"hr", HR),
	ADD_TAG(L"html", HTML),
	ADD_TAG(L"i", I),
	ADD_TAG(L"iframe", IFRAME),
	ADD_TAG(L"img", IMG),
	ADD_TAG(L"input", INPUT),
	ADD_TAG(L"ins", INS),
	ADD_TAG(L"isindex", ISINDEX),
	ADD_TAG(L"kbd", KBD),
	ADD_TAG(L"keygen", KEYGEN),
	ADD_TAG(L"label", LABEL),
	ADD_TAG(L"legend", LEGEND),
	ADD_TAG(L"li", LI),
	ADD_TAG(L"link", LINK),
	ADD_TAG(L"main", MAIN),
	ADD_TAG(L"map", MAP),
	ADD_TAG(L"marquee", MARQUEE),
	ADD_TAG(L"mark", MARK),
	ADD_TAG(L"menu", MENU),
	ADD_TAG(L"meta", META),
	ADD_TAG(L"meter", METER),
	ADD_TAG(L"nav", NAV),
	ADD_TAG(L"nobr", NOBR),
	ADD_TAG(L"noembed", NOEMBED),
	ADD_TAG(L"noframes", NOFRAMES),
	ADD_TAG(L"noscript", NOSCRIPT),
	ADD_TAG(L"object", OBJECT),
	ADD_TAG(L"ol", OL),
	ADD_TAG(L"optgroup", OPTGROUP),
	ADD_TAG(L"option", OPTION),
	ADD_TAG(L"output", OUTPUT),
	ADD_TAG(L"p", P),
	ADD_TAG(L"param", PARAM),
	ADD_TAG(L"plaintext", PLAINTEXT),
	ADD_TAG(L"pre", PRE),
	ADD_TAG(L"progress", PROGRESS),
	ADD_TAG(L"q", Q),
	ADD_TAG(L"rp", RP),
	ADD_TAG(L"rt", RT),
	ADD_TAG(L"ruby", RUBY),
	ADD_TAG(L"s", S),
	ADD_TAG(L"samp", SAMP),
	ADD_TAG(L"script", SCRIPT),
	ADD_TAG(L"section", SECTION),
	ADD_TAG(L"select", SELECT),
	ADD_TAG(L"small", SMALL),
	ADD_TAG(L"span", SPAN),
	ADD_TAG(L"source", SOURCE),
	ADD_TAG(L"strike", STRIKE),
	ADD_TAG(L"strong", STRONG),
	ADD_TAG(L"style", STYLE),
	ADD_TAG(L"sub", SUB),
	ADD_TAG(L"summary", SUMMARY),
	ADD_TAG(L"sup", SUP),
	ADD_TAG(L"table", TABLE),
	ADD_TAG(L"tbody", TBODY),
	ADD_TAG(L"td", TD),
	ADD_TAG(L"textarea", TEXTAREA),
	ADD_TAG(L"tfoot", TFOOT),
	ADD_TAG(L"th", TH),
	ADD_TAG(L"thead", THEAD),
	ADD_TAG(L"time", TIME),
	ADD_TAG(L"title", TITLE),
	ADD_TAG(L"tr", TR),
	ADD_TAG(L"tt", TT),
	ADD_TAG(L"u", U),
	ADD_TAG(L"ul", UL),
	ADD_TAG(L"var", VAR),
	ADD_TAG(L"video", VIDEO),
	ADD_TAG(L"wbr", WBR),
	ADD_TAG(L"xmp", XMP),

	ADD_TAG(L"svg", SVG)
};

bool HTML2XHTML(const std::wstring& wsFileName, XmlUtils::CXmlLiteReader& oLiteReader)
{
	BYTE* pData;
	DWORD nLength;
	if (!NSFile::CFileBinary::ReadAllBytes(wsFileName, &pData, nLength))
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

	const std::wstring sRes{htmlToXhtml(sFileContent, bNeedConvert)};

	#ifdef SAVE_NORMALIZED_HTML
	#if 1 == SAVE_NORMALIZED_HTML
	NSFile::CFileBinary oWriter;
	if (oWriter.CreateFileW(L"res.html"))
	{
		oWriter.WriteStringUTF8(sRes);
		oWriter.CloseFile();
	}
	#endif
	#endif

	return oLiteReader.FromString(sRes);
}

bool MHT2XHTML(const std::wstring& wsFileName, XmlUtils::CXmlLiteReader& oLiteReader)
{
	NSFile::CFileBinary file;
	if (!file.OpenFile(wsFileName))
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

	const std::string sContentType = NSStringFinder::FindProperty(xml_string, "content-type", ":", ";");
	bool bRes = false;

	if(NSStringFinder::Equals(sContentType, "multipart/related"))
	{
		BYTE* pData;
		DWORD nLength;
		if (!NSFile::CFileBinary::ReadAllBytes(wsFileName, &pData, nLength))
			return false;

		std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
		RELEASEARRAYOBJECTS(pData);
		const std::wstring sRes = mhtToXhtml(sFileContent);
		bRes = oLiteReader.FromString(sRes);
	}
	else
		bRes = HTML2XHTML(wsFileName, oLiteReader);

	RELEASEARRAYOBJECTS(buffer);
	return bRes;
}

inline std::wstring GetArgumentValue(XmlUtils::CXmlLiteReader& oLiteReader, const std::wstring& wsArgumentName, const std::wstring& wsDefaultValue = L"");
inline bool CheckArgumentMath(const std::wstring& wsNodeName, const std::wstring& wsStyleName);
inline HtmlTag GetHtmlTag(const std::wstring& wsStrTag);
inline bool UnreadableNode(const std::wstring& wsNodeName);
inline bool TagIsUnprocessed(const std::wstring& wsTagName);

CHTMLReader::CHTMLReader()
	: m_bIsTempDirOwner(true), m_pWriter(nullptr)
{}

CHTMLReader::~CHTMLReader()
{
	if (nullptr != m_pWriter)
		delete m_pWriter;

	if (m_bIsTempDirOwner && !m_wsTempDirectory.empty())
		NSDirectory::DeleteDirectory(m_wsTempDirectory);
}

void CHTMLReader::SetTempDirectory(const std::wstring& wsPath)
{
	m_wsTempDirectory = wsPath;
	m_bIsTempDirOwner = m_wsTempDirectory.empty();
}

void CHTMLReader::SetCoreDirectory(const std::wstring& wsPath)
{
	m_wsCoreDirectory = wsPath;
}

HRESULT CHTMLReader::ConvertHTML2OOXML(const std::wstring& wsPath, const std::wstring& wsDirectory, THTMLParameters* pParameters)
{
	return InitAndConvert2OOXML({wsPath}, wsDirectory, HTML2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertHTML2Markdown(const std::wstring& wsPath, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters)
{
	return InitAndConvert2Markdown({wsPath}, wsFinalFile, HTML2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertHTML2OOXML(const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, THTMLParameters* pParameters)
{
	return InitAndConvert2OOXML(arPaths, wsDirectory, HTML2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertHTML2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters)
{
	return InitAndConvert2Markdown(arPaths, wsFinalFile, HTML2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertMHT2OOXML(const std::wstring& wsPath, const std::wstring& wsDirectory, THTMLParameters* pParameters)
{
	return InitAndConvert2OOXML({wsPath}, wsDirectory, MHT2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertMHT2Markdown(const std::wstring& wsPath, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters)
{
	return InitAndConvert2Markdown({wsPath}, wsFinalFile, MHT2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertMHT2OOXML(const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, THTMLParameters* pParameters)
{
	return InitAndConvert2OOXML(arPaths, wsDirectory, MHT2XHTML, pParameters);
}

HRESULT CHTMLReader::ConvertMHT2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, TMarkdownParameters* pParameters)
{
	return InitAndConvert2Markdown(arPaths, wsFinalFile, MHT2XHTML, pParameters);
}

void CHTMLReader::Clear()
{
	if (nullptr != m_pWriter)
		delete m_pWriter;

	m_mTags.clear();

	m_wsSrcDirectory .clear();
	m_wsDstDirectory .clear();
	m_wsBaseDirectory.clear();
	m_wsCoreDirectory.clear();
}

void CHTMLReader::InitOOXMLTags(THTMLParameters* pParametrs)
{
	Clear();

	COOXMLWriter *pWriter = new COOXMLWriter(pParametrs, &m_oCSSCalculator);

	if (nullptr == pWriter)
		return;

	if (nullptr != m_pWriter)
		delete m_pWriter;

	pWriter->SetSrcDirectory (m_wsSrcDirectory);
	pWriter->SetDstDirectory (m_wsDstDirectory);
	pWriter->SetTempDirectory(m_wsTempDirectory);
	pWriter->SetBaseDirectory(m_wsBaseDirectory);
	pWriter->SetCoreDirectory(m_wsCoreDirectory);

	m_pWriter = pWriter;

	m_mTags.clear();
	m_mTags.reserve(TAGS_COUNT);

	m_mTags[HTML_TAG(A)]          = std::make_shared<CAnchor        <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(ABBR)]       = std::make_shared<CAnchor        <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(BR)]         = std::make_shared<CBreak         <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(DIV)]        = std::make_shared<CDivision      <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(IMG)]        = std::make_shared<CImage         <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(FONT)]       = std::make_shared<CFont          <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(INPUT)]      = std::make_shared<CInput         <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(BASEFONT)]   = std::make_shared<CBaseFont      <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(BLOCKQUOTE)] = std::make_shared<CBlockquote    <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(HR)]         = std::make_shared<CHorizontalRule<COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(OL)]         = std::make_shared<CList          <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(LI)]         = std::make_shared<CListElement   <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(CAPTION)]    = std::make_shared<CCaption       <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(TABLE)]      = std::make_shared<CTable         <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(TR)]         = std::make_shared<CTableRow      <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(TD)]         = std::make_shared<CTableCell     <COOXMLWriter>>(pWriter);
	m_mTags[HTML_TAG(HTML)]       = std::make_shared<CHTML          <COOXMLWriter>>(pWriter);

	std::shared_ptr<ITag> oIgnoredTag{std::make_shared<CEmptyTag>()};

	m_mTags[HTML_TAG(B)]      = oIgnoredTag;
	m_mTags[HTML_TAG(I)]      = oIgnoredTag;
	m_mTags[HTML_TAG(PRE)]    = oIgnoredTag;
	m_mTags[HTML_TAG(CENTER)] = oIgnoredTag;
	m_mTags[HTML_TAG(KBD)]    = oIgnoredTag;
	m_mTags[HTML_TAG(S)]      = oIgnoredTag;
	m_mTags[HTML_TAG(U)]      = oIgnoredTag;
	m_mTags[HTML_TAG(MARK)]   = oIgnoredTag;
	m_mTags[HTML_TAG(SUP)]    = oIgnoredTag;
	m_mTags[HTML_TAG(DD)]     = oIgnoredTag;
	m_mTags[HTML_TAG(Q)]      = oIgnoredTag;
	m_mTags[HTML_TAG(BDO)]    = oIgnoredTag;
	m_mTags[HTML_TAG(SPAN)]   = oIgnoredTag;
	m_mTags[HTML_TAG(H1)]     = oIgnoredTag;
	m_mTags[HTML_TAG(CODE)]   = oIgnoredTag;
}

void CHTMLReader::InitMDTags(TMarkdownParameters* pParametrs)
{
	CMDWriter *pWriter = new CMDWriter((nullptr != pParametrs) ? *pParametrs : TMarkdownParameters{});

	if (nullptr == pWriter)
		return;

	if (nullptr != m_pWriter)
		delete m_pWriter;

	m_pWriter = pWriter;

	m_mTags.clear();
	m_mTags.reserve(TAGS_COUNT);

	m_mTags[HTML_TAG(A)]          = std::make_shared<CAnchor        <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(B)]          = std::make_shared<CBold          <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(BR)]         = std::make_shared<CBreak         <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(I)]          = std::make_shared<CItalic        <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(S)]          = std::make_shared<CStrike        <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(Q)]          = std::make_shared<CQuotation     <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(H1)]         = std::make_shared<CHeader        <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(IMG)]        = std::make_shared<CImage         <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(HR)]         = std::make_shared<CHorizontalRule<CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(BLOCKQUOTE)] = std::make_shared<CBlockquote    <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(TABLE)]      = std::make_shared<CTable         <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(TR)]         = std::make_shared<CTableRow      <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(TD)]         = std::make_shared<CTableCell     <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(OL)]         = std::make_shared<CList          <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(LI)]         = std::make_shared<CListElement   <CMDWriter>>(pWriter);
	m_mTags[HTML_TAG(PRE)]        = std::make_shared<CPreformatted  <CMDWriter>>(pWriter);

	std::shared_ptr<ITag> oCode{std::make_shared<CCode<CMDWriter>>(pWriter)};

	m_mTags[HTML_TAG(CODE)] = oCode;
	m_mTags[HTML_TAG(KBD)]  = oCode;

	std::shared_ptr<ITag> oIgnoredTag{std::make_shared<CEmptyTag>()};

	m_mTags[HTML_TAG(ABBR)]     = oIgnoredTag;
	m_mTags[HTML_TAG(DIV)]      = oIgnoredTag;
	m_mTags[HTML_TAG(FONT)]     = oIgnoredTag;
	m_mTags[HTML_TAG(INPUT)]    = oIgnoredTag;
	m_mTags[HTML_TAG(BASEFONT)] = oIgnoredTag;
	m_mTags[HTML_TAG(CENTER)]   = oIgnoredTag;
	m_mTags[HTML_TAG(MARK)]     = oIgnoredTag;
	m_mTags[HTML_TAG(SUP)]      = oIgnoredTag;
	m_mTags[HTML_TAG(DD)]       = oIgnoredTag;
	m_mTags[HTML_TAG(BDO)]      = oIgnoredTag;
	m_mTags[HTML_TAG(SPAN)]     = oIgnoredTag;
	m_mTags[HTML_TAG(CAPTION)]  = oIgnoredTag;
	m_mTags[HTML_TAG(U)]        = oIgnoredTag;
	m_mTags[HTML_TAG(HTML)]     = oIgnoredTag;
}

bool CHTMLReader::IsHTML()
{
	return ((m_oLightReader.MoveToStart() && m_oLightReader.ReadNextNode()) ? m_oLightReader.GetName() == L"html" : false);
}

HRESULT CHTMLReader::InitAndConvert2OOXML(const std::vector<std::wstring>& arPaths, const std::wstring& wsDirectory, Convert_Func Convertation, THTMLParameters* pParameters)
{
	InitOOXMLTags(pParameters);
	m_wsDstDirectory = wsDirectory;

	HRESULT lResult{S_FALSE};

	m_pWriter->Begin(wsDirectory);

	for (const std::wstring& wsPath : arPaths)
	{
		if (Convert(wsPath, Convertation))
		{
			lResult = S_OK;

			if (nullptr != pParameters && pParameters->m_bNeedPageBreakBefore)
				m_pWriter->PageBreak();
		}
	}

	m_pWriter->End(wsDirectory);

	return lResult;
}

HRESULT CHTMLReader::InitAndConvert2Markdown(const std::vector<std::wstring>& arPaths, const std::wstring& wsFinalFile, Convert_Func Convertation, TMarkdownParameters* pParameters)
{
	InitMDTags(pParameters);

	HRESULT lResult{S_FALSE};

	m_pWriter->Begin(L"");

	for (const std::wstring& wsPath : arPaths)
	{
		if (Convert(wsPath, Convertation))
			lResult = S_OK;
	}

	m_pWriter->End(wsFinalFile);

	return lResult;
}

bool CHTMLReader::Convert(const std::wstring& wsPath, Convert_Func Convertation)
{
	if (nullptr == m_pWriter || !Convertation(wsPath, m_oLightReader) || !m_oLightReader.IsValid() || !IsHTML())
		return false;

	if (m_wsTempDirectory.empty())
	{
		m_wsTempDirectory = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());
	}

	m_wsSrcDirectory = NSSystemPath::GetDirectoryName(wsPath);

	m_oLightReader.MoveToStart();
	m_oLightReader.ReadNextNode();
	ReadStyle();

	// Переходим в начало
	if(!m_oLightReader.MoveToStart())
		return S_FALSE;

	ReadDocument();

	return true;
}

void CHTMLReader::ReadStyle()
{
	if(m_oLightReader.IsEmptyNode())
		return;

	const int nDeath = m_oLightReader.GetDepth();
	std::wstring sName;

	while(m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		sName = m_oLightReader.GetName();

		if(sName == L"body")
			ReadStyle2();
		else
		{
			// Стиль по ссылке
			if(sName == L"link")
			{
				while(m_oLightReader.MoveToNextAttribute())
					ReadStyleFromNetwork();

				m_oLightReader.MoveToElement();
			}
			// тэг style содержит стили для styles.xml
			else if(sName == L"style")
				m_oCSSCalculator.AddStyles(m_oLightReader.GetText2());
			else
				ReadStyle();
		}
	}
}

void CHTMLReader::ReadStyle2()
{
	const std::wstring wsName = m_oLightReader.GetName();
	// Стиль по ссылке
	if(wsName == L"link")
	{
		while(m_oLightReader.MoveToNextAttribute())
			ReadStyleFromNetwork();
		m_oLightReader.MoveToElement();
	}
	// тэг style содержит стили для styles.xml
	else if(wsName == L"style")
		m_oCSSCalculator.AddStyles(m_oLightReader.GetText2());

	const int nDeath = m_oLightReader.GetDepth();
	while(m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		if(!m_oLightReader.IsEmptyNode())
			ReadStyle2();
	}
}

void CHTMLReader::ReadStyleFromNetwork()
{
	if(m_oLightReader.GetName() != L"href")
		return;
	std::wstring sRef = m_oLightReader.GetText();
	if(NSFile::GetFileExtention(sRef) != L"css")
		return;
	std::wstring sFName = NSFile::GetFileName(sRef);
	// Стиль в сети
	if(sRef.substr(0, 4) == L"http")
	{
		sFName = m_wsTempDirectory + L'/' + sFName;
		NSNetwork::NSFileTransport::CFileDownloader oDownloadStyle(sRef, false);
		oDownloadStyle.SetFilePath(sFName);
		if(oDownloadStyle.DownloadSync())
		{
			m_oCSSCalculator.AddStylesFromFile(sFName);
			NSFile::CFileBinary::Remove(sFName);
		}
	}
	else
	{
		m_oCSSCalculator.AddStylesFromFile(m_wsSrcDirectory + L'/' + sFName);
		m_oCSSCalculator.AddStylesFromFile(m_wsSrcDirectory + L'/' + sRef);
	}
}

void CHTMLReader::ReadDocument()
{
	m_oLightReader.ReadNextNode();

	int nDeath = m_oLightReader.GetDepth();
	while(m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		const std::wstring wsName = m_oLightReader.GetName();
		if(wsName == L"head")
			ReadHead();
		else if(wsName == L"body")
			ReadBody();
	}
}

void CHTMLReader::ReadHead()
{
	if(m_oLightReader.IsEmptyNode())
		return;
	int nDeath = m_oLightReader.GetDepth();
	while (m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		const std::wstring wsName = m_oLightReader.GetName();
		// Базовый адрес
		if (L"base" == wsName)
			m_wsBaseDirectory = GetArgumentValue(m_oLightReader, L"href");
	}

	m_oLightReader.MoveToElement();
}

void CHTMLReader::ReadBody()
{
	std::vector<NSCSS::CNode> arSelectors;

	arSelectors.push_back(NSCSS::CNode(L"html", L"", L""));

	GetSubClass(arSelectors);

	if (!m_mTags[HTML_TAG(HTML)]->Open(arSelectors))
		return;

	std::map<std::wstring, std::wstring>::iterator itFound = arSelectors.back().m_mAttributes.find(L"bgcolor");

	if (arSelectors.back().m_mAttributes.end() != itFound)
		arSelectors.back().m_mAttributes.erase(itFound);

	m_oLightReader.MoveToElement();

	ReadStream(arSelectors);

	m_mTags[HTML_TAG(HTML)]->Close(arSelectors);
}

bool CHTMLReader::ReadStream(std::vector<NSCSS::CNode>& arSelectors, bool bInsertEmptyP)
{
	if (nullptr == m_pWriter)
		return false;

	bool bResult{false};

	const int nDeath = m_oLightReader.GetDepth();
	if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
	{
		if (!bInsertEmptyP)
			return false;

		m_pWriter->WriteEmptyParagraph();
		return true;
	}

	do
	{
		if (ReadInside(arSelectors))
			bResult = true;
	} while(m_oLightReader.ReadNextSiblingNode2(nDeath));

	if (!bResult && bInsertEmptyP)
		m_pWriter->WriteEmptyParagraph();

	return bResult;
}

bool CHTMLReader::ReadInside(std::vector<NSCSS::CNode>& arSelectors)
{
	const std::wstring wsName{m_oLightReader.GetName()};

	if(wsName == L"#text")
		return ReadText(arSelectors);

	//TODO:: обработать все варианты return'а
	if (UnreadableNode(wsName) || TagIsUnprocessed(wsName))
		return false;

	GetSubClass(arSelectors);

	bool bResult = true;

	const HtmlTag eHtmlTag{GetHtmlTag(wsName)};

	switch(eHtmlTag)
	{
		case HTML_TAG(A):
		case HTML_TAG(AREA):
		{
			bResult = ReadDefaultTag(HTML_TAG(A), arSelectors);
			break;
		}
		case HTML_TAG(ABBR):
		{
			bResult = ReadDefaultTag(HTML_TAG(ABBR), arSelectors);
			break;
		}
		case HTML_TAG(B):
		case HTML_TAG(STRONG):
		{
			bResult = ReadDefaultTag(HTML_TAG(B), arSelectors);
			break;
		}
		case HTML_TAG(BDO):
		case HTML_TAG(BDI):
		{
			bResult = ReadDefaultTag(HTML_TAG(BDO), arSelectors);
			break;
		}
		case HTML_TAG(BR):
		{
			bResult = ReadEmptyTag(HTML_TAG(BR), arSelectors);
			break;
		}
		case HTML_TAG(CENTER):
		{
			bResult = ReadDefaultTag(HTML_TAG(CENTER), arSelectors);
			break;
		}
		case HTML_TAG(CITE):
		case HTML_TAG(DFN):
		case HTML_TAG(EM):
		case HTML_TAG(I):
		case HTML_TAG(VAR):
		{
			bResult = ReadDefaultTag(HTML_TAG(I), arSelectors);
			break;
		}
		case HTML_TAG(CODE):
		case HTML_TAG(SAMP):
		case HTML_TAG(TT):
		case HTML_TAG(OUTPUT):
		{
			bResult = ReadDefaultTag(HTML_TAG(CODE), arSelectors);
			break;
		}
		case HTML_TAG(KBD):
		{
			bResult = ReadDefaultTag(HTML_TAG(KBD), arSelectors);
			break;
		}
		case HTML_TAG(DEL):
		case HTML_TAG(S):
		case HTML_TAG(STRIKE):
		{
			bResult = ReadDefaultTag(HTML_TAG(S), arSelectors);
			break;
		}
		case HTML_TAG(FONT):
		{
			bResult = ReadDefaultTag(HTML_TAG(FONT), arSelectors);
			break;
		}
		case HTML_TAG(IMG):

		{
			bResult = ReadEmptyTag(HTML_TAG(IMG), arSelectors);
			break;
		}
		case HTML_TAG(SVG):
		{
			bResult = ReadSVG(arSelectors);
			break;
		}
		case HTML_TAG(INS):
		case HTML_TAG(U):
		{
			bResult = ReadDefaultTag(HTML_TAG(U), arSelectors);
			break;
		}
		case HTML_TAG(MARK):
		{
			bResult = ReadDefaultTag(HTML_TAG(MARK), arSelectors);
			break;
		}
		case HTML_TAG(Q):
		{
			bResult = ReadDefaultTag(HTML_TAG(Q), arSelectors);
			break;
		}
		case HTML_TAG(SUP):
		case HTML_TAG(SUB):
		{
			bResult = ReadDefaultTag(HTML_TAG(SUP), arSelectors);
			break;
		}
		case HTML_TAG(INPUT):
		{
			bResult = ReadDefaultTag(HTML_TAG(INPUT), arSelectors);
			break;
		}
		case HTML_TAG(CANVAS):
		case HTML_TAG(VIDEO):
		case HTML_TAG(MATH):
		case HTML_TAG(IFRAME):
		case HTML_TAG(EMBED):
		case HTML_TAG(WBR):
		case HTML_TAG(AUDIO):
		case HTML_TAG(BGSOUND):
		case HTML_TAG(APPLET):
		case HTML_TAG(BLINK):
		case HTML_TAG(KEYGEN):
		case HTML_TAG(TITLE):
		case HTML_TAG(STYLE):
		case HTML_TAG(SCRIPT):
		{
			//Если встретили не обрабатываемые теги, то просто пропускаем
			arSelectors.pop_back();
			return false;
		}
		case HTML_TAG(SPAN):
		{
			bResult = ReadDefaultTag(HTML_TAG(SPAN), arSelectors);
			break;
		}
		case HTML_TAG(NOBR):
		{
			bResult = ReadDefaultTag(HTML_TAG(PRE), arSelectors);
			break;
		}
		case HTML_TAG(BASEFONT):
		{
			bResult = ReadDefaultTag(HTML_TAG(BASEFONT), arSelectors);
			break;
		}
		case HTML_TAG(BUTTON):
		case HTML_TAG(LABEL):
		case HTML_TAG(DATA):
		case HTML_TAG(OBJECT):
		case HTML_TAG(NOSCRIPT):
		case HTML_TAG(TIME):
		case HTML_TAG(SMALL):
		case HTML_TAG(PROGRESS):
		case HTML_TAG(HGROUP):
		case HTML_TAG(METER):
		case HTML_TAG(ACRONYM):
		case HTML_TAG(BIG):
		{
			bResult = ReadStream(arSelectors);
			break;
		}
		default:
		{
			m_pWriter->BeginBlock();

			switch(eHtmlTag)
			{
				case HTML_TAG(ADDRESS):
				{
					bResult = ReadDefaultTag(HTML_TAG(I), arSelectors);;
					break;
				}
				case HTML_TAG(DD):
				{
					bResult = ReadDefaultTag(HTML_TAG(DD), arSelectors);;
					break;
				}
				case HTML_TAG(H1):
				case HTML_TAG(H2):
				case HTML_TAG(H3):
				case HTML_TAG(H4):
				case HTML_TAG(H5):
				case HTML_TAG(H6):
				{
					bResult = ReadDefaultTag(HTML_TAG(H1), arSelectors);
					break;
				}
				case HTML_TAG(ASIDE):
				case HTML_TAG(DIV):
				{
					bResult = ReadDefaultTag(HTML_TAG(DIV), arSelectors);
					break;
				}
				case HTML_TAG(BLOCKQUOTE):
				{
					bResult = ReadDefaultTag(HTML_TAG(BLOCKQUOTE), arSelectors);
					break;
				}
				case HTML_TAG(ARTICLE):
				case HTML_TAG(HEADER):
				case HTML_TAG(MAIN):
				case HTML_TAG(SUMMARY):
				case HTML_TAG(FOOTER):
				case HTML_TAG(NAV):
				case HTML_TAG(FIGCAPTION):
				case HTML_TAG(FORM):
				case HTML_TAG(OPTION):
				case HTML_TAG(DT):
				case HTML_TAG(P):
				case HTML_TAG(SECTION):
				case HTML_TAG(FIGURE):
				case HTML_TAG(DL):
				case HTML_TAG(LEGEND):
				case HTML_TAG(MAP):
				{
					bResult = ReadStream(arSelectors);
					break;
				}
				case HTML_TAG(HR):
				{
					bResult = ReadEmptyTag(HTML_TAG(HR), arSelectors);
					break;
				}
				case HTML_TAG(LI):
				{
					bResult = ReadDefaultTag(HTML_TAG(LI), arSelectors);
					break;
				}
				case HTML_TAG(OL):
				case HTML_TAG(UL):
				{
					bResult = ReadDefaultTag(HTML_TAG(OL), arSelectors);
					break;
				}
		// 		case HTML_TAG(MENU):
		// 		case HTML_TAG(SELECT):
		// 		case HTML_TAG(DATALIST):
		// 		case HTML_TAG(DIR):
		// 		{
		// 			bResult = readLi(&oXmlData, sSelectors, oTS, HTML_TAG(OL) != eHtmlTag);
		// 			break;
		// 		}
				case HTML_TAG(PRE):
				case HTML_TAG(XMP):
				{
					bResult = ReadDefaultTag(HTML_TAG(PRE), arSelectors);
					break;
				}
				case HTML_TAG(TABLE):
				{
					bResult = ReadTable(arSelectors);
					break;
				}
		// 		case HTML_TAG(RUBY):
		// 		{
		// 			bResult = ParseRuby(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
				// case HTML_TAG(TEXTAREA):
				// case HTML_TAG(FIELDSET):
				// {
				// 	bResult = ReadStream(arSelectors);
				// 	break;
				// }
		// 		case HTML_TAG(DETAILS):
		// 		{
		// 			bResult = ReadDetails(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
				default:
				{
					bResult = ReadStream(arSelectors);
					break;
				}
			}

			m_pWriter->EndBlock(bResult);
		}
	}

	arSelectors.pop_back();
	return bResult;
}

bool CHTMLReader::ReadText(std::vector<NSCSS::CNode>& arSelectors)
{
	if (nullptr == m_pWriter)
		return false;

	GetSubClass(arSelectors);

	const bool bResult{m_pWriter->WriteText(m_oLightReader.GetText(), arSelectors)};

	arSelectors.pop_back();

	return bResult;
}

bool CHTMLReader::ReadSVG(const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[HTML_TAG(IMG)]->Open(arSelectors, m_oLightReader.GetOuterXml()))
		return false;

	m_mTags[HTML_TAG(IMG)]->Close(arSelectors);

	return true;
}

bool CHTMLReader::ReadTable(std::vector<NSCSS::CNode>& arSelectors)
{
	if(m_oLightReader.IsEmptyNode())
		return false;

	if (nullptr != m_pWriter && !m_pWriter->SupportNestedTables())
	{
		//Временно разруливаем это тут, так как по текущей логике мы сначала
		//читаем всю таблицу и её вложенные элементы, а потом приступаем к конвертации,
		//поэтому конвертору уже приходят вложенные таблицы, что в MD запрещено
		for (std::vector<NSCSS::CNode>::const_reverse_iterator itElement{arSelectors.crbegin() + 1}; itElement < arSelectors.crend(); ++itElement)
			if (L"table" == itElement->m_wsName)
				return false;

		if (nullptr != dynamic_cast<CMDWriter*>(m_pWriter))
			((CMDWriter*)m_pWriter)->EnteredTable();
	}

	CStorageTable oTable;

	NSCSS::CCompiledStyle *pStyle = arSelectors.back().m_pCompiledStyle;

	//Table styles
	std::wstring wsFrame;
	std::wstring wsValue;

	if (arSelectors.back().GetAttributeValue(L"border", wsValue))
	{
		const int nWidth = NSStringFinder::ToInt(wsValue);

		if (0 < nWidth)
		{
			oTable.SetRules(L"all");

			if (pStyle->m_oBorder.Empty())
			{
				pStyle->m_oBorder.SetStyle(L"outset",  0, true);
				pStyle->m_oBorder.SetWidth(nWidth,     NSCSS::UnitMeasure::Point, 0, true);
				pStyle->m_oBorder.SetColor(L"auto",    0, true);
			}
		}
		else if (pStyle->m_oBorder.Empty())
		{
			pStyle->m_oBorder.SetNone(0, true);
			oTable.SetRules(L"none");
		}
	}

	if (arSelectors.back().GetAttributeValue(L"cellpadding", wsValue))
		pStyle->m_oPadding.SetValues(wsValue + L"px", 0, true);

	if (arSelectors.back().GetAttributeValue(L"rules", wsValue))
		oTable.SetRules(wsValue);

	arSelectors.back().GetAttributeValue(L"frame", wsFrame);

	if (!wsFrame.empty() && pStyle->m_oBorder.Empty())
	{
		#define SetDefaultBorderSide(side) \
			pStyle->m_oBorder.SetStyle##side(L"solid", 0, true); \
			pStyle->m_oBorder.SetWidth##side(1,        NSCSS::UnitMeasure::Point, 0, true); \
			pStyle->m_oBorder.SetColor##side(L"black", 0, true)

		if (NSStringFinder::Equals(L"border", wsFrame))
		{
			SetDefaultBorderSide();
		}
		else if (NSStringFinder::Equals(L"above", wsFrame))
		{
			SetDefaultBorderSide(TopSide);
		}
		else if (NSStringFinder::Equals(L"below", wsFrame))
		{
			SetDefaultBorderSide(BottomSide);
		}
		else if (NSStringFinder::Equals(L"hsides", wsFrame))
		{
			SetDefaultBorderSide(TopSide);
			SetDefaultBorderSide(BottomSide);
		}
		else if (NSStringFinder::Equals(L"vsides", wsFrame))
		{
			SetDefaultBorderSide(LeftSide);
			SetDefaultBorderSide(RightSide);
		}
		else if (NSStringFinder::Equals(L"rhs", wsFrame))
		{
			SetDefaultBorderSide(RightSide);
		}
		else if (NSStringFinder::Equals(L"lhs", wsFrame))
		{
			SetDefaultBorderSide(LeftSide);
		}
	}

	if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Collapse)
		oTable.SetCellSpacing(0);
	else if (arSelectors.back().GetAttributeValue(L"cellspacing", wsValue))
		oTable.SetCellSpacing(NSStringFinder::ToInt(wsValue));
	else if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Separate)
		oTable.SetCellSpacing(15);

	oTable.SetWidth(pStyle->m_oDisplay.GetWidth());
	oTable.SetBorder(pStyle->m_oBorder);
	oTable.SetPadding(pStyle->m_oPadding);
	oTable.SetMargin(pStyle->m_oMargin);
	oTable.SetAlign(pStyle->m_oDisplay.GetHAlign().ToWString());
	//------

	//TODO:: переписать работу с таблицами без предварительной конвертации ячеек и хранения их внутренних данных
	//Читаем содержимое таблицы -> считаем ячейки -> нормализуем таблицу -> конвертим таблицу

	int nDeath = m_oLightReader.GetDepth();
	while(m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		const std::wstring sName = m_oLightReader.GetName();
		GetSubClass(arSelectors);

		if(sName == L"caption")
			ReadTableCaption(oTable, arSelectors);
		if(sName == L"thead")
			ReadTableRows(oTable, arSelectors, ERowParseMode::Header);
		if(sName == L"tbody")
			ReadTableRows(oTable, arSelectors, ERowParseMode::Body);
		else if(sName == L"tfoot")
			ReadTableRows(oTable, arSelectors, ERowParseMode::Foother);
		else if (sName == L"colgroup")
			ReadTableColspan(oTable);

		arSelectors.pop_back();
	}

	oTable.Shorten();
	oTable.CompleteTable();

	#define CONVERT_ROWS(rows, parse_mode)\
	{\
	const std::vector<CStorageTableRow*> arRows{rows};\
	\
	for (UINT unRow = 0; unRow < arRows.size(); ++unRow)\
	{\
		ERowPosition eRowPosition{ERowPosition::Middle};\
		\
		if (0 == unRow)\
			eRowPosition = ERowPosition::First;\
		else if (arRows.size() - 1 == unRow)\
			eRowPosition = ERowPosition::Last;\
		\
		if (!m_mTags[HTML_TAG(TR)]->Open(arSelectors, boost::tuple<const TTableRowStyle*, const CStorageTable&, ERowParseMode, ERowPosition>(&arRows[unRow]->GetStyles(), oTable, parse_mode, eRowPosition)))\
			continue;\
		\
		const std::vector<CStorageTableCell*>& arCells{arRows[unRow]->GetCells()};\
		\
		for (UINT unCol = 0; unCol < arCells.size(); ++unCol)\
		{\
			m_mTags[HTML_TAG(TD)]->Open(arSelectors, boost::tuple<const CStorageTableCell&, const CStorageTable&, UINT, ERowParseMode, ERowPosition>(*arCells[unCol], oTable, unCol, parse_mode, eRowPosition));\
			\
			if (0 != arCells[unCol]->GetData()->GetCurSize())\
			{\
				WriteToStringBuilder(*(arCells[unCol]->GetData()), *(m_pWriter->GetCurrentDocument()));\
				arCells[unCol]->GetData()->Clear();\
			}\
			else\
				m_pWriter->WriteEmptyParagraph();\
			\
			m_mTags[HTML_TAG(TD)]->Close(arSelectors);\
		}\
		\
		m_mTags[HTML_TAG(TR)]->Close(arSelectors);\
	}}

	if (!m_mTags[HTML_TAG(TABLE)]->Open(arSelectors, &oTable))
		return false;

	if (!oTable.HaveHeader())
	{
		if (m_mTags[HTML_TAG(TR)]->Open(arSelectors, boost::tuple<const TTableRowStyle*, const CStorageTable&, ERowParseMode, ERowPosition>(nullptr, oTable, ERowParseMode::Header, ERowPosition::First)))
			m_mTags[HTML_TAG(TR)]->Close(arSelectors);
	}
	else
	{
		for (const std::vector<CStorageTableRow*>& arHeader : oTable.GetHeaders())
			CONVERT_ROWS(arHeader, ERowParseMode::Header)
	}

	CONVERT_ROWS(oTable.GetRows(), ERowParseMode::Body)
	CONVERT_ROWS(oTable.GetFoothers(), ERowParseMode::Foother)

	m_mTags[HTML_TAG(TABLE)]->Close(arSelectors);

	return true;
}

void CHTMLReader::ReadTableCaption(CStorageTable& oTable, std::vector<NSCSS::CNode>& arSelectors)
{
	if (nullptr == m_pWriter)
		return;

	GetSubClass(arSelectors);
	m_pWriter->SetDataOutput(oTable.GetCaptionData());

	arSelectors.back().m_pCompiledStyle->m_oDisplay.SetVAlign(L"center", arSelectors.size());

	ReadDefaultTag(HTML_TAG(CAPTION), arSelectors);

	m_pWriter->RevertDataOutput();
	arSelectors.pop_back();
}

void CalculateCellStyles(TTableCellStyle* pCellStyle, std::vector<NSCSS::CNode>& arSelectors)
{
	if (NULL == pCellStyle)
		return;

	pCellStyle->m_wsVAlign    = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetVAlign().ToWString();
	pCellStyle->m_wsHAlign    = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHAlign().ToWString();
	pCellStyle->m_oBackground = arSelectors.back().m_pCompiledStyle->m_oBackground.GetColor();
	pCellStyle->m_oHeight     = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHeight();
	pCellStyle->m_oWidth      = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetWidth();
	pCellStyle->m_oPadding    = arSelectors.back().m_pCompiledStyle->m_oPadding;
	pCellStyle->m_oBorder     = arSelectors.back().m_pCompiledStyle->m_oBorder;

	if (pCellStyle->m_wsHAlign.empty())
		pCellStyle->m_wsHAlign = arSelectors.back().m_pCompiledStyle->m_oText.GetAlign().ToWString();
}

struct TRowspanElement
{
	UINT  m_unRowSpan;
	UINT  m_unColumnIndex;
	const CStorageTableCell* m_pCell;

	TRowspanElement(UINT unRowSpan, UINT unColumnIndex, const CStorageTableCell* pCell)
		: m_unRowSpan(unRowSpan), m_unColumnIndex(unColumnIndex), m_pCell(pCell)
	{}
};

void CHTMLReader::ReadTableRows(CStorageTable& oTable, std::vector<NSCSS::CNode>& arSelectors, ERowParseMode eMode)
{
	if (nullptr == m_pWriter)
		return;

	std::vector<TRowspanElement> arRowspanElements;
	std::vector<CStorageTableRow*>      arRows;

	int nDeath = m_oLightReader.GetDepth();
	while (m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		if (L"tr" != m_oLightReader.GetName())
			continue;

		GetSubClass(arSelectors);

		CStorageTableRow *pRow = new CStorageTableRow();

		for (std::vector<TRowspanElement>::iterator itElement = arRowspanElements.begin(); itElement < arRowspanElements.end();)
		{
			pRow->InsertCell(CStorageTableCell::CreateEmpty(itElement->m_pCell->GetColspan(), true, itElement->m_pCell->GetStyles()), itElement->m_unColumnIndex);

			itElement->m_unRowSpan--;
			if (1 == itElement->m_unRowSpan)
				itElement = arRowspanElements.erase(itElement);
			else
				++itElement;
		}

		UINT unColumnIndex = 0;
		int nTrDepth = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nTrDepth))
		{
			CStorageTableCell *pCell = new CStorageTableCell();

			if (NULL == pCell)
				continue;

			GetSubClass(arSelectors);

			std::vector<NSCSS::CNode> arNewSelectors{(std::vector<NSCSS::CNode>::const_iterator)std::find_if(arSelectors.begin(), arSelectors.end(), [](const NSCSS::CNode& oNode){ return L"table" == oNode.m_wsName; }), arSelectors.cend()};

			CalculateCellStyles(pCell->GetStyles(), arNewSelectors);

			std::wstring wsValue;

			if (arSelectors.back().GetAttributeValue(L"colspan", wsValue))
				pCell->SetColspan(NSStringFinder::ToInt(wsValue, 1), pRow->GetIndex());

			if (arSelectors.back().GetAttributeValue(L"rowspan", wsValue))
			{
				pCell->SetRowspan(NSStringFinder::ToInt(wsValue, 1));

				if (1 != pCell->GetRowspan())
					arRowspanElements.push_back({pCell->GetRowspan(), unColumnIndex, pCell});
			}

			// Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
			if(m_oLightReader.GetName() == L"th")
			{
				if (pCell->GetStyles()->m_wsHAlign.empty())
					arSelectors.back().m_pCompiledStyle->m_oText.SetAlign(L"center", arSelectors.size());

				arSelectors.back().m_pCompiledStyle->m_oFont.SetWeight(L"bold", arSelectors.size());

				m_pWriter->SetDataOutput(pCell->GetData());
				ReadStream(arSelectors, true);
				m_pWriter->RevertDataOutput();
			}
			// Читаем td. Ячейка таблицы
			else if(m_oLightReader.GetName() == L"td")
			{
				m_pWriter->SetDataOutput(pCell->GetData());
				ReadStream(arSelectors, true);
				m_pWriter->RevertDataOutput();
			}

			if (pRow->GetIndex() == MAXCOLUMNSINTABLE - 1)
			{
				while (m_oLightReader.ReadNextSiblingNode(nTrDepth))
				{
					if (L"td" != m_oLightReader.GetName() && L"th" != m_oLightReader.GetName())
						continue;

					GetSubClass(arSelectors);
					m_pWriter->SetDataOutput(pCell->GetData());
					ReadStream(arSelectors);
					m_pWriter->RevertDataOutput();
					arSelectors.pop_back();
				}
			}

			pRow->AddCell(pCell);
			arSelectors.pop_back();

			++unColumnIndex;

			if (pRow->GetIndex() == MAXCOLUMNSINTABLE)
				break;
		}

		arSelectors.pop_back();
		arRows.push_back(pRow);
	}

	oTable.AddRows(arRows, eMode);
}

void CHTMLReader::ReadTableColspan(CStorageTable& oTable)
{
	std::vector<NSCSS::CNode> arNodes;
	GetSubClass(arNodes);

	CTableColgroup *pColgroup = new CTableColgroup(arNodes.back());

	if (NULL == pColgroup)
		return;

	oTable.AddColgroup(pColgroup);

	const int nDeath = m_oLightReader.GetDepth();
	if (!m_oLightReader.IsEmptyNode() && m_oLightReader.ReadNextSiblingNode2(nDeath))
	{
		do
		{
			if (L"col" != m_oLightReader.GetName())
				continue;

			GetSubClass(arNodes);

			CTableCol *pCol = new CTableCol(arNodes.back());

			if (NULL == pCol)
			{
				arNodes.pop_back();
				continue;
			}

			CalculateCellStyles(pCol->GetStyle(), arNodes);
			arNodes.pop_back();

			if (NULL == pCol)
				continue;

			pColgroup->AddCol(pCol);
		} while(m_oLightReader.ReadNextSiblingNode2(nDeath));
	}

	if(pColgroup->Empty())
	{
		std::map<std::wstring, std::wstring>::const_iterator itFound = arNodes.begin()->m_mAttributes.find(L"span");

		CTableCol *pCol = new CTableCol((arNodes.begin()->m_mAttributes.cend() != itFound) ? NSStringFinder::ToInt(itFound->second, 1) : 1);

		if (NULL == pCol)
			return;

		CalculateCellStyles(pCol->GetStyle(), arNodes);

		pColgroup->AddCol(pCol);
	}
}

bool CHTMLReader::ReadEmptyTag(UINT unTag, const std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[unTag]->Open(arSelectors))
		return false;

	m_mTags[unTag]->Close(arSelectors);

	return true;
}

bool CHTMLReader::ReadDefaultTag(UINT unTag, std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[unTag]->Open(arSelectors))
		return false;

	const bool bResult{ReadStream(arSelectors)};

	m_mTags[unTag]->Close(arSelectors);

	return bResult;
}

void CHTMLReader::GetSubClass(std::vector<NSCSS::CNode>& arSelectors)
{
	NSCSS::CNode oNode;

	oNode.m_wsName = m_oLightReader.GetName();
	// Стиль по атрибуту
	std::wstring wsAttributeName;

	if (m_oLightReader.MoveToFirstAttribute())
	{
		do
		{
			wsAttributeName = m_oLightReader.GetName();
			if(wsAttributeName == L"class")
				oNode.m_wsClass  = EncodeXmlString(m_oLightReader.GetText());
			else if(wsAttributeName == L"id")
			{
				oNode.m_wsId = EncodeXmlString(m_oLightReader.GetText());
				// WriteEmptyBookmark(oXml, oNode.m_wsId);

				// if (!m_oStylesCalculator.HaveStylesById(oNode.m_wsId))
					// oNode.m_wsId.clear();
			}
			else if(wsAttributeName == L"style")
				oNode.m_wsStyle += m_oLightReader.GetText();
			else
			{
				if (CheckArgumentMath(oNode.m_wsName, wsAttributeName))
					oNode.m_mAttributes[wsAttributeName] = m_oLightReader.GetText();
			}
		}while(m_oLightReader.MoveToNextAttribute());
	}

	m_oLightReader.MoveToElement();
	arSelectors.push_back(oNode);

	m_oCSSCalculator.CalculateCompiledStyle(arSelectors);
}

inline std::wstring GetArgumentValue(XmlUtils::CXmlLiteReader& oLiteReader, const std::wstring& wsArgumentName, const std::wstring& wsDefaultValue)
{
	if (!oLiteReader.MoveToFirstAttribute())
		return wsDefaultValue;

	std::wstring wsValue{wsDefaultValue};

	do
	{
		if (wsArgumentName == oLiteReader.GetName())
		{
			wsValue = oLiteReader.GetText();
			break;
		}
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
	return wsValue;
}

// Так как CSS калькулятор не знает для какой ноды производится расчет стиля
// и не знает, что некоторые стили предназначены только определенной ноде,
// то проще пока обрабатывать это заранее
// ! Используется для стилей, заданных через аргументы !
inline bool CheckArgumentMath(const std::wstring& wsNodeName, const std::wstring& wsStyleName)
{
	if (L"border" == wsStyleName && L"table" != wsNodeName)
		return false;

	return true;
}

inline HtmlTag GetHtmlTag(const std::wstring& wsStrTag)
{
	std::map<std::wstring, HtmlTag>::const_iterator oFound = m_HTML_TAGS.find(wsStrTag);

	if (oFound == m_HTML_TAGS.cend())
	{
		if (wsStrTag.length() > 3 && wsStrTag.compare(wsStrTag.length() - 3, 3, L"svg") == 0)
			return HTML_TAG(SVG);
		return UNKNOWN_TAG;
	}

	return oFound->second;
}

inline bool UnreadableNode(const std::wstring& wsNodeName)
{
	return L"head" == wsNodeName || L"meta" == wsNodeName || L"style" == wsNodeName;
}

inline bool TagIsUnprocessed(const std::wstring& wsTagName)
{
	return L"xml" == wsTagName;
}
}

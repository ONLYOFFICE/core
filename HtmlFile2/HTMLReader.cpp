#include "HTMLReader.h"

#include "../Common/Network/FileTransporter/include/FileTransporter.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Path.h"

#include "Common.h"
#include "Tags/HTMLTags2OOXML.h"

namespace HTML
{
#define HTML_TAG(tag) GUMBO_TAG_##tag
#define ADD_TAG(strName, enumName) {strName, HTML_TAG(enumName)}
#define SKIP_TAG SCRIPT
#define UNKNOWN_TAG GumboTag::GUMBO_TAG_UNKNOWN

const std::map<std::wstring, HtmlTag> m_HTML_TAGS
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
inline std::wstring GetArgumentValue(XmlUtils::CXmlLiteReader& oLiteReader, const std::wstring& wsArgumentName, const std::wstring& wsDefaultValue = L"");
inline bool CheckArgumentMath(const std::wstring& wsNodeName, const std::wstring& wsStyleName);
inline HtmlTag GetHtmlTag(const std::wstring& wsStrTag);
inline bool UnreadableNode(const std::wstring& wsNodeName);
inline bool TagIsUnprocessed(const std::wstring& wsTagName);

#define ADD_OOXML_TAG(tag_name, struct_name)\
	m_mTags[HTML_TAG(tag_name)] = new struct_name<CHTML2OOXMLInterpretator>((CHTML2OOXMLInterpretator*)m_pInterpretator)

#define ADD_EMPTY_OOXML_TAG(tag_name, struct_name)\
	m_mTags[HTML_TAG(tag_name)] = new struct_name<CHTML2OOXMLInterpretator>()

CHTMLReader::CHTMLReader()
	: m_pInterpretator(new CHTML2OOXMLInterpretator())
{
	ADD_EMPTY_OOXML_TAG(B, TBold);
	ADD_EMPTY_OOXML_TAG(BR, TCenter);
	ADD_EMPTY_OOXML_TAG(I, TItalic);
	ADD_EMPTY_OOXML_TAG(CODE, TCode);
	ADD_EMPTY_OOXML_TAG(S, TStrike);
	ADD_EMPTY_OOXML_TAG(U, TUnderline);
	ADD_EMPTY_OOXML_TAG(MARK, TMark);
	ADD_EMPTY_OOXML_TAG(SUP, TSup);
	ADD_EMPTY_OOXML_TAG(SUB, TSub);
	ADD_EMPTY_OOXML_TAG(DD, TDD);
	ADD_EMPTY_OOXML_TAG(H1, THeader);

	ADD_OOXML_TAG(A, TAnchor);
	ADD_OOXML_TAG(ABBR, TAbbr);
	ADD_OOXML_TAG(BR, TBr);
	ADD_OOXML_TAG(BDO, TBidirectional);
	ADD_OOXML_TAG(Q, TQuotation);
	ADD_OOXML_TAG(SPAN, TSpan);
	ADD_OOXML_TAG(PRE, TPreformatted);
}

CHTMLReader::~CHTMLReader()
{
	if (nullptr != m_pInterpretator)
		delete m_pInterpretator;
}

HRESULT CHTMLReader::ConvertFromTo(const std::wstring& wsFrom, const std::wstring& wsTo)
{
	if (nullptr == m_pInterpretator || !HTML2XHTML(wsFrom) || !m_oLightReader.IsValid() || !IsHTML())
		return S_FALSE;

	m_pInterpretator->Begin(wsTo, nullptr);

	m_sSrc = NSSystemPath::GetDirectoryName(wsFrom);
	// m_sDst = sDst;

	ReadStyle();

	// Переходим в начало
	if(!m_oLightReader.MoveToStart())
		return S_FALSE;

	// if(oParams && oParams->m_bNeedPageBreakBefore)
	// 	m_internal->PageBreakBefore();

	ReadDocument();

	m_pInterpretator->End(wsTo);
	return S_OK;
}

bool CHTMLReader::IsHTML()
{
	return ((m_oLightReader.MoveToStart() && m_oLightReader.ReadNextNode()) ? m_oLightReader.GetName() == L"html" : false) && m_oLightReader.MoveToStart();
}

bool CHTMLReader::HTML2XHTML(const std::wstring& wsFileName)
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

	// const std::wstring sRes{htmlToXhtml(sFileContent, bNeedConvert)};

	#ifdef SAVE_NORMALIZED_HTML
	#if 1 == SAVE_NORMALIZED_HTML
	NSFile::CFileBinary oWriter;
	if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
	{
		oWriter.WriteStringUTF8(sRes);
		oWriter.CloseFile();
	}
	#endif
	#endif

	return m_oLightReader.FromString(UTF8_TO_U(sFileContent));
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
				m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());
			else
				ReadStyle();
		}
	}
}

void CHTMLReader::ReadStyle2()
{
	std::wstring sName = m_oLightReader.GetName();
	// Стиль по ссылке
	if(sName == L"link")
	{
		while(m_oLightReader.MoveToNextAttribute())
			ReadStyleFromNetwork();
		m_oLightReader.MoveToElement();
	}
	// тэг style содержит стили для styles.xml
	else if(sName == L"style")
		m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());

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

void CHTMLReader::ReadDocument()
{
	m_oLightReader.ReadNextNode();

	int nDeath = m_oLightReader.GetDepth();
	while(m_oLightReader.ReadNextSiblingNode(nDeath))
	{
		std::wstring sName = m_oLightReader.GetName();
		if(sName == L"head")
			ReadHead();
		else if(sName == L"body")
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
			m_sBase = GetArgumentValue(m_oLightReader, L"href");
	}

	m_oLightReader.MoveToElement();
}

void CHTMLReader::ReadBody()
{
	std::vector<NSCSS::CNode> arSelectors;

	arSelectors.push_back(NSCSS::CNode(L"html", L"", L""));

	GetSubClass(arSelectors);

	/*
	if (!sSelectors.back().m_mAttributes.empty())
	{
		std::map<std::wstring, std::wstring>::iterator itFound = sSelectors.back().m_mAttributes.find(L"bgcolor");

		if (sSelectors.back().m_mAttributes.end() != itFound)
		{
			NSCSS::NSProperties::CColor oColor;
			oColor.SetValue(itFound->second);

			if (!oColor.Empty() && !oColor.None())
			{
				const std::wstring wsHEXColor{oColor.ToHEX()};

				if (!wsHEXColor.empty())
					m_oDocXml.WriteString(L"<w:background w:color=\"" + wsHEXColor + L"\"/>");

				sSelectors.back().m_mAttributes.erase(itFound);
			}
		}
	}

	m_oLightReader.MoveToElement();
	*/

	ReadStream(arSelectors);
}

bool CHTMLReader::ReadStream(std::vector<NSCSS::CNode>& arSelectors, bool bInsertEmptyP)
{
	int nDeath = m_oLightReader.GetDepth();
	if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
	{
		if (!bInsertEmptyP)
			return false;

		m_pInterpretator->WriteEmptyParagraph(true, true);
		return true;

		// WriteEmptyParagraph(sSelectors, oTS);
		// if (bInsertEmptyP)
		// {
		// 	wrP(oXml, sSelectors, oTS);
		// 	wrRPr(oXml, sSelectors, oTS);
		// 	CloseP(oXml, sSelectors);
		// 	m_oState.m_bInP = false;

		// 	return true;
		// }
	}

	bool bResult = false;

	do
	{
		if (ReadInside(arSelectors))
			bResult = true;
	} while(m_oLightReader.ReadNextSiblingNode2(nDeath));

	if (!bResult && bInsertEmptyP)
		m_pInterpretator->WriteEmptyParagraph(true, true);

	return bResult;
}

bool CHTMLReader::ReadInside(std::vector<NSCSS::CNode>& arSelectors)
{
	const std::wstring wsName{m_oLightReader.GetName()};

	//TODO:: обработать все варианты return'а
	if(wsName == L"#text")
		return ReadText(arSelectors);

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
			bResult = ReadA(arSelectors);
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
			bResult = ReadBr(arSelectors);
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
		// case HTML_TAG(FONT):
		// {
		// 	bResult = m_pInterpretator->ReadFont(oXml, sSelectors, oTS);
		// 	break;
		// }
		// case HTML_TAG(IMG):
		// {
		// 	bResult = m_pInterpretator->readImage(oXml, sSelectors, oTS);
		// 	break;
		// }
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
		{
			bResult = ReadDefaultTag(HTML_TAG(SUP), arSelectors);
			break;
		}
		case HTML_TAG(SUB):
		{
			bResult = ReadDefaultTag(HTML_TAG(SUB), arSelectors);
			break;
		}
		// case HTML_TAG(SVG):
		// {
		// 	m_pInterpretator->readSVG(oXml, sSelectors, oTS);

		// 	wrP(oXml, sSelectors, oTS);
		// 	bResult = readSVG(m_oLightReader.GetOuterXml());

		// 	if (bResult)
		// 		ImageRels(oXml, -1, L"", L"png");

		// 	break;
		// }
		// case HTML_TAG(INPUT):
		// {
		// 	bResult = readInput(oXml, sSelectors, oTS);
		// 	break;
		// }
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
		// case HTML_TAG(BASEFONT):
		// {
		// 	bResult = ReadBasefont(oXml, sSelectors, oTS);
		// 	break;
		// }
		case HTML_TAG(BUTTON):
		case HTML_TAG(LABEL):
		case HTML_TAG(DATA):
		case HTML_TAG(OBJECT):
		case HTML_TAG(NOSCRIPT):
		case HTML_TAG(OUTPUT):
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
			m_pInterpretator->BeginBlock(arSelectors);

			switch(eHtmlTag)
			{
				case HTML_TAG(ADDRESS):
				{
					bResult = ReadDefaultTag(HTML_TAG(I), arSelectors);
					break;
				}
				case HTML_TAG(DD):
				{
					bResult = ReadDefaultTag(HTML_TAG(DD), arSelectors);
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
		// 		case HTML_TAG(ASIDE):
		// 		case HTML_TAG(DIV):
		// 		{
		// 			bResult = ReadDiv(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(BLOCKQUOTE):
		// 		{
		// 			bResult = ReadBlockquote(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(ARTICLE):
		// 		case HTML_TAG(HEADER):
		// 		case HTML_TAG(MAIN):
		// 		case HTML_TAG(SUMMARY):
		// 		case HTML_TAG(FOOTER):
		// 		case HTML_TAG(NAV):
		// 		case HTML_TAG(FIGCAPTION):
		// 		case HTML_TAG(FORM):
		// 		case HTML_TAG(OPTION):
		// 		case HTML_TAG(DT):
		// 		case HTML_TAG(P):
		// 		case HTML_TAG(SECTION):
		// 		case HTML_TAG(FIGURE):
		// 		case HTML_TAG(DL):
		// 		case HTML_TAG(LEGEND):
		// 		case HTML_TAG(MAP):
		// 		case HTML_TAG(H1):
		// 		case HTML_TAG(H2):
		// 		case HTML_TAG(H3):
		// 		case HTML_TAG(H4):
		// 		case HTML_TAG(H5):
		// 		case HTML_TAG(H6):
		// 		{
		// 			bResult = readStream(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(HR):
		// 		{
		// 			bResult = ReadHr(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(LI):
		// 		{
		// 			bResult = ReadListElement(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(OL):
		// 		case HTML_TAG(UL):
		// 		{
		// 			bResult = ReadList(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
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
		// 		case HTML_TAG(TABLE):
		// 		{
		// 			bResult = ParseTable(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(RUBY):
		// 		{
		// 			bResult = ParseRuby(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
		// 		case HTML_TAG(TEXTAREA):
		// 		case HTML_TAG(FIELDSET):
		// 		{
		// 			bResult = ReadTextarea(&oXmlData, sSelectors, oTS);
		// 			break;
		// 		}
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

			m_pInterpretator->EndBlock(bResult, arSelectors);
		}
	}

	// if (HTML_TAG(DIV) != eHtmlTag && HTML_TAG(ASIDE) != eHtmlTag)
	// {
	// 	if (bResult)
	// 		m_oState.m_eLastElement = eHtmlTag;

	// 	m_oState.m_bBanUpdatePageData = true;
	// }

	// readNote(oXml, sSelectors, sNote);
	arSelectors.pop_back();
	return bResult;
}

bool CHTMLReader::ReadText(std::vector<NSCSS::CNode>& arSelectors)
{
	if (nullptr == m_pInterpretator)
		return false;

	GetSubClass(arSelectors);

	const bool bResult{m_pInterpretator->WriteText(m_oLightReader.GetText(), arSelectors)};

	arSelectors.pop_back();

	return bResult;
}

bool CHTMLReader::ReadA(std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[HTML_TAG(A)]->Open(arSelectors))
		return false;

	if(!ReadStream(arSelectors))
		m_pInterpretator->WriteEmptyParagraph(true);

	m_mTags[HTML_TAG(A)]->Close(arSelectors);

	return true;
}

bool CHTMLReader::ReadBr(std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[HTML_TAG(BR)]->Open(arSelectors))
		return false;

	m_mTags[HTML_TAG(BR)]->Close(arSelectors);

	return true;
}


bool CHTMLReader::ReadDefaultTag(HtmlTag eTag, std::vector<NSCSS::CNode>& arSelectors)
{
	if (!m_mTags[eTag]->Open(arSelectors))
		return false;

	const bool bResult = ReadStream(arSelectors);

	m_mTags[eTag]->Close(arSelectors);

	return bResult;
}

void CHTMLReader::GetSubClass(std::vector<NSCSS::CNode>& arSelectors)
{
	NSCSS::CNode oNode;

	oNode.m_wsName = m_oLightReader.GetName();
	// Стиль по атрибуту
	while(m_oLightReader.MoveToNextAttribute())
	{
		std::wstring sName  = m_oLightReader.GetName();
		if(sName == L"class")
			oNode.m_wsClass  = EncodeXmlString(m_oLightReader.GetText());
		else if(sName == L"id")
		{
			oNode.m_wsId = EncodeXmlString(m_oLightReader.GetText());
			// WriteEmptyBookmark(oXml, oNode.m_wsId);

			if (!m_oStylesCalculator.HaveStylesById(oNode.m_wsId))
				oNode.m_wsId.clear();
		}
		else if(sName == L"style")
			oNode.m_wsStyle += m_oLightReader.GetText();
		else
		{
			if (CheckArgumentMath(oNode.m_wsName, sName))
				oNode.m_mAttributes[sName] = m_oLightReader.GetText();
		}
	}
	m_oLightReader.MoveToElement();
	arSelectors.push_back(oNode);

	m_oStylesCalculator.CalculateCompiledStyle(arSelectors);
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

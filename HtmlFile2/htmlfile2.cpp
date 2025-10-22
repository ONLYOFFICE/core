#include "htmlfile2.h"

#include <string>
#include <cwctype>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

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
#include "../DesktopEditor/common/ProcessEnv.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"

#include "htmlfile2.h"
#include "src/Languages.h"

#include <boost/regex.hpp>

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

#define MAXCOLUMNSINTABLE 63
#define MAXROWSINTABLE    32767

#define DEFAULT_PAGE_WIDTH  12240 // Значение в Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Значение в Twips

#define DEFAULT_LANGUAGE std::wstring(L"en-US")
#define DEFAULT_FONT_FAMILY std::wstring(L"Times New Roman")
#define DEFAULT_FONT_SIZE 24

#define DEFAULT_IMAGE_WIDTH  304800
#define DEFAULT_IMAGE_HEIGHT 304800

#define SAVE_NORMALIZED_HTML 0

#define RELEASE_VECTOR_PTR(vector_object, object_type) \
	for (object_type* pElement : vector_object) \
		RELEASEOBJECT(pElement) \

const static double HTML_FONTS[7] = {7.5, 10, 12, 13.5, 18, 24, 36};

#define HTML_TAG(tag) GUMBO_TAG_##tag
#define ADD_TAG(strName, enumName) {strName, HTML_TAG(enumName)}
#define SKIP_TAG SCRIPT
#define UNKNOWN_TAG GumboTag::GUMBO_TAG_UNKNOWN

#define HtmlTag GumboTag

#define MAX_STRING_BLOCK_SIZE (size_t)10485760

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

bool TagIsUnprocessed(const std::wstring& wsTagName)
{
	return L"xml" == wsTagName;
}

static inline HtmlTag GetHtmlTag(const std::wstring& wsStrTag)
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

static inline void WriteToStringBuilder(NSStringUtils::CStringBuilder& oSrcStringBuilder, NSStringUtils::CStringBuilder& oDstStringBuilder)
{
	if (oSrcStringBuilder.GetCurSize() < MAX_STRING_BLOCK_SIZE)
	{
		oDstStringBuilder.Write(oSrcStringBuilder);
		return;
	}

	size_t ulSize = oSrcStringBuilder.GetCurSize();
	size_t ulCurrentBlockSize = 0, ulPosition = 0;

	while (ulSize > 0)
	{
		ulCurrentBlockSize = std::min(ulSize, MAX_STRING_BLOCK_SIZE);
		oDstStringBuilder.WriteString(oSrcStringBuilder.GetSubData(ulPosition, ulCurrentBlockSize));

		ulSize -= ulCurrentBlockSize;
		ulPosition += ulCurrentBlockSize;
	}
}

// Ячейка таблицы
struct CTc
{
	int i;
	int j;
	std::wstring sGridSpan = L"1";
	std::wstring sPr = L"";

	CTc(int _i, int _j, const std::wstring& sColspan, const std::wstring& sTcPr = L"") 
		: i(_i), j(_j), sGridSpan(sColspan), sPr(sTcPr) {}

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
	bool bQ;   // Цитата
	bool bAddSpaces; // Добавлять пробелы перед текстом?
	bool bMergeText; // Объединять подяр идущий текст в 1?
	int  nLi;  // Уровень списка
	bool bNumberingLi; // Является ли список нумерованным
	bool bWritedLi; // Записан ли уже w:numPr

	std::wstring sPStyle;

	enum ETextMode
	{
		Normal,
		Superscript,
		Subscript
	} eTextMode;

	NSCSS::CCompiledStyle oAdditionalStyle;

	CTextSettings()
		: bBdo(false), bPre(false), bQ(false), bAddSpaces(true), bMergeText(false), nLi(-1), bNumberingLi(false), bWritedLi(false), eTextMode(Normal)
	{}

	CTextSettings(const CTextSettings& oTS) :
		bBdo(oTS.bBdo), bPre(oTS.bPre), bQ(oTS.bQ), bAddSpaces(oTS.bAddSpaces), bMergeText(oTS.bMergeText),
	    nLi(oTS.nLi), bNumberingLi(oTS.bNumberingLi), bWritedLi(oTS.bWritedLi), sPStyle(oTS.sPStyle), eTextMode(oTS.eTextMode)
	{}

	void AddPStyle(const std::wstring& wsStyle)
	{
		if (std::wstring::npos == sPStyle.find(wsStyle))
			sPStyle += wsStyle;
	}
};

struct TImageData
{
	UINT m_unWidth;
	UINT m_unHeight;

	int m_nHSpace;
	int m_nVSpace;

	std::wstring m_wsAlign;

	TImageData()
	    : m_unWidth(0), m_unHeight(0), m_nHSpace(0), m_nVSpace(0), m_wsAlign(L"left")
	{}

	bool ZeroSize() const
	{
		return 0 == m_unWidth || 0 == m_unHeight;
	}

	bool ZeroSpaces() const
	{
		return 0 == m_nHSpace && 0 == m_nVSpace;
	}
};

void WriteEmptyParagraph(NSStringUtils::CStringBuilder* pXml, bool bVahish = false, bool bInP = false)
{
	if (NULL == pXml)
		return;

	if (!bInP)
		pXml->WriteString(L"<w:p><w:pPr>");

	pXml->WriteString(L"<w:r><w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/>");

	if (bVahish)
		pXml->WriteString(L"<w:vanish/>");

	pXml->WriteString(L"</w:rPr></w:r>");

	if (!bInP)
		pXml->WriteString(L"</w:pPr></w:p>");
}

void WriteLine(NSStringUtils::CStringBuilder* pXml, const std::wstring& wsAlign, const std::wstring& wsColor, bool bShade, double dSize, double dWidth)
{
	if (dWidth < 0)
		dWidth = -dWidth;

	if (dSize < 0)
		dSize = -dSize;

	if (dWidth > 100)
		dWidth = 0;

	pXml->WriteNodeBegin(L"w:pict");
	pXml->WriteString(L"<v:rect style=\"height:" + std::to_wstring(dSize) + L"pt\"" + ((0. != dWidth) ? (L" o:hrpct=\"" + std::to_wstring((int)(10. * dWidth)) + L"\"") : L"") + L" o:hralign=\"" + wsAlign + L"\" o:hrstd=\"t\" " + ((bShade) ? L"o:hrnoshade=\"t\" " : L"") + L"o:hr=\"t\" fillcolor=\"#" + wsColor + L"\" stroked=\"f\"/>");
	pXml->WriteNodeEnd(L"w:pict");
}

bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}

UINT GetFontSizeLevel(UINT unFontSize)
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

UINT GetFontSizeByLevel(UINT unLevel)
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

int CalculateFontChange(const std::vector<NSCSS::CNode>& arSelectors)
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

#define FIRST_ELEMENT 0x00000001
#define LAST_ELEMENT  0x00000002
#define MID_ELEMENT   0x00000004

#define PARSE_MODE_HEADER  0x00000100
#define PARSE_MODE_BODY    0x00000200
#define PARSE_MODE_FOOTHER 0x00000400

#define COL_POSITION_MASK 0x0000000F
#define ROW_POSITION_MASK 0x000000F0
#define PARSE_MODE_MASK   0x00000F00

typedef enum
{
	ParseModeHeader,
	ParseModeBody,
	ParseModeFoother
} ERowParseMode;

//Необходимые стили таблицы
struct TTableStyles
{
	NSCSS::NSProperties::CIndent m_oPadding;
	NSCSS::NSProperties::CIndent m_oMargin;
	NSCSS::NSProperties::CBorder m_oBorder;
	NSCSS::NSProperties::CDigit  m_oWidth;

	int  m_nCellSpacing;

	std::wstring m_wsAlign;

	enum ETableRules
	{
		All,
		Groups,
		Cols,
		None,
		Rows
	} m_enRules;

	TTableStyles()
		: m_nCellSpacing(-1), m_enRules(None)
	{}

	bool Empty() const
	{
		return m_oPadding.Empty() && m_oMargin.Empty() && m_oBorder.Empty() && m_oWidth.Empty() && -1 == m_nCellSpacing && m_wsAlign.empty();
	}
};

std::wstring CreateBorders(const NSCSS::NSProperties::CBorder& oBorder, const NSCSS::NSProperties::CIndent* pPadding = NULL, bool bAddIntermediateLines = false, TTableStyles::ETableRules enTableRule = TTableStyles::ETableRules::None)
{
	std::wstring wsTable;

	if (oBorder.EqualSides() && (NULL == pPadding || pPadding->Equals()))
	{
		const std::wstring wsBorderStyle = NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))));

		wsTable = L"<w:top "    + wsBorderStyle + L"/>" + L"<w:left "   + wsBorderStyle + L"/>" +
		          L"<w:bottom " + wsBorderStyle + L"/>" + L"<w:right "  + wsBorderStyle + L"/>";
	}
	else
	{
		if (oBorder.GetTopBorder().Valid())
			wsTable += L"<w:top "    + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetTopBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetTop()))))       + L"/>";

		if (oBorder.GetLeftBorder().Valid())
			wsTable += L"<w:left "   + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))))     + L"/>";

		if (oBorder.GetBottomBorder().Valid())
			wsTable += L"<w:bottom " + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetBottomBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetBottom())))) + L"/>";

		if (oBorder.GetRightBorder().Valid())
			wsTable += L"<w:right "  + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetRightBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetRight()))))   + L"/>";
	}

	if (!bAddIntermediateLines)
		return wsTable;

	if (TTableStyles::ETableRules::Rows == enTableRule || TTableStyles::ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetBottomBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideH " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	if (TTableStyles::ETableRules::Cols == enTableRule || TTableStyles::ETableRules::All == enTableRule)
	{
		NSCSS::NSProperties::CBorderSide oNewSide(oBorder.GetRightBorder());
		oNewSide.SetWidth(L"1pt", 0, true);

		wsTable += L"<w:insideV " + NSCSS::CDocumentStyle::CalculateBorderStyle(oNewSide) + L"/>";
	}

	return wsTable;
}

std::wstring CreateBorders(const std::wstring& wsStyle, UINT unSize, UINT unSpace, const std::wstring& wsAuto)
{
	const std::wstring wsBodyBorder{L"w:val=\"" + wsStyle + L"\" w:sz=\"" + std::to_wstring(unSize) + L"\" w:space=\"" + std::to_wstring(unSpace) + L"\" w:color=\"" + wsAuto + L"\""};

	return L"<w:top "    + wsBodyBorder + L"/>" +
	       L"<w:left "   + wsBodyBorder + L"/>" +
	       L"<w:bottom " + wsBodyBorder + L"/>" +
	       L"<w:right "  + wsBodyBorder + L"/>";
}

#define CreateOutsetBorders(enType) CreateBorders(L"outset", 6, 0, L"auto", enType)

std::wstring CreateDefaultBorder(std::wstring wsSideName)
{
	std::transform(wsSideName.begin(), wsSideName.end(), wsSideName.begin(), std::towlower);
	return L"<w:" + wsSideName + L" w:val=\"single\" w:sz=\"1\" w:space=\"0\" w:color=\"auto\"/>";
}

struct TTableRowStyle
{
	UINT m_unMaxIndex;
	UINT m_unMaxHeight;
	bool m_bIsHeader;

	TTableRowStyle()
		: m_unMaxIndex(0), m_unMaxHeight(0), m_bIsHeader(false)
	{}

	bool Empty() const
	{
		return 0 == m_unMaxHeight && false == m_bIsHeader;
	}
};

struct TTableCellStyle
{
	NSCSS::NSProperties::CDigit  m_oWidth;
	NSCSS::NSProperties::CDigit  m_oHeight;
	NSCSS::NSProperties::CBorder m_oBorder;
	NSCSS::NSProperties::CIndent m_oPadding;
	NSCSS::NSProperties::CColor  m_oBackground;

	std::wstring m_wsHAlign;
	std::wstring m_wsVAlign;

	TTableCellStyle(){}

	bool Empty()
	{
		return m_oWidth.Empty() && m_oHeight.Empty() && m_oBorder.Empty() && m_oPadding.Empty() && m_wsVAlign.empty() && m_wsVAlign.empty();
	}

	void Copy(const TTableCellStyle* pTableCellStyle)
	{
		if (NULL == pTableCellStyle)
			return;

		m_oWidth      = pTableCellStyle->m_oWidth;
		m_oHeight     = pTableCellStyle->m_oHeight;
		m_oBorder     = pTableCellStyle->m_oBorder;
		m_oPadding    = pTableCellStyle->m_oPadding;
		m_oBackground = pTableCellStyle->m_oBackground;

		m_wsHAlign    = pTableCellStyle->m_wsHAlign;
		m_wsVAlign    = pTableCellStyle->m_wsVAlign;
	}

	TTableCellStyle& operator+=(const TTableCellStyle* pCellStyle)
	{
		if (NULL == pCellStyle)
			return *this;

		m_oWidth += pCellStyle->m_oWidth;
		m_oHeight += pCellStyle->m_oHeight;
		m_oBorder += pCellStyle->m_oBorder;
		m_oPadding += pCellStyle->m_oPadding;
		m_oBackground += pCellStyle->m_oBackground;

		if (m_wsHAlign.empty())
			m_wsHAlign = pCellStyle->m_wsHAlign;

		if (m_wsVAlign.empty())
			m_wsVAlign = pCellStyle->m_wsVAlign;

		return *this;
	}
};

class CTable;

class CTableCell
{
public:
	CTableCell() 
		: m_unColspan(1), m_unRowSpan(1), m_bIsMerged(false), m_bIsEmpty(false)
	{}

	CTableCell(UINT unColspan, UINT unRowspan, bool bIsMerged, bool bIsEmpty)
		: m_unColspan(unColspan), m_unRowSpan(unRowspan), m_bIsMerged(bIsMerged), m_bIsEmpty(bIsEmpty)
	{}

	CTableCell(CTableCell& oCell)
		: m_unColspan(oCell.m_unColspan), m_unRowSpan(oCell.m_unRowSpan), m_bIsMerged(oCell.m_bIsMerged), 
		  m_bIsEmpty(oCell.m_bIsEmpty), m_oStyles(oCell.m_oStyles)
	{
		WriteToStringBuilder(oCell.m_oData, m_oData);
	}

	bool Empty()
	{
		return m_bIsEmpty;
	}

	bool Merged()
	{
		return m_bIsMerged;
	}

	CTableCell* Copy()
	{
		return new CTableCell(*this);
	}

	static CTableCell* CreateEmpty(UINT unColspan = 1, bool m_bIsMerged = false, const TTableCellStyle* pStyle = NULL)
	{
		CTableCell *pCell = new CTableCell(unColspan, 1, m_bIsMerged, true);

		pCell->m_oStyles.Copy(pStyle);

		return pCell;
	}

	static CTableCell* CreateEmpty(const TTableCellStyle* pStyle)
	{
		CTableCell *pCell = new CTableCell(1, 1, false, true);

		pCell->m_oStyles.Copy(pStyle);

		return pCell;
	}

	void SetColspan(UINT unColspan, UINT unCurrentIndex)
	{
		if (MAXCOLUMNSINTABLE - 1 != unCurrentIndex)
			m_unColspan = std::min(MAXCOLUMNSINTABLE - 1 - unCurrentIndex, unColspan);
		else
			m_unColspan = 1;
	}

	UINT GetColspan() const
	{
		return m_unColspan;
	}

	void SetRowspan(UINT unRowspan)
	{
		m_unRowSpan = unRowspan;
	}

	UINT GetRowspan() const
	{
		return m_unRowSpan;
	}

	NSStringUtils::CStringBuilder* GetData()
	{
		return &m_oData;
	}

	const TTableCellStyle* GetStyles() const
	{
		return &m_oStyles;
	}

	TTableCellStyle* GetStyles()
	{
		return &m_oStyles;
	}

	void SetWidth(const NSCSS::NSProperties::CDigit& oWidth)
	{
		m_oStyles.m_oWidth = oWidth;
	}

	void SetHeight(const NSCSS::NSProperties::CDigit& oHeight)
	{
		m_oStyles.m_oHeight = oHeight;
	}

	UINT GetHeight() const
	{
		return m_oStyles.m_oHeight.ToInt(NSCSS::Twips, DEFAULT_PAGE_HEIGHT);
	}

	void SetBorder(const NSCSS::NSProperties::CBorder& oBorder)
	{
		m_oStyles.m_oBorder = oBorder;
	}

	void ClearTopBorder()
	{
		m_oStyles.m_oBorder.SetTopSide(L"none", 0, true);
	}

	void ClearLeftBorder()
	{
		m_oStyles.m_oBorder.SetLeftSide(L"none", 0, true);
	}

	void ClearBottomBorder()
	{
		m_oStyles.m_oBorder.SetBottomSide(L"none", 0, true);
	}

	void ClearRightBorder()
	{
		m_oStyles.m_oBorder.SetRightSide(L"none", 0, true);
	}

	void SetPadding(const NSCSS::NSProperties::CIndent& oPadding)
	{
		m_oStyles.m_oPadding = oPadding;
	}

	void SetHAlign(const std::wstring& wsAlign)
	{
		m_oStyles.m_wsHAlign = wsAlign;
	}

	void SetVAlign(const std::wstring& wsAlign)
	{
		m_oStyles.m_wsVAlign = wsAlign;
	}

	void SetBackground(const NSCSS::NSProperties::CColor& oColor)
	{
		m_oStyles.m_oBackground = oColor;
	}

	std::wstring ConvertToOOXML(const CTable& oTable, UINT unColumnNumber, int nInstruction);
private:
	UINT m_unColspan;
	UINT m_unRowSpan;

	bool m_bIsMerged;
	bool m_bIsEmpty;

	TTableCellStyle m_oStyles;
	NSStringUtils::CStringBuilder m_oData;
};

class CTableRow
{
public:
	CTableRow()
	{}

	~CTableRow()
	{
		for (CTableCell* pCell : m_arCells)
			RELEASEOBJECT(pCell);
	}

	void AddCell(CTableCell* pCell)
	{
		InsertCell(pCell, -1);
	}

	void InsertCell(CTableCell *pCell, int nPosition)
	{
		if (NULL == pCell)
			return;

		if (nPosition < 0)
		{
			std::vector<CTableCell*>::iterator itFoundEmpty = std::find_if(m_arCells.begin(), m_arCells.end(), [](CTableCell* pCell) { return pCell->Empty() && !pCell->Merged(); });

			if (m_arCells.end() != itFoundEmpty)
			{
				--m_oStyles.m_unMaxIndex;
				delete *itFoundEmpty;
				*itFoundEmpty = pCell;
				
				if (1 != pCell->GetColspan())
				{
					++itFoundEmpty;
					UINT unColspan = pCell->GetColspan() - 1;
	
					while (m_arCells.end() != itFoundEmpty && (*itFoundEmpty)->Empty() && unColspan > 0)
					{
						--m_oStyles.m_unMaxIndex;
						--unColspan;
						delete (*itFoundEmpty);
						itFoundEmpty = m_arCells.erase(itFoundEmpty);
					}
	
					if (unColspan != 0)
						pCell->SetColspan(pCell->GetColspan() - unColspan, MAXCOLUMNSINTABLE);
				}
			}
			else
				m_arCells.push_back(pCell);
		}
		else if (nPosition >= m_arCells.size())
		{
			const UINT unMissingCount = nPosition - m_arCells.size();

			for (UINT unIndex = 0; unIndex < unMissingCount; ++unIndex)
				m_arCells.push_back(CTableCell::CreateEmpty());

			m_oStyles.m_unMaxIndex += unMissingCount;

			m_arCells.push_back(pCell);
		}
		else if (m_arCells[nPosition]->Empty())
		{
			delete m_arCells[nPosition];
			--m_oStyles.m_unMaxIndex;
			m_arCells[nPosition] = pCell;

			if (1 != pCell->GetColspan())
			{
				++nPosition;
				UINT unDeleteCount =  pCell->GetColspan() - 1;
				while (nPosition < m_arCells.size() && m_arCells[nPosition]->Empty() && !m_arCells[nPosition]->Merged() && unDeleteCount > 0)
				{
					delete m_arCells[nPosition];
					--m_oStyles.m_unMaxIndex;
					m_arCells.erase(m_arCells.begin() + nPosition);
					--unDeleteCount;
				}

				if (0 != unDeleteCount)
					pCell->SetColspan(pCell->GetColspan() - unDeleteCount, MAXCOLUMNSINTABLE);
			}
		}
		else
			m_arCells.insert(m_arCells.begin() + nPosition, pCell);

		m_oStyles.m_unMaxIndex += pCell->GetColspan();

		if (1 == pCell->GetColspan() && 1 == pCell->GetRowspan())
			m_oStyles.m_unMaxHeight = std::max(m_oStyles.m_unMaxHeight, pCell->GetHeight());
	}

	UINT GetIndex() const
	{
		return m_oStyles.m_unMaxIndex;
	}

	UINT GetCount() const
	{
		return m_arCells.size();
	}

	std::wstring ConvertToOOXML(const CTable& oTable, int nInstruction);

	CTableCell* operator[](UINT unIndex)
	{
		if (unIndex >= m_arCells.size())
			return NULL;

		return m_arCells[unIndex];
	}
private:
	TTableRowStyle m_oStyles;
	std::vector<CTableCell*> m_arCells;
};

class CTableCol
{
public:
	CTableCol(UINT unSpan)
		: m_unSpan(unSpan)
	{}

	CTableCol(XmlUtils::CXmlLiteReader& oLiteReader)
		: m_unSpan(1)
	{
		while (oLiteReader.MoveToNextAttribute())
		{
			if (L"span" == oLiteReader.GetName())
				m_unSpan = NSStringFinder::ToInt(oLiteReader.GetText(), 1);
		}

		oLiteReader.MoveToElement();
	}

	UINT GetSpan() const
	{
		return m_unSpan;
	}

	TTableCellStyle* GetStyle()
	{
		return &m_oStyle;
	}

	const TTableCellStyle* GetStyle() const
	{
		return &m_oStyle;
	}
private:
	UINT m_unSpan;
	TTableCellStyle m_oStyle;
};

class CTableColgroup
{
public:
	CTableColgroup(XmlUtils::CXmlLiteReader& oLiteReader)
		: m_unWidth(0)
	{
		while (oLiteReader.MoveToNextAttribute())
		{
			if (L"width" == oLiteReader.GetName())
				m_unWidth = NSStringFinder::ToInt(oLiteReader.GetText());
		}

		oLiteReader.MoveToElement();
	}

	~CTableColgroup()
	{
		RELEASE_VECTOR_PTR(m_arCols, CTableCol)
	}

	bool Empty() const
	{
		return m_arCols.empty();
	}

	void AddCol(CTableCol* pCol)
	{
		if (NULL != pCol)
			m_arCols.push_back(pCol);
	}

	const std::vector<CTableCol*>& GetCols() const
	{
		return m_arCols;
	}
private:
	std::vector<CTableCol*> m_arCols;
	UINT                    m_unWidth;
};

class CTable
{
public:
	CTable()
	    : m_unMaxColumns(0)
	{}

	~CTable()
	{
		for (std::vector<CTableRow*>& arHeaders : m_arHeaders)
			RELEASE_VECTOR_PTR(arHeaders, CTableRow)

		RELEASE_VECTOR_PTR(m_arFoother, CTableRow)
		RELEASE_VECTOR_PTR(m_arRows, CTableRow)
		RELEASE_VECTOR_PTR(m_arColgroups, CTableColgroup)
	}

	CTableRow* operator[](UINT unIndex)
	{
		if (unIndex < m_arRows.size())
			return m_arRows[unIndex];

		return NULL;
	}

	bool Empty() const
	{
		return m_arRows.empty();
	}

	bool HaveCaption()
	{
		return 0 != m_oCaption.GetCurSize();
	}

	bool HaveColgroups() const
	{
		return !m_arColgroups.empty();
	}

	UINT GetRowCount() const
	{
		return m_arRows.size();
	}

	TTableStyles GetTableStyles() const
	{
		return m_oStyles;
	}

	const TTableCellStyle* GetColStyle(UINT unColumnNumber) const
	{
		if (m_arColgroups.empty())
			return NULL;

		UINT unCurrentNumber = 0;

		for (const CTableColgroup* pColgroup : m_arColgroups)
		{
			for (const CTableCol* pCol : pColgroup->GetCols())
			{
				unCurrentNumber += pCol->GetSpan();

				if (unCurrentNumber >= unColumnNumber)
					return pCol->GetStyle();
			}
		}

		return NULL;
	}

	std::wstring CalculateSidesToClean(UINT unColumnNumber) const
	{
		if (m_arColgroups.empty())
			return std::wstring();

		UINT unCurrentNumber = 0;

		for (const CTableColgroup* pColgroup : m_arColgroups)
		{
			for (const CTableCol* pCol : pColgroup->GetCols())
			{
				if (unCurrentNumber + 1 == unCurrentNumber)
					return (1 != pCol->GetSpan()) ? L"<w:right w:val=\"nil\"/>" : std::wstring();

				unCurrentNumber += pCol->GetSpan();

				if (unColumnNumber == unCurrentNumber)
					return (1 != pCol->GetSpan()) ? L"<w:left w:val=\"nil\"/>" : std::wstring();
				else if (unColumnNumber < unCurrentNumber)
					return std::wstring((1 != unColumnNumber) ? L"<w:left w:val=\"nil\"/>" : L"") + std::wstring((m_unMaxColumns != unColumnNumber) ? L"<w:right w:val=\"nil\"/>" : L"");
			}
		}

		return std::wstring();
	}

	void AddRows(std::vector<CTableRow*>& m_arRows, ERowParseMode eParseMode = ERowParseMode::ParseModeBody)
	{
		if (m_arRows.empty())
			return;

		if (ERowParseMode::ParseModeFoother == eParseMode && !m_arFoother.empty())
			eParseMode = ERowParseMode::ParseModeHeader;

		if (ERowParseMode::ParseModeHeader == eParseMode)
			m_arHeaders.push_back({});

		for (CTableRow* pRow : m_arRows)
			AddRow(pRow, eParseMode);
	}

	void AddRow(CTableRow* pRow, ERowParseMode eParseMode = ERowParseMode::ParseModeBody)
	{
		if (NULL == pRow)
			return;

		for (UINT unIndex = 0; unIndex < pRow->GetCount(); ++unIndex)
		{
			if (unIndex >= m_arMinColspan.size())
				m_arMinColspan.push_back((*pRow)[unIndex]->GetColspan());
			else if ((*pRow)[unIndex]->GetColspan() < m_arMinColspan[unIndex])
				m_arMinColspan[unIndex] = (*pRow)[unIndex]->GetColspan();
		}

		switch (eParseMode)
		{
			default:
			case ERowParseMode::ParseModeBody:
			{
				m_arRows.push_back(pRow);
				break;
			}
			case ERowParseMode::ParseModeHeader:
			{
				if (m_arHeaders.empty())
					m_arHeaders.push_back({});

				m_arHeaders.back().push_back(pRow);
				break;
			}
			case ERowParseMode::ParseModeFoother:
			{
				m_arFoother.push_back(pRow);
				break;
			}
		}
	}

	void AddCaption(NSStringUtils::CStringBuilder& oCaption)
	{
		WriteToStringBuilder(oCaption, m_oCaption);
	}

	void SetPadding(const NSCSS::NSProperties::CIndent& oPadding)
	{
		m_oStyles.m_oPadding = oPadding;
	}

	void SetMargin(const NSCSS::NSProperties::CIndent& oMargin)
	{
		m_oStyles.m_oMargin = oMargin;
	}

	const NSCSS::NSProperties::CIndent& GetPadding() const
	{
		return m_oStyles.m_oPadding;
	}

	void SetBorder(const NSCSS::NSProperties::CBorder& oBorder)
	{
		m_oStyles.m_oBorder = oBorder;
	}

	void SetWidth(const NSCSS::NSProperties::CDigit& oWidth)
	{
		m_oStyles.m_oWidth = oWidth;
	}

	void SetCellSpacing(int nCellSpacing)
	{
		m_oStyles.m_nCellSpacing = nCellSpacing;
	}

	void SetAlign(const std::wstring& wsValue)
	{
		m_oStyles.m_wsAlign = wsValue;
	}

	void SetRules(const std::wstring& wsValue)
	{
		if (wsValue.empty())
			return;

		if (NSStringFinder::Equals(wsValue, L"all"))
			m_oStyles.m_enRules = TTableStyles::ETableRules::All;
		else if (NSStringFinder::Equals(wsValue, L"groups"))
			m_oStyles.m_enRules = TTableStyles::ETableRules::Groups;
		else if (NSStringFinder::Equals(wsValue, L"cols"))
			m_oStyles.m_enRules = TTableStyles::ETableRules::Cols;
		else if (NSStringFinder::Equals(wsValue, L"none"))
			m_oStyles.m_enRules = TTableStyles::ETableRules::None;
		else if (NSStringFinder::Equals(wsValue, L"rows"))
			m_oStyles.m_enRules = TTableStyles::ETableRules::Rows;
	}

	void AddColgroup(CTableColgroup* pElement)
	{
		if (NULL != pElement)
			m_arColgroups.push_back(pElement);
	}

	void RecalculateMaxColumns()
	{
		for (const CTableRow* pRow : m_arRows)
			m_unMaxColumns = std::max(m_unMaxColumns, pRow->GetIndex());
	}

	void Shorten()
	{
		UINT unIndex      = 0;
		CTableCell* pCell = NULL;

		UINT unMaxIndex = 0; //Максимальный индекс без учета строк, где имеется только 1 ячейка

		for (const CTableRow* pRow : m_arRows)
		{
			if (1 < pRow->GetCount())
				unMaxIndex = std::max(unMaxIndex, pRow->GetIndex());
		}

		while (unIndex < m_arMinColspan.size())
		{
			for (CTableRow* pRow : m_arRows)
			{
				if (0 != unMaxIndex && 1 == pRow->GetCount() && pRow->GetIndex() > unMaxIndex)
				{
					pCell = (*pRow)[unIndex];
	
					if (NULL == pCell)
						continue;

					pCell->SetColspan(unMaxIndex, MAXCOLUMNSINTABLE);
					continue;
				}

				if (1 == m_arMinColspan[unIndex])
					break;

				pCell = (*pRow)[unIndex];

				if (NULL == pCell)
					continue;

				if (1 < pCell->GetColspan() && pCell->GetColspan() > m_arMinColspan[unIndex])
				{
					pCell->SetColspan(m_arMinColspan[unIndex], MAXCOLUMNSINTABLE);
					continue;
				}

				if ((*pRow)[unIndex]->GetColspan() == m_arMinColspan[unIndex] + 1)
					(*pRow)[unIndex]->SetColspan(2, MAXCOLUMNSINTABLE);
				else if ((*pRow)[unIndex]->GetColspan() > m_arMinColspan[unIndex])
					(*pRow)[unIndex]->SetColspan((*pRow)[unIndex]->GetColspan() - m_arMinColspan[unIndex], MAXCOLUMNSINTABLE);
			}

			++unIndex;
		}
	}

	void CompleteTable()
	{
		UINT unMaxIndex = 0;

		for (CTableRow* pRow : m_arRows)
			unMaxIndex = std::max(unMaxIndex, pRow->GetIndex());

		for (CTableRow* pRow : m_arRows)
		{
			if (NULL == pRow || 0 == pRow->GetCount())
				continue;

			for (UINT unIndex = pRow->GetIndex(); unIndex < unMaxIndex; ++unIndex)
				pRow->InsertCell(CTableCell::CreateEmpty(), unIndex);
		}

		RecalculateMaxColumns();
	}

	bool ConvertToOOXML(NSStringUtils::CStringBuilder& oStringBuilder)
	{
		if (m_arRows.empty())
			return false;

		oStringBuilder.WriteNodeBegin(L"w:tbl");
		oStringBuilder.WriteNodeBegin(L"w:tblPr");

		if (!m_oStyles.m_oWidth.Empty() && !m_oStyles.m_oWidth.Zero())
		{
			if (NSCSS::UnitMeasure::Percent == m_oStyles.m_oWidth.GetUnitMeasure())
				oStringBuilder += L"<w:tblW w:w=\"" + std::to_wstring(m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oStringBuilder += L"<w:tblW w:w=\"" + std::to_wstring(m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}
		else
			oStringBuilder += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

		if (!m_oStyles.m_oMargin.GetLeft().Empty() && !m_oStyles.m_oMargin.GetLeft().Zero())
		{
			if (NSCSS::UnitMeasure::Percent == m_oStyles.m_oMargin.GetLeft().GetUnitMeasure())
				oStringBuilder += L"<w:tblInd w:w=\"" + std::to_wstring(m_oStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oStringBuilder += L"<w:tblInd w:w=\"" + std::to_wstring(m_oStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}

		if (!m_oStyles.m_wsAlign.empty())
			oStringBuilder += L"<w:jc w:val=\"" + m_oStyles.m_wsAlign + L"\"/>";

		if (0 < m_oStyles.m_nCellSpacing && m_oStyles.m_oBorder.GetCollapse() != NSCSS::NSProperties::BorderCollapse::Collapse)
			oStringBuilder += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(m_oStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";

		if (!m_oStyles.m_oBorder.Empty() && !m_oStyles.m_oBorder.Zero())
			oStringBuilder += L"<w:tblBorders>" + CreateBorders(m_oStyles.m_oBorder, NULL, true, (TTableStyles::ETableRules::Groups == m_oStyles.m_enRules && !m_arColgroups.empty()) ? TTableStyles::ETableRules::Cols : m_oStyles.m_enRules) + L"</w:tblBorders>";

		if (!m_oStyles.m_oPadding.Empty() && !m_oStyles.m_oPadding.Zero())
		{
			const int nTopPadding    = std::max(0, m_oStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nLeftPadding   = std::max(0, m_oStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
			const int nBottomPadding = std::max(0, m_oStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nRightPadding  = std::max(0, m_oStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));

			oStringBuilder.WriteNodeBegin(L"w:tblCellMar");

			if (0 != nTopPadding)
				oStringBuilder += L"<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>";

			if (0 != nLeftPadding)
				oStringBuilder += L"<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>";

			if (0 != nBottomPadding)
				oStringBuilder += L"<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>";

			if (0 != nRightPadding)
				oStringBuilder += L"<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>";

			oStringBuilder.WriteNodeEnd(L"w:tblCellMar");
		}
		else
			oStringBuilder += L"<w:tblCellMar><w:top w:w=\"15\" w:type=\"dxa\"/><w:left w:w=\"15\" w:type=\"dxa\"/><w:bottom w:w=\"15\" w:type=\"dxa\"/><w:right w:w=\"15\" w:type=\"dxa\"/></w:tblCellMar>";

		oStringBuilder += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
		oStringBuilder.WriteNodeEnd(L"w:tblPr");

		if (HaveCaption())
		{
			oStringBuilder.WriteNodeBegin(L"w:tr");
			oStringBuilder.WriteNodeBegin(L"w:tc");
			oStringBuilder.WriteNodeBegin(L"w:tcPr");
			oStringBuilder += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";
			oStringBuilder += L"<w:gridSpan w:val=\"" + std::to_wstring(m_unMaxColumns) + L"\"/>";
			oStringBuilder += L"<w:tcBorders><w:top w:val=\"nil\"/><w:left w:val=\"nil\"/><w:bottom w:val=\"nil\"/><w:right w:val=\"nil\"/></w:tcBorders>";
			oStringBuilder += L"<w:vAlign w:val=\"center\"/>";
			oStringBuilder += L"<w:hideMark/>";
			oStringBuilder.WriteNodeEnd(L"w:tcPr");
			WriteToStringBuilder(m_oCaption, oStringBuilder);
			oStringBuilder.WriteNodeEnd(L"w:tc");
			oStringBuilder.WriteNodeEnd(L"w:tr");
		}

		#define CONVERT_ROWS(rows, mode) \
		{ \
			for (UINT unRowIndex = 0; unRowIndex < rows.size(); ++unRowIndex) \
			{ \
				int nInstruction = 0; \
				if (0 == unRowIndex) \
					nInstruction |= FIRST_ELEMENT << 4; \
				if (rows.size() - 1 == unRowIndex) \
					nInstruction |= LAST_ELEMENT << 4; \
				else if (0 != unRowIndex) \
					nInstruction |= MID_ELEMENT << 4; \
				nInstruction |= mode; \
				oStringBuilder += rows[unRowIndex]->ConvertToOOXML(*this, nInstruction); \
			} \
		}

		for (std::vector<CTableRow*>& arRows : m_arHeaders)
			CONVERT_ROWS(arRows, PARSE_MODE_HEADER)

		CONVERT_ROWS(m_arRows,    PARSE_MODE_BODY)
		CONVERT_ROWS(m_arFoother, PARSE_MODE_FOOTHER)

		oStringBuilder.WriteNodeEnd(L"w:tbl");

		return true;
	}
private:
	std::vector<std::vector<CTableRow*>> m_arHeaders;
	std::vector<CTableRow*>              m_arFoother;
	std::vector<CTableRow*>              m_arRows;

	std::vector<UINT> m_arMinColspan;

	NSStringUtils::CStringBuilder m_oCaption;

	std::vector<CTableColgroup*> m_arColgroups;

	TTableStyles m_oStyles;

	UINT m_unMaxColumns;
};

enum class EAbstructNumType
{
	Bullet,
	CardinalText,
	Chicago,
	Decimal,
	DecimalEnclosedCircle,
	DecimalEnclosedFullstop,
	DecimalEnclosedParen,
	DecimalZero,
	LowerLetter,
	LowerRoman,
	None,
	OrdinalText,
	UpperLetter,
	UpperRoman
};

void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
	size_t pos = s.find(s1);
	size_t l = s2.length();
	while (pos != std::string::npos)
	{
		if (!(s1 == L"&" && s2 == L"&amp;" && s.length() > pos + 4 && s[pos] == L'&' && s[pos + 1] == L'a' && s[pos + 2] == L'm' && s[pos + 3] == L'p' && s[pos + 4] == L';'))
			s.replace(pos, s1.length(), s2);
		pos = s.find(s1, pos + l);
	}
}

void ReplaceSpaces(std::wstring& wsValue)
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

std::wstring EncodeXmlString(const std::wstring& s)
{
	std::wstring sRes = s;

	replace_all(sRes, L"&", L"&amp;");
	replace_all(sRes, L"<", L"&lt;");
	replace_all(sRes, L">", L"&gt;");
	replace_all(sRes, L"\"", L"&quot;");
	replace_all(sRes, L"\'", L"&#39;");
	replace_all(sRes, L"\n", L"&#xA;");
	replace_all(sRes, L"\r", L"&#xD;");
	replace_all(sRes, L"\t", L"&#x9;");

	return sRes;
}

bool GetStatusUsingExternalLocalFiles()
{
	if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
		return NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

	return true;
}

bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, bool bIsAllowExternalLocalFiles)
{
	if (bIsAllowExternalLocalFiles)
		return true;

	if (!wsCorePath.empty())
	{
		const std::wstring wsFullPath = NSSystemPath::ShortenPath(NSSystemPath::Combine(wsSrcPath, wsPath));
		return boost::starts_with(wsFullPath, wsCorePath);
	}

	if (wsPath.length() >= 3 && L"../" == wsPath.substr(0, 3))
		return false;

	return true;
}

class CHtmlFile2_Private
{
public:
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator m_oStylesCalculator; // Css калькулятор
	NSCSS::CDocumentStyle m_oXmlStyle;         // Ooxml стиль

	NSCSS::NSProperties::CPage m_oPageData; // Стили страницы

	std::wstring m_sTmp;  // Temp папка
	std::wstring m_sSrc;  // Директория источника
	std::wstring m_sDst;  // Директория назначения
	std::wstring m_sBase; // Полный базовый адрес
	std::wstring m_sCore; // Путь до корневого файла (используется для работы с Epub)

private:
	int m_nFootnoteId;  // ID сноски
	int m_nHyperlinkId; // ID ссылки
	int m_nNumberingId; // ID списка
	int m_nId;          // ID остальные элементы

	NSStringUtils::CStringBuilder m_oStylesXml;   // styles.xml
	NSStringUtils::CStringBuilder m_oDocXmlRels;  // document.xml.rels
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels
	NSStringUtils::CStringBuilder m_oDocXml;      // document.xml
	NSStringUtils::CStringBuilder m_oNoteXml;     // footnotes.xml
	NSStringUtils::CStringBuilder m_oNumberXml;   // numbering.xml
	NSStringUtils::CStringBuilder m_oWebSettings; // webSettings.xml

	struct TState
	{
		bool m_bInP;         // <w:p> открыт?
		bool m_bInR;         // <w:r> открыт?
		bool m_bInT;         // <w:t> открыт?
		bool m_bWasPStyle;   // <w:pStyle> записан?
		bool m_bWasSpace;    // Был пробел?
		bool m_bInHyperlink; // <w:hyperlink> открыт?

		bool m_bBanUpdatePageData; // Запретить обновление данных о странице?

		TState()
			: m_bInP(false), m_bInR(false), m_bInT(false), m_bWasPStyle(false), m_bWasSpace(true), m_bInHyperlink(false), m_bBanUpdatePageData(false)
		{}
	} m_oState;

	std::vector<std::wstring>            m_arrImages;  // Картинки
	std::map<std::wstring, std::wstring> m_mFootnotes; // Сноски
	std::map<std::wstring, UINT>         m_mBookmarks; // Закладки
	std::map<std::wstring, UINT>         m_mDivs;      // Div элементы

	NSFonts::IApplicationFonts*          m_pFonts;     // Необходимо для оптимизации работы со шрифтами
public:

	CHtmlFile2_Private() 
		: m_nFootnoteId(1), m_nHyperlinkId(1), m_nNumberingId(1), m_nId(1), m_pFonts(NULL)
	{
		m_oPageData.SetSize  (std::to_wstring(DEFAULT_PAGE_WIDTH) + L"tw " + std::to_wstring(DEFAULT_PAGE_HEIGHT) + L"tw", 0, true);
		m_oPageData.SetMargin(L"1440tw 1440tw 1440tw 1440tw", 0, true);
		m_oPageData.SetFooter(L"720tw", 0, true);
		m_oPageData.SetHeader(L"720tw", 0, true);
	}

	~CHtmlFile2_Private()
	{
		m_oLightReader     .Clear();
		m_oStylesCalculator.Clear();
		m_oXmlStyle        .Clear();
		m_oStylesXml       .Clear();
		m_oDocXmlRels      .Clear();
		m_oNoteXmlRels     .Clear();
		m_oDocXml          .Clear();
		m_oNoteXml         .Clear();
		m_oNumberXml       .Clear();
		m_oWebSettings     .Clear();

		if (NULL != m_pFonts)
			RELEASEINTERFACE(m_pFonts);
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
		std::wstring sTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:majorFont><a:minorFont><a:latin typeface=\"Times New Roman\"/><a:ea typeface=\"Times New Roman\"/><a:cs typeface=\"Times New Roman\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
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

		// fontTable.xml
		std::wstring sFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
		NSFile::CFileBinary oFontTableWriter;
		if (oFontTableWriter.CreateFileW(m_sDst + L"/word/fontTable.xml"))
		{
			oFontTableWriter.WriteStringUTF8(sFontTable);
			oFontTableWriter.CloseFile();
		}

		// settings.xml
		std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:displayBackgroundShape/><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
		NSFile::CFileBinary oSettingsWriter;
		if (oSettingsWriter.CreateFileW(m_sDst + L"/word/settings.xml"))
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
		if(oParams)
		{
			if(!oParams->m_sBookTitle.empty())
			{
				sCore += L"<dc:title>";
				sCore += EncodeXmlString(oParams->m_sBookTitle);
				sCore += L"</dc:title>";
			}
			if(!oParams->m_sAuthors.empty())
			{
				sCore += L"<dc:creator>";
				sCore += EncodeXmlString(oParams->m_sAuthors);
				sCore += L"</dc:creator>";
			}
			if(!oParams->m_sGenres.empty())
			{
				sCore += L"<dc:subject>";
				sCore += EncodeXmlString(oParams->m_sGenres);
				sCore += L"</dc:subject>";
			}
			if(!oParams->m_sDate.empty())
			{
				sCore += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">";
				sCore += EncodeXmlString(oParams->m_sDate);
				sCore += L"</dcterms:created>";
			}
			if(!oParams->m_sDescription.empty())
			{
				sCore += L"<dc:description>";
				sCore += EncodeXmlString(oParams->m_sDescription);
				sCore += L"</dc:description>";
			}
			if (!oParams->m_sLanguage.empty())
			{
				wsCurrentLanguage = IndentifyLanguage(oParams->m_sLanguage);

				sCore += L"<dc:language>";
				sCore += wsCurrentLanguage;
				sCore += L"</dc:language>";
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

		m_nId += 7;

		// docDefaults по умолчанию
		if(oParams && !oParams->m_sdocDefaults.empty())
			m_oStylesXml += oParams->m_sdocDefaults;
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
		if(oParams && !oParams->m_sNormal.empty())
			m_oStylesXml += oParams->m_sNormal;
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

		for (const std::pair<std::wstring, std::wstring>& oFootnote : m_mFootnotes)
		{
			m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
			m_oNoteXml.WriteString(oFootnote.second);
			m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
			m_oNoteXml.WriteEncodeXmlString(oFootnote.first);
			m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");
		}

		m_oNoteXmlRels.WriteString(L"</Relationships>");
		if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/footnotes.xml.rels"))
		{
			oRelsWriter.WriteStringUTF8(m_oNoteXmlRels.GetData());
			oRelsWriter.CloseFile();
		}

		if (m_oState.m_bInP)
			m_oDocXml.WriteString(L"</w:p>");

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

		// webSettings.xml
		if (!m_mDivs.empty())
			m_oWebSettings.WriteString(L"</w:divs>");

		m_oWebSettings.WriteString(L"</w:webSettings>");
		NSFile::CFileBinary oWebSettingsWriter;
		if (oWebSettingsWriter.CreateFileW(m_sDst + L"/word/webSettings.xml"))
		{
			oWebSettingsWriter.WriteStringUTF8(m_oWebSettings.GetData());
			oWebSettingsWriter.CloseFile();
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

		const std::wstring sRes{htmlToXhtml(sFileContent, bNeedConvert)};

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

		return m_oLightReader.FromString(sRes);
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

		const std::string sContentType = NSStringFinder::FindProperty(xml_string, "content-type", ":", ";");
		bool bRes = false;

		if(NSStringFinder::Equals(sContentType, "multipart/related"))
		{
			BYTE* pData;
			DWORD nLength;
			if (!NSFile::CFileBinary::ReadAllBytes(sSrc, &pData, nLength))
				return false;

			std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
			RELEASEARRAYOBJECTS(pData);
			std::wstring sRes = mhtToXhtml(sFileContent);
			/*
			NSFile::CFileBinary oWriter;
			if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
			{
				oWriter.WriteStringUTF8(sRes);
				oWriter.CloseFile();
			}
			*/
			bRes = m_oLightReader.FromString(sRes);
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
				readStyle2();
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

	void readStyle2()
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

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			if(!m_oLightReader.IsEmptyNode())
				readStyle2();
		}
	}

	void PageBreakBefore()
	{
		if (!m_oState.m_bInP)
			m_oDocXml.WriteString(L"<w:p>");

		m_oDocXml.WriteString(L"<w:pPr><w:pageBreakBefore/></w:pPr>");

		if (!m_oState.m_bInP)
			m_oDocXml.WriteString(L"</w:p>");
	}

private:
	bool NodeBelongToTable(const std::wstring& wsNodeName) const
	{
		return L"table" == wsNodeName || L"tbody" == wsNodeName || L"th" == wsNodeName || L"td" == wsNodeName ||
		       L"tr" == wsNodeName    || L"thead" == wsNodeName || L"tfoot" == wsNodeName;
	}

	std::wstring GetArgumentValue(const std::wstring& wsArgumentName, const std::wstring& wsDefaultValue = L"")
	{
		if (!m_oLightReader.MoveToFirstAttribute())
			return wsDefaultValue;

		std::wstring wsValue{wsDefaultValue};

		do
		{
			if (wsArgumentName == m_oLightReader.GetName())
			{
				wsValue = m_oLightReader.GetText();
				break;
			}
		} while (m_oLightReader.MoveToNextAttribute());

		m_oLightReader.MoveToElement();
		return wsValue;
	}

	// Так как CSS калькулятор не знает для какой ноды производится расчет стиля
	// и не знает, что некоторые стили предназначены только определенной ноде,
	// то проще пока обрабатывать это заранее
	// ! Используется для стилей, заданных через аргументы !
	bool CheckArgumentMath(const std::wstring& wsNodeName, const std::wstring& wsStyleName) const
	{
		if (L"border" == wsStyleName && L"table" != wsNodeName)
			return false;

		return true;
	}

	bool OpenP(NSStringUtils::CStringBuilder* pXml)
	{
		if (m_oState.m_bInP)
			return false;

		pXml->WriteString(L"<w:p>");
		m_oState.m_bInP = true;
		m_oState.m_bWasPStyle = false;

		return true;
	}

	bool OpenR(NSStringUtils::CStringBuilder* pXml)
	{
		if (m_oState.m_bInR)
			return false;

		pXml->WriteString(L"<w:r>");
		m_oState.m_bInR = true;
		return true;
	}

	void CloseR(NSStringUtils::CStringBuilder* pXml)
	{
		if (!m_oState.m_bInR)
			return;

		pXml->WriteString(L"</w:r>");
		m_oState.m_bInR = false;
	}

	bool OpenT(NSStringUtils::CStringBuilder* pXml)
	{
		if (m_oState.m_bInT)
			return false;

		pXml->WriteString(L"<w:t xml:space=\"preserve\">");
		m_oState.m_bInT = true;
		return true;
	}

	void CloseT(NSStringUtils::CStringBuilder* pXml)
	{
		if (!m_oState.m_bInT)
			return;

		pXml->WriteString(L"</w:t>");
		m_oState.m_bInT = false;
	}

	void CloseP(NSStringUtils::CStringBuilder* pXml, const std::vector<NSCSS::CNode>& arSelectors)
	{
		m_oState.m_bWasSpace = true;

		if (!m_oState.m_bInP)
			return;

		CloseT(pXml);
		CloseR(pXml);

		if (m_oState.m_bInHyperlink)
		{
			if (arSelectors.rend() != std::find_if(arSelectors.rbegin(), arSelectors.rend(), [](const NSCSS::CNode& oNode) { return L"a" == oNode.m_wsName; }))
			{
				pXml->WriteString(L"</w:hyperlink>");
				m_oState.m_bInHyperlink = false;
			}
		}

		pXml->WriteString(L"</w:p>");
		m_oState.m_bInP = false;
	}

	void WriteSpace(NSStringUtils::CStringBuilder* pXml)
	{
		if (NULL == pXml)
			return;

		OpenR(pXml);
		pXml->WriteString(L"<w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/></w:rPr><w:t xml:space=\"preserve\"> </w:t>");
		CloseR(pXml);
		m_oState.m_bWasSpace = true;
	}

	void WriteBookmark(NSStringUtils::CStringBuilder* pXml, const std::wstring& wsId)
	{
		if (NULL == pXml)
			return;

		const std::wstring sCrossId = std::to_wstring(m_mBookmarks.size() + 1);
		std::wstring sName;

		if (m_mBookmarks.end() != m_mBookmarks.find(wsId))
			sName = wsId + L"_" + std::to_wstring(++m_mBookmarks[wsId]);
		else
		{
			sName = wsId;
			m_mBookmarks.insert({wsId, 1});
		}

		pXml->WriteString(L"<w:bookmarkStart w:id=\"");
		pXml->WriteString(sCrossId);
		pXml->WriteString(L"\" w:name=\"");
		pXml->WriteEncodeXmlString(sName);
		pXml->WriteString(L"\"/><w:bookmarkEnd w:id=\"");
		pXml->WriteString(sCrossId);
		pXml->WriteString(L"\"/>");
	}

	std::wstring WriteDiv(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml || sSelectors.empty())
			return std::wstring();

		const std::wstring wsKeyWord{sSelectors.back().m_wsName};

		std::map<std::wstring, UINT>::const_iterator itFound = m_mDivs.find(wsKeyWord);

		if (m_mDivs.end() != itFound)
			return std::to_wstring(itFound->second);

		const std::wstring wsId{std::to_wstring(m_mDivs.size() + 1)};

		if (m_mDivs.empty())
			pXml->WriteString(L"<w:divs>");

		m_oStylesCalculator.CalculateCompiledStyle(sSelectors);
		NSCSS::CCompiledStyle *pStyle = sSelectors.back().m_pCompiledStyle;

		const bool bInTable = ElementInTable(sSelectors);

		INT nMarLeft   = (!bInTable) ? 720 : 0;
		INT nMarRight  = (!bInTable) ? 720 : 0;
		INT nMarTop    = (!bInTable) ? 100 : 0;
		INT nMarBottom = (!bInTable) ? 100 : 0;

		if (!pStyle->m_oMargin.GetLeft().Empty() && !pStyle->m_oMargin.GetLeft().Zero())
			nMarLeft  = pStyle->m_oMargin.GetLeft().ToInt(NSCSS::Twips, m_oPageData.GetWidth().ToInt(NSCSS::Twips));

		if (!pStyle->m_oMargin.GetRight().Empty() && !pStyle->m_oMargin.GetRight().Zero())
			nMarRight = pStyle->m_oMargin.GetRight().ToInt(NSCSS::Twips, m_oPageData.GetWidth().ToInt(NSCSS::Twips));

		if (!pStyle->m_oMargin.GetTop().Empty() && !pStyle->m_oMargin.GetTop().Zero())
			nMarTop = pStyle->m_oMargin.GetTop().ToInt(NSCSS::Twips, m_oPageData.GetHeight().ToInt(NSCSS::Twips));

		if (!pStyle->m_oMargin.GetBottom().Empty() && !pStyle->m_oMargin.GetBottom().Zero())
			nMarBottom = pStyle->m_oMargin.GetBottom().ToInt(NSCSS::Twips, m_oPageData.GetHeight().ToInt(NSCSS::Twips));

		if (L"blockquote" == wsKeyWord)
		{
			sSelectors.back().m_mAttributes.insert({L"margin", L"0px"});

			pXml->WriteString(L"<w:div w:id=\"" + wsId + L"\">");
			pXml->WriteString(L"<w:blockQuote w:val=\"1\"/>");
			pXml->WriteString(L"<w:marLeft w:val=\"" + std::to_wstring(nMarLeft) + L"\"/>");
			pXml->WriteString(L"<w:marRight w:val=\"" + std::to_wstring(nMarRight) + L"\"/>");
			pXml->WriteString(L"<w:marTop w:val=\"" + std::to_wstring(nMarTop) + L"\"/>");
			pXml->WriteString(L"<w:marBottom w:val=\"" + std::to_wstring(nMarBottom) + L"\"/>");
			pXml->WriteString(L"<w:divBdr>");
			pXml->WriteString(L"<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
			pXml->WriteString(L"<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
			pXml->WriteString(L"<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
			pXml->WriteString(L"<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>");
			pXml->WriteString(L"</w:divBdr>");
			pXml->WriteString(L"</w:div>");
		}

		m_mDivs.insert(std::make_pair(wsKeyWord, m_mDivs.size() + 1));

		return wsId;
	}

	std::wstring GetSubClass(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors)
	{
		NSCSS::CNode oNode;
		std::wstring sNote;
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
				WriteBookmark(oXml, oNode.m_wsId);

				if (!m_oStylesCalculator.HaveStylesById(oNode.m_wsId))
					oNode.m_wsId.clear();
			}
			else if(sName == L"style")
				oNode.m_wsStyle += m_oLightReader.GetText();
			else if(sName == L"title")
				sNote           = m_oLightReader.GetText();
			else
			{
				if (CheckArgumentMath(oNode.m_wsName, sName))
					oNode.m_mAttributes[sName] = m_oLightReader.GetText();
			}
		}
		m_oLightReader.MoveToElement();
		sSelectors.push_back(oNode);
		return sNote;
	}

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bP)
	{
//        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
		if ((bP && !m_oXmlStyle.WritePStyle(oStyle)) || (!bP && !m_oXmlStyle.WriteRStyle(oStyle)))
			return L"";

		m_oStylesXml.WriteString(m_oXmlStyle.GetStyle());
		return m_oXmlStyle.GetIdAndClear();
	}

	void readHead()
	{
		if(m_oLightReader.IsEmptyNode())
			return;
		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			const std::wstring wsName = m_oLightReader.GetName();
			// Базовый адрес
			if (L"base" == wsName)
				m_sBase = GetArgumentValue(L"href");
		}

		m_oLightReader.MoveToElement();
	}

	void readBody()
	{
		std::vector<NSCSS::CNode> sSelectors;
		
		sSelectors.push_back(NSCSS::CNode(L"html", L"", L""));
		
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

		CTextSettings oTS;
		readStream(&m_oDocXml, sSelectors, oTS);
	}

	bool ReadText(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		std::wstring sText = m_oLightReader.GetText();

		if (sText.empty())
			return false;

		m_oStylesCalculator.CalculateCompiledStyle(arSelectors);

		bool bPre = oTS.bPre;

		if (!bPre && nullptr != arSelectors.back().m_pCompiledStyle)
		{
			NSCSS::CCompiledStyle* pCompiledStyle{arSelectors.back().m_pCompiledStyle};

			// TODO::поведение должно быть немного разное (реализовать)
			switch(pCompiledStyle->m_oDisplay.GetWhiteSpace().ToInt())
			{
				case NSCSS::NSProperties::EWhiteSpace::Pre:
				case NSCSS::NSProperties::EWhiteSpace::Pre_Wrap:
				case NSCSS::NSProperties::EWhiteSpace::Pre_Line:
					bPre = true;
				default:
					break;
			}
		}

		if (!bPre && sText.end() == std::find_if_not(sText.begin(), sText.end(), [](wchar_t wchChar){ return iswspace(wchChar) && 0xa0 != wchChar;}))
			return false;

		if(oTS.bBdo)
			std::reverse(sText.begin(), sText.end());

		const bool bInT = m_oState.m_bInT;

		GetSubClass(pXml, arSelectors);

		//TODO:: сделать так, чтобы параграф (со своими стилями) открывался при чтении сооответствующей ноды, а не при чтении текста
		OpenP(pXml);

		NSStringUtils::CStringBuilder oPPr;

		const std::wstring sPStyle = wrP(&oPPr, arSelectors, oTS);

		WriteToStringBuilder(oPPr, *pXml);

		NSStringUtils::CStringBuilder oRPr;
		std::wstring sRStyle;

		if (OpenR(pXml))
		{
			sRStyle = wrRPr(&oRPr, arSelectors, oTS);

			WriteToStringBuilder(oRPr, *pXml);

			if (oTS.bQ)
				pXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t>");
		}

		if (oTS.bQ)
			pXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t>");

		if (!bPre && oTS.bAddSpaces && m_oState.m_bInP && !m_oState.m_bInR && !iswspace(sText.front()) && !iswpunct(sText.front()) && !m_oState.m_bWasSpace && CTextSettings::Normal == oTS.eTextMode)
			WriteSpace(pXml);

		if(bPre)
		{
			size_t unBegin = 0, unEnd = sText.find_first_of(L"\n\r\t");

			while (std::wstring::npos != unBegin)
			{
				if (OpenR(pXml))
				{
					pXml->WriteString(L"<w:test/>");
					WriteToStringBuilder(oRPr, *pXml);
				}

				OpenT(pXml);
				if (unEnd == std::wstring::npos)
				{
					pXml->WriteEncodeXmlString(sText.c_str() + unBegin, sText.length() - unBegin);
					break;
				}

				if (unBegin != unEnd)
				{
					pXml->WriteEncodeXmlString(sText.c_str() + unBegin, unEnd - unBegin);
					CloseT(pXml);
				}

				if (L'\n' == sText[unEnd])
				{
					pXml->WriteString(L"<w:br/>");
				}
				else if (L'\t' == sText[unEnd])
				{
					pXml->WriteString(L"<w:tab/>");
				}

				unBegin = unEnd + 1;
				unEnd = sText.find_first_of(L"\n\r\t", unBegin);
			}
		}
		else
		{
			ReplaceSpaces(sText);

			if (!sText.empty() && L'\t' == sText[0])
			{
				pXml->WriteString(L"<w:tab/>");
				sText.erase(0, 1);
			}

			if (!sText.empty() && std::iswspace(sText.front()) && m_oState.m_bWasSpace)
				sText.erase(0, 1);

			OpenT(pXml);

			if (oTS.bMergeText && !m_oState.m_bWasSpace && bInT && !bPre)
				pXml->WriteEncodeXmlString(L" ");

			if (!sText.empty())
			{
				m_oState.m_bWasSpace = std::iswspace(sText.back());
				pXml->WriteEncodeXmlString(sText);
			}
		}

		if (oTS.bQ)
			pXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t>");

		CloseT(pXml);

		if (!oTS.bMergeText)
			CloseR(pXml);

		arSelectors.pop_back();
		return true;
	}

	bool ReadAbbr(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS, std::wstring& wsNote)
	{
		if (NULL == pXml || wsNote.empty())
			return false;

		wrP(pXml, arSelectors, oTS);
		const std::wstring wsName{L"Bookmark" + std::to_wstring(m_mBookmarks.size() + 1)};
		m_mBookmarks.insert(std::make_pair(wsName, m_mBookmarks.size() + 1));
		pXml->WriteString(L"<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r><w:r><w:instrText>HYPERLINK  \\l \"" + wsName + L"\" \\o \"");
		pXml->WriteEncodeXmlString(wsNote);
		pXml->WriteString(L"\"</w:instrText></w:r>");
		pXml->WriteString(L"<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r>");
		const bool bResult = readStream(pXml, arSelectors, oTS);
		pXml->WriteString(L"<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>");
		wsNote.clear();

		return bResult;
	}

	bool ReadBold(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oFont.SetWeight(L"bold", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadBdo(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		const std::wstring sDir{GetArgumentValue(L"dir")};

		CTextSettings oTSBdo(oTS);
		oTSBdo.bBdo = (sDir == L"rtl");

		return readStream(pXml, arSelectors, oTSBdo);
	}

	bool ReadBdi(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSBdo(oTS);
		oTSBdo.bBdo = false;
		return readStream(pXml, arSelectors, oTSBdo);
	}

	bool ReadBr(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		if (m_oState.m_bInP)
		{
			OpenR(pXml);
			m_oStylesCalculator.CalculateCompiledStyle(arSelectors);
			if(arSelectors.back().m_pCompiledStyle->m_oText.GetAlign() == L"both")
				pXml->WriteString(L"<w:tab/>");
			pXml->WriteString(L"<w:br/>");
			CloseR(pXml);
		}
		else
			WriteEmptyParagraph(pXml, false, m_oState.m_bInP);

		m_oState.m_bWasSpace = true;

		return true;
	}

	bool ReadCenter(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSP(oTS);
		oTSP.oAdditionalStyle.m_oText.SetAlign(L"center", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSP);
	}

	bool ReadItalic(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oFont.SetStyle(L"italic", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadCode(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml || arSelectors.empty())
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oFont.SetFamily(L"Courier New", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadKbd(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oFont.SetFamily(L"Courier New", UINT_MAX, true);
		oTSR.oAdditionalStyle.m_oFont.SetSize(20, UINT_MAX, true);
		oTSR.oAdditionalStyle.m_oFont.SetWeight(L"bold", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadSamp(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oFont.SetFamily(L"Courier New", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadStrike(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oText.SetDecoration(L"line-through", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadFont(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);

		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sAName = m_oLightReader.GetName();
			if(sAName == L"color")
				oTSR.oAdditionalStyle.m_oText.SetColor(m_oLightReader.GetText(), UINT_MAX, true);
			else if(sAName == L"face")
				oTSR.oAdditionalStyle.m_oFont.SetFamily(m_oLightReader.GetText(), UINT_MAX, true);
			else if(sAName == L"size")
			{
				int nSize = 3;
				const std::wstring sSize = m_oLightReader.GetText();
				if(!sSize.empty())
				{
					if(sSize.front() == L'+')
						nSize += NSStringFinder::ToInt(sSize.substr(1));
					else if(sSize.front() == L'-')
						nSize -= NSStringFinder::ToInt(sSize.substr(1));
					else
						nSize = NSStringFinder::ToInt(sSize);
				}

				if (nSize < 1 || nSize > 7)
					nSize = 3;

				oTSR.oAdditionalStyle.m_oFont.SetSize(HTML_FONTS[nSize - 1], UINT_MAX, true);
			}
		}
		m_oLightReader.MoveToElement();
		const bool bResult = readStream(pXml, arSelectors, oTSR);
		m_oState.m_bWasSpace = true;

		return bResult;
	}

	bool ReadUnderline(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oText.SetDecoration(L"underline", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadMark(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.oAdditionalStyle.m_oText.SetHighlight(L"yellow", UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadQ(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSQ(oTS);
		oTSQ.bQ = true;

		return readStream(pXml, arSelectors, oTSQ);
	}

	bool ReadSup(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.eTextMode = CTextSettings::Superscript;

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadSub(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSR(oTS);
		oTSR.eTextMode = CTextSettings::Subscript;

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadSpan(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml || arSelectors.empty() || arSelectors.back().m_wsClass == L"MsoFootnoteReference")
			return false;

		CTextSettings oTSR(oTS);
		oTSR.bAddSpaces = false;

		return readStream(pXml, arSelectors, oTSR);
	}

	bool ReadNobr(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSPre(oTS);
		oTSPre.bPre = true;

		return readStream(pXml, arSelectors, oTSPre);
	}

	bool ReadBasefont(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml || !m_oLightReader.MoveToFirstAttribute())
			return false;

		do
		{
			if (L"face" == m_oLightReader.GetName())
				oTS.oAdditionalStyle.m_oFont.SetFamily(m_oLightReader.GetText(), UINT_MAX, true);
			else if (L"size" == m_oLightReader.GetName())
			{
				switch(NSStringFinder::ToInt(m_oLightReader.GetText(), 3))
				{
					case 1: oTS.oAdditionalStyle.m_oFont.SetSize(7.5, UINT_MAX, true);  break;
					case 2: oTS.oAdditionalStyle.m_oFont.SetSize(10, UINT_MAX, true);   break;
					default:
					case 3: oTS.oAdditionalStyle.m_oFont.SetSize(12, UINT_MAX, true);   break;
					case 4: oTS.oAdditionalStyle.m_oFont.SetSize(13.5, UINT_MAX, true); break;
					case 5: oTS.oAdditionalStyle.m_oFont.SetSize(18, UINT_MAX, true);   break;
					case 6: oTS.oAdditionalStyle.m_oFont.SetSize(24, UINT_MAX, true);   break;
					case 7: oTS.oAdditionalStyle.m_oFont.SetSize(36, UINT_MAX, true);   break;
				}
			}
			else if (L"color" == m_oLightReader.GetName())
				oTS.oAdditionalStyle.m_oText.SetColor(m_oLightReader.GetText(), UINT_MAX, true);
		} while (m_oLightReader.MoveToNextAttribute());

		m_oLightReader.MoveToElement();

		oTS.oAdditionalStyle.SetID(m_oStylesCalculator.CalculateStyleId(arSelectors.back()));

		return true;
	}

	bool ReadDD(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSP(oTS);
		oTSP.oAdditionalStyle.m_oMargin.SetLeft(720, UINT_MAX, true);

		return readStream(pXml, arSelectors, oTSP);
	}

	bool ReadDiv(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		if (!m_oState.m_bBanUpdatePageData)
			m_oStylesCalculator.CalculatePageStyle(m_oPageData, arSelectors);

		int bMsoFootnote = 0;
		std::wstring sFootnoteID;
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sAName = m_oLightReader.GetName();
			std::wstring sAText = m_oLightReader.GetText();
			if (sAName == L"epub:type" && sAText == L"footnote")
				bMsoFootnote++;
			else if (sAName == L"style" && sAText == L"mso-element:footnote")
				bMsoFootnote++;
			else if (sAName == L"id")
			{
				std::map<std::wstring, std::wstring>::iterator it = m_mFootnotes.find(sAText);
				if (it != m_mFootnotes.end())
				{
					bMsoFootnote++;
					sFootnoteID = it->second;
				}
			}
		}
		m_oLightReader.MoveToElement();
		if (bMsoFootnote >= 2)
		{
			m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
			m_oNoteXml.WriteString(sFootnoteID);
			m_oNoteXml.WriteString(L"\">");
			readStream(&m_oNoteXml, arSelectors, oTS);
			m_oNoteXml.WriteString(L"</w:footnote>");
		}
		else
			return readStream(pXml, arSelectors, oTS);

		return true;
	}

	bool ReadBlockquote(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oNewTS{oTS};
		oNewTS.sPStyle += L"<w:divId w:val=\"" + WriteDiv(&m_oWebSettings, arSelectors, oNewTS) + L"\"/>";

		return readStream(pXml, arSelectors, oNewTS);
	}

	bool ReadHr(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		bool bPrint = true;

		for (const NSCSS::CNode& item : arSelectors)
		{
			if (item.m_wsName == L"div" && item.m_wsStyle == L"mso-element:footnote-list")
			{
				bPrint = false;
				break;
			}
		}

		if (bPrint)
		{
			NSCSS::NSProperties::CDigit oSize, oWidth;
			NSCSS::NSProperties::CColor oColor;
			bool bShade = false;
			std::wstring wsAlign{L"center"};

			if (m_oLightReader.MoveToFirstAttribute())
			{
				std::wstring wsAttributeName;
				do
				{
					wsAttributeName =  m_oLightReader.GetName();

					if (L"align" == wsAttributeName)
					{
						const std::wstring wsValue{m_oLightReader.GetText()};

						if (NSStringFinder::Equals(L"left", wsValue))
							wsAlign = L"left";
						else if (NSStringFinder::Equals(L"right", wsValue))
							wsAlign = L"right";
						else if (NSStringFinder::Equals(L"center", wsValue))
							wsAlign = L"center";
					}
					if (L"color" == wsAttributeName)
						oColor.SetValue(m_oLightReader.GetText());
					else if (L"noshade" == wsAttributeName)
						bShade = true;
					else if (L"size" == wsAttributeName)
						oSize.SetValue(m_oLightReader.GetText());
					else if (L"width" == wsAttributeName)
						oWidth.SetValue(m_oLightReader.GetText());
				} while (m_oLightReader.MoveToNextAttribute());

				m_oLightReader.MoveToElement();
			}

			const bool bOpenedP = OpenP(pXml);

			OpenR(pXml);
			WriteLine(pXml, wsAlign, (!oColor.Empty()) ? oColor.ToWString() : L"a0a0a0", bShade, (!oSize.Empty()) ? oSize.ToDouble(NSCSS::Point) : 1.5, (NSCSS::UnitMeasure::Percent == oWidth.GetUnitMeasure()) ? oWidth.ToDouble() : 0);
			CloseR(pXml);

			if (bOpenedP)
				CloseP(pXml, arSelectors);

			return true;
		}

		return false;
	}

	bool ReadPre(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSPre(oTS);
		oTSPre.oAdditionalStyle.m_oFont.SetFamily(L"Courier New", NEXT_LEVEL);
		oTSPre.oAdditionalStyle.m_oFont.SetSize(20, NEXT_LEVEL);
		oTSPre.oAdditionalStyle.m_oMargin.SetTop(0, NEXT_LEVEL);
		oTSPre.oAdditionalStyle.m_oMargin.SetBottom(0, NEXT_LEVEL);
		oTSPre.bPre = true;

		return readStream(pXml, arSelectors, oTSPre);
	}

	bool ReadTextarea(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		CTextSettings oTSP(oTS);
		oTSP.AddPStyle(L"<w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr>");

		return readStream(pXml, arSelectors, oTSP);
	}

	bool ReadDetails(NSStringUtils::CStringBuilder* pXml, std::vector<NSCSS::CNode>& arSelectors, CTextSettings& oTS)
	{
		if (NULL == pXml)
			return false;

		bool bOpened = false;
		if (m_oLightReader.MoveToFirstAttribute())
		{
			do
			{
				bOpened = (L"open" == m_oLightReader.GetName());
			} while (m_oLightReader.MoveToNextAttribute() && !bOpened);
		}
		m_oLightReader.MoveToElement();

		int nDeath = m_oLightReader.GetDepth();
		if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
			return false;

		NSStringUtils::CStringBuilder oSummary;
		NSStringUtils::CStringBuilder oBody;

		const TState oCurrentState{m_oState};
		TState oSummaryState{m_oState};
		TState oBodyState{m_oState};

		do
		{
			if (L"summary" == m_oLightReader.GetName())
			{
				m_oState = oSummaryState;
				if (0 == oSummary.GetSize())
				{
					OpenP(&oSummary);
					OpenR(&oSummary);
					OpenT(&oSummary);
					oSummary.WriteString((bOpened) ? L"\u25BD" : L"\u25B7");
				}

				readStream(&oSummary, arSelectors, oTS);
				CloseP(&oSummary, arSelectors);
				oSummaryState = m_oState;
				m_oState = oCurrentState;
			}
			else if (bOpened)
			{
				m_oState = oBodyState;
				readStream(&oBody, arSelectors, oTS);
				CloseP(&oBody, arSelectors);
				oBodyState = m_oState;
				m_oState = oCurrentState;
			}
		} while (m_oLightReader.ReadNextSiblingNode2(nDeath));

		WriteToStringBuilder(oSummary, *pXml);

		if (bOpened)
		{
			m_oState = oBodyState;
			WriteToStringBuilder(oBody, *pXml);
		}

		return true;
	}

	bool readInside (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS, const std::wstring& sName)
	{
		//TODO:: обработать все варианты return'а

		if(sName == L"#text")
			return ReadText(oXml, sSelectors, oTS);

		if (TagIsUnprocessed(sName))
			return false;

		std::wstring sNote = GetSubClass(oXml, sSelectors);
		bool bResult = true;
		// Ссылка
		// Область ссылки

		const HtmlTag eHtmlTag{GetHtmlTag(sName)};

		switch(eHtmlTag)
		{
			case HTML_TAG(A):
			case HTML_TAG(AREA):
			{
				bResult = readA(oXml, sSelectors, oTS, sNote);
				break;
			}
			case HTML_TAG(ABBR):
			{
				bResult = ReadAbbr(oXml, sSelectors, oTS, sNote);
				break;
			}
			case HTML_TAG(B):
			case HTML_TAG(STRONG):
			{
				bResult = ReadBold(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(BDO):
			{
				bResult = ReadBdo(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(BDI):
			{
				bResult = ReadBdi(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(BR):
			{
				bResult = ReadBr(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(CENTER):
			{
				bResult = ReadCenter(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(CITE):
			case HTML_TAG(DFN):
			case HTML_TAG(EM):
			case HTML_TAG(I):
			case HTML_TAG(VAR):
			{
				bResult = ReadItalic(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(CODE):
			case HTML_TAG(TT):
			{
				bResult = ReadCode(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(KBD):
			{
				bResult = ReadKbd(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(SAMP):
			{
				bResult = ReadSamp(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(DEL):
			case HTML_TAG(S):
			case HTML_TAG(STRIKE):
			{
				bResult = ReadStrike(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(FONT):
			{
				bResult = ReadFont(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(IMG):
			{
				bResult = readImage(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(INS):
			case HTML_TAG(U):
			{
				bResult = ReadUnderline(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(MARK):
			{
				bResult = ReadMark(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(Q):
			{
				bResult = ReadQ(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(SUP):
			{
				bResult = ReadSup(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(SUB):
			{
				bResult = ReadSub(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(SVG):
			{
				wrP(oXml, sSelectors, oTS);
				bResult = readSVG(m_oLightReader.GetOuterXml());

				if (bResult)
					ImageRels(oXml, -1, L"", L"png");

				break;
			}
			case HTML_TAG(INPUT):
			{
				bResult = readInput(oXml, sSelectors, oTS);
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
				sSelectors.pop_back();
				return false;
			}
			case HTML_TAG(SPAN):
			{
				bResult = ReadSpan(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(NOBR):
			{
				bResult = ReadNobr(oXml, sSelectors, oTS);
				break;
			}
			case HTML_TAG(BASEFONT):
			{
				bResult = ReadBasefont(oXml, sSelectors, oTS);
				break;
			}
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
				bResult = readStream(oXml, sSelectors, oTS);
				break;
			}
			default:
			{
				NSStringUtils::CStringBuilder oXmlData;
				TState oCurentState{m_oState};

				CloseP(&oXmlData, sSelectors);

				switch(eHtmlTag)
				{
					case HTML_TAG(ADDRESS):
					{
						bResult = ReadItalic(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(DD):
					{
						bResult = ReadDD(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(ASIDE):
					case HTML_TAG(DIV):
					{
						bResult = ReadDiv(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(BLOCKQUOTE):
					{
						bResult = ReadBlockquote(&oXmlData, sSelectors, oTS);
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
					case HTML_TAG(H1):
					case HTML_TAG(H2):
					case HTML_TAG(H3):
					case HTML_TAG(H4):
					case HTML_TAG(H5):
					case HTML_TAG(H6):
					{
						bResult = readStream(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(HR):
					{
						bResult = ReadHr(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(UL):
					case HTML_TAG(MENU):
					case HTML_TAG(SELECT):
					case HTML_TAG(DATALIST):
					case HTML_TAG(DIR):
					case HTML_TAG(OL):
					{
						bResult = readLi(&oXmlData, sSelectors, oTS, HTML_TAG(OL) != eHtmlTag);
						break;
					}
					case HTML_TAG(PRE):
					case HTML_TAG(XMP):
					{
						bResult = ReadPre(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(TABLE):
					{
						bResult = ParseTable(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(RUBY):
					{
						bResult = ParseRuby(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(TEXTAREA):
					case HTML_TAG(FIELDSET):
					{
						bResult = ReadTextarea(&oXmlData, sSelectors, oTS);
						break;
					}
					case HTML_TAG(DETAILS):
					{
						bResult = ReadDetails(&oXmlData, sSelectors, oTS);
						break;
					}
					default:
					{
						bResult = readStream(&oXmlData, sSelectors, oTS);
						break;
					}
				}

				readNote(&oXmlData, sSelectors, sNote);

				CloseP(&oXmlData, sSelectors);

				if (bResult)
					WriteToStringBuilder(oXmlData, *oXml);
				else
					m_oState = oCurentState;
			}
		}

		if (HTML_TAG(DIV) != eHtmlTag && HTML_TAG(ASIDE) != eHtmlTag)
			m_oState.m_bBanUpdatePageData = true;

		readNote(oXml, sSelectors, sNote);
		sSelectors.pop_back();
		return bResult;
	}

	bool readStream (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS, bool bInsertEmptyP = false)
	{
		int nDeath = m_oLightReader.GetDepth();
		if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
		{
			if (bInsertEmptyP)
			{
				wrP(oXml, sSelectors, oTS);
				wrRPr(oXml, sSelectors, oTS);
				CloseP(oXml, sSelectors);
				m_oState.m_bInP = false;
			}
			return false;
		}

		bool bResult = false;

		do
		{
			if (readInside(oXml, sSelectors, oTS, m_oLightReader.GetName()))
				bResult = true;
		} while(m_oLightReader.ReadNextSiblingNode2(nDeath));

		if (!bResult && bInsertEmptyP)
		{
			wrP(oXml, sSelectors, oTS);
			wrRPr(oXml, sSelectors, oTS);
			CloseP(oXml, sSelectors);
			m_oState.m_bInP = false;
		}

		return bResult;
	}

	void CalculateCellStyles(TTableCellStyle* pCellStyle, std::vector<NSCSS::CNode>& arSelectors)
	{
		if (NULL == pCellStyle)
			return;

		m_oStylesCalculator.CalculateCompiledStyle(arSelectors);

		pCellStyle->m_wsVAlign     = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetVAlign().ToWString();
		pCellStyle->m_wsHAlign     = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHAlign().ToWString();
		pCellStyle->m_oBackground  = arSelectors.back().m_pCompiledStyle->m_oBackground.GetColor();
		pCellStyle->m_oHeight      = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetHeight();
		pCellStyle->m_oWidth       = arSelectors.back().m_pCompiledStyle->m_oDisplay.GetWidth();
		pCellStyle->m_oPadding     = arSelectors.back().m_pCompiledStyle->m_oPadding;
		pCellStyle->m_oBorder      = arSelectors.back().m_pCompiledStyle->m_oBorder;

		if (pCellStyle->m_wsHAlign.empty())
			pCellStyle->m_wsHAlign = arSelectors.back().m_pCompiledStyle->m_oText.GetAlign().ToWString();
	}

	void ParseTableCaption(CTable& oTable, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		GetSubClass(NULL, sSelectors);

		NSStringUtils::CStringBuilder oData;

		CTextSettings oTSCaption{oTS};
		oTSCaption.sPStyle += L"<w:jc w:val=\"center\"/>";

		wrP(&oData, sSelectors, oTSCaption);
		readStream(&oData, sSelectors, oTSCaption);
		CloseP(&oData, sSelectors);

		oTable.AddCaption(oData);

		sSelectors.pop_back();
		return;
	}

	void ParseTableColspan(CTable& oTable)
	{
		CTableColgroup *pColgroup = new CTableColgroup(m_oLightReader);

		if (NULL == pColgroup)
			return;

		std::vector<NSCSS::CNode> arNodes;

		GetSubClass(NULL, arNodes);

		oTable.AddColgroup(pColgroup);

		const int nDeath = m_oLightReader.GetDepth();
		if (!m_oLightReader.IsEmptyNode() && m_oLightReader.ReadNextSiblingNode2(nDeath))
		{
			do
			{
				if (L"col" != m_oLightReader.GetName())
					continue;

				CTableCol *pCol = new CTableCol(m_oLightReader);

				if (NULL == pCol)
					continue;

				GetSubClass(NULL, arNodes);
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
	
	struct TRowspanElement
	{
		UINT  m_unRowSpan;
		UINT  m_unColumnIndex;
		const CTableCell* m_pCell;

		TRowspanElement(UINT unRowSpan, UINT unColumnIndex, const CTableCell* pCell)
			: m_unRowSpan(unRowSpan), m_unColumnIndex(unColumnIndex), m_pCell(pCell)
		{}
	};

	void ParseTableRows(CTable& oTable, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS, ERowParseMode eMode)
	{
		std::vector<TRowspanElement> arRowspanElements;
		std::vector<CTableRow*>      arRows;

		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			if (L"tr" != m_oLightReader.GetName())
				continue;

			GetSubClass(NULL, sSelectors);

			CTableRow *pRow = new CTableRow();

			for (std::vector<TRowspanElement>::iterator itElement = arRowspanElements.begin(); itElement < arRowspanElements.end();)
			{
				pRow->InsertCell(CTableCell::CreateEmpty(itElement->m_pCell->GetColspan(), true, itElement->m_pCell->GetStyles()), itElement->m_unColumnIndex);

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
				CTableCell *pCell = new CTableCell();

				if (NULL == pCell)
					continue;

				GetSubClass(pCell->GetData(), sSelectors);

				std::vector<NSCSS::CNode> arNewSelectors{(std::vector<NSCSS::CNode>::const_iterator)std::find_if(sSelectors.begin(), sSelectors.end(), [](const NSCSS::CNode& oNode){ return L"table" == oNode.m_wsName; }), sSelectors.cend()};

				CalculateCellStyles(pCell->GetStyles(), arNewSelectors);

				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"colspan")
						pCell->SetColspan(NSStringFinder::ToInt(m_oLightReader.GetText(), 1), pRow->GetIndex());
					else if(m_oLightReader.GetName() == L"rowspan")
					{
						pCell->SetRowspan(NSStringFinder::ToInt(m_oLightReader.GetText(), 1));
						
						if (1 != pCell->GetRowspan())
							arRowspanElements.push_back({pCell->GetRowspan(), unColumnIndex, pCell});
					}
				}

				m_oLightReader.MoveToElement();
				m_oState.m_bWasPStyle = false;

				// Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
				if(m_oLightReader.GetName() == L"th")
				{
					CTextSettings oTSR(oTS);

					if (pCell->GetStyles()->m_wsHAlign.empty())
						oTSR.oAdditionalStyle.m_oText.SetAlign(L"center", NEXT_LEVEL);

					oTSR.oAdditionalStyle.m_oFont.SetWeight(L"bold", NEXT_LEVEL);

					readStream(pCell->GetData(), sSelectors, oTSR, true);
				}
				// Читаем td. Ячейка таблицы
				else if(m_oLightReader.GetName() == L"td")
					readStream(pCell->GetData(), sSelectors, oTS, true);

				if (pRow->GetIndex() == MAXCOLUMNSINTABLE - 1)
				{
					CTextSettings oTrTS{oTS};
					oTrTS.bMergeText     = true;
					oTrTS.bAddSpaces     = true;
					m_oState.m_bWasSpace = true;

					while (m_oLightReader.ReadNextSiblingNode(nTrDepth))
					{
						if (L"td" != m_oLightReader.GetName() && L"th" != m_oLightReader.GetName())
							continue;

						GetSubClass(pCell->GetData(), sSelectors);
						readStream(pCell->GetData(), sSelectors, oTrTS);
						sSelectors.pop_back();
					}
				}
				CloseP(pCell->GetData(), sSelectors);

				pRow->AddCell(pCell);

				sSelectors.pop_back();

				++unColumnIndex;

				if (pRow->GetIndex() == MAXCOLUMNSINTABLE)
					break;
			}

			sSelectors.pop_back();
			arRows.push_back(pRow);
		}

		oTable.AddRows(arRows, eMode);
	}

	bool ParseRuby(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)
	{
		if(m_oLightReader.IsEmptyNode())
			return false;

		const int nDepth = m_oLightReader.GetDepth();

		#ifdef DISABLE_RUBY_SUPPORT
		wrP(oXml, sSelectors, oTS);

		while (m_oLightReader.ReadNextSiblingNode2(nDepth))
		{
			GetSubClass(NULL, sSelectors);

			if (L"rp" == sSelectors.back().m_wsName)
			{
				sSelectors.pop_back();
				continue;
			}
			else if (L"rt" == sSelectors.back().m_wsName)
				readStream(oXml, sSelectors, oTS);
			else
				readInside(oXml, sSelectors, oTS, sSelectors.back().m_wsName);

			sSelectors.pop_back();
		}

		CloseP(oXml, sSelectors);

		return true;
		#endif

		NSStringUtils::CStringBuilder oBase;
		NSStringUtils::CStringBuilder oRT;

		TState oRtState{m_oState};
		oRtState.m_bInP = true;

		TState oBaseState{m_oState};
		oBaseState.m_bInP = true;

		while (m_oLightReader.ReadNextSiblingNode2(nDepth))
		{
			GetSubClass(NULL, sSelectors);

			CTextSettings oNewSettings{oTS};

			m_oStylesCalculator.CalculateCompiledStyle(sSelectors);

			const std::wstring wsHighlight{sSelectors.back().m_pCompiledStyle->m_oBackground.GetColor()
				        .EquateToColor({{{0,   0,   0},   L"black"},    {{0,   0,   255}, L"blue"},      {{0,   255, 255}, L"cyan"},
			                            {{0,   255, 0},   L"green"},    {{255, 0,   255}, L"magenta"},   {{255, 0,   0},   L"red"},
			                             {{255, 255, 0},   L"yellow"},   {{255, 255, 255}, L"white"},     {{0,   0,   139}, L"darkBlue"},
			                             {{0,   139, 139}, L"darkCyan"}, {{0,   100, 0},   L"darkGreen"}, {{139, 0,   139}, L"darkMagenta"},
			                             {{139, 0,   0},   L"darkRed"},  {{128, 128, 0},   L"darkYellow"},{{169, 169, 169}, L"darkGray"},
			                             {{211, 211, 211}, L"lightGray"}})};

			if (L"none" != wsHighlight)
				oNewSettings.oAdditionalStyle.m_oText.SetHighlight(sSelectors.back().m_pCompiledStyle->m_oBackground.GetColor().ToWString(), NEXT_LEVEL);
				// oNewSettings.AddRStyle(L"<w:shd w:val=\"" + wsHighlight + L"\"/>");

			if (L"rt" == sSelectors.back().m_wsName)
			{
				std::swap(m_oState, oRtState);
				readStream(&oRT, sSelectors, oNewSettings);
				std::swap(m_oState, oRtState);
			}
			else if (L"rp" == sSelectors.back().m_wsName)
			{
				sSelectors.pop_back();
				continue;
			}
			else if (L"#text" == sSelectors.back().m_wsName)
			{
				std::swap(m_oState, oBaseState);
				readInside(&oBase, sSelectors, oNewSettings, sSelectors.back().m_wsName);
				std::swap(m_oState, oBaseState);
			}
			sSelectors.pop_back();
		}

		WriteSpace(&oBase);

		wrP(oXml, sSelectors, oTS);

		if (0 != oRT.GetSize())
		{
			m_oStylesCalculator.CalculateCompiledStyle(sSelectors);
			NSCSS::CCompiledStyle *pStyle = sSelectors.back().m_pCompiledStyle;

			int nFontSize = 24;

			if (!pStyle->m_oFont.GetSize().Empty() && !pStyle->m_oFont.GetSize().Zero())
				nFontSize = pStyle->m_oFont.GetSize().ToInt(NSCSS::Point) * 2;

			bool bConsistsChineseCharacters = false;

			const std::vector<NSCSS::CNode>::const_reverse_iterator oFound{std::find_if(sSelectors.crbegin(), sSelectors.crend(), [](const NSCSS::CNode& oNode){ return oNode.m_mAttributes.cend() != oNode.m_mAttributes.find(L"lang");})};

			if (sSelectors.crend() != oFound)
			{
				const size_t unFound{oFound->m_mAttributes.at(L"lang").find(L"-")};

				if (std::wstring::npos != unFound)
					bConsistsChineseCharacters = ConsistsChineseCharacters(oFound->m_mAttributes.at(L"lang").substr(0, unFound));
			}

			OpenR(oXml);
			oXml->WriteString(L"<w:ruby><w:rubyPr><w:rubyAlign w:val=\"" + std::wstring((bConsistsChineseCharacters) ? L"distributeSpace" : L"center") + L"\"/><w:hps w:val=\"" + std::to_wstring(nFontSize) + L"\"/><w:hpsRaise w:val=\"" + std::to_wstring(nFontSize - 2) + L"\"/><w:hpsBaseText w:val=\"" + std::to_wstring(nFontSize) + L"\"/></w:rubyPr>");
			oXml->WriteString(L"<w:rt>");
			WriteToStringBuilder(oRT, *oXml);
			oXml->WriteString(L"</w:rt>");
			oXml->WriteString(L"<w:rubyBase>");
			WriteToStringBuilder(oBase, *oXml);
			oXml->WriteString(L"</w:rubyBase>");
			oXml->WriteString(L"</w:ruby>");
			CloseR(oXml);
		}
		else
			WriteToStringBuilder(oBase, *oXml);

		CloseP(oXml, sSelectors);

		return true;
	}

	bool ParseTable(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if(m_oLightReader.IsEmptyNode())
			return false;

		CTable oTable;
		CTextSettings oTextSettings{oTS};
		oTextSettings.sPStyle.clear();

		m_oStylesCalculator.CalculateCompiledStyle(sSelectors);
		NSCSS::CCompiledStyle *pStyle = sSelectors.back().m_pCompiledStyle;

		//Table styles
		std::wstring wsFrame;

		for (const std::pair<std::wstring, std::wstring>& oArgument : sSelectors.back().m_mAttributes)
		{
			if (L"border" == oArgument.first)
			{
				const int nWidth = NSStringFinder::ToInt(oArgument.second);

				if (0 < nWidth)
				{
					oTable.SetRules(L"all");

					if (!pStyle->m_oBorder.Empty())
						continue;

					pStyle->m_oBorder.SetStyle(L"outset",  0, true);
					pStyle->m_oBorder.SetWidth(nWidth,     0, true);
					pStyle->m_oBorder.SetColor(L"auto",    0, true);
				}
				else if (pStyle->m_oBorder.Empty())
				{
					pStyle->m_oBorder.SetNone(0, true);
					oTable.SetRules(L"none");
				}
			}
			else if (L"cellpadding" == oArgument.first)
				pStyle->m_oPadding.SetValues(oArgument.second + L"px", 0, true);
			else if (L"rules" == oArgument.first)
				oTable.SetRules(oArgument.second);
			else if (L"frame" == oArgument.first)
				wsFrame = oArgument.second;
		}

		if (!wsFrame.empty() && pStyle->m_oBorder.Empty())
		{
			#define SetDefaultBorderSide(side) \
				pStyle->m_oBorder.SetStyle##side(L"solid", 0, true); \
				pStyle->m_oBorder.SetWidth##side(1,        0, true); \
				pStyle->m_oBorder.SetColor##side(L"black", 0, true);

			if (NSStringFinder::Equals(L"border", wsFrame))
			{
				SetDefaultBorderSide()
			}
			else if (NSStringFinder::Equals(L"above", wsFrame))
			{
				SetDefaultBorderSide(TopSide)
			}
			else if (NSStringFinder::Equals(L"below", wsFrame))
			{
				SetDefaultBorderSide(BottomSide)
			}
			else if (NSStringFinder::Equals(L"hsides", wsFrame))
			{
				SetDefaultBorderSide(TopSide)
				SetDefaultBorderSide(BottomSide)
			}
			else if (NSStringFinder::Equals(L"vsides", wsFrame))
			{
				SetDefaultBorderSide(LeftSide)
				SetDefaultBorderSide(RightSide)
			}
			else if (NSStringFinder::Equals(L"rhs", wsFrame))
			{
				SetDefaultBorderSide(RightSide)
			}
			else if (NSStringFinder::Equals(L"lhs", wsFrame))
			{
				SetDefaultBorderSide(LeftSide)
			}
		}

		if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Collapse)
			oTable.SetCellSpacing(0);
		else if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"cellspacing"))
			oTable.SetCellSpacing(NSStringFinder::ToInt(sSelectors.back().m_mAttributes[L"cellspacing"]));
		else if (pStyle->m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Separate)
			oTable.SetCellSpacing(15);

		oTable.SetWidth(pStyle->m_oDisplay.GetWidth());
		oTable.SetBorder(pStyle->m_oBorder);
		oTable.SetPadding(pStyle->m_oPadding);
		oTable.SetMargin(pStyle->m_oMargin);
		oTable.SetAlign(pStyle->m_oDisplay.GetHAlign().ToWString());
		//------

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath)) 
		{
			const std::wstring sName = m_oLightReader.GetName();
			GetSubClass(oXml, sSelectors);

			if(sName == L"caption")
				ParseTableCaption(oTable, sSelectors, oTS);
			if(sName == L"thead")
				ParseTableRows(oTable, sSelectors, oTextSettings, ERowParseMode::ParseModeHeader);
			if(sName == L"tbody")
				ParseTableRows(oTable, sSelectors, oTextSettings, ERowParseMode::ParseModeBody);
			else if(sName == L"tfoot")
				ParseTableRows(oTable, sSelectors, oTextSettings, ERowParseMode::ParseModeFoother);
			else if (sName == L"colgroup")
				ParseTableColspan(oTable);

			sSelectors.pop_back();
		}

		oTable.Shorten();
		oTable.CompleteTable();
		oTable.ConvertToOOXML(*oXml);
		WriteEmptyParagraph(oXml, true);

		return true;
	}

	bool readInput  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)
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
			return false;
		if(sValue.empty())
			sValue = sAlt;
		if(!sValue.empty())
		{
			wrP(oXml, sSelectors, oTS);
			OpenR(oXml);
			wrRPr(oXml, sSelectors, oTS);
			OpenT(oXml);
			oXml->WriteEncodeXmlString(sValue + L' ');
			CloseT(oXml);
			CloseR(oXml);
		}

		return readStream(oXml, sSelectors, oTS, ElementInTable(sSelectors));
	}

	bool readLi     (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS, bool bType)
	{
		if(m_oLightReader.IsEmptyNode())
			return false;

		int nStart = 1;
		while(m_oLightReader.MoveToNextAttribute())
			if(m_oLightReader.GetName() == L"start")
				nStart = NSStringFinder::ToInt(m_oLightReader.GetText(), 1);
		m_oLightReader.MoveToElement();

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			const std::wstring sName = m_oLightReader.GetName();
			if (sName == L"optgroup")
			{
				GetSubClass(oXml, sSelectors);
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() != L"label")
						continue;

					CloseP(oXml, sSelectors);
					wrP(oXml, sSelectors, oTS);
					OpenR(oXml);
					wrRPr(oXml, sSelectors, oTS);
					OpenT(oXml);
					oXml->WriteEncodeXmlString(m_oLightReader.GetText());
					CloseT(oXml);
					CloseR(oXml);
				}
				m_oLightReader.MoveToElement();
				readLi(oXml, sSelectors, oTS, true);
				sSelectors.pop_back();
				continue;
			}
			if (sName != L"li" && sName != L"option")
			{
				readInside(oXml, sSelectors, oTS, sName);
				continue;
			}

			CloseP(oXml, sSelectors);

			CTextSettings oTSLiP(oTS);

			oTSLiP.bNumberingLi = !bType;

			std::wstring wsValue;
			const std::wstring wsParentName{(!sSelectors.empty()) ? sSelectors.back().m_wsName : L""};

			GetSubClass(oXml, sSelectors);

			std::wstring wsArgumentName;

			while(m_oLightReader.MoveToNextAttribute())
			{
				wsArgumentName = m_oLightReader.GetName();

				if(L"value" == wsArgumentName && L"datalist" == wsParentName)
				{
					if (sName == L"option")
						wsValue = m_oLightReader.GetText();
					else
						nStart = NSStringFinder::ToInt(m_oLightReader.GetText(), 1);
				}
				else if (L"disabled" == wsArgumentName)
					oTSLiP.oAdditionalStyle.m_oText.SetColor(L"#808080", NEXT_LEVEL);
				else if (L"selected" == wsArgumentName)
					oTSLiP.oAdditionalStyle.m_oText.SetDecoration(L"underline", NEXT_LEVEL);
			}
			m_oLightReader.MoveToElement();

			oTSLiP.nLi++;
			oTSLiP.bWritedLi = false;

			if (!wsValue.empty())
			{
				OpenR(oXml);
				OpenT(oXml);
				oXml->WriteEncodeXmlString(wsValue);
				CloseT(oXml);
				CloseR(oXml);
			}

			readStream(oXml, sSelectors, oTSLiP);

			CloseP(oXml, sSelectors);

			sSelectors.pop_back();
		}
		// Нумерованный список
		if(!bType)
		{
			const std::wstring wsStart(std::to_wstring(nStart));
			m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
			m_oNumberXml.WriteString(std::to_wstring(m_nNumberingId++));
			m_oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"");
			m_oNumberXml.WriteString(wsStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");
		}

		return true;
	}

	bool readA (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS, std::wstring& sNote)
	{
		std::wstring sRef;
		std::wstring sAlt;
		bool bCross = false;
		std::wstring sFootnote;
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName = m_oLightReader.GetName();
			std::wstring sText = m_oLightReader.GetText();
			if(sName == L"href")
			{
				sRef = sText;
				if(sRef.find('#') != std::wstring::npos)
					bCross = true;
			}
			else if(sName == L"name")
				WriteBookmark(oXml, sText);
			else if(sName == L"alt")
				sAlt = sText;
			else if (sName == L"style" && sText.find(L"mso-footnote-id") != std::wstring::npos)
				sFootnote = sText.substr(sText.rfind(L':') + 1);
			else if (sName == L"epub:type" && sText.find(L"noteref"))
				sFootnote = L"href";
		}
		m_oLightReader.MoveToElement();

		if(sNote.empty())
			sNote = sRef;

		if (bCross && sFootnote == L"href")
			sFootnote = sRef.substr(sRef.find('#') + 1);

		if (!OpenP(oXml))
			CloseR(oXml);
		else
			wrP(oXml, sSelectors, oTS);

		// Перекрестная ссылка внутри файла
		if(bCross)
		{
			m_oState.m_bInHyperlink = true;
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
			NSStringUtils::CStringBuilder* oRelationshipXml = &m_oDocXmlRels;
			if (oXml == &m_oNoteXml)
				oRelationshipXml = &m_oNoteXmlRels;
			oRelationshipXml->WriteString(L"<Relationship Id=\"rHyp");
			oRelationshipXml->WriteString(std::to_wstring(m_nHyperlinkId));
			oRelationshipXml->WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
			oRelationshipXml->WriteEncodeXmlString(sRef);
			oRelationshipXml->WriteString(L"\" TargetMode=\"External\"/>");

			m_oState.m_bInHyperlink = true;
			// Пишем в document.xml
			oXml->WriteString(L"<w:hyperlink w:tooltip=\"");
			oXml->WriteEncodeXmlString(sNote);
			oXml->WriteString(L"\" r:id=\"rHyp");
			oXml->WriteString(std::to_wstring(m_nHyperlinkId++));
		}
		oXml->WriteString(L"\">");

		if(!readStream(oXml, sSelectors, oTS))
		{
			OpenR(oXml);
			wrRPr(oXml, sSelectors, oTS);
			OpenT(oXml);
			oXml->WriteEncodeXmlString(!sAlt.empty() ? sAlt : L" ");
			CloseT(oXml);
			CloseR(oXml);
		}

		if (m_oState.m_bInP)
		{
			if (m_oState.m_bInHyperlink)
			{
				CloseT(oXml);
				CloseR(oXml);
				oXml->WriteString(L"</w:hyperlink>");
				m_oState.m_bInHyperlink = false;
			}

			bool bFootnote = false;
			if (sSelectors.size() > 1)
			{
				const NSCSS::CNode& oNode = sSelectors[sSelectors.size() - 2];
				bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
			}

			// Сноска
			if (bCross && !sFootnote.empty())
			{
				if (!bFootnote)
				{
					std::wstring sFootnoteID = std::to_wstring(m_nFootnoteId++);
					OpenR(oXml);
					oXml->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
					oXml->WriteString(sFootnoteID);
					oXml->WriteString(L"\"/>");
					CloseR(oXml);
					m_mFootnotes.insert(std::make_pair(sFootnote, sFootnoteID));
				}
				else
				{
					OpenR(oXml);
					oXml->WriteString(L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/>");
					CloseR(oXml);
				}
			}

			// CloseP(oXml, sSelectors);
		}

		sNote.clear();

		return true;
	}

	bool readBase64 (const std::wstring& sSrcM, std::wstring& sExtention)
	{
		bool bRes = false;
		size_t nBase = sSrcM.find(L"/", 4);
		nBase++;

		size_t nEndBase = sSrcM.find(L";", nBase);
		if (nEndBase == std::wstring::npos)
			return bRes;
		sExtention = sSrcM.substr(nBase, nEndBase - nBase);
		if (sExtention == L"octet-stream")
			sExtention = L"jpg";

		if (NotValidExtension(sExtention))
			return bRes;

		nBase = sSrcM.find(L"base64", nEndBase);
		if (nBase == std::wstring::npos)
			return bRes;

		int nOffset = nBase + 7;
		int nSrcLen = (int)(sSrcM.length() - nBase + 1);
		int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
		if (nDecodeLen != 0)
		{
			BYTE* pImageData = new BYTE[nDecodeLen];

			if (!pImageData || FALSE == NSBase64::Base64Decode(sSrcM.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
				return bRes;

			if (L"svg" == sExtention || L"svg+xml" == sExtention)
			{
				std::wstring wsSvg(pImageData, pImageData + nDecodeLen);
				bRes = readSVG(wsSvg);
				sExtention = L"png";
			}
			else
			{
				NSFile::CFileBinary oImageWriter;
				std::wstring sImageName = std::to_wstring(m_arrImages.size()) + L'.' + sExtention;

				if (oImageWriter.CreateFileW(m_sDst + L"/word/media/i" + sImageName))
					bRes = oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);

				oImageWriter.CloseFile();
			}

			RELEASEARRAYOBJECTS(pImageData);
		}

		return bRes;
	}

	bool CopyImage  (const std::wstring& wscSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles)
	{
		bool bRes = false;
		bool bAllow = true;

		std::wstring wsSrc = wscSrc;
		if (!bIsAllowExternalLocalFiles)
		{
			wsSrc = NSSystemPath::NormalizePath(wsSrc);
			std::wstring wsStartSrc = NSSystemPath::NormalizePath(m_sSrc);
			bAllow = wsSrc.substr(0, wsStartSrc.length()) == wsStartSrc;
		}
		if (bAllow)
			bRes = NSFile::CFileBinary::Copy(wsSrc, wsDst);

		return bRes;
	}

	inline bool NotValidExtension(const std::wstring& sExtention)
	{
		return  sExtention != L"bmp" && sExtention != L"emf"  && sExtention != L"emz"  && sExtention != L"eps"  && sExtention != L"fpx" && sExtention != L"gif"  &&
				sExtention != L"jpe" && sExtention != L"jpeg" && sExtention != L"jpg"  && sExtention != L"jfif" && sExtention != L"pct" && sExtention != L"pict" &&
				sExtention != L"png" && sExtention != L"pntg" && sExtention != L"psd"  && sExtention != L"qtif" && sExtention != L"sgi" && sExtention != L"svg"  &&
				sExtention != L"tga" && sExtention != L"tpic" && sExtention != L"tiff" && sExtention != L"tif"  && sExtention != L"wmf" && sExtention != L"wmz";
	}

	void ImageAlternative(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& wsAlt, const std::wstring& wsSrc, const TImageData& oImageData)
	{
		m_oDocXmlRels.WriteString(L"<Relationship Id=\"rId");
		m_oDocXmlRels.WriteString(std::to_wstring(m_nId));
		m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"");
		m_oDocXmlRels.WriteEncodeXmlString(wsSrc);
		m_oDocXmlRels.WriteString(L"\" TargetMode=\"External\"/>");

		const bool bOpenedP{OpenP(oXml)};

		WriteEmptyImage(oXml, (0 != oImageData.m_unWidth) ? oImageData.m_unWidth : DEFAULT_IMAGE_WIDTH, (0 != oImageData.m_unHeight) ? oImageData.m_unHeight : DEFAULT_IMAGE_HEIGHT, L"", wsAlt);

		if (bOpenedP)
			CloseP(oXml, sSelectors);
	}

	bool readImage  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)
	{
		std::wstring wsAlt, sSrcM;
		bool bRes = false;
		TImageData oImageData;

		#define READ_IMAGE_DATA(data) \
		{ \
			NSCSS::NSProperties::CDigit oDigit; \
			if (oDigit.SetValue(m_oLightReader.GetText())) \
			{ \
				if (NSCSS::UnitMeasure::None == oDigit.GetUnitMeasure()) \
					data = static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(oDigit.ToDouble(), NSCSS::Inch, 96) * 914400); \
				else \
					data = static_cast<int>(oDigit.ToDouble(NSCSS::Inch) * 914400); \
			} \
		}\

		while (m_oLightReader.MoveToNextAttribute())
		{
			const std::wstring wsName = m_oLightReader.GetName();
			if (wsName == L"alt")
				wsAlt = m_oLightReader.GetText();
			else if (wsName == L"src")
				sSrcM = m_oLightReader.GetText();
			else if (wsName == L"width")
				READ_IMAGE_DATA(oImageData.m_unWidth)
			else if (wsName == L"height")
				READ_IMAGE_DATA(oImageData.m_unHeight)
			else if (wsName == L"hspace")
				READ_IMAGE_DATA(oImageData.m_nHSpace)
			else if (wsName == L"vspace")
				READ_IMAGE_DATA(oImageData.m_nVSpace)
		}
		m_oLightReader.MoveToElement();

		if (sSrcM.empty())
		{
			ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM, oImageData);
			return true;
		}

		const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

		bool bIsBase64 = false;
		if (sSrcM.length() > 4 && sSrcM.substr(0, 4) == L"data" && sSrcM.find(L"/", 4) != std::wstring::npos)
			bIsBase64 = true;

		if (!bIsBase64 && (sSrcM.length() <= 7 || L"http" != sSrcM.substr(0, 4)))
		{
			sSrcM = NSSystemPath::ShortenPath(sSrcM);

			if (!CanUseThisPath(sSrcM, m_sSrc, m_sCore, bIsAllowExternalLocalFiles))
				return true;
		}

		int nImageId = -1;
		std::wstring sImageSrc, sExtention;
		// Предполагаем картинку в Base64
		if (bIsBase64)
			bRes = readBase64(sSrcM, sExtention);

		if (!bRes)
		{
			// Проверка расширения
			sExtention = NSFile::GetFileExtention(sSrcM);
			std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
			if (NotValidExtension(sExtention))
			{
				ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM, oImageData);
				return true;
			}

			// Проверка на повтор
			std::vector<std::wstring>::iterator nFind = std::find(m_arrImages.begin(), m_arrImages.end(), sSrcM);
			if (nFind != m_arrImages.end())
			{
				bRes = true;
				nImageId = nFind - m_arrImages.begin();
			}
		}

		if (!bRes)
		{
			sImageSrc = sSrcM;
			std::wstring wsDst = m_sDst + L"/word/media/i" + std::to_wstring(m_arrImages.size()) + L'.' + sExtention;

			// Предполагаем картинку по локальному пути
			if (!((!m_sBase.empty() && m_sBase.length() > 4 && m_sBase.substr(0, 4) == L"http") || (sSrcM.length() > 4 && sSrcM.substr(0, 4) == L"http")))
			{
				if (!m_sBase.empty())
				{
					if (!bRes)
						bRes = CopyImage(NSSystemPath::Combine(m_sBase, sSrcM), wsDst, bIsAllowExternalLocalFiles);
					if (!bRes)
						bRes = CopyImage(NSSystemPath::Combine(m_sSrc, m_sBase + sSrcM), wsDst, bIsAllowExternalLocalFiles);
				}
				if (!bRes)
					bRes = CopyImage(NSSystemPath::Combine(m_sSrc, sSrcM), wsDst, bIsAllowExternalLocalFiles);
				if (!bRes)
					bRes = CopyImage(m_sSrc + L"/" + NSFile::GetFileName(sSrcM), wsDst, bIsAllowExternalLocalFiles);
				if (!bRes)
					bRes = CopyImage(sSrcM, wsDst, bIsAllowExternalLocalFiles);
			}
			// Предполагаем картинку в сети
			else
			{
				// Проверка gc_allowNetworkRequest предполагается в kernel_network
				NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
				oDownloadImg.SetFilePath(wsDst);
				bRes = oDownloadImg.DownloadSync();
			}
		}

		if (!bRes)
			ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM, oImageData);
		else
		{
			wrP(oXml, sSelectors, oTS);
			ImageRels(oXml, nImageId, sImageSrc, sExtention, oImageData);
		}

		return true;
	}

	std::wstring wrP(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, CTextSettings& oTS)
	{
		OpenP(oXml);

		if (m_oState.m_bWasPStyle)
			return L"";

		m_oStylesCalculator.CalculateCompiledStyle(sSelectors);

		std::wstring sPStyle = GetStyle(*sSelectors.back().m_pCompiledStyle, true);

		if (sPStyle.empty() && !ElementInTable(sSelectors))
			sPStyle = L"normal-web";

		if (sPStyle.empty() && oTS.sPStyle.empty() && 0 > oTS.nLi)
			return L"";

		m_oXmlStyle.WriteLitePStyle(oTS.oAdditionalStyle);
		const std::wstring sPSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		oXml->WriteNodeBegin(L"w:pPr");

		if (!sPStyle.empty())
		{
			oXml->WriteString(L"<w:pStyle w:val=\"");
			oXml->WriteString(sPStyle);
			oXml->WriteString(L"\"/>");
		}

		if (oTS.nLi >= 0)
		{
			if (!oTS.bWritedLi)
			{
				oXml->WriteString(L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(oTS.nLi) + L"\"/><w:numId w:val=\"" +
				                  (!oTS.bNumberingLi ? L"1" : std::to_wstring(m_nNumberingId + 1)) + L"\"/></w:numPr>");

				oTS.bWritedLi = true;
			}
			else if (sSelectors.back().m_pCompiledStyle->m_oText.GetIndent().Empty() &&
			         oTS.oAdditionalStyle.m_oText.GetIndent().Empty())
				oXml->WriteString(L"<w:ind w:left=\"" + std::to_wstring(720 * (oTS.nLi + 1)) + L"\"/>");
		}

		oXml->WriteString(oTS.sPStyle + sPSettings);
		oXml->WriteNodeEnd(L"w:pPr");
		m_oState.m_bWasPStyle = true;

		return sPStyle;
	}

	std::wstring wrRPr(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if (!m_oState.m_bInP)
			return L"";

		m_oStylesCalculator.CalculateCompiledStyle(sSelectors);

		std::wstring sRStyle = GetStyle(*sSelectors.back().m_pCompiledStyle, false);

		m_oXmlStyle.WriteLiteRStyle(oTS.oAdditionalStyle);
		const std::wstring sRSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		std::wstring wsFontSize;

		const int nCalculatedFontChange{CalculateFontChange(sSelectors)};

		if (0 != nCalculatedFontChange)
		{
			int nFontSizeLevel{static_cast<int>((sSelectors.back().m_pCompiledStyle->m_oFont.Empty()) ? 3 : GetFontSizeLevel(sSelectors.back().m_pCompiledStyle->m_oFont.GetSize().ToInt(NSCSS::Point) * 2))};

			nFontSizeLevel += nCalculatedFontChange;

			const UINT unFontSize{GetFontSizeByLevel(nFontSizeLevel)};

			wsFontSize += L"<w:sz w:val=\"" + std::to_wstring(unFontSize) + L"\"/><w:szCs w:val=\"" + std::to_wstring(unFontSize) + L"\"/>";
		}

		if (!sRStyle.empty() || CTextSettings::Normal != oTS.eTextMode || !wsFontSize.empty() || !sRSettings.empty())
		{
			oXml->WriteString(L"<w:rPr>");
			if (!sRStyle.empty())
			{
				oXml->WriteString(L"<w:rStyle w:val=\"");
				oXml->WriteString(sRStyle);
				oXml->WriteString(L"\"/>");
			}

			switch (oTS.eTextMode)
			{
				case CTextSettings::Subscript:
				{
					oXml->WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
					break;
				}
				case CTextSettings::Superscript:
				{
					oXml->WriteString(L"<w:vertAlign w:val=\"superscript\"/>");
					break;
				}
				default:
					break;
			}

			oXml->WriteString(sRSettings + wsFontSize);
			oXml->WriteString(L"</w:rPr>");
		}
		return sRStyle;
	}

	void WriteImage(NSStringUtils::CStringBuilder* pXml, const TImageData& oImageData, const std::wstring& wsId)
	{
		if (NULL == pXml)
			return;

		OpenR(pXml);

		// Пишем в document.xml
		if (oImageData.ZeroSpaces())
		{
			pXml->WriteString(L"<w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"");
			pXml->WriteString(std::to_wstring(oImageData.m_unWidth));
			pXml->WriteString(L"\" cy=\"");
			pXml->WriteString(std::to_wstring(oImageData.m_unHeight));
			pXml->WriteString(L"\"/><wp:docPr id=\"");
			pXml->WriteString(wsId);
			pXml->WriteString(L"\" name=\"Picture " + wsId + L"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"");
			pXml->WriteString(wsId);
			pXml->WriteString(L"\" name=\"Picture " + wsId + L"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic");
			pXml->WriteString(wsId);
			pXml->WriteString(L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"");
			pXml->WriteString(std::to_wstring(oImageData.m_unWidth));
			pXml->WriteString(L"\" cy=\"");
			pXml->WriteString(std::to_wstring(oImageData.m_unHeight));
			pXml->WriteString(L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>");
		}
		else
		{
			pXml->WriteString(L"<w:rPr><w:noProof/></w:rPr><w:drawing>");
			pXml->WriteString(L"<wp:anchor distT=\"" + std::to_wstring(oImageData.m_nHSpace) + L"\" distB=\"" + std::to_wstring(oImageData.m_nHSpace) + L"\" distL=\"" + std::to_wstring(oImageData.m_nVSpace) + L"\" distR=\"" + std::to_wstring(oImageData.m_nVSpace) + L"\" simplePos=\"0\" relativeHeight=\"251658240\" behindDoc=\"0\" locked=\"0\" layoutInCell=\"1\" allowOverlap=\"0\">");
			pXml->WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");
			pXml->WriteString(L"<wp:positionH relativeFrom=\"column\"><wp:align>" + oImageData.m_wsAlign + L"</wp:align></wp:positionH>");
			pXml->WriteString(L"<wp:positionV relativeFrom=\"line\"><wp:posOffset>0</wp:posOffset></wp:positionV>");
			pXml->WriteString(L"<wp:extent cx=\"" + std::to_wstring(oImageData.m_unWidth) + L"\" cy=\"" + std::to_wstring(oImageData.m_unHeight) + L"\"/>");
			pXml->WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
			pXml->WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
			pXml->WriteString(L"<wp:docPr id=\"" + wsId + L"\" name=\"Picture " + wsId + L"\"/>");
			pXml->WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
			pXml->WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");
			pXml->WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
			pXml->WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
			pXml->WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"" + wsId + L"\" name=\"Picture " + wsId + L"\"/>");
			pXml->WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
			pXml->WriteString(L"<pic:blipFill><a:blip r:link=\"rPic" + wsId + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
			pXml->WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(oImageData.m_unWidth) + L"\" cy=\"" + std::to_wstring(oImageData.m_unHeight) + L"\"/></a:xfrm>");
			pXml->WriteString(L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr></pic:pic></a:graphicData></a:graphic>");
			pXml->WriteString(L"<wp14:sizeRelH relativeFrom=\"page\"><wp14:pctWidth>0</wp14:pctWidth></wp14:sizeRelH><wp14:sizeRelV relativeFrom=\"page\"><wp14:pctHeight>0</wp14:pctHeight></wp14:sizeRelV>");
			pXml->WriteString(L"</wp:anchor></w:drawing>");
		}

		CloseR(pXml);
	}

	#define WRITE_ENCODE_ARGUMENT(ptrStringBuilder, argumentName, argumentValue) \
	ptrStringBuilder->WriteString(L" " + std::wstring(argumentName) + L"=\""); \
	ptrStringBuilder->WriteEncodeXmlString(argumentValue); \
	ptrStringBuilder->WriteString(L"\"")

	void WriteEmptyImage(NSStringUtils::CStringBuilder* pXml, int nWidth, int nHeight, const std::wstring& wsName = L"", const std::wstring& wsDescr = L"")
	{
		if (NULL == pXml)
			return;

		OpenR(pXml);

		pXml->WriteString(L"<w:rPr><w:noProof/></w:rPr><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
		pXml->WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_nId - 7) + L"\"");
		WRITE_ENCODE_ARGUMENT(pXml, L"name", wsName);
		WRITE_ENCODE_ARGUMENT(pXml, L"descr", wsDescr);
		pXml->WriteString(L"/>");
		pXml->WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
		pXml->WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
		pXml->WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"0\"");
		WRITE_ENCODE_ARGUMENT(pXml, L"name", wsName);
		WRITE_ENCODE_ARGUMENT(pXml, L"descr", wsDescr);
		pXml->WriteString(L"/>");
		pXml->WriteString(L"<pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
		pXml->WriteString(L"<pic:blipFill><a:blip r:link=\"rId" + std::to_wstring(m_nId++) + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
		pXml->WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr>");
		pXml->WriteString(L"</pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing>");

		CloseR(pXml);
	}

	void ImageRels  (NSStringUtils::CStringBuilder* oXml, int nImageId, const std::wstring& sImageSrc, const std::wstring& sExtention, const TImageData& oImageData = TImageData())
	{
		bool bNew = nImageId < 0;
		if (bNew)
			nImageId = m_arrImages.size();

		std::wstring sImageId = std::to_wstring(nImageId);
		std::wstring sImageName = sImageId + L'.' + sExtention;
		CBgraFrame oBgraFrame;
		if (!oBgraFrame.OpenFile(m_sDst + L"/word/media/i" + sImageName))
		{
			NSFile::CFileBinary::Remove(m_sDst + L"/word/media/i" + sImageName);
			return;
		}
		// Прописать рельсы
		if (bNew)
		{
			m_arrImages.push_back(sImageSrc);

			m_oDocXmlRels.WriteString(L"<Relationship Id=\"rPic");
			m_oDocXmlRels.WriteString(sImageId);
			m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/i");
			m_oDocXmlRels.WriteEncodeXmlString(sImageName);
			m_oDocXmlRels.WriteString(L"\"/>");
		}

		if (!oImageData.ZeroSize())
			return WriteImage(oXml, oImageData, sImageId);

		TImageData oNewImageData{oImageData};

		// Получаем размеры картинки
		oNewImageData.m_unWidth  = oBgraFrame.get_Width();
		oNewImageData.m_unHeight = oBgraFrame.get_Height();

		if (oNewImageData.m_unWidth > oNewImageData.m_unHeight)
		{
			int nW = oNewImageData.m_unWidth * 9525;
			nW = (nW > 7000000 ? 7000000 : nW);
			oNewImageData.m_unHeight = (int)((double)oNewImageData.m_unHeight * (double)nW / (double)oNewImageData.m_unWidth);
			oNewImageData.m_unWidth = nW;
		}
		else
		{
			int nH = oNewImageData.m_unHeight * 9525;
			nH = (nH > 8000000 ? 8000000 : nH);
			int nW = (int)((double)oNewImageData.m_unWidth * (double)nH / (double)oNewImageData.m_unHeight);
			if (nW > 7000000)
			{
				nW = 7000000;
				oNewImageData.m_unHeight = (int)((double)oNewImageData.m_unHeight * (double)nW / (double)oNewImageData.m_unWidth);
			}
			else
				oNewImageData.m_unHeight = nH;
			oNewImageData.m_unWidth = nW;
		}

		WriteImage(oXml, oNewImageData, sImageId);
	}

	void readNote   (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, std::wstring& sNote)
	{
		if(sNote.empty())
			return;

		OpenP(oXml);

		oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
		oXml->WriteString(std::to_wstring(m_nFootnoteId));
		oXml->WriteString(L"\"/></w:r>");
		m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
		m_oNoteXml.WriteString(std::to_wstring(m_nFootnoteId++));
		m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
		m_oNoteXml.WriteEncodeXmlString(sNote);
		m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");

		sNote.clear();
	}

	bool readSVG    (const std::wstring& wsSvg)
	{
		if (wsSvg.empty())
			return false;

		if (NULL == m_pFonts)
			m_pFonts = NSFonts::NSApplication::Create();

		MetaFile::IMetaFile* pSvgReader = MetaFile::Create(m_pFonts);
		if (!pSvgReader->LoadFromString(wsSvg))
		{
			RELEASEINTERFACE(pSvgReader);
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

		BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		if (!pBgraData)
		{
			double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

			nWidth = (int)(dKoef * nWidth);
			nHeight = (int)(dKoef * nHeight);

			dWidth  = 25.4 * nWidth / 96;
			dHeight = 25.4 * nHeight / 96;

			pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
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

		// TODO: src directory as tmp - it's not good idea
		pSvgReader->SetTempDirectory(m_sSrc);
		pSvgReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(m_sDst + L"/word/media/i" + std::to_wstring(m_arrImages.size()) + L".png", 4);
		oFrame.put_Data(NULL);

		RELEASEINTERFACE(pGrRenderer);

		if (pBgraData)
			free(pBgraData);

		RELEASEINTERFACE(pSvgReader);

		return true;
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
	m_internal->m_sTmp = NSSystemPath::NormalizePath(sFolder);
}

void CHtmlFile2::SetCoreDirectory(const std::wstring& wsFolder)
{
	m_internal->m_sCore = NSSystemPath::NormalizePath(wsFolder);
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

		// Очищаем разрешенные файлы стилей
		// Это необходимо, чтобы мы не могли взять стили из не подключенного файла, но при этом, чтобы данные оставались,
		// т.к. ко многим файлам может быть подключен один и тот же файл (проблема возникает когда он большой)
		// и подключать (в нашем случае заново парсить) его будет долго
		m_internal->m_oStylesCalculator.ClearAllowedStyleFiles();
		m_internal->m_oStylesCalculator.ClearEmbeddedStyles();
	}

	m_internal->write();
	return S_OK;
}

std::wstring CTableRow::ConvertToOOXML(const CTable& oTable, int nInstruction)
{
	if (m_arCells.empty())
		return std::wstring();

	NSStringUtils::CStringBuilder oRow;
	oRow.WriteNodeBegin(L"w:tr");

	const TTableStyles oTableStyles{oTable.GetTableStyles()};

	if (!m_oStyles.Empty() || 0 < oTableStyles.m_nCellSpacing)
	{
		oRow.WriteNodeBegin(L"w:trPr");

		if (m_oStyles.m_bIsHeader)
			oRow += L"<w:tblHeader/>";

		if (0 < m_oStyles.m_unMaxHeight)
			oRow += L"<w:trHeight w:val=\"" + std::to_wstring(m_oStyles.m_unMaxHeight) + L"\"/>";

		if (0 < oTableStyles.m_nCellSpacing)
			oRow += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";

		oRow.WriteNodeEnd(L"w:trPr");
	}

	for (UINT unIndex = 0; unIndex < m_arCells.size(); ++unIndex)
	{
		int nNewInstruction{nInstruction};

		if (0 == unIndex)
			nNewInstruction |= FIRST_ELEMENT;
		if (m_arCells.size() - 1 == unIndex)
			nNewInstruction |= LAST_ELEMENT;
		else if (0 != unIndex)
			nNewInstruction |= MID_ELEMENT;

		oRow += m_arCells[unIndex]->ConvertToOOXML(oTable, unIndex, nNewInstruction);
	}

	oRow.WriteNodeEnd(L"w:tr");

	return oRow.GetData();
}

std::wstring CTableCell::ConvertToOOXML(const CTable& oTable, UINT unColumnNumber, int nInstruction)
{
	NSStringUtils::CStringBuilder oCell;

	oCell.WriteNodeBegin(L"w:tc");
	oCell.WriteNodeBegin(L"w:tcPr");

	if (PARSE_MODE_HEADER == (nInstruction & PARSE_MODE_MASK))
		oCell += L"<w:tblHeader/>";

	TTableCellStyle oCellStyle(m_oStyles);

	const TTableCellStyle* pColStyle = oTable.GetColStyle(unColumnNumber);

	if (NULL != pColStyle)
		oCellStyle += pColStyle;

	if (!oCellStyle.m_oWidth.Empty())
	{
		if (NSCSS::UnitMeasure::Percent == oCellStyle.m_oWidth.GetUnitMeasure())
			oCell += L"<w:tcW w:w=\"" + std::to_wstring(oCellStyle.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
		else
		{
			if (!oCellStyle.m_oWidth.Zero())
			{
				int nWidth;
				if (NSCSS::UnitMeasure::None != oCellStyle.m_oWidth.GetUnitMeasure())
					nWidth = oCellStyle.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips);
				else
					nWidth = static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(oCellStyle.m_oWidth.ToDouble(), NSCSS::UnitMeasure::Twips, 96) + 0.5);

				oCell += L"<w:tcW w:w=\"" + std::to_wstring(nWidth) + L"\" w:type=\"dxa\"/>";
			}
			else
				oCell += L"<w:tcW w:w=\"6\" w:type=\"dxa\"/>";
		}
	}
	else
		oCell += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";

	if (1 != m_unColspan)
		oCell += L"<w:gridSpan w:val=\"" + std::to_wstring(m_unColspan) + L"\"/>";

	if (m_bIsMerged)
		oCell += L"<w:vMerge w:val=\"continue\"/>";
	else if (1 < m_unRowSpan)
		oCell += L"<w:vMerge w:val=\"restart\"/>";

	const TTableStyles oTableStyles{oTable.GetTableStyles()};

	if (!oCellStyle.m_oBorder.Empty() && !oCellStyle.m_oBorder.Zero() /*&& oCellStyle.m_oBorder != oTableStyles.m_oBorder*/)
		oCell += L"<w:tcBorders>" + CreateBorders(oCellStyle.m_oBorder, &oCellStyle.m_oPadding) + L"</w:tcBorders>";
	else if (TTableStyles::ETableRules::Groups == oTable.GetTableStyles().m_enRules)
	{
		std::wstring wsBorders;

		if (oTable.HaveColgroups())
			wsBorders += oTable.CalculateSidesToClean(unColumnNumber);

		if (PARSE_MODE_HEADER == (nInstruction & PARSE_MODE_MASK) && (((nInstruction & ROW_POSITION_MASK) >> 4) & LAST_ELEMENT))
			wsBorders += CreateDefaultBorder(L"bottom");
		else if (PARSE_MODE_FOOTHER == (nInstruction & PARSE_MODE_MASK) && (((nInstruction & ROW_POSITION_MASK) >> 4) & FIRST_ELEMENT))
			wsBorders += CreateDefaultBorder(L"top");

		if (!wsBorders.empty())
			oCell += L"<w:tcBorders>" + wsBorders + L"</w:tcBorders>";
	}

	if (!oCellStyle.m_oBackground.Empty())
	{
		const std::wstring wsShdFill{(NSCSS::NSProperties::ColorNone == oCellStyle.m_oBackground.GetType()) ? L"auto" : oCellStyle.m_oBackground.ToWString()};
		oCell += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + wsShdFill + L"\"/>";
	}

	if (!oCellStyle.m_wsVAlign.empty())
		oCell += L"<w:vAlign w:val=\"" + oCellStyle.m_wsVAlign + L"\"/>";
	else
		oCell += L"<w:vAlign w:val=\"center\"/>";

	if (!oCellStyle.m_oPadding.Empty() && oTableStyles.m_oPadding != oCellStyle.m_oPadding)
	{
		const int nTopPadding    = std::max(oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oCellStyle  .m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nLeftPadding   = std::max(oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oCellStyle  .m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));
		const int nBottomPadding = std::max(oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
		                                    oCellStyle  .m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
		const int nRightPadding  = std::max(oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
		                                    oCellStyle  .m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));

		oCell += L"<w:tcMar>"
		              "<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>"
		              "<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>"
		              "<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>"
		              "<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>"
		          "</w:tcMar>";
	}

	oCell += L"<w:hideMark/>";
	oCell.WriteNodeEnd(L"w:tcPr");

	if (0 != m_oData.GetCurSize())
		oCell += m_oData.GetData();
	else
		WriteEmptyParagraph(&oCell);

	oCell.WriteNodeEnd(L"w:tc");

	return oCell.GetData();
}

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
#include "../DesktopEditor/common/ProcessEnv.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/raster/Metafile/svg/CSvgFile.h"

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

#define SAVE_NORMALIZED_HTML 0

std::wstring rStyle = L" a area b strong bdo bdi big br center cite dfn em i var code kbd samp tt del s font img ins u mark q rt sup small sub svg input basefont button label data object noscript output abbr time ruby progress hgroup meter span acronym ";

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
	bool bAddSpaces; // Добавлять пробелы перед текстом?
	bool bMergeText; // Объединять подяр идущий текст в 1?
	int  nLi;  // Уровень списка
	std::wstring sRStyle; // w:rStyle
	std::wstring sPStyle; // w:pStyle

	CTextSettings(bool _bBdo, bool _bPre, bool _bAddSpaces, bool _bMergeText, int _nLi, const std::wstring& _sRStyle, const std::wstring& _sPStyle) :
		bBdo(_bBdo), bPre(_bPre), bAddSpaces(_bAddSpaces), bMergeText(_bMergeText), nLi(_nLi), sRStyle(_sRStyle), sPStyle(_sPStyle) 
	{}

	CTextSettings(const CTextSettings& oTS) :
		bBdo(oTS.bBdo), bPre(oTS.bPre), bAddSpaces(oTS.bAddSpaces), bMergeText(oTS.bMergeText), nLi(oTS.nLi), sRStyle(oTS.sRStyle), sPStyle(oTS.sPStyle) 
	{}

	void AddRStyle(const std::wstring& wsStyle)
	{
		if (std::wstring::npos == sRStyle.find(wsStyle))
			sRStyle += wsStyle;
	}

	void AddPStyle(const std::wstring& wsStyle)
	{
		if (std::wstring::npos == sPStyle.find(wsStyle))
			sPStyle += wsStyle;
	}
};

std::wstring CreateBorders(const NSCSS::NSProperties::CBorder& oBorder, const NSCSS::NSProperties::CIndent* pPadding = NULL)
{
	if (oBorder.EqualSides() && (NULL == pPadding || pPadding->Equals()))
	{
		const std::wstring wsBorderStyle = NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))));

		return L"<w:top "    + wsBorderStyle + L"/>" +
			   L"<w:left "   + wsBorderStyle + L"/>" +
			   L"<w:bottom " + wsBorderStyle + L"/>" +
			   L"<w:right "  + wsBorderStyle + L"/>";
	}
	else
	{
		std::wstring wsTable;

		if (oBorder.GetTopBorder().Valid())
			wsTable += L"<w:top "    + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetTopBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetTop()))))       + L"/>";

		if (oBorder.GetLeftBorder().Valid())
			wsTable += L"<w:left "   + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetLeft()))))     + L"/>";

		if (oBorder.GetBottomBorder().Valid())
			wsTable += L"<w:bottom " + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetBottomBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetBottom())))) + L"/>";

		if (oBorder.GetRightBorder().Valid())
			wsTable += L"<w:right "  + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetRightBorder(), ((NULL == pPadding) ? NULL : (&(pPadding->GetRight()))))   + L"/>";

		return wsTable;
	}

	return L"";
}

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

void WriteLine(NSStringUtils::CStringBuilder* pXml, double dHeight, const std::wstring& wsColor)
{
	pXml->WriteNodeBegin(L"w:pict");
	pXml->WriteString(L"<v:rect style=\"width:0;height:" + std::to_wstring(dHeight) + L"pt\" o:hralign=\"center\" o:hrstd=\"t\" o:hr=\"t\" fillcolor=\"#" + wsColor + L"\" stroked=\"f\"/>");
	pXml->WriteNodeEnd(L"w:pict");
}

bool ElementInTable(const std::vector<NSCSS::CNode>& arSelectors)
{
	return arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const NSCSS::CNode& oNode) { return L"table" == oNode.m_wsName; });
}

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
	bool m_bHaveBorderAttribute;

	std::wstring m_wsAlign;

	TTableStyles()
		: m_nCellSpacing(-1), m_bHaveBorderAttribute(false)
	{}

	bool Empty() const
	{
		return m_oPadding.Empty() && m_oMargin.Empty() && m_oBorder.Empty() && m_oWidth.Empty() && -1 == m_nCellSpacing && false == m_bHaveBorderAttribute && m_wsAlign.empty();
	}
};

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
};

class CTableCell
{
public:
	CTableCell() 
		: m_unColspan(1), m_unRowSpan(1), m_bIsMerged(false), m_bIsEmpty(false), m_enMode(ParseModeBody)
	{}

	CTableCell(UINT unColspan, UINT unRowspan, bool bIsMerged, bool bIsEmpty)
		: m_unColspan(unColspan), m_unRowSpan(unRowspan), m_bIsMerged(bIsMerged), m_bIsEmpty(bIsEmpty), m_enMode(ParseModeBody)
	{}

	CTableCell(CTableCell& oCell)
		: m_unColspan(oCell.m_unColspan), m_unRowSpan(oCell.m_unRowSpan), m_bIsMerged(oCell.m_bIsMerged), 
		  m_bIsEmpty(oCell.m_bIsEmpty), m_enMode(oCell.m_enMode), m_oStyles(oCell.m_oStyles)
	{
		m_oData.SetText(oCell.m_oData.GetData());
	}

	bool Empty()
	{
		return m_bIsEmpty;
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

	void SetMode(ERowParseMode eMode)
	{
		m_enMode = eMode;
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

	std::wstring ConvertToOOXML(const TTableStyles& oTableStyles)
	{
		NSStringUtils::CStringBuilder oCell;

		oCell.WriteNodeBegin(L"w:tc");
		oCell.WriteNodeBegin(L"w:tcPr");

		if (ParseModeHeader == m_enMode)
			oCell += L"<w:tblHeader/>";

		if (!m_oStyles.m_oWidth.Empty())
		{
			if (NSCSS::UnitMeasure::Percent == m_oStyles.m_oWidth.GetUnitMeasure())
				oCell += L"<w:tcW w:w=\"" + std::to_wstring(m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
			{
				if (!m_oStyles.m_oWidth.Zero())
				{
					int nWidth;
					if (NSCSS::UnitMeasure::None != m_oStyles.m_oWidth.GetUnitMeasure())
						nWidth = m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips);
					else
						nWidth = static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(m_oStyles.m_oWidth.ToDouble(), NSCSS::UnitMeasure::Twips, 96) + 0.5);

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

		if (!m_oStyles.m_oBorder.Zero() && !m_oStyles.m_oBorder.Empty())
			oCell += L"<w:tcBorders>" + CreateBorders(m_oStyles.m_oBorder) + L"</w:tcBorders>";
		else if (oTableStyles.m_bHaveBorderAttribute)
			oCell += L"<w:tcBorders><w:top w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:left w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:bottom w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:right w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/></w:tcBorders>";

		if (!m_oStyles.m_oBackground.Empty())
		{
			const std::wstring wsShdFill{(NSCSS::NSProperties::ColorNone == m_oStyles.m_oBackground.GetType()) ? L"auto" : m_oStyles.m_oBackground.ToWString()};
			oCell += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + wsShdFill + L"\"/>";
		}

		if (!m_oStyles.m_wsVAlign.empty())
			oCell += L"<w:vAlign w:val=\"" + m_oStyles.m_wsVAlign + L"\"/>";
		else
			oCell += L"<w:vAlign w:val=\"center\"/>";

		if (!m_oStyles.m_oPadding.Empty() && oTableStyles.m_oPadding != m_oStyles.m_oPadding)
		{
			const int nTopPadding    = std::max(oTableStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
			                                    m_oStyles   .m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nLeftPadding   = std::max(oTableStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
			                                    m_oStyles   .m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));
			const int nBottomPadding = std::max(oTableStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
			                                    m_oStyles   .m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nRightPadding  = std::max(oTableStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH),
			                                    m_oStyles   .m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH));

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

private:
	UINT m_unColspan;
	UINT m_unRowSpan;

	bool m_bIsMerged;
	bool m_bIsEmpty;
	ERowParseMode m_enMode;

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
			std::vector<CTableCell*>::iterator itFoundEmpty = std::find_if(m_arCells.begin(), m_arCells.end(), [](CTableCell* pCell) { return pCell->Empty(); });

			if (m_arCells.end() != itFoundEmpty)
			{
				--m_oStyles.m_unMaxIndex;
				delete *itFoundEmpty;
				*itFoundEmpty = pCell;
				
				if (1 != pCell->GetColspan())
				{
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
			m_arCells[nPosition++] = pCell;

			if (1 != pCell->GetColspan())
			{
				UINT unDeleteCount =  pCell->GetColspan() - 1;
				while (m_arCells[nPosition]->Empty() && nPosition < m_arCells.size() && unDeleteCount > 0)
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

	std::wstring ConvertToOOXML(const TTableStyles& oTableStyles)
	{
		if (m_arCells.empty())
			return std::wstring();

		NSStringUtils::CStringBuilder oRow;
		oRow.WriteNodeBegin(L"w:tr");

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

		for (CTableCell* pCell : m_arCells)
			oRow += pCell->ConvertToOOXML(oTableStyles);

		oRow.WriteNodeEnd(L"w:tr");

		return oRow.GetData();
	}

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

class CTable
{
public:
	CTable()
	{}

	~CTable()
	{
		for (CTableRow* pRow : m_arRows)
			RELEASEOBJECT(pRow);
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

	UINT GetRowCount() const
	{
		return m_arRows.size();
	}

	void AddRow(CTableRow* pRow)
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

		m_arRows.push_back(pRow);
	}

	void AddCaption(NSStringUtils::CStringBuilder& oCaption)
	{
		m_oCaption += oCaption.GetData();
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

	void HaveBorderAttribute()
	{
		m_oStyles.m_bHaveBorderAttribute = true;
	}

	bool IsHaveBorderAttribute() const
	{
		return m_oStyles.m_bHaveBorderAttribute;
	}

	UINT GetMaxColumns()
	{
		UINT unMaxColumns = 0;

		for (const CTableRow* pRow : m_arRows)
			unMaxColumns = std::max(unMaxColumns, pRow->GetIndex());

		return unMaxColumns;
	}

	void Shorten()
	{
		UINT unIndex   = 0;
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

					pCell->SetColspan(unMaxIndex , MAXCOLUMNSINTABLE);
					continue;
				}

				if (1 == m_arMinColspan[unIndex])
					break;

				pCell = (*pRow)[unIndex];

				if (NULL == pCell)
					continue;

				if (1 < pCell->GetColspan() && unIndex + pCell->GetColspan() > m_arMinColspan[unIndex])
				{
					pCell->SetColspan(m_arMinColspan[unIndex] - unIndex, MAXCOLUMNSINTABLE);
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
			for (UINT unIndex = pRow->GetIndex(); unIndex < unMaxIndex; ++unIndex)
				pRow->InsertCell(CTableCell::CreateEmpty(), unIndex);
		}
	}

	std::wstring ConvertToOOXML()
	{
		if (m_arRows.empty())
			return std::wstring();

		NSStringUtils::CStringBuilder oTable;

		oTable.WriteNodeBegin(L"w:tbl");
		oTable.WriteNodeBegin(L"w:tblPr");

		if (!m_oStyles.m_oWidth.Empty() && !m_oStyles.m_oWidth.Zero())
		{
			if (NSCSS::UnitMeasure::Percent == m_oStyles.m_oWidth.GetUnitMeasure())
				oTable += L"<w:tblW w:w=\"" + std::to_wstring(m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oTable += L"<w:tblInd w:w=\"" + std::to_wstring(m_oStyles.m_oWidth.ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}
		else
			oTable += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

		if (!m_oStyles.m_oMargin.GetLeft().Empty() && !m_oStyles.m_oMargin.GetLeft().Zero())
		{
			if (NSCSS::UnitMeasure::Percent == m_oStyles.m_oMargin.GetLeft().GetUnitMeasure())
				oTable += L"<w:tblInd w:w=\"" + std::to_wstring(m_oStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			else
				oTable += L"<w:tblInd w:w=\"" + std::to_wstring(m_oStyles.m_oMargin.GetLeft().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}

		if (!m_oStyles.m_wsAlign.empty())
			oTable += L"<w:jc w:val=\"" + m_oStyles.m_wsAlign + L"\"/>";

		if (0 < m_oStyles.m_nCellSpacing && m_oStyles.m_oBorder.GetCollapse() != NSCSS::NSProperties::BorderCollapse::Collapse)
			oTable += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(m_oStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";

		if (!m_oStyles.m_oBorder.Empty() && !m_oStyles.m_oBorder.Zero())
			oTable += L"<w:tblBorders>" + CreateBorders(m_oStyles.m_oBorder) + L"</w:tblBorders>";

		if (!m_oStyles.m_oPadding.Empty() && !m_oStyles.m_oPadding.Zero())
		{
			const int nTopPadding    = std::max(0, m_oStyles.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nLeftPadding   = std::max(0, m_oStyles.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
			const int nBottomPadding = std::max(0, m_oStyles.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nRightPadding  = std::max(0, m_oStyles.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));

			oTable.WriteNodeBegin(L"w:tblCellMar");

			if (0 != nTopPadding)
				oTable += L"<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>";

			if (0 != nLeftPadding)
				oTable += L"<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>";

			if (0 != nBottomPadding)
				oTable += L"<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>";

			if (0 != nRightPadding)
				oTable += L"<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>";

			oTable.WriteNodeEnd(L"w:tblCellMar");
		}
		else
			oTable += L"<w:tblCellMar><w:top w:w=\"15\" w:type=\"dxa\"/><w:left w:w=\"15\" w:type=\"dxa\"/><w:bottom w:w=\"15\" w:type=\"dxa\"/><w:right w:w=\"15\" w:type=\"dxa\"/></w:tblCellMar>";

		oTable += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
		oTable.WriteNodeEnd(L"w:tblPr");

		if (HaveCaption())
		{
			oTable.WriteNodeBegin(L"w:tr");
			oTable.WriteNodeBegin(L"w:tc");
			oTable.WriteNodeBegin(L"w:tcPr");
			oTable += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";
			oTable += L"<w:gridSpan w:val=\"" + std::to_wstring(GetMaxColumns()) + L"\"/>";
			oTable += L"<w:tcBorders><w:top w:val=\"nil\"/><w:left w:val=\"nil\"/><w:bottom w:val=\"nil\"/><w:right w:val=\"nil\"/></w:tcBorders>";
			oTable += L"<w:vAlign w:val=\"center\"/>";
			oTable += L"<w:hideMark/>";
			oTable.WriteNodeEnd(L"w:tcPr");
			oTable.WriteString(m_oCaption.GetData());
			oTable.WriteNodeEnd(L"w:tc");
			oTable.WriteNodeEnd(L"w:tr");
		}

		for (CTableRow* pRow : m_arRows)
			oTable += pRow->ConvertToOOXML(m_oStyles);

		oTable.WriteNodeEnd(L"w:tbl");

		return oTable.GetData();
	}
private:
	std::vector<CTableRow*> m_arRows;
	std::vector<UINT> m_arMinColspan;

	NSStringUtils::CStringBuilder m_oCaption;

	TTableStyles m_oStyles;
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
	boost::wregex oRegex(L"\\s+");
	wsValue = boost::regex_replace(wsValue, oRegex, L" ");
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

bool CanUseThisPath(const std::wstring& wsPath, bool bIsAllowExternalLocalFiles)
{
	if (bIsAllowExternalLocalFiles)
		return true;

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

	NSCSS::CTree m_oTree; // Дерево body html-файла

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

	struct TState
	{
		bool m_bInP;         // <w:p> открыт?
		bool m_bInR;         // <w:r> открыт?
		bool m_bInT;         // <w:t> открыт?
		bool m_bWasPStyle;   // <w:pStyle> записан?
		bool m_bWasSpace;    // Был пробел?
		bool m_bInHyperlink; // <w:hyperlink> открыт?

		TState()
			: m_bInP(false), m_bInR(false), m_bInT(false), m_bWasPStyle(false), m_bWasSpace(true), m_bInHyperlink(false)
		{}
	} m_oState;

	std::vector<std::wstring>            m_arrImages;  // Картинки
	std::map<std::wstring, std::wstring> m_mFootnotes; // Сноски
	std::map<std::wstring, UINT>         m_mBookmarks; // Закладки
public:

	CHtmlFile2_Private() 
		: m_nFootnoteId(1), m_nHyperlinkId(1), m_nNumberingId(1), m_nId(1)
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
		std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
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
			m_oStylesXml += L"</w:rPr></w:rPrDefault>";

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

		std::wstring sRes = htmlToXhtml(sFileContent, bNeedConvert);

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

		const std::string sContentType = NSStringFinder::FindPropety(xml_string, "content-type", ":", ";");
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

		oTree.m_oNode.m_wsName = sName;
		// Стиль по атрибуту
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sNameA  = m_oLightReader.GetName();
			if(sNameA == L"class")
				oTree.m_oNode.m_wsClass  = m_oLightReader.GetText();
			else if(sNameA == L"id")
				oTree.m_oNode.m_wsId = m_oLightReader.GetText();
			else if(sNameA == L"style")
				oTree.m_oNode.m_wsStyle += m_oLightReader.GetText();
			else
				oTree.m_oNode.m_mAttributes[sNameA] = m_oLightReader.GetText();
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

		readStream(&m_oDocXml, sSelectors, { false, false, true, false, -1, L"", L"" });
	}

	bool readInside (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& sName)
	{
		//TODO:: обработать все варианты return'а

		if(sName == L"#text")
		{
			std::wstring sText = m_oLightReader.GetText();

			if (sText.end() == std::find_if_not(sText.begin(), sText.end(), [](wchar_t wchChar){ return iswspace(wchChar);}))
				return false;

			bool bInT = m_oState.m_bInT;

			if (!oTS.sRStyle.empty() || oTS.bPre)
			{
				CloseT(oXml);
				CloseR(oXml);
			}

			if (oTS.bAddSpaces && m_oState.m_bInP && !m_oState.m_bInR && !iswspace(sText.front()) && !m_oState.m_bWasSpace)
			{
				oXml->WriteString(L"<w:r><w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/></w:rPr><w:t xml:space=\"preserve\"> </w:t></w:r>");
				m_oState.m_bWasSpace = true;
			}

			std::wstring sPStyle = wrP(oXml, sSelectors, oTS);
			std::wstring sRStyle;

			if (OpenR(oXml))
			{
				sRStyle = wrRPr(oXml, sSelectors, oTS);
				OpenT(oXml);
			}

			if(oTS.bBdo)
				std::reverse(sText.begin(), sText.end());

			if(oTS.bPre)
			{
				size_t nAfter = sText.find_first_of(L"\n\r");
				while(nAfter != std::wstring::npos)
				{
					oXml->WriteEncodeXmlString(sText.c_str(), nAfter);
					oXml->WriteString(L"</w:t></w:r></w:p><w:p>");
					if(!sPStyle.empty() || !oTS.sPStyle.empty())
					{
						oXml->WriteNodeBegin(L"w:pPr");

						if (!sPStyle.empty())
							oXml->WriteString(L"<w:pStyle w:val=\"" + sPStyle + L"\"/>");

						oXml->WriteString(oTS.sPStyle);

						oXml->WriteNodeEnd(L"w:pPr");
					}
					oXml->WriteNodeBegin(L"w:r");
					if (!sRStyle.empty() || !oTS.sRStyle.empty())
					{
						oXml->WriteNodeBegin(L"w:rPr");

						if (!sRStyle.empty())
							oXml->WriteString(L"<w:rStyle w:val=\"" + sRStyle + L"\"/>");

						oXml->WriteString(oTS.sRStyle);

						oXml->WriteNodeEnd(L"w:rPr");
					}
					oXml->WriteString(L"<w:t xml:space=\"preserve\">");
					sText.erase(0, nAfter + 1);
					nAfter = sText.find_first_of(L"\n\r");
				}

				if (sText.empty())
					return true;
			}
			else
				ReplaceSpaces(sText);

			if (std::iswspace(sText.front()) && m_oState.m_bWasSpace)
				sText.erase(0, 1);

			if (oTS.bMergeText && !m_oState.m_bWasSpace && bInT)
				oXml->WriteEncodeXmlString(L" ");

			if (!sText.empty())
				m_oState.m_bWasSpace = std::iswspace(sText.back());

			oXml->WriteEncodeXmlString(sText);

			if (!oTS.bMergeText)
			{
				CloseT(oXml);
				CloseR(oXml);
			}

			return true;
		}

		std::wstring sNote = GetSubClass(oXml, sSelectors);
		bool bResult = true;
		// Ссылка
		// Область ссылки
		if(sName == L"a" || sName == L"area")
			readA(oXml, sSelectors, oTS, sNote);
		// Полужирный текст
		// Акцентированный текст
		else if(sName == L"b" || sName == L"strong")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:b/><w:bCs/>");
			bResult = readStream(oXml, sSelectors, oTSR);
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
			bResult = readStream(oXml, sSelectors, oTSBdo);
		}
		// Отмена направления текста
		else if(sName == L"bdi")
		{
			CTextSettings oTSBdo(oTS);
			oTSBdo.bBdo = false;
			bResult = readStream(oXml, sSelectors, oTSBdo);
		}
		// Увеличивает размер шрифта
		else if(sName == L"big")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:sz w:val=\"26\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Перенос строки
		else if(sName == L"br")
		{
			if (m_oState.m_bInP)
			{
				oXml->WriteString(L"<w:r>");
				NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
				if(oStyle.m_oText.GetAlign() == L"both")
					oXml->WriteString(L"<w:tab/>");
				oXml->WriteString(L"<w:br/></w:r>");
			}
			else
				WriteEmptyParagraph(oXml, false, m_oState.m_bInP);

			m_oState.m_bWasSpace = true;
		}
		else if(sName == L"center")
		{
			CTextSettings oTSP(oTS);
			oTSP.AddPStyle(L"<w:jc w:val=\"center\"/>");
			bResult = readStream(oXml, sSelectors, oTSP);
		}
		// Цитата, обычно выделяется курсивом
		// Новый термин, обычно выделяется курсивом
		// Акцентированный текст
		// Курсивный текст
		// Переменная, обычно выделяется курсивом
		else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i" || sName == L"var")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:i/><w:iCs/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Код
		// Моноширинный шрифт, например, Consolas
		// Результат скрипта
		else if(sName == L"code" || sName == L"kbd" || sName == L"samp" || sName == L"tt")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Зачеркнутый текст
		else if(sName == L"del" || sName == L"s")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:strike/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		else if(sName == L"font")
		{
			while(m_oLightReader.MoveToNextAttribute())
			{
				std::wstring sAName = m_oLightReader.GetName();
				if(sAName == L"color")
					sSelectors.back().m_wsStyle += L"; color: " + m_oLightReader.GetText();
				else if(sAName == L"face")
					sSelectors.back().m_wsStyle += L"; font-family: " + m_oLightReader.GetText();
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

					switch (nSize)
					{
						case 1:  nSize = 10; break;
						case 2:  nSize = 12; break;
						case 3: 
						default: nSize = 14; break;
						case 4:  nSize = 18; break;
						case 5:  nSize = 24; break;
						case 6:  nSize = 32; break;
						case 7:  nSize = 48; break;
					}
					
					sSelectors.back().m_wsStyle += L"; font-size: " + std::to_wstring(nSize) + L"px";
				}
			}
			m_oLightReader.MoveToElement();
			bResult = readStream(oXml, sSelectors, oTS);
		}
		// Картинки
		else if(sName == L"img")
			readImage(oXml, sSelectors, oTS);
		// Подчеркнутый
		else if(sName == L"ins" || sName == L"u")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:u w:val=\"single\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Выделенный текст, обычно выделяется желтым
		else if(sName == L"mark")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:highlight w:val=\"yellow\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Цитата, выделенная кавычками, обычно выделяется курсивом
		else if(sName == L"q")
		{
			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			std::wstring sRStyle = wrRPr(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t></w:r>");

			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:i/><w:iCs/>");
			readStream(oXml, sSelectors, oTSR);

			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			if (!sRStyle.empty())
			{
				oXml->WriteString(L"<w:rPr><w:rStyle w:val=\"");
				oXml->WriteString(sRStyle);
				oXml->WriteString(L"\"/>");
				oXml->WriteString(oTS.sRStyle);
				oXml->WriteString(L"</w:rPr>");
			}
			oXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t></w:r>");
		}
		// Текст верхнего регистра
		else if(sName == L"rt" || sName == L"sup")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:vertAlign w:val=\"superscript\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Уменьшает размер шрифта
		else if(sName == L"small")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:sz w:val=\"18\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Текст нижнего регистра
		else if(sName == L"sub")
		{
			CTextSettings oTSR(oTS);
			oTSR.AddRStyle(L"<w:vertAlign w:val=\"subscript\"/>");
			bResult = readStream(oXml, sSelectors, oTSR);
		}
		// Векторная картинка
		else if(sName == L"svg" || (sName.length() > 3 && sName.compare(sName.length() - 3, 3, L"svg") == 0))
		{
			wrP(oXml, sSelectors, oTS);
			if (readSVG(m_oLightReader.GetOuterXml()))
				ImageRels(oXml, -1, L"", L"png");
		}
		else if(sName == L"input")
			readInput(oXml, sSelectors, oTS);
		// Игнорируются тэги выполняющие скрипт
		else if(sName == L"template" || sName == L"canvas" || sName == L"video" || sName == L"math" || sName == L"rp"    ||
				sName == L"command"  || sName == L"iframe" || sName == L"embed" || sName == L"wbr"  || sName == L"audio" ||
				sName == L"bgsound"  || sName == L"applet" || sName == L"blink" || sName == L"keygen"|| sName == L"script" ||
				sName == L"comment"  || sName == L"title"  || sName == L"style")
		{
			WriteEmptyParagraph(oXml, false, m_oState.m_bInP);
			sSelectors.pop_back();
			return true;
		}
		else if (sName == L"span")
		{
			if (sSelectors.back().m_wsClass == L"MsoFootnoteReference")
			{
				sSelectors.pop_back();
				return false;
			}
			bResult = readStream(oXml, sSelectors, oTS);
		}
		else if (sName == L"nobr")
		{
			CTextSettings oTSPre(oTS);
			oTSPre.bPre = true;
			bResult = readStream(oXml, sSelectors, oTSPre);
		}
		// Без нового абзаца
		else if(sName == L"basefont" || sName == L"button" || sName == L"label" || sName == L"data" || sName == L"object" ||
				sName == L"noscript" || sName == L"output" || sName == L"abbr"  || sName == L"time" || sName == L"ruby"   ||
				sName == L"progress" || sName == L"hgroup" || sName == L"meter" || sName == L"acronym")
			bResult = readStream(oXml, sSelectors, oTS);
		// С нового абзаца
		else
		{
			NSStringUtils::CStringBuilder oXmlData;
			TState oCurentState{m_oState};

			CloseP(&oXmlData, sSelectors);

			// Адрес
			if(sName == L"address")
			{
				CTextSettings oTSR(oTS);
				oTSR.AddRStyle(L"<w:i/><w:iCs/>");
				bResult = readStream(&oXmlData, sSelectors, oTSR);
			}
			// Определение термина, отступ от левого края
			else if(sName == L"dd")
			{
				CTextSettings oTSP(oTS);
				oTSP.sPStyle += L"<w:ind w:left=\"567\"/>";
				bResult = readStream(&oXmlData, sSelectors, oTSP);
			}
			// aside возможно использовать для сносок в epub
			else if (sName == L"aside" || sName == L"div")
			{
				m_oStylesCalculator.CalculatePageStyle(m_oPageData, sSelectors);

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
					readStream(&m_oNoteXml, sSelectors, oTS);
					m_oNoteXml.WriteString(L"</w:footnote>");
				}
				else
					bResult = readStream(&oXmlData, sSelectors, oTS);
			}
			// С нового абзаца
			else if(sName == L"article" || sName == L"header" || sName == L"blockquote" || sName == L"main" || sName == L"dir" ||
					sName == L"summary" || sName == L"footer" || sName == L"nav" || sName == L"figcaption" || sName == L"form" ||
					sName == L"details" || sName == L"option" || sName == L"dt"  || sName == L"p"    ||
					sName == L"section" || sName == L"figure" || sName == L"dl"  || sName == L"legend"     || sName == L"map"  ||
					sName == L"h1" || sName == L"h2" || sName == L"h3" || sName == L"h4" || sName == L"h5" || sName == L"h6")
				bResult = readStream(&oXmlData, sSelectors, oTS);
			// Горизонтальная линия
			else if(sName == L"hr")
			{
				bool bPrint = true;
				for (const NSCSS::CNode& item : sSelectors)
				{
					if (item.m_wsName == L"div" && item.m_wsStyle == L"mso-element:footnote-list")
					{
						bPrint = false;
						break;
					}
				}
				if (bPrint)
				{
					const bool bOpenedP = OpenP(&oXmlData);
					OpenR(&oXmlData);
					WriteLine(&oXmlData, 1.5, L"a0a0a0");
					CloseR(&oXmlData);
					if (bOpenedP)
						CloseP(&oXmlData, sSelectors);
				}
//					oXml->WriteString(L"<w:p><w:pPr><w:pBdr><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr></w:p>");
			}
			// Меню
			// Маркированный список
			else if(sName == L"menu" || sName == L"ul" || sName == L"select" || sName == L"datalist")
				readLi(&oXmlData, sSelectors, oTS, true);
			// Нумерованный список
			else if(sName == L"ol")
				readLi(&oXmlData, sSelectors, oTS, false);
			// Предварительно форматированный текст
			else if(sName == L"pre" || sName == L"xmp")
			{
				CTextSettings oTSPre(oTS);
				sSelectors.back().m_wsStyle += L"; font-family:Consolas";
				oTSPre.bPre = true;
				bResult = readStream(&oXmlData, sSelectors, oTSPre);
			}
			// Таблицы
			else if(sName == L"table")
				ParseTable(&oXmlData, sSelectors, oTS);
			// Текст с границами
			else if(sName == L"textarea" || sName == L"fieldset")
			{
				CTextSettings oTSP(oTS);
				oTSP.AddPStyle(L"<w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr>");
				bResult = readStream(&oXmlData, sSelectors, oTSP);
			}
			else if (sName == L"xml")
			{
				sSelectors.pop_back();
				return false;
			}
			// Неизвестный тэг. Выделять ли его абзацем?
			else
				bResult = readStream(&oXmlData, sSelectors, oTS);

			readNote(&oXmlData, sSelectors, sNote);
			sNote = L"";

			CloseP(&oXmlData, sSelectors);

			if (bResult)
				oXml->WriteString(oXmlData.GetData());
			else
				m_oState = oCurentState;
		}
		readNote(oXml, sSelectors, sNote);
		sSelectors.pop_back();
		return bResult;
	}

	bool readStream (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, bool bInsertEmptyP = false)
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

	void CalculateCellStyles(CTableCell* pCell, const std::vector<NSCSS::CNode>& arSelectors)
	{
		if (NULL == pCell)
			return;

		std::vector<NSCSS::CNode> arNewSelectors{(std::vector<NSCSS::CNode>::const_iterator)std::find_if(arSelectors.begin(), arSelectors.end(), [](const NSCSS::CNode& oNode){ return L"table" == oNode.m_wsName; }), arSelectors.cend()};

		NSCSS::CCompiledStyle oStyle;
		m_oStylesCalculator.GetCompiledStyle(oStyle, arNewSelectors);

		pCell->SetVAlign(oStyle.m_oDisplay.GetVAlign().ToWString());
		pCell->SetHAlign(oStyle.m_oDisplay.GetHAlign().ToWString());
		pCell->SetBackground(oStyle.m_oBackground.GetColor());
		pCell->SetHeight(oStyle.m_oDisplay.GetHeight());
		pCell->SetWidth(oStyle.m_oDisplay.GetWidth());
		pCell->SetPadding(oStyle.m_oPadding);
		pCell->SetBorder(oStyle.m_oBorder);

		if (pCell->GetStyles()->m_wsHAlign.empty())
			pCell->SetHAlign(oStyle.m_oText.GetAlign().ToWString());
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
	
	struct TRowspanElement
	{
		UINT  m_unRowSpan;
		UINT  m_unColumnIndex;
		const CTableCell* m_pCell;

		TRowspanElement(UINT unRowSpan, UINT unColumnIndex, const CTableCell* pCell)
			: m_unRowSpan(unRowSpan), m_unColumnIndex(unColumnIndex), m_pCell(pCell)
		{}
	};

	void ParseTableRows(CTable& oTable, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, ERowParseMode eMode)
	{
		std::vector<TRowspanElement> arRowspanElements;

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

				pCell->SetMode(eMode);

				GetSubClass(pCell->GetData(), sSelectors);
				CalculateCellStyles(pCell, sSelectors);

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
						oTSR.sPStyle += L"<w:jc w:val=\"center\"/>";

					oTSR.AddRStyle(L"<w:b/><w:bCs/>");
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

					while (m_oLightReader.ReadNextSiblingNode(nTrDepth) && (L"td" == m_oLightReader.GetName() || L"th" == m_oLightReader.GetName()))
					{
						GetSubClass(pCell->GetData(), sSelectors);
						readStream(pCell->GetData(), sSelectors, oTrTS, true);
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

			oTable.AddRow(pRow);
		}
	}

	void ParseTable(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		CTable oTable;

		//Table styles
		if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"border"))
		{
			const int nWidth = NSStringFinder::ToInt(sSelectors.back().m_mAttributes[L"border"]);

			if (0 < nWidth)
			{
				sSelectors.back().m_mAttributes[L"border"] = L"outset " + std::to_wstring(nWidth) + L"px auto";
				oTable.HaveBorderAttribute();
			}
			else
				sSelectors.back().m_mAttributes[L"border"] = L"none";
		}

		NSCSS::CCompiledStyle oStyle;
		m_oStylesCalculator.GetCompiledStyle(oStyle, sSelectors);

		if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"cellpadding"))
			oStyle.m_oPadding.SetValues(sSelectors.back().m_mAttributes[L"cellpadding"] + L"px", 0, true);

		if (oStyle.m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Collapse)
			oTable.SetCellSpacing(0);
		else if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"cellspacing"))
			oTable.SetCellSpacing(NSStringFinder::ToInt(sSelectors.back().m_mAttributes[L"cellspacing"]));
		else if (oStyle.m_oBorder.GetCollapse() == NSCSS::NSProperties::BorderCollapse::Separate)
			oTable.SetCellSpacing(15);

		oTable.SetWidth(oStyle.m_oDisplay.GetWidth());
		oTable.SetBorder(oStyle.m_oBorder);
		oTable.SetPadding(oStyle.m_oPadding);
		oTable.SetMargin(oStyle.m_oMargin);
		oTable.SetAlign(oStyle.m_oDisplay.GetHAlign().ToWString());
		//------

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath)) 
		{
			const std::wstring sName = m_oLightReader.GetName();
			GetSubClass(oXml, sSelectors);

			if(sName == L"caption")
				ParseTableCaption(oTable, sSelectors, oTS);
			if(sName == L"thead")
				ParseTableRows(oTable, sSelectors, oTS,  ERowParseMode::ParseModeHeader);
			if(sName == L"tbody")
				ParseTableRows(oTable, sSelectors, oTS, ERowParseMode::ParseModeBody);
			else if(sName == L"tfoot")
				ParseTableRows(oTable, sSelectors, oTS,  ERowParseMode::ParseModeFoother);

			sSelectors.pop_back();
		}

		oTable.Shorten();
		oTable.CompleteTable();
		oXml->WriteString(oTable.ConvertToOOXML());
		WriteEmptyParagraph(oXml, true);
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
			wrRPr(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">");
			oXml->WriteEncodeXmlString(sValue + L' ');
			oXml->WriteString(L"</w:t></w:r>");
		}

		readStream(oXml, sSelectors, oTS, ElementInTable(sSelectors));
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
			if (sName == L"optgroup")
			{
				GetSubClass(oXml, sSelectors);
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() != L"label")
						continue;
					CloseP(oXml, sSelectors);
					wrP(oXml, sSelectors, oTS);
					oXml->WriteString(L"<w:r>");
					wrRPr(oXml, sSelectors, oTS);
					oXml->WriteString(L"<w:t xml:space=\"preserve\">");
					oXml->WriteEncodeXmlString(m_oLightReader.GetText());
					oXml->WriteString(L"</w:t></w:r>");
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

			GetSubClass(oXml, sSelectors);
			while(m_oLightReader.MoveToNextAttribute())
				if(m_oLightReader.GetName() == L"value")
					sStart = m_oLightReader.GetText();
			m_oLightReader.MoveToElement();

			CloseP(oXml, sSelectors);

			CTextSettings oTSLiP(oTS);
			oTSLiP.nLi++;
			oTSLiP.sPStyle += L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(oTSLiP.nLi) + L"\"/><w:numId w:val=\"" +
			                  (bType ? L"1" : std::to_wstring(m_nNumberingId + 1)) + L"\"/></w:numPr>";
			readStream(oXml, sSelectors, oTSLiP);

			CloseP(oXml, sSelectors);

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
			oXml->WriteString(L"<w:r>");
			wrRPr(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">");
			oXml->WriteEncodeXmlString(!sAlt.empty() ? sAlt : L" ");
			oXml->WriteString(L"</w:t></w:r>");
		}

		if (m_oState.m_bInP)
		{
			if (m_oState.m_bInHyperlink)
			{
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
					oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
					oXml->WriteString(sFootnoteID);
					oXml->WriteString(L"\"/></w:r>");
					m_mFootnotes.insert(std::make_pair(sFootnote, sFootnoteID));
				}
				else
					oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/></w:r>");
			}

			CloseP(oXml, sSelectors);
		}

		sNote.clear();
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

	void ImageAlternative(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& wsAlt, const std::wstring& wsSrc)
	{
		if (wsAlt.empty())
		{
			//TODO:: реализовать отображение того, что картинку не удалось получить
			if (wsSrc.empty())
				WriteEmptyParagraph(oXml, false, m_oState.m_bInP);
			else
			{
				m_oDocXmlRels.WriteString(L"<Relationship Id=\"rId");
				m_oDocXmlRels.WriteString(std::to_wstring(m_nId));
				m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"");
				m_oDocXmlRels.WriteEncodeXmlString(wsSrc);
				m_oDocXmlRels.WriteString(L"\" TargetMode=\"External\"/>");

				const bool bOpenedP{OpenP(oXml)};

				WriteEmptyImage(oXml, 304800, 304800);

				if (bOpenedP)
					CloseP(oXml, sSelectors);
			}
			return;
		}

		wrP(oXml, sSelectors, oTS);
		oXml->WriteString(L"<w:r>");
		wrRPr(oXml, sSelectors, oTS);
		oXml->WriteString(L"<w:t xml:space=\"preserve\">");
		oXml->WriteEncodeXmlString(wsAlt);
		oXml->WriteString(L"</w:t></w:r>");
	}

	void readImage  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		std::wstring wsAlt, sSrcM;
		bool bRes = false;
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring wsName = m_oLightReader.GetName();
			if (wsName == L"alt")
				wsAlt = m_oLightReader.GetText();
			else if (wsName == L"src")
				sSrcM = m_oLightReader.GetText();
		}
		m_oLightReader.MoveToElement();

		if (sSrcM.empty())
		{
			ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM);
			return;
		}

		const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

		bool bIsBase64 = false;
		if (sSrcM.length() > 4 && sSrcM.substr(0, 4) == L"data" && sSrcM.find(L"/", 4) != std::wstring::npos)
			bIsBase64 = true;

		if (!bIsBase64 && (sSrcM.length() <= 7 || L"http" != sSrcM.substr(0, 4)))
		{
			sSrcM = NSSystemPath::ShortenPath(sSrcM);

			if (!CanUseThisPath(sSrcM, bIsAllowExternalLocalFiles))
				return;
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
				ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM);
				return;
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
			ImageAlternative(oXml, sSelectors, oTS, wsAlt, sSrcM);
		else
		{
			wrP(oXml, sSelectors, oTS);
			ImageRels(oXml, nImageId, sImageSrc, sExtention);
		}
	}

	std::wstring wrP(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		OpenP(oXml);

		if (m_oState.m_bWasPStyle)
			return L"";

		std::vector<std::pair<size_t, NSCSS::CNode>> temporary;
		size_t i = 0;
		while(i != sSelectors.size())
		{
			if (sSelectors[i].Empty() && rStyle.find(L' ' + sSelectors[i].m_wsName + L' ') != std::wstring::npos)
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

		if (sPStyle.empty() && oTS.sPStyle.empty())
			return L"";

		m_oXmlStyle.WriteLitePStyle(oStyleSetting);
		std::wstring sPSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		for(int i = temporary.size() - 1; i >= 0; i--)
			sSelectors.insert(sSelectors.begin() + temporary[i].first, temporary[i].second);

		// Если в таблице, то игнориуются Paragraph Borders
		bool bInTable = false;
		for (const NSCSS::CNode& item : sSelectors)
			if (item.m_wsName == L"table")
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

		oXml->WriteNodeBegin(L"w:pPr");

		if (!sPStyle.empty())
		{
			oXml->WriteString(L"<w:pStyle w:val=\"");
			oXml->WriteString(sPStyle);
			oXml->WriteString(L"\"/>");
		}

		oXml->WriteString(oTS.sPStyle + L' ' + sPSettings);
		oXml->WriteNodeEnd(L"w:pPr");
		m_oState.m_bWasPStyle = true;

		return sPStyle;
	}

	std::wstring wrRPr(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if (!m_oState.m_bInP)
			return L"";

		NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
		NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);

		NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

		std::wstring sRStyle = GetStyle(oStyle, false);

		m_oXmlStyle.WriteLiteRStyle(oStyleSetting);
		const std::wstring sRSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		if (!sRStyle.empty() || !oTS.sRStyle.empty())
		{
			oXml->WriteString(L"<w:rPr>");
			if (!sRStyle.empty())
			{
				oXml->WriteString(L"<w:rStyle w:val=\"");
				oXml->WriteString(sRStyle);
				oXml->WriteString(L"\"/>");
			}

			oXml->WriteString(oTS.sRStyle + L' ' + sRSettings);
			oXml->WriteString(L"</w:rPr>");
		}
		return sRStyle;
	}

	void WriteImage(NSStringUtils::CStringBuilder* pXml, int nWidth, int nHeight, const std::wstring& wsId)
	{
		// Пишем в document.xml
		pXml->WriteString(L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"");
		pXml->WriteString(std::to_wstring(nWidth));
		pXml->WriteString(L"\" cy=\"");
		pXml->WriteString(std::to_wstring(nHeight));
		pXml->WriteString(L"\"/><wp:docPr id=\"");
		pXml->WriteString(wsId);
		pXml->WriteString(L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"");
		pXml->WriteString(wsId);
		pXml->WriteString(L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic");
		pXml->WriteString(wsId);
		pXml->WriteString(L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"");
		pXml->WriteString(std::to_wstring(nWidth));
		pXml->WriteString(L"\" cy=\"");
		pXml->WriteString(std::to_wstring(nHeight));
		pXml->WriteString(L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>");
	}

	void WriteEmptyImage(NSStringUtils::CStringBuilder* pXml, int nWidth, int nHeight)
	{
		pXml->WriteString(L"<w:r><w:rPr><w:noProof/></w:rPr><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/><wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>");
		pXml->WriteString(L"<wp:docPr id=\"" + std::to_wstring(m_nId - 7) + L"\" name=\"\"/>");
		pXml->WriteString(L"<wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/></wp:cNvGraphicFramePr>");
		pXml->WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
		pXml->WriteString(L"<pic:nvPicPr><pic:cNvPr id=\"0\" name=\"\"/><pic:cNvPicPr><a:picLocks noChangeAspect=\"1\" noChangeArrowheads=\"1\"/></pic:cNvPicPr></pic:nvPicPr>");
		pXml->WriteString(L"<pic:blipFill><a:blip r:link=\"rId" + std::to_wstring(m_nId++) + L"\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:srcRect/><a:stretch><a:fillRect/></a:stretch></pic:blipFill>");
		pXml->WriteString(L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"" + std::to_wstring(nWidth) + L"\" cy=\"" + std::to_wstring(nHeight) + L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/><a:ln><a:noFill/></a:ln></pic:spPr>");
		pXml->WriteString(L"</pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>");
	}

	void ImageRels  (NSStringUtils::CStringBuilder* oXml, int nImageId, const std::wstring& sImageSrc, const std::wstring& sExtention)
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

		// Получаем размеры картинки
		int nHy = oBgraFrame.get_Height();
		int nWx = oBgraFrame.get_Width();
		if (nWx > nHy)
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
			if (nW > 7000000)
			{
				nW = 7000000;
				nHy = (int)((double)nHy * (double)nW / (double)nWx);
			}
			else
				nHy = nH;
			nWx = nW;
		}

		WriteImage(oXml, nWx, nHy, sImageId);
	}

	void readNote   (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const std::wstring& sNote)
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
	}

	bool readSVG    (const std::wstring& wsSvg)
	{
		if (wsSvg.empty())
			return false;

		CSvgFile oSvgReader;

		NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
		NSFonts::IFontManager* pFontManager = pFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();

		pFontCache->SetStreams(pFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		oSvgReader.SetFontManager(pFontManager);

		if (!oSvgReader.ReadFromWString(wsSvg))
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

		oSvgReader.SetWorkingDirectory(m_sSrc);
		oSvgReader.Draw(pGrRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(m_sDst + L"/word/media/i" + std::to_wstring(m_arrImages.size()) + L".png", 4);
		oFrame.put_Data(NULL);

		RELEASEINTERFACE(pFontManager);
		RELEASEINTERFACE(pGrRenderer);

		if (pBgraData)
			free(pBgraData);

		pFonts->Release();

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

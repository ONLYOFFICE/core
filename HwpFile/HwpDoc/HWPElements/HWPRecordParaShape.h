#ifndef HWPRECORDPARASHAPE_H
#define HWPRECORDPARASHAPE_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"
#include "../Common/XMLNode.h"

namespace HWP
{
enum class EHeadingType
{
	NONE,
	OUTLINE,
	NUMBER,
	BULLET
};

enum class EHorizontalAlign
{
	JUSTIFY,
	LEFT,
	RIGHT,
	CENTER,
	DISTRIBUTE,
	DISTRIBUTE_SPACE
};

enum class EVerticalAlign
{
	BASELINE,
	TOP,
	CENTER,
	BOTTOM
};

class CHWPRecordParaShape : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	EHorizontalAlign m_eAlign;
	HWP_BYTE m_chBreakLatinWord;
	HWP_BYTE m_chBreakNonLatinWord;
	bool m_bSnapToGrid;
	HWP_BYTE m_chCondense;
	bool m_bWidowOrphan;
	bool m_bKeepWithNext;
	bool m_bPageBreakBefore;
	EVerticalAlign m_eVertAlign;
	bool m_bFontLineHeight;
	EHeadingType m_eHeadingType;

	HWP_BYTE m_chHeadingLevel;
	bool m_bConnect;
	bool m_bIgnoreMargin;
	bool m_bParaTailShape;

	int m_nIndent;
	int m_nMarginLeft;
	int m_nMarginRight;
	int m_nMarginPrev;
	int m_nMarginNext;
	int m_nLineSpacing;

	short m_shTabDef;
	short m_shHeadingIdRef;
	short m_shBorderFill;
	short m_shOffsetLeft;
	short m_shOffsetRight;
	short m_shOffsetTop;
	short m_shOffsetBottom;

	HWP_BYTE m_chLineWrap;
	bool m_bAutoSpaceEAsianEng;
	bool m_bAutoSpaceEAsianNum;

	int m_nLineSpacingType;
	void RecursiveParaShape(CXMLNode& oNode);
public:
	CHWPRecordParaShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordParaShape(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion);

	EHorizontalAlign GetHorizantalAlign() const;
	EVerticalAlign GetVerticalAlign() const;
	EHeadingType GetHeadingType() const;
	HWP_BYTE GetHeadingLevel() const;
	short GetHeadingIdRef() const;
	bool GetPageBreakBefore() const;
	int GetLineSpacingType() const;
	int GetLineSpacing() const;
	int GetMarginPrev() const;
	int GetMarginNext() const;

	int GetIndent() const;
	int GetLeftIndent() const;
	int GetRightIndent() const;

	bool KeepWithNext() const;
};
}

#endif // HWPRECORDPARASHAPE_H

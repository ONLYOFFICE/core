#ifndef NOTESHAPE_H
#define NOTESHAPE_H

#include "../HWPElements/HwpRecordTypes.h"
#include "../HWPStream.h"
#include "../Common/XMLNode.h"

namespace HWP
{
enum class ENoteNumbering
{
	CONTINUOUS,
	ON_SECTION,
	ON_PAGE,
	UNKNOWN
};

//TODO:: проверить данный enum
// в олигинале и EachColumn и EndOfDocument имеют одинаковые значения
enum class ENotePlacement
{
	EachColumn,
	MergedColumn,
	RightMostColumn,

	EndOfDocument,
	EndOfSection
};

class CNoteShape
{
	ENumberShape2 m_eNumberShape;
	HWP_BYTE m_chPlacement;
	ENoteNumbering m_eNumbering;
	bool m_bSuperscript;
	bool m_bBeneathText;
	char16_t m_chUserChar;
	char16_t m_chPrefixChar;
	char16_t m_chSuffixChar;
	short m_shNewNumber;
	int m_nNoteLineLength;
	short m_shSpacingAboveLine;
	short m_shSpacingBelowLine;
	short m_shSpacingBetweenNotes;
	ELineStyle1 m_eNoteLineType;
	HWP_BYTE m_chNoteLineWidth;
	int m_nNoteLineColor;
public:
	CNoteShape();
	CNoteShape(CXMLNode& oNode, int nVersion);

	static CNoteShape* Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // NOTESHAPE_H

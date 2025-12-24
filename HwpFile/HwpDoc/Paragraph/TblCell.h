#ifndef TBLCELL_H
#define TBLCELL_H

#include "CellParagraph.h"
#include "CtrlCommon.h"

namespace HWP
{
class CTblCell
{
	int m_nSize;

	short m_shColAddr;
	short m_shRowAddr;
	short m_shColSpan;
	short m_shRowSpan;
	int m_nWidth;
	int m_nHeight;
	int m_arMargin[4];
	short m_shBorderFill;
	VECTOR<CCellParagraph*> m_arParas;
	EVertAlign m_eVertAlign;

	HWP_STRING m_sMergedColName;

	void ReadFromHWPX(CXMLReader& oReader);
	void ReadFromHWPML(CXMLReader& oReader);
	void ReadCellMargin(CXMLReader& oReader, EHanType eType);
	void ReadCell(CXMLReader& oReader, EHanType eType);
public:
	CTblCell(int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CTblCell(CXMLReader& oReader, EHanType eType);

	void SetVertAlign(EVertAlign eVertAlign);

	void AddParagraph(CCellParagraph* pParagraph);

	int GetSize();

	short GetColAddr() const;
	short GetRowAddr() const;
	short GetColSpan() const;
	short GetRowSpan() const;
	VECTOR<CCellParagraph*> GetParagraphs() const;
	int GetWidth() const;
	int GetHeight() const;
	EVertAlign GetVertAlign() const;
	short GetBorderFillID() const;
};
}

#endif // TBLCELL_H

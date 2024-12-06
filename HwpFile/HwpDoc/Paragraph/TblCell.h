#ifndef TBLCELL_H
#define TBLCELL_H

#include "CellParagraph.h"
#include "CtrlCommon.h"
#include <list>

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
	std::list<CCellParagraph*> m_arParas;
	EVertAlign m_eVertAlign;

	std::string m_sMergedColName;
public:
	CTblCell(int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize();
};
}

#endif // TBLCELL_H

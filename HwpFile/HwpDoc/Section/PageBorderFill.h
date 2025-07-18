#ifndef PAGEBORDERFILL_H
#define PAGEBORDERFILL_H

#include "../HWPStream.h"
#include "../Common/XMLReader.h"

namespace HWP
{
class CPageBorderFill
{
	bool m_bTextBorder;
	bool m_bHeaderInside;
	bool m_bFooterInside;
	HWP_BYTE m_chFillArea;
	short m_shOffsetLeft;
	short m_shOffsetRight;
	short m_shOffsetTop;
	short m_shOffsetBottom;
	short m_shBorderFill;
public:
	CPageBorderFill();
	CPageBorderFill(CXMLReader& oReader, int nVersion);

	static CPageBorderFill* Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // PAGEBORDERFILL_H

#ifndef PAGE_H
#define PAGE_H

#include "../HWPStream.h"

namespace HWP
{
class CPage
{
	bool m_bLandscape;
	int m_nWidth;
	int m_nHeight;
	BYTE m_chGutterType;
	int m_nMarginLeft;
	int m_nMarginRight;
	int m_nMarginTop;
	int m_nMarginBottom;
	int m_nMarginHeader;
	int m_nMarginFooter;
	int m_nMarginGutter;
public:
	CPage();

	static CPage* Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // PAGE_H

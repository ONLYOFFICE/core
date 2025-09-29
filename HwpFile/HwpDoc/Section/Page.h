#ifndef PAGE_H
#define PAGE_H

#include "../HanType.h"
#include "../HWPStream.h"
#include "../Common/XMLReader.h"

namespace HWP
{
class CPage
{
	bool m_bLandscape;
	int m_nWidth;
	int m_nHeight;
	HWP_BYTE m_chGutterType;
	int m_nMarginLeft;
	int m_nMarginRight;
	int m_nMarginTop;
	int m_nMarginBottom;
	int m_nMarginHeader;
	int m_nMarginFooter;
	int m_nMarginGutter;
public:
	CPage();
	CPage(CXMLReader& oReader, EHanType eType);

	int GetWidth() const;
	int GetHeight() const;
	int GetMarginLeft() const;
	int GetMarginRight() const;
	int GetMarginTop() const;
	int GetMarginBottom() const;
	int GetMarginHeader() const;
	int GetMarginFooter() const;
	int GetMarginGutter() const;

	static CPage* Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // PAGE_H

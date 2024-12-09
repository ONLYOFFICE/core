#include "Page.h"

namespace HWP
{
HWP::CPage::CPage()
{}

CPage* CPage::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CPage *pPage = new CPage();

	if (nullptr == pPage)
		return nullptr;

	oBuffer.ReadInt(pPage->m_nWidth);
	oBuffer.ReadInt(pPage->m_nHeight);
	oBuffer.ReadInt(pPage->m_nMarginLeft);
	oBuffer.ReadInt(pPage->m_nMarginRight);
	oBuffer.ReadInt(pPage->m_nMarginTop);
	oBuffer.ReadInt(pPage->m_nMarginBottom);
	oBuffer.ReadInt(pPage->m_nMarginHeader);
	oBuffer.ReadInt(pPage->m_nMarginFooter);
	oBuffer.ReadInt(pPage->m_nMarginGutter);

	int nAttr;
	oBuffer.ReadInt(nAttr);

	pPage->m_bLandscape = CHECK_FLAG(nAttr, 0x01);
	pPage->m_chGutterType = (BYTE)((nAttr >> 1) & 0x03);

	return pPage;
}
}

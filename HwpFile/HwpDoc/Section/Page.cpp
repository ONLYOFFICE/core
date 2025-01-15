#include "Page.h"

namespace HWP
{
HWP::CPage::CPage()
{}

int CPage::GetWidth() const
{
	return m_nWidth;
}

int CPage::GetHeight() const
{
	return m_nHeight;
}

int CPage::GetMarginLeft() const
{
	return m_nMarginLeft;
}

int CPage::GetMarginRight() const
{
	return m_nMarginRight;
}

int CPage::GetMarginTop() const
{
	return m_nMarginTop;
}

int CPage::GetMarginBottom() const
{
	return m_nMarginBottom;
}

int CPage::GetMarginHeader() const
{
	return m_nMarginHeader;
}

int CPage::GetMarginFooter() const
{
	return m_nMarginFooter;
}

int CPage::GetMarginGutter() const
{
	return m_nMarginGutter;
}

CPage* CPage::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CPage *pPage = new CPage();

	if (nullptr == pPage)
	{
		oBuffer.Skip(nSize);
		oBuffer.RemoveLastSavedPos();
		return nullptr;
	}

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
	pPage->m_chGutterType = (HWP_BYTE)((nAttr >> 1) & 0x03);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pPage;
}
}

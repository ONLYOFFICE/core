#include "Page.h"

namespace HWP
{
HWP::CPage::CPage()
{}

CPage::CPage(CXMLNode& oNode)
{
	m_bLandscape = L"NARROWLY" == oNode.GetAttribute(L"landscape");

	m_nWidth = oNode.GetAttributeInt(L"width");
	m_nHeight = oNode.GetAttributeInt(L"height");

	std::wstring wsType = oNode.GetAttribute(L"gutterType");

	if (L"LEFT_ONELY" == wsType)
		m_chGutterType = 0;
	else if (L"LEFT_RIGHT" == wsType)
		m_chGutterType = 1;
	else if (L"TOP_BOTTOM" == wsType)
		m_chGutterType = 2;

	for (CXMLNode& oChild : oNode.GetChilds(L"hp:margin"))
	{
		m_nMarginLeft   = oChild.GetAttributeInt(L"left");
		m_nMarginRight  = oChild.GetAttributeInt(L"right");
		m_nMarginTop    = oChild.GetAttributeInt(L"top");
		m_nMarginBottom = oChild.GetAttributeInt(L"bottom");
		m_nMarginHeader = oChild.GetAttributeInt(L"header");
		m_nMarginFooter = oChild.GetAttributeInt(L"footer");
		m_nMarginGutter = oChild.GetAttributeInt(L"gutter");
	}
}

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

#include "Page.h"
#include "../Common/NodeNames.h"

namespace HWP
{
HWP::CPage::CPage()
{}

CPage::CPage(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Landscape, eType) == sAttributeName)
		{
			if (EHanType::HWPX == eType)
				m_bLandscape = "NARROWLY" == oReader.GetTextA();
			else
				m_bLandscape = oReader.GetBool();
		}
		else if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
			m_nWidth = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Height, eType) == sAttributeName)
			m_nHeight = oReader.GetInt();
		else if (GetAttributeName(EAttribute::GutterType, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::LeftOnly, eType) == sType)
				m_chGutterType = 0;
			else if (GetValueName(EValue::LeftRight, eType) == sType)
				m_chGutterType = 1;
			else if (GetValueName(EValue::TopBottom, eType) == sType)
				m_chGutterType = 2;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::PageMargin, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
				m_nMarginLeft = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
				m_nMarginRight = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
				m_nMarginTop = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
				m_nMarginBottom = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Header, eType) == sAttributeName)
				m_nMarginHeader = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Footer, eType) == sAttributeName)
				m_nMarginFooter = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Gutter, eType) == sAttributeName)
				m_nMarginGutter = oReader.GetInt();

		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
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

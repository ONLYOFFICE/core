#include "TblCell.h"
#include "CtrlCharacter.h"

namespace HWP
{
CTblCell::CTblCell(int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: m_nSize(nSize)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(m_shColAddr);
	oBuffer.ReadShort(m_shRowAddr);
	oBuffer.ReadShort(m_shColSpan);
	oBuffer.ReadShort(m_shRowSpan);
	oBuffer.ReadInt(m_nWidth);
	oBuffer.ReadInt(m_nHeight);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
		m_arMargin[unIndex] = oBuffer.ReadShort();

	oBuffer.ReadShort(m_shBorderFill);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

CTblCell::CTblCell(CXMLNode& oNode, int nVersion)
{
	m_shBorderFill = oNode.GetAttributeInt(L"borderFillIDRef");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:cellAddr" == oChild.GetName())
		{
			m_shColAddr = oChild.GetAttributeInt(L"colAddr");
			m_shRowAddr = oChild.GetAttributeInt(L"rowAddr");
		}
		else if (L"hp:cellSpan" == oChild.GetName())
		{
			m_shColSpan = oChild.GetAttributeInt(L"colSpan");
			m_shRowSpan = oChild.GetAttributeInt(L"rowSpan");
		}
		else if (L"hp:cellSz" == oChild.GetName())
		{
			m_nWidth = oChild.GetAttributeInt(L"width");
			m_nHeight = oChild.GetAttributeInt(L"height");
		}
		else if (L"hp:cellMargin" == oChild.GetName())
		{
			m_arMargin[0] = oChild.GetAttributeInt(L"left");
			m_arMargin[1] = oChild.GetAttributeInt(L"rifht");
			m_arMargin[2] = oChild.GetAttributeInt(L"top");
			m_arMargin[3] = oChild.GetAttributeInt(L"bottom");
		}
		else if (L"hp:subList" == oChild.GetName())
		{
			m_eVertAlign = ::HWP::GetVertAlign(oChild.GetAttribute(L"vertAlign"));

			for (CXMLNode& oGrandChild : oChild.GetChilds(L"hp:p"))
			{
				CCellParagraph *pCellParagraphs = new CCellParagraph(oGrandChild, nVersion);

				if (nullptr == pCellParagraphs)
					continue;

				if (ECtrlObjectType::Character != pCellParagraphs->GetCtrls().back()->GetCtrlType())
					pCellParagraphs->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));

				m_arParas.push_back(pCellParagraphs);
			}
		}
	}
}

void CTblCell::SetVertAlign(EVertAlign eVertAlign)
{
	m_eVertAlign = eVertAlign;
}

void CTblCell::AddParagraph(CCellParagraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

int CTblCell::GetSize()
{
	return m_nSize;
}

short CTblCell::GetColAddr() const
{
	return m_shColAddr;
}

short CTblCell::GetRowAddr() const
{
	return m_shRowAddr;
}

short CTblCell::GetColSpan() const
{
	return m_shColSpan;
}

short CTblCell::GetRowSpan() const
{
	return m_shRowSpan;
}

VECTOR<CCellParagraph*> CTblCell::GetParagraphs() const
{
	return m_arParas;
}

int CTblCell::GetWidth() const
{
	return m_nWidth;
}

int CTblCell::GetHeight() const
{
	return m_nHeight;
}

EVertAlign CTblCell::GetVertAlign() const
{
	return m_eVertAlign;
}

short CTblCell::GetBorderFillID() const
{
	return m_shBorderFill;
}

}

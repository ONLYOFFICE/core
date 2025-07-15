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

CTblCell::CTblCell(CXMLReader& oReader, int nVersion)
{
	m_shBorderFill = oReader.GetAttributeInt("borderFillIDRef");

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:cellAddr" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("colAddr" == sAttributeName)
					m_shColAddr = oReader.GetInt();
				else if ("rowAddr" == sAttributeName)
					m_shRowAddr = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:cellSpan" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("colSpan" == sAttributeName)
					m_shColSpan = oReader.GetInt();
				else if ("rowSpan" == sAttributeName)
					m_shRowSpan = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:cellSz" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("width" == sAttributeName)
					m_nWidth = oReader.GetInt();
				else if ("height" == sAttributeName)
					m_nHeight = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:cellSz" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("left" == sAttributeName)
					m_arMargin[0] = oReader.GetInt();
				else if ("right" == sAttributeName)
					m_arMargin[1] = oReader.GetInt();
				else if ("top" == sAttributeName)
					m_arMargin[2] = oReader.GetInt();
				else if ("bottom" == sAttributeName)
					m_arMargin[3] = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:subList" == sNodeName)
		{
			m_eVertAlign = ::HWP::GetVertAlign(oReader.GetAttribute("vertAlign"));

			WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:p")
			{
				CCellParagraph *pCellParagraphs = new CCellParagraph(oReader, nVersion);

				if (nullptr == pCellParagraphs)
					continue;

				if (ECtrlObjectType::Character != pCellParagraphs->GetCtrls().back()->GetCtrlType())
					pCellParagraphs->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));

				m_arParas.push_back(pCellParagraphs);
			}
			END_WHILE
		}
	}
	END_WHILE
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

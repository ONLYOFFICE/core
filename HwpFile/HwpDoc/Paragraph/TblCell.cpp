#include "TblCell.h"
#include "CtrlCharacter.h"

#include "../Common/NodeNames.h"

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

CTblCell::CTblCell(CXMLReader& oReader, int nVersion, EHanType eType)
{
	switch (eType)
	{
		case EHanType::HWPX: ReadFromHWPX(oReader, nVersion); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
	}
}

void CTblCell::ReadFromHWPX(CXMLReader &oReader, int nVersion)
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
			ReadCellMargin(oReader, EHanType::HWPX);
		else if ("hp:subList" == sNodeName)
		{
			m_eVertAlign = ::HWP::GetVertAlign(oReader.GetAttributeA("vertAlign"), EHanType::HWPX);

			WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:p")
				ReadCell(oReader, nVersion, EHanType::HWPX);
			END_WHILE
		}
	}
	END_WHILE
}

void CTblCell::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Name" == sAttributeName)
			m_sMergedColName = oReader.GetText();
		else if ("ColAddr" == sAttributeName)
			m_shColAddr = oReader.GetInt();
		else if ("RowAddr" == sAttributeName)
			m_shRowAddr = oReader.GetInt();
		else if ("ColSpan" == sAttributeName)
			m_shColSpan = oReader.GetInt();
		else if ("RowSpan" == sAttributeName)
			m_shRowSpan = oReader.GetInt();
		else if ("Width" == sAttributeName)
			m_nWidth = oReader.GetInt();
		else if ("Height" == sAttributeName)
			m_nHeight = oReader.GetInt();
		else if ("BorderFill" == sAttributeName)
			m_shBorderFill = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("CELLMARGIN" == sNodeName)
			ReadCellMargin(oReader, EHanType::HWPML);
		else if ("PARALIST" == sNodeName)
		{
			m_eVertAlign = ::HWP::GetVertAlign(oReader.GetAttributeA("VertAlign"), EHanType::HWPML);

			WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "P")
				ReadCell(oReader, 0, EHanType::HWPML);
			END_WHILE
		}
	}
	END_WHILE
}

void CTblCell::ReadCellMargin(CXMLReader &oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
			m_arMargin[0] = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
			m_arMargin[1] = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
			m_arMargin[2] = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
			m_arMargin[3] = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

void HWP::CTblCell::ReadCell(CXMLReader &oReader, int nVersion, EHanType eType)
{
	CCellParagraph *pCellParagraphs = new CCellParagraph(oReader, nVersion, eType);

	if (nullptr == pCellParagraphs)
		return;

	if (ECtrlObjectType::Character != pCellParagraphs->GetCtrls().back()->GetCtrlType())
		pCellParagraphs->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));

	m_arParas.push_back(pCellParagraphs);
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

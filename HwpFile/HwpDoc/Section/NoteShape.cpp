#include "NoteShape.h"

namespace HWP
{
ENoteNumbering GetNoteNumbering(int nValue)
{
	switch(static_cast<ENoteNumbering>(nValue))
	{
		case ENoteNumbering::CONTINUOUS: return ENoteNumbering::CONTINUOUS;
		case ENoteNumbering::ON_SECTION: return ENoteNumbering::ON_SECTION;
		case ENoteNumbering::ON_PAGE: return ENoteNumbering::ON_PAGE;
		default:
			return ENoteNumbering::UNKNOWN;
	}
}

CNoteShape::CNoteShape()
{}

CNoteShape* CNoteShape::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CNoteShape* pNoteShape = new CNoteShape();

	if (nullptr == pNoteShape)
	{
		oBuffer.Skip(nSize);
		return nullptr;
	}

	oBuffer.SavePosition();

	pNoteShape->m_eNumberShape = GetNumberShape2(oBuffer.ReadByte());

	HWP_BYTE chAttr;
	oBuffer.ReadByte(chAttr);

	pNoteShape->m_chPlacement = (HWP_BYTE)(chAttr & 0x03);
	pNoteShape->m_eNumbering = GetNoteNumbering((chAttr >> 2) & 0x03);
	pNoteShape->m_bSuperscript = CHECK_FLAG(chAttr >> 4, 0x01);
	pNoteShape->m_bBeneathText = CHECK_FLAG(chAttr >> 5, 0x01);
	oBuffer.Skip(2);

	oBuffer.ReadChar(pNoteShape->m_chUserChar);
	oBuffer.ReadChar(pNoteShape->m_chPrefixChar);
	oBuffer.ReadChar(pNoteShape->m_chSuffixChar);
	oBuffer.ReadShort(pNoteShape->m_shNewNumber);
	oBuffer.ReadInt(pNoteShape->m_nNoteLineLength);
	oBuffer.ReadShort(pNoteShape->m_shSpacingAboveLine);
	oBuffer.ReadShort(pNoteShape->m_shSpacingBelowLine);
	oBuffer.ReadShort(pNoteShape->m_shSpacingBetweenNotes);

	pNoteShape->m_eNoteLineType = GetLineStyle1(oBuffer.ReadByte());
	oBuffer.ReadByte(pNoteShape->m_chNoteLineWidth);
	oBuffer.ReadColor(pNoteShape->m_nNoteLineColor);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pNoteShape;
}


}

#include "HWPRecordListHeader.h"

namespace HWP
{
CHWPRecordListHeader::CHWPRecordListHeader(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

int CHWPRecordListHeader::GetCount(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	short m_shParas;
	oBuffer.ReadShort(m_shParas);
	oBuffer.Skip(4);

	return m_shParas;
}

EVertAlign CHWPRecordListHeader::GetVertAlign(int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.Skip(2);
	int nAttr;
	oBuffer.ReadInt(nAttr);

	return HWP::GetVertAlign((nAttr >> 21) & 0x03);
}
}

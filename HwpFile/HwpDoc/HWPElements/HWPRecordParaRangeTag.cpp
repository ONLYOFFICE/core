#include "HWPRecordParaRangeTag.h"
#include "../Paragraph/RangeTag.h"

namespace HWP
{
CHWPRecordParaRangeTag::CHWPRecordParaRangeTag(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

int CHWPRecordParaRangeTag::Parse(CHWPPargraph& oPara, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	while (nSize - oBuffer.GetDistanceToLastPos() >= 12)
	{
		TRangeTag oRangeTag;

		oBuffer.ReadInt(oRangeTag.m_nStartPos);
		oBuffer.ReadInt(oRangeTag.m_nEndPos);

		for (unsigned short ushIndex = 0; ushIndex < 3; ++ushIndex)
			oBuffer.ReadByte(oRangeTag.m_arData[ushIndex]);

		oBuffer.ReadByte(oRangeTag.m_chType);

		oPara.AddRangeTag(oRangeTag);
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}
}

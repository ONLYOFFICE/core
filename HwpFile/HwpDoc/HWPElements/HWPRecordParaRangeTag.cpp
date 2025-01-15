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

	TRangeTag *pRangeTag = nullptr;

	while (nSize - oBuffer.GetDistanceToLastPos() >= 12)
	{
		pRangeTag = new TRangeTag();

		if (nullptr == pRangeTag)
		{
			oBuffer.Skip(12);
			continue;
		}

		oBuffer.ReadInt(pRangeTag->m_nStartPos);
		oBuffer.ReadInt(pRangeTag->m_nEndPos);
		oBuffer.ReadInt(pRangeTag->m_nTag);

		oPara.AddRangeTag(pRangeTag);
	}


	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}


}

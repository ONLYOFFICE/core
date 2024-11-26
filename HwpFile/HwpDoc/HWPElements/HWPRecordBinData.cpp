#include "HWPRecordBinData.h"

namespace HWP
{
CHWPRecordBinData::CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize)
{

}

std::string CHWPRecordBinData::GetItemID() const
{
	return m_sItemID;
}

}

#include "HWPDocInfo.h"

#include "HWPElements/HWPRecordBinData.h"

namespace HWP
{
CHWPDocInfo::CHWPDocInfo(EHanType eHanType)
	: m_eHanType(eHanType)
{

}

// CHWPDocInfo::CHWPDocInfo(CHWPXFile* pHWPXFile)
// 	: m_eHanType(EHanType::HWPX), m_pParentHWPX(pHWPXFile)
// {}

CHWPDocInfo::CHWPDocInfo(CHWPFile_Private* pHWPFile)
	: m_eHanType(EHanType::HWP), m_pParentHWP(pHWPFile)
{}

#define REMOVE_LIST_DATA(list) \
	for (CHWPRecord* pRecord : list) \
	{ \
		if (nullptr != pRecord) \
			delete pRecord; \
	} \
	list.clear() \

CHWPDocInfo::~CHWPDocInfo()
{
	REMOVE_LIST_DATA(m_lRecords);
	REMOVE_LIST_DATA(m_lFaseNames);
	REMOVE_LIST_DATA(m_lBorderFills);
	REMOVE_LIST_DATA(m_lCharShapes);
	REMOVE_LIST_DATA(m_lNumberings);
	REMOVE_LIST_DATA(m_lBullets);
	REMOVE_LIST_DATA(m_lParaShapes);
	REMOVE_LIST_DATA(m_lStyles);
	REMOVE_LIST_DATA(m_lTabDefs);

	for (std::pair<STRING, CHWPRecord*> oBinData : m_mBinDatas)
	{
		if (nullptr != oBinData.second)
			delete oBinData.second;
	}

	m_mBinDatas.clear();
}

bool CHWPDocInfo::Parse(CHWPStream& oBuffer, int nVersion)
{
	int nOff = 0;

	while (nOff < oBuffer.GetSize())
	{
		int nHeader = ((oBuffer[nOff + 3] << 24) & 0xFF000000) | ((oBuffer[nOff + 2] << 16) & 0xFF0000) | ((oBuffer[nOff + 1] << 8) & 0xFF00) | (oBuffer[nOff] & 0xFF);
		int nTagNum = nHeader & 0x3FF;
		int nLevel  = (nHeader & 0xFFC00) >> 10; //TODO:: Проверить побитовый сдвиг без знака
		int nSize   = (nHeader & 0xFFF00000) >> 20;

		if (0xFFF == nSize)
		{
			nSize = ((oBuffer[nOff + 7] << 24) & 0xFF000000) | ((oBuffer[nOff + 6] << 16) & 0xFF0000) | ((oBuffer[nOff + 5] << 8) & 0xFF00) | (oBuffer[nOff + 4] & 0xFF);
		}
		else
			nOff += 4;

		CHWPRecord *pRecord = nullptr;
		EHWPTag eTag = GetTagFromNum(nTagNum);

		switch (eTag)
		{
		case EHWPTag::HWPTAG_BIN_DATA:
		{
			CHWPRecordBinData *pBindData = new CHWPRecordBinData(*this, nTagNum, nLevel, nSize, oBuffer, nOff, nVersion);

			if (nullptr != pBindData)
				m_mBinDatas.insert(std::make_pair(pBindData->GetItemID(), pBindData));
		}
		default: break;
		}
	}

	return true;
}
}

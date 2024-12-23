#include "HWPDocInfo.h"

#include "HWPElements/HWPRecordBinData.h"
#include "HWPElements/HWPRecordDocumentProperties.h"
#include "HWPElements/HWPRecordIDMaping.h"
#include "HWPElements/HWPRecordFaceName.h"
#include "HWPElements/HWPRecordBorderFill.h"
#include "HWPElements/HWPRecordCharShape.h"
#include "HwpDoc/HWPElements/HWPRecordBullet.h"
#include "HwpDoc/HWPElements/HWPRecordNumbering.h"
#include "HwpDoc/HWPElements/HWPRecordParaShape.h"
#include "HwpDoc/HWPElements/HWPRecordStyle.h"
#include "HwpDoc/HWPElements/HwpRecordTabDef.h"

namespace HWP
{
ECompatDoc GetCompatDoc(int nValue)
{
	switch(static_cast<ECompatDoc>(nValue))
	{
		case ECompatDoc::HWP: return ECompatDoc::HWP;
		case ECompatDoc::OLD_HWP: return ECompatDoc::OLD_HWP;
		case ECompatDoc::MS_WORD: return ECompatDoc::MS_WORD;
		default:
			return ECompatDoc::UNKNOWN;
	}
}

CHWPDocInfo::CHWPDocInfo(EHanType eHanType)
	: m_eHanType(eHanType)
{}

// CHWPDocInfo::CHWPDocInfo(CHWPXFile* pHWPXFile)
// 	: m_eHanType(EHanType::HWPX), m_pParentHWPX(pHWPXFile)
// {}

CHWPDocInfo::CHWPDocInfo(CHWPFile_Private* pHWPFile)
	: m_eHanType(EHanType::HWP), m_pParentHWP(pHWPFile)
{}

CHWPDocInfo::~CHWPDocInfo()
{
	#define REMOVE_LIST_DATA(array) \
	for (CHWPRecord* pRecord : array) \
	{ \
		if (nullptr != pRecord) \
			delete pRecord; \
	} \
	array.clear() \

	REMOVE_LIST_DATA(m_arRecords);
	REMOVE_LIST_DATA(m_arFaseNames);
	REMOVE_LIST_DATA(m_arBorderFills);
	REMOVE_LIST_DATA(m_arCharShapes);
	REMOVE_LIST_DATA(m_arNumberings);
	REMOVE_LIST_DATA(m_arBullets);
	REMOVE_LIST_DATA(m_arParaShapes);
	REMOVE_LIST_DATA(m_arStyles);
	REMOVE_LIST_DATA(m_arTabDefs);

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

		#define CREATE_AND_ADD_RECORD(type_record, array) \
		pRecord = new type_record(*this, nTagNum, nLevel, nSize, oBuffer, nOff, nVersion); \
		if (nullptr != pRecord) \
			array.push_back(pRecord)

		switch (eTag)
		{
		case HWPTAG_DOCUMENT_PROPERTIES:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordDocumentProperties, m_arRecords);
			break;
		}
		case HWPTAG_ID_MAPPINGS:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordIDMaping, m_arRecords);
			break;
		}
		case HWPTAG_BIN_DATA:
		{
			CHWPRecordBinData *pBindData = new CHWPRecordBinData(*this, nTagNum, nLevel, nSize, oBuffer, nOff, nVersion);

			if (nullptr != pBindData)
				m_mBinDatas.insert(std::make_pair(pBindData->GetItemID(), pBindData));

			break;
		}
		case HWPTAG_FACE_NAME:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordFaceName, m_arFaseNames);
			break;
		}
		case HWPTAG_BORDER_FILL:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordBorderFill, m_arBorderFills);
			break;
		}
		case HWPTAG_CHAR_SHAPE:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordCharShape, m_arCharShapes);
			break;
		}
		case HWPTAG_TAB_DEF:
		{
			CREATE_AND_ADD_RECORD(CHwpRecordTabDef, m_arTabDefs);
			break;
		}
		case HWPTAG_NUMBERING:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordNumbering, m_arNumberings);
			break;
		}
		case HWPTAG_BULLET:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordBullet, m_arBullets);
			break;
		}
		case HWPTAG_PARA_SHAPE:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordParaShape, m_arParaShapes);
			break;
		}
		case HWPTAG_STYLE:
		{
			CREATE_AND_ADD_RECORD(CHWPRecordStyle, m_arStyles);
			break;
		}
		case HWPTAG_COMPATIBLE_DOCUMENT:
		{
			int nCompatDoc;
			oBuffer.ReadInt(nCompatDoc);

			m_eCompatibleDoc = GetCompatDoc(nCompatDoc);
			break;
		}
		case HWPTAG_LAYOUT_COMPATIBILITY:
		case HWPTAG_DOC_DATA:
		case HWPTAG_DISTRIBUTE_DOC_DATA:
		case HWPTAG_TRACKCHANGE:
		case HWPTAG_MEMO_SHAPE:
		case HWPTAG_FORBIDDEN_CHAR:
		case HWPTAG_TRACK_CHANGE:
		case HWPTAG_TRACK_CHANGE_AUTHOR:
		default:
		{
			oBuffer.Skip(nSize);
			break;
		}
		}
	}

	return true;
}

#define GET_RECORD(array_records, index) \
	if (array_records.size() <= index) \
		return nullptr; \
	return array_records[index]

const CHWPRecord* CHWPDocInfo::GetRecord(int nIndex) const
{
	GET_RECORD(m_arRecords, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetFaceName(int nIndex) const
{
	GET_RECORD(m_arFaseNames, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetBorderFill(int nIndex) const
{
	GET_RECORD(m_arBorderFills, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetCharShape(int nIndex) const
{
	GET_RECORD(m_arCharShapes, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetNumbering(int nIndex) const
{
	GET_RECORD(m_arNumberings, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetBullet(int nIndex) const
{
	GET_RECORD(m_arBullets, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetParaShape(int nIndex) const
{
	GET_RECORD(m_arParaShapes, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetStyle(int nIndex) const
{
	GET_RECORD(m_arStyles, nIndex);
}

const CHWPRecord* CHWPDocInfo::GetTabDef(int nIndex) const
{
	GET_RECORD(m_arTabDefs, nIndex);
}

CHWPFile_Private* CHWPDocInfo::GetParentHWP()
{
	return m_pParentHWP;
}
}

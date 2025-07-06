#include "HWPDocInfo.h"

#include "HWPElements/HWPRecordBinData.h"
#include "HWPElements/HWPRecordDocumentProperties.h"
#include "HWPElements/HWPRecordIDMaping.h"
#include "HWPElements/HWPRecordFaceName.h"
#include "HWPElements/HWPRecordBorderFill.h"
#include "HWPElements/HWPRecordCharShape.h"
#include "HWPElements/HWPRecordBullet.h"
#include "HWPElements/HWPRecordNumbering.h"
#include "HWPElements/HWPRecordParaShape.h"
#include "HWPElements/HWPRecordStyle.h"
#include "HWPElements/HwpRecordTabDef.h"

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
	: m_eHanType(eHanType), m_eCompatibleDoc(ECompatDoc::HWP)
{}

CHWPDocInfo::CHWPDocInfo(CHWPXFile* pHWPXFile)
	: m_eHanType(EHanType::HWPX), m_pParentHWPX(pHWPXFile), m_eCompatibleDoc(ECompatDoc::UNKNOWN)
{}

CHWPDocInfo::CHWPDocInfo(CHWPFile* pHWPFile)
	: m_eHanType(EHanType::HWP), m_pParentHWP(pHWPFile), m_eCompatibleDoc(ECompatDoc::HWP)
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

	for (const std::pair<HWP_STRING, CHWPRecord*>& oBinData : m_mBinDatas)
	{
		if (nullptr != oBinData.second)
			delete oBinData.second;
	}

	m_mBinDatas.clear();
}

bool CHWPDocInfo::Parse(CHWPStream& oBuffer, int nVersion)
{
	int nHeader, nTagNum, nLevel, nSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (0xFFF == nSize)
		{
			//TODO:: buf[off+7]<<24&0xFF000000 | buf[off+6]<<16&0xFF0000 | buf[off+5]<<8&0xFF00 | buf[off+4]&0xFF;
			oBuffer.ReadInt(nSize);
		}

		CHWPRecord *pRecord = nullptr;
		EHWPTag eTag = GetTagFromNum(nTagNum);

		#define CREATE_AND_ADD_RECORD(type_record, array) \
		pRecord = new type_record(*this, nTagNum, nLevel, nSize, oBuffer, 0, nVersion); \
		if (nullptr != pRecord) \
			array.push_back(pRecord)

		oBuffer.SavePosition();

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
			CHWPRecordBinData *pBindData = new CHWPRecordBinData(*this, nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

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
		case HWPTAG_HWP_CHAR_SHAPE:
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
		case HWPTAG_FORBIDDEN_HWP_CHAR:
		case HWPTAG_TRACK_CHANGE:
		case HWPTAG_TRACK_CHANGE_AUTHOR:
			break;
		default:
		{}
		}

		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	}

	return true;
}

bool CHWPDocInfo::Parse(CXMLReader& oReader, int nVersion)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hh:beginNum" == sNodeName)
			m_arRecords.push_back(new CHWPRecordDocumentProperties(*this, oReader, nVersion));
		else if ("hh:refList" == sNodeName)
			ReadRefList(oReader, nVersion);
	}
	END_WHILE

	return true;
}

bool CHWPDocInfo::ReadRefList(CXMLReader& oReader, int nVersion)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hh:fontfaces" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, FontFace, "hh:fontface")
			        WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Font, "hh:font")
			        m_arFaseNames.push_back(new CHWPRecordFaceName(*this, oReader, nVersion));
				END_WHILE
			END_WHILE
		}
		else if ("hh:borderFills" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, BorderFill, "hh:borderFill")
				m_arBorderFills.push_back(new CHWPRecordBorderFill(*this, oReader, nVersion));
			END_WHILE
		}
		else if ("hh:charProperties" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, CharPr, "hh:charPr")
				m_arCharShapes.push_back(new CHWPRecordCharShape(*this, oReader, nVersion));
			END_WHILE
		}
		else if ("hh:tabProperties" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, TabPr, "hh:tabPr")
				m_arTabDefs.push_back(new CHwpRecordTabDef(*this, oReader, nVersion));
			END_WHILE
		}
		else if ("hh:numberings" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Numbering, "hh:numbering")
				m_arNumberings.push_back(new CHWPRecordNumbering(*this, oReader, nVersion));
			END_WHILE
		}
		else if ("hh:bullets" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH(oReader, Bullet)
				m_arBullets.push_back(new CHWPRecordBullet(*this, oReader, nVersion));
		}
		else if ("hh:paraProperties" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, ParaPr, "hh:paraPr")
				m_arParaShapes.push_back(new CHWPRecordParaShape(*this, oReader, nVersion));
			END_WHILE
		}
		else if ("hh:styles" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Style, "hh:style")
				m_arStyles.push_back(new CHWPRecordStyle(*this, oReader, nVersion));
			END_WHILE
		}
	}
	END_WHILE

	return true;
}

bool CHWPDocInfo::ReadContentHpf(CXMLReader& oReader, int nVersion)
{
	CHWPRecordBinData *pRecordBinData = nullptr;

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "opf:manifest")
	{
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Item, "opf:item")
		{
			pRecordBinData = new CHWPRecordBinData(oReader, nVersion);
			m_mBinDatas.insert(std::make_pair<HWP_STRING, CHWPRecord*>(pRecordBinData->GetItemID(), (HWP::CHWPRecord*)pRecordBinData));
		}
		END_WHILE
	}
	END_WHILE

	return true;
}

#define GET_RECORD(array_records, index) \
	if (array_records.size() <= index || index < 0) \
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
	GET_RECORD(m_arBorderFills, nIndex - 1);
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
	GET_RECORD(m_arBullets, nIndex - 1);
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

CHWPFile* CHWPDocInfo::GetParentHWP()
{
	return m_pParentHWP;
}

const CHWPRecord* CHWPDocInfo::GetBinData(const HWP_STRING& sID) const
{
	switch (m_eHanType)
	{
		case EHanType::HWP:
		case EHanType::HWPX:
		{
			std::map<HWP_STRING, CHWPRecord*>::const_iterator itFound = m_mBinDatas.find(sID);

			if (m_mBinDatas.cend() != itFound)
				return itFound->second;
		}
		default:
			return nullptr;
	}
}

EHanType CHWPDocInfo::GetHanType() const
{
	return m_eHanType;
}

ECompatDoc CHWPDocInfo::GetCompatibleDoc() const
{
	return m_eCompatibleDoc;
}
}

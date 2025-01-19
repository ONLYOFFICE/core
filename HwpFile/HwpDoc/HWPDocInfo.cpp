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

bool CHWPDocInfo::Parse(CXMLNode& oNode, int nVersion)
{
	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hh:beginNum" == oChild.GetName())
			m_arRecords.push_back(new CHWPRecordDocumentProperties(*this, oChild, nVersion));
		else if (L"hh:refList" == oChild.GetName())
			ReadRefList(oChild, nVersion);
	}

	return true;
}

bool CHWPDocInfo::ReadRefList(CXMLNode& oNode, int nVersion)
{
	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hh:fontfaces" == oChild.GetName())
		{
			for (CXMLNode& oFontFaceNode : oChild.GetChilds(L"hh:fontface"))
				for (CXMLNode& oFontNode : oFontFaceNode.GetChilds(L"hh:font"))
					m_arFaseNames.push_back(new CHWPRecordFaceName(*this, oFontNode, nVersion));
		}
		else if (L"hh:borderFills" == oChild.GetName())
		{
			for (CXMLNode& oBorderFillNode : oChild.GetChilds(L"hh:borderFill"))
				m_arBorderFills.push_back(new CHWPRecordBorderFill(*this, oBorderFillNode, nVersion));
		}
		else if (L"hh:charProperties" == oChild.GetName())
		{
			for (CXMLNode& oCharPrNode : oChild.GetChilds(L"hh:charPr"))
				m_arCharShapes.push_back(new CHWPRecordCharShape(*this, oCharPrNode, nVersion));
		}
		else if (L"hh:tabProperties" == oChild.GetName())
		{
			for (CXMLNode& oTabPrNode : oChild.GetChilds(L"hh:tabPr"))
				m_arTabDefs.push_back(new CHwpRecordTabDef(*this, oTabPrNode, nVersion));
		}
		else if (L"hh:numberings" == oChild.GetName())
		{
			for (CXMLNode& oNumberingNode : oChild.GetChilds(L"hh:numbering"))
				m_arNumberings.push_back(new CHWPRecordNumbering(*this, oNumberingNode, nVersion));
		}
		else if (L"hh:bullets" == oChild.GetName())
		{
			for (CXMLNode& oBulletNode : oChild.GetChilds())
				m_arBullets.push_back(new CHWPRecordBullet(*this, oBulletNode, nVersion));
		}
		else if (L"hh:paraProperties" == oChild.GetName())
		{
			for (CXMLNode& oParaPrNode : oChild.GetChilds(L"hh:paraPr"))
				m_arParaShapes.push_back(new CHWPRecordParaShape(*this, oParaPrNode, nVersion));
		}
		else if (L"hh:styles" == oChild.GetName())
		{
			for (CXMLNode& oStyleNode : oChild.GetChilds(L"hh:style"))
				m_arStyles.push_back(new CHWPRecordStyle(*this, oStyleNode, nVersion));
		}
	}

	return true;
}

bool CHWPDocInfo::ReadContentHpf(CXMLNode& oNode, int nVersion)
{
	CHWPRecordBinData *pRecordBinData = nullptr;

	for (CXMLNode& oChild : oNode.GetChilds(L"opf:manifest"))
	{
		for (CXMLNode& oGrandChild : oChild.GetChilds(L"opf:item"))
		{
			pRecordBinData = new CHWPRecordBinData(oGrandChild, nVersion);
			m_mBinDatas.insert(std::make_pair<HWP_STRING, CHWPRecord*>(pRecordBinData->GetItemID(), pRecordBinData));
		}
	}

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
	GET_RECORD(m_arNumberings, nIndex - 1);
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
	if (m_mBinDatas.end() == m_mBinDatas.find(sID))
		return nullptr;

	return m_mBinDatas.at(sID);
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

#include "HWPRecordStyle.h"

namespace HWP
{
CHWPRecordStyle::CHWPRecordStyle(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr)
{}

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.ReadString(m_sName, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sEngName, EStringCharacter::UTF16);

	m_chType = (HWP_BYTE)(oBuffer.ReadByte() & 0x00FF);
	m_chNextStyle = (HWP_BYTE)(oBuffer.ReadByte() & 0x00FF);

	oBuffer.ReadShort(m_shLangID);
	m_nParaShape = oBuffer.ReadShort();
	m_nCharShape = oBuffer.ReadShort();
}

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_STYLE, 0, 0), m_pParent(&oDocInfo)
{
	HWP_STRING sType = oNode.GetAttribute(L"type");

	if (L"PARA" == sType)
		m_chType = 0;
	else if (L"CHAR" == sType)
		m_chType = 1;

	m_sName = oNode.GetAttribute(L"name");
	m_sEngName = oNode.GetAttribute(L"engName");
	m_nParaShape = oNode.GetAttributeInt(L"paraPrIDRef");
	m_nCharShape = oNode.GetAttributeInt(L"charPrIDRef");
	m_chNextStyle = oNode.GetAttributeInt(L"nextStyleIDRef");
	m_shLangID = oNode.GetAttributeInt(L"langID");
	m_bLockForm = oNode.GetAttributeBool(L"lockForm");
}
}

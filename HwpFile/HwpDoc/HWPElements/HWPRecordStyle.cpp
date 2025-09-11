#include "HWPRecordStyle.h"

#include "../Common/NodeNames.h"
#include <iostream>
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

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_STYLE, 0, 0), m_pParent(&oDocInfo)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::Para, eType) == sType)
				m_chType = 0;
			else if (GetValueName(EValue::Char, eType) == sType)
				m_chType = 1;
		}
		else if (GetAttributeName(EAttribute::Name, eType) == sAttributeName)
			m_sName = oReader.GetText();
		else if (GetAttributeName(EAttribute::EngName, eType) == sAttributeName)
			m_sEngName = oReader.GetText();
		else if (GetAttributeName(EAttribute::ParaShape, eType) == sAttributeName)
			m_nParaShape = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CharShape, eType) == sAttributeName)
			m_nCharShape = oReader.GetInt();
		else if (GetAttributeName(EAttribute::NextStyle, eType) == sAttributeName)
			m_chNextStyle = oReader.GetInt();
		else if (GetAttributeName(EAttribute::LangId, eType) == sAttributeName)
			m_shLangID = oReader.GetInt();
		else if (GetAttributeName(EAttribute::LockForm, eType) == sAttributeName)
			m_bLockForm = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)
}

HWP_STRING CHWPRecordStyle::GetName() const
{
	return m_sName;
}

HWP_STRING CHWPRecordStyle::GetEngName() const
{
	return m_sEngName;
}

HWP_BYTE CHWPRecordStyle::GetType() const
{
	return m_chType;
}

HWP_BYTE CHWPRecordStyle::GetNextStyle() const
{
	return m_chNextStyle;
}

short CHWPRecordStyle::GetLangId() const
{
	return m_shLangID;
}

int CHWPRecordStyle::GetParaShapeId() const
{
	return m_nParaShape;
}

int CHWPRecordStyle::GetCharShapeId() const
{
	return m_nCharShape;
}

bool CHWPRecordStyle::LockForm() const
{
	return m_bLockForm;
}
}

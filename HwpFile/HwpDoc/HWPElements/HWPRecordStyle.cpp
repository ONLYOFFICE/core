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

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_STYLE, 0, 0), m_pParent(&oDocInfo)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("type" == sAttributeName)
		{
			const std::string sType{oReader.GetText2A()};

			if ("PARA" == sType)
				m_chType = 0;
			else if ("CHAR" == sType)
				m_chType = 1;
		}
		else if ("name" == sAttributeName)
			m_sName = oReader.GetText2();
		else if ("engName" == sAttributeName)
			m_sEngName = oReader.GetText2();
		else if ("paraPrIDRef" == sAttributeName)
			m_nParaShape = oReader.GetInt();
		else if ("charPrIDRef" == sAttributeName)
			m_nCharShape = oReader.GetInt();
		else if ("nextStyleIDRef" == sAttributeName)
			m_chNextStyle = oReader.GetInt();
		else if ("langID" == sAttributeName)
			m_shLangID = oReader.GetInt();
		else if ("lockForm" == sAttributeName)
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

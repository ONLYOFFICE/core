#include "CtrlNewNumber.h"

namespace HWP
{
CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = ::HWP::GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2((nAttr >> 4) & 0xF);

	oBuffer.ReadShort(m_shNum);

	m_bFullFilled = true;
}

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
	: CCtrl(sCtrlID)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("num" == sAttributeName)
			m_shNum = oReader.GetInt();
		else if ("numType" == sAttributeName)
			m_eNumType = ::HWP::GetNumType(oReader.GetText());
		//TODO:: проверить данный момент
		else if ("autoNumFormat" == sAttributeName)
			m_eNumShape = GetNumberShape2(oReader.GetInt());
	}
	END_READ_ATTRIBUTES(oReader)

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlNewNumber::GetCtrlType() const
{
	return ECtrlObjectType::NewNumber;
}

ENumType CCtrlNewNumber::GetNumType() const
{
	return m_eNumType;
}

short CCtrlNewNumber::GetNum() const
{
	return m_shNum;
}
}

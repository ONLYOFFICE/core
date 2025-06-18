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

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID)
{
	m_shNum = oNode.GetAttributeInt(L"num");
	m_eNumType = ::HWP::GetNumType(oNode.GetAttribute(L"numType"));
	//TODO:: проверить данный момент
	m_eNumShape = GetNumberShape2(oNode.GetAttributeInt(L"autoNumFormat"));

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

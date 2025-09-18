#include "Ctrl.h"

#include "CtrlColumnDef.h"
#include "CtrlAutoNumber.h"
#include "CtrlHeadFoot.h"
#include "CtrlNewNumber.h"
#include "CtrlNote.h"
#include "CtrlPageNumPos.h"
#include "CtrlField.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrl::CCtrl()
	:  m_bFullFilled(false)
{}

CCtrl::CCtrl(const HWP_STRING& sCtrlID)
	: m_sCtrlID(sCtrlID), m_bFullFilled(false)
{}

CCtrl::~CCtrl()
{}

void CCtrl::SetID(const HWP_STRING& sCtrlID)
{
	m_sCtrlID = sCtrlID;
}

HWP_STRING CCtrl::GetID() const
{
	return m_sCtrlID;
}

bool CCtrl::FullFilled() const
{
	return m_bFullFilled;
}

void CCtrl::SetFullFilled()
{
	m_bFullFilled = true;
}

bool CCtrl::Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl)
{
	if (nullptr == pFirstCtrl || nullptr == pSecondCtrl)
		return false;

	return pFirstCtrl->m_sCtrlID == pSecondCtrl->m_sCtrlID &&
	       pFirstCtrl->m_bFullFilled == pSecondCtrl->m_bFullFilled;
}

CCtrl* CCtrl::GetCtrl(CXMLReader& oReader, int nVersion, EHanType eType)
{
	const std::string sNodeName{oReader.GetName()};

	if ("hp:colPr" == sNodeName)
		return new CCtrlColumnDef(L"dloc", oReader, nVersion);
	else if (GetNodeName(ENode::Header, eType) == sNodeName)
		return new CCtrlHeadFoot(L"daeh", oReader, nVersion, eType);
	else if (GetNodeName(ENode::Footer, eType) == sNodeName)
		return new CCtrlHeadFoot(L"toof", oReader, nVersion, eType);
	else if ("hp:footNote" == sNodeName)
		return new CCtrlNote(L"  nf", oReader, nVersion);
	else if ("hp:endNote" == sNodeName)
		return new CCtrlNote(L"  ne", oReader, nVersion);
	else if ("hp:autoNum" == sNodeName)
		return new CCtrlAutoNumber(L"onta", oReader, nVersion);
	else if ("hp:newNum" == sNodeName)
		return new CCtrlNewNumber(L"onwn", oReader, nVersion);
	else if ("hp:pageNum" == sNodeName)
		return new CCtrlPageNumPos(L"pngp", oReader, nVersion);
	else if ("hp:fieldBegin" == sNodeName ||
	         "hp:fieldEnd"   == sNodeName)
		return new CCtrlField(L"", oReader, nVersion);

	return nullptr;
}
}

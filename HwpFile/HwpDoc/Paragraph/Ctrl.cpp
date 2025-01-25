#include "Ctrl.h"

#include "CtrlColumnDef.h"
#include "CtrlAutoNumber.h"
#include "CtrlHeadFoot.h"
#include "CtrlNewNumber.h"
#include "CtrlNote.h"
#include "CtrlPageNumPos.h"
#include "CCtrlField.h"

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

CCtrl* CCtrl::GetCtrl(CXMLNode& oNode, int nVersion)
{
	if (L"hp:colPr" == oNode.GetName())
		return new CCtrlColumnDef(L"dloc", oNode, nVersion);
	else if (L"hp:header" == oNode.GetName())
		return new CCtrlHeadFoot(L"daeh", oNode, nVersion);
	else if (L"hp:footer" == oNode.GetName())
		return new CCtrlHeadFoot(L"toof", oNode, nVersion);
	else if (L"hp:footNote" == oNode.GetName())
		return new CCtrlNote(L"  nf", oNode, nVersion);
	else if (L"hp:endNote" == oNode.GetName())
		return new CCtrlNote(L"  ne", oNode, nVersion);
	else if (L"hp:autoNum" == oNode.GetName())
		return new CCtrlAutoNumber(L"onta", oNode, nVersion);
	else if (L"hp:newNum" == oNode.GetName())
		return new CCtrlNewNumber(L"onwn", oNode, nVersion);
	else if (L"hp:pageNum" == oNode.GetName())
		return new CCtrlPageNumPos(L"pngp", oNode, nVersion);
	else if (L"hp:fieldBegin" == oNode.GetName() ||
	         L"hp:fieldEnd" == oNode.GetName())
		return new CCtrlField(L"", oNode, nVersion);

	return nullptr;
}
}

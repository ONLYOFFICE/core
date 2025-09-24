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

CCtrl* CCtrl::GetCtrl(CXMLReader& oReader, EHanType eType)
{
	const std::string sNodeName{oReader.GetName()};

	if (GetNodeName(ENode::ColumnDef, eType) == sNodeName)
		return new CCtrlColumnDef(L"dloc", oReader, eType);
	else if (GetNodeName(ENode::Header, eType) == sNodeName)
		return new CCtrlHeadFoot(L"daeh", oReader, eType);
	else if (GetNodeName(ENode::Footer, eType) == sNodeName)
		return new CCtrlHeadFoot(L"toof", oReader, eType);
	else if (GetNodeName(ENode::FootNote, eType) == sNodeName)
		return new CCtrlNote(L"  nf", oReader, eType);
	else if (GetNodeName(ENode::EndNote, eType) == sNodeName)
		return new CCtrlNote(L"  ne", oReader, eType);
	else if (GetNodeName(ENode::AutoNum, eType) == sNodeName)
		return new CCtrlAutoNumber(L"onta", oReader, eType);
	else if (GetNodeName(ENode::NewNum, eType) == sNodeName)
		return new CCtrlNewNumber(L"onwn", oReader, eType);
	else if (GetNodeName(ENode::PageNum, eType) == sNodeName)
		return new CCtrlPageNumPos(L"pngp", oReader, eType);
	else if (GetNodeName(ENode::FieldBegin, eType) == sNodeName ||
	         GetNodeName(ENode::FieldEnd,   eType) == sNodeName)
		return new CCtrlField(L"", oReader, eType);

	return nullptr;
}
}

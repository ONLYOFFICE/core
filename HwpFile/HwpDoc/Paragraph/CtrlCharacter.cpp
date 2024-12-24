#include "CtrlCharacter.h"

namespace HWP
{
CCtrlCharacter::CCtrlCharacter(const STRING& sCtrlID, ECtrlCharType eCtrlChar)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar)
{}

CCtrlCharacter::CCtrlCharacter(const STRING& sCtrlID, ECtrlCharType eCtrlChar, int nCharShapeID)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar), m_nCharShapeID(nCharShapeID)
{}

int CCtrlCharacter::GetSize()
{
	return 1;
}

ECtrlCharType CCtrlCharacter::GetType() const
{
	return m_eCtrlChar;
}

void CCtrlCharacter::SetCharShapeID(int nCharShapeID)
{
	m_nCharShapeID = nCharShapeID;
}
}

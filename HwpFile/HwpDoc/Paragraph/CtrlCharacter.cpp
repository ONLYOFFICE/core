#include "CtrlCharacter.h"

namespace HWP
{
CCtrlCharacter::CCtrlCharacter(const HWP_STRING& sCtrlID, ECtrlCharType eCtrlChar)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar)
{}

CCtrlCharacter::CCtrlCharacter(const HWP_STRING& sCtrlID, ECtrlCharType eCtrlChar, int nCharShapeID)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar), m_nCharShapeID(nCharShapeID)
{}

ECtrlObjectType CCtrlCharacter::GetCtrlType() const
{
	return ECtrlObjectType::Character;
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

#include "CtrlCharacter.h"

namespace HWP
{
CCtrlCharacter::CCtrlCharacter(const std::string& sCtrlID, ECtrlCharType eCtrlChar)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar)
{}

CCtrlCharacter::CCtrlCharacter(const std::string& sCtrlID, ECtrlCharType eCtrlChar, int nCharShapeID)
	: CCtrl(sCtrlID), m_eCtrlChar(eCtrlChar), m_nCharShapeID(nCharShapeID)
{}

int CCtrlCharacter::GetSize()
{
	return 1;
}
}

#ifndef CTRLCHARACTER_H
#define CTRLCHARACTER_H

#include "Ctrl.h"

namespace HWP
{
enum class ECtrlCharType
{
	LINE_BREAK = 0x1,
	PARAGRAPH_BREAK = 0x2,
	HARD_HYPHEN = 0x3,
	HARD_SPACE = 0x04
};

class CCtrlCharacter : public CCtrl
{
	ECtrlCharType m_eCtrlChar;
	int m_nCharShapeID;
public:
	CCtrlCharacter(const STRING& sCtrlID, ECtrlCharType eCtrlChar);
	CCtrlCharacter(const STRING& sCtrlID, ECtrlCharType eCtrlChar, int nCharShapeID);

	int GetSize() override;
	ECtrlCharType GetType() const;

	void SetCharShapeID(int nCharShapeID);
};
}

#endif // CTRLCHARACTER_H

#ifndef CTRLNEWNUMBER_H
#define CTRLNEWNUMBER_H

#include "CtrlAutoNumber.h"

namespace HWP
{
class CCtrlNewNumber : public CCtrl
{
	int m_nSize;

	ENumType m_eNumType;
	ENumberShape2 m_eNumShape;
	short m_shNum;
public:
	CCtrlNewNumber(const HWP_STRING& sCtrlID);
	CCtrlNewNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLNEWNUMBER_H

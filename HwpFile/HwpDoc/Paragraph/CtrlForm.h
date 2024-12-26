#ifndef CTRLFORM_H
#define CTRLFORM_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CCtrlForm : public CCtrl
{
	int m_nSize;
public:
	CCtrlForm(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLFORM_H

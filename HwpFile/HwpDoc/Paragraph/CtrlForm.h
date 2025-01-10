#ifndef CTRLFORM_H
#define CTRLFORM_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CCtrlForm : public CCtrl
{
public:
	CCtrlForm(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	ECtrlObjectType GetCtrlType() const override;
};
}

#endif // CTRLFORM_H

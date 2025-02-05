#ifndef CTRLEMPTY_H
#define CTRLEMPTY_H

#include "Ctrl.h"

namespace HWP
{
class CCtrlEmpty : public CCtrl
{
public:
	CCtrlEmpty();
	CCtrlEmpty(const HWP_STRING& sCtrlID);

	ECtrlObjectType GetCtrlType() const override;
};
}

#endif // CTRLEMPTY_H

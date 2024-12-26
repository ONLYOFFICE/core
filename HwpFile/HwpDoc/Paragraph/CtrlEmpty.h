#ifndef CTRLEMPTY_H
#define CTRLEMPTY_H

#include "Ctrl.h"

namespace HWP
{
class CCtrlEmpty : public CCtrl
{
	int m_nSize;
public:
	CCtrlEmpty();
	CCtrlEmpty(const HWP_STRING& sCtrlID);

	int GetSize() override;
	void SetSize(int nSize);
};
}

#endif // CTRLEMPTY_H

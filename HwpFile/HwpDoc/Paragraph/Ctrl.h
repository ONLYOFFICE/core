#ifndef CTRL_H
#define CTRL_H

#include "../Common.h"

namespace HWP
{
class CCtrl
{
	HWP_STRING m_sCtrlID;
protected:
	bool m_bFullFilled;
public:
	CCtrl();
	CCtrl(const HWP_STRING& sCtrlID);
	virtual ~CCtrl();

	void SetID(const HWP_STRING& sCtrlID);
	HWP_STRING GetID() const;
	bool FullFilled() const;

	void SetFullFilled();

	virtual int GetSize() = 0;

	static bool Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl);
};
}

#endif // CTRL_H

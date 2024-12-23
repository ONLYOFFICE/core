#ifndef CTRL_H
#define CTRL_H

#include "../Common.h"

namespace HWP
{
class CCtrl
{
	STRING m_sCtrlID;
protected:
	bool m_bFullFilled;
public:
	CCtrl();
	CCtrl(const STRING& sCtrlID);
	virtual ~CCtrl();

	void SetID(const STRING& sCtrlID);
	STRING GetID() const;
	bool FullFilled() const;

	void SetFullFilled();

	virtual int GetSize() = 0;

	static bool Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl);
};
}

#endif // CTRL_H

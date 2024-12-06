#ifndef CTRL_H
#define CTRL_H

#include <string>

namespace HWP
{
class CCtrl
{
	std::string m_sCtrlID;
	bool m_bFullFilled;

	friend class CCtrlColumnDef;
	friend class CCtrlHeadFoot;
	friend class CCtrlEqEdit;
public:
	CCtrl();
	CCtrl(const std::string& sCtrlID);

	void SetID(const std::string& sCtrlID);
	std::string GetID() const;

	virtual int GetSize() = 0;
};
}

#endif // CTRL_H

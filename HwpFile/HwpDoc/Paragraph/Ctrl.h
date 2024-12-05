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
public:
	CCtrl();
	CCtrl(const std::string& sCtrlID);

	std::string GetID() const;

	virtual int GetSize() = 0;
};
}

#endif // CTRL_H

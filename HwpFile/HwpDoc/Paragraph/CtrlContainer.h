#ifndef CTRLCONTAINER_H
#define CTRLCONTAINER_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlContainer : public CCtrlGeneralShape
{
	short m_shNElement;
	std::vector<std::string> m_arCtrlIdList;
	std::vector<CCtrlGeneralShape*> m_arList;
public:
	CCtrlContainer(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlContainer();

	static int ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCONTAINER_H

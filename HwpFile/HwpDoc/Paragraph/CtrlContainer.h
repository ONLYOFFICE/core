#ifndef CTRLCONTAINER_H
#define CTRLCONTAINER_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlContainer : public CCtrlGeneralShape
{
	short m_shNElement;
	VECTOR<STRING> m_arCtrlIdList;
	VECTOR<CCtrlGeneralShape*> m_arList;
public:
	CCtrlContainer(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlContainer();

	bool Empty() const;

	template <typename FindClass>
	FindClass* FindLastElement();

	static int ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCONTAINER_H

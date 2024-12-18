#ifndef CTRLCONTAINER_H
#define CTRLCONTAINER_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlContainer : public CCtrlGeneralShape
{
	short m_shNElement;
	VECTOR<STRING> m_arCtrlIdList;
	VECTOR<CCtrlGeneralShape*> m_arShapes;
public:
	CCtrlContainer(const STRING& sCtrlID);
	CCtrlContainer(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlContainer();

	bool Empty() const;

	VECTOR<CCtrlGeneralShape*> GetShapes() const;

	void AddShape(CCtrlGeneralShape* pShape);
	CCtrlGeneralShape* GetLastShape();

	static int ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCONTAINER_H

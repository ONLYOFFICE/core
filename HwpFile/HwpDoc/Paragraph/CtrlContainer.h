#ifndef CTRLCONTAINER_H
#define CTRLCONTAINER_H

#include "CtrlGeneralShape.h"

namespace HWP
{
class CCtrlContainer : public CCtrlGeneralShape
{
	short m_shNElement;
	VECTOR<HWP_STRING> m_arCtrlIdList;
	VECTOR<CCtrlGeneralShape*> m_arShapes;
public:
	CCtrlContainer(const HWP_STRING& sCtrlID);
	CCtrlContainer(const CCtrlGeneralShape& oShape);
	CCtrlContainer(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlContainer(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);

	~CCtrlContainer();

	EShapeType GetShapeType() const override;

	bool Empty() const;

	VECTOR<CCtrlGeneralShape*> GetShapes() const;

	void AddShape(CCtrlGeneralShape* pShape);
	CCtrlGeneralShape* GetLastShape();

	static int ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCONTAINER_H

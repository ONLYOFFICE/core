#include "CtrlContainer.h"

#include "CtrlShapeArc.h"
#include "CtrlShapePic.h"
#include "CtrlShapeOle.h"
#include "CtrlShapeRect.h"
#include "CtrlShapeLine.h"
#include "CtrlContainer.h"
#include "CtrlShapeCurve.h"
#include "CtrlShapeEllipse.h"
#include "CtrlShapePolygon.h"

namespace HWP
{
CCtrlContainer::CCtrlContainer(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlContainer::~CCtrlContainer()
{
	for (CCtrlGeneralShape* pElement : m_arList)
	{
		if (nullptr != pElement)
			delete pElement;
	}
}

int CCtrlContainer::ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE *pOldCurentPos = oBuffer.GetCurPtr();

	oBuffer.ReadShort(oObj.m_shNElement);

	if (oObj.m_shNElement <= 0)
		return oBuffer.GetCurPtr() - pOldCurentPos;

	oObj.m_arCtrlIdList.reserve(oObj.m_shNElement);

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
		oBuffer.ReadString(oObj.m_arCtrlIdList[unIndex], 4);

	oObj.m_arList.reserve(oObj.m_shNElement);

	std::string sCtrlId;

	#define CREATE_OBJECT(class_name) \
	{ \
	pChldObj = new class_name(sCtrlId, nSize - (oBuffer.GetCurPtr() - pOldCurentPos), oBuffer, 0, nVersion); \
	class_name::ParseCtrl((class_name&)pChldObj, nSize, oBuffer, 0, nVersion); \
	pChldObj->SetID(sCtrlId); \
	}

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
	{
		CCtrlGeneralShape* pChldObj = nullptr;

		oBuffer.ReadString(sCtrlId, 4);

		if ("cip$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapePic)
		else if ("cer$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeRect)
		else if ("nil$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeLine)
		else if ("noc$" == sCtrlId)
			CREATE_OBJECT(CCtrlContainer)
		else if ("lle$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeEllipse)
		else if ("lop$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapePolygon)
		else if ("cra$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeArc)
		else if ("ruc$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeCurve)
		else if ("elo$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeOle)

		oObj.m_arList[unIndex] = pChldObj;
	}

	return oBuffer.GetCurPtr() - pOldCurentPos;
}

int CCtrlContainer::ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE *pOldCurentPos = oBuffer.GetCurPtr();

	CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadShort(oObj.m_shNElement);

	oObj.m_arCtrlIdList.reserve(oObj.m_shNElement);

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
		oBuffer.ReadString(oObj.m_arCtrlIdList[unIndex], 4);

	oBuffer.Skip(4);

	return oBuffer.GetCurPtr() - pOldCurentPos;
}
}

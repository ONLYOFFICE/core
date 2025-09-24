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
#include "CtrlShapeConnectLine.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlContainer::CCtrlContainer(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlContainer::CCtrlContainer(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlContainer::CCtrlContainer(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlContainer::CCtrlContainer(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::Container, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlContainer(L"noc$", oReader, eType));
		else if (GetNodeName(ENode::Line, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeLine(L"nil$", oReader, eType));
		else if (GetNodeName(ENode::Rectangle, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeRect(L"cer$", oReader, eType));
		else if (GetNodeName(ENode::Ellipse, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeEllipse(L"lle$", oReader, eType));
		else if (GetNodeName(ENode::Arc, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeArc(L"cra$", oReader, eType));
		else if (GetNodeName(ENode::Polygon, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapePolygon(L"lop$", oReader, eType));
		else if (GetNodeName(ENode::Curve, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeCurve(L"ruc$", oReader, eType));
		else if (GetNodeName(ENode::ConnectLine, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeConnectLine(L"loc$", oReader, eType));
		else if (GetNodeName(ENode::Picture, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapePic(L"cip$", oReader, eType));
		else if (GetNodeName(ENode::Ole, eType) == sNodeName)
			m_arShapes.push_back(new CCtrlShapeOle(L"elo$", oReader, eType));
	}
	END_WHILE

	m_shNElement = m_arShapes.size();
}

CCtrlContainer::~CCtrlContainer()
{
	for (const CCtrlGeneralShape* pElement : m_arShapes)
	{
		if (nullptr != pElement)
			delete pElement;
	}
}

EShapeType CCtrlContainer::GetShapeType() const
{
	return EShapeType::Container;
}

void CCtrlContainer::AddShape(CCtrlGeneralShape* pShape)
{
	m_arShapes.push_back(pShape);
}

bool CCtrlContainer::Empty() const
{
	return m_arShapes.empty();
}

VECTOR<CCtrlGeneralShape*> CCtrlContainer::GetShapes() const
{
	return m_arShapes;
}

CCtrlGeneralShape* CCtrlContainer::GetLastShape()
{
	return (!m_arShapes.empty()) ? m_arShapes.back() : nullptr;
}

int CCtrlContainer::ParseElement(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadShort(oObj.m_shNElement);

	if (oObj.m_shNElement <= 0)
		return oBuffer.GetDistanceToLastPos(true);

	oObj.m_arCtrlIdList.resize(oObj.m_shNElement);

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
		oBuffer.ReadString(oObj.m_arCtrlIdList[unIndex], 4, EStringCharacter::ASCII);

	oObj.m_arShapes.resize(oObj.m_shNElement);

	HWP_STRING sCtrlId;

	#define CREATE_OBJECT(class_name) \
	{ \
	pChldObj = new class_name(sCtrlId, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion); \
	class_name::ParseCtrl((class_name&)pChldObj, nSize, oBuffer, 0, nVersion); \
	pChldObj->SetID(sCtrlId); \
	}

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
	{
		CCtrlGeneralShape* pChldObj = nullptr;

		oBuffer.ReadString(sCtrlId, 4, EStringCharacter::ASCII);

		if (L"cip$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapePic)
		else if (L"cer$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeRect)
		else if (L"nil$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeLine)
		else if (L"noc$" == sCtrlId)
			CREATE_OBJECT(CCtrlContainer)
		else if (L"lle$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeEllipse)
		else if (L"lop$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapePolygon)
		else if (L"cra$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeArc)
		else if (L"ruc$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeCurve)
		else if (L"elo$" == sCtrlId)
			CREATE_OBJECT(CCtrlShapeOle)

		oObj.m_arShapes[unIndex] = pChldObj;
	}

	return oBuffer.GetDistanceToLastPos(true);
}

int CCtrlContainer::ParseCtrl(CCtrlContainer& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadShort(oObj.m_shNElement);

	oObj.m_arCtrlIdList.resize(oObj.m_shNElement);

	for (unsigned int unIndex = 0; unIndex < oObj.m_shNElement; ++unIndex)
		oBuffer.ReadString(oObj.m_arCtrlIdList[unIndex], 4, EStringCharacter::ASCII);

	oBuffer.Skip(4);

	return oBuffer.GetDistanceToLastPos(true);
}
}

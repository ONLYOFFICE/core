#include "CtrlObjElement.h"

namespace HWP
{
CCtrlObjElement::CCtrlObjElement()
{}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID)
	: CCtrlCommon(sCtrlID)
{}

TMatrix ReadMatrix(CHWPStream& oBuffer)
{
	TMatrix oMatrix;

	oBuffer.ReadDouble(oMatrix.m_dM11);
	oBuffer.ReadDouble(oMatrix.m_dM12);
	oBuffer.ReadDouble(oMatrix.m_dDX);
	oBuffer.ReadDouble(oMatrix.m_dM21);
	oBuffer.ReadDouble(oMatrix.m_dM22);
	oBuffer.ReadDouble(oMatrix.m_dDY);

	return oMatrix;
}

CCtrlObjElement::CCtrlObjElement(const CCtrlObjElement& oObjElement)
	: CCtrlCommon(oObjElement)
{
	m_nXGrpOffset = oObjElement.m_nXGrpOffset;
	m_nYGrpOffset = oObjElement.m_nYGrpOffset;
	m_shNGrp = oObjElement.m_shNGrp;
	m_shVer = oObjElement.m_shVer;
	m_nOrgWidth = oObjElement.m_nOrgWidth;
	m_nOrgHeight = oObjElement.m_nOrgHeight;
	m_nCurWidth = oObjElement.m_nCurWidth;
	m_nCurHeight = oObjElement.m_nCurHeight;
	m_bHorzFlip = oObjElement.m_bHorzFlip;
	m_bVerFlip = oObjElement.m_bVerFlip;
	m_shRotat = oObjElement.m_shRotat;
	m_nXCenter = oObjElement.m_nXCenter;
	m_nYCenter = oObjElement.m_nYCenter;
	m_shMatCnt = oObjElement.m_shMatCnt;
	m_arMatrixs = oObjElement.m_arMatrixs;
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

TMatrix ReadMatrix(CXMLNode& oNode)
{
	return TMatrix
	{
		oNode.GetAttributeDouble(L"e1", 1.),
		oNode.GetAttributeDouble(L"e2"),
		oNode.GetAttributeDouble(L"e4"),
		oNode.GetAttributeDouble(L"e5", 1.),
		oNode.GetAttributeDouble(L"e3"),
		oNode.GetAttributeDouble(L"e6"),
	};
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlCommon(sCtrlID, oNode, nVersion)
{
	m_shNGrp = oNode.GetAttributeInt(L"groupLevel");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:offset" == oChild.GetName())
		{
			m_nXGrpOffset = oChild.GetAttributeInt(L"x");
			m_nYGrpOffset = oChild.GetAttributeInt(L"y");
		}
		else if (L"hp:orgSz" == oChild.GetName())
		{
			m_nOrgWidth  = oChild.GetAttributeInt(L"width");
			m_nOrgHeight = oChild.GetAttributeInt(L"height");
		}
		else if (L"hp:curSz" == oChild.GetName())
		{
			m_nCurWidth  = oChild.GetAttributeInt(L"width");
			m_nCurHeight = oChild.GetAttributeInt(L"height");
		}
		else if (L"hp:flip" == oChild.GetName())
		{
			m_bHorzFlip = oChild.GetAttributeBool(L"horizontal");
			m_bVerFlip  = oChild.GetAttributeBool(L"vertical");
		}
		else if (L"hp:rotationInfo" == oChild.GetName())
		{
			m_shRotat = oChild.GetAttributeInt(L"angle");
			m_nXCenter = oChild.GetAttributeInt(L"centerX");
			m_nYCenter = oChild.GetAttributeInt(L"centerY");
		}
		else if (L"hp:renderingInfo" == oChild.GetName())
		{
			for (CXMLNode& oGrandChild : oChild.GetChilds())
			{
				// Сначала идёт 1 hc:transMatrix, а после попарно идут hc:scaMatrix с hc:rotMatrix
				if (L"hc:transMatrix" == oGrandChild.GetName())
					m_arMatrixs.push_back(ReadMatrix(oGrandChild));
				else if (L"hc:scaMatrix" == oGrandChild.GetName())
					m_arMatrixs.push_back(ReadMatrix(oGrandChild));
				else if (L"hc:rotMatrix" == oGrandChild.GetName())
					m_arMatrixs.push_back(ReadMatrix(oGrandChild));
			}
		}
	}
}

int CCtrlObjElement::GetCurWidth() const
{
	return m_nCurWidth;
}

int CCtrlObjElement::GetCurHeight() const
{
	return m_nCurHeight;
}

int CCtrlObjElement::GetOrgWidth() const
{
	return m_nOrgWidth;
}

int CCtrlObjElement::GetOrgHeight() const
{
	return m_nOrgHeight;
}

int CCtrlObjElement::GetFinalWidth() const
{
	if (0 != m_nCurWidth)
		return m_nCurWidth;

	return CCtrlCommon::GetWidth();
}

int CCtrlObjElement::GetFinalHeight() const
{
	if (0 != m_nCurHeight)
		return m_nCurHeight;

	return CCtrlCommon::GetHeight();
}

TMatrix CCtrlObjElement::GetFinalMatrix() const
{
	if (m_arMatrixs.empty() || 0 == m_shNGrp)
		return TMatrix();

	TMatrix oMatrix{m_arMatrixs.front()};

	for (unsigned int unIndex = 1; unIndex < m_arMatrixs.size(); ++ unIndex)
		oMatrix.Multiply(m_arMatrixs[unIndex]);

	return oMatrix;
}

bool CCtrlObjElement::HorzFlip() const
{
	return m_bHorzFlip;
}

bool CCtrlObjElement::VertFlip() const
{
	return m_bVerFlip;
}

int CCtrlObjElement::ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CCtrlCommon::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadInt(oObj.m_nXGrpOffset);
	oBuffer.ReadInt(oObj.m_nYGrpOffset);
	oBuffer.ReadShort(oObj.m_shNGrp);
	oBuffer.ReadShort(oObj.m_shVer);
	oBuffer.ReadInt(oObj.m_nOrgWidth);
	oBuffer.ReadInt(oObj.m_nOrgHeight);
	oBuffer.ReadInt(oObj.m_nCurWidth);
	oBuffer.ReadInt(oObj.m_nCurHeight);
	oObj.m_bHorzFlip = CHECK_FLAG(oBuffer[0], 0x01);
	oObj.m_bVerFlip = CHECK_FLAG(oBuffer[0], 0x02);
	oBuffer.Skip(4);
	oBuffer.ReadShort(oObj.m_shRotat);
	oBuffer.ReadInt(oObj.m_nXCenter);
	oBuffer.ReadInt(oObj.m_nYCenter);
	oBuffer.ReadShort(oObj.m_shMatCnt);

	oObj.m_arMatrixs.push_back(ReadMatrix(oBuffer));

	for (unsigned int unIndex = 0; unIndex < ((int)oObj.m_shMatCnt) * 2; ++ unIndex)
		oObj.m_arMatrixs.push_back(ReadMatrix(oBuffer));

	return oBuffer.GetDistanceToLastPos(true);
}

TMatrix::TMatrix()
	: m_dM11(1.), m_dM12(0.), m_dM21(0.), m_dM22(1.), m_dDX(0.), m_dDY(0.)
{}

TMatrix::TMatrix(double dM11, double dM12, double dM21, double dM22, double dDX, double dDY)
    : m_dM11(dM11), m_dM12(dM12), m_dM21(dM21), m_dM22(dM22), m_dDX(dDX), m_dDY(dDY)
{}

void TMatrix::Multiply(const TMatrix& oMatrix)
{
	const double dM11 = m_dM11 * oMatrix.m_dM11 + m_dM12 * oMatrix.m_dM21;
	const double dM12 = m_dM11 * oMatrix.m_dM12 + m_dM12 * oMatrix.m_dM22;
	const double dM21 = m_dM21 * oMatrix.m_dM11 + m_dM22 * oMatrix.m_dM21;
	const double dM22 = m_dM21 * oMatrix.m_dM12 + m_dM22 * oMatrix.m_dM22;

	const double dDx = m_dDX * oMatrix.m_dM11 + m_dDY * oMatrix.m_dM21 + oMatrix.m_dDX;
	const double dDy = m_dDX * oMatrix.m_dM12 + m_dDY * oMatrix.m_dM22 + oMatrix.m_dDY;

	m_dM11 = dM11;
	m_dM12 = dM12;
	m_dM21 = dM21;
	m_dM22 = dM22;
	m_dDX  = dDx;
	m_dDY  = dDy;
}

void TMatrix::ApplyToPoint(double& dX, double& dY)
{
	const double _dX = dX;
	const double _dY = dY;

	dX = _dX * m_dM11 + _dY * m_dM21 + m_dDX;
	dY = _dX * m_dM12 + _dY * m_dM22 + m_dDY;
}

void TMatrix::ApplyToSize(double& dW, double& dH)
{
	const double _dW = dW;
	const double _dH = dH;

	dW = _dW * m_dM11 + _dH * m_dM21;
	dH = _dW * m_dM12 + _dH * m_dM22;
}
}

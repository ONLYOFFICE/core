#include "CtrlObjElement.h"

#include "../Common/NodeNames.h"

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

TMatrix ReadMatrix(CXMLReader& oReader, EHanType eType)
{
	TMatrix oMatrix;

	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::MatrixElement1_1, eType) == sAttributeName)
			oMatrix.m_dM11 = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::MatrixElement1_2, eType) == sAttributeName)
			oMatrix.m_dM12 = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::MatrixElementOffsetX, eType) == sAttributeName)
			oMatrix.m_dDX = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::MatrixElement2_1, eType) == sAttributeName)
			oMatrix.m_dM21 = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::MatrixElement2_2, eType) == sAttributeName)
			oMatrix.m_dM22 = oReader.GetDouble();
		else if (GetAttributeName(EAttribute::MatrixElementOffsetY, eType) == sAttributeName)
			oMatrix.m_dDY = oReader.GetDouble();
	}
	END_READ_ATTRIBUTES(oReader)

	return oMatrix;
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlCommon(sCtrlID, oReader, eType), m_nCurWidth(0), m_nCurHeight(0)
{
	if (EHanType::HWPX == eType)
		m_shNGrp = oReader.GetAttributeInt("groupLevel");
}

void CCtrlObjElement::ParseChildren(CXMLReader& oReader, EHanType eType)
{
	const std::string sNodeName{oReader.GetName()};

	if (EHanType::HWPML == eType && "SHAPECOMPONENT" == sNodeName)
		ParseHWPMLElement(oReader);
	else if (EHanType::HWPX == eType)
		ParseHWPXChildren(oReader);
	else
		CCtrlCommon::ParseChildren(oReader, eType);
}

void CCtrlObjElement::ParseRotationInfo(CXMLReader &oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Angle, eType) == sAttributeName)
			m_shRotat = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CenterX, eType) == sAttributeName)
			m_nXCenter = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CenterY, eType) == sAttributeName)
			m_nYCenter = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}

void CCtrlObjElement::ParseRenderingInfo(CXMLReader &oReader, EHanType eType)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		// Сначала идёт 1 hc:transMatrix, а после попарно идут hc:scaMatrix с hc:rotMatrix

		if (GetNodeName(ENode::TransformMatrix, eType) != sNodeName &&
		    GetNodeName(ENode::ScaleMatrix, eType)     != sNodeName &&
		    GetNodeName(ENode::RotationMatrix, eType)  != sNodeName)
			continue;

		m_arMatrixs.push_back(ReadMatrix(oReader, eType));
	}
	END_WHILE
}

void CCtrlObjElement::ParseHWPXChildren(CXMLReader& oReader)
{
	const std::string sNodeName{oReader.GetName()};

	if ("hp:offset" == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("x" == sAttributeName)
				m_nXGrpOffset = oReader.GetInt();
			else if ("y" == sAttributeName)
				m_nYGrpOffset = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}

	else if ("hp:orgSz" == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("width" == sAttributeName)
				m_nOrgWidth = oReader.GetInt();
			else if ("height" == sAttributeName)
				m_nOrgHeight = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	else if ("hp:curSz" == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("width" == sAttributeName)
				m_nCurWidth = oReader.GetInt();
			else if ("height" == sAttributeName)
				m_nCurHeight = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	else if ("hp:flip" == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("horizontal" == sAttributeName)
				m_bHorzFlip = oReader.GetBool();
			else if ("vertical" == sAttributeName)
				m_bVerFlip = oReader.GetBool();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	else if ("hp:rotationInfo" == sNodeName)
		ParseRotationInfo(oReader, EHanType::HWPX);
	else if ("hp:renderingInfo" == sNodeName)
		ParseRenderingInfo(oReader, EHanType::HWPX);
	else
		CCtrlCommon::ParseChildren(oReader, EHanType::HWPX);
}

void CCtrlObjElement::ParseHWPMLElement(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("XPos" == sAttributeName)
			m_nXGrpOffset = oReader.GetInt();
		else if ("YPos" == sAttributeName)
			m_nYGrpOffset = oReader.GetInt();
		else if ("GroupLevel" == sAttributeName)
			m_shNGrp = oReader.GetInt();
		else if ("OriWidth" == sAttributeName)
			m_nOrgWidth = oReader.GetInt();
		else if ("OriHeight" == sAttributeName)
			m_nOrgHeight = oReader.GetInt();
		else if ("CurWidth" == sAttributeName)
			m_nCurWidth = oReader.GetInt();
		else if ("CurHeight" == sAttributeName)
			m_nCurHeight = oReader.GetInt();
		else if ("HorzFlip" == sAttributeName)
			m_bHorzFlip = oReader.GetBool();
		else if ("VertFlip" == sAttributeName)
			m_bVerFlip = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("ROTATIONINFO" == sNodeName)
			ParseRotationInfo(oReader, EHanType::HWPML);
		else if ("RENDERINGINFO" == sNodeName)
			ParseRenderingInfo(oReader, EHanType::HWPML);
	}
	END_WHILE
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

	if (0 != CCtrlCommon::GetWidth())
		return CCtrlCommon::GetWidth();

	return m_nOrgWidth;
}

int CCtrlObjElement::GetFinalHeight() const
{
	if (0 != m_nCurHeight)
		return m_nCurHeight;

	if (0 != CCtrlCommon::GetHeight())
		return CCtrlCommon::GetHeight();

	return m_nOrgHeight;
}

short CCtrlObjElement::GetGroupLevel() const
{
	return m_shNGrp;
}

TMatrix CCtrlObjElement::GetFinalMatrix() const
{
	if (m_arMatrixs.empty())
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

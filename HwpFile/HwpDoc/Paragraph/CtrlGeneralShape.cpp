#include "CtrlGeneralShape.h"

#include "CtrlShapePic.h"
#include "CtrlShapeRect.h"
#include "CtrlShapeLine.h"
#include "CtrlContainer.h"
#include "CtrlShapeEllipse.h"
#include "CtrlShapePolygon.h"
#include "CtrlShapeArc.h"
#include "CtrlShapeCurve.h"
#include "CtrlEqEdit.h"
#include "CtrlShapeOle.h"
#include "CtrlShapeVideo.h"
#include "CtrlShapeTextArt.h"
#include "CtrlCharacter.h"

namespace HWP
{
CCtrlGeneralShape::CCtrlGeneralShape()
{
	InitData();
}

CCtrlGeneralShape::CCtrlGeneralShape(const HWP_STRING& sCtrlID)
	: CCtrlObjElement(sCtrlID)
{
	InitData();
}

CCtrlGeneralShape::CCtrlGeneralShape(const CCtrlGeneralShape& oGeneralShape)
	: CCtrlObjElement(oGeneralShape)
{
	InitData();

	m_pParent = oGeneralShape.m_pParent;

	m_nLineColor = oGeneralShape.m_nLineColor;
	m_nLineThick = oGeneralShape.m_nLineThick;
	m_eLineHead = oGeneralShape.m_eLineHead;
	m_eLineTail = oGeneralShape.m_eLineTail;
	m_eLineHeadSz = oGeneralShape.m_eLineHeadSz;
	m_eLineTailSz = oGeneralShape.m_eLineTailSz;
	m_eLineStyle = oGeneralShape.m_eLineStyle;
	m_chOutline = oGeneralShape.m_chOutline;

	m_nFillType = oGeneralShape.m_nFillType;
	m_pFill = oGeneralShape.m_pFill;

	if (nullptr != m_pFill)
		m_pFill->AddRef();

	m_shLeftSpace = oGeneralShape.m_shLeftSpace;
	m_shRightSpace = oGeneralShape.m_shRightSpace;
	m_shTopSpace = oGeneralShape.m_shTopSpace;
	m_shBottomSpace = oGeneralShape.m_shBottomSpace;
	m_nMaxTxtWidth = oGeneralShape.m_nMaxTxtWidth;
}

CCtrlGeneralShape::CCtrlGeneralShape(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlObjElement(sCtrlID, nSize, oBuffer, nOff, nVersion)
{
	InitData();
}

CCtrlGeneralShape::CCtrlGeneralShape(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlObjElement(sCtrlID)
{
	InitData();

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:lineShape" == oChild.GetName())
		{
			m_nLineColor = oChild.GetAttributeColor(L"color");
			m_nLineThick = std::abs(oChild.GetAttributeInt(L"width"));
			m_eLineStyle = GetLineStyle2(oChild.GetAttributeInt(L"style"));

			HWP_STRING sType = oChild.GetAttribute(L"headStyle");

			if (L"ARROW" == sType)
				m_eLineHead = ELineArrowStyle::ARROW;
			else if (L"SPEAR" == sType)
				m_eLineHead = ELineArrowStyle::SPEAR;
			else if (L"CONCAVE_ARROW" == sType)
				m_eLineHead = ELineArrowStyle::CONCAVE_ARROW;
			else if (L"EMPTY_DIAMOND" == sType)
				m_eLineHead = (oChild.GetAttributeBool(L"headfill")) ? ELineArrowStyle::DIAMOND : ELineArrowStyle::EMPTY_DIAMOND;
			else if (L"EMPTY_CIRCLE" == sType)
				m_eLineHead = (oChild.GetAttributeBool(L"headfill")) ? ELineArrowStyle::CIRCLE : ELineArrowStyle::EMPTY_CIRCLE;
			else if (L"EMPTY_BOX" == sType)
				m_eLineHead = (oChild.GetAttributeBool(L"headfill")) ? ELineArrowStyle::BOX : ELineArrowStyle::EMPTY_BOX;
			else
				m_eLineHead = ELineArrowStyle::NORMAL;

			sType = oChild.GetAttribute(L"headSz");

			if (L"SMALL_SMALL" == sType)
				m_eLineHeadSz = ELineArrowSize::SMALL_SMALL;
			else if (L"SMALL_MEDIUM" == sType)
				m_eLineHeadSz = ELineArrowSize::SMALL_MEDIUM;
			else if (L"SMALL_LARGE" == sType)
				m_eLineHeadSz = ELineArrowSize::SMALL_LARGE;
			else if (L"MEDIUM_SMALL" == sType)
				m_eLineHeadSz = ELineArrowSize::MEDIUM_SMALL;
			else if (L"MEDIUM_MEDIUM" == sType)
				m_eLineHeadSz = ELineArrowSize::MEDIUM_MEDIUM;
			else if (L"MEDIUM_LARGE" == sType)
				m_eLineHeadSz = ELineArrowSize::MEDIUM_LARGE;
			else if (L"LARGE_SMALL" == sType)
				m_eLineHeadSz = ELineArrowSize::LARGE_SMALL;
			else if (L"LARGE_MEDIUM" == sType)
				m_eLineHeadSz = ELineArrowSize::LARGE_MEDIUM;
			else if (L"LARGE_LARGE" == sType)
				m_eLineHeadSz = ELineArrowSize::LARGE_LARGE;
			else
				m_eLineHeadSz = ELineArrowSize::MEDIUM_MEDIUM;

			sType = oChild.GetAttribute(L"tailStyle");

			if (L"ARROW" == sType)
				m_eLineTail = ELineArrowStyle::ARROW;
			else if (L"SPEAR" == sType)
				m_eLineTail = ELineArrowStyle::SPEAR;
			else if (L"CONCAVE_ARROW" == sType)
				m_eLineTail = ELineArrowStyle::CONCAVE_ARROW;
			else if (L"EMPTY_DIAMOND" == sType)
				m_eLineTail = (oChild.GetAttributeBool(L"tailfill")) ? ELineArrowStyle::DIAMOND : ELineArrowStyle::EMPTY_DIAMOND;
			else if (L"EMPTY_CIRCLE" == sType)
				m_eLineTail = (oChild.GetAttributeBool(L"tailfill")) ? ELineArrowStyle::CIRCLE : ELineArrowStyle::EMPTY_CIRCLE;
			else if (L"EMPTY_BOX" == sType)
				m_eLineTail = (oChild.GetAttributeBool(L"tailfill")) ? ELineArrowStyle::BOX : ELineArrowStyle::EMPTY_BOX;
			else
				m_eLineTail = ELineArrowStyle::NORMAL;

			sType = oChild.GetAttribute(L"tailSz");

			if (L"SMALL_SMALL" == sType)
				m_eLineTailSz = ELineArrowSize::SMALL_SMALL;
			else if (L"SMALL_MEDIUM" == sType)
				m_eLineTailSz = ELineArrowSize::SMALL_MEDIUM;
			else if (L"SMALL_LARGE" == sType)
				m_eLineTailSz = ELineArrowSize::SMALL_LARGE;
			else if (L"MEDIUM_SMALL" == sType)
				m_eLineTailSz = ELineArrowSize::MEDIUM_SMALL;
			else if (L"MEDIUM_MEDIUM" == sType)
				m_eLineTailSz = ELineArrowSize::MEDIUM_MEDIUM;
			else if (L"MEDIUM_LARGE" == sType)
				m_eLineTailSz = ELineArrowSize::MEDIUM_LARGE;
			else if (L"LARGE_SMALL" == sType)
				m_eLineTailSz = ELineArrowSize::LARGE_SMALL;
			else if (L"LARGE_MEDIUM" == sType)
				m_eLineTailSz = ELineArrowSize::LARGE_MEDIUM;
			else if (L"LARGE_LARGE" == sType)
				m_eLineTailSz = ELineArrowSize::LARGE_LARGE;
			else
				m_eLineTailSz = ELineArrowSize::MEDIUM_MEDIUM;
		}
		else if (L"hc:fillBrush" ==  oChild.GetName())
			m_pFill = new CFill(oChild);
		else if (L"hp:drawText" == oChild.GetName())
		{
			m_nMaxTxtWidth = oChild.GetAttributeInt(L"lastWidth");

			for (CXMLNode& oGrandChild : oChild.GetChilds())
			{
				if (L"hp:textMargin" == oGrandChild.GetName())
				{
					m_shLeftSpace = oGrandChild.GetAttributeInt(L"left");
					m_shRightSpace = oGrandChild.GetAttributeInt(L"right");
					m_shTopSpace = oGrandChild.GetAttributeInt(L"top");
					m_shBottomSpace = oGrandChild.GetAttributeInt(L"bottom");
				}
				else if (L"hp:subList" == oGrandChild.GetName())
				{
					ReadSubList(oGrandChild, nVersion);
				}
			}
		}
	}
}

CCtrlGeneralShape::~CCtrlGeneralShape()
{
	if (nullptr != m_pFill && 0 == m_pFill->Release())
		m_pFill = nullptr;
}

ECtrlObjectType CCtrlGeneralShape::GetCtrlType() const
{
	return ECtrlObjectType::Shape;
}

EShapeType CCtrlGeneralShape::GetShapeType() const
{
	return EShapeType::GeneralShape;
}

void CCtrlGeneralShape::InitData()
{
	m_eLineStyle = ELineStyle2::NONE;
	m_pFill = nullptr;
}

void CCtrlGeneralShape::ReadSubList(CXMLNode& oNode, int nVersion)
{
	m_eTextVerAlign = GetVertAlign(oNode.ReadAttributeInt(L"vertAlign"));

	std::vector<CXMLNode> arChilds{oNode.GetChilds()};

	for (unsigned int unIndex = 0; unIndex < arChilds.size(); ++unIndex)
	{
		CHWPPargraph* pLatestParagraph = nullptr;

		if (L"hp:p" == arChilds[unIndex].GetName())
		{
			CHWPPargraph* pParagraph = new CHWPPargraph(arChilds[unIndex], nVersion);

			if (nullptr == pParagraph)
				continue;

			m_arParas.push_back(pParagraph);
			pLatestParagraph = pParagraph;
		}

		if (nullptr != pLatestParagraph && 0 != pLatestParagraph->GetCountCtrls() && ECtrlObjectType::ParaText == pLatestParagraph->GetCtrls().back()->GetCtrlType() && unIndex < arChilds.size() - 1)
			pLatestParagraph->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
	}
}

void CCtrlGeneralShape::SetParent(CHWPPargraph* pParent)
{
	m_pParent = pParent;
}

CHWPPargraph* CCtrlGeneralShape::GetParent()
{
	return m_pParent;
}

const CFill* CCtrlGeneralShape::GetFill() const
{
	return m_pFill;
}

ELineStyle2 CCtrlGeneralShape::GetLineStyle() const
{
	return m_eLineStyle;
}

int CCtrlGeneralShape::GetLineColor() const
{
	return m_nLineColor;
}

int CCtrlGeneralShape::GetLineThick() const
{
	return m_nLineThick;
}

CCtrlGeneralShape* CCtrlGeneralShape::Parse(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	HWP_STRING sCtrlId;
	oBuffer.ReadString(sCtrlId, 4, EStringCharacter::ASCII);

	CCtrlGeneralShape *pShape = nullptr;

	#define CREATE_AND_PARSE_SHAPE(type)\
	pShape = new type(oObj); \
	\
	if (nullptr != pShape) \
		type::ParseCtrl((type&)(*pShape), nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion); \
	else \
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos())

	if (L"cip$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapePic);
	}
	else if (L"cer$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeRect);
	}
	else if (L"nil$" == sCtrlId ||
	         L"loc$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeLine);
	}
	else if (L"noc$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlContainer);
	}
	else if (L"lle$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeEllipse);
	}
	else if (L"lop$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapePolygon);
	}
	else if (L"cra$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeArc);
	}
	else if (L"ruc$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeCurve);
	}
	else if (L"deqe" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlEqEdit);
	}
	else if (L"elo$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeOle);
	}
	else if (L"div$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeVideo);
	}
	else if (L"tat$" == sCtrlId)
	{
		CREATE_AND_PARSE_SHAPE(CCtrlShapeTextArt);
	}

	if (nullptr != pShape)
		pShape->SetID(sCtrlId);

	oBuffer.RemoveLastSavedPos();

	return pShape;
}

int CCtrlGeneralShape::ParseListHeaderApend(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	if (nSize >= 16)
	{
		oBuffer.Skip(2);
		oBuffer.ReadInt(oObj.m_nCaptionAttr);
		oBuffer.ReadInt(oObj.m_nCaptionWidth);
		oObj.m_nCaptionSpacing = oBuffer.ReadShort();
		oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	}

	if ((nSize - oBuffer.GetDistanceToLastPos()) == 8)
		oBuffer.Skip(8);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlGeneralShape::ParseCtrl(CCtrlGeneralShape& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadColor(oObj.m_nLineColor);
	oObj.m_nLineThick = oBuffer.ReadShort();

	oBuffer.Skip(2);

	int nLineAttr;
	oBuffer.ReadInt(nLineAttr);

	oObj.m_eLineStyle = GetLineStyle2(nLineAttr & 0x3F);
	oObj.m_eLineHead = GetLineArrowStyle(((nLineAttr >> 10) & 0x3F), CHECK_FLAG(nLineAttr >> 30, 0x1));
	oObj.m_eLineHeadSz = GetLineArrowSize((nLineAttr >> 22) & 0x0F);
	oObj.m_eLineTail = GetLineArrowStyle(((nLineAttr >> 16) & 0x3F), CHECK_FLAG(nLineAttr >> 31, 0x1));
	oObj.m_eLineTailSz = GetLineArrowSize((nLineAttr >> 26) & 0x0F);
	oBuffer.ReadByte(oObj.m_chOutline);

	oObj.m_pFill = new CFill(oBuffer, 0, nSize - oBuffer.GetDistanceToLastPos() - 22);

	oBuffer.Skip(22);

	return oBuffer.GetDistanceToLastPos(true);
}
}

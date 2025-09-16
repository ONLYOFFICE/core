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

#include "../Common/NodeNames.h"

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

CCtrlGeneralShape::CCtrlGeneralShape(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlObjElement(sCtrlID, oReader, nVersion, eType)
{
	InitData();
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

ELineArrowSize CheckLineSize(const std::string& sLineSzType, EHanType eType)
{
	if (GetValueName(EValue::SmallSmall, eType) == sLineSzType)
		return ELineArrowSize::SMALL_SMALL;
	else if (GetValueName(EValue::SmallMedium, eType) == sLineSzType)
		return ELineArrowSize::SMALL_MEDIUM;
	else if (GetValueName(EValue::SmallLarge, eType) == sLineSzType)
		return ELineArrowSize::SMALL_LARGE;
	else if (GetValueName(EValue::MediumSmall, eType) == sLineSzType)
		return ELineArrowSize::MEDIUM_SMALL;
	else if (GetValueName(EValue::MediumMedium, eType) == sLineSzType)
		return ELineArrowSize::MEDIUM_MEDIUM;
	else if (GetValueName(EValue::MediumLarge, eType) == sLineSzType)
		return ELineArrowSize::MEDIUM_LARGE;
	else if (GetValueName(EValue::LargeSmall, eType) == sLineSzType)
		return ELineArrowSize::LARGE_SMALL;
	else if (GetValueName(EValue::LargeMedium, eType) == sLineSzType)
		return ELineArrowSize::LARGE_MEDIUM;
	else if (GetValueName(EValue::LargeLarge, eType) == sLineSzType)
		return ELineArrowSize::LARGE_LARGE;

	return ELineArrowSize::MEDIUM_MEDIUM;
}

ELineArrowStyle CheckLineArrowStyle(const std::string& sLineArrowStyle, bool bHeadFill, EHanType eType)
{
	if (GetValueName(EValue::Arrow, eType) == sLineArrowStyle)
		return ELineArrowStyle::ARROW;
	else if (GetValueName(EValue::Spear, eType) == sLineArrowStyle)
		return ELineArrowStyle::SPEAR;
	else if (GetValueName(EValue::ConcaveArrow, eType) == sLineArrowStyle)
		return ELineArrowStyle::CONCAVE_ARROW;
	else if (GetValueName(EValue::EmptyDiamond, eType) == sLineArrowStyle)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::DIAMOND : ELineArrowStyle::EMPTY_DIAMOND;

		return ELineArrowStyle::EMPTY_DIAMOND;
	}
	else if (GetValueName(EValue::EmptyCircle, eType) == sLineArrowStyle)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::CIRCLE : ELineArrowStyle::EMPTY_CIRCLE;

		return ELineArrowStyle::EMPTY_CIRCLE;
	}
	else if (GetValueName(EValue::EmptyBox, eType) == sLineArrowStyle)
	{
		if (EHanType::HWPX == eType)
			return bHeadFill ? ELineArrowStyle::BOX : ELineArrowStyle::EMPTY_BOX;
		
		return ELineArrowStyle::EMPTY_BOX;
	}

	return ELineArrowStyle::NORMAL;
}

void CCtrlGeneralShape::ParseChildren(CXMLReader& oReader, int nVersion, EHanType eType)
{
	bool bHeadFill = false, bTailFill = false;

	const std::string sNodeName{oReader.GetName()};

	if (GetNodeName(ENode::LineShape, eType) == sNodeName)
	{
		std::string sHeadStyle, sTailStyle;

		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
				m_nLineColor = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
				m_nLineThick = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Style, eType) == sAttributeName)
				m_eLineStyle = GetLineStyle2(oReader.GetText());
			else if (GetAttributeName(EAttribute::HeadStyle, eType) == sAttributeName)
				sHeadStyle = oReader.GetTextA();
			else if (EHanType::HWPX == eType && "headfill" == sAttributeName)
				bHeadFill = oReader.GetBool();
			else if (GetAttributeName(EAttribute::HeadSize, eType) == sAttributeName)
				m_eLineHeadSz = CheckLineSize(oReader.GetTextA(), eType);
			else if (GetAttributeName(EAttribute::TailStyle, eType) == sAttributeName)
				sTailStyle = oReader.GetTextA();
			else if (EHanType::HWPX == eType && "tailfill" == sAttributeName)
				bTailFill = oReader.GetBool();
			else if (GetAttributeName(EAttribute::TailSize, eType) == sAttributeName)
				m_eLineTailSz = CheckLineSize(oReader.GetTextA(), eType);
		}
		END_READ_ATTRIBUTES(oReader)

		m_eLineHead = CheckLineArrowStyle(sHeadStyle, bHeadFill, eType);
		m_eLineTail = CheckLineArrowStyle(sTailStyle, bTailFill, eType);
	}
	else if ("hc:fillBrush" == sNodeName)
		m_pFill = new CFill(oReader, eType);
	else if ("hp:drawText" == sNodeName)
	{
		m_nMaxTxtWidth = oReader.GetAttributeInt("lastWidth");

		WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Child)
		{
			if ("hp:textMargin" == sNodeChildName)
			{
				START_READ_ATTRIBUTES(oReader)
				{
					if ("left" == sAttributeName)
						m_shLeftSpace = oReader.GetInt();
					else if ("right" == sAttributeName)
						m_shRightSpace =  oReader.GetInt();
					else if ("top" == sAttributeName)
						m_shTopSpace =  oReader.GetInt();
					else if ("bottom" == sAttributeName)
						m_shBottomSpace =  oReader.GetInt();
				}
				END_READ_ATTRIBUTES(oReader)
			}
			else if ("hp:subList" == sNodeChildName)
				ReadSubList(oReader, nVersion);
		}
		END_WHILE
	}
	else
		CCtrlObjElement::ParseChildren(oReader, nVersion, eType);
}

void CCtrlGeneralShape::InitData()
{
	m_eLineStyle = ELineStyle2::NONE;
	m_pFill = nullptr;
}

void CCtrlGeneralShape::ReadSubList(CXMLReader& oReader, int nVersion)
{
	m_eTextVerAlign = GetVertAlign(oReader.GetAttributeInt("vertAlign"));

	// CHWPPargraph* pLatestParagraph = nullptr;

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:p")
	{
		CHWPPargraph* pParagraph = new CHWPPargraph(oReader, nVersion, EHanType::HWPX);

		if (nullptr == pParagraph)
			continue;

		m_arParas.push_back(pParagraph);
		// pLatestParagraph = pParagraph;

		// if (nullptr != pLatestParagraph && 0 != pLatestParagraph->GetCountCtrls() && ECtrlObjectType::ParaText == pLatestParagraph->GetCtrls().back()->GetCtrlType())
		// 	pLatestParagraph->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
	}
	END_WHILE
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

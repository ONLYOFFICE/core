#include "HWPPargraph.h"
#include <algorithm>

#include "CtrlCharacter.h"
#include "CtrlSectionDef.h"
#include "CtrlContainer.h"
#include "CtrlShapePic.h"
#include "CtrlTable.h"
#include "CtrlEqEdit.h"
#include "CtrlShapeArc.h"
#include "CtrlShapeConnectLine.h"
#include "CtrlShapeCurve.h"
#include "CtrlShapeEllipse.h"
#include "CtrlShapeLine.h"
#include "CtrlShapeOle.h"
#include "CtrlShapePolygon.h"
#include "CtrlShapeRect.h"
#include "CtrlShapeTextArt.h"
#include "CtrlShapeVideo.h"
#include "ParaText.h"

#include "../Common/NodeNames.h"

#include "../../../DesktopEditor/common/File.h"

namespace HWP
{
CHWPPargraph::CHWPPargraph()
	: m_shParaShapeID(0), m_shParaStyleID(0), m_chBreakType(0), m_pLineSegs(nullptr)
{}

CHWPPargraph::CHWPPargraph(CXMLReader& oReader, EHanType eType)
	: m_shParaShapeID(0), m_shParaStyleID(0), m_chBreakType(0), m_pLineSegs(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::ParaShape, eType) == sAttributeName)
			m_shParaShapeID = oReader.GetInt();
		else if (GetAttributeName(EAttribute::StyleId, eType) == sAttributeName)
			m_shParaStyleID = oReader.GetInt();
		else if (GetAttributeName(EAttribute::PageBreak, eType) == sAttributeName)
		{
			if (oReader.GetBool())
				m_chBreakType |= 0b00000100;
			else
				m_chBreakType &= 0b11111011;
		}
		else if (Equals(EAttribute::ColumnBreak, eType, sAttributeName))
		{
			if (oReader.GetBool())
				m_chBreakType |= 0b00001000;
			else
				m_chBreakType &= 0b11110111;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	int nCharShapeID = 0;

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::Text, eType) == sNodeName)
		{
			nCharShapeID = oReader.GetAttributeInt(GetAttributeName(EAttribute::CharShape, eType));

			WHILE_READ_NEXT_NODE_WITH_DEPTH(oReader, Child)
				ParseHWPParagraph(oReader, nCharShapeID, eType);
			END_WHILE
		}
		else if (GetNodeName(ENode::LinesegArray, eType) == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, GetNodeName(ENode::Lineseg, eType))
			{
				m_pLineSegs = new CLineSeg(oReader);
				break;
			}
			END_WHILE
		}
	}
	END_WHILE

	if (m_arP.empty() || ECtrlObjectType::Character != m_arP.back()->GetCtrlType())
		m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK, nCharShapeID));
}

CHWPPargraph::~CHWPPargraph()
{
	if (nullptr != m_pLineSegs)
		delete m_pLineSegs;
}

bool CHWPPargraph::ParseHWPParagraph(CXMLReader& oReader, int nCharShapeID, EHanType eType)
{
	const size_t unCurrentParaCount = m_arP.size();

	const std::string sNodeName{oReader.GetName()};

	if (GetNodeName(ENode::SectionDef, eType) == sNodeName)
		m_arP.push_back(new CCtrlSectionDef(L"dces", oReader));
	else if (GetNodeName(ENode::Char, eType) == sNodeName)
	{
		if (oReader.IsEmptyNode())
			return false;

		const int nDepth = oReader.GetDepth();
		XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
		while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
		{
			if (eNodeType == XmlUtils::XmlNodeType_Text ||
			    eNodeType == XmlUtils::XmlNodeType_Whitespace ||
			    eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE ||
			    eNodeType == XmlUtils::XmlNodeType_CDATA)
			{
				const char* pValue = oReader.GetTextChar();
				std::wstring wsValue;

				if('\0' != pValue[0])
				{
					NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen(pValue), wsValue);
					m_arP.push_back(new CParaText(L"____", wsValue, 0, nCharShapeID));
				}
			}
			else if (eNodeType == XmlUtils::XmlNodeType_Element)
			{
				const std::string sChildNodeName{oReader.GetName()};

				if (GetNodeName(ENode::LineBreak, eType) == sChildNodeName)
					m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
				else if (GetNodeName(ENode::Tab, eType) == sChildNodeName)
					m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::TABULATION));
				else if (GetNodeName(ENode::Hyphen, eType) == sChildNodeName)
					m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
				else if (GetNodeName(ENode::NbSpace, eType) == sChildNodeName ||
				         GetNodeName(ENode::FwSpace, eType) == sChildNodeName)
					m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
			}
		}
	}
	else if (GetNodeName(ENode::Table, eType) == sNodeName)
		m_arP.push_back(new CCtrlTable(L" lbt", oReader, eType));
	else if (GetNodeName(ENode::Picture, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapePic(L"cip$", oReader, eType));
	else if (GetNodeName(ENode::Container, eType) == sNodeName)
		m_arP.push_back(new CCtrlContainer(L"noc$", oReader, eType));
	else if (GetNodeName(ENode::Ole, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeOle(L"elo$", oReader, eType));
	else if (GetNodeName(ENode::Equation, eType) == sNodeName)
		m_arP.push_back(new CCtrlEqEdit(L"deqe", oReader, eType));
	else if (GetNodeName(ENode::Line, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeLine(L"nil$", oReader, eType));
	else if (GetNodeName(ENode::Rectangle, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeRect(L"cer$", oReader, eType));
	else if (GetNodeName(ENode::Ellipse, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeEllipse(L"lle$", oReader, eType));
	else if (GetNodeName(ENode::Arc, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeArc(L"cra$", oReader, eType));
	else if (GetNodeName(ENode::Polygon, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapePolygon(L"lop$", oReader, eType));
	else if (GetNodeName(ENode::Curve, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeCurve(L"ruc$", oReader, eType));
	else if (GetNodeName(ENode::ConnectLine, eType) == sNodeName)
		m_arP.push_back(new CCtrlShapeConnectLine(L"loc$", oReader, eType));
	else if (GetNodeName(ENode::TextArt, eType) == sNodeName)
			m_arP.push_back(new CCtrlShapeTextArt(L"tat$", oReader, eType));
	else if (EHanType::HWPX == eType)
	{
		
		if (GetNodeName(ENode::Video, eType) == sNodeName)
			m_arP.push_back(new CCtrlShapeVideo(L"div$", oReader, eType));
		else if ("hp:ctrl" == sNodeName)
		{
			WHILE_READ_NEXT_NODE(oReader)
				AddCtrl(CCtrl::GetCtrl(oReader, EHanType::HWPX));
			END_WHILE
		}
	}
	else if (EHanType::HWPML == eType)
		AddCtrl(CCtrl::GetCtrl(oReader, EHanType::HWPML));

	if (unCurrentParaCount != m_arP.size())
		return true;

	if (GetNodeName(ENode::Switch, eType) == sNodeName)
	{
		WHILE_READ_NEXT_NODE(oReader)
		{
			if (GetNodeName(ENode::Case, eType) != oReader.GetName() &&
			    GetNodeName(ENode::Default, eType) != oReader.GetName())
				continue;

			WHILE_READ_NEXT_NODE(oReader)
				if (ParseHWPParagraph(oReader, nCharShapeID, eType))
					return true;
			END_WHILE
		}
		END_WHILE
	}

	return false;
}

EParagraphType CHWPPargraph::GetType() const
{
	return EParagraphType::Normal;
}

void CHWPPargraph::SetLineSeg(CLineSeg* pLineSeg)
{
	if (nullptr != m_pLineSegs)
		delete m_pLineSegs;

	m_pLineSegs = pLineSeg;
}

void CHWPPargraph::AddRangeTag(const TRangeTag& oRangeTag)
{
	m_arRangeTags.push_back(oRangeTag);
}

void CHWPPargraph::AddCtrl(CCtrl* pCtrl)
{
	if (nullptr != pCtrl)
		m_arP.push_back(pCtrl);
}

void CHWPPargraph::AddCtrls(const LIST<CCtrl*>& arCtrls)
{
	m_arP.insert(m_arP.end(), arCtrls.begin(), arCtrls.end());
}

bool CHWPPargraph::SetCtrl(CCtrl* pCtrl, unsigned int unIndex)
{
	if (unIndex >= m_arP.size() || nullptr == pCtrl)
		return false;

	CCtrl* pOldCtrl = m_arP[unIndex];

	if (nullptr != pOldCtrl)
		delete pOldCtrl;

	m_arP[unIndex] = pCtrl;

	return true;
}

VECTOR<CCtrl*>& CHWPPargraph::GetCtrls()
{
	return m_arP;
}

std::vector<const CCtrl*> CHWPPargraph::GetCtrls() const
{
	RETURN_VECTOR_CONST_PTR(CCtrl, m_arP);
}

unsigned int CHWPPargraph::GetCountCtrls() const
{
	return m_arP.size();
}

short CHWPPargraph::GetShapeID() const
{
	return m_shParaShapeID;
}

short CHWPPargraph::GetStyleID() const
{
	return m_shParaStyleID;
}

HWP_BYTE CHWPPargraph::GetBreakType() const
{
	return m_chBreakType;
}

const CLineSeg* CHWPPargraph::GetLineSeg() const
{
	return m_pLineSegs;
}

VECTOR<TRangeTag> CHWPPargraph::GetRangeTags() const
{
	return m_arRangeTags;
}

CHWPPargraph* CHWPPargraph::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CHWPPargraph *pPara = new CHWPPargraph();

	if (nullptr == pPara)
		return nullptr;

	oBuffer.SavePosition();
	Parse(*pPara, nSize, oBuffer, nOff, nVersion);
	oBuffer.Skip(-oBuffer.GetDistanceToLastPos(true));

	return pPara;
}

int CHWPPargraph::Parse(CHWPPargraph& oPara, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(4); // nChars

	int nControlMask;
	oBuffer.ReadInt(nControlMask);

	oBuffer.ReadShort(oPara.m_shParaShapeID);
	oPara.m_shParaStyleID = (short)(oBuffer.ReadByte() & 0x00FF);
	oBuffer.ReadByte(oPara.m_chBreakType);

	short shNCharShapeInfo;
	oBuffer.ReadShort(shNCharShapeInfo);

	short shNRangeTags;
	oBuffer.ReadShort(shNRangeTags);

	short shNLineSeg;
	oBuffer.ReadShort(shNLineSeg);

	int nParaInstanceID;
	oBuffer.ReadInt(nParaInstanceID);

	if (nVersion >= 5032 && oBuffer.GetDistanceToLastPos() < nSize)
	{
		short shCangeTrackingMerge;
		oBuffer.ReadShort(shCangeTrackingMerge);
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

CCtrl* CHWPPargraph::FindFirstElement(const HWP_STRING& sID, bool bFullfilled, unsigned int& nIndex) const
{
	for (VECTOR<CCtrl*>::const_iterator itCtrl = m_arP.cbegin(); itCtrl != m_arP.cend(); ++itCtrl)
	{
		if (sID == (*itCtrl)->GetID() && bFullfilled == (*itCtrl)->FullFilled())
		{
			nIndex = itCtrl - m_arP.cbegin();
			return (*itCtrl);
		}
	}

	return nullptr;
}

CCtrl* CHWPPargraph::FindLastElement(const HWP_STRING& sID)
{
	for (VECTOR<CCtrl*>::const_reverse_iterator itCtrl = m_arP.crbegin(); itCtrl != m_arP.crend(); ++itCtrl)
	{
		if (sID == (*itCtrl)->GetID())
			return (*itCtrl);
	}

	return nullptr;
}

int CHWPPargraph::IndexOf(CCtrl* pCtrl)
{
	if (nullptr == pCtrl || m_arP.empty())
		return -1;

	VECTOR<CCtrl*>::const_iterator itFound = std::find_if(m_arP.cbegin(), m_arP.cend(), [pCtrl](CCtrl *pCurrCtrl){ return CCtrl::Equals(pCurrCtrl, pCtrl); });

	if (itFound != m_arP.cend())
		return itFound - m_arP.cbegin();

	return -1;
}

}

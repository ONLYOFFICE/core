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

namespace HWP
{
CHWPPargraph::CHWPPargraph()
	: m_pLineSegs(nullptr)
{}

CHWPPargraph::CHWPPargraph(CXMLReader& oReader, int nVersion)
	: m_chBreakType(0), m_pLineSegs(nullptr)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("paraPrIDRef" == sAttributeName)
			m_shParaShapeID = oReader.GetInt();
		else if ("styleIDRef" == sAttributeName)
			m_shParaStyleID = oReader.GetInt();
		else if ("pageBreak" == sAttributeName)
		{
			if (oReader.GetBool())
				m_chBreakType |= 0b00000100;
			else
				m_chBreakType &= 0b11111011;
		}
		else if ("columnBreak" == sAttributeName)
		{
			if (oReader.GetBool())
				m_chBreakType |= 0b00001000;
			else
				m_chBreakType &= 0b11110111;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	int nCharShapeID = 0;
	std::string sNodeName;

	WHILE_READ_NEXT_NODE(oReader)
	{
		sNodeName = oReader.GetNameA();

		if ("hp:run" == sNodeName)
		{
			nCharShapeID = oReader.GetAttributeInt("charPrIDRef");

			WHILE_READ_NEXT_NODE(oReader)
				ParseHWPParagraph(oReader, nCharShapeID, nVersion);
		}
		else if ("hp:linesegarray" == sNodeName)
		{
			WHILE_READ_NEXT_NODE(oReader)
			{
				if ("hp:lineseg" != oReader.GetNameA())
					continue;

				m_pLineSegs = new CLineSeg(oReader, nVersion);
				break;
			}
		}
	}

	if (m_arP.empty() || ECtrlObjectType::Character != m_arP.back()->GetCtrlType())
		m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK, nCharShapeID));
}

CHWPPargraph::~CHWPPargraph()
{
	if (nullptr != m_pLineSegs)
		delete m_pLineSegs;
}

bool CHWPPargraph::ParseHWPParagraph(CXMLReader& oReader, int nCharShapeID, int nVersion)
{
	const size_t unCurrentParaCount = m_arP.size();

	const std::string sNodeName{oReader.GetNameA()};

	if ("hp:secPr" == sNodeName)
		m_arP.push_back(new CCtrlSectionDef(L"dces", oReader, nVersion));
	else if ("hp:ctrl" == sNodeName)
	{
		WHILE_READ_NEXT_NODE(oReader)
			AddCtrl(CCtrl::GetCtrl(oReader, nVersion));
	}
	else if ("hp:t" == sNodeName)
	{
		m_arP.push_back(new CParaText(L"____", oReader.GetText(), 0, nCharShapeID));

		std::string sChildNodeName;

		WHILE_READ_NEXT_NODE(oReader)
		{
			sChildNodeName = oReader.GetNameA();

			if ("hp:lineBreak" == sChildNodeName)
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
			else if ("hp:hyphen" == sChildNodeName)
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
			else if ("hp:nbSpace" == sChildNodeName ||
			         "hp:fwSpace" == sChildNodeName)
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
			else if ("hp:tab" == sChildNodeName)
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::TABULATION));
		}
	}
	else if ("hp:tbl" == sNodeName)
		m_arP.push_back(new CCtrlTable(L" lbt", oReader, nVersion));
	else if ("hp:pic" == sNodeName)
		m_arP.push_back(new CCtrlShapePic(L"cip$", oReader, nVersion));
	else if ("hp:container" == sNodeName)
		m_arP.push_back(new CCtrlContainer(L"noc$", oReader, nVersion));
	else if ("hp:ole" == sNodeName)
		m_arP.push_back(new CCtrlShapeOle(L"elo$", oReader, nVersion));
	else if ("hp:equation" == sNodeName)
		m_arP.push_back(new CCtrlEqEdit(L"deqe", oReader, nVersion));
	else if ("hp:line" == sNodeName)
		m_arP.push_back(new CCtrlShapeLine(L"nil$", oReader, nVersion));
	else if ("hp:rect" == sNodeName)
		m_arP.push_back(new CCtrlShapeRect(L"cer$", oReader, nVersion));
	else if ("hp:ellipse" == sNodeName)
		m_arP.push_back(new CCtrlShapeEllipse(L"lle$", oReader, nVersion));
	else if ("hp:arc" == sNodeName)
		m_arP.push_back(new CCtrlShapeArc(L"cra$", oReader, nVersion));
	else if ("hp:polygon" == sNodeName)
		m_arP.push_back(new CCtrlShapePolygon(L"lop$", oReader, nVersion));
	else if ("hp:curve" == sNodeName)
		m_arP.push_back(new CCtrlShapeCurve(L"ruc$", oReader, nVersion));
	else if ("hp:connectLine" == sNodeName)
		m_arP.push_back(new CCtrlShapeConnectLine(L"loc$", oReader, nVersion));
	else if ("hp:textart" == sNodeName)
		m_arP.push_back(new CCtrlShapeTextArt(L"tat$", oReader, nVersion));
	else if ("hp:video" == sNodeName)
		m_arP.push_back(new CCtrlShapeVideo(L"div$", oReader, nVersion));

	if (unCurrentParaCount != m_arP.size())
		return true;

	if ("hp:switch" == sNodeName)
	{
		WHILE_READ_NEXT_NODE(oReader)
		{
			if ("hp:case" != oReader.GetNameA() &&
			    "hp:default" != oReader.GetNameA())
				continue;

			WHILE_READ_NEXT_NODE(oReader)
				if (ParseHWPParagraph(oReader, nCharShapeID, nVersion))
					return true;
		}
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

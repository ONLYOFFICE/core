#include "HWPPargraph.h"

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

CHWPPargraph::CHWPPargraph(CXMLNode& oNode, int nVersion)
	: m_pLineSegs(nullptr)
{
	m_shParaShapeID = oNode.GetAttributeInt(L"paraPrIDRef");
	m_shParaStyleID = oNode.GetAttributeInt(L"styleIDRef");

	if (oNode.GetAttributeBool(L"pageBreak"))
		m_chBreakType |= 0b00000100;
	else
		m_chBreakType &= 0b11111011;

	if (oNode.GetAttributeBool(L"columnBreak"))
		m_chBreakType |= 0b00001000;
	else
		m_chBreakType &= 0b11110111;

	int nCharShapeID = 0;

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:run" == oChild.GetName())
		{
			nCharShapeID = oChild.GetAttributeInt(L"charPrIDRef");

			for (CXMLNode& oGrandChild : oChild.GetChilds())
				ParseHWPParagraph(oGrandChild, nCharShapeID, nVersion);
		}
		else if (L"hp:linesegarray" == oChild.GetName())
		{
			CXMLNode oGrandChild{oChild.GetChild(L"hp:lineseg")};
			m_pLineSegs = new CLineSeg(oGrandChild, nVersion);
		}
	}

	if (m_arP.empty() || ECtrlObjectType::Character != m_arP.back()->GetCtrlType())
		m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));
}

CHWPPargraph::~CHWPPargraph()
{
	if (nullptr != m_pLineSegs)
		delete m_pLineSegs;

	CLEAR_ARRAY(TRangeTag, m_arRangeTags);
}

void CHWPPargraph::ParseHWPParagraph(CXMLNode& oNode, int nCharShapeID, int nVersion)
{
	if (L"hp:secPr" == oNode.GetName())
		m_arP.push_back(new CCtrlSectionDef(L"dces", oNode, nVersion));
	else if (L"hp:ctrl" == oNode.GetName())
	{
		for(CXMLNode& oChild : oNode.GetChilds())
			m_arP.push_back(CCtrl::GetCtrl(oChild, nVersion));
	}
	else if (L"hp:t" == oNode.GetName())
	{
		m_arP.push_back(new CParaText(L"____", oNode.GetText(), 0, nCharShapeID));

		for(CXMLNode& oChild : oNode.GetChilds())
		{
			if (L"hp:lineBreak" == oChild.GetName())
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
			else if (L"hp:hyphen" == oChild.GetName())
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
			else if (L"hp:nbSpace" == oChild.GetName()||
			         L"hp:fwSpace" == oChild.GetName())
				m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
			else if (L"hp:tab" == oChild.GetName())
				m_arP.push_back(new CParaText(L"____", L"\t", 0));
		}
	}
	else if (L"hp:tbl" == oNode.GetName())
		m_arP.push_back(new CCtrlTable(L" lbt", oNode, nVersion));
	else if (L"hp:pic" == oNode.GetName())
		m_arP.push_back(new CCtrlShapePic(L"cip$", oNode, nVersion));
	else if (L"hp:container" == oNode.GetName())
		m_arP.push_back(new CCtrlContainer(L"noc$", oNode, nVersion));
	else if (L"hp:ole" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeOle(L"elo$", oNode, nVersion));
	else if (L"hp:equation" == oNode.GetName())
		m_arP.push_back(new CCtrlEqEdit(L"deqe", oNode, nVersion));
	else if (L"hp:line" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeLine(L"nil$", oNode, nVersion));
	else if (L"hp:rect" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeRect(L"cer$", oNode, nVersion));
	else if (L"hp:ellipse" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeEllipse(L"lle$", oNode, nVersion));
	else if (L"hp:arc" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeArc(L"cra$", oNode, nVersion));
	else if (L"hp:polygon" == oNode.GetName())
		m_arP.push_back(new CCtrlShapePolygon(L"lop$", oNode, nVersion));
	else if (L"hp:curve" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeCurve(L"ruc$", oNode, nVersion));
	else if (L"hp:connectLine" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeConnectLine(L"loc$", oNode, nVersion));
	else if (L"hp:textart" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeTextArt(L"tat$", oNode, nVersion));
	else if (L"hp:video" == oNode.GetName())
		m_arP.push_back(new CCtrlShapeVideo(L"div$", oNode, nVersion));
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

void CHWPPargraph::AddRangeTag(TRangeTag* pRangeTag)
{
	m_arRangeTags.push_back(pRangeTag);
}

void CHWPPargraph::AddCtrl(CCtrl* pCtrl)
{
	m_arP.push_back(pCtrl);
}

void CHWPPargraph::AddCtrls(const LIST<CCtrl*>& arCtrls)
{
	m_arP.insert(m_arP.end(), arCtrls.begin(), arCtrls.end());
}

bool CHWPPargraph::SetCtrl(CCtrl* pCtrl, unsigned int unIndex)
{
	if (unIndex >= m_arP.size())
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

#include "HWPPargraph.h"

namespace HWP
{
CHWPPargraph::CHWPPargraph()
	: m_pLineSegs(nullptr)
{}

CHWPPargraph::~CHWPPargraph()
{
	if (nullptr != m_pLineSegs)
		delete m_pLineSegs;

	CLEAR_ARRAY(TRangeTag, m_arRangeTags);
}

void CHWPPargraph::SetLineSeg(CLineSeg* pLineSeg)
{
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

void CHWPPargraph::SetCtrl(CCtrl* pCtrl, unsigned int unIndex)
{
	if (unIndex >= m_arP.size())
		return;

	CCtrl* pOldCtrl = m_arP[unIndex];

	//TODO:: проверить нужно ли это
	if (nullptr != pOldCtrl)
		delete pOldCtrl;

	m_arP[unIndex] = pCtrl;
}

VECTOR<CCtrl*>& CHWPPargraph::GetCtrls()
{
	return m_arP;
}

const std::vector<CCtrl*> CHWPPargraph::GetCtrls() const
{
	return m_arP;
}

unsigned int CHWPPargraph::GetCountCtrls() const
{
	return m_arP.size();
}

CHWPPargraph* CHWPPargraph::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CHWPPargraph *pPara = new CHWPPargraph();

	if (nullptr == pPara)
		return nullptr;

	Parse(*pPara, nSize, oBuffer, nOff, nVersion);

	return pPara;
}

int CHWPPargraph::Parse(CHWPPargraph& oPara, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	int nChars;
	oBuffer.ReadInt(nChars);

	if ((nChars & 0x80000000) != 0)
		nChars = 0x7fffffff;

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

CCtrl* CHWPPargraph::FindFirstElement(const STRING& sID, bool bFullfilled, unsigned int& nIndex) const
{
	nIndex = 0;
	for (VECTOR<CCtrl*>::const_iterator itCtrl = m_arP.cbegin(); itCtrl != m_arP.cend(); ++itCtrl)
	{
		++nIndex;
		if (sID == (*itCtrl)->GetID() && bFullfilled == (*itCtrl)->FullFilled())
			return (*itCtrl);
	}

	return nullptr;
}

CCtrl* CHWPPargraph::FindLastElement(const STRING& sID)
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

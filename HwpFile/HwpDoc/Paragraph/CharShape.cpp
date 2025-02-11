#include "CharShape.h"
#include "CtrlCharacter.h"
#include "CtrlAutoNumber.h"
#include "ParaText.h"
#include <algorithm>

namespace HWP
{
CCharShape::CCharShape()
{
}

CCharShape::CCharShape(int nStart, int nID)
	: m_nStart(nStart), m_nCharShapeID(nID)
{
}

int CCharShape::GetStart() const
{
	return m_nStart;
}

int CCharShape::GetCharShapeID() const
{
	return m_nCharShapeID;
}

VECTOR<CCharShape*> CCharShape::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	VECTOR<CCharShape*> arCharShapes;

	int nStart, nCharShapeID;

	while (nSize - 8 >= oBuffer.GetDistanceToLastPos())
	{
		oBuffer.ReadInt(nStart);
		oBuffer.ReadInt(nCharShapeID);

		arCharShapes.push_back(new CCharShape(nStart, nCharShapeID));
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return arCharShapes;
}

int CCharShape::FillCharShape(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, std::vector<CCtrl*>& arParas)
{
	if (arParas.empty())
	{
		oBuffer.Skip(nSize);
		return 0;
	}

	oBuffer.SavePosition();

	VECTOR<CCharShape*> arCharShape = Parse(nTagNum, nLevel, nSize, oBuffer, nOff, nVersion);

	for (CCharShape* pCharShape : arCharShape)
	{
		if (0 == pCharShape->m_nStart)
		{
			for (CCtrl* pCtrl : arParas)
			{
				if (ECtrlObjectType::ParaText == pCtrl->GetCtrlType())
					((CParaText*)pCtrl)->SetCharShapeID(pCharShape->m_nCharShapeID);

				if (ECtrlObjectType::Character == pCtrl->GetCtrlType())
					((CCtrlCharacter*)pCtrl)->SetCharShapeID(pCharShape->m_nCharShapeID);
			}
		}
		else if (0 < pCharShape->m_nStart)
		{
			VECTOR<CCtrl*>::const_reverse_iterator itFound = std::find_if(arParas.crbegin(), arParas.crend(),
			                                                              [pCharShape](CCtrl* pCurCtrl)
			                                                              { if (nullptr == dynamic_cast<CParaText*>(pCurCtrl)) return false;
			                                                                return ((CParaText*)pCurCtrl)->GetStartIDx() <= pCharShape->m_nStart && pCharShape->m_nStart < ((CParaText*)pCurCtrl)->GetStartIDx() + ((CParaText*)pCurCtrl)->GetTextLength(); });

			if (itFound != arParas.crend())
			{
				CParaText* pParaText = (CParaText*)*itFound;

				if (pParaText->GetStartIDx() == pCharShape->m_nStart)
					pParaText->SetCharShapeID(pCharShape->m_nCharShapeID);
				else
				{
					// split
					int nLenToSplit = pCharShape->m_nStart - pParaText->GetStartIDx();

					HWP_STRING sSplitLeftText = pParaText->GetText().substr(0, nLenToSplit);
					HWP_STRING sSplitRightText = pParaText->GetText().substr(nLenToSplit);

					pParaText->SetText(sSplitLeftText);

					CParaText *pNewParaText = new CParaText(L"____", sSplitRightText, pCharShape->m_nStart, pCharShape->m_nCharShapeID);

					arParas.insert(itFound.base(), pNewParaText);
				}
			}

			for (CCtrl* pCtrl : arParas)
			{
				if (ECtrlObjectType::ParaText == pCtrl->GetCtrlType() && ((CParaText*)pCtrl)->GetStartIDx() > pCharShape->m_nStart)
					((CParaText*)pCtrl)->SetCharShapeID(pCharShape->m_nCharShapeID);

				if (ECtrlObjectType::Character == pCtrl->GetCtrlType())
					((CCtrlCharacter*)pCtrl)->SetCharShapeID(pCharShape->m_nCharShapeID);
			}
		}
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}
}

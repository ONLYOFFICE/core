#pragma once
#include "stdafx.h"
#include "ElementsContainer.h"

void IElement::SetUpProperties(CProperties *pProps, CElementsContainer *pSlide)
{
	if (NULL == pProps)
		return;

	for (long nIndex = 0; nIndex < pProps->m_lCount; ++nIndex)
	{
		SetUpProperty(&pProps->m_arProperties[nIndex], pSlide);
	}
}

void IElement::SetUpProperty(CProperty *pProp, CElementsContainer *pSlide)
{
	if (hspMaster == pProp->m_ePID)
	{
		m_lMasterID = (LONG)pProp->m_lValue;
	}

	if (rotation == pProp->m_ePID)
	{
		m_dRotate = (double)((LONG)pProp->m_lValue) / 0x00010000;
	}
	if (fFlipH == pProp->m_ePID)
	{
		BYTE flag1 = (BYTE)pProp->m_lValue;
		BYTE flag3 = (BYTE)(pProp->m_lValue >> 16);

		bool bFlipH = (0x01 == (0x01 & flag1));
		bool bFlipV = (0x02 == (0x02 & flag1));

		bool bUseFlipH = (0x01 == (0x01 & flag3));
		bool bUseFlipV = (0x02 == (0x02 & flag3));

		if (bUseFlipH)
			m_bFlipH = bFlipH;

		if (bUseFlipV)
			m_bFlipV = bFlipV;
	}
}

void CImageElement::SetUpProperty(CProperty* pProp, CElementsContainer* pSlide)
{
	IElement::SetUpProperty(pProp, pSlide);
	if (Pib == pProp->m_ePID)
	{
		DWORD dwIndex = pSlide->GetIndexPicture(pProp->m_lValue);
		
		CStringW strVal = (CStringW)CDirectory::ToString(dwIndex);
		m_strFileName = m_strFileName + strVal + L".jpg";
	}
}

void CShapeElement::SetUpProperties(CProperties *pProps, CElementsContainer* pSlide)
{
	IElement::SetUpProperties(pProps, pSlide);
	m_oShape.SetProperties(pProps, pSlide);
}
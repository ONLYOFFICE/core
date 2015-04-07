#pragma once
#include "stdafx.h"
#include "..\ElementsContainer.h"

void CShape::SetProperties(CProperties* pProperties, CElementsContainer* pSlide)
{
#ifdef PPT_DEF
	
	if (NSBaseShape::ppt == m_pShape->GetClassType())
	{	
		CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_pShape);
		if (NULL != pPPTShape)
		{
			pPPTShape->SetProperties(pProperties, pSlide, this);
		}
	}

#endif
	ReCalculate();
}

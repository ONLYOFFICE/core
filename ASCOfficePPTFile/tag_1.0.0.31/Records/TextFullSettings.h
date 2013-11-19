#pragma once
#include "TextHeaderAtom.h"
#include "TextCharsAtom.h"
#include "TextBytesAtom.h"
#include "StyleTextPropAtom.h"
#include "TextSpecInfoAtom.h"
#include "TextRulerAtom.h"

class CTextFullSettings
{
public:
	CRecordStyleTextPropAtom* m_pTextStyleProp;
	CRecordTextSpecInfoAtom* m_pTextSpecInfo;
	CRecordTextRulerAtom* m_pTextRuler;

	CStringW m_strText;
	DWORD m_nTextType;

public:
	CTextFullSettings()
	{
		m_pTextStyleProp = NULL;
		m_pTextSpecInfo = NULL;
		m_pTextRuler = NULL;

		m_nTextType = 0xFFFFFFFF;
		m_strText.Empty();
	}
	CTextFullSettings& operator =(const CTextFullSettings& oSrc)
	{
		m_pTextStyleProp = oSrc.m_pTextStyleProp;
		m_pTextSpecInfo = oSrc.m_pTextSpecInfo;
		m_pTextRuler = oSrc.m_pTextRuler;

		m_nTextType = oSrc.m_nTextType;
		m_strText = oSrc.m_strText;
		return (*this);
	}
	~CTextFullSettings()
	{
		m_pTextStyleProp = NULL;
		m_pTextSpecInfo = NULL;
		m_pTextRuler = NULL;
	}
	
	void CTextFullSettings::ApplyProperties(CTextAttributesEx* pTextAttributes)
	{
		if (NULL == pTextAttributes)
			return;
		
		pTextAttributes->m_sText = m_strText;

		size_t nCountPFs = pTextAttributes->m_arPFs.GetCount();
		if (0 < nCountPFs)
		{
			pTextAttributes->m_arPFs[0].lCount = (DWORD)pTextAttributes->m_sText.GetLength();
			if (1 < nCountPFs)
			{
				pTextAttributes->m_arPFs.RemoveAt(1, nCountPFs - 1);
			}
		}
		size_t nCountCFs = pTextAttributes->m_arCFs.GetCount();
		if (0 < nCountCFs)
		{
			pTextAttributes->m_arCFs[0].lCount = (DWORD)pTextAttributes->m_sText.GetLength();
			if (1 < nCountCFs)
			{
				pTextAttributes->m_arCFs.RemoveAt(1, nCountCFs - 1);
			}
		}
		
		DWORD lTextType = pTextAttributes->m_lTextType;
		pTextAttributes->m_lTextType = m_nTextType;

		if (NULL != m_pTextStyleProp)
		{
			m_pTextStyleProp->ApplyProperties(pTextAttributes);
		}
		if (NULL != m_pTextSpecInfo)
		{
			m_pTextSpecInfo->ApplyProperties(pTextAttributes);
		}
		if (NULL != m_pTextRuler)
		{
			pTextAttributes->m_oRuler = m_pTextRuler->m_oTextRuler;
		}

		pTextAttributes->m_lTextType = lTextType;
	}
};

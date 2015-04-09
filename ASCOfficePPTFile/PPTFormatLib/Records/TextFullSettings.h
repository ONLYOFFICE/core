#pragma once
#include "TextHeaderAtom.h"
#include "TextCharsAtom.h"
#include "TextBytesAtom.h"
#include "StyleTextPropAtom.h"
#include "TextSpecInfoAtom.h"
#include "TextRulerAtom.h"
#include "TextInteractiveInfoAtom.h"
#include "../../../ASCPresentationEditor/OfficeDrawing/Interactive.h"

class CTextFullSettings
{
public:
    CRecordStyleTextPropAtom*   m_pTextStyleProp;
    CRecordTextSpecInfoAtom*    m_pTextSpecInfo;
    CRecordTextRulerAtom*       m_pTextRuler;

	std::vector<NSPresentationEditor::CTextRange> m_arRanges;

	CStringW m_strText;
	DWORD m_nTextType;

public:
	CTextFullSettings() : m_arRanges()
	{
		m_pTextStyleProp = NULL;
		m_pTextSpecInfo = NULL;
		m_pTextRuler = NULL;

		m_nTextType = 0xFFFFFFFF;
		m_strText.Empty();
	}
	CTextFullSettings(const CTextFullSettings& oSrc)
	{
		*this = oSrc;
	}
	CTextFullSettings& operator =(const CTextFullSettings& oSrc)
	{
		m_pTextStyleProp = oSrc.m_pTextStyleProp;
		m_pTextSpecInfo = oSrc.m_pTextSpecInfo;
		m_pTextRuler = oSrc.m_pTextRuler;

		m_nTextType = oSrc.m_nTextType;
		m_strText = oSrc.m_strText;
		
		m_arRanges = oSrc.m_arRanges;
		return (*this);
	}
	~CTextFullSettings()
	{
		m_pTextStyleProp = NULL;
		m_pTextSpecInfo = NULL;
		m_pTextRuler = NULL;
	}
	
    CString ApplyProperties(CTextAttributesEx* pTextAttributes)
	{
		if (NULL == pTextAttributes)
			return m_strText;

		pTextAttributes->m_lTextType = m_nTextType;
		if (NULL != m_pTextStyleProp)
			NSPresentationEditor::ConvertPPTTextToEditorStructure(m_pTextStyleProp->m_arrPFs, m_pTextStyleProp->m_arrCFs, m_strText, *pTextAttributes); 

		if (NULL != m_pTextRuler)
			pTextAttributes->m_oRuler = m_pTextRuler->m_oTextRuler;

		return m_strText;
	}
};

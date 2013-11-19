#pragma once
#include "..\Reader\Records.h"

class CRecordColorSchemeAtom : public CUnknownRecord
{
	SColorAtom m_oBackgroundColor;
	SColorAtom m_oTextAndLinesColor;
	SColorAtom m_oShadowsColor;
	SColorAtom m_oTitleTextColor;
	SColorAtom m_oFillsColor;
	SColorAtom m_oAccentColor;
	SColorAtom m_oAccentAndHyperlinkColor;
	SColorAtom m_oAccentAndFollowingHyperlinkColor;

public:
	
	CRecordColorSchemeAtom()
	{
	}

	~CRecordColorSchemeAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		
		m_oBackgroundColor.FromStream(pStream);
		m_oTextAndLinesColor.FromStream(pStream);
		m_oShadowsColor.FromStream(pStream);
		m_oTitleTextColor.FromStream(pStream);
		m_oFillsColor.FromStream(pStream);
		m_oAccentColor.FromStream(pStream);
		m_oAccentAndHyperlinkColor.FromStream(pStream);
		m_oAccentAndFollowingHyperlinkColor.FromStream(pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));
		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		oWriter.WriteString(m_oBackgroundColor.ToString(_T("Background")));
		oWriter.WriteString(m_oTextAndLinesColor.ToString(_T("TextAndLines")));
		oWriter.WriteString(m_oShadowsColor.ToString(_T("ShadowsColor")));
		oWriter.WriteString(m_oTitleTextColor.ToString(_T("TitleText")));
		oWriter.WriteString(m_oFillsColor.ToString(_T("FillsColor")));
		oWriter.WriteString(m_oAccentColor.ToString(_T("Accent")));
		oWriter.WriteString(m_oAccentAndHyperlinkColor.ToString(_T("AccentAndHyperlink")));
		oWriter.WriteString(m_oAccentAndFollowingHyperlinkColor.ToString(_T("AccentAndFollowingHyperlink")));

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}

	void ToArray(CSimpleArray<SColorAtom>* pArray)
	{
		if (NULL != pArray)
		{
			pArray->Add(m_oBackgroundColor);
			pArray->Add(m_oTextAndLinesColor);
			pArray->Add(m_oShadowsColor);
			pArray->Add(m_oTitleTextColor);
			pArray->Add(m_oFillsColor);
			pArray->Add(m_oAccentColor);
			pArray->Add(m_oAccentAndHyperlinkColor);
			pArray->Add(m_oAccentAndFollowingHyperlinkColor);
		}
	}
};
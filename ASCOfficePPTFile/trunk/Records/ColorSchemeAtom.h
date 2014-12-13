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
		
		NSStreamReader::Read(pStream, m_oBackgroundColor);
		NSStreamReader::Read(pStream, m_oTextAndLinesColor);
		NSStreamReader::Read(pStream, m_oShadowsColor);
		NSStreamReader::Read(pStream, m_oTitleTextColor);
		NSStreamReader::Read(pStream, m_oFillsColor);
		NSStreamReader::Read(pStream, m_oAccentColor);
		NSStreamReader::Read(pStream, m_oAccentAndHyperlinkColor);
		NSStreamReader::Read(pStream, m_oAccentAndFollowingHyperlinkColor);
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

	void ToArray(std::vector<SColorAtom>* pArray)
	{
		if (NULL != pArray)
		{
			pArray->push_back(m_oBackgroundColor);
			pArray->push_back(m_oTextAndLinesColor);
			pArray->push_back(m_oShadowsColor);
			pArray->push_back(m_oTitleTextColor);
			pArray->push_back(m_oFillsColor);
			pArray->push_back(m_oAccentColor);
			pArray->push_back(m_oAccentAndHyperlinkColor);
			pArray->push_back(m_oAccentAndFollowingHyperlinkColor);
		}
	}
	void ToArray(std::vector<CColor>* pArray)
	{
		CColor oColor;
		if (NULL != pArray)
		{
			m_oBackgroundColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oTextAndLinesColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oShadowsColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oTitleTextColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oFillsColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oAccentColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oAccentAndHyperlinkColor.ToColor(&oColor);
			pArray->push_back(oColor);

			m_oAccentAndFollowingHyperlinkColor.ToColor(&oColor);
			pArray->push_back(oColor);
		}
	}
};
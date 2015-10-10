#pragma once
#include "../Reader/Records.h"

class CRecordTextMasterStyleAtom : public CUnknownRecord, public NSPresentationEditor::CTextStyles
{
public:
	LONG m_nTextType;
	
	CRecordTextMasterStyleAtom() : NSPresentationEditor::CTextStyles()
	{
		m_nTextType = 0;
	}

	~CRecordTextMasterStyleAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_nTextType = m_oHeader.RecInstance;

		bool bIsLevelsPresent = (0x05 <= m_oHeader.RecInstance);
		
		int lLevels = StreamUtils::ReadWORD(pStream);
		if (0 < lLevels)
			LoadLevel(0, pStream, bIsLevelsPresent);
		if (1 < lLevels)
			LoadLevel(1, pStream, bIsLevelsPresent);
		if (2 < lLevels)
			LoadLevel(2, pStream, bIsLevelsPresent);
		if (3 < lLevels)
			LoadLevel(3, pStream, bIsLevelsPresent);
		if (4 < lLevels)
			LoadLevel(4, pStream, bIsLevelsPresent);

		// походу нужно делать так: ----------------------------------------------
		if (m_pLevels[0].is_init())
		{
			if (!m_pLevels[1].is_init())
				m_pLevels[1] = m_pLevels[0].get();
			else
				m_pLevels[1]->ApplyBefore(m_pLevels[0].get());
		}
		if (m_pLevels[1].is_init())
		{
			if (!m_pLevels[2].is_init())
				m_pLevels[2] = m_pLevels[1].get();
			else
				m_pLevels[2]->ApplyBefore(m_pLevels[1].get());
		}
		if (m_pLevels[2].is_init())
		{
			if (!m_pLevels[3].is_init())
				m_pLevels[3] = m_pLevels[2].get();
			else
				m_pLevels[3]->ApplyBefore(m_pLevels[2].get());
		}
		if (m_pLevels[3].is_init())
		{
			if (!m_pLevels[4].is_init())
				m_pLevels[4] = m_pLevels[3].get();
			else
				m_pLevels[4]->ApplyBefore(m_pLevels[3].get());
		}
		// -----------------------------------------------------------------------

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
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

		for (int i = 0; i < 5; ++i)
		{
			oWriter.WriteString(_T("<TextStyleLevel>"));

			if (m_pLevels[i].is_init())
			{
				oWriter.WriteString(m_pLevels[i]->m_oPFRun.ToString(0));
				oWriter.WriteString(m_pLevels[i]->m_oCFRun.ToString(0));
			}

			oWriter.WriteString(_T("</TextStyleLevel>"));
		}
		
		//oWriter.WriteString(NSPresentationEditor::CTextStyles::ToString());

		oWriter.WriteNodeEnd(strName);
		return oWriter.GetXmlString();
	}

protected:
	void LoadLevel(LONG lLevel, POLE::Stream* pStream, bool bIsLevelPresent)
	{
		LONG lLevelOld = lLevel;
		
		if (bIsLevelPresent)
			lLevel = StreamUtils::ReadSHORT(pStream);

		CTextPFRun_ppt oPF;
		CTextCFRun_ppt oCF;
		oPF.LoadFromStream(pStream, false);
		oCF.LoadFromStream(pStream, false);

		m_pLevels[lLevelOld] = new NSPresentationEditor::CTextStyleLevel();
		m_pLevels[lLevelOld]->m_oPFRun = oPF.m_oRun;
		m_pLevels[lLevelOld]->m_oCFRun = oCF.m_oRun;
	}
};
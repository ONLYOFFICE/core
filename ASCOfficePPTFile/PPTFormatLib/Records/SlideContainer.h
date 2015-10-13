#pragma once
#include "../Reader/Records.h"
#include "../Records/Animations/AnimationTypes.h"

#include "../Records/SSSlideInfoAtom.h"

class CRecordSlide : public CRecordsContainer
{
public:
	SSlidePersist			m_oPersist;
	SlideProgTagsContainer*	m_pSlideProgTagsContainer;

	bool					m_bExistsTransition;
	
	CSlideShowSlideInfoAtom	m_oSlideShowSlideInfoAtom;

	int						m_Index;
	int						m_IndexUser;
	
private:
	DWORD m_lCountReferences;

public:
	
	CRecordSlide()
	{
		m_bExistsTransition			=	false;
		m_pSlideProgTagsContainer	=	NULL;
		m_lCountReferences			=	0;

		m_Index						=	-1;
		m_IndexUser					=	0;

		AddRef();
	}

    virtual ~CRecordSlide()
	{
        Clear();
		RELEASEOBJECT ( m_pSlideProgTagsContainer );
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_arRecords.clear();

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oHeader = oHeader;

		UINT lCurLen = 0;
		SRecordHeader oRec;
	
		while (lCurLen < m_oHeader.RecLen)
		{		
			if (oRec.ReadFromStream(pStream) == FALSE)
			{
				break;
			}

			if ( 0x03F9 == oRec.RecType )
			{
				m_bExistsTransition	=	true;
				m_oSlideShowSlideInfoAtom.ReadFromStream ( oRec, pStream );

				// TODO : временно
				if ( 0 == m_oSlideShowSlideInfoAtom.m_nEffectType )
					m_bExistsTransition	=	false;
				
				lCurLen += (8 + oRec.RecLen);
				continue;
			}

			IRecord* pRecord	=	CreateByType ( oRec );

			if ( RECORD_PROG_TAGS == oRec.RecType )
			{
				m_pSlideProgTagsContainer	=	new	SlideProgTagsContainer ();
				m_pSlideProgTagsContainer->ReadFromStream(oRec, pStream);
			}
			else
			{
				pRecord->ReadFromStream(oRec, pStream);
				m_arRecords.push_back(pRecord);
			}
		
			lCurLen += (8 + oRec.RecLen);
		}
		if (lCurLen != m_oHeader.RecLen)
		{
			// нужно разобраться, что тут такое!!!
			LONG lPosition = 0;
			StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
		}
	}

	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = _T("__SLIDE__");
		
		oWriter.WriteNodeBegin(strName);

		oWriter.WriteString(m_oPersist.ToString());
		oWriter.WriteString(CRecordsContainer::ToString());

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
	virtual CString ToString(DWORD nPID)
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = _T("__SLIDE__");
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("id"), CDirectory::ToString(nPID));
		oWriter.WriteNodeEnd(strName, TRUE, FALSE);

		oWriter.WriteString(m_oPersist.ToString());
		oWriter.WriteString(m_oSlideShowSlideInfoAtom.ToString());
		oWriter.WriteString(CRecordsContainer::ToString());

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}

    bool IsSlide()
	{
		for (int nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
		{
			if ((RECORD_TYPE_SLIDE			== m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_MAINMASTER		== m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_NOTES			== m_arRecords[nIndex]->m_oHeader.RecType))
			{
				return true;
			}
		}
		return false;
	}

	void AddRef()
	{
		++m_lCountReferences;
	}
	void Release()
	{
		--m_lCountReferences;
		if (0 == m_lCountReferences)
			delete this;
	}
	CRecordSlide* QueryInterface()
	{
		AddRef();
		return this;
	}
};

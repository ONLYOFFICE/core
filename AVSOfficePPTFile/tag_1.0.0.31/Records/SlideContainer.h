#pragma once
#include "..\Reader\Records.h"
#include "../Records/Animations/AnimationTypes.h"

#include "../Records/SSSlideInfoAtom.h"

class CRecordSlide : public CRecordsContainer
{
public:
	SSlidePersist m_oPersist;
	SlideProgTagsContainer*	m_pSlideProgTagsContainer;

	bool					m_bExistsTransition;
	
	CSlideShowSlideInfoAtom	m_oSlideShowSlideInfoAtom;
	
private:
	DWORD m_lCountReferences;

public:
	
	CRecordSlide()
	{
		m_bExistsTransition			=	false;
		m_pSlideProgTagsContainer	=	NULL;
		m_lCountReferences			=	0;

		AddRef();
	}

	~CRecordSlide()
	{
		CRecordsContainer::~CRecordsContainer();
		RELEASEOBJECT ( m_pSlideProgTagsContainer );
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		m_arRecords.RemoveAll();

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oHeader = oHeader;

		UINT lCurLen = 0;
		ULONG lReadLen = 0;
		SRecordHeader oRec;
		while (lCurLen < m_oHeader.RecLen)
		{
			if ( FAILED ( pStream->Read ( &oRec, sizeof(SRecordHeader), &lReadLen ) ) )
				break;
			
			if (0 >= lReadLen)
			{
				break;
			}

			if ( 0x03F9 == oRec.RecType )
			{
				m_bExistsTransition	=	true;
				m_oSlideShowSlideInfoAtom.ReadFromStream ( oRec, pStream );

				// TODO : временно
				if ( 0 == m_oSlideShowSlideInfoAtom.m_nEffectType )
				{
					if (0 != m_oSlideShowSlideInfoAtom.m_nSoundRef)
					{
						m_bExistsTransition	=	true;
					}
					else
					{
						m_bExistsTransition =	false;
					}
				}
				
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
				m_arRecords.Add(pRecord);
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

	BOOL IsSlide()
	{
		for (int nIndex = 0; nIndex < m_arRecords.GetSize(); ++nIndex)
		{
			if ((RECORD_TYPE_SLIDE == m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_MAINMASTER == m_arRecords[nIndex]->m_oHeader.RecType) ||
				(RECORD_TYPE_NOTES == m_arRecords[nIndex]->m_oHeader.RecType))
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
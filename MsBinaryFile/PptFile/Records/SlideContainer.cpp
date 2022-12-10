#include "SlideContainer.h"

using namespace PPT;

CRecordSlide::CRecordSlide()
{
    m_bExistsTransition			=	false;
    m_pSlideProgTagsContainer	=	NULL;
    m_lCountReferences			=	0;

    m_Index						=	-1;
    m_IndexUser					=	0;

    AddRef();
}

CRecordSlide::~CRecordSlide()
{
    Clear();
    RELEASEOBJECT ( m_pSlideProgTagsContainer );
}

void CRecordSlide::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
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

        if ( RT_SlideShowSlideInfoAtom == oRec.RecType )
        {
            m_bExistsTransition	=	true;
            m_oSlideShowSlideInfoAtom.ReadFromStream ( oRec, pStream );

            lCurLen += (8 + oRec.RecLen);
            continue;
        }

        if ( RT_ProgTags == oRec.RecType )
        {
            m_pSlideProgTagsContainer =
                    new CRecordSlideProgTagsContainer();
            m_pSlideProgTagsContainer->ReadFromStream(oRec, pStream);

            lCurLen += (8 + oRec.RecLen);
            continue;
        }

        if (RT_VbaInfo == oRec.RecType)
        {
            auto pVbaInfo = new CRecordsContainer;
            pVbaInfo->ReadFromStream(oRec, pStream);
            m_arRecords.push_back(pVbaInfo);

            lCurLen += (8 + oRec.RecLen);
            continue;
        }


        IRecord* pRecord	=	CreateByType ( oRec );



        pRecord->ReadFromStream(oRec, pStream);
        m_arRecords.push_back(pRecord);


        lCurLen += (8 + oRec.RecLen);
    }
    if (lCurLen != m_oHeader.RecLen)
    {
        // нужно разобраться, что тут такое!!!
        LONG lPosition = 0;
        StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
    }
}

bool CRecordSlide::IsSlide()
{
    for (size_t nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
    {
        if ((RT_Slide			== m_arRecords[nIndex]->m_oHeader.RecType) ||
                (RT_MainMaster		== m_arRecords[nIndex]->m_oHeader.RecType) ||
                (RT_Notes			== m_arRecords[nIndex]->m_oHeader.RecType))
        {
            return true;
        }
    }
    return false;
}

void CRecordSlide::AddRef()
{
    ++m_lCountReferences;
}

void CRecordSlide::Release()
{
    --m_lCountReferences;
    if (0 == m_lCountReferences)
        delete this;
}

CRecordSlide *CRecordSlide::QueryInterface()
{
    AddRef();
    return this;
}

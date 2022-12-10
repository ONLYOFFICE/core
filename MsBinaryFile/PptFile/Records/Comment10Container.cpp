#include "Comment10Container.h"

using namespace PPT;

void CRecordComment10AuthorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentAuthor = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10TextAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentText = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10AuthorInitialAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sCommentAuthorInitials = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen);
}

void CRecordComment10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nIndex = StreamUtils::ReadLONG(pStream);
    m_oDatetime.ReadFromStream(pStream);
    m_oAnchor.ReadFromStream(pStream);
}

CRecordComment10Container::CRecordComment10Container() :
    m_pCommentAuthorAtom(nullptr),
    m_pCommentTextAtom(nullptr),
    m_pCommentAuthorInitialsAtom(nullptr),

    m_haveAuthorAtom(false),
    m_haveTextAtom(false),
    m_haveAuthorInitialAtom(false)
{

}

CRecordComment10Container::~CRecordComment10Container()
{
    RELEASEOBJECT(m_pCommentAuthorAtom)
    RELEASEOBJECT(m_pCommentTextAtom)
            RELEASEOBJECT(m_pCommentAuthorInitialsAtom)
}

void CRecordComment10Container::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    _UINT32 lCurLen(0);
    SRecordHeader ReadHeader;

    while (lCurLen < m_oHeader.RecLen) {
        if ( ReadHeader.ReadFromStream(pStream) == false )
        {
            break;
        }

        lCurLen += 8 + ReadHeader.RecLen;

        if (ReadHeader.RecType == RT_CString) {
            switch (ReadHeader.RecInstance) {
            case 0:
                m_pCommentAuthorAtom = new CRecordComment10AuthorAtom();
                m_pCommentAuthorAtom->ReadFromStream(ReadHeader, pStream);
                m_haveAuthorAtom = true;
                break;
            case 1:
                m_pCommentTextAtom = new CRecordComment10TextAtom();
                m_pCommentTextAtom->ReadFromStream(ReadHeader, pStream);
                m_haveTextAtom = true;
                break;

            case 2:
                m_pCommentAuthorInitialsAtom = new CRecordComment10AuthorInitialAtom();
                m_pCommentAuthorInitialsAtom->ReadFromStream(ReadHeader, pStream);
                m_haveAuthorInitialAtom = true;
                break;
            default:
                break;
            }

        } else
        {
            m_oCommentAtom.ReadFromStream(ReadHeader, pStream);
        }
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}



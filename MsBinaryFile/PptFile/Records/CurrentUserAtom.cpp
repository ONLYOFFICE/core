#include "CurrentUserAtom.h"

using namespace PPT;

CRecordCurrentUserAtom::CRecordCurrentUserAtom() : m_nToken(0)
{
    m_nSize = m_nRelVersion = m_nToken = m_nOffsetToCurEdit = 0;
    m_nLenUserName = m_nDocFileVersion = 0;
    m_nMinorVersion = m_nMajorVersion = 0;

}

CRecordCurrentUserAtom::~CRecordCurrentUserAtom()
{

}

void CRecordCurrentUserAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nSize				= StreamUtils::ReadDWORD(pStream);

    long sz = (long)(pStream->size() - pStream->tell());

    if ((long)m_nSize >  sz )
    {
        m_nSize =  sz ;
    }

    if (m_nSize < 16) return;

    m_nToken			= StreamUtils::ReadDWORD(pStream);

    m_nOffsetToCurEdit	= StreamUtils::ReadDWORD(pStream);

    m_nLenUserName		= StreamUtils::ReadWORD(pStream);

    m_nDocFileVersion	= StreamUtils::ReadWORD(pStream);

    m_nMajorVersion		= StreamUtils::ReadBYTE(pStream);
    m_nMinorVersion		= StreamUtils::ReadBYTE(pStream);

    StreamUtils::StreamSkip(2, pStream);

    m_strANSIUserName = StreamUtils::ReadStringA(pStream, m_nLenUserName);

    m_nRelVersion = StreamUtils::ReadDWORD(pStream);

    m_strUNICODEUserName = StreamUtils::ReadStringW(pStream, m_nLenUserName );

}

bool CRecordCurrentUserAtom::IsSupported() const
{
    return (NO_ENCRYPT == m_nToken);
}

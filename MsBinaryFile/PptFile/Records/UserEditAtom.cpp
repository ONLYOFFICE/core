#include "UserEditAtom.h"

using namespace PPT;

CRecordUserEditAtom::CRecordUserEditAtom()
{
    m_nEncryptSessionPersistIdRef = 0;
}

CRecordUserEditAtom::~CRecordUserEditAtom()
{
}

void CRecordUserEditAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nLastSlideIdRef = StreamUtils::ReadDWORD(pStream);

    m_nVersion		= StreamUtils::ReadWORD(pStream);
    m_nMinorVersion = StreamUtils::ReadBYTE(pStream);
    m_nMajorVersion = StreamUtils::ReadBYTE(pStream);

    m_nOffsetLastEdit				= StreamUtils::ReadDWORD(pStream);
    m_nOffsetPersistDirectory		= StreamUtils::ReadDWORD(pStream);
    m_nOffsetDocPersistIdRef		= StreamUtils::ReadDWORD(pStream);

    m_nPersistIdSeed				= StreamUtils::ReadDWORD(pStream);
    m_nLastView						= StreamUtils::ReadWORD(pStream);

    StreamUtils::StreamSkip(2, pStream);

    if (m_oHeader.RecLen > 28)
    {
        m_nEncryptSessionPersistIdRef	= StreamUtils::ReadDWORD(pStream);
    }
}

CRecordUserEditAtom &CRecordUserEditAtom::operator =(const CRecordUserEditAtom &oSrc)
{
    m_nLastSlideIdRef				= oSrc.m_nLastSlideIdRef;

    m_nVersion						= oSrc.m_nVersion;
    m_nMinorVersion					= oSrc.m_nMinorVersion;
    m_nMajorVersion					= oSrc.m_nMajorVersion;

    m_nOffsetLastEdit				= oSrc.m_nOffsetLastEdit;
    m_nOffsetPersistDirectory		= oSrc.m_nOffsetPersistDirectory;
    m_nOffsetDocPersistIdRef		= oSrc.m_nOffsetDocPersistIdRef;
    m_nPersistIdSeed				= oSrc.m_nPersistIdSeed;
    m_nLastView						= oSrc.m_nLastView;
    m_nEncryptSessionPersistIdRef	= oSrc.m_nEncryptSessionPersistIdRef;
    return (*this);
}

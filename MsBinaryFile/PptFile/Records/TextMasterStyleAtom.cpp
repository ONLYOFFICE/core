#include "TextMasterStyleAtom.h"

using namespace PPT;

CRecordTextMasterStyleAtom::CRecordTextMasterStyleAtom() : PPT::CTextStyles()
{
    m_nTextType = -1;
}

CRecordTextMasterStyleAtom::~CRecordTextMasterStyleAtom()
{
}

void CRecordTextMasterStyleAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
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

void CRecordTextMasterStyleAtom::LoadLevel(LONG lLevel, POLE::Stream *pStream, bool bIsLevelPresent)
{
    LONG lLevelOld = lLevel;

    if (bIsLevelPresent)
        lLevel = StreamUtils::ReadSHORT(pStream);

    CTextPFRunRecord oPF;
    CTextCFRunRecord oCF;
    oPF.LoadFromStream(pStream, false);
    oCF.LoadFromStream(pStream, false);

    m_pLevels[lLevelOld] = new PPT::CTextStyleLevel();
    m_pLevels[lLevelOld]->m_oPFRun = oPF.m_oRun;
    m_pLevels[lLevelOld]->m_oCFRun = oCF.m_oRun;
}

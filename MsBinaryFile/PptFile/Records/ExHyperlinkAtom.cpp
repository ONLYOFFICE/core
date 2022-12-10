#include "ExHyperlinkAtom.h"

using namespace PPT;

void CRecordExHyperlinkAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nHyperlinkID = StreamUtils::ReadDWORD(pStream);
}

bool CRecordExHyperlinkContainer::hasCString() const
{
    return m_friendlyNameAtom.IsInit() || m_targetAtom.IsInit() || m_locationAtom.IsInit();
}

void CRecordExHyperlinkContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader header;

    header.ReadFromStream(pStream);
    m_exHyperlinkAtom.ReadFromStream(header, pStream);

    unsigned currentLen = 12; // 12(atom)
    while (currentLen < m_oHeader.RecLen)
    {
        header.ReadFromStream(pStream);
        auto* pCString = new CRecordCString;
        pCString->ReadFromStream(header, pStream);
        switch (header.RecInstance)
        {
        case 0: m_friendlyNameAtom  = pCString; break;
        case 1: m_targetAtom        = pCString; break;
        case 3: m_locationAtom      = pCString; break;
        default: delete pCString;
        }
        currentLen += 8 + header.RecLen; // headerLen + CStringLen
    }
//        if (m_friendlyNameAtom.IsInit() && (int)m_friendlyNameAtom->m_strText.find(L"NEXT") == -1)
//            std::wcout << m_exHyperlinkAtom.m_nHyperlinkID << L" "
//                       << (m_friendlyNameAtom.IsInit() ? m_friendlyNameAtom->m_strText : L"-") << L" "
//                       << (m_targetAtom.IsInit() ? m_targetAtom->m_strText : L"-") << L" "
//                       << (m_locationAtom.IsInit() ? m_locationAtom->m_strText : L"-")
//                       << std::endl;
}

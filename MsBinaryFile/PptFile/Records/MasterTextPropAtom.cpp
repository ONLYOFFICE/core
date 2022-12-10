#include "MasterTextPropAtom.h"

using namespace PPT;

CRecordMasterTextPropAtom::SMasterTextPropRun::SMasterTextPropRun()
{

}

CRecordMasterTextPropAtom::SMasterTextPropRun::SMasterTextPropRun(const SMasterTextPropRun &oSrc)
{
    lCount = oSrc.lCount;
    lIndentLevel = oSrc.lIndentLevel;
}

CRecordMasterTextPropAtom::SMasterTextPropRun &CRecordMasterTextPropAtom::SMasterTextPropRun::operator=(const SMasterTextPropRun &oSrc)
{
    lCount = oSrc.lCount;
    lIndentLevel = oSrc.lIndentLevel;

    return *this;
}

void CRecordMasterTextPropAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_arrProps.clear();

    size_t nCount = m_oHeader.RecLen / 6;
    while (nCount != 0)
    {
        --nCount;

        SMasterTextPropRun oRun;
        oRun.lCount = StreamUtils::ReadDWORD(pStream);
        oRun.lIndentLevel = StreamUtils::ReadWORD(pStream);

        m_arrProps.push_back(oRun);
    }
}

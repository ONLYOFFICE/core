
void CRecordExObjRefAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nExObjID = StreamUtils::ReadDWORD(pStream);
}

#include "DateTimeStruct.h"


PPT::DateTimeStruct::DateTimeStruct() :
    m_wYear(0), m_wMonth(0),    m_wDayOfWeek(0),    m_wDay(0),
    m_wHour(0), m_wMinute(0),   m_wSecond(0),       m_wMilliseconds(0)
{}

void PPT::DateTimeStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_wYear         = StreamUtils::ReadWORD(pStream);
    m_wMonth        = StreamUtils::ReadWORD(pStream);
    m_wDayOfWeek    = StreamUtils::ReadWORD(pStream);
    m_wDay          = StreamUtils::ReadWORD(pStream);
    m_wHour         = StreamUtils::ReadWORD(pStream);
    m_wMinute       = StreamUtils::ReadWORD(pStream);
    m_wSecond       = StreamUtils::ReadWORD(pStream);
    m_wMilliseconds = StreamUtils::ReadWORD(pStream);
}

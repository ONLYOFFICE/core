#include "RoundTripNotesMasterTextStyles12Atom.h"

using namespace PPT;

CRecordRoundTripNotesMasterTextStyles12Atom::CRecordRoundTripNotesMasterTextStyles12Atom()
{
}

CRecordRoundTripNotesMasterTextStyles12Atom::~CRecordRoundTripNotesMasterTextStyles12Atom()
{
}

void CRecordRoundTripNotesMasterTextStyles12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
    pStream->read(data.first.get(), data.second);

    //                std::string filename = "notes/" + std::to_string(data.second) + ".zip";
    //                std::ofstream file(filename, std::ios::binary | std::ios::out);
    //                file.write((char*)data.first.get(), data.second);
    //                file.close();
}

CRecordRoundTripCustomTableStyles12Atom::CRecordRoundTripCustomTableStyles12Atom()
{
}

CRecordRoundTripCustomTableStyles12Atom::~CRecordRoundTripCustomTableStyles12Atom()
{
}

void CRecordRoundTripCustomTableStyles12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[m_oHeader.RecLen]), m_oHeader.RecLen);
    pStream->read(data.first.get(), data.second);
}

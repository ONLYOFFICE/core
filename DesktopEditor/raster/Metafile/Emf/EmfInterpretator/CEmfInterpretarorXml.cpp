#include "CEmfInterpretarorXml.h"

namespace MetaFile
{
    CEmfInterpretarorXml::CEmfInterpretarorXml() :
        m_pOutputXml(new CXmlOutput(TypeXmlOutput::IsWriter))
    {
        m_pOutputXml->WriteString(L"<?xml version=\"1.0\"?>\n");
        m_pOutputXml->WriteNodeBegin(L"EMF");
    }

    CEmfInterpretarorXml::~CEmfInterpretarorXml()
    {
        if (NULL != m_pOutputXml)
        {
            m_pOutputXml->WriteNodeEnd(L"EMF");
            m_pOutputXml->SaveToFile(NSFile::GetProcessDirectory() + L"/test.xml");
            delete m_pOutputXml;
        }
    }

    void CEmfInterpretarorXml::Save_EMR_HEADER(const TEmfHeader &oTEmfHeader, const unsigned int &unValue, CDataStream &oDataStream)
    {
        m_pOutputXml->WriteNodeBegin(L"EMR_HEADER", {XmlArgument(L"Id", EMR_HEADER),
                                                     XmlArgument(L"Size", unValue + 8)});

		m_pOutputXml->WriteNode(L"Bounds",		oTEmfHeader.oBounds);
		m_pOutputXml->WriteNode(L"Frame",		oTEmfHeader.oFrame);
		m_pOutputXml->WriteNode(L"Signature",		oTEmfHeader.ulSignature);
		m_pOutputXml->WriteNode(L"Version",		oTEmfHeader.ulVersion);
		m_pOutputXml->WriteNode(L"Size",		oTEmfHeader.ulSize);
		m_pOutputXml->WriteNode(L"Records",		oTEmfHeader.ulRecords);
		m_pOutputXml->WriteNode(L"CountObjects",	oTEmfHeader.ushObjects);
		m_pOutputXml->WriteNode(L"Reserved",		oTEmfHeader.ushReserved);
		m_pOutputXml->WriteNode(L"SizeDescription",	oTEmfHeader.ulSizeDescription);
		m_pOutputXml->WriteNode(L"OffsetDescription",	oTEmfHeader.ulOffsetDescription);
		m_pOutputXml->WriteNode(L"PalletEntries",	oTEmfHeader.ulPalEntries);
		m_pOutputXml->WriteNode(L"Device",		oTEmfHeader.oDevice);
		m_pOutputXml->WriteNode(L"Millimeters",		oTEmfHeader.oMillimeters);

		unsigned int unSize = unValue - 80;
		if (unSize > 0)
			m_pOutputXml->WriteNode(L"Buffer", oDataStream, unSize);

                m_pOutputXml->WriteNodeEnd(L"EMR_HEADER");
    }
}

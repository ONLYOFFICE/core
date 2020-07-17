#include "CXmlFile.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"

CXmlFile::CXmlFile()
{
    m_oXmlStructure = NULL;
}

CXmlFile::~CXmlFile()
{
    delete m_oXmlStructure;
}

std::wstring CXmlFile::GetXml()
{
    std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";

    if (m_oXmlStructure != NULL)
        sXml += m_oXmlStructure->GetElementXML();

    return sXml;
}

void CXmlFile::SetXmlStructure(CElement *oXmlStructure)
{
    m_oXmlStructure = oXmlStructure;
}

CElement* CXmlFile::GetXmlStructure()
{
    return m_oXmlStructure;
}

void CXmlFile::SetDefoult()
{
}

bool CXmlFile::SaveToFile(const std::wstring& sPathFile)
{
    XmlUtils::CXmlWriter oXmlWriter;
    oXmlWriter.WriteString(GetXml());
    if (!oXmlWriter.SaveToFile(sPathFile))
        return false;

    return true;
}

bool CXmlFile::IsEmpty()
{
    if (m_oXmlStructure == NULL)
        return true;

    return false;
}

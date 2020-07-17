#include "CAppXml.h"
#include "CProperties.h"

CAppXml::CAppXml()
    : CXmlFile()
{
}

void CAppXml::SetDefoult()
{
    CProperties *oProperties = new CProperties;
    oProperties->SetDefoult();
    SetXmlStructure(oProperties);
}

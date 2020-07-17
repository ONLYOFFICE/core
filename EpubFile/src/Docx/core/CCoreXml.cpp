#include "CCoreXml.h"
#include "CCoreProperties.h"

CCoreXml::CCoreXml()
    : CXmlFile()
{
}

void CCoreXml::SetDefoult()
{
    CCoreProperties *oCoreProperties = new CCoreProperties();
    oCoreProperties->SetDefoult();
    SetXmlStructure(oCoreProperties);
}

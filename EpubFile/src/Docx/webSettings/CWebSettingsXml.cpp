#include "CWebSettingsXml.h"
#include "CWebSettings.h"

CWebSettingsXml::CWebSettingsXml()
    : CXmlFile()
{
}

void CWebSettingsXml::SetDefoult()
{
    CWebSettings *oWebSettings = new CWebSettings;
    oWebSettings->SetDefoult();
    SetXmlStructure(oWebSettings);
}

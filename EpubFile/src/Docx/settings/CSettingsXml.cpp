#include "CSettingsXml.h"
#include "CSettings.h"

CSettingsXml::CSettingsXml()
    :CXmlFile()
{
}

void CSettingsXml::SetDefoult()
{
    CSettings *oSettings = new CSettings();
    oSettings->SetDefoult();
    SetXmlStructure(oSettings);
}

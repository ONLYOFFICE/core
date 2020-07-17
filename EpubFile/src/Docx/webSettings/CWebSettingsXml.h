#ifndef CWEBSETTINGSXML_H
#define CWEBSETTINGSXML_H

#include "../CXmlFile.h"

class CWebSettingsXml : public CXmlFile
{
public:
    CWebSettingsXml();

    void SetDefoult() override;
};

#endif // CWEBSETTINGSXML_H

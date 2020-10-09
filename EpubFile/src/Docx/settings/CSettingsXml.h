#ifndef CSETTINGSXML_H
#define CSETTINGSXML_H

#include "../CXmlFile.h"

class CSettingsXml : public CXmlFile
{
public:
    CSettingsXml();

    void SetDefoult() override;
};

#endif // CSETTINGSXML_H

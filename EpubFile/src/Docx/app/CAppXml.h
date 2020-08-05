#ifndef CAPPXML_H
#define CAPPXML_H

#include "../CXmlFile.h"

class CAppXml : public CXmlFile
{
public:
    CAppXml();

    void SetDefoult() override;
};

#endif // CAPPXML_H

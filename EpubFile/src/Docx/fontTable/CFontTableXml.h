#ifndef CFONTTABLEXML_H
#define CFONTTABLEXML_H

#include "../CXmlFile.h"

class CFontTableXml : public CXmlFile
{
public:
    CFontTableXml();

    void SetDefoult() override;
};

#endif // CFONTTABLEXML_H

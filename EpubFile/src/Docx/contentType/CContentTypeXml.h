#ifndef CCONTENTTYPEXML_H
#define CCONTENTTYPEXML_H

#include "../CXmlFile.h"

class CContentTypeXml : public CXmlFile
{
public:
    CContentTypeXml();

    void SetDefoult() override;
};

#endif // CCONTENTTYPEXML_H

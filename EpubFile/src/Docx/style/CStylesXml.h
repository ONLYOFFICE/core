#ifndef CSTYLESXML_H
#define CSTYLESXML_H

#include "../CXmlFile.h"

class CStylesXml : public CXmlFile
{
public:
    CStylesXml();

    void SetDefoult() override;
};

#endif // CSTYLESXML_H

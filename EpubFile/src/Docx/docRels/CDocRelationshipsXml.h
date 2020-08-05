#ifndef CDOCRELATIONSHIPSXML_H
#define CDOCRELATIONSHIPSXML_H

#include "../CXmlFile.h"

class CDocRelationshipsXml : public CXmlFile
{
public:
    CDocRelationshipsXml();

    void SetDefoult() override;

    void AddLinkToFile(std::wstring sFilePath, int nId);
};

#endif // CDOCRELATIONSHIPSXML_H

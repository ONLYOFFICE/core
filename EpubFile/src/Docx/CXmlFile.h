#ifndef CXMLFILE_H
#define CXMLFILE_H

#include "CElement.h"

class CXmlFile
{
    CElement *m_oXmlStructure;
public:
    CXmlFile();
    virtual ~CXmlFile();

    std::wstring GetXml();

    void SetXmlStructure(CElement* oDocumentXml);
    virtual void SetDefoult();

    bool SaveToFile(const std::wstring& sPathFile);
    bool IsEmpty();

    CElement* GetXmlStructure();
};

#endif // CXMLFILE_H

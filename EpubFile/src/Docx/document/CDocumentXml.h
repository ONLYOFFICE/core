#ifndef CDOCUMENTXML_H
#define CDOCUMENTXML_H

#include <iostream>
#include "../CXmlFile.h"
#include "CParagraph.h"

class CDocumentXml : public CXmlFile
{
public:
    CDocumentXml();

    void SetDefoult() override;

    bool AddParagraph(std::wstring sText, bool bNewPAge = false);
    bool AddParagraph(CParagraph *oParagraph);

    bool AddElement(CElement *oElement, bool bEnd = false);

    CParagraph* GetParagraph(int nIndex);
};

#endif // CDOCUMENTXML_H

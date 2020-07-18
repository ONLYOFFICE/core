#ifndef CPARAGRAPH_H
#define CPARAGRAPH_H

#include "../CElement.h"
#include "../docRels/CDocRelationshipsXml.h"

class CParagraph : public CElement
{
    void SetTagNewPage();
public:
    CParagraph();
    CParagraph(std::wstring sText, bool bNewPage = false);

    void SetDefoult() override;
    void SetText(std::wstring sText, bool newPage);
    void AddText(std::wstring sText, bool newPage);

    void AddHyperlinkToc(int nIdToc);
    void AddLinkToFile(CDocRelationshipsXml *oDocRel, std::wstring sPathFile);

    void SetInCenter();
};

#endif // CPARAGRAPH_H

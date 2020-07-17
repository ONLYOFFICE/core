#ifndef CPARAGRAPH_H
#define CPARAGRAPH_H

#include "../CElement.h"

class CParagraph : public CElement
{
    void SetTagNewPage();
public:
    CParagraph();
    CParagraph(std::wstring sText, bool bNewPage = false);

    void SetDefoult() override;
    void SetText(std::wstring sText, bool newPage);
    void AddText(std::wstring sText, bool newPage);
};

#endif // CPARAGRAPH_H

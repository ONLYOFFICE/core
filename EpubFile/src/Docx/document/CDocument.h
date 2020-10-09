#ifndef CDOCUMENT_H
#define CDOCUMENT_H

#include "../CElement.h"
#include "CParagraph.h"

class CDocument : public CElement
{
public:
    CDocument();

    void SetDefoult() override;
    bool AddParagraph(std::wstring sText, bool bNewPAge = false);
    bool AddParagraph(CParagraph *oParagraph);

    CParagraph* GetParagraph(int nIndex);
};

#endif // CDOCUMENT_H

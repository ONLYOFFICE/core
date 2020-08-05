#ifndef CHYPERLINK_H
#define CHYPERLINK_H

#include "../CElement.h"

class CHyperlink : public CElement
{
    int m_nId;
public:
    CHyperlink();

    void SetDefoult() override;
    void CreateTocHyperlink(std::wstring sText);
    void CreateFileLink(std::wstring sText);

    int GetId();
};

#endif // CHYPERLINK_H

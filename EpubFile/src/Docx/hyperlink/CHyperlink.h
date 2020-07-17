#ifndef CHYPERLINK_H
#define CHYPERLINK_H

#include "../CElement.h"

class CHyperlink : public CElement
{
    std::wstring m_sPageRef;
public:
    CHyperlink();

    void SetDefoult() override;
    void SetText(std::wstring sText);
};

#endif // CHYPERLINK_H

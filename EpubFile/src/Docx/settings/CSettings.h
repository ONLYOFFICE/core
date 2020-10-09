#ifndef CSETTINGS_H
#define CSETTINGS_H

#include "../CElement.h"

class CSettings : public CElement
{
public:
    CSettings();

    void SetDefoult() override;
};

#endif // CSETTINGS_H

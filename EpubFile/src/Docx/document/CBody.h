#ifndef CBODY_H
#define CBODY_H

#include "../CElement.h"

class CBody : public CElement
{
    int nCountParagraphs;
    int nCountWords;
    int nCountLines;
public:
    CBody();

    void SetDefoult() override;
};

#endif // CBODY_H

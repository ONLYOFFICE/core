#ifndef CFILEDOCUMENT_H
#define CFILEDOCUMENT_H

#include <iostream>
#include "document/CBody.h"

class CFileDocument
{
public:
    CFileDocument(std::wstring sPathDir);
    ~CFileDocument();
};

#endif // CFILEDOCUMENT_H

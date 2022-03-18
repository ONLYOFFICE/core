#ifndef OO_DECOMPRESS_H
#define OO_DECOMPRESS_H

#include "../../BgraFrame.h"

namespace openjpeg
{
    bool Parse(const std::wstring& sFile, CBgraFrame* pFrame, bool isBGRA);
    bool Parse(BYTE* pData, const unsigned int& nSize, CBgraFrame* pFrame, bool isBGRA);
}

#endif

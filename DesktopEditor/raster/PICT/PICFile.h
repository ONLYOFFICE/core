#ifndef PICFILE_H
#define PICFILE_H

#include "../BgraFrame.h"

namespace PICT {
    class GRAPHICS_DECL CPictFile {
    public:
        bool Open(CBgraFrame* pFrame, const std::wstring& strFileName, bool isRGBA);
        bool Open(CBgraFrame* pFrame, BYTE* pBuffer, int nSize, bool isRGBA);
    };
}

#endif // PICFILE_H

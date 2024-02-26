#include "../BgraFrame.h"
#include "../../../Common/3dParty/libjxr/jxrgluelib/JXRGlue.h"

namespace WDP {
	class GRAPHICS_DECL CWDPFile {
    public:
        bool Open(CBgraFrame* pFrame, const std::wstring& strFileName, bool isRGBA);
    };
}


#include "../BgraFrame.h"
#include "../../Common/3dParty/heif/libheif/libheif/api/libheif/heif.h"
#include "../../UnicodeConverter/UnicodeConverter.h"

namespace NSHeif {
	class GRAPHICS_DECL CHeifFile {
	private:
		CHeifFile() = delete;
	public:
		static bool isHeif(const std::wstring& fileName);
		static bool isHeif(BYTE* buffer, DWORD size);

		static bool Open(CBgraFrame* frame, const std::wstring& fileName);
		static bool Open(CBgraFrame* frame, BYTE* buffer, DWORD size);
		static bool Save(const BYTE* source, int width, int height, int sourceStride, const std::wstring& dstPath);

	private:
		static bool IsError(heif_error err);
		static bool Decode(heif_context* ctx, CBgraFrame* frame);
		static NSUnicodeConverter::CUnicodeConverter m_oConverter;
	};
}

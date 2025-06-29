#include "../BgraFrame.h"

namespace NSHeif {
	class GRAPHICS_DECL CHeifFile {
	private:
		CHeifFile() = delete;
	public:
		static bool isHeif(const std::wstring& fileName);
		static bool isHeif(BYTE* buffer, size_t size);

		static bool Open(CBgraFrame* frame, const std::wstring& fileName, bool isRGBA);
		static bool Open(CBgraFrame* frame, BYTE* buffer, size_t size, bool isRGBA);
		static bool Save(CBgraFrame* frame, const std::wstring& dstPath);
	};
}

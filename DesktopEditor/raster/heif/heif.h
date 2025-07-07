#include "../BgraFrame.h"
#include "../../Common/3dParty/heif/libheif/libheif/api/libheif/heif.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
#include <functional>

namespace NSHeif {
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define defer(code) Defer CONCAT(_defer_, __COUNTER__)([&](){code;})

	class Defer {
		std::function<void()> func;
	public:
		explicit Defer(std::function<void()> func) : func(func) {}
		~Defer() { func(); }
	};

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
		static inline bool IsError(heif_error err);
		static inline bool Decode(heif_context* ctx, CBgraFrame* frame);
		static inline NSUnicodeConverter::CUnicodeConverter m_oConverter{};
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
		static inline const char* m_sPluginPath = "..\\..\\..\\Common\\3dParty\\heif\\libheif\\libheif\\api\\libheif\\libheif\\plugins\\Release\\heif-x265.dll";
#elif defined(_LINUX)
		static inline const char* m_sPluginPath = "../../../Common/3dParty/heif/libheif/libheif/api/libheif/libheif/plugins/libheif-x265.so";
#else
		static inline const char* m_sPluginPath = "../../../Common/3dParty/heif/libheif/libheif/api/libheif/libheif/plugins/libheif-x265.dylib";
#endif
	};
}

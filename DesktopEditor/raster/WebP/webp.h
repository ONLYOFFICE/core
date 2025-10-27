#ifndef WEBP_H
#define WEBP_H

#include "../../graphics/pro/Graphics.h"

namespace NSWebP {
    class CWebPFile
	{
    private:
        class Stream;
	private:
		CWebPFile() = delete;

	public:
		static bool isWebP(const std::wstring& fileName);
		static bool isWebP(BYTE* buffer, const DWORD& size);

		static bool Open(CBgraFrame* frame, const std::wstring& fileName, bool isRGBA);
		static bool Open(CBgraFrame* frame, BYTE* buffer, const DWORD& size, bool isRGBA);
		static bool Save(const BYTE* source, int width, int height, const std::wstring& dstPath, bool isRGBA);

	private:
		static bool Decode(Stream* stream, CBgraFrame* frame, bool isRGBA);

};
}
#endif

#include <malloc.h>
#include <stdio.h>

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

class CFontBinary
{
public:
	unsigned char* Data  = NULL;
	int            Size  = 0;
	int            Index = 0;

public:
	CFontBinary()
	{
	}
	~CFontBinary()
	{
		if (NULL != Data)
			delete [] Data;
	}
};

#include "emscripten.h"
//EM_JS
EM_ASYNC_JS(CFontBinary*, js_get_system_font, (int index), {
	return AscFonts.getSystemFont(index);	
});

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT CFontBinary* createFontBinary(int index, unsigned char* data, int size)
{
	CFontBinary* pFont = new CFontBinary();
	pFont->Index = index;
	pFont->Data = data;
	pFont->Size = size;
	
	return pFont;
}

WASM_EXPORT void deleteFontBinary(CFontBinary* pFont)
{
	delete pFont;
}

WASM_EXPORT void parseSystemFonts(int size)
{
	printf("[native] fonts: %d\n", size);

	for (int i = 0; i < size; ++i)
	{
		CFontBinary* pFont = js_get_system_font(i);

		printf("[native] %d, %d\n", pFont->Index, pFont->Size);

		delete pFont;
	}
}

#ifdef __cplusplus
}
#endif

#ifndef _BUILD_GRAPHICS_SVG_TRANSFORMER_H_
#define _BUILD_GRAPHICS_SVG_TRANSFORMER_H_

#include "../../../graphics/IRenderer.h"
#include "../../../graphics/config.h"
#include "../../../graphics/pro/Fonts.h"

class CSVGTransformer_private;
class GRAPHICS_DECL CSVGTransformer
{
public:
	CSVGTransformer();
	~CSVGTransformer();

public:

	bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize);
	bool OpenFromFile(const std::wstring& file);
	bool Load(const std::wstring& content);

	void Close();

	void SetFontManager(NSFonts::IFontManager* pFontManager);

	bool Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
	int get_Width();
	int get_Height();
	int get_Metrics();

private:
	CSVGTransformer_private* m_internal;
};

#endif // _BUILD_GRAPHICS_SVG_TRANSFORMER_H_

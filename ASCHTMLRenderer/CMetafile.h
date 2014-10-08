#ifndef C_METAFILE
#define C_METAFILE

#include "../DesktopEditor/graphics/IRenderer.h"
namespace NSHtmlRenderer
{
	class CImageExt
	{
	public:
		LONG GetImageType(const std::wstring& strFile);

		void DrawOnRenderer(IRenderer* pRenderer, const std::wstring& strFile, const double& dX, const double& dY, const double& dW, const double& dH, const BOOL& bIsFromFileUse);
	};
}
#endif
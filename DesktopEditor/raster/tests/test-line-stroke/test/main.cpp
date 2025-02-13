#include "../../../../graphics/pro/Graphics.h"
#include "../../../../../raster/BgraFrame.h"
#include "../../../../../common/Directory.h"

#define RGB_TO_INT(r, g, b) ((long)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) | ( (unsigned char)(255) << 24 ) ) )

void DrawLine(IRenderer* pRenderer, unsigned int unX1, unsigned int unY1, unsigned int unX2, unsigned int unY2)
{
	pRenderer->BeginCommand(c_nPathType);
	pRenderer->PathCommandMoveTo(unX1, unY1);
	pRenderer->PathCommandLineTo(unX2, unY2);
	pRenderer->DrawPath(c_nStroke);
	pRenderer->EndCommand(c_nPathType);
}

void DrawEquilateralPolygon(IRenderer* pRenderer, unsigned int unCenterX, unsigned int unCenterY, unsigned int unRadius, unsigned int unCountCorners)
{
	if (nullptr == pRenderer)
		return;

	unsigned int unX1, unY1, unX2, unY2;

	for (unsigned int unIndex = 1; unIndex < unCountCorners; ++unIndex)
	{
		unX1 = (unsigned int)(unRadius * sin(2 * (unIndex - 1) * M_PI / unCountCorners) + 0.5);
		unY1 = (unsigned int)(unRadius * cos(2 * (unIndex - 1) * M_PI / unCountCorners) + 0.5);
		unX2 = (unsigned int)(unRadius * sin(2 * (unIndex - 0) * M_PI / unCountCorners) + 0.5);
		unY2 = (unsigned int)(unRadius * cos(2 * (unIndex - 0) * M_PI / unCountCorners) + 0.5);

		DrawLine(pRenderer, unCenterX + unX1, unCenterY + unY1, unCenterX + unX2, unCenterY + unY2);
	}

	unX1 = unX2;
	unY1 = unY2;

	unX2 = (unsigned int)(unRadius * sin(0) + 0.5);
	unY2 = (unsigned int)(unRadius * cos(0) + 0.5);

	DrawLine(pRenderer, unCenterX + unX1, unCenterY + unY1, unCenterX + unX2, unCenterY + unY2);
}

int main(int argc, char *argv[])
{
	NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();

	unsigned int unWidth  = 521;
	unsigned int unHeight = 172;

	BYTE* pData = new BYTE[4 * unWidth * unHeight];
	for (unsigned long unIndex = 0; unIndex < unWidth * unHeight; ++unIndex)
		((unsigned int*)pData)[unIndex] = 0xffffff;

	CBgraFrame oFrame;
	oFrame.put_Data(pData);
	oFrame.put_Width(unWidth);
	oFrame.put_Height(unHeight);
	oFrame.put_Stride(4 * unWidth);

	pRasterRenderer->CreateFromBgraFrame(&oFrame);
	pRasterRenderer->SetSwapRGB(false);

	double dW_MM = unWidth * 25.4 / 96.;
	double dH_MM = unHeight * 25.4 / 96.;

	pRasterRenderer->put_Width(dW_MM);
	pRasterRenderer->put_Height(dH_MM);

	pRasterRenderer->put_PenColor(RGB_TO_INT(255, 0, 0));
	pRasterRenderer->put_PenSize(15);

	Aggplus::CMatrix oTransform(0.13338, 0, 0, 0.13395, 0, 0);

	const double dScale = 0.4;

	oTransform.Scale(dScale, dScale);

	pRasterRenderer->SetTransform(oTransform.sx(), oTransform.shy(), oTransform.shx(), oTransform.sy(), oTransform.tx(), oTransform.ty());

	// Рисуем многоугольник
	// Видим, что появля.тся практически черные точки (на самом деле это очень темный красный цвет)
	// Если же хотя бы чуть-чуть сместим местоположение отрисовки, то проблемные пиксели будут уже в других местах
	DrawEquilateralPolygon(pRasterRenderer, 1000, 410, 400, 6);
	// Если ещё что-то нарисуем, то некоторые проблемные места пропадут (у 6-угольника например нормализуются левая и правая стороны)
	// DrawLine(pRasterRenderer, 100, 0, 100, 300);

	oFrame.SaveFile(L"test-line-stroke.png", 4);

	RELEASEINTERFACE(pRasterRenderer);

	return 0;
}

#include "../../pro/Graphics.h"
#include "../../../raster/BgraFrame.h"
#include "../../../common/Directory.h"

#ifndef RGBA_TO_INT
#define RGBA_TO_INT(r, g, b, a) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) ) | ( ( (unsigned char)(a) ) << 24 ) )
#endif

struct TRectD
{
	double m_dX;
	double m_dY;
	double m_dWidth;
	double m_dHeight;
};

void StartDrawing(IRenderer* pRenderer)
{
	pRenderer->BeginCommand(c_nPathType);
	pRenderer->PathCommandStart();
}

void DrawRectangle(IRenderer* pRenderer, const TRectD& oRect)
{
	pRenderer->PathCommandMoveTo(oRect.m_dX, oRect.m_dY);
	pRenderer->PathCommandLineTo(oRect.m_dX + oRect.m_dWidth, oRect.m_dY);
	pRenderer->PathCommandLineTo(oRect.m_dX + oRect.m_dWidth, oRect.m_dY + oRect.m_dHeight);
	pRenderer->PathCommandLineTo(oRect.m_dX, oRect.m_dY + oRect.m_dHeight);
	pRenderer->PathCommandClose();
}

void EndDrawing(IRenderer* pRenderer)
{
	pRenderer->EndCommand(c_nPathType);
	pRenderer->DrawPath(c_nWindingFillMode);
	pRenderer->PathCommandEnd();
}

#define USETRANSFORM 0 // используем для трансформации Transform (1) или Bounds (0)

int main(int argc, char *argv[])
{
	NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();

	unsigned int unWidth  = 1000;
	unsigned int unHeight = 1000;

	// Создание основной картинки
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

	double dW_MM = unWidth;
	double dH_MM = unHeight;

	pRasterRenderer->put_Width(dW_MM);
	pRasterRenderer->put_Height(dH_MM);

	// Объявляем цвета градиента и их позиции
	const UINT unCountColors = 3;
	
	LONG *pColors = new LONG[unCountColors];
	double *pPositions = new double[unCountColors];
	
	pColors[0] = RGBA_TO_INT(255, 0, 0, 255);     // Красный
	pColors[1] = RGBA_TO_INT(255, 255, 255, 255); // Белый
	pColors[2] = RGBA_TO_INT(0, 0, 255, 255);     // Синий
	
	pPositions[0] = 0;
	pPositions[1] = 0.5;
	pPositions[2] = 1;
	
	pRasterRenderer->put_BrushType(c_BrushTypePathGradient1);
	pRasterRenderer->put_BrushGradientColors(pColors, pPositions, unCountColors);
	
	// Создадим 3 прямоугольника, которые будут заполнены градиентами
	TRectD oRect1{100, 100, 800, 200};
	TRectD oRect2{100, 400, 800, 200};
	TRectD oRect3{100, 700, 800, 200};
	
	// Отрисовываем первый прямоугольник (градиент без изменений)
	StartDrawing(pRasterRenderer);
	DrawRectangle(pRasterRenderer, oRect1);
	pRasterRenderer->BrushBounds(oRect1.m_dX, oRect1.m_dY, oRect1.m_dWidth, oRect1.m_dHeight);
	EndDrawing(pRasterRenderer);
	
	// Отрисовываем второй прямоугольник (сам градиент в 2 раза больше, а размер прямоугольника без изменений)
	StartDrawing(pRasterRenderer);
	DrawRectangle(pRasterRenderer, oRect2);
	const double dXScale = 2;
	const double dYScale = 2;
	#if USETRANSFORM == 1
	pRasterRenderer->put_BrushTransform(Aggplus::CMatrix(dXScale, 0., 0., dYScale, 0., 0.));
	pRasterRenderer->BrushBounds(oRect2.m_dX, oRect2.m_dY, oRect2.m_dWidth, oRect2.m_dHeight);
	#else
	pRasterRenderer->BrushBounds(oRect2.m_dX, oRect2.m_dY, oRect2.m_dWidth * dXScale, oRect2.m_dHeight * dYScale);
	#endif
	EndDrawing(pRasterRenderer);
	
	// Отрисовываем второй прямоугольник (сам градиент смещен вправо)
	StartDrawing(pRasterRenderer);
	DrawRectangle(pRasterRenderer, oRect3);
	const double dSkipX = 200;
	const double dSkipY = 0;
	#if USETRANSFORM == 1
	pRasterRenderer->put_BrushTransform(Aggplus::CMatrix(1., 0., 0., 1., dSkipX, dSkipY));
	pRasterRenderer->BrushBounds(oRect3.m_dX, oRect3.m_dY, oRect3.m_dWidth, oRect3.m_dHeight);
	#else
	pRasterRenderer->BrushBounds(oRect3.m_dX + dSkipX, oRect3.m_dY + dSkipY, oRect3.m_dWidth, oRect3.m_dHeight);
	#endif
	EndDrawing(pRasterRenderer);
	
	oFrame.SaveFile(L"testGradients.png", 4);

	delete [] pColors;
	delete [] pPositions;
	
	RELEASEINTERFACE(pRasterRenderer);

	return 0;
}

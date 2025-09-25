#include "../../pro/Graphics.h"
#include "../../../raster/BgraFrame.h"
#include "../../../common/Directory.h"

#define RGB_TO_INT(r, g, b) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) | ( (unsigned char)(0) << 24 ) ) )

int main(int argc, char *argv[])
{
	NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();

	const unsigned int unWidth  = 1000;
	const unsigned int unHeight = 1000;

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

	pRasterRenderer->BeginCommand(c_nPathType);
	pRasterRenderer->PathCommandStart();

	// Отрисовываем основной слой
	pRasterRenderer->PathCommandRect(50, 200, 600, 300);

	pRasterRenderer->put_BrushAlpha1(255);
	pRasterRenderer->put_BrushColor1(RGB_TO_INT(255, 0, 0)); // красный

	pRasterRenderer->put_PenAlpha(255);
	pRasterRenderer->put_PenColor(RGB_TO_INT(255, 0, 255));
	pRasterRenderer->put_PenSize(40);

	pRasterRenderer->DrawPath(c_nWindingFillMode | c_nStroke);

	pRasterRenderer->PathCommandEnd();
	pRasterRenderer->EndCommand(c_nPathType);

	// Отрисовываем второй слой
	pRasterRenderer->BeginCommand(c_nLayerType);

	pRasterRenderer->BeginCommand(c_nPathType);
	pRasterRenderer->PathCommandStart();

	pRasterRenderer->PathCommandRect(300, 300, 300, 300);

	pRasterRenderer->put_BrushAlpha1(255);
	pRasterRenderer->put_BrushColor1(RGB_TO_INT(0, 255, 0)); // зеленый

	pRasterRenderer->put_PenAlpha(150);
	pRasterRenderer->put_PenColor(RGB_TO_INT(0, 0, 255));
	pRasterRenderer->put_PenSize(40);

	pRasterRenderer->put_LayerOpacity(0.5);

	pRasterRenderer->DrawPath(c_nWindingFillMode | c_nStroke);

	pRasterRenderer->PathCommandEnd();
	pRasterRenderer->EndCommand(c_nPathType);

	//Отрисовываем третий слой
	pRasterRenderer->BeginCommand(c_nLayerType);

	pRasterRenderer->BeginCommand(c_nPathType);
	pRasterRenderer->PathCommandStart();

	pRasterRenderer->PathCommandRect(500, 400, 300, 300);

	pRasterRenderer->put_BrushAlpha1(150);
	pRasterRenderer->put_BrushColor1(RGB_TO_INT(0, 0, 255)); // синий

	pRasterRenderer->put_PenAlpha(255);
	pRasterRenderer->put_PenColor(RGB_TO_INT(255, 0, 0));
	pRasterRenderer->put_PenSize(40);

	pRasterRenderer->put_LayerOpacity(1);

	pRasterRenderer->DrawPath(c_nWindingFillMode | c_nStroke);

	pRasterRenderer->PathCommandEnd();
	pRasterRenderer->EndCommand(c_nPathType);

	// блитируем трутий слой на второй
	pRasterRenderer->EndCommand(c_nLayerType);

	// блитируем второй слой на основной
	pRasterRenderer->EndCommand(c_nLayerType);

	oFrame.SaveFile(L"testGraphicsLayers.png", 4);

	RELEASEINTERFACE(pRasterRenderer);

	return 0;
}

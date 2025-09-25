#include "../../pro/Graphics.h"
#include "../../../raster/BgraFrame.h"
#include "../../../common/Directory.h"
#include "../../AlphaMask.h"

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

	enum
	{
		GenerationMask,
		LoadMaskFromBuffer,
		LoadMaskFromFile
	} enMode = LoadMaskFromBuffer;

	switch (enMode)
	{
		case GenerationMask:
		{
			// Генерируем маску
			pRasterRenderer->BeginCommand(c_nMaskType);
			pRasterRenderer->PathCommandStart();

			pRasterRenderer->PathCommandMoveTo(0,    300);
			pRasterRenderer->PathCommandLineTo(1000, 300);
			pRasterRenderer->PathCommandLineTo(1000, 700);
			pRasterRenderer->PathCommandLineTo(0,    700);
			pRasterRenderer->PathCommandClose();

			// Задаем значения для пера и кисти маски
			pRasterRenderer->put_PenSize(100);
			pRasterRenderer->put_PenColor(16777215); // белый 16777215

			pRasterRenderer->put_BrushAlpha1(255);
			pRasterRenderer->put_BrushColor1(8355711); // серый

			pRasterRenderer->Stroke();
			pRasterRenderer->Fill();

			pRasterRenderer->EndCommand(c_nMaskType);
			pRasterRenderer->PathCommandEnd();

			break;
		}
		case LoadMaskFromBuffer:
		{
			Aggplus::CAlphaMask* pAlphaMask = new Aggplus::CAlphaMask();

			pAlphaMask->Create(unWidth, unHeight, Aggplus::EMaskDataType::AlphaBuffer);
			
			BYTE* pAlphaBuffer = pAlphaMask->GetBuffer();

			BYTE uchAlphaValue = 0;

			for (UINT unRow = 0; unRow < unHeight; ++unRow)
			{
				for (UINT unColumn = 0; unColumn < unWidth; ++unColumn)
					pAlphaBuffer[unRow * unWidth + unColumn] = uchAlphaValue;

				if (0 != unRow && 0 == unRow % 100)
					uchAlphaValue += 25;
			}

			pRasterRenderer->SetAlphaMask(pAlphaMask);

			pAlphaMask->Release();
			break;
		}
		case LoadMaskFromFile:
		{
			break;
		}
	}

	// Отрисовываем тестовый объект
	pRasterRenderer->BeginCommand(c_nPathType);
	pRasterRenderer->PathCommandStart();

//	pRasterRenderer->PathCommandMoveTo(400, 0);
//	pRasterRenderer->PathCommandLineTo(600, 0);
//	pRasterRenderer->PathCommandLineTo(600, 1000);
//	pRasterRenderer->PathCommandLineTo(400, 1000);
//	pRasterRenderer->PathCommandClose();

	pRasterRenderer->PathCommandMoveTo(300, 100);
	pRasterRenderer->PathCommandLineTo(700, 100);
	pRasterRenderer->PathCommandLineTo(900, 300);
	pRasterRenderer->PathCommandLineTo(900, 700);
	pRasterRenderer->PathCommandLineTo(700, 900);
	pRasterRenderer->PathCommandLineTo(300, 900);
	pRasterRenderer->PathCommandLineTo(100, 700);
	pRasterRenderer->PathCommandLineTo(100, 300);
	pRasterRenderer->PathCommandClose();

	// Задаем значения для пера и кисти
	pRasterRenderer->put_PenSize(100);
	pRasterRenderer->put_PenColor(255); // красный

	pRasterRenderer->put_BrushAlpha1(255);
	pRasterRenderer->put_BrushColor1(65280); // зеленый

	pRasterRenderer->Stroke();
	pRasterRenderer->Fill();

	//Сбрасываем маску
	pRasterRenderer->BeginCommand(c_nResetMaskType);
	pRasterRenderer->EndCommand(c_nResetMaskType);

	pRasterRenderer->PathCommandEnd();
	pRasterRenderer->EndCommand(c_nPathType);

	oFrame.SaveFile(L"testAlphaMask.png", 4);

	RELEASEINTERFACE(pRasterRenderer);

	return 0;
}

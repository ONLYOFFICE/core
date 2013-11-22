#pragma once
#include "..\stdafx.h"

#include "..\Objects\AVSGraphics.h"
#include "TimeMeasurer.h"
#include "..\agg\Aggplus.h"

class CTester
{
private:
	LONG m_lWidthPix;
	LONG m_lHeightPix;

	LONG m_lStridePix;
	LONG m_lPitchPix;

	LONG m_lCount;

	MediaCore::IAVSUncompressedVideoFrame* m_pImage;

public:

	CTester()
	{
		m_lWidthPix			= 1000;
		m_lHeightPix		= 1000;

		m_lStridePix		= 4 * m_lWidthPix;
		m_lPitchPix			= 0;

		m_lCount			= 100;

		m_pImage = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_INPROC, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pImage);

		m_pImage->put_ColorSpace(64);
		m_pImage->put_Width(m_lWidthPix);
		m_pImage->put_Height(m_lHeightPix);
		m_pImage->SetDefaultStrides();
		m_pImage->AllocateBuffer(-1);

		BYTE* pPixels = NULL;
		m_pImage->get_Buffer(&pPixels);
		memset(pPixels, 255, m_lStridePix * m_lHeightPix);
	}

	~CTester()
	{
		RELEASEINTERFACE(m_pImage);
	}

	float GDI_DRAW()
	{
		BYTE* pPixels = NULL;
		m_pImage->get_Buffer(&pPixels);

		Gdiplus::Bitmap oBitmap(m_lWidthPix, m_lHeightPix, m_lStridePix, PixelFormat32bppARGB, pPixels);

		Gdiplus::Graphics oGr(&oBitmap);

		oGr.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		oGr.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		Gdiplus::SolidBrush oBrush(Gdiplus::Color::Red);
		Gdiplus::Pen oPen(Gdiplus::Color::Red, 2);

		oGr.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

		Gdiplus::Font oFont(L"Arial", 30);
		Gdiplus::PointF point(800, 800);

		//Gdiplus::Bitmap oImage(L"C:\\Documents and Settings\\Oleg.Korshul.AVSMEDIA\\Desktop\\Images\\121.jpg");

		CTimeMeasurer oTimer;
		oTimer.Reset();

		
		/*for (int i = 0; i < m_lCount; ++i)
		{
			Gdiplus::Bitmap oImage(L"C:\\Documents and Settings\\Oleg.Korshul.AVSMEDIA\\Desktop\\Images\\121.bmp");
			Gdiplus::BitmapData oData;

			oImage.LockBits(NULL, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &oData);
		}*/

		oGr.DrawString(L"Hellow", 6, &oFont, point, &oBrush);

		//for (int i = 0; i < m_lCount; ++i)
		//{
		//	Gdiplus::GraphicsPath oPath;
		//	oPath.AddEllipse(500, 500, 200, 200);
		//	oGr.FillPath(&oBrush, &oPath);
		//	
		//	//oGr.DrawLine(&oPen, 500, 500, 700, 700);
		//}

		/*Gdiplus::Bitmap oImage(L"C:\\Documents and Settings\\Oleg.Korshul.AVSMEDIA\\Desktop\\Images\\1.gif");
		oGr.DrawImage(&oImage, 500, 500, 200, 200);*/

		float fTime = oTimer.GetTimeInterval();
		return fTime;
	}

	float NEW_DRAW()
	{
		BYTE* pPixels = NULL;
		m_pImage->get_Buffer(&pPixels);

		//// agg
		/*Aggplus::Bitmap oBitmap((Aggplus::INT)m_lWidthPix, (Aggplus::INT)m_lHeightPix, (Aggplus::INT)m_lStridePix, PixelFormat32bppARGB, pPixels);
		Aggplus::Graphics oGr(&oBitmap);

		Aggplus::Color oColor(0, 0, 255);
		Aggplus::SolidBrush oBrush(oColor);
		Aggplus::Pen oPen(oColor, 2);

		Aggplus::Font oFont(L"Vivaldi", 30);

		Aggplus::PointF point(400, 400);
		oGr.DrawString(L"Hellow", 6, &oFont, point, &oBrush);

		CTimeMeasurer oTimer;
		oTimer.Reset();*/
		
		//for (int i = 0; i < m_lCount; ++i)
		//{
		//	Aggplus::GraphicsPath oPath;
		//	oPath.AddEllipse(100, 100, 200, 200);
		//	oGr.FillPath(&oBrush, &oPath);
		//	//oGr.FillEllipse(&oBrush, 100, 100, 200, 200);
		//	//oGr.DrawLine(&oPen, 100, 100, 300, 300);
		//}

		/*for (int i = 0; i < m_lCount; ++i)
		{
			Aggplus::Image oImage(L"C:\\Documents and Settings\\Oleg.Korshul.AVSMEDIA\\Desktop\\Images\\121.bmp");
		}*/

		/*Aggplus::Image oImage(L"C:\\Documents and Settings\\Oleg.Korshul.AVSMEDIA\\Desktop\\Images\\1.gif");
		oGr.DrawImage(&oImage, 100, 100, 200, 200);*/

		/*float fTime = oTimer.GetTimeInterval();
		return fTime;*/

		IAVSGraphics* pGraphics = NULL;
		CoCreateInstance(__uuidof(CAVSGraphics), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphics), (void**)&pGraphics);

		pGraphics->Create(pPixels, m_lWidthPix, m_lHeightPix, m_lStridePix, m_lPitchPix, 5);
		
		IAVSBrush* pBrush = NULL;
		CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSBrush), (void**)&pBrush);

		pBrush->put_Color1(0xFFFF0000);

		IAVSFontManager* pFont = NULL;
		CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&pFont);

		pFont->Initialize(L"");
		pFont->LoadFontByName(L"Vivaldi", 30, 0, 96, 96);

		CTimeMeasurer oTimer;
		oTimer.Reset();

		///*for (int i = 0; i < m_lCount; ++i)
		//{
		//	pGraphics->FillEllipse(pBrush, 600, 600, 200, 200);
		//}*/

		pGraphics->DrawString(L"Hellow", pFont, pBrush, 0, 500);

		float fTime = oTimer.GetTimeInterval();
		
		RELEASEINTERFACE(pBrush);
		RELEASEINTERFACE(pGraphics);
		RELEASEINTERFACE(pFont);

		return fTime;
	}

	void Test()
	{
		float t1 = GDI_DRAW();
		float t2 = NEW_DRAW();


		FILE* f = fopen("c:\\graphics.txt", "a+");
		fprintf(f, "count = %d: gdi/new: %lf/%lf\n", m_lCount, (double)t1, (double)t2);
		fclose(f);

		Save();
	}

	void Save()
	{
		ImageStudio::IImageTransforms* pTransform = NULL;
		CoCreateInstance( ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)(&pTransform));

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = (IUnknown*)m_pImage;

		VARIANT_BOOL vbRes=VARIANT_TRUE;
		pTransform->SetSource(0, var);
		pTransform->SetXml(L"<transforms><ImageFile-SaveAsBmp destinationpath='c:\\out.bmp' format='888' quality='100'></ImageFile-SaveAsBmp></transforms>", &vbRes);
		pTransform->Transform(&vbRes);

		RELEASEINTERFACE(pTransform);
	}
};
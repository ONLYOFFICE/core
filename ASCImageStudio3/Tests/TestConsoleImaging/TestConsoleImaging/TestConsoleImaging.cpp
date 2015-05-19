#include "stdafx.h"

#define RTC_RESIZE      0x00000000
#define RTC_SHRINK      0x00000100
#define RTC_NEAREST     0x00000000
#define RTC_BILLINEAR   0x00000001
#define RTC_BICUBIC     0x00000010
#define RTC_CROPENABLE  0x00001000

class CApplication
{
	ImageFile::IImageFile3* m_pFile;
	ImageTransform::IImageTransform3* m_pTransform;

	SAFEARRAY* m_pImageSource;
	SAFEARRAY* m_pImageResult;
	SAFEARRAY* m_pImageDraw;
	
	IUnknown* m_pIImageSource;
	IUnknown* m_pIImageResult;
	IUnknown* m_pIImageDraw;
	
public:
	
	CApplication()
	{
		CoInitialize(NULL);

		m_pFile = NULL;
		m_pTransform = NULL;

		m_pImageSource = NULL;
		m_pImageResult = NULL;
		m_pImageDraw = NULL;

		m_pIImageSource = NULL;
		m_pIImageResult = NULL;
		m_pIImageDraw = NULL;
	}
	~CApplication()
	{
		Destroy();

		CoUninitialize();
	}
	
	void Destroy()
	{
		if (m_pFile != NULL)
		{
			m_pFile->Release();
			m_pFile = NULL;
		}
		if (m_pTransform != NULL)
		{
			m_pTransform->Release();
			m_pTransform = NULL;
		}
		if (m_pImageSource != NULL)
		{
			SafeArrayDestroy(m_pImageSource);
			m_pImageSource = NULL;
		}
		if (m_pImageResult != NULL)
		{
			SafeArrayDestroy(m_pImageResult);
			m_pImageResult = NULL;
		}
		if (m_pImageDraw != NULL)
		{
			SafeArrayDestroy(m_pImageDraw);
			m_pImageDraw = NULL;
		}

		if (m_pIImageSource != NULL)
		{
			m_pIImageSource->Release();
			m_pIImageSource = NULL;
		}
		if (m_pIImageResult != NULL)
		{
			m_pIImageResult->Release();
			m_pIImageResult = NULL;
		}
		if (m_pIImageDraw != NULL)
		{
			m_pIImageDraw->Release();
			m_pIImageDraw = NULL;
		}
	}

	BOOL Create()
	{
		Destroy();

		if (S_OK != CoCreateInstance(ImageFile::CLSID_ImageFile3, NULL, CLSCTX_INPROC, ImageFile::IID_IImageFile3, (void**)&m_pFile))
			m_pFile = NULL;

		if (S_OK != CoCreateInstance(ImageTransform::CLSID_ImageTransform3, NULL, CLSCTX_INPROC, ImageTransform::IID_IImageTransform3, (void**)&m_pTransform))
			m_pTransform = NULL;

		if (m_pFile == NULL || m_pTransform == NULL)
			return FALSE;

		return TRUE;
	}
	
	BOOL LoadImage(BSTR bstrFilePath)
	{
		if (m_pFile == NULL)
			return FALSE;

		if (m_pImageSource != NULL)
		{
			SafeArrayDestroy(m_pImageSource);
			m_pImageSource = NULL;
		}
		if (m_pIImageSource != NULL)
		{
			m_pIImageSource->Release();
			m_pIImageSource = NULL;
		}

		VARIANT_BOOL bSuccess = VARIANT_FALSE;
		
		m_pFile->LoadImage(bstrFilePath, &m_pImageSource, &bSuccess);

		for (int index = 0; index < 1000; ++index)
		{
			m_pIImageSource = NULL;
			m_pFile->LoadImage2(bstrFilePath, &m_pIImageSource, &bSuccess);
			
			IUnknown* pImg = m_pIImageSource;

			m_pFile->SaveImage2(&m_pIImageSource, 2, L"c:\\out.jpg", &bSuccess);

			pImg->Release();
			m_pIImageSource->Release();
		}

		return ((bSuccess == VARIANT_TRUE) && (m_pImageSource != NULL));
	}
	BOOL ResizeImage(int nWidth, int nHeight, int nBackColor, int nType)
	{
		if (m_pTransform == NULL)
			return FALSE;

		if (m_pImageSource == NULL || nWidth < 2 || nHeight < 2)
			return FALSE;

		if (m_pImageResult != NULL)
		{
			SafeArrayDestroy(m_pImageResult);
			m_pImageResult = NULL;
		}

		SAFEARRAYBOUND arrBounds[3];
		arrBounds[0].lLbound = 0;
		arrBounds[0].cElements = 4;
		arrBounds[1].lLbound = 0;
		arrBounds[1].cElements = nWidth;
		arrBounds[2].lLbound = 0;
		arrBounds[2].cElements = nHeight;

		m_pImageResult = SafeArrayCreate(VT_UI1, 3, arrBounds);
		if (!m_pImageResult)
			return FALSE;

		m_pTransform->SetSafeArrayPtr((BYTE*)m_pImageSource);

		m_pTransform->TransformResize(&m_pImageResult, nBackColor, nType);

		return (m_pImageResult != NULL);
 	}
	BOOL DrawImage(BSTR bstrFilePath, int nLeft, int nTop, int nBackColor, int nAlpha)
	{
		// TODO: draw image will not work in console app due to gdi+ initialization (gdi+ requires window)
		if (m_pFile == NULL)
			return FALSE;

		if (m_pImageDraw != NULL)
		{
			SafeArrayDestroy(m_pImageDraw);
			m_pImageDraw = NULL;
		}

		VARIANT_BOOL bSuccess = VARIANT_FALSE;
		
		m_pFile->LoadImage(bstrFilePath, &m_pImageDraw, &bSuccess);

		if ((bSuccess == VARIANT_FALSE) || (m_pImageDraw == NULL))
			return FALSE;

		m_pTransform->DrawImage(1.0, nLeft, nTop, &m_pImageDraw, nBackColor, nAlpha);

		return TRUE;
	}
	BOOL SaveImage(BSTR bstrFilePath)
	{
		if (m_pFile == NULL)
			return FALSE;
		if (m_pImageResult == NULL)
			return FALSE;

		VARIANT_BOOL bSuccess = VARIANT_FALSE;

		m_pFile->SaveAsBmp(&m_pImageResult, bstrFilePath, &bSuccess);

		return (bSuccess == VARIANT_TRUE);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		printf("Usage: <application> <source file path> <destinatio file path>\n");
		return 1;
	}

	CApplication oApplication;
	if (!oApplication.Create())
	{
		printf("Failed to create necessary ImageFile3 and ImageTransform3 interfaces\n");
		return 1;
	}

	_bstr_t bstrSource; bstrSource = argv[1];
	_bstr_t bstrResult; bstrResult = argv[2];

	if (!oApplication.LoadImage(bstrSource))
	{
		printf("Failed to load image from specified path: ");
		printf(argv[1]);
		printf("\n");
		return 1;
	}


	if (!oApplication.ResizeImage(300, 200, 255, RTC_RESIZE | RTC_NEAREST))
	{
		printf("Failed to resize source image with specified size and type\n");
		return 1;
	}

	if (!oApplication.DrawImage(L"c:\\in1.bmp", 50, 40, 255, 128))
	{
		printf("Failed to resize source image with specified size and type\n");
		return 1;
	}

	if (!oApplication.SaveImage(bstrResult))
	{
		printf("Failed to save image to specified path: ");
		printf(argv[2]);
		printf("\n");
		return 1;
	}

	return 0;
}


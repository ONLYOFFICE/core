#pragma once

#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

/*
	По простому (когда инициализация в конструкторе CGdiPlusInit()) сделать 
	не получилось, поскольку конструктор вызывается до InitInstance приложения
	а GDI+ создает свое какое-то окно прятанное, и DDE сообщения валятся туда
	вместо того чтобы дождаться пока создадим MainFrame. И через это если 
	открывали чертеж из проводника двойным кликом, а ни одного экзмпляра 
	приложения еще не было загружено, проводник орал и файл не открывал.
	Поэтому пришлось добавить процедуру Init() и вызывать ее после того как
	MainFrame уже создан.

	ВАЖНО!!!! Не надо переделывать не подумав, этот глюк отлавливался 3 часа.
*/

class CGdiPlusInit  
{
public:

	CGdiPlusInit()
	{
		m_bPresent = FALSE;
		m_nToken = 0;
	}
	virtual ~CGdiPlusInit()
	{
		if (m_bPresent) 
			Gdiplus::GdiplusShutdown(m_nToken);
	}
	BOOL Init()
	{
		Gdiplus::GdiplusStartupInput pInput;

		try
		{
			Gdiplus::GdiplusStartup(&m_nToken, &pInput, 0);

			m_bPresent = TRUE;
		}
		catch (...)
		{
			m_bPresent = FALSE;
		}

		return m_bPresent;
	}
	BOOL Good()
	{
		return m_bPresent;
	}
	BOOL IsValid()
	{
		return m_bPresent;
	}

private:

	BOOL m_bPresent;
	ULONG_PTR m_nToken;
};
	
namespace GdiPlusEx
{
	class CGdiPlusObject
	{
	protected:
	
		void* m_pGdiPlusObject; // pointer to a GDI+ graphic object
	
	public:

		CGdiPlusObject()
		{
			m_pGdiPlusObject = NULL;
		}
		virtual ~CGdiPlusObject()
		{
		}
		
		virtual BOOL IsValid()
		{
			if (m_pGdiPlusObject)
				return TRUE;

			return FALSE;
		}
	};
	class CGdiPlusImage : public CGdiPlusObject
	{
	protected:
	
		BYTE* m_pDataCopy;			// byte storage for the image pixel data (for copied image)
		BYTE* m_pDataAttach;		// byte storage for the image pixel data (for attached image)
		HGLOBAL m_hDataCopy;		// handle storage for the image pixel data (for copied image)
	
	public:
		
		CGdiPlusImage()
		{
			m_pDataCopy = NULL;
			m_pDataAttach = NULL;
			m_hDataCopy = NULL;

		}
		~CGdiPlusImage()
		{
			Destroy();
		}

		virtual void Destroy()
		{
			// destroy the GDI+ object first
			if (m_pGdiPlusObject)
			{
				Gdiplus::Image* pGdiPlusImage = (Gdiplus::Image*)m_pGdiPlusObject;

				if (pGdiPlusImage)
					delete pGdiPlusImage;

				m_pGdiPlusObject = NULL;
			}

			// release pixel data
			if (m_pDataCopy)
			{
				delete[] m_pDataCopy;
				m_pDataCopy = NULL;
			}

			// release pixel data
			if (m_hDataCopy)
			{
				GlobalFree(m_hDataCopy);
				m_hDataCopy = NULL;
			}

			m_pDataAttach = NULL;
		}
		
		operator Gdiplus::Image*()
		{
			return (Gdiplus::Image*)m_pGdiPlusObject;
		}
		operator Gdiplus::Bitmap*()
		{
			return (Gdiplus::Bitmap*)m_pGdiPlusObject;
		}
		Gdiplus::Image* operator= (Gdiplus::Image* pImage)
		{
			Destroy();

			m_pGdiPlusObject = (void*)pImage;

			return (Gdiplus::Image*)m_pGdiPlusObject;
		}
		Gdiplus::Bitmap* operator= (Gdiplus::Bitmap* pBitmap)
		{
			Destroy();

			m_pGdiPlusObject = (void*)pBitmap;

			return (Gdiplus::Bitmap*)m_pGdiPlusObject;
		}
		
		int GetWidth()
		{
			if (!IsValid())
				return 0;
			
			return ((Gdiplus::Bitmap*)m_pGdiPlusObject)->GetWidth();
		}
		int GetHeight()
		{
			if (!IsValid())
				return 0;
			
			return ((Gdiplus::Bitmap*)m_pGdiPlusObject)->GetHeight();
		}
		
		BOOL CreateFromFile(BSTR FilePath, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL )
		{
			Destroy();

			try
			{
				/* read through stream
				CString s; s = FilePath;
				FILE* pFile = fopen(s, "rb");
				fseek(pFile, 0, SEEK_END);
				int nSize = ftell(pFile);
				fseek(pFile, 0, SEEK_SET);
				BYTE* pBytes = new BYTE[nSize];
				fread(pBytes, nSize, 1, pFile);
				CreateFromStream(pBytes, nSize);
				delete[] pBytes;
				fclose(pFile);

				return TRUE;
				*/
				
				Bitmap* pBitmap = new Bitmap(FilePath);

				if (pBitmap && pBitmap->GetLastStatus() == Ok)
				{
					GUID format;

					// retrieve image format
					pBitmap->GetRawFormat(&format);

					if( nFrame > 0 )
					{
						// variables
						long nFrameCount = 0;

						// check for format type
						if (format == ImageFormatTIFF)
							nFrameCount = pBitmap->GetFrameCount(&FrameDimensionPage);
						else if (format == ImageFormatGIF)
							nFrameCount = pBitmap->GetFrameCount(&FrameDimensionTime);

						// check for frame count
						if (nFrame < nFrameCount)
						{
							// select necessary frame
							if (format == ImageFormatTIFF)
								pBitmap->SelectActiveFrame(&FrameDimensionPage, nFrame);
							else if (format == ImageFormatGIF)
								pBitmap->SelectActiveFrame(&FrameDimensionTime, nFrame);
						}
					}
					
					m_pGdiPlusObject = (void*)pBitmap;

					if( pHorResolution )
						*pHorResolution = pBitmap->GetHorizontalResolution();

					if( pVerResolution )
						*pVerResolution = pBitmap->GetVerticalResolution();

					return TRUE;
				}
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
		BOOL CreateFromSafeArray(SAFEARRAY** AvsArray, BOOL CreateCopy)
		{
			if (!AvsArray || !*AvsArray)
				return FALSE;

			try
			{
				int nWidth = (*AvsArray)->rgsabound[1].cElements;
				int nHeight = (*AvsArray)->rgsabound[0].cElements;

				return CreateFromByteArray((BYTE*)((*AvsArray)->pvData), nWidth, nHeight, CreateCopy); 
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
		BOOL CreateFromByteArray(BYTE* ByteArray, long Width, long Height, BOOL CreateCopy)
		{
			Destroy();

			if (!ByteArray || Width < 1 || Height < 1)
				return FALSE;

			try
			{
				Destroy();

				Bitmap* pBitmap = NULL;

				if (CreateCopy)
				{
					m_pDataCopy = new BYTE[4*Width*Height];

					memcpy(m_pDataCopy, ByteArray, 4*Width*Height);

					pBitmap = new Bitmap(Width, Height, 4*Width, PixelFormat32bppRGB, m_pDataCopy);
				}
				else
				{
					m_pDataAttach = ByteArray;

					pBitmap = new Bitmap(Width, Height, 4*Width, PixelFormat32bppRGB, m_pDataAttach);
				}

				if (pBitmap && pBitmap->GetLastStatus() == Ok)
				{
					m_pGdiPlusObject = (void*)pBitmap;

					return TRUE;
				}
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
		BOOL CreateFromByteArray(BYTE* ByteArray, long Width, long Height, int Stride, int Format, BOOL CreateCopy)
		{
			Destroy();

			if (!ByteArray || Width < 1 || Height < 1)
				return FALSE;

			try
			{
				Destroy();

				Bitmap* pBitmap = NULL;

				if (CreateCopy)
				{
					m_pDataCopy = new BYTE[Stride*Height];

					memcpy(m_pDataCopy, ByteArray, Stride*Height);

					pBitmap = new Bitmap(Width, Height, Stride, Format, m_pDataCopy);
				}
				else
				{
					m_pDataAttach = ByteArray;

					pBitmap = new Bitmap(Width, Height, Stride, Format, m_pDataAttach);
				}

				if (pBitmap && pBitmap->GetLastStatus() == Ok)
				{
					m_pGdiPlusObject = (void*)pBitmap;

					return TRUE;
				}
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
		BOOL CreateFromStream(BYTE* ByteArray, long Size)
		{
			Destroy();

			if (!ByteArray || Size < 1)
				return FALSE;

			try
			{
				// create global memory for stream with compressed image within
				m_hDataCopy = GlobalAlloc(GMEM_FIXED, Size);
				if (!m_hDataCopy)
				{
					Destroy();

					return FALSE;
				}

				// put the image into a global memory
				LPVOID pImageGlobalPtr = GlobalLock(m_hDataCopy);
				if (!pImageGlobalPtr)
				{
					Destroy();

					return FALSE;
				}
				memcpy(pImageGlobalPtr, ByteArray, Size);
				GlobalUnlock(m_hDataCopy);

				// create stream with the image
				LPSTREAM pImageStream = NULL;
				if (FAILED(CreateStreamOnHGlobal(m_hDataCopy, FALSE, &pImageStream)))
				{
					Destroy();

					return FALSE;
				}

				// create gdi plus image on this stream
				Bitmap* pBitmap = new Bitmap(pImageStream);

				pImageStream->Release();

				if (pBitmap && pBitmap->GetLastStatus() == Ok)
				{
					m_pGdiPlusObject = (void*)pBitmap;

					return TRUE;
				}
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
			
		BOOL SaveToSafeArray(SAFEARRAY*& pArray)
		{
			if (!IsValid())
				return FALSE;

			int nWidth = GetWidth();
			int nHeight = GetHeight();
			Gdiplus::Bitmap* pBitmap = (Gdiplus::Bitmap*)m_pGdiPlusObject;

			SAFEARRAYBOUND bounds[3];
			BitmapData oLockData;
			Rect oLockRect(0, 0, nWidth, nHeight);
			
			bounds[0].lLbound = 0;
			bounds[0].cElements = 4;
			bounds[1].lLbound = 0;
			bounds[1].cElements = nWidth;
			bounds[2].lLbound = 0;
			bounds[2].cElements = nHeight;

			pArray = SafeArrayCreate(VT_UI1, 3, bounds);

			if (!pArray)
				return FALSE;

			if (pBitmap->LockBits(&oLockRect, ImageLockModeRead, PixelFormat32bppARGB, &oLockData) == Ok)
			{
				memcpy(pArray->pvData, oLockData.Scan0, 4*nWidth*nHeight);
			}

			if (pBitmap->GetPixelFormat() == PixelFormat1bppIndexed ||
				pBitmap->GetPixelFormat() == PixelFormat4bppIndexed  ||
				pBitmap->GetPixelFormat() == PixelFormat16bppGrayScale ||
				pBitmap->GetPixelFormat() == PixelFormat16bppRGB555 ||
				pBitmap->GetPixelFormat() == PixelFormat16bppRGB565 ||
				pBitmap->GetPixelFormat() == PixelFormat24bppRGB ||
				pBitmap->GetPixelFormat() == PixelFormat32bppRGB )
			{
				BYTE* pData = (BYTE*)(pArray->pvData) + 3;

				for (int index = 0; index < 4*nWidth*nHeight; ++index)
					*pData = 255;
			}

			return TRUE;
		}
	};
	class CGdiPlusGraphics : public CGdiPlusObject
	{
	public:
	
		~CGdiPlusGraphics()
		{
			Destroy();
		}

		virtual void Destroy()
		{
			// destroy the GDI+ object first
			if (m_pGdiPlusObject)
			{
				Gdiplus::Graphics* pGdiPlusGraphics = (Gdiplus::Graphics*)m_pGdiPlusObject;

				if (pGdiPlusGraphics)
					delete pGdiPlusGraphics;

				m_pGdiPlusObject = NULL;
			}
		}
	
		operator Gdiplus::Graphics*()
		{
			return (Gdiplus::Graphics*)m_pGdiPlusObject;
		}
		Gdiplus::Graphics* operator= (Gdiplus::Graphics* pGraphics)
		{
			Destroy();

			m_pGdiPlusObject = (void*)pGraphics;

			return (Gdiplus::Graphics*)m_pGdiPlusObject;
		}
	
		BOOL CreateFromImage(Gdiplus::Image* pImage)
		{
			if (!pImage)
				return FALSE;

			Destroy();

			try
			{

				Graphics* pGraphics = Graphics::FromImage(pImage);

				if (pGraphics && pGraphics->GetLastStatus() == Ok)
				{
					m_pGdiPlusObject = (void*)pGraphics;

					return TRUE;
				}
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
		BOOL CreateFromImage(GdiPlusEx::CGdiPlusImage* pImage)
		{
			if (!pImage)
				return FALSE;

			Destroy();

			try
			{
				Bitmap* pBitmap = (Bitmap*)(*pImage);

				if (pBitmap && pBitmap->GetLastStatus() == Ok)
					return CreateFromImage(pBitmap);
			}
			catch (...)
			{
			}

			Destroy();

			return FALSE;
		}
	};
}

#pragma once

#include <ImageCaching.h>

namespace ImageStudio
{
	namespace Paint
	{
		namespace Effects
		{
			namespace Constants
			{
				const int c_nArtefactsCount = 8;
				
				const int c_arArtefactsWidth[8] =  { 4, 14, 13, 11, 17, 12,  6, 15};
				const int c_arArtefactsHeight[8] = {20, 19, 16, 17, 15, 14, 10, 16};

				// заготовка для артефакта
				//const BYTE c_arArtefact[20][20] = { 
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				//};

				 
				const BYTE c_arArtefact1[20][4] = { 
					0, 1, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 1,
					0, 0, 1, 1,
					0, 0, 0, 1,
					0, 0, 0, 1,
					0, 0, 1, 1,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 0, 1, 0,
					0, 1, 1, 0,
					0, 1, 0, 0,
					0, 1, 0, 1,
					0, 0, 1, 1
				};
				
				const BYTE c_arArtefact2[19][14] = { 
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
					0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
					0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
				};
				
				const BYTE c_arArtefact3[16][13] = { 
					0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
					0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
				};

				const BYTE c_arArtefact4[17][11] = { 
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
					0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
					0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
				};

				const BYTE c_arArtefact5[15][17] = { 
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
					0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
					0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
				};

				const BYTE c_arArtefact6[14][12] = { 
					0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
					0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
					0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
					0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				};

				const BYTE c_arArtefact7[10][6] = { 
					0, 0, 0, 1, 0, 0,
					0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 1,
					0, 0, 0, 0, 1, 1,
					0, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 0, 0,
					1, 1, 1, 1, 1, 0,
					1, 1, 1, 1, 1, 0,
					1, 1, 1, 1, 1, 0,
					0, 1, 1, 1, 0, 0,
				};

				const BYTE c_arArtefact8[16][15] = { 
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
					0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
					0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
				};


			}

			
			class Cinema : public ImageStudio::Caching::CCacheItemPointer
			{
			protected:
				
				enum EArtefactCounts
				{
					c_nCountFibers = 14,
					c_nCountSpots = 5,
					c_nCountDirts = 1
				};

				DWORD m_dwStartRnd;

				double m_dGrayFactor;
				double m_dNoiseFactor;

				Gdiplus::Rect m_arrFibers[c_nCountFibers];
				Gdiplus::Rect m_arrSpots[c_nCountSpots];
				Gdiplus::Rect m_arrDirts[c_nCountDirts];

				(Gdiplus::Bitmap*) m_arpArtefactImage[Constants::c_nArtefactsCount];


				int    m_nShiftBaseRnd;
				//double m_dShiftPeriod;
				double m_dShiftOffsetX;
				double m_dShiftOffsetY;
				double m_dShiftFactorX;
				double m_dShiftFactorY;

				double m_dLastShiftX; // эти две переменные нужны для того чтобы не было 
				double m_dLastShiftY; // эффекта резкого дерганья, чтобы оно было плавным

				int m_nMaxArtefactsShow;
				int m_nMinArtefactsShow;

				int m_nMinLinesShow;
				int m_nMaxLinesShow;

				int m_arLinesX[50];
				int m_arLinesCurLife[50];
				int m_nLinesCurCount;
				int m_nMaxFramesLineShow;
			
			public:
				//Cinema( CString sFileWithArtefacts )
				//{
				//	LARGE_INTEGER liTime;
				//	
				//	if( ::QueryPerformanceCounter( &liTime ))
				//		m_dwStartRnd = liTime.LowPart;
				//	else
				//		m_dwStartRnd = DWORD(::rand());

				//	//m_dwStartRnd *= 317539;

				//	m_dGrayFactor  = 0.1;
				//	m_dNoiseFactor = 0.2;

				//	m_pArtefactsImage = NULL;

				//	// загрузка артефактов
				//	_bstr_t bsFile = sFileWithArtefacts.AllocSysString();
				//	Gdiplus::Bitmap oSrcImage( bsFile );
				//	SysFreeString( bsFile );
				//	if( Gdiplus::Ok == oSrcImage.GetLastStatus() )
				//	{
				//		int nWidth = oSrcImage.GetWidth();
				//		int nHeight = oSrcImage.GetHeight();

				//		if( nWidth >= 256 && nHeight >= 320 )
				//		{
				//			m_pArtefactsImage = oSrcImage.Clone( 0, 0, nWidth, nHeight, PixelFormat32bppARGB );
				//			if( m_pArtefactsImage )
				//			{
				//				m_arrFibers[0]  = Gdiplus::Rect( 66,  2,   60, 60 );
				//				m_arrFibers[1]  = Gdiplus::Rect( 130, 2,   60, 60 );
				//				m_arrFibers[2]  = Gdiplus::Rect( 2,   66,  60, 60 );
				//				m_arrFibers[3]  = Gdiplus::Rect( 66,  66,  60, 60 );
				//				m_arrFibers[4]  = Gdiplus::Rect( 130, 66,  60, 60 );
				//				m_arrFibers[5]  = Gdiplus::Rect( 194, 66,  60, 60 );
				//				m_arrFibers[6]  = Gdiplus::Rect( 2,   130, 60, 60 );
				//				m_arrFibers[7]  = Gdiplus::Rect( 66,  130, 60, 60 );
				//				m_arrFibers[8]  = Gdiplus::Rect( 130, 130, 60, 60 );
				//				m_arrFibers[9]  = Gdiplus::Rect( 194, 130, 60, 60 );
				//				m_arrFibers[10] = Gdiplus::Rect( 2,   194, 60, 60 );
				//				m_arrFibers[11] = Gdiplus::Rect( 66,  194, 60, 60 );
				//				m_arrFibers[12] = Gdiplus::Rect( 130, 194, 60, 60 );
				//				m_arrFibers[13] = Gdiplus::Rect( 194, 194, 60, 60 );

				//				m_arrSpots[0] = Gdiplus::Rect( 2,   2,   60, 60 );
				//				m_arrSpots[1] = Gdiplus::Rect( 194, 2,   60, 60 );
				//				m_arrSpots[2] = Gdiplus::Rect( 2,   258, 60, 60 );
				//				m_arrSpots[3] = Gdiplus::Rect( 130, 258, 60, 60 );
				//				m_arrSpots[4] = Gdiplus::Rect( 194, 258, 60, 60 );

				//				m_arrDirts[0] = Gdiplus::Rect( 66, 258, 60, 60 );
				//			
				//				// делаем нужную альфу в картинке
				//				Gdiplus::BitmapData oImageData;
				//				m_pArtefactsImage->LockBits( NULL, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &oImageData );
				//				if( oImageData.Scan0 )
				//				{
				//					BYTE* pRow = LPBYTE(oImageData.Scan0);
				//					for( int nRow = 0; nRow < nHeight; ++nRow, pRow += oImageData.Stride )
				//					{
				//						BYTE* pPixel = pRow;
				//						for( int nPos = 0; nPos < nWidth; ++nPos, pPixel += 4 )
				//						{
				//							pPixel[3] = pPixel[0];
				//						}
				//					}
				//					
				//					m_pArtefactsImage->UnlockBits( &oImageData );
				//				}
				//			}
				//		}
				//	}
				//}

				Cinema( )
				{
					LARGE_INTEGER liTime;
					
					if( ::QueryPerformanceCounter( &liTime ))
						m_dwStartRnd = liTime.LowPart;
					else
						m_dwStartRnd = DWORD(::rand());

					//m_dwStartRnd *= 317539;

					m_dGrayFactor  = 0.1;
					m_dNoiseFactor = 0.2;

					m_nLinesCurCount = 0;

					// загрузка артефактов
					for (int nCurArt = 0; nCurArt < Constants::c_nArtefactsCount; nCurArt++)
					{
						Gdiplus::Bitmap* pCurImage;

						const int nHeight = Constants::c_arArtefactsHeight[nCurArt];
						const int nWidth  = Constants::c_arArtefactsWidth[nCurArt];
						
						BYTE* pResult = new BYTE[(nWidth + 4) * (nHeight + 4)];
						BYTE* pBuffer = new BYTE[(nWidth + 8) * (nHeight + 8)];

						ZeroMemory(pBuffer, (nWidth + 8) * (nHeight + 8));

						BYTE *pArtefact = new BYTE[nHeight * nWidth];

						switch (nCurArt)
						{
						case 0:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact1[nYIndex][nXIndex];
								break;
							}
						case 1:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact2[nYIndex][nXIndex];
								break;
							}
						case 2:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact3[nYIndex][nXIndex];
								break;
							}
						case 3:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact4[nYIndex][nXIndex];
								break;
							}
						case 4:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact5[nYIndex][nXIndex];
								break;
							}
						case 5:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact6[nYIndex][nXIndex];
								break;
							}
						case 6:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact7[nYIndex][nXIndex];
								break;
							}
						case 7:
							{
								for (int nYIndex = 0; nYIndex < nHeight; nYIndex++)
									for (int nXIndex = 0; nXIndex < nWidth; nXIndex++)
										pArtefact[nXIndex + nYIndex * nWidth] = Constants::c_arArtefact8[nYIndex][nXIndex];
								break;
							}
						};

						int nIndex = (4 * (nWidth + 8) + 4);

						for (int nYIndex = 0; nYIndex < nHeight; nYIndex++, nIndex += 8 )
							for (int nXIndex = 0; nXIndex < nWidth; nXIndex++, nIndex ++ )
								pBuffer[nIndex] = 255 * pArtefact[nXIndex + nYIndex * nWidth];
	
						IppiSize roiSize = {nWidth + 4, nHeight + 4};
						ippiFilterGauss_8u_C1R( pBuffer + (2 + 2 * (nWidth + 8)), nWidth + 8, pResult, nWidth + 4, roiSize, ippMskSize5x5 );
						
						pCurImage = new Gdiplus::Bitmap(nWidth + 4, nHeight + 4, PixelFormat32bppARGB);

						// копируем данные в картинку
						Gdiplus::BitmapData oImageData;
						pCurImage->LockBits( NULL, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &oImageData );
						if( oImageData.Scan0 )
						{
							
							int nIndex = (nHeight + 4 - 1) * (nWidth + 4);
							BYTE* pRow = LPBYTE(oImageData.Scan0);
							for( int nRow = 0; nRow < nHeight + 4; ++nRow, pRow += oImageData.Stride,  nIndex -= 2 * (nWidth + 4))
							{
								BYTE* pPixel = pRow;
								for( int nPos = 0; nPos < nWidth + 4; ++nPos, pPixel += 4, nIndex ++ )
								{
									pPixel[0] = (pResult[nIndex] > 0? 255 : 0);
									pPixel[1] = (pResult[nIndex] > 0? 255 : 0);
									pPixel[2] = (pResult[nIndex] > 0? 255 : 0);
									if ( nRow >= 2 && nRow < nHeight + 2 && nPos >= 2 && nPos < nWidth + 2 && 1 == pArtefact[nWidth * (nHeight - (nRow - 2) - 1) + nPos - 2] )
										pPixel[3] = 255;
									else
										pPixel[3] = pResult[nIndex];
								}
							}
							
							pCurImage->UnlockBits( &oImageData );
						}
						delete []pBuffer;
						delete []pResult;
						
						m_arpArtefactImage[nCurArt] = pCurImage;
					}
				}

				virtual ~Cinema()
				{
					for ( int nIndex = 0 ; nIndex < Constants::c_nArtefactsCount; nIndex++ )
						if ( m_arpArtefactImage[nIndex] )
							delete m_arpArtefactImage[nIndex];
				}

				
				void Create( ImageStudio::Serialize::Effects::EffectCinema& pEffect )
				{
					::srand(GetTickCount());
					m_nShiftBaseRnd = ::rand();
//					m_dShiftPeriod  = max( 0, pEffect.m_dShiftTimePeriod );
					m_dShiftOffsetX = max( 0, min(10, pEffect.m_dShiftMaxOffsetX));
					m_dShiftOffsetY = max( 0, min(10, pEffect.m_dShiftMaxOffsetY));
					m_dShiftFactorX = max( 0, min(10, pEffect.m_dShiftFactorOffsetX));
				    m_dShiftFactorY = max( 0, min(10, pEffect.m_dShiftFactorOffsetY));
					m_dLastShiftX = 1.0;
					m_dLastShiftY = 1.0;

					m_nMaxArtefactsShow = max(0 , min( 49, pEffect.m_nMaxShowArtifacts));
					m_nMinArtefactsShow = max(0 , min( 49, pEffect.m_nMinShowArtifacts));

					m_nMinLinesShow = max(0 , min( 49, pEffect.m_nMinShowLines));
					m_nMaxLinesShow = max(0 , min( 49, pEffect.m_nMaxShowLines));

					m_nMaxFramesLineShow = max(1, min(100, pEffect.m_nNumFramesForShowLines));
				}
				
				BOOL Draw( BYTE* pBGRA, int nWidth, int nHeight, int nStride, double dTime, double dCompleteness )
				{
					CBuffer<BYTE> oBuffer(nWidth*nHeight*4);
					if( oBuffer.IsEmpty() )
						return FALSE;
					
					BYTE* pBuffer = oBuffer.GetPtr();

					// "трясём" изображение
					//TransformImage( pBGRA, pBuffer, nWidth, nHeight, dTime, dCompleteness );

					// создаём рендер для отрисовки артефактов
					if( true )
					{
						Gdiplus::Bitmap oImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pBuffer );
						if( Gdiplus::Ok != oImage.GetLastStatus() )
							return FALSE;
						
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok != oRender.GetLastStatus() )
							return FALSE;

						oRender.SetInterpolationMode( InterpolationModeBilinear );
						oRender.SetSmoothingMode( SmoothingModeAntiAlias );
						TransformImage( pBGRA, oRender, nWidth, nHeight, dTime, dCompleteness );
						AddSmallArtefacts( oRender, nWidth, nHeight, dCompleteness );
						AddVerticalLines( oRender, nWidth, nHeight, dCompleteness );
					}

					// переводим в серый цвет
					//ToGray( pBuffer, nWidth, nHeight, dCompleteness );
					GrayscaleImage( pBuffer, nWidth, nHeight, dCompleteness, dTime );

					// добавляем "гранулирование"
					AddNoise( pBuffer, nWidth, nHeight, dCompleteness );

					memcpy( pBGRA, pBuffer, nWidth*nHeight*4 );

					return TRUE;
				}
			
			protected:

				BOOL TransformImage( BYTE* pBGRASource, Gdiplus::Graphics& oRender, int nWidth, int nHeight, double dTime, double dCompleteness )
				{
					//if( m_dShiftPeriod < 0.001 )
					//{
					//	memcpy( pBGRAResult, pBGRASource, nWidth * nHeight * 4 );
					//	return TRUE;
					//}

					Gdiplus::Bitmap oSourceImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pBGRASource );
					if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
						return FALSE;
					
					double dOffsetX = dCompleteness * m_dShiftOffsetX * nWidth / 640.0;
					double dOffsetY = dCompleteness * m_dShiftOffsetY * nHeight / 480.0;


					double dShiftFactorX = m_dShiftFactorX * nWidth / 640.0;
					double dShiftFactorY = m_dShiftFactorY * nHeight / 480.0;

					float fX;
					if ( 0 < m_dLastShiftX )
						fX = float( -fabs( (rand() % 1000) / 1000.0 * dOffsetX ) );
					else
					{
						double dCurX = (rand() % 1000) / 1000.0 * (2 * dShiftFactorX ) - dShiftFactorX; // отклонение от предыдущего положения не больше чем на dShiftFactorX пикселя
						fX = float ( m_dLastShiftX + dCurX );

						fX = float( (fX > 0 ? 0 : fX) );
						fX = float( (fX < -dOffsetX ? -dOffsetX : fX) );
					}


					float fY;
					if ( 0 < m_dLastShiftY )
						fY = float( -fabs( (rand() % 1000) / 1000.0 * dOffsetY ) );
					else
					{
						double dCurY = (rand() % 1000) /1000.0 * (2 * dShiftFactorY) - dShiftFactorY; // отклонение от предыдущего положения не больше чем на dShiftFactorY пикселя
						fY = float ( m_dLastShiftY + dCurY );

						fY = float( (fY > 0 ? 0 : fY) );
						fY = float( (fY < -dOffsetY ? -dOffsetY : fY) );
					}

					float fHeight = float(nHeight + dOffsetY);
					float fWidth  = float(nWidth  + dOffsetX);

					oRender.DrawImage(&oSourceImage, fX, fY, fWidth, fHeight);
					
					m_dLastShiftX = fX;
					m_dLastShiftY = fY;

					//if( m_dShiftPeriod < 0.001 )
					//{
					//	memcpy( pBGRAResult, pBGRASource, nWidth * nHeight * 4 );
					//	return TRUE;
					//}

					//CBuffer<Ipp32f> oMapX(nWidth*nHeight);
					//CBuffer<Ipp32f> oMapY(nWidth*nHeight);

					//if( oMapX.IsEmpty() || oMapY.IsEmpty() )
					//{
					//	memcpy( pBGRAResult, pBGRASource, nWidth * nHeight * 4 );
					//	return TRUE /*FALSE*/;
					//}

					//CRandom oRnd( DWORD(int(dTime / m_dShiftPeriod) + m_nShiftBaseRnd) );

					//double dOffsetX = dCompleteness * m_dShiftOffsetX;
					//double dOffsetY = dCompleteness * m_dShiftOffsetY;
					//double dSinX = oRnd.RandF();
					//double dSinY = oRnd.RandF();
					//double dX = dOffsetX * (1 + (2 * dSinX - 1) * dSinX);
					//double dY = dOffsetY * (1 + (2 * dSinY - 1) * dSinY);
					//double dScaleX = (nWidth  - dOffsetX*2) / nWidth;
					//double dScaleY = (nHeight - dOffsetY*2) / nHeight;

					//Ipp32f* pxMap = oMapX.GetPtr();
					//Ipp32f* pyMap = oMapY.GetPtr();

					//for( int nRow = 0; nRow < nHeight; ++nRow )
					//{
					//	for( int nPos = 0; nPos < nWidth; ++nPos, ++pxMap, ++pyMap )
					//	{
					//		*pxMap = float(dX + nPos * dScaleX);
					//		*pyMap = float(dY + nRow * dScaleY);
					//	}
					//}

					//IppiSize SrcSize = {nWidth, nHeight};
					//IppiRect SrcRect = {0, 0, nWidth, nHeight};

					//ippiRemap_8u_C4R( 
					//	pBGRASource, SrcSize, nWidth*4, SrcRect, 
					//	oMapX.GetPtr(), nWidth * sizeof(Ipp32f),
					//	oMapY.GetPtr(), nWidth * sizeof(Ipp32f), 
					//	pBGRAResult, nWidth*4, SrcSize, IPPI_INTER_LINEAR);

					return TRUE;
				}

				BOOL GrayscaleImage( BYTE* pBGRASource, int nWidth, int nHeight, double dCompleteness, double dTime )
				{
					IppiSize ImageSize = {nWidth, nHeight};

					Ipp8u* pIntensity = new Ipp8u[nWidth*nHeight];
					Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight*4];
					Ipp32f coeffs[3];
					Ipp8u* pSrc[4];

					if (pBuffer && pIntensity)
					{
						coeffs[0] = 0.11f;
						coeffs[1] = 0.59f;
						coeffs[2] = 0.3f;

						ippiColorToGray_8u_AC4C1R( pBGRASource , 4*nWidth, pIntensity, nWidth, ImageSize, coeffs);


						// noise
						if( true )
						{
							unsigned int nSeed = GetTickCount();
																					
							Ipp16s* psChanel = new Ipp16s[nWidth*nHeight];

							ippiConvert_8u16s_C1R(pIntensity, nWidth, psChanel, sizeof(Ipp16s)*nWidth, ImageSize);	
							ippiAddRandUniform_Direct_16s_C1IR(psChanel, sizeof(Ipp16s)*nWidth, ImageSize, 0, Ipp16s(4 * dCompleteness), &nSeed);
							ippiConvert_16s8u_C1R(psChanel, sizeof(Ipp16s)*nWidth, pIntensity, nWidth, ImageSize);	

							delete [] psChanel;
						}

						// мелкие артефакты
						int count = rand() % 7 - 2;
						for( int i = 0; i < count; i++)
						{
							int rnd = rand() % 6;
							int size = 1 + rnd*rnd/4;
							int x = rand() % (nWidth - size);
							int y = rand() % (nHeight - size);
							
							IppiSize CellSize = {size, size};
							BYTE c = 0;

							if( rand() % 2 )
							{
								ippiMin_8u_C1R( pIntensity + y * nWidth + x, nWidth, CellSize, &c);
								c += 20;
							}
							else
							{
								ippiMax_8u_C1R( pIntensity + y * nWidth + x, nWidth, CellSize, &c);
								c -= 20;
							}

							for( int j = size*size/2; j > 0; --j )
							{
								int xx = rand() % size;
								int yy = rand() % size;
								pIntensity[(y + yy)*nWidth + (x + xx)] = c;
							}
						}

						// крупные артефакты
						
						// свезённая полоса
						if( !(rand() % 300) )
						{
							int nCountRows = 20 + rand() % 10;
							int nRow = rand() % (nHeight - nCountRows - 1);
							BYTE* pBaseRow = pIntensity + (nRow + nCountRows)*nWidth;
							BYTE* pCurRow = pBaseRow - nWidth;

							// верхняя граница полосы
							int nPos = 0;
							for( nPos = 0; nPos < nWidth; nPos++ )
							{
								int rnd = rand() % 10;
								if( rnd < 4 )
								{
									pCurRow[nPos] = pBaseRow[nPos] - 20;
									pCurRow[nPos-nWidth] = pBaseRow[nPos];
								}
								else if( rnd < 6 )
								{
									pCurRow[nPos-nWidth] = pBaseRow[nPos];
								}
							}
							
							// сама полоса
							pCurRow -= nWidth*2;
							for( ; nCountRows > 4; --nCountRows, pCurRow -= nWidth )
							{
								memcpy( pCurRow, pBaseRow, nWidth );
							}

							// нижняя граница полосы
							for( nPos = 0; nPos < nWidth; nPos++ )
							{
								int rnd = rand() % 10;
								if( rnd < 4 )
								{
									pCurRow[nPos] = pBaseRow[nPos] - 20;
									pCurRow[nPos-nWidth] = pBaseRow[nPos];
								}
								else if( rnd < 6 )
								{
									pCurRow[nPos-nWidth] = pBaseRow[nPos];
								}
							}
						}

						//  нити
						//if( !(rand() % 50) )
						//{
						//	int x = rand() % (nWidth - Constants::c_nWidthFiber1);
						//	int y = rand() % (nHeight - Constants::c_nHeightFiber1);

						//	for( int j = 0; j < Constants::c_nHeightFiber1; j++ )
						//	{
						//		for( int i = 0; i < Constants::c_nWidthFiber1; i++ )
						//		{
						//			if( Constants::c_arFiber1[j][i] )
						//			{
						//				pIntensity[(y+j)*nWidth + (x + i)] = 220;
						//			}
						//		}
						//	}
						//}

						// вертикальные полоски
						//if( true )
						//{
						//	int dx = rand() % 5 - 2;

						//	int t = int(dTime / 7) * 7;
						//	::srand( DWORD(t + m_dwStartRnd ));
						//	::rand();

						//	if( !(rand() % 100) )
						//	{
						//		int nCount = rand() % 3 + 1;
						//		
						//		while( nCount-- > 0 )
						//		{
						//			int x = rand() % (nWidth - 5) + 2 + dx;
						//			int y1 = 0;
						//			int y2 = nHeight;

						//			for( int y = y1; y < y2; ++y )							
						//			{
						//				int c = pIntensity[y*nWidth + x] * 13 / 16;
						//				
						//				pIntensity[y*nWidth + x] = BYTE(c);
						//			}
						//		}
						//	}
						//}
						
						// блюр
						if( true )
						{
							Ipp8u* pGauss = new Ipp8u[nWidth*nHeight];
							IppiSize GaussSize = {nWidth - 4, nHeight - 4};
							Ipp8u* pTempPtr = pIntensity + nWidth*2 + 2;
							
							ippiFilterGauss_8u_C1R( pTempPtr, nWidth, pGauss, nWidth, GaussSize, ippMskSize5x5 );
							ippiCopy_8u_C1R( pGauss, nWidth, pTempPtr, nWidth, GaussSize );

							delete [] pGauss;
						}

						// копирование чёрно-белого изображения
						pSrc[0] = pIntensity;
						pSrc[1] = pIntensity;
						pSrc[2] = pIntensity;
						pSrc[3] = pIntensity;

						ippiCopy_8u_P4C4R(pSrc, nWidth, pBuffer, 4*nWidth, ImageSize);
						
						int alpha = int((1 - dCompleteness) * 255);
						if( alpha > 255 ) alpha = 255;
						if( alpha < 0 ) alpha = 0;
						
						ippiAlphaCompC_8u_AC4R( pBGRASource, 4*nWidth, Ipp8u(alpha), pBuffer, 4*nWidth, 255, pBGRASource, 4*nWidth, ImageSize, ippAlphaOver);
					}

					if (pBuffer)
						delete[] pBuffer;
					if (pIntensity)
						delete[] pIntensity;

					return TRUE;
				}
				
				BOOL ToGray( BYTE* pBGRASrcDst, int nWidth, int nHeight, double dCompleteness )
				{
					if( 0 == m_dGrayFactor )
						return TRUE;

					Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight*4];
					if( !pBuffer )
						return FALSE;

					Ipp8u* pGrayscale = new Ipp8u[nWidth*nHeight];
					if( !pGrayscale )
					{
						delete [] pBuffer;
						return FALSE;
					}

					IppiSize ImageSize = {nWidth, nHeight};

					ippiRGBToGray_8u_AC4C1R( pBGRASrcDst , 4*nWidth, pGrayscale, nWidth, ImageSize );

					Ipp8u* pSrcPlanes[4] = {
						pGrayscale,
						pGrayscale,
						pGrayscale,
						pGrayscale
					};

					ippiCopy_8u_P4C4R( pSrcPlanes, nWidth, pBuffer, 4*nWidth, ImageSize );
					
					int nSrcAlpha = int((1 - dCompleteness * m_dGrayFactor) * 255);

					if( nSrcAlpha > 255 ) nSrcAlpha = 255;
					if( nSrcAlpha < 0 )   nSrcAlpha = 0;
					
					ippiAlphaCompC_8u_AC4R( pBGRASrcDst, 4*nWidth, nSrcAlpha, pBuffer, 4*nWidth, 255, pBGRASrcDst, 4*nWidth, ImageSize, ippAlphaOver);

					delete [] pGrayscale;
					delete [] pBuffer;

					return TRUE;
				}

				BOOL AddNoise( BYTE* pBGRASrcDst, int nWidth, int nHeight, double dCompleteness )
				{
					if( nWidth < 3 && nHeight < 3 )
						return TRUE;

					Ipp8u* pNoisePlane = new Ipp8u[nWidth*nHeight];
					if( !pNoisePlane )
						return FALSE;

					Ipp8u* pAlphaPlane = new Ipp8u[nWidth*nHeight];
					if( !pAlphaPlane )
					{
						delete [] pNoisePlane;
						return FALSE;
					}

					unsigned int nSeed = GetTickCount();
					IppiSize ImageSize = {nWidth, nHeight};

					int nNoiseLevel = int((1 - m_dNoiseFactor * dCompleteness) * 254);

					if( nNoiseLevel > 254 ) nNoiseLevel = 254;
					if( nNoiseLevel < 0 )   nNoiseLevel = 0;

					// создаём рандомное затемнение
					ZeroMemory( pNoisePlane, nWidth*nHeight );
					ippiAddRandUniform_Direct_8u_C1IR( pNoisePlane, nWidth, ImageSize, Ipp8u(nNoiseLevel), 255, &nSeed );
					
					IppiSize GaussSize = {nWidth - 4, nHeight - 4};
					Ipp8u* pSrcGauss = pNoisePlane + 2*nWidth + 2;
					Ipp8u* pDstGauss = pAlphaPlane + 2*nWidth + 2;
					
					ippiFilterLowpass_8u_C1R( pSrcGauss, nWidth, pDstGauss, nWidth, GaussSize, ippMskSize5x5 );
					ippiCopyReplicateBorder_8u_C1R( pSrcGauss, nWidth, GaussSize, pAlphaPlane, nWidth, ImageSize, 2, 2 );
					ippiFilterLowpass_8u_C1R( pDstGauss, nWidth, pSrcGauss, nWidth, GaussSize, ippMskSize5x5 );
					//ippiCopy_8u_C1R( pAlphaPlane, nWidth, pGaussPtr, nWidth, GaussSize );
					
					// сохраняем альфу
					ippiCopy_8u_C4C1R( pBGRASrcDst + 3, nWidth*4, pAlphaPlane, nWidth, ImageSize);

					// копируем затемнение в альфу изображения
					ippiCopy_8u_C1C4R( pNoisePlane, nWidth, pBGRASrcDst + 3, nWidth*4, ImageSize);

					// "затемняем" пикселы
					ippiAlphaPremul_8u_AC4IR( pBGRASrcDst, nWidth*4, ImageSize );

					// возвращаем старую альфу
					ippiCopy_8u_C1C4R( pAlphaPlane, nWidth, pBGRASrcDst + 3, nWidth*4, ImageSize);
					
					delete [] pAlphaPlane;
					delete [] pNoisePlane;

					return TRUE;
				}

				BOOL AddSmallArtefacts( Gdiplus::Graphics& oRender, int nWidth, int nHeight, double dCompleteness )
				{
					Gdiplus::ImageAttributes oImageAttr;
					Gdiplus::ColorMatrix oColorMatrix = {
						1, 0, 0, 0, 0,
						0, 1, 0, 0, 0,
						0, 0, 1, 0, 0,
						0, 0, 0, 1, 0,
						0, 0, 0, 0, 1
					};
					
					if( m_arpArtefactImage )
					{
						int nCountArtefactsShow = 0;
						if ( m_nMaxArtefactsShow > m_nMinArtefactsShow )
							nCountArtefactsShow = rand() % (m_nMaxArtefactsShow - m_nMinArtefactsShow) + m_nMinArtefactsShow;
						else
							nCountArtefactsShow = m_nMinArtefactsShow;
						for (int nIndex = 0; nIndex < nCountArtefactsShow; nIndex++)
						{
							int nCurArtefact = rand() % Constants::c_nArtefactsCount;

							int nSrcX = 0;
							int nSrcY = 0;
							int nSrcW = Constants::c_arArtefactsWidth[nCurArtefact] + 4;
							int nSrcH = Constants::c_arArtefactsHeight[nCurArtefact] + 4;

							int nDstX =	rand() % nWidth;
							int nDstY =	nHeight - rand() % nHeight;
							int nDstW =	rand() % (nSrcW / 2) + 3;
							int nDstH =	rand() % (nSrcH / 2) + 3;

							oColorMatrix.m[3][3] = float((rand() % 50 + 200)*dCompleteness/255);
					

							if ( rand() % 2)
							{
								oColorMatrix.m[0][0] = 0;
								oColorMatrix.m[1][1] = 0;
								oColorMatrix.m[2][2] = 0;
							}

							oImageAttr.SetColorMatrix( &oColorMatrix );


							oRender.DrawImage( m_arpArtefactImage[nCurArtefact], Gdiplus::Rect(nDstX, nDstY, nDstW, nDstH), nSrcX, nSrcY, nSrcW, nSrcH, Gdiplus::UnitPixel, &oImageAttr );
						}
						//while( nCountSpots-- > 0 )
						//{
						//	int nSpot = rand() % Constants::c_nArtefactsCount;
						//	//int nSrcX = m_arrSpots[nSpot].X;
						//	//int nSrcY = m_arrSpots[nSpot].Y;
						//	//int nSrcW = m_arrSpots[nSpot].Width;
						//	//int nSrcH = m_arrSpots[nSpot].Height;

						//	int nSrcX = 0;
						//	int nSrcY = 0;
						//	int nSrcW = Constants::c_arArtefactsWidth[nSpot] + 4;
						//	int nSrcH = Constants::c_arArtefactsHeight[nSpot] + 4;

						//	//int nDstX =	rand() % nWidth;
						//	//int nDstY =	rand() % nHeight;
						//	//int nDstW =	rand() % (nSrcW / 2) + 3;
						//	//int nDstH =	rand() % (nSrcH / 2) + 3;

						//	int nDstX =	100;
						//	int nDstY =	100;
						//	int nDstW =	200;
						//	int nDstH =	200;
						//	oColorMatrix.m[3][3] = float((rand() % 50 + 200)*dCompleteness/255);
						//	oImageAttr.SetColorMatrix( &oColorMatrix );

						//	oRender.DrawImage( m_arpArtefactImage[7], 100, 100/*Gdiplus::Rect(nDstX, nDstY, nDstW, nDstH), nSrcX, nSrcY, nSrcW, nSrcH, Gdiplus::UnitPixel, &oImageAttr */);
						//}
					}
					return TRUE;
				}
				
				BOOL AddVerticalLines( Gdiplus::Graphics& oRender, int nWidth, int nHeight, double dCompleteness )
				{
					
					if ( m_nLinesCurCount < m_nMaxLinesShow )
					{
						int nCountLinesShow = 0;
						if ( m_nMaxLinesShow > m_nMinLinesShow )
							nCountLinesShow = rand() % (m_nMaxLinesShow - m_nMinLinesShow) + m_nMinLinesShow;
						else
							nCountLinesShow = m_nMinLinesShow;

						for ( int nIndex = m_nLinesCurCount; nIndex < min(49, nCountLinesShow); nIndex++ )
						{
							m_arLinesX[nIndex] = rand() % nWidth;
							m_arLinesCurLife[nIndex] = rand() % m_nMaxFramesLineShow + 1;
							if ( rand() % 100 ) // здесь фактор появляения линии
								m_arLinesCurLife[nIndex] = 0;
						}

						m_nLinesCurCount = ( m_nLinesCurCount > nCountLinesShow ? m_nLinesCurCount : nCountLinesShow);
					}

					for (int nIndex = 0; nIndex < m_nLinesCurCount; nIndex++)
					{
						int nDstX =	rand() % nWidth;
						if ( 0 != m_arLinesCurLife[nIndex] )
						{
							Gdiplus::Pen oPen(Gdiplus::Color(BYTE( (rand() % 100 + 100) * dCompleteness ) % 255, 0, 0, 0), 1);
							oRender.DrawLine(&oPen, float(m_arLinesX[nIndex]), float(-1.0), float(m_arLinesX[nIndex]), float(nHeight + 1.0) );
							m_arLinesCurLife[nIndex] -= 1;
						}
					}

					for (int nIndex = 0; nIndex < m_nLinesCurCount; nIndex++)
					{
						if ( 0 == m_arLinesCurLife[nIndex] )
						{
							for ( int nShiftIndex = nIndex; nShiftIndex < m_nLinesCurCount - 1; nShiftIndex++)
							{
								m_arLinesCurLife[nShiftIndex] = m_arLinesCurLife[nShiftIndex + 1];
								m_arLinesX[nShiftIndex] = m_arLinesX[nShiftIndex + 1];
							}
							m_nLinesCurCount--;
						}
					}

					return TRUE;
				}
			};


			class TVSimulator : public ImageStudio::Caching::CCacheItemPointer
			{
			protected:
				struct TSinus
				{
					float phase;
					float offset;
					float ampl;

					TSinus() : phase( 0 ), offset( 0 ), ampl( 0 ) {}
				};

			protected:
				double m_dSpeedVer;
				int    m_nSpeedFactorVer;
				double m_dOffsetVer;
				double m_dInterframeSize;

				double m_dInterframeXSize;

				int m_nInterline;

				static const int c_nCountSinus = 5;
				double m_dAmplOffset;
				double m_dAmplFactor;
				double m_dPhaseSpeed;

				TSinus m_aSinus[c_nCountSinus];

			public:
				TVSimulator()
				{
					m_dSpeedVer  =      0;
					m_nSpeedFactorVer = 0;
					m_dOffsetVer =      0;
					m_nInterline =      0;

					m_dAmplOffset = 0;
					m_dAmplFactor = 1;
					m_dPhaseSpeed = 0;
					
				}

				void Create( const ImageStudio::Serialize::Effects::EffectTVSimulation& pEffect, double dMetricX, double dMetricY )
				{
					::srand( ::GetTickCount() );
					
					m_dSpeedVer        = pEffect.dSpeedSynVer  * dMetricY;
					m_nSpeedFactorVer  = pEffect.nSpeedFactorVer;
					m_dOffsetVer       = pEffect.dOffsetSynVer * dMetricY;
					m_dInterframeSize  = max(pEffect.dInterframeSize * dMetricY, 0);
					m_dInterframeXSize = max(pEffect.dInterframeXSize * dMetricX, 0);

					m_nInterline = pEffect.nInterline;
					m_dAmplOffset = 0;

					double ampl   = max(0, min(0.5, pEffect.dAmplitudeSynHor));
					double offset = pEffect.dOffsetSynHor * IPP_PI2;

					for( int i = 0; i < c_nCountSinus; i++ )
					{
						m_aSinus[i].phase  = float(IPP_2PI * ((i + 1) * 2));
						m_aSinus[i].offset = float(offset);
						m_aSinus[i].ampl   = float(ampl / ((i + 1) * 2));
						
						m_dAmplOffset += m_aSinus[i].ampl;
					}

					m_dAmplOffset *= max(-100, min(100, pEffect.nAmplOffset)) / 101.0 * 5;
					m_dAmplFactor  = max(-100, min(100, pEffect.nAmplFactor)) / 101.0 + 1;
					m_dPhaseSpeed  = max(-100, min(100, pEffect.nPhaseSpeed)) / 100.0 * IPP_PI;
				}

				BOOL Draw( BYTE* pPixels, int nWidth, int nHeight, int nStride, double dTime, double dCompleteness )
				{
					CBuffer<BYTE>  oBuffer( nWidth*nHeight*4 );
					CBuffer<float> oMapX( nWidth * nHeight );
					CBuffer<float> oMapY( nWidth * nHeight );

					if( oBuffer.IsEmpty() || oMapX.IsEmpty() || oMapY.IsEmpty() )
						return FALSE;

					// создаём карту помех
					float* pMapX = oMapX.GetPtr();
					float* pMapY = oMapY.GetPtr();
					
					int nBaseInterline = 0;
					if( m_nInterline != 0 ) 
						nBaseInterline = (::rand() % 2) * 2 - 1;

					double dFactor = dCompleteness * nWidth;
					double dPhaseFactor = 1.0 / nHeight;
					
					for( int nRow = 0; nRow < nHeight; ++nRow )
					{
						double dy = nRow + ((m_dOffsetVer + (nBaseInterline + nRow) % (m_nInterline + 1)) + m_nSpeedFactorVer * m_dSpeedVer * dTime/1000) * dCompleteness;
						dy -= int(dy / (nHeight + m_dInterframeSize)) * (nHeight + m_dInterframeSize);

						if( dy < 0 )
							dy += nHeight;

						// вертикальные волны
						double dx = 0;
						for( int i = 0; i < c_nCountSinus; i++ )
						{
							dx += abs(dFactor * m_aSinus[i].ampl * ::sin( m_aSinus[i].phase * nRow * dPhaseFactor + m_aSinus[i].offset * dTime * m_dPhaseSpeed ));
						}
						if( dx > 0 )
							dx = -::pow( dx, m_dAmplFactor );
						
						dx -= dFactor * m_dAmplOffset;

						for( int nPos = 0; nPos < nWidth; ++nPos, ++pMapX, ++pMapY )
						{
							dx += 1;
							while( dx < 0 ) 
							{
								dx += (nWidth - 1) + m_dInterframeXSize;
							}
							while( dx > nWidth - 1 )
							{
								dx -= nWidth - 1 + m_dInterframeXSize;
							}

							*pMapX = float(dx);
							*pMapY = float(dy);
						}
					}

					// закрашиваем фоновым цветом
					BYTE* pBuffer = oBuffer.GetPtr();

					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize SrcSize = {nWidth, nHeight};

					Ipp8u Colors[4] = {0, 0, 0, 255};
					ippiSet_8u_C4R( Colors, pBuffer, nWidth*4, SrcSize );

					// ремепим
					ippiRemap_8u_C4R( pPixels, SrcSize, nWidth*4, SrcRect, 
						oMapX.GetPtr(), sizeof(float)*nWidth, 
						oMapY.GetPtr(), sizeof(float)*nWidth, 
						pBuffer, nWidth*4, SrcSize, IPPI_INTER_LINEAR);

					// отрисовка примитивов gdi+
					//Gdiplus::Bitmap oImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pBuffer );
					//if( Gdiplus::Ok == oImage.GetLastStatus() )
					//{
					//	Gdiplus::Graphics oRender( &oImage );
					//	if( Gdiplus::Ok == oRender.GetLastStatus() )
					//	{
					//		int alpha = int(dCompleteness * 64 + 0.5);

					//		Gdiplus::Pen oPenRed  ( Gdiplus::Color(alpha, 250, 50,  50),  2);
					//		Gdiplus::Pen oPenGreen( Gdiplus::Color(alpha, 50,  250, 50),  2);
					//		Gdiplus::Pen oPenBlue ( Gdiplus::Color(alpha, 0,   0,   250), 2);

					//		Gdiplus::Pen* aPens[3] = {&oPenRed, &oPenGreen, &oPenBlue};
					//		
					//		for( int nRow = 0; nRow < nHeight; nRow++ )
					//		{
					//			for( int nPos = 0; nPos < nWidth; nPos++ )
					//			{
					//				if( (::rand() % 25) > 0 )
					//					continue;

					//				Gdiplus::Color color;
					//				
					//				int nClr = ::rand() % 3;
					//				int nLen = ::rand() % 3 + 5;

					//				oRender.DrawLine( aPens[nClr], nPos, nRow, nPos + nLen, nRow );
					//			}
					//		}
					//	}
					//}
					
					// копируем буфер обратно
					ippiCopy_8u_C4R( pBuffer, nWidth*4, pPixels, nWidth*4, SrcSize );
				
					return TRUE;
				}
			};

			class Snow : public ImageStudio::Caching::CCacheItemPointer
			{
			protected:
				struct TSnowFlake
				{
					float fTimePeriod;
					float fTimeOffset;
					DWORD nBaseRandom;
				};


				CBuffer<TSnowFlake> m_aoSnowFlakes; // массив с исходными данными о снежинках
				int m_nCountSnowFlakes; // количество снежинок
				
				float m_fSizeSnowFlake; // размер снежинок в пикселях
				float m_fOffsetLeft;    // максимальное смещение влево
				float m_fOffsetRight;   // максимальное смещение вправо

				BYTE* m_pSnowRawImage_32x32; // image 64x64
				int m_nQuality;

				Gdiplus::Color m_oColor; // цвет снежинок

				double *m_adSnowDrift;      // граница сугроба
				int m_nSnowDriftWidth;      //
				
				int m_nBorder;
				Gdiplus::PointF* m_arrPoints; 
				double dSnowDriftSpeedFactor; // коэффициент скорости роста сугроба

				double m_dOldTime; // для автоматического сбрасывания сугроба при проигрывании с начала
			
			public:
				Snow()
				{
					m_nCountSnowFlakes = 0;
					m_fSizeSnowFlake = 0;
					m_fOffsetLeft = 0;
					m_fOffsetRight = 0;

					m_adSnowDrift = 0;
					m_nSnowDriftWidth = -1;
					m_arrPoints = 0;
					m_nBorder = -1;

					m_dOldTime = 0;

					m_pSnowRawImage_32x32 = NULL;
					m_nQuality = 0;
				}

				virtual ~Snow()
				{
					if(m_adSnowDrift)
						delete []m_adSnowDrift;
					
					if (m_arrPoints)
						delete []m_arrPoints;
				}

				void Create( const ImageStudio::Serialize::Effects::EffectSnow& pEffect )
				{
					m_nCountSnowFlakes = min(1000, max(0, pEffect.nCount));
					m_fSizeSnowFlake   = float(min(100, max(0, pEffect.nSize)) / 100.0 * 50);

					m_nBorder = int(6 * m_fSizeSnowFlake);
					
					int nAngleDir    = min( 45, max(-45, pEffect.nAngleDir));
					int nAngleOffset = min( 45, max(  0, pEffect.nAngleOffset));
					m_fOffsetLeft    = float(::tan( IPP_PI180 * max(-45, nAngleDir - nAngleOffset)));
					m_fOffsetRight   = float(::tan( IPP_PI180 * min( 45, nAngleDir + nAngleOffset)));

					float fMinDropTime = float(pEffect.dMinTime < 1 ? 1 : pEffect.dMinTime);
					float fMaxDropTime = float(pEffect.dMaxTime < 1 ? 1 : pEffect.dMaxTime);

					TSnowFlake* pSnowFlake = m_aoSnowFlakes.Create( m_nCountSnowFlakes );
					if( !pSnowFlake )
						return;

					for( int i = m_nCountSnowFlakes; i > 0; --i, ++pSnowFlake )
					{
						pSnowFlake->fTimePeriod = Random( fMinDropTime, fMaxDropTime );
						pSnowFlake->fTimeOffset = pSnowFlake->fTimePeriod * Random();
						pSnowFlake->nBaseRandom = ::rand();
					}

					SetSnowColor( pEffect.nColor, pEffect.nAlpha );
					
					LoadSnowImage( pEffect.nType );

					m_nQuality = pEffect.nQuality;
				}

				BOOL Draw( BYTE* pPixels, int nWidth, int nHeight, int nStride, double dTime, double dCompleteness, BOOL bSnowDrift, double dSnowDriftSpeedFactorIn )
				{
					int nSnowDriftWidth = nWidth + 2 * m_nBorder;
					// обнуляем верхнюю границу сугроба

					if( (dTime + 0.25) < m_dOldTime )
					{
						if( m_adSnowDrift )
						{
							delete [] m_adSnowDrift;
							m_adSnowDrift = NULL;
						}
					}

					m_dOldTime = dTime;

					double dSnowDriftSpeedFactor = min( 10, max(0.2, dSnowDriftSpeedFactorIn));
					if ( !m_adSnowDrift || m_nSnowDriftWidth != nSnowDriftWidth && bSnowDrift)
					{
						if ( m_adSnowDrift )
							delete []m_adSnowDrift;
						m_adSnowDrift = new double[nSnowDriftWidth];
						m_nSnowDriftWidth = nSnowDriftWidth;
						for (int nIndex = 0; nIndex < m_nSnowDriftWidth; nIndex++)
							m_adSnowDrift[nIndex] = 0;
					}

					// работаем со снежинками

					TSnowFlake* pSnowFlake = m_aoSnowFlakes.GetPtr();
					if( !pSnowFlake )
						return FALSE;

					if( nStride < 0 )
						pPixels -= (nHeight - 1) * nStride;


					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pPixels );
					if( Gdiplus::Ok != oImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Graphics oRender( &oImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;

					InterpolationMode mode;
					switch( m_nQuality )
					{
					case 1:  mode = Gdiplus::InterpolationModeBicubic; break;
					case 2:  mode = Gdiplus::InterpolationModeHighQualityBilinear; break;
					case 3:  mode = Gdiplus::InterpolationModeHighQualityBicubic; break;
					default: mode = Gdiplus::InterpolationModeBilinear;
					}

					oRender.SetInterpolationMode( mode );
					//oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

					Gdiplus::SolidBrush oBrush( Gdiplus::Color( int(dCompleteness * m_oColor.GetAlpha() + 0.5), m_oColor.GetRed(), m_oColor.GetGreen(), m_oColor.GetBlue() ));

					Gdiplus::Bitmap* pSnowImage = CreateSnowImage( m_oColor, dCompleteness );
					
					for( int i = 0; i < m_nCountSnowFlakes; ++i, ++pSnowFlake )
					{
						double time   = dTime + pSnowFlake->fTimeOffset;
						int    period = int(time / pSnowFlake->fTimePeriod);
						
						CRandom oRnd( DWORD(period + pSnowFlake->nBaseRandom) );
						
						time -= period * pSnowFlake->fTimePeriod;

						double dSpeedX = oRnd.RandF( m_fOffsetLeft, m_fOffsetRight ) * nHeight / pSnowFlake->fTimePeriod;
						double dSpeedY = (nHeight + m_fSizeSnowFlake) / pSnowFlake->fTimePeriod;
						double dPosX   = dSpeedX * time + oRnd.RandF( 0, float(nWidth) );
						double dPosY   = dSpeedY * time - m_fSizeSnowFlake;
						double dAngle  = oRnd.RandF( -1000.0, 1000.0 ) * time / pSnowFlake->fTimePeriod;

						// проверяем попалала ли снежинка в сугроб
						// если попала ее не рисуем, а у сугроба увеличиваем границу в данном месте


						// todo: 1. надо в структуре снежинки ввести булевскую переменную попадания в сугроб
						//       2. улучшить проверку на попадание снежинки в сугроб

						BOOL bDrawFlake = TRUE; 

						if ( bSnowDrift )
						{
							int nSize = int(m_fSizeSnowFlake);
							double dCenterX = dPosX - nSize;
							double dCenterY = dPosY + nSize;

							double dRevSize = (0.5 / nSize) * (0.5 / nSize);
							int nCurX = int(dPosX);

							if ( nCurX >= - m_nBorder && nCurX < nWidth + m_nBorder )
							{
								int nCurIndex = nCurX + m_nBorder;

								if ( dPosY <= nHeight - m_adSnowDrift[nCurIndex] && dPosY > nHeight - ( m_adSnowDrift[nCurIndex] + 1 ) )
								{
									double dCurVal = 0.5 * dSnowDriftSpeedFactor;
									m_adSnowDrift[nCurIndex] += dCurVal;
									
									for(int nX = nCurIndex; nX < min(nSnowDriftWidth - 1, nCurIndex + nSize); nX++)
									{
										dCurVal += -dRevSize;
										m_adSnowDrift[nX] += dCurVal; // эпицентр падения
										if( m_adSnowDrift[nX] > m_adSnowDrift[nCurIndex]) // если соседняя точка границы больше значения в эпицентре,
											m_adSnowDrift[nX] -= dCurVal / 2;         // тогда ее значение меньше увеличиваем
									}
									dCurVal = 1.0;
									for(int nX = nCurIndex; nX > max(0, nCurIndex - nSize); nX --)
									{
										dCurVal += -dRevSize;
										m_adSnowDrift[nX] += dCurVal;
										if( m_adSnowDrift[nX] > m_adSnowDrift[nCurIndex])
											m_adSnowDrift[nX] -= dCurVal / 2;
									}

								}
								if ( dPosY > nHeight - (m_adSnowDrift[nCurIndex] + 1))
									bDrawFlake = FALSE;
							}
						}
						
						if( bDrawFlake )
						{
							if( !DrawSnow( oRender, pSnowImage, float(dPosX), float(dPosY), m_fSizeSnowFlake, float(dAngle) ) )
							{
								oRender.FillEllipse( &oBrush, float(dPosX), float(dPosY), m_fSizeSnowFlake, m_fSizeSnowFlake );
							}
						}
					}

					if( pSnowImage )
						delete pSnowImage;


					oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
					oRender.SetSmoothingMode( Gdiplus::SmoothingModeAntiAlias );

					if ( bSnowDrift )
					{
						double dFirst = m_adSnowDrift[0];
						double dSecond = m_adSnowDrift[1];

						for(int nIndex = 2; nIndex < nSnowDriftWidth - 2; nIndex++)
						{
							// интерполяция по 5 точкам, для сглаживания
							double dCurVal = (dFirst + dSecond + m_adSnowDrift[nIndex] + m_adSnowDrift[nIndex + 1] + m_adSnowDrift[nIndex + 2]) / 5;
							dFirst = dSecond; 
							dSecond = m_adSnowDrift[nIndex];
							m_adSnowDrift[nIndex] = dCurVal;
						}

						if (m_arrPoints)
							delete []m_arrPoints;
						m_arrPoints = new Gdiplus::PointF[nSnowDriftWidth + 2];
						for(int nIndex = 0; nIndex < nSnowDriftWidth; nIndex++)
						{
							m_arrPoints[nIndex].X = float(nIndex) - m_nBorder;
							m_arrPoints[nIndex].Y = float(nHeight - m_adSnowDrift[nIndex]);
						}
						m_arrPoints[nSnowDriftWidth].X = float(nWidth + 1);
						m_arrPoints[nSnowDriftWidth].Y = float(nHeight + 1);
						m_arrPoints[nSnowDriftWidth + 1].X = float(-10.0);
						m_arrPoints[nSnowDriftWidth + 1].Y = float(nHeight + 1);

						oRender.FillPolygon( &oBrush, m_arrPoints, nSnowDriftWidth + 2);
					}
					return TRUE;
				}

			protected:
				void SetSnowColor( int color, int alpha )
				{
					m_oColor = Gdiplus::Color( min(255, max(0, alpha)), GetRValue(color), GetGValue(color), GetBValue(color));
				}

				
				BOOL LoadSnowImage( int nType )
				{
					static const LPCTSTR szNames[5] = { _T("#208"), _T("#209"), _T("#210"), _T("#211"), _T("#212") };
						
					if( nType < 0 || nType > 4 )
					{
						nType = 0;
					}

					HMODULE hModule = ::GetModuleHandle( _T("AVSImageStudio3.dll") );
					HRSRC hResHandle = ::FindResource( hModule, szNames[nType], _T("RawImage") );
					if( !hResHandle )
						return FALSE;

					HGLOBAL hResMem = ::LoadResource( hModule, hResHandle );
					if( !hResMem )
						return FALSE;

					m_pSnowRawImage_32x32 = (BYTE*)::LockResource( hResMem );
					
					return m_pSnowRawImage_32x32 != NULL;
				}

				Gdiplus::Bitmap* CreateSnowImage( Gdiplus::Color color, double dCompleteness ) const 
				{
					if( !m_pSnowRawImage_32x32 )
						return NULL;

					const int nWidth  = 32;
					const int nHeight = 32;

					Gdiplus::Bitmap* pResult = new Gdiplus::Bitmap( nWidth, nHeight );
					if( !pResult || pResult->GetLastStatus() != Gdiplus::Ok )
					{
						return NULL;
					}

					Gdiplus::BitmapData oData;

					if( Gdiplus::Ok != pResult->LockBits( NULL, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &oData ) )
					{
						delete pResult;
						return NULL;
					}

					DWORD clr[4] = {
						DWORD(color.GetBlue())  * 0x8081,
						DWORD(color.GetGreen()) * 0x8081,
						DWORD(color.GetRed())   * 0x8081,
						DWORD(color.GetAlpha() * dCompleteness + 0.5) * 0x8081,
					};

					int nDstStride = oData.Stride - nWidth * 4;
					BYTE* pDst = (BYTE*)oData.Scan0;
					BYTE* pSrc = m_pSnowRawImage_32x32;

					for( int nY = 0; nY < nHeight; ++nY, pDst += nDstStride )
					{
						for( int nX = 0; nX < nWidth; ++nX, pSrc += 4, pDst += 4 )
						{
							for( int i = 0; i < 4; i++ )
							{
								pDst[i] = BYTE((pSrc[i] * clr[i] + 128 * 0x8081) >> 23 /*/ 255*/);
							}
						}
					}

					pResult->UnlockBits( &oData );

					return pResult;
				}

				BOOL DrawSnow( Gdiplus::Graphics& render, Gdiplus::Bitmap* pSnowImage, float x, float y, float size, float angle ) const
				{
					if( !pSnowImage )
						return FALSE;

					Gdiplus::RectF rect( x - size / 2, y - size / 2, size, size );

					Gdiplus::Matrix matrix1;
					Gdiplus::Matrix matrix2;

					matrix2.RotateAt( angle, Gdiplus::PointF( x, y ), Gdiplus::MatrixOrderAppend );

					render.GetTransform( &matrix1 );
					render.SetTransform( &matrix2 );

					Gdiplus::Status status = render.DrawImage( pSnowImage, rect );

					render.SetTransform( &matrix1 );

					return Gdiplus::Ok == status;
				}
			};

			class Puzzle : public ImageStudio::Caching::CCacheItemPointer
			{
			protected:
		
				int m_nFigureType;

				// параметры для пазла и плитки
				int *m_pHorizontalFactor;
				int *m_pVerticalFactor;
				int m_nRow;
				int m_nCol;

				// параметры для остальных случаев

				double m_dSide;
				int m_nXCount;
				int m_nYCount;
				int m_nCount;


				double m_dBigRadius;
				double m_dShiftY;  // вспомогательные параметры, обычно используются для нахождения
				double m_dShiftX;  // координат вершин, относительно координат центра

				Gdiplus::PointF* m_arrPoints;
				int              m_nPointsCount;
			
			public:
				Puzzle()
				{
					m_pHorizontalFactor = 0;
					m_pVerticalFactor   = 0;
					m_nRow = -1;
					m_nCol = -1;

					m_dSide = -1;
					m_nXCount = -1;
					m_nYCount = -1;
					m_nCount = -1;

					m_arrPoints = 0;
					m_nPointsCount = -1;
				}

				virtual ~Puzzle()
				{
					if ( m_pHorizontalFactor )
						delete []m_pHorizontalFactor;

					if ( m_pVerticalFactor )
						delete []m_pVerticalFactor;
					if ( m_arrPoints )
						delete []m_arrPoints;
				}

				void Create( const ImageStudio::Serialize::Paint::Common::DrawPuzzle& pEffect, double dSide, DWORD dwBaseRnd )
				{
					m_nFigureType = pEffect.m_nType;

					if ( 1 == pEffect.m_nType ) 
						GeneratePuzzleMask(pEffect.m_nRow, pEffect.m_nCol, dwBaseRnd );
					else if ( 2 <= pEffect.m_nType && 6 >= pEffect.m_nType )
					{
						m_dSide = dSide;
						m_arrPoints = 0;
						m_nPointsCount = -1;

						m_nFigureType = pEffect.m_nType;

						// далее заполняем дополнительные данные
						if ( 2 == m_nFigureType )
						{
							m_dBigRadius   = dSide;
							m_dShiftX = m_dBigRadius / 2;
							m_dShiftY = sqrt(3.0) * m_dBigRadius / 2;
						}
						else if ( 5 == m_nFigureType)
						{
							m_dBigRadius = sqrt(3.0) * dSide / 3; // радиус описанной окружности
							m_dShiftY = m_dBigRadius / 2;         // радиус вписанной окружности
							m_dShiftX = dSide / 2;
						}
						else if ( 3 == m_nFigureType )
						{
							m_dBigRadius = dSide / sqrt(2.0);
						}
						else if ( 4 == m_nFigureType )
						{
							m_dShiftX = dSide / 2;
							m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
						}
						else if ( 6 == m_nFigureType )
						{
							m_dShiftX = dSide / 2;
							m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
						}
					}
					return;
				}


				void GeneratePuzzleMask( int nRow, int nCol, DWORD dwBaseRnd )
				{
					if ( m_pHorizontalFactor && m_pVerticalFactor && nRow == m_nRow && nCol == m_nCol )
						return;

					if ( m_pHorizontalFactor )
						delete []m_pHorizontalFactor;
					if ( m_pVerticalFactor )
						delete []m_pVerticalFactor;

					int nSize = nCol * nRow * 2;

					m_nRow = nRow;
					m_nCol = nCol;

					m_pHorizontalFactor = new int[nSize];
					m_pVerticalFactor = new int[nSize];

					CRandom rnd( dwBaseRnd );
					
					for ( int nYIndex = 0, nIndex = 0; nYIndex < nRow; nYIndex++ )
					{
						for ( int nXIndex = 0; nXIndex < nCol; nXIndex++, nIndex += 2 )
						{
							if ( 1 == nRow )
							{
								m_pHorizontalFactor[nIndex + 0] = 0;
								m_pHorizontalFactor[nIndex + 1] = 0;
								continue;
							}
							if ( 0 == nYIndex )
							{
								m_pHorizontalFactor[nIndex + 0] = 0;
								m_pHorizontalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
							}
							else if ( nRow - 1 == nYIndex )
							{
								m_pHorizontalFactor[nIndex + 1] = 0;
								m_pHorizontalFactor[nIndex + 0] = - m_pHorizontalFactor[nIndex - nCol * 2 + 1];
							}
							else
							{
								m_pHorizontalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
								m_pHorizontalFactor[nIndex + 0] = - m_pHorizontalFactor[nIndex - nCol * 2 + 1];
							}
							
							if ( 1 == nCol )
							{
								m_pVerticalFactor[nIndex + 0] = 0;
								m_pVerticalFactor[nIndex + 1] = 0;
								continue;
							}
							if ( 0 == nXIndex )
							{
								m_pVerticalFactor[nIndex + 0] = 0;
								m_pVerticalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
							}
							else if ( nCol - 1 == nXIndex )
							{
								m_pVerticalFactor[nIndex + 1] = 0;
								m_pVerticalFactor[nIndex + 0] = - m_pVerticalFactor[nIndex - 1];
							}
							else
							{
								m_pVerticalFactor[nIndex + 1] = -1 + 2 * rnd.Rand( 2 );
								m_pVerticalFactor[nIndex + 0] = - m_pVerticalFactor[nIndex - 1];
							}

						}
					}
					return;
				}			

				void SetCount(int nFigureType, int nWidth, int nHeight)
				{
					int nXCount = 0;
					int nYCount = 0;

					double dSide = m_dSide;
					if ( dSide < 5 )
						return;

					//
					// Расчет числа кусочков
					//
					if ( 2 == nFigureType ) // шестиугольники
					{
						nXCount = nWidth / int(1.5 * dSide) + 1;
						if( nWidth % int(1.5 * dSide) > 0 )
							nXCount++;
						
						nYCount = nHeight / int(sqrt(3.0) * dSide) + 1;
						if( nHeight % int(sqrt(3.0) * dSide) > 0 )
							nYCount++;

					}
					else if ( 5 == nFigureType ) // треугольники
					{
						nXCount = nWidth / int(0.5 * dSide) + 1;
						if( nWidth % int(0.5 * dSide) > 0 )
							nXCount++;

						nYCount = nHeight / int(sqrt(3.0) * 0.5 * dSide) + 1;
						if( nHeight % int(sqrt(3.0) * 0.5 * dSide) > 0 )
							nYCount++;
					}
					else if ( 3 == nFigureType ) // ромбы
					{
						double dRadius = dSide / sqrt(2.0); // радиус описанной окружности

						nXCount = nWidth / int(dRadius) + 2;

						nYCount = nHeight / int(2 * dRadius) + 2;
					}
					else if ( 4 == nFigureType ) // кирпичи
					{
						double dXShift = dSide;
						double dYShift = dSide * (12.0 / 25.0);

						nXCount = nWidth / int(dXShift) + 2;

						nYCount = nHeight / int(dYShift) + 2;
					}
					else if ( 6 == nFigureType ) //наклонные кирпичи
					{
						double dAngle =  atan(25.0 / 24.0) - IPP_PI4;
						double dXShift = dSide * sqrt( 1.0 / 4.0 + 144.0 / 625.0 ) * cos(dAngle);

						nXCount = int((nWidth + nHeight * tan(dAngle)) / int(dXShift) + 2);

						nYCount = int((nHeight + nWidth * tan(dAngle)) / int(dXShift) + 2);
					}
					int nCount = nXCount * nYCount;

					m_nXCount = nXCount;
					m_nYCount = nYCount;
					m_nCount = nCount;

				}
				BOOL DrawPiece(Gdiplus::Graphics *oRender, double dXCoorResult, double dYCoorResult, int nWidth, int nHeight, double dAlpha, double dFigWidth, double dFigHeight, int nTopFactor, int nRightFactor, int nBottomFactor, int nLeftFactor)
				{
					if ( !oRender )
						return FALSE;
					
					int nSize = 4 * nHeight * nWidth;

					double dDiagonal = min(dFigHeight, dFigWidth);
					
					double dUnit = 1 / (72.0) * dDiagonal;
					double dCircleWidth = 26 * dUnit;                  // ширина кружочка, т.е. 2 * радиус - (зазор со средней частью)
					double dRadius      = 15 * dUnit;                  // радиус кружочка
					double dSqrRadius   = dRadius * dRadius;

					double dInStep      = 37 * dUnit;

					double dFullHeight = dFigHeight + 2 * dCircleWidth; // полная высота кусочка, с учетом кружков
					double dFullWidth  = dFigWidth  + 2 * dCircleWidth; // полная ширина кусочка

					double dXResult = dXCoorResult - dCircleWidth;
					double dYResult = dYCoorResult - dCircleWidth;

					double dStep = 11 * dUnit;
					double dSrez  = sqrt(dSqrRadius - dStep * dStep);
					double dAngle = acos(11 * dUnit / dRadius) * 180 / IPP_PI;

					double dAlphaKoef = dAlpha / 255.0;
					int nAlphaBlack = int(448 * dAlphaKoef);
					int nAlphaWhite = int(448 * dAlphaKoef);
					for (int nIndex = 0; nIndex < 5; nIndex++ )
					{
						nAlphaBlack /= 2; 
						if (nIndex == 1)
						{
							nAlphaBlack = int(224 * dAlphaKoef);
							nAlphaWhite = int(224 * dAlphaKoef);
						}
						Gdiplus::Pen oPen( Color( nAlphaBlack, 0, 0, 0) , 1);
						if ( 0 == nRightFactor )
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
						else
						{
							if ( 1 == nRightFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius + nIndex),             nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), float(180 + dAngle), float(360 - 2 * dAngle));
							else if ( -1 == nRightFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius - nIndex - 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), float(dAngle + nIndex), float(360 - 2 * dAngle - 2* nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez +nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
						}

						if ( 0 == nTopFactor )
							oRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
						else
						{
							if ( 1 == nTopFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + 2 * dRadius - nIndex),             float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), float(270 + dAngle), float(360 - 2 * dAngle));
							else if ( -1 == nTopFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + 2 * dRadius + nIndex + 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), float(90 + dAngle + nIndex), float(360 - 2 * dAngle - 2* nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
						}

						nAlphaWhite /= 2;
						oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));

						if ( 0 == nLeftFactor )
							oRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
						else
						{
							if ( 1 == nLeftFactor)
								oRender->DrawArc(&oPen, float(dXResult + nIndex),             nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), float(dAngle), float(360 - 2 * dAngle));
							else if ( -1 == nLeftFactor)
								oRender->DrawArc(&oPen, float(dXResult - nIndex + 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), float(180 + dAngle + nIndex), float(360 - 2 * dAngle - 2* nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
						}

						if ( 0 == nBottomFactor )
							oRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
						else
						{
							if ( 1 == nBottomFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + dFullHeight - nIndex),             float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), float(90 + dAngle), float(360 - 2 * dAngle));
							else if ( -1 == nBottomFactor)
								oRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + dFullHeight + nIndex - 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), float(270 + dAngle + nIndex), float(360 - 2 * dAngle - 2* nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
							oRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
						}


					}
					

					return TRUE;
				}

				BOOL CreatePolygon(int nNumber, double dX, double dY)
				{
					if (m_arrPoints)
						delete []m_arrPoints;
					if (2 == nNumber)
					{
						m_nPointsCount = 6;
						m_arrPoints = new Gdiplus::PointF[6];
						int nRadius = int(m_dBigRadius);
						double dXShift = m_dShiftX;
						double dYShift = m_dShiftY;

						m_arrPoints[0].X = float(dX - dXShift);
						m_arrPoints[0].Y = float(dY + dYShift);
						m_arrPoints[1].X = float(dX + dXShift);
						m_arrPoints[1].Y = float(dY + dYShift);
						m_arrPoints[2].X = float(dX + nRadius);
						m_arrPoints[2].Y = float(dY);
						m_arrPoints[3].X = float(dX + dXShift);
						m_arrPoints[3].Y = float(dY - dYShift);

						m_arrPoints[4].X = float(dX - dXShift);
						m_arrPoints[4].Y = float(dY - dYShift);
						m_arrPoints[5].X = float(dX - nRadius);
						m_arrPoints[5].Y = float(dY);

					}

					else if ( 30 == nNumber )
					{
						m_nPointsCount = 3;
						m_arrPoints = new Gdiplus::PointF[3];
						m_arrPoints[0].X = float(dX - m_dShiftX);
						m_arrPoints[0].Y = float(dY + m_dShiftY);

						m_arrPoints[1].X = float(dX);
						m_arrPoints[1].Y = float(dY - m_dBigRadius);
						
						m_arrPoints[2].X = float(dX + m_dShiftX);
						m_arrPoints[2].Y = float(dY + m_dShiftY);
					}
					else if ( 31 == nNumber )
					{
						m_nPointsCount = 3;
						m_arrPoints = new Gdiplus::PointF[3];
						m_arrPoints[0].X = float(dX);
						m_arrPoints[0].Y = float(dY + m_dBigRadius);

						m_arrPoints[1].X = float(dX - m_dShiftX);
						m_arrPoints[1].Y = float(dY - m_dShiftY);
						
						m_arrPoints[2].X = float(dX + m_dShiftX);
						m_arrPoints[2].Y = float(dY - m_dShiftY);
					}
					else if ( 3 == m_nFigureType )
					{
						m_nPointsCount = 4;
						m_arrPoints = new Gdiplus::PointF[4];

						m_arrPoints[0].X = float(dX - m_dBigRadius);
						m_arrPoints[0].Y = float(dY);

						m_arrPoints[1].X = float(dX);
						m_arrPoints[1].Y = float(dY + m_dBigRadius);
						
						m_arrPoints[2].X = float(dX + m_dBigRadius);
						m_arrPoints[2].Y = float(dY);
						
						m_arrPoints[3].X = float(dX);
						m_arrPoints[3].Y = float(dY - m_dBigRadius);
					}
					else if ( 4 == m_nFigureType )
					{
						m_nPointsCount = 4;
						m_arrPoints = new Gdiplus::PointF[4];

						m_arrPoints[0].X = float(dX - m_dShiftX);
						m_arrPoints[0].Y = float(dY - m_dShiftY);

						m_arrPoints[1].X = float(dX + m_dShiftX);
						m_arrPoints[1].Y = float(dY - m_dShiftY);
						
						m_arrPoints[2].X = float(dX + m_dShiftX);
						m_arrPoints[2].Y = float(dY + m_dShiftY);
						
						m_arrPoints[3].X = float(dX - m_dShiftX);
						m_arrPoints[3].Y = float(dY + m_dShiftY);
					}
					else if ( 6 == m_nFigureType )
					{
						m_nPointsCount = 4;
						m_arrPoints = new Gdiplus::PointF[4];

						double dCos = cos(-IPP_PI4);
						double dSin = sin(-IPP_PI4);
						
						m_arrPoints[3].X = float(- m_dShiftX * dCos - (- m_dShiftY) * dSin + dX);
						m_arrPoints[3].Y = float(- m_dShiftX * dSin + (- m_dShiftY) * dCos + dY);

						m_arrPoints[2].X = float(  m_dShiftX * dCos - (- m_dShiftY) * dSin + dX);
						m_arrPoints[2].Y = float(  m_dShiftX * dSin + (- m_dShiftY) * dCos + dY);
						
						m_arrPoints[1].X = float(  m_dShiftX * dCos - (  m_dShiftY) * dSin + dX);
						m_arrPoints[1].Y = float(  m_dShiftX * dSin + (  m_dShiftY) * dCos + dY);
						
						m_arrPoints[0].X = float(- m_dShiftX * dCos - (  m_dShiftY) * dSin + dX);
						m_arrPoints[0].Y = float(- m_dShiftX * dSin + (  m_dShiftY) * dCos + dY);
					}
					return TRUE;
				}



				BOOL DrawHexagonBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
				{
					Gdiplus::Pen oPen(Color(0, 0, 0), 1);
					CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

					int nAlphaBlack = int(448 * dEffect);
					int nAlphaWhite = int(448 * dEffect);
					
					for ( int nIndex = 0; nIndex < 5; nIndex++ )
					{
						nAlphaBlack /= 2; 
						if (nIndex == 1)
						{
							nAlphaBlack = int(224 * dEffect);
							nAlphaWhite = int(224 * dEffect);
						}
						nAlphaWhite /= 2;
						//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
						oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
						oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
						//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[4]);
						oMainRender->DrawLine(&oPen, m_arrPoints[4], m_arrPoints[5]);
						oMainRender->DrawLine(&oPen, m_arrPoints[5], m_arrPoints[0]);

						m_arrPoints[0].X++;
						m_arrPoints[0].Y--;

						m_arrPoints[1].X--;
						m_arrPoints[1].Y--;

						m_arrPoints[2].X--;

						m_arrPoints[3].X--;
						m_arrPoints[3].Y++;

						m_arrPoints[4].X++;
						m_arrPoints[4].Y++;

						m_arrPoints[5].X++;

					}
					return TRUE;
				}
				BOOL DrawTriangleBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect, int nType)
				{
					if (0 == nType )
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(30, dXCoorResult, dYCoorResult);

						int nAlphaBlack = int(448 * dEffect);
						int nAlphaWhite = int(448 * dEffect);
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1 || nIndex == 2)
							{
								nAlphaBlack = int(224 * dEffect);
								nAlphaWhite = int(224 * dEffect);
							}
							nAlphaWhite /= 2;
							if(nIndex == 0)
							{
								nAlphaWhite = 128;
								nAlphaBlack = 128;
							}
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

							m_arrPoints[0].X ++;
							m_arrPoints[0].Y -= float(0.4);

							m_arrPoints[1].Y++;

							m_arrPoints[2].X --;
							m_arrPoints[2].Y -= float(0.4);


						}
					}
					else if ( 1 == nType)
					{
						Gdiplus::Pen oPen(Color(0, 0, 0), 1);
						CreatePolygon(31, dXCoorResult, dYCoorResult);

						int nAlphaBlack = int(448 * dEffect);
						int nAlphaWhite = int(448 * dEffect);
						
						for ( int nIndex = 0; nIndex < 5; nIndex++ )
						{
							nAlphaBlack /= 2; 
							if (nIndex == 1 || nIndex == 2)
							{
								nAlphaBlack = int(224 * dEffect);
								nAlphaWhite = int(224 * dEffect);
							}
							nAlphaWhite /= 2;
							if(nIndex == 0)
							{
								nAlphaWhite = 128;
								nAlphaBlack = 128;
							}
							oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
							oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
							oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
							oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
							oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

							m_arrPoints[0].Y--;

							m_arrPoints[1].X ++;
							m_arrPoints[1].Y += float(0.4);

							m_arrPoints[2].X --;
							m_arrPoints[2].Y += float(0.4);


						}
					}
					return TRUE;
				}

				BOOL DrawRhombusBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
				{
					Gdiplus::Pen oPen(Color(0, 0, 0), 1);
					CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

					int nAlphaBlack = int(448 * dEffect);
					int nAlphaWhite = int(448 * dEffect);
					
					for ( int nIndex = 0; nIndex < 5; nIndex++ )
					{
						nAlphaBlack /= 2; 
						if (nIndex == 1)
						{
							nAlphaBlack = int(224 * dEffect);
							nAlphaWhite = int(224 * dEffect);
						}
						nAlphaWhite /= 2;
						//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
						oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
						//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
						oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

						m_arrPoints[0].X++;
						m_arrPoints[1].Y--;
						m_arrPoints[2].X--;
						m_arrPoints[3].Y++;

					}
					return TRUE;
				}

				BOOL DrawBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
				{
					Gdiplus::Pen oPen(Color(0, 0, 0), 1);
					CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

					int nAlphaBlack = int(448 * dEffect);
					int nAlphaWhite = int(448 * dEffect);
					
					for ( int nIndex = 0; nIndex < 5; nIndex++ )
					{
						nAlphaBlack /= 2; 
						if (nIndex == 1)
						{
							nAlphaBlack = int(224 * dEffect);
							nAlphaWhite = int(224 * dEffect);
						}
						nAlphaWhite /= 2;
						//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
						oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
						//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
						oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

						m_arrPoints[0].X++;
						m_arrPoints[0].Y++;

						m_arrPoints[1].X--;
						m_arrPoints[1].Y++;
						
						m_arrPoints[2].X--;
						m_arrPoints[2].Y--;
						
						m_arrPoints[3].X++;
						m_arrPoints[3].Y--;

					}
					return TRUE;
				}
				BOOL DrawInclinedBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
				{
					Gdiplus::Pen oPen(Color(0, 0, 0), 1);
					CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

					int nAlphaBlack = int(448 * dEffect);
					int nAlphaWhite = int(448 * dEffect);
					
					for ( int nIndex = 0; nIndex < 5; nIndex++ )
					{
						nAlphaBlack /= 2; 
						if (nIndex == 1)
						{
							nAlphaBlack = int(224 * dEffect);
							nAlphaWhite = int(224 * dEffect);
						}
						nAlphaWhite /= 2;
						//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
						oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
						//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
						oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
						oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
						oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

						m_arrPoints[1].X--;
						m_arrPoints[2].Y++;
						m_arrPoints[3].X++;
						m_arrPoints[0].Y--;

					}
					return TRUE;
				}


				BOOL Draw( BYTE* pPixels, int nWidth, int nHeight, int nStride, double dFrame, int nRow, int nCol, double dSideFactor, int nType )
				{
					double dAlpha = 255 * dFrame;

					if( nStride < 0 )
						pPixels -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pPixels );
					
					if( !(Gdiplus::Ok == oImage.GetLastStatus()) )
					{
						return FALSE;
					}
					Gdiplus::Graphics oRender( &oImage );					

					oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
					oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
					oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

					if ( 0 <= nType && 1 >= nType)
					{

						double dFigWidth = nWidth / nCol;
						double dFigHeight = nHeight / nRow;

						if ( 0 == nType )
						{
							for (int nYIndex = 0, nIndex = 0; nYIndex < nRow; nYIndex ++ )
							{
								for ( int nXIndex = 0; nXIndex < nCol; nXIndex++, nIndex += 2 )
								{
									DrawPiece( &oRender, nXIndex * nWidth / nCol, nYIndex * nHeight / nRow, nWidth, nHeight, dAlpha, dFigWidth, dFigHeight, 0, 0, 0, 0);
								}
							}
						}
						else if ( 1 == nType )
						{
							if ( !m_pHorizontalFactor || !m_pVerticalFactor )
								return FALSE;
							for (int nYIndex = 0, nIndex = 0; nYIndex < nRow; nYIndex ++ )
							{
								for ( int nXIndex = 0; nXIndex < nCol; nXIndex++, nIndex += 2 )
								{
									DrawPiece( &oRender, nXIndex * nWidth / nCol, nYIndex * nHeight / nRow, nWidth, nHeight, dAlpha, dFigWidth, dFigHeight, m_pHorizontalFactor[nIndex], m_pVerticalFactor[nIndex + 1], m_pHorizontalFactor[nIndex + 1], m_pVerticalFactor[nIndex]);
								}
							}
						}
					}
					else
					{
						SetCount(m_nFigureType, nWidth, nHeight);
						int nFullWidth = 0;
						int nFullHeight = 0;

						dAlpha = dAlpha /256.0;
						double dSide = m_dSide;
						int nXCount = m_nXCount;
						int nYCount = m_nYCount;
						int nCount = m_nCount;

						if ( 2 == m_nFigureType )
						{					
							nFullWidth = int(3 * dSide);
							nFullHeight = int(3 * dSide);

							double dCurX = 0; 
							double dCurY = 0;
							int nYLevel = 0;

							for ( int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								int nYShift = int(sqrt(3.0) * dSide / 2);
								if ( dCurX > nWidth + 2 * dSide )
								{
									if ( 0 == nYLevel )
										dCurX = dSide * 1.5;
									else
										dCurX = 0;
									dCurY += nYShift;
									nYLevel++;
									nYLevel = nYLevel % 2;
								}
								DrawHexagonBorder(&oRender, dCurX, dCurY, dAlpha);
								dCurX += 3 * dSide;
							}
						}
						else if ( 5 == m_nFigureType )
						{					
							nFullWidth = int(dSide);
							nFullHeight = int(dSide);

							int nCurX = 0; 
							int nCurY = 0;
							int nXLevel = 0;

							for ( int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								if ( nCurX >= nXCount )
								{
									nCurX = 0;
									nXLevel = 0;
									nCurY++;
								}
								DrawTriangleBorder(&oRender, nCurX * dSide / 2, nCurY * sqrt(3.0) * 0.5 * dSide + nXLevel * sqrt(3.0) / 6 * dSide, dAlpha, 1 - nXLevel);
								
								nCurX++;
								nXLevel = (nXLevel + 1) % 2;
							}
						}
						else if ( 3 == m_nFigureType )
						{
							nFullWidth = int(2 * dSide);
							nFullHeight = int(2 * dSide);

							double dRadius = dSide / sqrt(2.0); // радиус описанной окружности
							int nCurX = 0; 
							int nCurY = 0;
							int nXLevel = 0;

							for ( int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								if ( nCurX >= nXCount )
								{
									nCurX = 0;
									nXLevel = 0;
									nCurY++;
								}
								DrawRhombusBorder(&oRender, nCurX * dRadius, nCurY * 2*dRadius + nXLevel * dRadius, dAlpha);
								
								nCurX++;
								nXLevel = (nXLevel + 1) % 2;
							}

						}
						else if ( 4 == m_nFigureType )
						{
							nFullWidth = int(dSide);
							nFullHeight = int(dSide);

							double dXShift = dSide;
							double dYShift = dSide * (12.0 / 25.0);
							int nCurX = 0; 
							int nCurY = 0;
							int nYLevel = 0;

							for ( int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								if ( nCurX >= nXCount )
								{
									nCurX = 0;
									nYLevel = (nYLevel + 1) % 2;
									nCurY++;
								}
								DrawBrickBorder(&oRender, nCurX * dXShift + nYLevel * dXShift / 2, nCurY * dYShift, dAlpha);
								
								nCurX++;
							}

						}
						else if ( 6 == m_nFigureType )
						{
							nFullWidth =  int(2 * dSide);
							nFullHeight = int(2 * dSide);

							double dAngle = -IPP_PI4 + atan(25.0 / 24.0);
							double dDiagonal = dSide * sqrt (1.0 / 4.0 + 144.0 / 625.0);
							double dXShift = dDiagonal * cos(dAngle);
							double dXYShift = dDiagonal * sin(dAngle);

							int nCurX = 0; 
							int nCurY = 0;
							int nYLevel = 0;

							for ( int nIndex = 0; nIndex < nCount; nIndex++ )
							{
								if ( nCurX >= nXCount )
								{
									nCurX = 0;
									nYLevel = (nYLevel + 1) % 2;
									nCurY++;
								}
								DrawInclinedBrickBorder(&oRender, nCurX * dXShift - nCurY * dXYShift, nCurY * dXShift - nCurX * dXYShift, dAlpha);								
								nCurX++;
							}

						}

					}
					return TRUE;
				}
			
			};

			class OldPhoto : public ImageStudio::Caching::CCacheItemPointer
			{
			private:
				class CLocker
				{
				private: CRITICAL_SECTION m_csThread;
				public: CLocker()
						{
							InitializeCriticalSection(&m_csThread);
						}
				public: ~CLocker()
						{
							DeleteCriticalSection(&m_csThread);
						}
				public: void Lock()
						{
							EnterCriticalSection(&m_csThread);
						}
				public: void UnLock()
						{
							LeaveCriticalSection(&m_csThread);
						}
				};
				static CLocker oLocker;
				static   CAtlArray<Gdiplus::Bitmap*>*            GetMasks()
				{
					static  CAtlArray<Gdiplus::Bitmap*>     Masks;
					return &Masks;
				}

				static   int                                     ChangeCounter(bool increase)
				{
					static int  counter = 0;
					if (increase)
						counter++;
					else
						counter--;
					return counter;
				}
								
			public:
				OldPhoto()
				{
				}

				virtual ~OldPhoto()
				{
				}

				BOOL Draw( BYTE* pPixels, int nWidth, int nHeight, int nMask, int whiteBlack,bool vFlip )
				{
					oLocker.Lock();

					if (GetMasks()->GetCount()<1)
						return FALSE;

					nMask = nMask%((int)GetMasks()->GetCount());

					BYTE* pSourceImage = pPixels;

					if (whiteBlack)
						GrayscaleImage(pPixels, nWidth, nHeight, 1.0, 1.0);

					CBuffer<BYTE> oBuffer(nWidth*nHeight*4);
					if( oBuffer.IsEmpty() )
						return FALSE;

					BYTE* pBuffer = oBuffer.GetPtr();

				//	memcpy( pBuffer, pPixels,  nWidth*nHeight*4 );

					Gdiplus::Bitmap oImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pBuffer );
					if( Gdiplus::Ok != oImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Graphics oRender( &oImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;

					if (vFlip)
						GetMasks()->GetAt(nMask)->RotateFlip(Gdiplus::RotateNoneFlipY);

					oRender.SetInterpolationMode( InterpolationModeBilinear );
					oRender.SetSmoothingMode( SmoothingModeAntiAlias );

					oRender.DrawImage(GetMasks()->GetAt(nMask),0,0, nWidth, nHeight);

					{
						BYTE* pRow = pPixels;
						BYTE* pRow2 = pBuffer;
						for( int nRow = 0; nRow < nHeight; ++nRow, pRow += nWidth*4,  pRow2 += nWidth*4)
						{
							BYTE* pPix = pRow;
							BYTE* pPix2 = pRow2;
							for( int nPos = 0; nPos < nWidth; ++nPos, pPix += 4, pPix2 += 4 )
							{
								for (int i=0;i<3;i++)
								{
									int blackLevelLow  = ((int)pPix[i]);// + (int)pPix[1] +(int)pPix[2])/3;
									int blackLevelUp  = ((int)pPix2[i]);// + (int)pPix2[1] +(int)pPix2[2])/3;
									if (blackLevelLow>127)
									{	
										float tmp_val = ((float)(255-blackLevelLow))/127.0f;
										float min_val = (float)(blackLevelLow- (255-blackLevelLow));
										float res_val = ((float)blackLevelUp * tmp_val) + min_val;
										/*pPix[0]  = pPix[1] = */pPix[i] = (int)res_val;

									}
									else
									{
										float tmp_val = ((float)(blackLevelLow))/127.0f;
										float res_val = ((float)blackLevelUp * tmp_val);
										/*pPix[0]  = pPix[1] = */pPix[i] = (int)res_val;
									}
								}
								/*int alphaVal = int(255 - 1.2f*(float)blackLevel);
								if (alphaVal<0)
									pPix[3] = 0;
								else
									pPix[3] = alphaVal;*/
							}
						}
					}

					if (vFlip)
						GetMasks()->GetAt(nMask)->RotateFlip(Gdiplus::RotateNoneFlipY);
						
					//memcpy( pPixels, pBuffer, nWidth*nHeight*4 );
			
					oLocker.UnLock();
					return TRUE;
				}

				static   void InitMasks(const CString& masksFolder)
				{
					if (GetMasks()->GetCount()>1)
					{
						ChangeCounter(true);
						return;
					}
					WIN32_FIND_DATA FindData;
					CString  insideFilesMask = masksFolder+_T("*");
					HANDLE hFindFiles=FindFirstFile(insideFilesMask,&FindData); 

					if (hFindFiles==INVALID_HANDLE_VALUE) return;

					for(;;) 
					{
						if ((_tcscmp(FindData.cFileName,_T("."))!=0) && (_tcscmp(FindData.cFileName,_T(".."))!=0)) 
						{
							if (!(FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) 
							{
								TCHAR *ptr=_tcsrchr(FindData.cFileName,_T('.'));
								if (ptr) 
								{ 
									if (_tcslen(ptr)==4) 
									{
										if ((ptr[1]==_T('m') && ptr[2]==_T('s') && ptr[3]==_T('k')) ||
											(ptr[1]==_T('M') && ptr[2]==_T('S') && ptr[3]==_T('K')))   
										{
											CString sPath1=masksFolder;
											sPath1+=FindData.cFileName; 
											BSTR bstrFilePath = sPath1.AllocSysString();
											Gdiplus::Bitmap* newMask = new Gdiplus::Bitmap(bstrFilePath);
											SysFreeString(bstrFilePath);
											if (newMask && newMask->GetLastStatus() == Ok)
											{
												/*{
													Gdiplus::BitmapData         bitmap_data;
													BYTE*                       mask_bits = NULL;
													newMask->LockBits( NULL, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmap_data );
													mask_bits = (BYTE*)bitmap_data.Scan0;
													int nHeight = newMask->GetHeight();
													int nWidth = newMask->GetWidth();
													if (mask_bits)
													{
														BYTE* pRow = mask_bits;
														for( int nRow = 0; nRow < nHeight; ++nRow, pRow += bitmap_data.Stride )
														{
															BYTE* pPixel = pRow;
															for( int nPos = 0; nPos < nWidth; ++nPos, pPixel += 4 )
															{
																int blackLevel  = ((int)pPixel[0] + (int)pPixel[1] +(int)pPixel[2])/3;
																int alphaVal = int(255 - 1.2f*(float)blackLevel);
																if (alphaVal<0)
																	pPixel[3] = 0;
																else
																	pPixel[3] = alphaVal;
															}
														}
														newMask->UnlockBits(&bitmap_data);
													}
												}*/
												GetMasks()->Add(newMask);
											}
										}
									} 
								}
							}
						}
						if (!FindNextFile(hFindFiles,&FindData)) 
							break; 
					}
					FindClose(hFindFiles);
					ChangeCounter(true);
				}
				static   void FreeMasks()
				{
					if (ChangeCounter(false)==0)
					{
						for (size_t i=0;i<GetMasks()->GetCount();i++)
							delete GetMasks()->GetAt(i);
						GetMasks()->RemoveAll();
					}
				}
			private:

				BOOL GrayscaleImage( BYTE* pBGRASource, int nWidth, int nHeight,double dCompleteness, double dTime )
				{
					IppiSize ImageSize = {nWidth, nHeight};

					Ipp8u* pIntensity = new Ipp8u[nWidth*nHeight];
					Ipp8u* pBuffer = new Ipp8u[nWidth*nHeight*4];
					Ipp32f coeffs[3];
					Ipp8u* pSrc[4];

					if (pBuffer && pIntensity)
					{
						coeffs[0] = 0.11f;
						coeffs[1] = 0.59f;
						coeffs[2] = 0.3f;

						ippiColorToGray_8u_AC4C1R( pBGRASource , 4*nWidth, pIntensity, nWidth, ImageSize, coeffs);


						// noise
						/*if( true )
						{
							unsigned int nSeed = GetTickCount();

							Ipp16s* psChanel = new Ipp16s[nWidth*nHeight];

							ippiConvert_8u16s_C1R(pIntensity, nWidth, psChanel, sizeof(Ipp16s)*nWidth, ImageSize);	
							ippiAddRandUniform_Direct_16s_C1IR(psChanel, sizeof(Ipp16s)*nWidth, ImageSize, 0, Ipp16s(4 * dCompleteness), &nSeed);
							ippiConvert_16s8u_C1R(psChanel, sizeof(Ipp16s)*nWidth, pIntensity, nWidth, ImageSize);	

							delete [] psChanel;
						}

						// мелкие артефакты
						int count = rand() % 7 - 2;
						for( int i = 0; i < count; i++)
						{
							int rnd = rand() % 6;
							int size = 1 + rnd*rnd/4;
							int x = rand() % (nWidth - size);
							int y = rand() % (nHeight - size);

							IppiSize CellSize = {size, size};
							BYTE c = 0;

							if( rand() % 2 )
							{
								ippiMin_8u_C1R( pIntensity + y * nWidth + x, nWidth, CellSize, &c);
								c += 20;
							}
							else
							{
								ippiMax_8u_C1R( pIntensity + y * nWidth + x, nWidth, CellSize, &c);
								c -= 20;
							}

							for( int j = size*size/2; j > 0; --j )
							{
								int xx = rand() % size;
								int yy = rand() % size;
								pIntensity[(y + yy)*nWidth + (x + xx)] = c;
							}
						}*/
						
						// блюр
						if( true )
						{
							Ipp8u* pGauss = new Ipp8u[nWidth*nHeight];
							IppiSize GaussSize = {nWidth - 4, nHeight - 4};
							Ipp8u* pTempPtr = pIntensity + nWidth*2 + 2;

							ippiFilterGauss_8u_C1R( pTempPtr, nWidth, pGauss, nWidth, GaussSize, ippMskSize5x5 );
							ippiCopy_8u_C1R( pGauss, nWidth, pTempPtr, nWidth, GaussSize );

							delete [] pGauss;
						}

						// копирование чёрно-белого изображения
						pSrc[0] = pIntensity;
						pSrc[1] = pIntensity;
						pSrc[2] = pIntensity;
						pSrc[3] = pIntensity;

						ippiCopy_8u_P4C4R(pSrc, nWidth, pBuffer, 4*nWidth, ImageSize);

						int alpha = int((1 - dCompleteness) * 255);
						if( alpha > 255 ) alpha = 255;
						if( alpha < 0 ) alpha = 0;

						ippiAlphaCompC_8u_AC4R( pBGRASource, 4*nWidth, Ipp8u(alpha), pBuffer, 4*nWidth, 255, pBGRASource, 4*nWidth, ImageSize, ippAlphaOver);
					}

					if (pBuffer)
						delete[] pBuffer;
					if (pIntensity)
						delete[] pIntensity;

					return TRUE;
				}

				/*Gdiplus::Bitmap* BitmapFromResource(HINSTANCE hInstance, LPCTSTR szResName, LPCTSTR szResType)
				{
					HRSRC hrsrc = FindResource(hInstance, szResName, szResType);
					if(!hrsrc) return 0;
					// "ненастоящий" HGLOBAL - см. описание LoadResource
					HGLOBAL hgTemp = LoadResource(hInstance, hrsrc);
					DWORD sz = SizeofResource(hInstance, hrsrc);
					void* ptrRes = LockResource(hgTemp);
					HGLOBAL hgRes = GlobalAlloc(GMEM_MOVEABLE, sz);
					if(!hgRes) return 0;
					void* ptrMem = GlobalLock(hgRes);
					// Копируем растровые данные
					CopyMemory(ptrMem, ptrRes, sz);
					GlobalUnlock(hgRes);
					IStream *pStream;
					// TRUE означает освободить память при последнем Release
					HRESULT hr = CreateStreamOnHGlobal(hgRes, TRUE, &pStream);
					if(FAILED(hr))
					{
						GlobalFree(hgRes);
						return 0;
					}
					// Используем загрузку из IStream
					Gdiplus::Bitmap *image = Gdiplus::Bitmap::FromStream(pStream);
					pStream->Release();
					return image;
				}*/


			};
			OldPhoto::CLocker OldPhoto::oLocker;

			class Particles : public ImageStudio::Caching::CCacheItemPointer
			{
			private:
				struct TParticle
				{
				public:				
					double fTimePeriod;
					double fTimeOffset;
					DWORD nBaseRandom;
				};
			
			private:
				CBuffer<TParticle> m_Particles;
				int m_nCountParticles;

				IMediaFrame*  m_pSrcFrame;
				int           m_nSrcAlpha;

			public:
				double m_dDirBaseAngle; // угол направления движения
				double m_dDirDispAngle; // угол отклонения движения

				double m_dMinSize; // базовый масштаб
				double m_dMaxSize; // отклонение от базового масштаба

				double m_dMinLifeTime; // минимальное время жизни частицы
				double m_dMaxLifeTime; // максимальное время жизни частицы

				double m_dMinRotTime;
				double m_dMaxRotTime;

				double m_dMinRotAngle;
				double m_dMaxRotAngle;

				int m_nQuality; // качество отрисовки
				int m_nAbsRot;
				int m_nRotType;
				

			public:
				Particles()
				{
					m_pSrcFrame = NULL;
					
					m_nCountParticles = 0;

					m_dDirBaseAngle = 0;
					m_dDirDispAngle = 0;

					m_dMinSize = 0;
					m_dMaxSize = 0;

					m_dMinLifeTime = 0;
					m_dMaxLifeTime = 0;

					m_nQuality = 0;
					m_nAbsRot  = 0;
					m_nRotType = 0; //(0 - нет вращения, 1 - по часовой стрелке, 2 - против часовой стрелки, 3 - и по и против часовой стрелки, 4 - качание)
				}

				~Particles()
				{
					ReleaseSourceFrame();
				}

				
				BOOL SetSorceFrame( IMediaFrame* pFrame, int nAlpha )
				{
					ReleaseSourceFrame();
					
					if( !pFrame || nAlpha <= 0 )
						return FALSE;

					pFrame->AddRef();

					m_pSrcFrame = pFrame;
					m_nSrcAlpha = min(nAlpha, 255);

					return TRUE;
				}

				void ReleaseSourceFrame()
				{
					if( m_pSrcFrame )
					{
						m_pSrcFrame->Release();
						m_pSrcFrame = NULL;
					}

					m_nSrcAlpha = 255;
				}

	
				BOOL CreateParticles( int nMaxCountParticles )
				{
					m_Particles.Release();

					TParticle* pParticle = m_Particles.Create( nMaxCountParticles );
					if( !pParticle )
						return FALSE;

					m_nCountParticles = nMaxCountParticles;

					for( int i = 0; i < nMaxCountParticles; ++i, ++pParticle )
					{
						pParticle->fTimePeriod = Random( m_dMinLifeTime, m_dMaxLifeTime );
						pParticle->fTimeOffset = pParticle->fTimePeriod * Random();
						pParticle->nBaseRandom = ::rand();
					}
					
					return TRUE;
				}

				BOOL Draw( BYTE* pPixels, int nWidth, int nHeight, double dTime, double dCompleteness )
				{
					if( m_Particles.IsEmpty() )
						return TRUE;

					int nAlpha = int(m_nSrcAlpha * dCompleteness + 0.5);
					if( nAlpha <= 0 )
						return TRUE;

					if( !m_pSrcFrame )
						return TRUE;

					long lSrcWidth;   m_pSrcFrame->get_Width(&lSrcWidth);
					long lSrcHeight;  m_pSrcFrame->get_Height(&lSrcHeight);
					BYTE* pSrcPixels; m_pSrcFrame->get_Buffer(&pSrcPixels);
					
					if( lSrcWidth <= 0 || lSrcHeight <= 0 || !pSrcPixels )
						return TRUE;

					CBuffer<BYTE> oTmpBuffer;
					if( nAlpha < 255 )
					{
						long lSrcSize = lSrcWidth * lSrcHeight;

						if( !oTmpBuffer.Create( lSrcSize * 4 ) )
							return FALSE;
						
						DWORD alpha = DWORD(nAlpha) * 0x8081U;
						BYTE* src = pSrcPixels;
						BYTE* dst = oTmpBuffer.GetPtr();
						
						for( int i = 0; i < lSrcSize; ++i, src += 4, dst += 4 )
						{
							dst[0] = src[0];
							dst[1] = src[1];
							dst[2] = src[2];
							dst[3] = BYTE((src[3] * alpha + 128 * 0x8081U) >> 23);
						}

						pSrcPixels = oTmpBuffer.GetPtr();
					}

					Gdiplus::Bitmap oSrcImage( lSrcWidth, lSrcHeight, lSrcWidth * 4, PixelFormat32bppARGB, pSrcPixels );
					if( Gdiplus::Ok != oSrcImage.GetLastStatus() )
						return FALSE;

					// подготовка рендера
					Gdiplus::Bitmap oDstImage( nWidth, nHeight, nWidth * 4, PixelFormat32bppARGB, pPixels );
					if( Gdiplus::Ok != oDstImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Graphics oRender( &oDstImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;

					InterpolationMode mode;
					switch( m_nQuality )
					{
					case 1:  mode = Gdiplus::InterpolationModeBicubic; break;
					case 2:  mode = Gdiplus::InterpolationModeHighQualityBilinear; break;
					case 3:  mode = Gdiplus::InterpolationModeHighQualityBicubic; break;
					default: mode = Gdiplus::InterpolationModeBilinear;
					}

					oRender.SetInterpolationMode( mode );

					double dCenterX = nWidth  / 2.0;
					double dCenterY = nHeight / 2.0;

					// расчет положения и отрисовка частицы
					TParticle* pParticle = m_Particles.GetPtr();

					double dSrcDiagonal = sqrt(double(lSrcWidth * lSrcWidth + lSrcHeight * lSrcHeight));

					for( int i = 0; i < m_nCountParticles; ++i, ++pParticle )
					{
						if( pParticle->fTimePeriod < IPP_EPS23 )
							continue;

						double time   = dTime + pParticle->fTimeOffset;
						int    period = int(time / pParticle->fTimePeriod);
						
						time -= period * pParticle->fTimePeriod; // abs time 
						double t = time / pParticle->fTimePeriod; // (0 <= t <= 1)

						CRandom oRnd( DWORD(period + pParticle->nBaseRandom) );

						double dSize = oRnd.RandF( m_dMinSize, m_dMaxSize );
						if( dSize < IPP_EPS23 )
							dSize = lSrcWidth;

						double dScale = dSize / lSrcWidth;

						double dDirAngle = oRnd.RandF( m_dDirBaseAngle - m_dDirDispAngle, m_dDirBaseAngle + m_dDirDispAngle );
						double dCos = cos( IPP_PI180 * dDirAngle );
						double dSin = sin( IPP_PI180 * dDirAngle );

						double dDstWidth  = abs(nWidth  * dCos) + abs(nHeight * dSin);
						double dDstHeight = abs(nHeight * dCos) + abs(nWidth  * dSin);
						//double dDstScale  = sqrt((dDstWidth * dDstHeight) / (nWidth * nHeight));

						double dBorder = abs(dSrcDiagonal * dScale);
						double dX = (oRnd.RandF() - 0.5) * dDstWidth;
						double dY = (t - 0.5) * (dDstHeight + dBorder);

						double x = (dX * dCos + dY * dSin) + nWidth / 2.0;
						double y = (dY * dCos - dX * dSin) + nHeight / 2.0;
						double w = lSrcWidth  * dScale;
						double h = lSrcHeight * dScale;

						Gdiplus::RectF rect( float(x - (w - 1) / 2), float(y - (h - 1) / 2), float(w), float(h) );

						if( m_nRotType > 0 && m_nRotType < 5 )
						{
							double dRotAngle = oRnd.RandF( m_dMinRotAngle, m_dMaxRotAngle );
							
							double dRotTime = oRnd.RandF( m_dMinRotTime, m_dMaxRotTime );
							if( dRotTime > IPP_EPS23 )
							{
								double dAng = (360 / dRotTime) * time;

								switch( m_nRotType )
								{
								case 1: dRotAngle +=  dAng; break;
								case 2: dRotAngle += -dAng; break;
								case 3: dRotAngle += (oRnd.Rand( 2 ) > 0) ? dAng : -dAng; break;
								case 4:
									{
										double dRotLength = abs(m_dMaxRotAngle - m_dMinRotAngle);

										if( dRotLength > IPP_EPS23 )
										{
											dRotAngle -= m_dMinRotAngle;
											dRotAngle += (oRnd.Rand( 2 ) > 0) ? dAng : -dAng;
											dRotAngle -= 2 * dRotLength * floor( dRotAngle / (2 * dRotLength) );
											if( dRotAngle > dRotLength )
											{
												dRotAngle = 2 * dRotLength - dRotAngle;
											}
											dRotAngle += m_dMinRotAngle;
										}
									}
								}
							}

							if( !m_nAbsRot )
							{
								dRotAngle += dDirAngle;
							}

							Gdiplus::Matrix m(1, 0, 0, 1, 0, 0);
							m.RotateAt( float(-dRotAngle), Gdiplus::PointF( float(x), float(y) ), Gdiplus::MatrixOrderAppend );

							oRender.SetTransform( &m );
						}
						
						Gdiplus::Status status = oRender.DrawImage( &oSrcImage, rect );
					}

					return TRUE;
				}

			};
		}
	}
}	

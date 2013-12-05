#pragma once

#include <ImageTransformsCoreBGRA.h>
#include "../Common/ImageFileFormatChecker.h"

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			namespace COM
			{
				MediaCore::IAVSUncompressedVideoFrame* COM_CreateFrame()
				{
					MediaCore::IAVSUncompressedVideoFrame* pResult = NULL;
					::CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pResult) );
					return pResult;
				}
				
				int COM_ComputePolyline( double* pPointsCoordinates, Gdiplus::PointF* pPoints, int nPointsCount, double dMetricX, double dMetricY )
				{
					/* @desc: 
					int COM_ComputePolyline(int nHeight, double* pPointsCoordinates, int nPointsCount, double dMetricX, double dMetricY, Gdiplus::PointF*& pPoints)
					{
						производит пересчет координат точек polyline'а из (pPointsCoordinates, nPointsCount)
						в массив (pPoints) для дальнейшей отрисовки 
						при этом учитавается то, что картинка перевернута (nHeight - высота картинки),
						и дополничелтные коэффициенты для пресчета координат (dMetricX, dMetricY)
					}
					*/

					if( NULL == pPointsCoordinates || NULL == pPoints )
						return c_nErrorInvalidPointer;

					for (int index = 0; index < nPointsCount; ++index)
					{
						pPoints[index].X = REAL(dMetricX*pPointsCoordinates[2*index + 0]);
						pPoints[index].Y = REAL(dMetricY*pPointsCoordinates[2*index + 1]);
					}

					return c_nErrorNone;
				}
				int COM_ComputeBorderRectangle(int nWidth, int nHeight, int nType, double dSize, Gdiplus::RectF& oRectangle)
				{
					/* @desc: 
					int COM_ComputeBorderRectangle(int nWidth, int nHeight, int nType, double dSize, Gdiplus::RectF& oRectangle)
					{
						подсчитывает координаты прямоугольника, в который надо будет отрисовать картинку
						с учетом настроек border'а (типа и его размера)
					}
					*/

					if (nType == c_nBorderSolidAll || nType == c_nBorderGradientLightAll || nType == c_nBorderGradientDarkAll || nType == c_nBorderTransparentAll)
					{
						oRectangle.X = (float)dSize;
						oRectangle.Y = (float)dSize;
						oRectangle.Width = (float)max(0, nWidth - 2*dSize);
						oRectangle.Height = (float)max(0, nHeight - 2*dSize);
					}
					else if (nType == c_nBorderSolidLeft || nType == c_nBorderGradientLightLeft || nType == c_nBorderGradientDarkLeft || nType == c_nBorderTransparentLeft)
					{
						oRectangle.X = (float)dSize;
						oRectangle.Y = 0.0f;
						oRectangle.Width = (float)max(0, nWidth - dSize);
						oRectangle.Height = (float)nHeight;
					}
					else if (nType == c_nBorderSolidBottom || nType == c_nBorderGradientLightBottom || nType == c_nBorderGradientDarkBottom || nType == c_nBorderTransparentBottom)
					{
						oRectangle.X = 0.0f;
						oRectangle.Y = (float)dSize;
						oRectangle.Width = (float)nWidth;
						oRectangle.Height = (float)max(0, nHeight - dSize);
					}
					else if (nType == c_nBorderSolidRight || nType == c_nBorderGradientLightRight || nType == c_nBorderGradientDarkRight || nType == c_nBorderTransparentRight)
					{
						oRectangle.X = 0.0f;
						oRectangle.Y = 0.0f;
						oRectangle.Width = (float)max(0, nWidth - dSize);
						oRectangle.Height = (float)nHeight;
					}
					else if (nType == c_nBorderSolidTop || nType == c_nBorderGradientLightTop || nType == c_nBorderGradientDarkTop || nType == c_nBorderTransparentTop)
					{
						oRectangle.X = 0.0f;
						oRectangle.Y = 0.0f;
						oRectangle.Width = (float)nWidth;
						oRectangle.Height = (float)max(0, nHeight - dSize);
					}
					else if (nType == c_nBorderSolidLeftRight || nType == c_nBorderGradientLightLeftRight || nType == c_nBorderGradientDarkLeftRight || nType == c_nBorderTransparentLeftRight)
					{
						oRectangle.X = (float)dSize;
						oRectangle.Y = 0.0f;
						oRectangle.Width = (float)max(0, nWidth - 2*dSize);
						oRectangle.Height = (float)nHeight;
					}
					else if (nType == c_nBorderSolidTopBottom || nType == c_nBorderGradientLightTopBottom || nType == c_nBorderGradientDarkTopBottom || nType == c_nBorderTransparentTopBottom)
					{
						oRectangle.X = 0.0f;
						oRectangle.Y = (float)dSize;
						oRectangle.Width = (float)nWidth;
						oRectangle.Height = (float)max(0, nHeight - 2*dSize);
					}

					return c_nErrorNone;
				}
				
				int COM_DulicateImage(IUnknown* pSource, IUnknown*& pResult, BOOL bCloneInterface)
				{
					/* @desc:
					int COM_DulicateImage(IUnknown* pSource, IUnknown*& pResult, BOOL bCloneInterface)
					{
						для указанного интерфейса (pSource) создает дубликат: увеличивает счетчик, если (bCloneInterface) = TRUE
						или копирует содержимое картинки, если (bCloneInterface) = FALSE
						предполагается, что в (pSource) лежит (IAVSUncompressedVideoFrame)
					}
					*/

					if (NULL == pSource)
						return c_nErrorInvalidPointer;

					if (bCloneInterface)
					{
						pResult = pSource;
						pResult->AddRef();
					}
					else
					{
						if (S_OK != ((MediaCore::IAVSUncompressedVideoFrame*)pSource)->CreateDuplicate(DUBLICATE_TYPE_COPY, (MediaCore::IAVSMediaData**)(&pResult)))
							return c_nErrorInvalidInterface;
					}

					return c_nErrorNone;
				}
				int COM_CopyImagePixels(IUnknown* pSource, IUnknown* pResult)
				{
					/* @desc:
					int COM_CopyImageContent(IUnknown* pSource, IUnknown* pResult)
					{
						копирует сожержимое картинки из указанного интерфейса (pSource) в картинку в (pResult)

						предполагается, что обе картинки существуют, они обе валидны, формат у них один и тот же
						и размер данных один и тот же. 
						
						осуществляется простая проверка на размер памяти (но не более того)
					}
					*/

					if (NULL == pSource || NULL == pResult)
						return c_nErrorInvalidPointer;

					// вытаскиваем из интерфейса uncompressed frame
					MediaCore::IAVSUncompressedVideoFrame* pSourceUncompressedFrame = NULL; pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pSourceUncompressedFrame));
					MediaCore::IAVSUncompressedVideoFrame* pResultUncompressedFrame = NULL; pResult->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pResultUncompressedFrame));

					if (NULL == pSourceUncompressedFrame || NULL == pResultUncompressedFrame)
						return c_nErrorInvalidInterface;

					// запрашиваем размер буффера пикселов - доджны быть одинаковы
					long lSourceBufferSize = 0; pSourceUncompressedFrame->get_BufferSize(&lSourceBufferSize);
					long lResultBufferSize = 0; pResultUncompressedFrame->get_BufferSize(&lResultBufferSize);
					if (lSourceBufferSize != lResultBufferSize || lResultBufferSize < 1)
					{
						pSourceUncompressedFrame->Release();
						pResultUncompressedFrame->Release();

						return c_nErrorInvalidFormat;
					}

					// проверяем буфферы пикселов на валидность
					BYTE* pSourceBuffer = NULL; pSourceUncompressedFrame->get_Buffer(&pSourceBuffer);
					BYTE* pResultBuffer = NULL; pResultUncompressedFrame->get_Buffer(&pResultBuffer);
					if (NULL == pSourceBuffer || NULL == pResultBuffer)
					{
						pSourceUncompressedFrame->Release();
						pResultUncompressedFrame->Release();

						return c_nErrorInvalidFormat;
					}

					// копируем память
					memcpy(pResultBuffer, pSourceBuffer, lSourceBufferSize);

					// удаляем ссылки на созданные обхекты
					pSourceUncompressedFrame->Release();
					pResultUncompressedFrame->Release();

					return c_nErrorNone;
				}
				int COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
				{
					/* @desc:
					int COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight)
					{
						из указанного интерфейса (pSource) вытаскивается картинка (pBGRA, nWidth, nHeight)
						в формате BGRA; если формат интерфейса не CSP_BGRA - конвертация формата не производится
					}
					*/

					if (NULL == pSource)
						return c_nErrorInvalidPointer;

					// вытаскиваем из интерфейса uncompressed frame
					MediaCore::IAVSUncompressedVideoFrame* pUncompressedFrame = NULL;
					pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pUncompressedFrame));
					if (NULL == pUncompressedFrame)
						return c_nErrorInvalidInterface;

					// запрашиваем настройки картинки
					long lWidth = 0; pUncompressedFrame->get_Width(&lWidth);
					long lHeight = 0; pUncompressedFrame->get_Height(&lHeight);
					long lColorSpace = 0; pUncompressedFrame->get_ColorSpace(&lColorSpace);
					long lStride = 0; pUncompressedFrame->get_Stride(0, &lStride);
					long lBufferSize = 0; pUncompressedFrame->get_BufferSize(&lBufferSize);
					BYTE* pSourceBuffer = NULL; pUncompressedFrame->get_Buffer(&pSourceBuffer);

					// проверяем совместимость кадра и формата
					if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || 4*lWidth != lStride || lWidth < 1 || lHeight < 1 || lBufferSize != 4*lWidth*lHeight || !pSourceBuffer)
					{
						pUncompressedFrame->Release();

						return c_nErrorInvalidFormat;
					}

					if( pStride )
						*pStride = (lColorSpace & CSP_VFLIP) ? -lStride : lStride;

					if( pAspectX ) pUncompressedFrame->get_AspectRatioX( pAspectX );
					if( pAspectY ) pUncompressedFrame->get_AspectRatioY( pAspectY );

					// все хорошо, озвращаем наверх нужные параметры
					pBGRA = pSourceBuffer;
					nWidth = lWidth;
					nHeight = lHeight;

					pUncompressedFrame->Release();

					return c_nErrorNone;
				}

				int COM_GetBgraImage( IMediaFrame* source, ImageData& result, int& width, int& height, long* pAspectX = NULL, long* pAspectY = NULL )
				{
					if( !source )
						return c_nErrorInvalidPointer;

					// запрашиваем настройки картинки
					long lWidth      = 0; source->get_Width(&lWidth);
					long lHeight     = 0; source->get_Height(&lHeight);
					long lColorSpace = 0; source->get_ColorSpace(&lColorSpace);
					long lStride     = 0; source->get_Stride(0, &lStride);
					long lBufferSize = 0; source->get_BufferSize(&lBufferSize);
					BYTE* pBuffer    = 0; source->get_Buffer(&pBuffer);

					// проверяем совместимость кадра и формата
					if( !pBuffer || CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || lWidth < 1 || lHeight < 1 || lStride < lWidth * 4 || lBufferSize != lHeight * lStride )
					{
						return c_nErrorInvalidFormat;
					}

					result.pPixels = pBuffer;
					result.nStride = lStride;
					
					if( (lColorSpace & CSP_VFLIP) )
						result.Flip( lHeight );

					width  = lWidth;
					height = lHeight;

					if( pAspectX ) source->get_AspectRatioX( pAspectX );
					if( pAspectY ) source->get_AspectRatioY( pAspectY );

					return c_nErrorNone;
				}

				int COM_GetBgraImage( IUnknown* source, ImageData& result, int& width, int& height, long* pAspectX = NULL, long* pAspectY = NULL )
				{
					if( !source )
						return c_nErrorInvalidPointer;

					IMediaFrame* frame = NULL;
					source->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&frame);
					if( !frame )
						return c_nErrorInvalidInterface;

					int code = COM_GetBgraImage( frame, result, width, height, pAspectX, pAspectY );
					
					frame->Release();

					return code;
				}

				int COM_RemoveColorFromImage(IUnknown* pSource, int nColor)
				{
					/* @desc: 
					int COM_RemoveColorFromImage(IUnknown* pSource, int nColor)
					{
						COM аналог функции BGRA_RemoveColorFromImage
						в качестве картинки для обработки используется BGRA картинка из интерфейса (pSource)
						в случае несовпадения формата картинки с CSP_BGRA конвертация формата не производится
					}
					*/

					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_RemoveColorFromImage(pBGRA, nWidth, nHeight, nColor);
				}
				int COM_Rotate(IUnknown* pSource, IUnknown* pResult, double dResultCenterX, double dResultCenterY, double dAngleInDegrees, double dSourceScaleX, double dSourceScaleY, int nAlpha, BOOL bInterpolate )
				{
					BYTE* pSourceBGRA = NULL;
					int nSourceWidth = 0;
					int nSourceHeight = 0;
					int nSourceStride = 0;

					BYTE* pResultBGRA = NULL;
					int nResultWidth = 0;
					int nResultHeight = 0;
					int nResultStride = 0;

					int nStatus = c_nErrorNone;

					// вытаскиваем из интерфейсов указатели на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface(pSource, pSourceBGRA, nSourceWidth, nSourceHeight, &nSourceStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					nStatus = COM_GetBGRAImageFromInterface(pResult, pResultBGRA, nResultWidth, nResultHeight, &nResultStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию (с учетом переворота по y)
					return ImageStudio::Transforms::Core::BGRA::BGRA_Rotate(pSourceBGRA, nSourceWidth, nSourceHeight, nSourceStride, pResultBGRA, nResultWidth, nResultHeight, nResultStride, dResultCenterX, dResultCenterY, dAngleInDegrees, dSourceScaleX, dSourceScaleY, nAlpha, bInterpolate);
				}
				int COM_RotateSimple(IUnknown* pSource, int eType)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					int nStatus = c_nErrorNone;

					// вытаскиваем из интерфейсов указатели на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					nStatus = BGRA::BGRA_RotateSimple( pBGRA, nWidth, nHeight, nStride, eType );
					if( nStatus != c_nErrorNone )
						return nStatus;

					if( 1 == eType || 3 == eType )
					{
						// вытаскиваем из интерфейса uncompressed frame
						MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
						pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame));
						if( pFrame )
						{
							pFrame->put_Width( nHeight );
							pFrame->put_Height( nWidth );
							pFrame->put_Stride( 0, nHeight * 4 );

							long lAspectX = 0;
							long lAspectY = 0;
							
							pFrame->get_AspectRatioX( &lAspectX );
							pFrame->get_AspectRatioY( &lAspectY );
							pFrame->put_AspectRatioX( lAspectY );
							pFrame->put_AspectRatioY( lAspectX );

							pFrame->Release();
						}
					}

					return c_nErrorNone;
				}
				int COM_DrawMovObject(IUnknown* pSource, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CAlphaComposer* pComposer)
				{
					/* @desc: 
					int COM_AlphaComposition(IUnknown* pSource, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CAlphaComposer* pComposer)
					{
						COM аналог функции BGRA_AlphaComposition
						в качестве картинки для обработки используется BGRA картинка из интерфейса (pSource)
						в случае несовпадения формата картинки с CSP_BGRA конвертация формата не производится
					}
					*/

					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawMovObject(pBGRA, nWidth, nHeight, nColor, nAlpha, nTileSizeX, nTileSizeY, nType, dCompleteness, pComposer);
				}
				int COM_ImageComposition(IUnknown* pSource1, IUnknown* pSource2, IUnknown* pResult, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CImageComposer* pComposer)
				{
					/* @desc: 
					int COM_ImageComposition(IUnknown* pSource1, IUnknown* pSource2, IUnknown* pResult, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CImageComposer* pComposer)
					{
						COM аналог функции BGRA_ImageComposition
						в качестве картинки для обработки используется BGRA картинки из интерфейсов (pSource1, pSource2 и pResult)
						в случае несовпадения формата картинки с CSP_BGRA конвертация формата не производится
					}
					*/

					ImageData src1;
					int width1 = 0;
					int height1 = 0;

					ImageData src2;
					int width2 = 0;
					int height2 = 0;

					ImageData dst;
					int width = 0;
					int height = 0;

					int status = c_nErrorNone;

					// вытаскиваем из интерфейсов указатели на память в формате BGRA
					status = COM_GetBgraImage( pSource1, src1, width1, height1 );
					if( status != c_nErrorNone )
						return status;

					status = COM_GetBgraImage( pSource2, src2, width2, height2 );
					if( status != c_nErrorNone )
						return status;

					status = COM_GetBgraImage( pResult, dst, width, height );
					if( status != c_nErrorNone )
						return status;

					// проверяем, совпадают ли картинки по размеру
					if( width != width1 || width != width2 || height != height1 || height != height2 )
						return c_nErrorInvalidFormat;

					// временно! переворачиваем картинку
					if( src1.nStride > 0 )
					{
						src1.MirrowY( width * 4, height );
						//src1.Flip( height );

						((IMediaFrame*)pSource1)->put_ColorSpace( CSP_BGRA | CSP_VFLIP );
					}

					// временно! переворачиваем картинку
					if( src2.nStride > 0 )
					{
						src2.MirrowY( width * 4, height );
						//src2.Flip( height );

						((IMediaFrame*)pSource2)->put_ColorSpace( CSP_BGRA | CSP_VFLIP );
					}

					// временно! переворачиваем картинку
					if( dst.nStride > 0 )
					{
						dst.MirrowY( width * 4, height );
						//dst.Flip( height );
						
						((IMediaFrame*)pResult)->put_ColorSpace( CSP_BGRA | CSP_VFLIP );
					}

					src1.Normalize( height );
					src2.Normalize( height );
					dst.Normalize( height );

					if (dCompleteness < 0.001)
					{
						src1.CopyTo( dst, width * 4, height );
						return c_nErrorNone;
					}
					else if (dCompleteness > 0.999)
					{
						src2.CopyTo( dst, width * 4, height );
						return c_nErrorNone;
					}

					// выполняем соответствующую RAW функцию
					if( FALSE != pComposer->Transform( src1.pPixels, src2.pPixels, dst.pPixels, width, height, nTileSizeX, nTileSizeY, nType, dCompleteness ) )
						return c_nErrorNone;

					return c_nErrorImageCopositionFailure;
				}
				
				int COM_DrawLine(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dX1, double dY1, double dX2, double dY2)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawLine(pBGRA, nWidth, nHeight, nStride, oSettings, dX1, dY1, dX2, dY2);
				}
				int COM_DrawRectangle(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawRectangle(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_FillRectangle(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillRectangle(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_DrawEllipse(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawEllipse(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_FillEllipse(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillEllipse(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_FillInverseRectangle(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillInverseRectangle(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_FillInverseEllipse(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillInverseEllipse(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom);
				}
				int COM_DrawPolyline(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double* pPointsCoordinates, int nPointsCount, double dMetricX, double dMetricY)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					if( nPointsCount < 1 )
						return c_nErrorInvalidPointsCount;

					// создаем массив точек для отрисовки
					Gdiplus::PointF* pPoints = new Gdiplus::PointF[nPointsCount];
					if (NULL == pPoints)
						return c_nErrorNoMemory;

					nStatus = COM_ComputePolyline( pPointsCoordinates, pPoints, nPointsCount, dMetricX, dMetricY );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					nStatus = ImageStudio::Transforms::Core::BGRA::BGRA_DrawPolyline(pBGRA, nWidth, nHeight, nStride, oSettings, pPoints, nPointsCount);

					// удаляем временные объекты
					delete[] pPoints;

					return nStatus;
				}
				int COM_DrawPolygon(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double* pPointsCoordinates, int nPointsCount, double dMetricX, double dMetricY)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					if( nPointsCount < 1 )
						return c_nErrorInvalidPointsCount;

					// создаем массив точек для отрисовки
					Gdiplus::PointF* pPoints = new Gdiplus::PointF[nPointsCount];
					if (NULL == pPoints)
						return c_nErrorNoMemory;

					nStatus = COM_ComputePolyline( pPointsCoordinates, pPoints, nPointsCount, dMetricX, dMetricY );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					nStatus = ImageStudio::Transforms::Core::BGRA::BGRA_DrawPolygon(pBGRA, nWidth, nHeight, nStride, oSettings, pPoints, nPointsCount);

					// удаляем временные объекты
					delete[] pPoints;

					return nStatus;
				}
				int COM_FillPolygon(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double* pPointsCoordinates, int nPointsCount, double dMetricX, double dMetricY)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					if( nPointsCount < 1 )
						return c_nErrorInvalidPointsCount;

					// создаем массив точек для отрисовки
					Gdiplus::PointF* pPoints = new Gdiplus::PointF[nPointsCount];
					if (NULL == pPoints)
						return c_nErrorNoMemory;

					nStatus = COM_ComputePolyline( pPointsCoordinates, pPoints, nPointsCount, dMetricX, dMetricY );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					nStatus = ImageStudio::Transforms::Core::BGRA::BGRA_FillPolygon(pBGRA, nWidth, nHeight, nStride, oSettings, pPoints, nPointsCount);

					// удаляем временные объекты
					delete[] pPoints;

					return nStatus;
				}
				int COM_DrawPie(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawPie(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int COM_FillPie(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillPie(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int COM_DrawSector(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawSector(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRatio);
				}
				int COM_FillSector(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_FillSector(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRatio);
				}
				int COM_DrawSimplePie(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return COM_DrawPie(pSource, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int COM_FillSimplePie(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return COM_FillPie(pSource, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int COM_DrawSimpleSector(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return COM_DrawSector(pSource, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int COM_FillSimpleSector(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return COM_FillSector(pSource, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int COM_DrawBorder(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, int nType, double dSize, double dMetricX, double dMetricY)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					double dLeft   = 0;
					double dTop    = 0;
					double dRight  = 0;
					double dBottom = 0;

					if (nType == c_nBorderSolidAll || nType == c_nBorderGradientLightAll || nType == c_nBorderGradientDarkAll || nType == c_nBorderTransparentAll)
					{
						dSize *= min( dMetricX, dMetricY ) / 2;

						dLeft   = dSize;
						dTop    = dSize;
						dRight  = nWidth - dSize;
						dBottom = nHeight - dSize;
					}
					else if (nType == c_nBorderSolidLeft || nType == c_nBorderGradientLightLeft || nType == c_nBorderGradientDarkLeft || nType == c_nBorderTransparentLeft)
					{
						dSize *= dMetricX;

						dLeft   = dSize;
						dTop    = 0;
						dRight  = nWidth;
						dBottom = nHeight;
					}
					else if (nType == c_nBorderSolidBottom || nType == c_nBorderGradientLightBottom || nType == c_nBorderGradientDarkBottom || nType == c_nBorderTransparentBottom)
					{
						dSize *= dMetricY;

						dLeft   = 0;
						dTop    = 0;
						dRight  = nWidth;
						dBottom = nHeight - dSize;
					}
					else if (nType == c_nBorderSolidRight || nType == c_nBorderGradientLightRight || nType == c_nBorderGradientDarkRight || nType == c_nBorderTransparentRight)
					{
						dSize *= dMetricX;
						
						dLeft   = 0;
						dTop    = 0;
						dRight  = nWidth - dSize;
						dBottom = nHeight;
					}
					else if (nType == c_nBorderSolidTop || nType == c_nBorderGradientLightTop || nType == c_nBorderGradientDarkTop || nType == c_nBorderTransparentTop)
					{
						dSize *= dMetricY;
						
						dLeft   = 0;
						dTop    = dSize;
						dRight  = nWidth;
						dBottom = nHeight;
					}
					else if (nType == c_nBorderSolidLeftRight || nType == c_nBorderGradientLightLeftRight || nType == c_nBorderGradientDarkLeftRight || nType == c_nBorderTransparentLeftRight)
					{
						dSize *= dMetricX / 2;

						dLeft   = dSize;
						dTop    = 0;
						dRight  = nWidth - dSize;
						dBottom = nHeight;
					}
					else if (nType == c_nBorderSolidTopBottom || nType == c_nBorderGradientLightTopBottom || nType == c_nBorderGradientDarkTopBottom || nType == c_nBorderTransparentTopBottom)
					{
						dSize *= dMetricY / 2;
						
						dLeft   = 0;
						dTop    = dSize;
						dRight  = nWidth;
						dBottom = nHeight - dSize;
					}

					BOOL bLinearGradient = FALSE;

					if( (nType >= c_nBorderGradientLightAll && nType <= c_nBorderGradientLightTopBottom) ||
						(nType >= c_nBorderGradientDarkAll  && nType <= c_nBorderGradientDarkTopBottom) )
					{
						bLinearGradient = TRUE;
					}
					
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawBorder( pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, bLinearGradient );
				}
				int COM_DrawText(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, bool bAlign = false, int nAlignType = 5, double dMargin = 10)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawText(pBGRA, nWidth, nHeight, nStride, oSettings, strText, dLeft, dTop, bAlign, nAlignType, dMargin);
				}
				int COM_DrawText(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawText(pBGRA, nWidth, nHeight, nStride, oSettings, strText, dLeft, dTop, dRight, dBottom);
				}
				int COM_DrawTextScale(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawTextScale(pBGRA, nWidth, nHeight, nStride, oSettings, strText, dLeft, dTop, dRight, dBottom);
				}
				int COM_DrawCredits(IUnknown* pSource, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dWidth, double dHeight, double dCompleteness, int nType)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawCredits(pBGRA, nWidth, nHeight, nStride, oSettings, strText, dLeft, dTop, dWidth, dHeight, dCompleteness, nType);
				}
				
				int COM_DrawImageFromFile(IUnknown* pCanvas, IUnknown* pSource, int nRemoveColor, int nType, int nBackColor, int nAlpha, double dCompleteness, double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0, BOOL bAlign = FALSE, int nAlignType = 4, int nMarginX = 10, int nMarginY = 10, int nSizeType = 0, int nImWidth = 0, int nImHeight = 0 )
				{
					BYTE* pCanvasBGRA = NULL;
					int nCanvasWidth = 0;
					int nCanvasHeight = 0;
					int nCanvasStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pCanvas, pCanvasBGRA, nCanvasWidth, nCanvasHeight, &nCanvasStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					
					BYTE* pSourceBGRA = NULL;
					int nSourceWidth = 0;
					int nSourceHeight = 0;
					int nSourceStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface(pSource, pSourceBGRA, nSourceWidth, nSourceHeight, &nSourceStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					CBuffer<BYTE> pBuffer;

					if( nRemoveColor >= 0 )
					{
						if( !pBuffer.Create( nSourceWidth * nSourceHeight * 4 ) )
							return c_nErrorNoMemory;

						::memcpy( pBuffer.GetPtr(), pSourceBGRA, nSourceWidth * nSourceHeight * 4 );

						pSourceBGRA = pBuffer.GetPtr();

						ImageStudio::Transforms::Core::BGRA::BGRA_RemoveColorFromImage(pSourceBGRA, nSourceWidth, nSourceHeight, nRemoveColor);
					}

					if( dRight < dLeft || dBottom < dTop )
					{
						dRight = dLeft + nSourceWidth;
						dBottom = dTop + nSourceHeight;
					}

					switch ( nSizeType )
					{
					case 0: 
						break;
					case 1: 
						dRight  = dLeft + nImWidth;
						dBottom = dTop  + nImHeight;
						break;
					case 2:
						dRight  = dLeft + nImWidth;
						dBottom = dTop  + double(nImWidth * nSourceHeight) / double(nSourceWidth);
						break;
					case 3:
						dBottom = dTop  + nImHeight;
						dRight  = dLeft + double(nImHeight * nSourceWidth) / double(nSourceHeight);
						break;
					}

					if ( bAlign )
					{
						double dRectHeight = dBottom - dTop;
						double dRectWidth  = dRight - dLeft;

						int nMarginX_new = nMarginX;
						int nMarginY_new = nMarginY;


						if ( 0 != dAngle )
						{
							double dXOld = dRectWidth / 2;
							double dYOld = dRectHeight / 2;


							double dCurAngle = dAngle * 3.141592653589793 / 180;

							double dX1 = fabs( dXOld * cos(dCurAngle) - dYOld * sin(dCurAngle) );
							double dY1 = fabs( dXOld * sin(dCurAngle) + dYOld * cos(dCurAngle) );

							double dX2 = fabs( dXOld * cos(dCurAngle) + dYOld * sin(dCurAngle) );
							double dY2 = fabs( dXOld * sin(dCurAngle) - dYOld * cos(dCurAngle) );


							nMarginX_new += int(max( dX1, dX2 ) - dXOld);
							nMarginY_new += int(max( dY1, dY2 ) - dYOld);

						}


						switch (nAlignType)
						{
						case 0: dLeft = nMarginX_new;                                     dTop = nMarginY_new;                                       break;
						case 1: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = nMarginY_new;                                       break;
						case 2: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = nMarginY_new;                                       break;
						case 3: dLeft = nMarginX_new;                                     dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
						case 4: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
						case 5: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = double(nCanvasHeight) / 2  - dRectHeight / 2;       break;
						case 6: dLeft = nMarginX_new;                                     dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
						case 7: dLeft = double(nCanvasWidth) / 2  - dRectWidth / 2;       dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
						case 8: dLeft = double(nCanvasWidth) - dRectWidth - nMarginX_new; dTop = double(nCanvasHeight) - dRectHeight - nMarginY_new; break;
						}
 
						dRight  = dLeft + dRectWidth;
						dBottom = dTop + dRectHeight;

					}

					return Core::BGRA::BGRA_DrawImageFromFile( pCanvasBGRA, nCanvasWidth, nCanvasHeight, nCanvasStride, pSourceBGRA, nSourceWidth, nSourceHeight, nSourceStride, nType, nBackColor, nAlpha, dCompleteness, dLeft, dTop, dRight, dBottom, dAngle );
				}
				int COM_DrawImageFromFileEx( IUnknown* pSource, IUnknown* pResult, double dSrcX, double dSrcY, double dSrcWidth, double dSrcHeight, double dDstX, double dDstY, double dDstWidth, double dDstHeight, int nSrcColorKey, int nScaleType, double dCropScale, double dAngle, double dAlpha )
				{
					BYTE* pSourceBGRA = NULL;
					int nSourceWidth = 0;
					int nSourceHeight = 0;
					int nSourceStride = 0;
					long lSourceAspectX = 0;
					long lSourceAspectY = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pSourceBGRA, nSourceWidth, nSourceHeight, &nSourceStride, &lSourceAspectX, &lSourceAspectY);
					if (nStatus != c_nErrorNone)
						return nStatus;

					BYTE* pResultBGRA = NULL;
					int nResultWidth = 0;
					int nResultHeight = 0;
					int nResultStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface( pResult, pResultBGRA, nResultWidth, nResultHeight, &nResultStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					if( lSourceAspectX <= 0 ) lSourceAspectX = nSourceWidth;
					if( lSourceAspectY <= 0 ) lSourceAspectY = nSourceHeight;

					double dSrcAspect = double(lSourceAspectX) / lSourceAspectY;

					return Core::BGRA::BGRA_DrawImageFromFileEx( pSourceBGRA, nSourceWidth, nSourceHeight, nSourceStride, pResultBGRA, nResultWidth, nResultHeight, nResultStride, dSrcX, dSrcY, dSrcWidth, dSrcHeight, dDstX, dDstY, dDstWidth, dDstHeight, nSrcColorKey, nScaleType, dCropScale, dAngle, dAlpha, dSrcAspect );
				}

				int COM_DrawFilm(IUnknown* pSource, double dFrame, double dTime,int nSpeed, double dWideFactor, BOOL bVertical, BOOL bDrawBorder, int nBkColor, int nFilmColorMid, int nFilmColorBorder, int nBlendColor, BOOL bZoom, int nAlpha, BOOL bMoveDivider, double dSpeedFactor)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_DrawFilm(pBGRA, nWidth, nHeight, nStride, dFrame, dTime, nSpeed, dWideFactor, bVertical, bDrawBorder, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor, bZoom, nAlpha, bMoveDivider, dSpeedFactor);
				}
				int COM_DrawPuzzle(IUnknown* pSource, ImageStudio::Paint::Effects::Puzzle* pEffect, double dFrame, int nRow, int nCol, double dSideFactor, int nType)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_DrawPuzzle(pBGRA, nWidth, nHeight, nStride, dFrame, nRow, nCol, pEffect, dSideFactor, nType);
				}

				int COM_DrawFog(IUnknown* pSource, int nColor, double dAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;
					
					return BGRA::BGRA_Fog(pBGRA, nWidth, nHeight, nColor, dAlpha);
				}
				int COM_LoadMetaData( const CString& sFilePath, BSTR * bstrMetaDataXML)
				{
					//SimpleTimer timer;

					if( sFilePath.GetLength() <= 0 )
						return c_nErrorNone;

					BSTR bsFilePath = sFilePath.AllocSysString();
					
					ImageFile::IImageMetaData *pImageMetaData;
					::CoCreateInstance( ImageFile::CLSID_ImageFile3 , NULL, CLSCTX_INPROC, ImageFile::IID_IImageMetaData, (void**)&pImageMetaData );

					if ( !pImageMetaData )
						return FALSE;

					pImageMetaData->MetaDataFromFile(bsFilePath);
			
					::SysFreeString(bsFilePath);

					pImageMetaData->GetMetaData(bstrMetaDataXML);


					RELEASEINTERFACE(pImageMetaData);
					return c_nErrorNone;
				}

				BOOL COM_LoadImage(const CString& sFilePath,_ImageFileFormats formatInput,   int nFrame, ImageStudio::Core::Image * oImage, BOOL & bVerFlip, ImageStudio::IO::CMetaFile** pResultMeta )
				{
					int nFormatOutput = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
			
					BOOL bLoading = FALSE;
					switch( formatInput )
					{
						case c_Gdiplus:
						case c_Bmp:
						case c_Gif:
						case c_Tiff:
						case c_Png:
						case c_Ico:
						case c_Jpg:
						{
							int tableCX[] ={	//index == formatInput
								CXIMAGE_FORMAT_UNKNOWN,
								CXIMAGE_FORMAT_UNKNOWN,
								CXIMAGE_FORMAT_BMP,
								CXIMAGE_FORMAT_JPG,
								CXIMAGE_FORMAT_GIF,
								CXIMAGE_FORMAT_TIF,
								CXIMAGE_FORMAT_PNG,
								CXIMAGE_FORMAT_ICO,
								CXIMAGE_FORMAT_UNKNOWN,
								CXIMAGE_FORMAT_UNKNOWN,
								CXIMAGE_FORMAT_TGA,
								CXIMAGE_FORMAT_PCX,
								CXIMAGE_FORMAT_UNKNOWN
							};
							int cxType	= CXIMAGE_FORMAT_UNKNOWN;
							if (formatInput < sizeof(tableCX)/sizeof(int))cxType = tableCX[formatInput];
							bLoading = IO::Utils::FromFile( sFilePath, nFormatOutput, oImage, nFrame, 0, 0, cxType );
							break;
						}
					case c_Psd:
						{
							ImageStudio::IO::Psd oReaderPsd;
							bLoading = oReaderPsd.FromFile( sFilePath, nFormatOutput, oImage );
							break;
						}
					case c_Tga:
						{
							ImageStudio::IO::Tga oReaderTga;
							bLoading = oReaderTga.FromFile( sFilePath, nFormatOutput, oImage );
							break;
						}
					case c_Pcx:
						{
							ImageStudio::IO::Pcx oReaderPcx;
							bLoading = oReaderPcx.FromFile( sFilePath, nFormatOutput, oImage );
							break;
						}
					case c_Ras:
						{
							ImageStudio::IO::Ras oReaderRas;
							bLoading = oReaderRas.FromFile( sFilePath, nFormatOutput, oImage );
							break;
						}
					case c_WebShots:
						{
							ImageStudio::IO::WebShots::Wb1 oReaderWB1;
							bLoading = oReaderWB1.FromFile( sFilePath, nFormatOutput,oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_iPode:
						{
							ImageStudio::IO::Ithmb oReaderIPod;
							bLoading = oReaderIPod.FromFile( sFilePath, nFormatOutput, oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_Jpeg2000:
						{
							ImageStudio::IO::Jp2 oReaderJp2;
							bLoading = oReaderJp2.FromFile( sFilePath, nFormatOutput, oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_Metafile:
						{
							if( !pResultMeta )
							{
								ImageStudio::IO::Meta oReaderMeta;
								bLoading = oReaderMeta.FromFile( sFilePath, nFormatOutput, oImage );
							}
							else
							{
								ImageStudio::IO::CMetaFile oMetaFile;
								bLoading = oMetaFile.LoadFromFile( sFilePath );
								
								if( bLoading )
								{
									ImageStudio::IO::CMetaFile* pMeta = new ImageStudio::IO::CMetaFile( oMetaFile );
									if( pMeta )
										*pResultMeta = pMeta;
									else
										bLoading = FALSE;
								}
							}
							break;
						}
					case c_RawFotos:
						{
							const TCHAR * settingsXML = _T("<ImageRaw3 mode=\"0\" xmlns=\"http://tempuri.org/ImageRaw3Options.xsd\"> \
									<SimpleMode ver=\"1\"><CameraWhiteBalance use=\"1\"/><CameraMatrix use=\"0\"/> \
									</SimpleMode></ImageRaw3>");

							ImageStudio::IO::Raw oRenderRaw;
							bLoading = oRenderRaw.FromFile( sFilePath, nFormatOutput, oImage, settingsXML);
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_Wbz:
						{
							ImageStudio::IO::WebShots::Wbz oRenderWbz;
							bLoading = oRenderWbz.FromFile( sFilePath, nFormatOutput, oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_Wbc:
						{
							ImageStudio::IO::WebShots::Wbc oRenderWbc;
							bLoading = oRenderWbc.FromFile( sFilePath, nFormatOutput, oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					case c_Svm:
						{
							ImageStudio::IO::Svm oRenderSvm;
							bLoading = oRenderSvm.FromFile( sFilePath, nFormatOutput, oImage );
							bVerFlip = !(bVerFlip);
							break;
						}
					};

					return bLoading;
				}

				int COM_LoadImage( IUnknown** pResult, const CString& sFilePath, int nFrame, BOOL bVerFlip = FALSE, IO::CMetaFile** pResultMetaFile = NULL)
				{
					if( sFilePath.GetLength() <= 0 )
						return c_nErrorNone;

					const int c_nCountFileFormats = sizeof(tableSupportImageFormats) / sizeof(tableSupportImageFormats[0]);
					
					_ImageFileFormats eFileType		= c_Unknown;

					int nPos = sFilePath.ReverseFind( _T('.') );
					if( nPos > 0 )
					{
						CString sFileEx = sFilePath.Right( sFilePath.GetLength() - nPos - 1 );
						sFileEx.MakeLower();

						for( int i = 1; i < c_nCountFileFormats; i++ )
						{
							if( sFileEx == tableSupportImageFormats[i].str )
							{
								eFileType	= tableSupportImageFormats[i].format;
								break;
							}
						}
					}
					CImageFileFormatChecker checker(sFilePath);

					ImageStudio::Core::Image oImage;
					BOOL bLoading = FALSE;

					if (checker.eFileType == eFileType && checker.eFileType!=c_Unknown)
					{
						//расширение и проверка совпали
						bLoading =COM_LoadImage( sFilePath, eFileType, nFrame, &oImage, bVerFlip, pResultMetaFile);
					}
					else if (checker.eFileType || eFileType)
					{
						if (eFileType)
						{
							bLoading =COM_LoadImage( sFilePath,eFileType,nFrame,&oImage, bVerFlip,pResultMetaFile);
						}
						
						if (!bLoading && checker.eFileType)
						{
							bLoading =COM_LoadImage( sFilePath,checker.eFileType, nFrame,&oImage, bVerFlip,pResultMetaFile);
						}					
					}


					if( !oImage.IsValid() )
						return c_nErrorInvalidFormat;

					if( pResult )
					{
						if( !oImage.ToMediaData( pResult, bVerFlip) )
							return c_nErrorInvalidFormat;
					}

					return c_nErrorNone;
				}
				int COM_LoadRaw( IUnknown** pResult, const CString& sFilePath, BOOL bVerFlip = FALSE, const CString& strSettingXML = NULL)
				{
					int nFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
					
					ImageStudio::Core::Image oImage;
					
					ImageStudio::IO::Raw    oRenderRaw;

					do
					{
							if (oRenderRaw.FromFile( sFilePath, nFormat, &oImage, strSettingXML))
							{
								bVerFlip = !(bVerFlip);
								break;
							}


					} while( false );

					
					if( !oImage.IsValid() )
						return c_nErrorInvalidFormat;
					
					if( pResult )
					{
						if( !oImage.ToMediaData( pResult, bVerFlip) )
							return c_nErrorInvalidFormat;
					}
					
					return c_nErrorNone;
				}
				int COM_SaveImageAsMetafile( IUnknown* pSource, const CString& sFilePath, BOOL bEnhancedMetaFile )
				{
					if( !pSource )
						return c_nErrorInvalidInterface;

					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					nStatus = c_nErrorNone;

					try 
					{
						// create file
						HDC metafileDC = 0;
						if (!bEnhancedMetaFile)
							metafileDC = CreateMetaFile(/*OLE2T*/(sFilePath));
						else
							metafileDC = CreateEnhMetaFile(0, /*OLE2T*/(sFilePath), 0, 0);

						if (!metafileDC)
							throw 0;		// error creating metafile

						// make BITMAPINFO
						int nScanLine = (nWidth * 3 + 3) & ~3;
						BITMAPINFO* pBmpInfo = (BITMAPINFO*)GlobalAlloc(GPTR, sizeof(BITMAPINFOHEADER) + nScanLine * nHeight);
						if( !pBmpInfo )
							throw 0;

						// setup bitmap header
						pBmpInfo->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
						pBmpInfo->bmiHeader.biWidth       = nWidth;
						pBmpInfo->bmiHeader.biHeight      = -nHeight;
						pBmpInfo->bmiHeader.biPlanes      = 1;
						pBmpInfo->bmiHeader.biBitCount    = 24;
						pBmpInfo->bmiHeader.biCompression = BI_RGB;
						pBmpInfo->bmiHeader.biSizeImage   = nScanLine * nHeight;

						// setup bitmap data
						BYTE* pDst = ((BYTE*)pBmpInfo) + sizeof(BITMAPINFOHEADER);
						nStride -= nWidth * 4;
						nScanLine -= nWidth * 3;

						for( int row = 0; row < nHeight; ++row, pBGRA += nStride, pDst += nScanLine )
						{
							for( int pos = 0; pos < nWidth; ++pos, pBGRA += 4, pDst += 3 )
							{
								pDst[0] = pBGRA[0];
								pDst[1] = pBGRA[1];
								pDst[2] = pBGRA[2];
							}
						}

						// draw bitmap in metafile
						SetDIBitsToDevice(metafileDC, 0, 0, nWidth, nHeight, 0, 0, 0, nHeight, ((BYTE*)pBmpInfo) + sizeof(BITMAPINFOHEADER), pBmpInfo, DIB_RGB_COLORS);

						// close file and delete metafile handle
						if (!bEnhancedMetaFile)
							DeleteMetaFile(CloseMetaFile(metafileDC));
						else
							DeleteEnhMetaFile(CloseEnhMetaFile(metafileDC));

						// delete image
						GlobalFree(pBmpInfo);
					}
					catch (...)
					{
						nStatus = c_nErrorInvalidFormat;
					}

					return nStatus;
				}
				
				int COM_DrawText(IUnknown* pSource, ImageStudio::Paint::Text::CTextPainter* pPainter, double dFrame)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawText(pBGRA, nWidth, nHeight, nStride, pPainter, dFrame);
				}
				int COM_DrawText2(IUnknown* pSource, ImageStudio::Paint::Text::CTextPainter2* pPainter, double dFrame)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return ImageStudio::Transforms::Core::BGRA::BGRA_DrawText2(pBGRA, nWidth, nHeight, nStride, pPainter, dFrame);
				}

				int COM_GetAutoBrightness( IUnknown* pSource, double* Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoBrightness( pBGRA, nWidth, nHeight, Level );
				}				
				
				int COM_GetAutoContrast( IUnknown* pSource, double* Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface( pSource, pBGRA, nWidth, nHeight );
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoContrast( pBGRA, nWidth, nHeight, Level );
				}				
				int COM_GetAutoLevels( IUnknown* pSource, double* LevelR, double* LevelG, double* LevelB )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoLevels( pBGRA, nWidth, nHeight, LevelR, LevelG, LevelB );
				}
				int COM_GetAutoGamma( IUnknown* pSource, double* Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoGamma( pBGRA, nWidth, nHeight, Level );
				}
				int COM_GetAutoHue( IUnknown* pSource, double* Angle )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoHue( pBGRA, nWidth, nHeight, Angle );
				}
				int COM_GetAutoSaturation( IUnknown* pSource, double* Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoSaturation( pBGRA, nWidth, nHeight, Level );
				}
				int COM_GetAutoPosterize( IUnknown* pSource, double* Levels )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_GetAutoPosterize( pBGRA, nWidth, nHeight, Levels );
				}
				
				int COM_AdjustBrightness( IUnknown* pSource, double Frame, long Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustBrightness( pBGRA, nWidth, nHeight, Frame, Level );
				}
				int COM_AdjustBrightnessEx( IUnknown* pSource, double Frame, long LevelR, long LevelG, long LevelB )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustBrightnessEx( pBGRA, nWidth, nHeight, Frame, LevelR, LevelG, LevelB );
				}

				int COM_AdjustContrast( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustContrast( pBGRA, nWidth, nHeight, Frame, Level );
				}

				int COM_AdjustGamma( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight );
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustGamma( pBGRA, nWidth, nHeight, Frame, Level );
				}

				int COM_AdjustEqualize( IUnknown* pSource, double Frame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustEqualize( pBGRA, nWidth, nHeight, Frame );
				}

				int COM_AdjustGrayscale( IUnknown* pSource, double Frame, BOOL Desaturate )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustGrayscale( pBGRA, nWidth, nHeight, Frame, Desaturate );
				}

				int COM_AdjustInvert( IUnknown* pSource, double Frame, BOOL bOverGray )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustInvert( pBGRA, nWidth, nHeight, Frame, bOverGray );
				}

				int COM_AdjustAutobrightness( IUnknown* pSource, double Frame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustAutobrightness( pBGRA, nWidth, nHeight, Frame );
				}

				int COM_AdjustAutocontrast( IUnknown* pSource, double Frame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustAutocontrast( pBGRA, nWidth, nHeight, Frame );
				}

				int COM_AdjustAutolevels( IUnknown* pSource, double Frame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustAutolevels( pBGRA, nWidth, nHeight, Frame );
				}

				int COM_AdjustHue( IUnknown* pSource, double Frame, double Angle )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustHue( pBGRA, nWidth, nHeight, Frame, Angle );
				}

				int COM_AdjustSaturation( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustSaturation( pBGRA, nWidth, nHeight, Frame, Level );
				}

				int COM_AdjustPosterize( IUnknown* pSource, double Frame, double Levels )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustPosterize( pBGRA, nWidth, nHeight, Frame, Levels );
				}

				int COM_AdjustTemperature( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustTemperature( pBGRA, nWidth, nHeight, Frame, Level );
				}

				int COM_AdjustColorize( IUnknown* pSource, double Frame, long Type, long Red, long Green, long Blue )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustColorize( pBGRA, nWidth, nHeight, Frame, Type, Red, Green, Blue );
				}

				int COM_AdjustThreshold( IUnknown* pSource, double Frame, long Level, BOOL Grayscale )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustThreshold( pBGRA, nWidth, nHeight, Frame, Level, Grayscale );
				}
				int COM_AdjustSepia( IUnknown* pSource, float fLevel, float fRed, float fGreen, float fBlue )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustSepia(pBGRA, nWidth, nHeight, fLevel, fRed, fGreen, fBlue);
				}

				int COM_AdjustWhiteBalance( IUnknown* pSource, double dFrame, int nRed, int nGreen, int nBlue )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustWhiteBalance(pBGRA, nWidth, nHeight, dFrame, nRed, nGreen, nBlue);
				}

				int COM_AdjustAutoWhiteBalance( IUnknown* pSource, double Frame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					return BGRA::BGRA_AdjustAutoWhiteBalance( pBGRA, nWidth, nHeight, Frame );
				}

				int COM_AdjustClarity( IUnknown* pSource, double dFrame, double dLevel, double dRadius, double dThreshold )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustClarity(pBGRA, nWidth, nHeight, dFrame, dLevel, dRadius, dThreshold);
				}

				int COM_AdjustVibrance( IUnknown* pSource, double dFrame, double dLevel )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustVibrance(pBGRA, nWidth, nHeight, dFrame, dLevel);
				}

				int COM_AdjustRecovery( IUnknown* pSource, double Frame, int nLevel )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustRecovery(pBGRA, nWidth, nHeight, Frame, nLevel);
				}
				int COM_AdjustFillLight( IUnknown* pSource, double Frame, int nLevel )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustFillLight(pBGRA, nWidth, nHeight, Frame, nLevel);
				}
				int COM_AdjustExposure( IUnknown* pSource, double Frame, int nLevel )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustExposure(pBGRA, nWidth, nHeight, Frame, nLevel);
				}
				int COM_AdjustBlacks( IUnknown* pSource, double Frame, int nLevel )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustBlacks(pBGRA, nWidth, nHeight, Frame, nLevel);
				}
				int COM_AdjustHighLight( IUnknown* pSource, double Frame, int nFillLight, int nExposure )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustHighLight(pBGRA, nWidth, nHeight, Frame, nFillLight, nExposure);
				}
				int COM_AdjustShadows( IUnknown* pSource, double Frame, int nBlacks, int nRecovery )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_AdjustShadows(pBGRA, nWidth, nHeight, Frame, nBlacks, nRecovery);
				}

				
				int COM_EffectBlur( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectBlur(pBGRA, nWidth, nHeight, Frame, Level);
				}

				int COM_EffectSharpen( IUnknown* pSource, double Frame, double Level )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectSharpen(pBGRA, nWidth, nHeight, Frame, Level);
				}
				int COM_EffectMosaic( IUnknown* pSource, double Frame, long Size, BOOL Simple )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectMosaic(pBGRA, nWidth, nHeight, Frame, Size, Simple);
				}
				int COM_EffectGaussianBlur( IUnknown* pSource, double Frame, long Size )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectGaussianBlur(pBGRA, nWidth, nHeight, Frame, Size );
				}
				int COM_EffectNoise( IUnknown* pSource, double Frame, double Level, BOOL Mono, DWORD BaseRnd )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectNoise(pBGRA, nWidth, nHeight, Frame, Level, Mono, BaseRnd );
				}
				int COM_EffectDiffuse( IUnknown* pSource, double Frame, double Distance, DWORD BaseRnd )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectDiffuse(pBGRA, nWidth, nHeight, Frame, Distance, BaseRnd );
				}
				int COM_EffectMotionBlur( IUnknown* pSource, double Frame, double Angle, double Distance )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectMotionBlur(pBGRA, nWidth, nHeight, Frame, Angle, Distance );
				}
				int COM_EffectEmboss( IUnknown* pSource, double Frame, double Angle, double Distance, double Amount, BOOL bGrayScale )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectEmboss(pBGRA, nWidth, nHeight, Frame, Angle, Distance, Amount, bGrayScale );
				}
				int COM_EffectMinimal( IUnknown* pSource, double Frame, long Size )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectMinimal(pBGRA, nWidth, nHeight, Frame, Size );
				}
				int COM_EffectMaximal( IUnknown* pSource, double Frame, long Size )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectMaximal(pBGRA, nWidth, nHeight, Frame, Size );
				}

				int COM_EffectDeinterlace( IUnknown* pSource, double Frame, BOOL bEven, BOOL bInterpolate )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectDeinterlace(pBGRA, nWidth, nHeight, Frame, bEven, bInterpolate );
				}
				int COM_EffectMedian( IUnknown* pSource, double Frame, long MedianType )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectMedian(pBGRA, nWidth, nHeight, Frame, MedianType );
				}
				int COM_EffectChromoKeyRange(IUnknown* pSource, int nColor1, int nThreshold1, int nColor2, int nThreshold2, double dFrame, BOOL bComposeAlpha, int EdgesSmooth)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_EffectChromoKeyRange(pBGRA, nWidth, nHeight, nColor1, nThreshold1, nColor2, nThreshold2, dFrame, bComposeAlpha, EdgesSmooth);
				}
				int COM_EffectChromoKeyLevel(IUnknown* pSource, int nColor, int nThreshold, double dFrame, BOOL bComposeAlpha, int EdgesSmooth)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_EffectChromoKeyLevel(pBGRA, nWidth, nHeight, nColor, nThreshold, dFrame, bComposeAlpha, EdgesSmooth);
				}
				int COM_EffectCinema(IUnknown* pSource, Paint::Effects::Cinema* pEffect, double dTime, double dFrame, int nBrightness, int nContrast )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_EffectCinema(pBGRA, nWidth, nHeight, nStride, pEffect, dTime, dFrame, nBrightness, nContrast);
				}

				int COM_EffectGlass( IUnknown* pSource, double Frame, long Radius )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectGlass(pBGRA, nWidth, nHeight, Frame, Radius );
				}
				int COM_EffectGlassMosaic( IUnknown* pSource, double Frame, double Angle, double dSizeX, double dSizeY, double dOffsetX, double dOffsetY, double Curvature, double Smooth, int eQuality )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectGlassMosaic(pBGRA, nWidth, nHeight, nStride, eQuality, dSizeX, dSizeY, dOffsetX, dOffsetY, Curvature, Frame, Angle, Smooth);
				}
				int COM_EffectPencilSketch( IUnknown* pSource, double Frame, int Level, int nBrightness, int nContrast, int nLengthFactor )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectPencilSketch(pBGRA, nWidth, nHeight, Frame, Level, nBrightness, nContrast, nLengthFactor);
				}
				int COM_EffectWaterColor( IUnknown* pSource, double Frame, long BrashSize, long Coarseness )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectWaterColor(pBGRA, nWidth, nHeight, Frame, BrashSize, Coarseness);
				}
				int COM_EffectGlow( IUnknown* pSource, double Frame, long Radius )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectGlow(pBGRA, nWidth, nHeight, Frame, Radius);
				}
				int COM_EffectTVSimulation( IUnknown* pSource, Paint::Effects::TVSimulator* pEffect, double dTime, double dFrame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectTVSimulation(pBGRA, nWidth, nHeight, nStride, pEffect, dTime, dFrame);
				}
				int COM_EffectGrayOut( IUnknown* pSource, double Frame, int Color, int Filter, int LevelR, int LevelG, int LevelB, int Threshold, int nType )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectGrayOut(pBGRA, nWidth, nHeight, Frame, Color, Filter, LevelR, LevelG, LevelB, Threshold, nType);
				}
				int COM_EffectWind( IUnknown* pSource, double Frame, long Length, long Density )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectWind(pBGRA, nWidth, nHeight, Frame, Length, Density);
				}
				int COM_EffectNewsprint( IUnknown* pSource, double Frame, int Level, int Filter )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectNewsprint(pBGRA, nWidth, nHeight, Frame, Level, Filter);
				}
				int COM_EffectSnow( IUnknown* pSource, Paint::Effects::Snow* pEffect, double dTime, double dFrame, BOOL bSnowDrift, double dSnowDriftSpeedFactor )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectSnow(pBGRA, nWidth, nHeight, nStride, pEffect, dTime, dFrame, bSnowDrift, dSnowDriftSpeedFactor);
				}
				int COM_EffectParticles( IUnknown* pSource, Paint::Effects::Particles* pEffect, double dTime, double dFrame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectParticles( pBGRA, nWidth, nHeight, pEffect, dTime, dFrame );
				}

				int COM_EffectJpegCompression( IUnknown* pSource, double dFrame, int nQuality )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectJpegCompression(pBGRA, nWidth, nHeight, dFrame, nQuality );
				}
				int COM_EffectSoftFocus( IUnknown* pSource, double Frame, double dBlurSize, double dIntensity, int Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectSoftFocus(pBGRA, nWidth, nHeight, Frame, dBlurSize, dIntensity, Type );
				}
				int COM_EffectOldPhoto( IUnknown* pSource, double Frame, Paint::Effects::OldPhoto* pMaskPainter, int nMask, int nClarity, int WhiteBlack )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectOldPhoto(pBGRA, nWidth, nHeight, pMaskPainter, Frame, nMask, WhiteBlack,nStride<0);
				}
				int COM_EffectVignette( IUnknown* pSource, double Frame, double dCenterX, double dCenterY, double dRadius1X, double dRadius1Y, double dRadius2X, double dRadius2Y, double dAngle, double dIntensity )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectVignette(pBGRA, nWidth, nHeight, Frame, dCenterX, dCenterY, dRadius1X, dRadius1Y, dRadius2X, dRadius2Y, dAngle, dIntensity );
				}
				int COM_EffectPostCrop( IUnknown* pSource, double Frame, double dAmount, double dMidpoint, double dRoundness, double dFeather )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectPostCrop(pBGRA, nWidth, nHeight, Frame, dAmount, dMidpoint, dRoundness, dFeather );
				}
				int COM_EffectSetAlphaChannel( IUnknown* pSource, IUnknown* pMask, double Frame, int nType, double dDstX, double dDstY, double dDstWidth, double dDstHeight )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					BYTE* pMaskBGRA = NULL;
					int nMaskWidth = 0;
					int nMaskHeight = 0;
					int nMaskStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface(pMask, pMaskBGRA, nMaskWidth, nMaskHeight, &nMaskStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectSetAlphaChannel(pBGRA, nWidth, nHeight, Frame, pMaskBGRA, nMaskWidth, nMaskHeight, nType, dDstX, dDstY, dDstWidth, dDstHeight );
				}
				int COM_EffectZoomingSphere( IUnknown* pSource, double Frame, double Power )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectZoomingSphere(pBGRA, nWidth, nHeight, Frame, Power );
				}
				int COM_EffectWaterMirrow( IUnknown* pSource, double Frame, BYTE* pMask, double m[9], int alpha1, int alpha2 )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectWaterMirrow( pBGRA, nWidth, nHeight, Frame, pMask, m, alpha1, alpha2 );
				}
				int COM_EffectColorExchange( IUnknown* pSource, double Frame, int nColor1, int nColor2 )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectColorExchange( pBGRA, nWidth, nHeight, Frame, nColor1, nColor2 );
				}
				int COM_EffectShapeMosaic( IUnknown* pSource, double Frame, int nMosaicSize )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectShapeMosaic( pBGRA, nWidth, nHeight, Frame, nMosaicSize );
				}
				int COM_EffectCanvas( IUnknown* pSource, double Frame, double dAlpha, DWORD BaseRnd )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectCanvas( pBGRA, nWidth, nHeight, Frame, dAlpha, BaseRnd );
				}
				int COM_EffectAnaglyph3d( IUnknown* pSource, double Frame, double dShiftPercent, int nMixType )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_EffectAnaglyph3d( pBGRA, nWidth, nHeight, Frame, dShiftPercent, nMixType );
				}
			
				int COM_TransformFlip(IUnknown* pSource, double dFrame, BOOL Vertical, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformFlip(pBGRA, nWidth, nHeight, dFrame, Vertical, nBackColor, nBackAlpha );
				}
				int COM_TransformFlip2(IUnknown* pSource, double dFrame, int nDir, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformFlip2(pBGRA, nWidth, nHeight, dFrame, nDir, nBackColor, nBackAlpha );
				}
				int COM_TransformPerspective(IUnknown* pSource, double dFrame, BOOL Vertical, double Angle, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformPerspective(pBGRA, nWidth, nHeight, dFrame, Vertical, Angle, nBackColor, nBackAlpha );
				}
				int COM_TransformSkew(IUnknown* pSource, double dFrame, BOOL Vertical, double Angle, int nBackColor, int nBackAlpha)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformSkew(pBGRA, nWidth, nHeight, dFrame, Vertical, Angle, nBackColor, nBackAlpha );
				}
				int COM_TransformShift(IUnknown* pSource, double dFrame, long X, long Y, int nBackColor, int nBackAlpha)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride );
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformShift(pBGRA, nWidth, nHeight, nStride, dFrame, X, Y, nBackColor, nBackAlpha );
				}
				int COM_TransformRotate(IUnknown* pSource, double dFrame, double Angle, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformRotate(pBGRA, nWidth, nHeight, dFrame, Angle, nBackColor, nBackAlpha );
				}
				int COM_TransformRotateMedia(IUnknown* pSource, IUnknown** pResult, double dFrame, double Angle, int nBackColor, int nBackAlpha, BOOL bNewSize )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformRotateMedia(pBGRA, nWidth, nHeight, nStride, pResult, dFrame, Angle, nBackColor, nBackAlpha, bNewSize );
				}
				int COM_TransformResample(IUnknown* pSource, double dFrame, long Times)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformResample(pBGRA, nWidth, nHeight, dFrame, Times );
				}
				int COM_TransformZoom( IUnknown* pSource, double dScale, double dOffsetX, double dOffsetY, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return BGRA::BGRA_TransformZoom( pBGRA, nWidth, nHeight, dScale, dScale, dOffsetX, dOffsetY, nBackColor, nBackAlpha );
				}
				int COM_TransformMirror(IUnknown* pSource, double dFrame, int nType, double dSize)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					dSize *= dFrame;
					return BGRA::BGRA_TransformMirror(pBGRA, nWidth, nHeight, nStride, nType, dSize);
					//return BGRA::BGRA_TransformMirrorIPP(pBGRA, nWidth, nHeight, dFrame, nType, dSize);
				}
				int COM_TransformResizeMedia(IUnknown* pSource, IUnknown** pResult, long nNewWidth, long nNewHeight, long Color, long Type, double dCropZoom, int alpha = 255, int nSrcX = 0, int nSrcY = 0, int nSrcWidth = 0, int nSrcHeight = 0 )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// получаем аспект
					double dAspect = 1;
					
					MediaCore::IAVSUncompressedVideoFrame* pUncompressedFrame = NULL;
					pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pUncompressedFrame));

					long lAspectX = 0; pUncompressedFrame->get_AspectRatioX(&lAspectX);
					long lAspectY = 0; pUncompressedFrame->get_AspectRatioY(&lAspectY);

					if( lAspectX > 0 && lAspectY > 0 )
					{
						dAspect = (double)lAspectX / lAspectY;
					}
					else
					{
						dAspect = (double)nWidth / nHeight;
					}

					pUncompressedFrame->Release();

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformResizeMedia(pBGRA, nWidth, nHeight, nStride, pResult, nNewWidth, nNewHeight, Color, Type, dAspect, dCropZoom, alpha, nSrcX, nSrcY, nSrcWidth, nSrcHeight );
				}
				int COM_TransformTwirl(IUnknown* pSource, double Frame, double Angle, double Degree, double Zoom, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformTwirl(pBGRA, nWidth, nHeight, Frame, Angle, Degree, Zoom, nBackColor, nBackAlpha);
				}
				int COM_TransformSphere(IUnknown* pSource, double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree, int nBackColor, int nBackAlpha)
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformSphere(pBGRA, nWidth, nHeight, nStride, Frame, CenterX, CenterY, RadiusX, RadiusY, Degree, nBackColor, nBackAlpha);
				}
				int COM_TransformCylinder(IUnknown* pSource, double Frame, long Center, long Radius, double Degree, long Type, int nBackColor, int nBackAlpha )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;
					int nStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight, &nStride);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую RAW функцию
					return BGRA::BGRA_TransformCylinder(pBGRA, nWidth, nHeight, nStride, Frame, Center, Radius, Degree, Type, nBackColor, nBackAlpha);
				}
				int COM_TransformWave( IUnknown* pSource, double dPhase, double dPhaseOffset, double dAmplitude, BOOL bVertical )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_TransformWave( pBGRA, nWidth, nHeight, dPhase, dPhaseOffset, dAmplitude, bVertical );
				}
				int COM_TransformRipple( IUnknown* pSource, double dOmega, double dOffset, double dFrame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_TransformRipple(pBGRA, nWidth, nHeight, dOmega, dOffset, dFrame);
				}
				int COM_TransformLinear( IUnknown* pSource, IUnknown** pResult, double dFrame, double dAngle, double dScaleX, double dScaleY, double dShiftX, double dShiftY, int nBackColor, int nBackAlpha )
				{
					BYTE* pSrcBGRA = NULL;
					int nSrcWidth = 0;
					int nSrcHeight = 0;
					int nSrcStride = 0;
					long lAspectX = 0;
					long lAspectY = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface( pSource, pSrcBGRA, nSrcWidth, nSrcHeight, &nSrcStride, &lAspectX, &lAspectY);
					if (nStatus != c_nErrorNone)
						return nStatus;

					MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
					::CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_INPROC, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
					if( !pFrame )
						return c_nErrorNoMemory;

					pFrame->put_Width( nSrcWidth );
					pFrame->put_Height( nSrcHeight );
					pFrame->put_ColorSpace( CSP_BGRA );
					pFrame->put_AspectRatioX( lAspectX );
					pFrame->put_AspectRatioY( lAspectY );
					pFrame->SetDefaultStrides();
					pFrame->AllocateBuffer( -1 );

					BYTE* pDstBGRA = NULL;
					int nDstWidth = 0;
					int nDstHeight = 0;
					int nDstStride = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					nStatus = COM_GetBGRAImageFromInterface( pFrame, pDstBGRA, nDstWidth, nDstHeight, &nDstStride );
					if( nStatus != c_nErrorNone )
					{
						pFrame->Release();
						return nStatus;
					}

					*pResult = pFrame;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_TransformLinear( pSrcBGRA, nSrcStride, pDstBGRA, nDstStride, nSrcWidth, nSrcHeight, dFrame, dAngle, dScaleX, dScaleY, dShiftX, dShiftY, nBackColor, nBackAlpha );
				}
				int COM_TransformGlassTile( IUnknown* pSource, int nSize, double dFrame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_TransformGlassTile(pBGRA, nWidth, nHeight, nSize, dFrame);
				}

				int COM_TransformBrokenGlass( IUnknown* pSource, int nDebris, double dFrame )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_TransformBrokenGlass(pBGRA, nWidth, nHeight, nDebris, dFrame);
				}
				
				int COM_FilterPrewitt( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterPrewitt(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterScharr( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterScharr(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterSobel( IUnknown* pSource, double Frame, long Type, BOOL bExtended )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					if( bExtended )
						return Core::BGRA::BGRA_FilterSobelEx(pBGRA, nWidth, nHeight, Frame, Type);
					else
						return Core::BGRA::BGRA_FilterSobel(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterRoberts( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterRoberts(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterLaplace( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterLaplace(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterHipass( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterHipass(pBGRA, nWidth, nHeight, Frame, Type);
				}				
				int COM_FilterLowpass( IUnknown* pSource, double Frame, long Type )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterLowpass(pBGRA, nWidth, nHeight, Frame, Type);
				}

				int COM_FilterBlur( IUnknown* pSource, double Frame, long Size )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterBlur(pBGRA, nWidth, nHeight, Frame, Size);
				}

				int COM_FilterCanny( IUnknown* pSource, double Frame, double LowThresh, double HighThresh )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					// вытаскиваем из интерфейса указатель на память в формате BGRA
					int nStatus = COM_GetBGRAImageFromInterface(pSource, pBGRA, nWidth, nHeight);
					if (nStatus != c_nErrorNone)
						return nStatus;

					// выполняем соответствующую BGRA функцию
					return Core::BGRA::BGRA_FilterCanny(pBGRA, nWidth, nHeight, Frame, LowThresh, HighThresh);
				}

			}
		}
	}
}

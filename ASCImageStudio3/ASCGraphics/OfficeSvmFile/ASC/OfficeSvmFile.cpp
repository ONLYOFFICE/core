// OfficeSvmFile.cpp : Defines the entry point for the console application.

#include "..\OfficeSvmFile.h"
#include "vcl/gdimtf.hxx"
#include "vcl/bitmapex.hxx"
#include "vcl/impbmp.hxx"
#include "vcl/salbmp.hxx"
#include "vcl/salbtype.hxx"
#include "WinStreamReader.h"

#include "OfficeFileErrorDescription.h"

#import "../../../../Redist/ASCMediaCore3.dll"		named_guids rename_namespace("MediaCore"), exclude("tagRECT")


OfficeSvmFile::OfficeSvmFile()
		{
			m_piSvmFile = NULL;
			m_eResizeType = rtAspect;
		}
OfficeSvmFile::~OfficeSvmFile()
		{
			Close();
		}
long OfficeSvmFile::Open( BSTR bsFilename )
		{

			long nError = NOERROR;
			if( false == IsSvmFile( bsFilename, nError ) )
			{
				if( NOERROR == nError )
					return AVS_ERROR_FILEFORMAT;
				else
					return nError;
			}
			std::wstring  sFilename(bsFilename);
			WinStreamReader oStreamReader;
			bool bRes = oStreamReader.Open( sFilename );
			if( false == bRes )
				return AVS_ERROR_FILEACCESS;
			RELEASEOBJECT( m_piSvmFile )
			m_piSvmFile = new SVMCore::GDIMetaFile();
			m_piSvmFile->Read( oStreamReader );
			return NOERROR;
		}
bool OfficeSvmFile::IsSvmFile( BSTR bsFilename, long& nError )
		{
			nError = NOERROR;
			BYTE    pBuffer[ 6 ];
			DWORD    dwBytesRead;
			HANDLE  hFile;

			std::wstring  sFilename(bsFilename);

			hFile = ::CreateFile( sFilename.c_str() , GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE==hFile) 
			{
				nError = AVS_ERROR_FILEACCESS;
				return false;
			}

			::ReadFile( hFile, pBuffer, 6, &dwBytesRead, NULL );
			::CloseHandle(hFile); 

			if( 5 <= dwBytesRead && 'S' == pBuffer[0] && 'V' == pBuffer[1] && 'G' == pBuffer[2] && 'D' == pBuffer[3] && 'I' == pBuffer[4] )
				return true;

			if( 6 <= dwBytesRead && 'V' == pBuffer[0] && 'C' == pBuffer[1] && 'L' == pBuffer[2] && 'M' == pBuffer[3] && 'T' == pBuffer[4] && 'F' == pBuffer[5] )
				return true;

			return false;
		}
void OfficeSvmFile::Close()
		{
			RELEASEOBJECT( m_piSvmFile )
		}
void OfficeSvmFile::SetResizeType( ResizeType eResizeType )
		{
			m_eResizeType = eResizeType;
		}
MediaCore::IAVSUncompressedVideoFrame* OfficeSvmFile::GetThumbnail( int nWidth, int nHeight )
		{
			if (!m_piSvmFile)return NULL;

			SVMCore::BitmapEx oNewBitmap;
			int nResizetype = 1;
			if( rtStretch == m_eResizeType )
				nResizetype = 0;
			m_piSvmFile->CreateThumbnail( nWidth, nHeight, nResizetype, oNewBitmap );
			return BitmapToUnkFrame( oNewBitmap );
		}
void OfficeSvmFile::DrawThumbnail( MediaCore::IAVSUncompressedVideoFrame* piFrame )
		{
			//делаем фон белым
			memset( piFrame->Buffer, 255, piFrame->BufferSize );
			//получаем картинку
			MediaCore::IAVSUncompressedVideoFrame* piResult = GetThumbnail( piFrame->Width, piFrame->Height );
			int nSize = piFrame->BufferSize;
			if( nSize > piResult->BufferSize )
				nSize = piResult->BufferSize;
			//копируем результат
			memcpy( piFrame->Buffer, piResult->Buffer, nSize );
			//удаляем созданную картинку
			RELEASEINTERFACE( piResult );
		}
MediaCore::IAVSUncompressedVideoFrame* OfficeSvmFile::BitmapToUnkFrame( SVMCore::BitmapEx& oBitmapEx )
		 {
			SVMCore::SalBitmap* poSalBitmap = oBitmapEx.ImplGetBitmapImpBitmap()->ImplGetSalBitmap();
			SVMCore::SalBitmap* poSalBitmapMask = oBitmapEx.ImplGetMaskImpBitmap()->ImplGetSalBitmap();
			SVMCore::BitmapBuffer* poSalBitmapBuffer = poSalBitmap->AcquireBuffer( false );
			SVMCore::BitmapBuffer* poSalBitmapMaskBuffer = poSalBitmapMask->AcquireBuffer( false );

			//int nWidthPix = 8 * ( poSalBitmapBuffer->mnScanlineSize + 23 ) / poSalBitmapBuffer->mnBitCount;
			int nWidthPix = oBitmapEx.GetSizePixel().getWidth();

			int nHeightPix = poSalBitmapBuffer->mnHeight;

			MediaCore::IAVSUncompressedVideoFrame* piImage;
			CoCreateInstance( __uuidof( MediaCore::CAVSUncompressedVideoFrame ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(MediaCore::IAVSUncompressedVideoFrame ), (void **)&piImage  );

			piImage->Width = nWidthPix;
			piImage->Height = nHeightPix;
			piImage->put_Stride(0, nWidthPix * 4);

			piImage->AspectRatioX = nWidthPix;
			piImage->AspectRatioY = nHeightPix;

			piImage->DataSize = 4 * nWidthPix * nHeightPix;

			piImage->ColorSpace = 64 | ( 1 << 31 ) ;
			piImage->AllocateBuffer(-1);
			//делаем фон прозрачным
			memset( piImage->Buffer, 0, piImage->BufferSize );

			//предполагаем, что всегда 24 бита (а это так)
			int nUnkSize = 4 * nWidthPix * nHeightPix;
			int nBitmapSize = poSalBitmapBuffer->mnScanlineSize * poSalBitmapBuffer->mnHeight;
			int nBitmapMaskSize = 8 * poSalBitmapMaskBuffer->mnScanlineSize * poSalBitmapMaskBuffer->mnHeight;//определяет прозрачность по битам 0 - прозрачный, 1 - не прозрачный
			int nRow = 0;
			int nCell = 0;
			int nMaskBit = 0;
			int nBitmapByte = 0;
			for( int i = 0, j = 0, k = 0; i < nUnkSize && j < nBitmapSize && k < nBitmapMaskSize; i += 4, j += 3, k++ )
			{
				if( nCell >= nWidthPix )
				{
					nCell = 0;
					nRow++;
				}
				nBitmapByte = nRow * poSalBitmapBuffer->mnScanlineSize + 3 * nCell;
				nMaskBit = nRow * poSalBitmapMaskBuffer->mnScanlineSize + nCell / 8;
				piImage->Buffer[ i ] = poSalBitmapBuffer->mpBits[ nBitmapByte ];
				piImage->Buffer[ i + 1 ] = poSalBitmapBuffer->mpBits[ nBitmapByte + 1 ];
				piImage->Buffer[ i + 2 ] = poSalBitmapBuffer->mpBits[ nBitmapByte + 2 ];
				piImage->Buffer[ i + 3 ] = GetAlphaFromBit( poSalBitmapMaskBuffer->mpBits[ nMaskBit ], nCell % 8 );
				nCell++;

			}
 			poSalBitmap->ReleaseBuffer( poSalBitmapBuffer, false );
 			poSalBitmap->ReleaseBuffer( poSalBitmapMaskBuffer, false );

			return piImage;
		 }
inline byte OfficeSvmFile::GetAlphaFromBit( int nValue, int nBit)
		 {
			 return 0 == (nValue & (1 << (7 - nBit)) ) ? 255 : 0;
		 }
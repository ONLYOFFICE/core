// CSvmFile.cpp : Defines the entry point for the console application.

#include "SvmFile.h"

#include "vcl/gdimtf.hxx"
#include "vcl/bitmapex.hxx"
#include "vcl/impbmp.hxx"
#include "vcl/salbmp.hxx"
#include "vcl/salbtype.hxx"

#include "ASC/WinStreamReader.h"

namespace SvmFile
{
CSvmFile::CSvmFile()
{
	m_piSvmFile = NULL;
	m_eResizeType = rtAspect;
}
CSvmFile::~CSvmFile()
{
	Close();
}
bool CSvmFile::Open( std::wstring sFilename )
{
	if( false == IsSvmFile( sFilename ) )
	{
		return false;
	}

	WinStreamReader oStreamReader;
	bool bRes = oStreamReader.Open( sFilename );
	if( false == bRes )
		return false;
	RELEASEOBJECT( m_piSvmFile )
	m_piSvmFile = new SVMCore::GDIMetaFile();
	m_piSvmFile->Read( oStreamReader );
	return true;
}

bool CSvmFile::IsSvmFile( std::wstring sFilename )
{
	BYTE    pBuffer[ 6 ];
	DWORD    dwBytesRead;
	HANDLE  hFile;
	NSFile::CFileBinary file;

	if (file.OpenFile(sFilename) == false) return false;

	file.ReadFile( pBuffer, 6, dwBytesRead);
	file.CloseFile(); 

	if( 5 <= dwBytesRead && 'S' == pBuffer[0] && 'V' == pBuffer[1] && 'G' == pBuffer[2] && 'D' == pBuffer[3] && 'I' == pBuffer[4] )
		return true;

	if( 6 <= dwBytesRead && 'V' == pBuffer[0] && 'C' == pBuffer[1] && 'L' == pBuffer[2] && 'M' == pBuffer[3] && 'T' == pBuffer[4] && 'F' == pBuffer[5] )
		return true;

	return false;
}

void CSvmFile::Close()
{
	RELEASEOBJECT( m_piSvmFile )
}
void CSvmFile::SetResizeType( ResizeType eResizeType )
{
	m_eResizeType = eResizeType;
}
bool CSvmFile::Open(CBgraFrame* pFrame, const std::wstring& wsSrcPath)
{
	if (!m_piSvmFile)return NULL;

	SVMCore::BitmapEx oNewBitmap;
	int nResizetype = 1;
	if( rtStretch == m_eResizeType )
		nResizetype = 0;

	m_piSvmFile->CreateThumbnail( -1, -1, nResizetype, oNewBitmap );

	return BitmapToBgraFrame(oNewBitmap, pFrame );

}

bool CSvmFile::BitmapToBgraFrame( SVMCore::BitmapEx& oBitmapEx, CBgraFrame*  pBgraFrame)
{
	SVMCore::SalBitmap* poSalBitmap = oBitmapEx.ImplGetBitmapImpBitmap()->ImplGetSalBitmap();
	SVMCore::SalBitmap* poSalBitmapMask = oBitmapEx.ImplGetMaskImpBitmap()->ImplGetSalBitmap();
	
	SVMCore::BitmapBuffer* poSalBitmapBuffer = poSalBitmap->AcquireBuffer( false );
	SVMCore::BitmapBuffer* poSalBitmapMaskBuffer = poSalBitmapMask->AcquireBuffer( false );

	//int nWidthPix = 8 * ( poSalBitmapBuffer->mnScanlineSize + 23 ) / poSalBitmapBuffer->mnBitCount;
	int nWidthPix = oBitmapEx.GetSizePixel().getWidth();

	int nHeightPix = poSalBitmapBuffer->mnHeight;

	pBgraFrame->put_Width(nWidthPix);
	pBgraFrame->put_Height(nHeightPix);
	pBgraFrame->put_Stride(4 * nWidthPix);

	int nBufferSize = 4 * nWidthPix * nHeightPix;
	BYTE* pData = new BYTE[nBufferSize];
	if (!pData)
	{
		return false;
	}

	pBgraFrame->put_Data(pData);

	//делаем фон прозрачным
	memset( pData, 0, nBufferSize );

	//предполагаем, что всегда 24 бита (а это так)
	int nBitmapSize = poSalBitmapBuffer->mnScanlineSize * poSalBitmapBuffer->mnHeight;
	int nBitmapMaskSize = 8 * poSalBitmapMaskBuffer->mnScanlineSize * poSalBitmapMaskBuffer->mnHeight;
								//определяет прозрачность по битам 0 - прозрачный, 1 - не прозрачный
	int nRow = 0;
	int nCell = 0;
	int nMaskBit = 0;
	int nBitmapByte = 0;
	
	for( int i = 0, j = 0, k = 0; i < nBufferSize && j < nBitmapSize && k < nBitmapMaskSize; i += 4, j += 3, k++ )
	{
		if( nCell >= nWidthPix )
		{
			nCell = 0;
			nRow++;
		}
		nBitmapByte = nRow * poSalBitmapBuffer->mnScanlineSize + 3 * nCell;
		nMaskBit = nRow * poSalBitmapMaskBuffer->mnScanlineSize + nCell / 8;
		pData[ i ] = poSalBitmapBuffer->mpBits[ nBitmapByte ];
		pData[ i + 1 ] = poSalBitmapBuffer->mpBits[ nBitmapByte + 1 ];
		pData[ i + 2 ] = poSalBitmapBuffer->mpBits[ nBitmapByte + 2 ];
		pData[ i + 3 ] = GetAlphaFromBit( poSalBitmapMaskBuffer->mpBits[ nMaskBit ], nCell % 8 );
		nCell++;

	}
	poSalBitmap->ReleaseBuffer( poSalBitmapBuffer, false );
	poSalBitmap->ReleaseBuffer( poSalBitmapMaskBuffer, false );

	return true;
}
inline unsigned char CSvmFile::GetAlphaFromBit( int nValue, int nBit)
 {
	 return 0 == (nValue & (1 << (7 - nBit)) ) ? 255 : 0;
 }
}
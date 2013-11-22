#pragma once

#import "./../../../Redist/ASCMediaCore3.dll"		named_guids rename_namespace("MediaCore"), exclude("tagRECT")


namespace SVMCore
{
	class GDIMetaFile;
	class BitmapEx;
}

class OfficeSvmFile
{
public:
	typedef enum{ rtStretch = 0, rtAspect = 1 } ResizeType;
	
	OfficeSvmFile();
	~OfficeSvmFile();
	long Open( BSTR sFilename );
	bool IsSvmFile( BSTR sFilename, long& nError );
	void Close();
	void SetResizeType( ResizeType eResizeType );
	MediaCore::IAVSUncompressedVideoFrame* GetThumbnail( int nWidth = -1, int nHeight = -1 );
	void DrawThumbnail( MediaCore::IAVSUncompressedVideoFrame* piFrame );
	MediaCore::IAVSUncompressedVideoFrame* BitmapToUnkFrame( SVMCore::BitmapEx& oBitmapEx );
private:
	SVMCore::GDIMetaFile *m_piSvmFile;
	ResizeType m_eResizeType;
	inline byte GetAlphaFromBit( int nValue, int nBit);
};
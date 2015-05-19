#pragma once

#include "../../BgraFrame.h"

namespace SVMCore
{
	class GDIMetaFile;
	class BitmapEx;
}

namespace SvmFile
{
class CSvmFile
{
public:
	typedef enum{ rtStretch = 0, rtAspect = 1 } ResizeType;
	
	CSvmFile();
	~CSvmFile();
	
	bool IsSvmFile( std::wstring sFilename );

	bool Open(CBgraFrame* pFrame, const std::wstring& wsSrcPath);
private:
	void SetResizeType( ResizeType eResizeType );
	bool Open( std::wstring sFilename );
	void Close();
	
	SVMCore::GDIMetaFile *m_piSvmFile;

	ResizeType m_eResizeType;
	inline unsigned char GetAlphaFromBit( int nValue, int nBit);

	bool BitmapToBgraFrame( SVMCore::BitmapEx& oBitmapEx, CBgraFrame*  pBgraFrame);
};
}
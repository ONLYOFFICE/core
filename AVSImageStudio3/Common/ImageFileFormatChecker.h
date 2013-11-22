#pragma once

enum _ImageFileFormats
{
	c_Unknown,
	c_Gdiplus,
	c_Bmp,
	c_Jpg,
	c_Gif,
	c_Tiff,
	c_Png,
	c_Ico,
	c_Metafile,
	c_Psd,
	c_Tga,
	c_Pcx,
	c_Ras,
	c_Jpeg2000,
	c_WebShots,
	c_iPode,
	c_RawFotos,
	c_Sfw,
	c_Swf,
	c_Wbz,
	c_Wbc,
	c_Svm
};

struct _ImageFileExtension
{
	const TCHAR*		str;
	_ImageFileFormats	format;
};					

static _ImageFileExtension tableSupportImageFormats[] = 
{
	{_T("bmp"),  c_Bmp},
	{_T("gif"),  c_Gif},
	{_T("tiff"), c_Tiff},
	{_T("tif"),  c_Tiff},
	{_T("png"),  c_Png},
	{_T("ico"),  c_Ico},
	{_T("mng"),  c_Gdiplus},
	{_T("jpc"),  c_Gdiplus},
	{_T("pgx"),  c_Gdiplus},
	{_T("jp2"),  c_Jpeg2000},
	{_T("j2k"),  c_Jpeg2000},
	{_T("emf"),  c_Metafile},
	{_T("wmf"),  c_Metafile},
	{_T("jpg"),  c_Jpg},
	{_T("jpeg"), c_Jpg},
	{_T("psd"),  c_Psd},
	{_T("pdd"),  c_Psd},
	{_T("psb"),  c_Psd},
	{_T("tga"),  c_Tga},
	{_T("pcx"),  c_Pcx},
	{_T("ras"),  c_Ras},
	{_T("sun"),  c_Ras},
	{_T("wb1"),  c_WebShots},
	{_T("wb2"),  c_WebShots},
	{_T("ithmb"), c_iPode},
	{_T("svg"),  c_Metafile},
	{_T("dng"),  c_RawFotos},
	{_T("crw"),  c_RawFotos},
	{_T("cr2"),  c_RawFotos},
	{_T("arw"),  c_RawFotos},
	{_T("sr2"),  c_RawFotos},
	{_T("nef"),  c_RawFotos},
	{_T("orf"),  c_RawFotos},
	{_T("raf"),  c_RawFotos},
	{_T("pef"),  c_RawFotos},
	{_T("raw"),  c_RawFotos},
	{_T("rw2"),  c_RawFotos},
	{_T("dcr"),  c_RawFotos},
	{_T("kdc"),  c_RawFotos},
	{_T("bay"),  c_RawFotos},
	{_T("3fr"),  c_RawFotos},
	{_T("rwl"),  c_RawFotos},
	{_T("mrw"),  c_RawFotos},
	{_T("dng"),  c_RawFotos},
	{_T("x3f"),  c_RawFotos},
	{_T("erf"),  c_RawFotos},
	{_T("mef"),  c_RawFotos},
	{_T("srw"),  c_RawFotos},
	{_T("sfw"),  c_Sfw},
	{_T("swf"),  c_Swf},
	{_T("wbz"),  c_Wbz},
	{_T("wbc"),  c_Wbc},
	{_T("svm"),  c_Svm},
	{_T("mj2"),  c_Jpeg2000}
};


class CImageFileFormatChecker
{
public:
	_ImageFileFormats eFileType;
	
	CImageFileFormatChecker()
	{
		eFileType = c_Unknown;
	}
	CImageFileFormatChecker(CStringW sFileName)
	{
		eFileType = c_Unknown;

		isImageFile(sFileName);
	}
	
	bool isImageFile(CStringW & fileName);	
	bool isPngFile(CStringW & fileName);//need for AVSImageCoverter

private:

	bool isRawFile(CStringW & fileName);
	bool isSvgFile(CStringW & fileName);

	bool isBmpFile(BYTE* pBuffer,DWORD dwBytes);
	bool isGifFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPngFile(BYTE* pBuffer,DWORD dwBytes);
	bool isEmfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWmfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isTgaFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPcxFile(BYTE* pBuffer,DWORD dwBytes);
	bool isTiffFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJpgFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbFile(BYTE* pBuffer,DWORD dwBytes);

	bool isIcoFile(BYTE* pBuffer,DWORD dwBytes);
	bool isRasFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPsdFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSvmFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSwfFile(BYTE* pBuffer,DWORD dwBytes);
	bool isSfwFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbcFile(BYTE* pBuffer,DWORD dwBytes);
	bool isWbzFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJ2kFile(BYTE* pBuffer,DWORD dwBytes);
	bool isJp2File(BYTE* pBuffer,DWORD dwBytes);
	bool isMj2File(BYTE* pBuffer,DWORD dwBytes);
	bool isIpodFile(BYTE* pBuffer,DWORD dwBytes);
	bool isPgxFile(BYTE* pBuffer,DWORD dwBytes);
	
};

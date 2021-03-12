#include "ximage.h"

#if CXIMAGE_SUPPORT_JPG
#include "ximajpg.h"
#endif

#if CXIMAGE_SUPPORT_GIF
#include "ximagif.h"
#endif

#if CXIMAGE_SUPPORT_PNG
#include "ximapng.h"
#endif

#if CXIMAGE_SUPPORT_MNG
#include "ximamng.h"
#endif

#if CXIMAGE_SUPPORT_BMP
#include "ximabmp.h"
#endif

#if CXIMAGE_SUPPORT_ICO
#include "ximaico.h"
#endif

#if CXIMAGE_SUPPORT_TIF
#include "ximatif.h"
#endif

#if CXIMAGE_SUPPORT_TGA
#include "ximatga.h"
#endif

#if CXIMAGE_SUPPORT_PCX
#include "ximapcx.h"
#endif

#if CXIMAGE_SUPPORT_WBMP
#include "ximawbmp.h"
#endif

#if CXIMAGE_SUPPORT_WMF
#include "ximawmf.h" // <vho> - WMF/EMF support
#endif

#if CXIMAGE_SUPPORT_JBG
#include "ximajbg.h"
#endif

#if CXIMAGE_SUPPORT_JASPER
#include "ximajas.h"
#endif

#if CXIMAGE_SUPPORT_SKA
#include "ximaska.h"
#endif

#if CXIMAGE_SUPPORT_RAW
#include "ximaraw.h"
#endif

#if CXIMAGE_SUPPORT_PSD
#include "ximapsd.h"
#endif

#if CXIMAGE_SUPPORT_DECODE
bool CxImage::Decode(uint8_t * buffer, uint32_t size, uint32_t imagetype)
{
	CxMemFile file(buffer,size);
	return Decode(&file,imagetype);
}
bool CxImage::Decode(CxFile *hFile, uint32_t imagetype)
{
	if (hFile == NULL){
		strcpy(info.szLastError,CXIMAGE_ERR_NOFILE);
		return false;
	}

	uint32_t pos = hFile->Tell();

#if CXIMAGE_SUPPORT_BMP
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_BMP==imagetype){
		CxImageBMP *newima = new CxImageBMP;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JPG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_JPG==imagetype){
		CxImageJPG *newima = new CxImageJPG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_ICO
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_ICO==imagetype){
		CxImageICO *newima = new CxImageICO;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_GIF
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_GIF==imagetype){
		CxImageGIF *newima = new CxImageGIF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PNG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PNG==imagetype){
		CxImagePNG *newima = new CxImagePNG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_TIF
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_TIF==imagetype){
		CxImageTIF *newima = new CxImageTIF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_MNG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_MNG==imagetype){
		CxImageMNG *newima = new CxImageMNG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			info.nNumFrames = newima->info.nNumFrames;
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_TGA
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_TGA==imagetype){
		CxImageTGA *newima = new CxImageTGA;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PCX
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PCX==imagetype){
		CxImagePCX *newima = new CxImagePCX;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WBMP
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_WBMP==imagetype){
		CxImageWBMP *newima = new CxImageWBMP;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_WMF && CXIMAGE_SUPPORT_WINDOWS
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_WMF==imagetype){
		CxImageWMF *newima = new CxImageWMF;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JBG
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_JBG==imagetype){
		CxImageJBG *newima = new CxImageJBG;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_JASPER
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype ||
#if	CXIMAGE_SUPPORT_JP2
	 CXIMAGE_FORMAT_JP2==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_JPC
	 CXIMAGE_FORMAT_JPC==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PGX
	 CXIMAGE_FORMAT_PGX==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_PNM
	 CXIMAGE_FORMAT_PNM==imagetype || 
#endif
#if	CXIMAGE_SUPPORT_RAS
	 CXIMAGE_FORMAT_RAS==imagetype || 
#endif
	 false ){
		CxImageJAS *newima = new CxImageJAS;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_SKA
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_SKA==imagetype){
		CxImageSKA *newima = new CxImageSKA;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_RAW
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_RAW==imagetype){
		CxImageRAW *newima = new CxImageRAW;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PSD
	if (CXIMAGE_FORMAT_UNKNOWN==imagetype || CXIMAGE_FORMAT_PSD==imagetype){
		CxImagePSD *newima = new CxImagePSD;
		if (!newima)
			return false;
		newima->CopyInfo(*this);
		if (newima->Decode(hFile)) {
			Transfer(*newima);
			delete newima;
			return true;
		} else {
			strcpy(info.szLastError,newima->GetLastError());
			hFile->Seek(pos,SEEK_SET);
			delete newima;
			if (CXIMAGE_FORMAT_UNKNOWN!=imagetype)
				return false;
		}
	}
#endif

	strcpy(info.szLastError,"Decode: Unknown or wrong format");
	return false;
}
#endif // CXIMAGE_SUPPORT_DECODE

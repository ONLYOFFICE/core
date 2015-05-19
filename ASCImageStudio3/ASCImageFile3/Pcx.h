#pragma once

#include <ImageStudioUtils.h>

typedef struct tag_PCX_HEADER
{
	BYTE Manufacturer;	// = 10
	BYTE Version;
	BYTE Encoding;		// if 1 - then encoding is used
	BYTE BitsPerPixel;
	WORD XMin,YMin,XMax,YMax;
	WORD HRes,VRes;
	BYTE ColorMap[48];
	BYTE Reserved;
	BYTE NPlanes;		// number of color planes
	WORD BytesPerLine;
	WORD PaletteInfo;
	BYTE Filler[58];
} PCX_HEADER;

class ImgFilesPCX
{
public:

	ImgFilesPCX()
	{
		handle = INVALID_HANDLE_VALUE;
		memset(&header,0,sizeof(header));
		bitsPerPixel = 0;
		errCode = 0;
		imgSize = 0;

		sabound[0].cElements = 4;		// planes
		sabound[0].lLbound   = 0;
		sabound[1].cElements = 1;		// width
		sabound[1].lLbound   = 0;
		sabound[2].cElements = 1;		// height
		sabound[2].lLbound   = 0;

		psarr = 0;
	}
	~ImgFilesPCX()
	{
		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);

		if (psarr) SafeArrayDestroy(psarr);
	}
	
	BOOL IsPcx(BSTR strPath, BOOL closeFlag)
	{
		USES_CONVERSION;

		BOOL result = TRUE;

		handle = CreateFile(OLE2T(strPath),
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
		if (handle == INVALID_HANDLE_VALUE)
		{ errCode=1; return FALSE; }

		DWORD bRead;
		if ((!ReadFile(handle,&header,sizeof(header),&bRead,NULL)) ||
			(bRead != sizeof(header)))
		{ errCode=2; result=FALSE; }

		// яЁютхЁър ЇюЁьрЄр
		if (header.Manufacturer != 10)
		{ errCode=3; result=FALSE; }

		if ((header.Version != 0) && (header.Version != 2) &&
			(header.Version != 3) && (header.Version != 5))
		{ errCode=4; result=FALSE; }

		if ((header.BitsPerPixel != 1) && (header.BitsPerPixel != 8))
		{ errCode=5; result=FALSE; }

		if ((header.Encoding != 0) && (header.Encoding != 1))
		{ errCode=6; result=FALSE; }

		if ((header.PaletteInfo != 0) && (header.PaletteInfo != 1))
		{ errCode=7; result=FALSE; }

		bitsPerPixel = header.BitsPerPixel*header.NPlanes;
		if ((bitsPerPixel != 1) && (bitsPerPixel != 4) &&
			(bitsPerPixel != 8) && (bitsPerPixel != 16) &&
			(bitsPerPixel != 24) && (bitsPerPixel != 32))
		{ errCode=8; result=FALSE; }

		if ((closeFlag) || (!result))
		{
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}

		return result;
	}

	BOOL LoadPcx(BSTR strPath)
	{
		if (!IsPcx(strPath, FALSE)) return FALSE;

		// получение величины файла
		DWORD size, size2;
		size = GetFileSize(handle,&size2);
		if (size2 == 0xFFFFFFFF)
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=9; return FALSE;
		}

		// считывание всего файла в память
		BYTE * data = (BYTE*) HeapAlloc(GetProcessHeap(),0,size);
		if (!data)
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=10; return FALSE;
		}
		size -= 128;

		DWORD bRead;
		if ((!ReadFile(handle,data,size,&bRead,NULL)) || (bRead != size))
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			errCode=11; return FALSE;
		}

		// получение параметров картинки	
		width = header.XMax - header.XMin + 1;
		height = header.YMax - header.YMin + 1;
		
		width2 = width;
		if ((bitsPerPixel>=8) && (width % 2)) width2 = ((width >> 1) + 1) << 1;
		// файлы сделанные Adobe Photoshop не придерживаются стандарта и не делают строки кратными 2
		if (header.BytesPerLine % 2)
		{
			if ((bitsPerPixel<8) && (width % 8)) width2 = ((width >> 3) + 1) << 3;
		}
		// файлы придерживающиеся стандарта
		else
		{
			if ((bitsPerPixel<8) && (width % 16)) width2 = ((width >> 4) + 1) << 4;
		}

		// создание массива для данных
		sabound[1].cElements = width;
		sabound[2].cElements = height;

		psarr = SafeArrayCreate(VT_UI1,3,sabound);
		if (psarr == NULL) { errCode=12; return FALSE; }
		if (SafeArrayLock(psarr) != S_OK)
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			SafeArrayDestroy(psarr);
			HeapFree(GetProcessHeap(),0,data);
			errCode=13; return FALSE;
		}
		memset(psarr->pvData,0,width*height*4);

		// определение величины несжатой картинки
		imgSize = (DWORD)width2 * height;
		if (bitsPerPixel == 1) imgSize >>= 3;
		else if (bitsPerPixel == 4) imgSize >>= 1;
		else if (bitsPerPixel == 24) imgSize *= 3;

		if (bitsPerPixel == 8)
		{
			if (data[size-769] != 12)
			{
				CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
				SafeArrayUnlock(psarr); SafeArrayDestroy(psarr);
				HeapFree(GetProcessHeap(),0,data);
				errCode=14; return FALSE;
			}
		}

		// считывание данных
		DWORD k,k1=0,k2=0,x=0,t;
		int hh=height-1,hh2=0,pp=header.NPlanes-1,ww=0;
		BYTE ch,ch1,bit;
		BYTE bits[8] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
		while (k2 < imgSize)
		{
			k=1;

			if (header.Encoding == 1)
			{
				if (k1 >= size) break;
				ch=data[k1]; k1++;
				if (0xC0 == (0xC0 & ch))
				{
					k = 0x3F & ch;
					if (k1 >= size) break;
					ch=data[k1]; k1++;
				}
			}
			else
			{
				if (k1 >= size) break;
				ch=data[k1]; k1++;
			}

			k2+=k;

			if (bitsPerPixel == 24)
			{
				while (k)
				{
					if (ww < width)
					{
						x = hh*width*4+ww*4+pp;
						((BYTE*)psarr->pvData)[x] = ch;
					}

					if (ww==width2-1)
					{
						ww=0;
						if (pp==0)
						{
							pp=header.NPlanes-1;
							if (hh==0) hh=height-1;
							else hh--;
						}
						else pp--;
					}
					else
						ww++;
					
					k--;
				}
			}

			if (bitsPerPixel == 8)
			{
				while (k)
				{
					if (ww < width)
					{
						x = (hh*width+ww)<<2;
						t = size-768+ch*3;
						((BYTE*)psarr->pvData)[x]   = data[t+2];
						((BYTE*)psarr->pvData)[x+1] = data[t+1];
						((BYTE*)psarr->pvData)[x+2] = data[t];
					}
					k--;
					ww++;
					if (ww == width2) { ww=0; hh--; }
				}
			}

			if (bitsPerPixel == 4)
			{
				while (k)
				{
					for (bit=0; bit<8; bit++)
					{
						if ((hh2 < height) && (ww < width))
						{
							ch1 = (ch & bits[bit]) ? 1 : 0;
							if ((ch1) || (pp==0))
							{
								x = (hh*width+ww)<<2;
								if (pp==3) ((BYTE*)psarr->pvData)[x] = ch1;
								else if (pp==2) ((BYTE*)psarr->pvData)[x] |= ch1<<1;
								else if (pp==1) ((BYTE*)psarr->pvData)[x] |= ch1<<2;
								else if (pp==0)
								{
									ch1 = (ch1<<3) | ((BYTE*)psarr->pvData)[x];
									((BYTE*)psarr->pvData)[x] = header.ColorMap[3*ch1+2];
									((BYTE*)psarr->pvData)[x+1] = header.ColorMap[3*ch1+1];
									((BYTE*)psarr->pvData)[x+2] = header.ColorMap[3*ch1];
								}
							}
						}
						ww++;
						if (ww == width2)
						{
							ww=0;
							if (pp==0)
							{
								pp=3; //header.NPlanes-1;
								hh--; hh2++;
							}
							else pp--;
						}
					}
					k--;
				}
			}

			if (bitsPerPixel == 1)
			{
				while (k)
				{
					for (bit=0; bit<8; bit++)
					{
						if ((hh2 < height) && (ww < width))
						{
							if (ch & bits[bit])
							{
								x = (hh*width+ww)<<2;
								((BYTE*)psarr->pvData)[x] = 0xFF;
								((BYTE*)psarr->pvData)[x+1] = 0xFF;
								((BYTE*)psarr->pvData)[x+2] = 0xFF;
							}
						}
						ww++;
						if (ww == width2) { ww=0; hh--; hh2++; }
					}
					k--;
				}
			}

		}

		SafeArrayUnlock(psarr);
		HeapFree(GetProcessHeap(),0,data);
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
		memset(&header,0,sizeof(header));
		bitsPerPixel = 0;
		errCode = 0;

		return TRUE;
	}

	BOOL Save(BSTR strPath, int width, int height, int channels, BYTE * data, BOOL encodingFlag)
	{
		USES_CONVERSION;

		PCX_HEADER header; memset(&header,0,sizeof(header));
		
		header.XMax = width-1;
		header.YMax = height-1;
		int width2 = (width % 2) ? width+1 : width;

		header.Manufacturer = 10;
		header.Version = 5;
		header.BitsPerPixel = 8;
		header.NPlanes = 3;
		header.Encoding = encodingFlag ? 1 : 0;
		header.BytesPerLine = width2;

		DWORD imgSize = width2 * height * 3;
		BYTE * data2 = (BYTE*) HeapAlloc(GetProcessHeap(),0,imgSize);
		if (!data2)
		{
			errCode=15; return FALSE;
		}
		memset(data2,0,imgSize);

		DWORD ii=0;
		int ww=0,hh=0,pp=0;
		for (hh=height-1; hh>=0; hh--)
		{
			for (pp=2; pp>=0; pp--)
			{
				for (ww=0; ww<width; ww++,ii++)
				{
					data2[ii] = data[hh*width*4+ww*4+pp];
				}
				ii += width2-width;
			}
		}


		// запись в файл
		HANDLE hOut = CreateFile(OLE2T(strPath),
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			HeapFree(GetProcessHeap(),0,data2);
			errCode=16; return FALSE;
		}
			
		DWORD bWriten;
		if ((!WriteFile(hOut,&header,sizeof(PCX_HEADER),&bWriten,NULL)) ||
			(bWriten != sizeof(PCX_HEADER)))
		{
			HeapFree(GetProcessHeap(),0,data2);
			CloseHandle(hOut); errCode=17; return FALSE;
		}

		if (!encodingFlag)
		{
			if ((!WriteFile(hOut,data2,imgSize,&bWriten,NULL)) ||
				(bWriten != imgSize))
			{
				HeapFree(GetProcessHeap(),0,data2);
				CloseHandle(hOut); errCode=18; return FALSE;
			}
		}
		else
		{
			for (hh=0; hh<height; hh++)
			{
				for (pp=0; pp<3; pp++)
				{
					int x = encLine(data2+(hh*width2*3+width2*pp),width,hOut);
					if (x == 0)
					{
						HeapFree(GetProcessHeap(),0,data2);
						CloseHandle(hOut); errCode=19; return FALSE;
					}
					// добавить байты для выравнивания.
					if (width != width2)
					{
						BYTE ch=0;
						if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
							(bWriten != 1))
						{
							HeapFree(GetProcessHeap(),0,data2);
							CloseHandle(hOut); errCode=19; return FALSE;
						}
					}
				}
			}
		}

		HeapFree(GetProcessHeap(),0,data2);
		CloseHandle(hOut);
		
		return TRUE;
	}
	
	// В функции отдается сам массив, а не его копия.
	// В этом случае будет в памяти будет всего один массив.
	/* int GetImgFromBuffer(SAFEARRAY *& pArray)
	{
		if (SafeArrayCopy(psarr, &pArray) != S_OK)
			return 99;

		return 0;
	}*/
	int GetImgFromBuffer(void** pData, BOOL (*CopyTo)(SAFEARRAY*, void**))
	{
		// reset the alpha channel
		ImageStudioUtils::FillAlphaChannel(&psarr, 255);

		if (CopyTo(psarr, pData))
			return 99;

		return 0;
	}
	
protected:

	static int encLine(BYTE * inBuff, int inLen, HANDLE _handle)
	{  
		unsigned char ch, last;
		int srcIndex, i;
		register int total;
		register unsigned char runCount; /* макс. значение равно 63 */

		total = 0;
		last = *(inBuff);  runCount = 1;

		for (srcIndex = 1; srcIndex < inLen; srcIndex++) {
			ch = *(++inBuff);
			if (ch == last) {
				runCount++; /* он кодируется */
				if (runCount == 63) {
					if (!(i=encput(last, runCount, _handle))) return(0);
					total += i;
					runCount = 0;
				}
			}
			else {   /* this != last */
				if (runCount) {
					if (!(i=encput(last, runCount, _handle))) return(0);
					total += i;
				}
				last = ch;
				runCount = 1;
			}
		} /* конец цикла */

		if (runCount) {  /* завершение */
			if (!(i=encput(last, runCount, _handle))) return(0);
			return(total + i);
		}

	return(total);

	}

	static int encput(BYTE byt, BYTE cnt, HANDLE _handle)
	{
	if(cnt)
	{
		if( (cnt==1) && (0xc0 != (0xc0&byt)) )
		{
				DWORD bWriten;
				if ((!WriteFile(_handle,&byt,1,&bWriten,NULL)) ||
					(bWriten != 1))
					return 0;
			return 1;
		}
		else
		{
				cnt = 0xC0 | cnt;
				DWORD bWriten;
				if ((!WriteFile(_handle,&cnt,1,&bWriten,NULL)) ||
					(bWriten != 1))
					return 0;
				if ((!WriteFile(_handle,&byt,1,&bWriten,NULL)) ||
					(bWriten != 1))
					return 0;
				return 2;
		}
	}

	return 0;
	}


	
private:
	
	HANDLE handle;
	PCX_HEADER header;
	
	BYTE bitsPerPixel;
	int width,height,width2;
	DWORD imgSize;

	SAFEARRAYBOUND sabound[3];
	SAFEARRAY * psarr;

	int errCode;
};

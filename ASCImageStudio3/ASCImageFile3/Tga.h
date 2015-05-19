#pragma once

#pragma pack(push,1)

typedef struct TargaColorMapSpec
{
    unsigned short first_entry_index;
    unsigned short color_map_length;
    unsigned char color_map_entry_size;
} TargaColorMapSpec;

typedef struct TargaImageSpec
{
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char pixel_depth;
    unsigned char image_descriptor;
} TargaImageSpec;

typedef struct TargaHeaderStruct
{
    unsigned char id_length;
    unsigned char color_map_type;
    unsigned char image_type;
    TargaColorMapSpec color_map_spec;
    TargaImageSpec image_spec;
} TargaHeaderStruct;

#pragma pack(pop)

// Constants (for TargaHeaderStruct.image_type)
const unsigned char TIT_NOIMAGE_DATA                    = 0;
const unsigned char TIT_UNCOMPRESSED_COLOR_MAPPED_IMAGE = 1;  /* tested */
const unsigned char TIT_UNCOMPRESSED_TRUE_COLOR_IMAGE   = 2;  /* tested */
const unsigned char TIT_UNCOMPRESSED_GRAYSCALE_IMAGE    = 3;  /* tested */
const unsigned char TIT_RLE_COLOR_MAPPED_IMAGE          = 9;  /* tested */
const unsigned char TIT_RLE_TRUE_COLOR_IMAGE            = 10; /* tested */
const unsigned char TIT_RLE_GRAYSCALE_IMAGE             = 11;

class ImgFilesTGA
{
public:
	
	ImgFilesTGA()
	{
		handle = INVALID_HANDLE_VALUE;

		sabound[0].cElements = 4;		// planes
		sabound[0].lLbound   = 0;
		sabound[1].cElements = 1;		// width
		sabound[1].lLbound   = 0;
		sabound[2].cElements = 1;		// height
		sabound[2].lLbound   = 0;

		psarr = 0;
	}
	~ImgFilesTGA()
	{
		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);

		if (psarr)
			SafeArrayDestroy(psarr);
	}
	
	BOOL IsTga(BSTR bstrPath, BOOL closeFlag)
	{
		USES_CONVERSION;

		handle = CreateFile(OLE2T(bstrPath),
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
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=2; return FALSE;
		}

		//  ============== ¦ЁютхЁър ЇюЁьрЄр =======================
		if (header.image_type == TIT_NOIMAGE_DATA )
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=3; return FALSE;
		}

		if ( header.image_spec.pixel_depth != 8 &&
			header.image_spec.pixel_depth != 15 &&
			header.image_spec.pixel_depth != 16 &&
			header.image_spec.pixel_depth != 24 &&
			header.image_spec.pixel_depth != 32 )
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=4; return FALSE;
		}

		if ( header.image_type != TIT_UNCOMPRESSED_COLOR_MAPPED_IMAGE &&
			header.image_type != TIT_UNCOMPRESSED_TRUE_COLOR_IMAGE &&
			header.image_type != TIT_UNCOMPRESSED_GRAYSCALE_IMAGE &&
			header.image_type != TIT_RLE_COLOR_MAPPED_IMAGE &&
			header.image_type != TIT_RLE_TRUE_COLOR_IMAGE &&
			header.image_type != TIT_RLE_GRAYSCALE_IMAGE )
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=5; return FALSE;
		}

		if ( header.color_map_spec.color_map_length > 256  )
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=6; return FALSE;
		}

		if (closeFlag) { CloseHandle(handle); handle=INVALID_HANDLE_VALUE; }

		return TRUE;
	}
	BOOL LoadTga(BSTR bstrPath)
	{
		unsigned short bytes_per_pixel;
		int alpha_present;
		BOOL swapCols=FALSE, swapRows=FALSE;

		if (!IsTga(bstrPath,FALSE)) return FALSE;

		alpha_present = header.image_spec.pixel_depth == 32 &&
						(header.image_spec.image_descriptor & 0x08) > 0;

		width = header.image_spec.width;
		height = header.image_spec.height;
		bits_per_pixel = header.image_spec.pixel_depth;
		if ( bits_per_pixel == 15 ) bits_per_pixel = 16;
		bytes_per_pixel = bits_per_pixel / 8;
		BYTE bytes_per_pal = header.color_map_spec.color_map_entry_size / 8;

		if ((header.image_type == TIT_UNCOMPRESSED_GRAYSCALE_IMAGE) ||
			(header.image_type == TIT_RLE_GRAYSCALE_IMAGE))
		{
			header.color_map_spec.color_map_length = 256;
			header.color_map_spec.color_map_entry_size = 24;
		}

		int palette_size;
		if ( header.color_map_type == 1 )
			palette_size = header.color_map_spec.color_map_length;


		// получение величины файла
		DWORD size, size2;
		size = GetFileSize(handle,&size2);
		if (size2 == 0xFFFFFFFF)
		{ errCode=9; return FALSE; }

		// считывание всего файла в память
		BYTE * data = (BYTE*) HeapAlloc(GetProcessHeap(),0,size);
		if (!data)
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			errCode=10; return FALSE;
		}
		size -= 18;

		int dataStart = header.id_length +
						header.color_map_spec.color_map_length *
						bytes_per_pal;
		
		BYTE * pal = data + header.id_length;
		if ((bits_per_pixel == 8) && (size < 768))
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			errCode=11; return FALSE;
		}

		DWORD bRead;
		if ((!ReadFile(handle,data,size,&bRead,NULL)) || (bRead != size))
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			errCode=11; return FALSE;
		}


		// создание массива для данных
		sabound[1].cElements = width;
		sabound[2].cElements = height;

		psarr = SafeArrayCreate(VT_UI1,3,sabound);
		if (psarr == NULL) { errCode=99; return FALSE; }
		if (SafeArrayLock(psarr) != S_OK)
		{
			CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
			SafeArrayDestroy(psarr);
			HeapFree(GetProcessHeap(),0,data);
			errCode=99; return FALSE;
		}
		memset(psarr->pvData,0,width*height*4);


		swapCols = header.image_spec.image_descriptor & 0x10;
		swapRows = header.image_spec.image_descriptor & 0x20;


		DWORD i=0,i1=dataStart,j1=0,j2=0;
		WORD x;
		BOOL changeRow=FALSE;
		BYTE bytesToRead = (BYTE)bytes_per_pixel;
		if ((header.image_spec.pixel_depth == 32) &&
			(alpha_present == 0))
			bytesToRead=3;
		BYTE type=0,count=0;

		int ww=0,hh=0;
		if (swapCols) ww=width-1;
		if (swapRows) hh=height-1;

		while (i < ((DWORD)width*height))
		{
			if (i1 >= size) break;
			count=1;
			if ((header.image_type == TIT_RLE_TRUE_COLOR_IMAGE) ||
				(header.image_type == TIT_RLE_COLOR_MAPPED_IMAGE) ||
				(header.image_type == TIT_RLE_GRAYSCALE_IMAGE))
			{
				if (i1 >= size) break;
				count=data[i1]; i1++;
				if (count & 0x80)
				{ count &= 0x7F; count++; type=1; }
				else
				{ count++; type=2; }
			}

			j1 = 4 * (hh*width+ww);
			j2 = i1;

			while (count)
			{
				if (bits_per_pixel == 8)
				{
					if (j2 >= size) break;
					if ((header.image_type == TIT_UNCOMPRESSED_COLOR_MAPPED_IMAGE) ||
						(header.image_type == TIT_RLE_COLOR_MAPPED_IMAGE))
					{
						if (bytes_per_pal == 2)
						{
							x = ((WORD*)(pal+data[j2]*2))[0];
							((BYTE*)psarr->pvData)[j1] = (x & 0x7C00) >> 7;
							((BYTE*)psarr->pvData)[j1+1] = (x & 0x03E0) >> 2;
							((BYTE*)psarr->pvData)[j1+2] = (x & 0x001F) << 3;
						}
						else if (bytes_per_pal == 3)
						{
							memcpy(((BYTE*)psarr->pvData)+j1,pal+data[j2]*3,3);
						}
						else if (bytes_per_pal == 4)
						{
							memcpy(((BYTE*)psarr->pvData)+j1,pal+data[j2]*4,3);
						}
					}

					else if ((header.image_type == TIT_UNCOMPRESSED_GRAYSCALE_IMAGE) ||
							(header.image_type == TIT_RLE_GRAYSCALE_IMAGE))
					{
						memset(((BYTE*)psarr->pvData)+j1,data[j2],3);
					}
				}				
					
				if (bits_per_pixel == 16)
				{
					if ((j2+2) >= size) break;
					x = ((WORD*)(data+j2))[0];
					((BYTE*)psarr->pvData)[j1+2] = (x & 0x7C00) >> 7;
					((BYTE*)psarr->pvData)[j1+1] = (x & 0x03E0) >> 2;
					((BYTE*)psarr->pvData)[j1] = (x & 0x001F) << 3;
				}

				if ((bits_per_pixel == 24) || (bits_per_pixel == 32))
				{
					if ((j2+bytesToRead) >= size) break;
					memcpy(((BYTE*)psarr->pvData)+j1,data+j2,bytesToRead);
				}
						
				i++;

				if (swapCols)
				{
					if (ww==0) { ww=width-1; changeRow=TRUE; }
					else ww--;
				}
				else
				{
					if (ww==(width-1)) { ww=0; changeRow=TRUE; }
					else ww++;
				}
				if (changeRow)
				{
					if (swapRows) hh--;
					else hh++;
					changeRow=FALSE;
				}

				j1 = 4 * (hh*width+ww);
				if (type==2) { i1+=bytes_per_pixel; j2=i1; }
				count--;
			}
			if (type!=2) i1+=bytes_per_pixel;
		}


		CloseHandle(handle); handle=INVALID_HANDLE_VALUE;
		HeapFree(GetProcessHeap(),0,data);
		SafeArrayUnlock(psarr);
		return TRUE;
	}

	BOOL Save(BSTR bstrPath, int width, int height, int channels, BYTE * data, BOOL encodingFlag)
	{
		USES_CONVERSION;

		errCode = 0;
		
		TargaHeaderStruct header; memset(&header,0,sizeof(header));

		DWORD imgSize = width * height * 4;
		
		if (!encodingFlag)
			header.image_type = TIT_UNCOMPRESSED_TRUE_COLOR_IMAGE;
		else
			header.image_type = TIT_RLE_TRUE_COLOR_IMAGE;
		header.image_spec.width = width;
		header.image_spec.height = height;
		header.image_spec.pixel_depth = 32;

		// запись в файл
		HANDLE hOut = CreateFile(OLE2T(bstrPath),
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
		if (hOut == INVALID_HANDLE_VALUE)
		{
			errCode=16; return FALSE;
		}
			
		DWORD bWriten;
		if ((!WriteFile(hOut,&header,sizeof(TargaHeaderStruct),&bWriten,NULL)) ||
			(bWriten != sizeof(TargaHeaderStruct)))
		{
			CloseHandle(hOut); errCode=17; return FALSE;
		}

		// создавать несжатый файл
		if (!encodingFlag)
		{
			if ((!WriteFile(hOut,data,imgSize,&bWriten,NULL)) ||
				(bWriten != imgSize))
			{
				CloseHandle(hOut); errCode=18; return FALSE;
			}
		}
		
		// создавать сжатый файл
		else
		{
			DWORD * inBuff = (DWORD*)data, * firstRaw = (DWORD*)data;
			BOOL rawFlag = FALSE, firstRawFlag = TRUE;
			DWORD last = *(inBuff), curr = 0, bWriten, total=0;
			BYTE runCount=1, ch;

			for (DWORD i=0; i<(DWORD)width*height; i++)
			{
				curr = *(++inBuff);
				
				if (curr==last)
				{
					if (rawFlag)
					{
						int count;
						count=(int)(inBuff-firstRaw-1);
						if (count>0)
						{
							ch=(count-1);
							if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
								(bWriten != 1))
							{ CloseHandle(hOut); errCode=99; return FALSE; }
							if ((!WriteFile(hOut,firstRaw,count*4,&bWriten,NULL)) ||
								(bWriten != (DWORD)count*4))
							{ CloseHandle(hOut); errCode=99; return FALSE; }
							total += (1+count*4);
						}
						runCount=1; rawFlag=FALSE;
					}
					runCount++;
					if (firstRawFlag) firstRawFlag=FALSE;
					if (runCount==128)
					{
						ch = 0xFF;
						if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
							(bWriten != 1))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						if ((!WriteFile(hOut,&last,4,&bWriten,NULL)) ||
							(bWriten != 4))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						total += 5; runCount=0;
					}
				}
				else
				{
					if ((!rawFlag) && (runCount > 1))
					{
						ch = 0x80 | (runCount-1);
						if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
							(bWriten != 1))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						if ((!WriteFile(hOut,&last,4,&bWriten,NULL)) ||
							(bWriten != 4))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						total += 5; runCount=0;
					}
					if (runCount==128)
					{
						ch=0x7F;
						if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
							(bWriten != 1))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						if ((!WriteFile(hOut,firstRaw,128*4,&bWriten,NULL)) ||
							(bWriten != 128*4))
						{ CloseHandle(hOut); errCode=99; return FALSE; }
						total += (1+128*4); runCount=0;
						firstRaw = inBuff;
					}
					last=curr; runCount++;
					if (!rawFlag)
					{
						rawFlag=TRUE;
						if (firstRawFlag) { firstRawFlag=FALSE; firstRaw=(DWORD*)data; }
						else firstRaw=inBuff;
					}
				}
			}

			if (rawFlag)
			{
				int count;
				count=(int)(inBuff-firstRaw);
				if (count>0)
				{
					ch=(count-1);
					if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
						(bWriten != 1))
					{ CloseHandle(hOut); errCode=99; return FALSE; }
					if ((!WriteFile(hOut,firstRaw,count*4,&bWriten,NULL)) ||
						(bWriten != (DWORD)count*4))
					{ CloseHandle(hOut); errCode=99; return FALSE; }
					total += (1+count*4);
				}
			}
			if ((!rawFlag) && (runCount > 1))
			{
				ch = 0x80 | (runCount-1);
				if ((!WriteFile(hOut,&ch,1,&bWriten,NULL)) ||
					(bWriten != 1))
				{ CloseHandle(hOut); errCode=99; return FALSE; }
				if ((!WriteFile(hOut,&last,4,&bWriten,NULL)) ||
						(bWriten != 4))
				{ CloseHandle(hOut); errCode=99; return FALSE; }
				total += 5;
			}
		
		}

		CloseHandle(hOut);
		return TRUE;
	}
	
	/*
	int GetImgFromBuffer(SAFEARRAY *& pArray)
	{
		if (SafeArrayCopy(psarr,&pArray) != S_OK)
			return 99;

		return 0;
	}*/
	int GetImgFromBuffer(void** pData, BOOL (*CopyTo)(SAFEARRAY*, void**))
	{
		/*
		BYTE* pArrayData = (BYTE*)psarr->pvData;
		for (int index = 0; index < width*height; ++index)
		{
			pArrayData[0] = pArrayData[1] = pArrayData[2] = pArrayData[3];
			pArrayData += 4;
		}
		*/

		if (CopyTo(psarr, pData))
			return 99;

		return 0;
	}
	
private:
		
	HANDLE handle;
	TargaHeaderStruct header;

	BYTE bits_per_pixel;
	int width,height,width2;
	DWORD imgSize;
	BYTE * bits, scan_width;

	SAFEARRAYBOUND sabound[3];
	SAFEARRAY * psarr;

	int errCode;
};

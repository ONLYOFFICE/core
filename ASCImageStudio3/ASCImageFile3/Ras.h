#pragma once

#define RAS_MAGIC_NUMBER	DWORD(0x59A66A95)

#define RAS_DEPTH_1  DWORD(1)
#define RAS_DEPTH_8  DWORD(8)
#define RAS_DEPTH_24 DWORD(24)
#define RAS_DEPTH_32 DWORD(32)

#define RAS_TYPE_OLD          DWORD(0x0000)
#define RAS_TYPE_STANDART     DWORD(0x0001)
#define RAS_TYPE_BYTEENCODED  DWORD(0x0002)
#define RAS_TYPE_RGB          DWORD(0x0003)
#define RAS_TYPE_TIFF         DWORD(0x0004)
#define RAS_TYPE_IFF          DWORD(0x0005)
#define RAS_TYPE_EXPERIMENTAL DWORD(0xFFFF)

#define RAS_CMAPTYPE_NO       DWORD(0x0000)
#define RAS_CMAPTYPE_RGB      DWORD(0x0001)
#define RAS_CMAPTYPE_RAW      DWORD(0x0002)

typedef struct 
{
	DWORD MagicNumber;      // Magic (identification) number 
	DWORD Width;            // Width of image in pixels 
	DWORD Height;           // Height of image in pixels 
	DWORD Depth;            // Number of bits per pixel 
	DWORD Length;           // Size of image data in bytes 
	DWORD Type;             // Type of raster file 
	DWORD ColorMapType;     // Type of color map 
	DWORD ColorMapLength;   // Size of the color map in bytes 
} SUNRASTER;

class ImgFilesRAS
{
public:
	
	ImgFilesRAS()
	{
		rasFile = INVALID_HANDLE_VALUE;
		memset(&rasHeader,0,sizeof(rasHeader));
		
		sabound[0].cElements = 4;		// planes
		sabound[0].lLbound   = 0;
		sabound[1].cElements = 1;		// width
		sabound[1].lLbound   = 0;
		sabound[2].cElements = 1;		// height
		sabound[2].lLbound   = 0;

		psarr = 0;
	}
	~ImgFilesRAS()
	{
		if (rasFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
		}

		if (psarr) SafeArrayDestroy(psarr);
	}
		
	BOOL IsRas(BSTR bstrPath, BOOL closeFlag)
	{
		USES_CONVERSION;

		BOOL result = TRUE;

		rasFile = CreateFile(
			OLE2T(bstrPath),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (rasFile == INVALID_HANDLE_VALUE)
		{ errCode=1; return FALSE; }

		DWORD bRead;
		if ((!ReadFile(rasFile,&rasHeader,sizeof(rasHeader),&bRead,NULL)) ||
			(bRead != sizeof(rasHeader)))
		{ errCode=2; result=FALSE; }

		if ((rasHeader.MagicNumber != RAS_MAGIC_NUMBER) &&
			(rasHeader.MagicNumber != be2le(RAS_MAGIC_NUMBER)))
		{ errCode=3; result=FALSE; }

		if (rasHeader.MagicNumber == be2le(RAS_MAGIC_NUMBER))
		{
			// Преобразуем DWORD'ы заголовка из Big-Endian в Low-Endian 
			// формат
			rasHeader.MagicNumber = be2le(rasHeader.MagicNumber);
			rasHeader.Width = be2le(rasHeader.Width);
			rasHeader.Height = be2le(rasHeader.Height);
			rasHeader.Depth = be2le(rasHeader.Depth);
			rasHeader.Length = be2le(rasHeader.Length);
			rasHeader.Type = be2le(rasHeader.Type);
			rasHeader.ColorMapType = be2le(rasHeader.ColorMapType);
			rasHeader.ColorMapLength = be2le(rasHeader.ColorMapLength);
		}

		// Проверка поддерживаемых форматов чтения/записи
		if ((rasHeader.Depth!=1) && (rasHeader.Depth!=8) && 
			(rasHeader.Depth!=24) && (rasHeader.Depth!=32))
		{ errCode=4; result=FALSE; }

		if ((rasHeader.Type!=RAS_TYPE_OLD) &&
			(rasHeader.Type!=RAS_TYPE_STANDART) &&
			(rasHeader.Type!=RAS_TYPE_BYTEENCODED) &&
			(rasHeader.Type!=RAS_TYPE_RGB) &&
			(rasHeader.Type!=RAS_TYPE_TIFF) &&
			(rasHeader.Type!=RAS_TYPE_IFF))
		{ errCode=5; result=FALSE; }

		if ((closeFlag) || (!result))
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
		}

		return result;
	}

	BOOL LoadRas(BSTR bstrPath)
	{
		if (!IsRas(bstrPath,FALSE)) return FALSE;

		// RAW - палитра не поддерживается основным форматом Sun Raster
		if (rasHeader.ColorMapType==RAS_CMAPTYPE_RAW)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			errCode=99;
			return FALSE;
		}

		// получение величины файла
		DWORD size, size2;
		size = GetFileSize(rasFile,&size2);
		if(size2 == 0xFFFFFFFF)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			errCode=9;
			return FALSE; 
		}

		// считывание всего файла в память
		BYTE * data = (BYTE*) HeapAlloc(GetProcessHeap(),0,size);
		if(!data)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			errCode=10;
			return FALSE;
		}
		size -= sizeof(rasHeader);
		DWORD bRead;
		if((!ReadFile(rasFile,data,size,&bRead,NULL))||
		(bRead != size))
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			errCode=11;
			return FALSE;
		}

		// Создание массива для данных
		sabound[1].cElements = rasHeader.Width;
		sabound[2].cElements = rasHeader.Height;
		psarr = SafeArrayCreate(VT_UI1,3,sabound);
		if (psarr == NULL)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			errCode=99;
			return FALSE;
		}
		if (SafeArrayLock(psarr) != S_OK)
		{
			CloseHandle(rasFile);
			rasFile = INVALID_HANDLE_VALUE;
			HeapFree(GetProcessHeap(),0,data);
			SafeArrayDestroy(psarr);		
			errCode=99; 
			return FALSE;
		}
		memset(psarr->pvData,0,rasHeader.Width*rasHeader.Height*4);

		// Считывание данных
		if(rasHeader.Depth==32)
		{
			// x и y - "координаты"
			unsigned int x,y,yy;
			// текущая позиция чтения из буфера
			unsigned int dataPos=0;
			// текущая позиция записи в safearray
			unsigned int arrPos=0;
			BYTE R,G,B,A;

			for(y=0; y<rasHeader.Height; y++)
			{
				for(x=0; x<rasHeader.Width; x++)
				{
					if (rasHeader.Type == RAS_TYPE_RGB)
					{
						B = *(((byte*)data)+dataPos); dataPos++;
						G = *(((byte*)data)+dataPos); dataPos++;
						R = *(((byte*)data)+dataPos); dataPos++;
						A = *(((byte*)data)+dataPos); dataPos++;
					}
					else
					{
						A = *(((byte*)data)+dataPos); dataPos++;
						B = *(((byte*)data)+dataPos); dataPos++;
						G = *(((byte*)data)+dataPos); dataPos++;
						R = *(((byte*)data)+dataPos); dataPos++;
					}

					// вычисление начальной позиции пикселя в массиве
					yy = rasHeader.Height - y - 1;
					arrPos = (yy * rasHeader.Width + x)*4;
					
					// запись в массив RGB-цвета
					((byte*)psarr->pvData)[arrPos] = B; arrPos++;
					((byte*)psarr->pvData)[arrPos] = G; arrPos++;
					((byte*)psarr->pvData)[arrPos] = R; arrPos++;
					((byte*)psarr->pvData)[arrPos] = A; arrPos++;
				}
			}
		}
		else if (rasHeader.Depth==1)
		{
			// x и y - "координаты"
			unsigned int i,x,y,yy;
			// текущая позиция чтения из буфера
			unsigned int dataPos=0;
			// текущая позиция записи в safearray
			unsigned int arrPos=0;
			// Счетчик байтов в строке
			unsigned int byteCount;
			// Текущий байт
			BYTE b,j;
			int k;
			
			// Вычисляем число байтов для одной строки
			unsigned int strLen = rasHeader.Width / 8;
			unsigned int hasShortByte=0;
			if (rasHeader.Width%8) 
			{
				strLen++;
				hasShortByte = 1;
			}

			DWORD imgSize = strLen * rasHeader.Height;

			// Чтение в буфер
			x = 0;
			y = 0;
			byteCount = 0;
			while ((dataPos<size) && (y<rasHeader.Height))
			{
				k=1;
				// Читаем текущий байт
				b=((byte*)data)[dataPos]; dataPos++;
				if (rasHeader.Type == RAS_TYPE_BYTEENCODED)
				{
					if (b == 0x80)
					{
						if (dataPos >= size) break;
						b=((byte*)data)[dataPos]; dataPos++;
						if (b == 0) b=0x80;
						else
						{
							k=b+1;
							if (dataPos >= size) break;
							b=((byte*)data)[dataPos]; dataPos++;
						}
					}
				}
				// Увеличиваем счетчик байтов в строке
				byteCount += k;
				
				while (k)
				{
					// Читаем побитно
					for (i=0; i<8; i++)
					{
						// Текущий бит...
						j = 128 >> i;
						j = b & j;
						
						// вычисление начальной позиции пикселя в массиве
						yy = rasHeader.Height - y - 1;
						arrPos = (yy * rasHeader.Width + x)*4;
				
						// запись в массив RGB-цвета
						if (!j)
						{
							memset(((BYTE*)psarr->pvData)+arrPos,0xFF,3);
							arrPos+=4;
						}

						x++;
						if (x==rasHeader.Width) { x=0; y++; break; }
					}
					// Если прочитан последний, "неполный байт",
					// и длина строки - нечетная
					if((byteCount==strLen)&&(byteCount%2)) 
					{
						dataPos++;
						byteCount = 0;
					}

					k--;
				}
			}
		}
		else if (rasHeader.Depth==24)
		{
			// x и y - "координаты"
			unsigned int x,y,yy;
			// текущая позиция чтения из буфера
			unsigned int dataPos=0;
			// текущая позиция записи в safearray
			unsigned int arrPos=0;
			BYTE R,G,B;

			for(y=0; y<rasHeader.Height; y++)
			{
				for(x=0; x<rasHeader.Width; x++)
				{
					if (rasHeader.Type == RAS_TYPE_RGB)
					{
						B = *(((byte*)data)+dataPos); dataPos++;
						G = *(((byte*)data)+dataPos); dataPos++;
						R = *(((byte*)data)+dataPos); dataPos++;
					}
					else
					{
						R = *(((byte*)data)+dataPos); dataPos++;
						G = *(((byte*)data)+dataPos); dataPos++;
						B = *(((byte*)data)+dataPos); dataPos++;
					}

					// вычисление начальной позиции пикселя в массиве
					yy = rasHeader.Height - y - 1;
					arrPos = (yy * rasHeader.Width + x)*4;
					
					// запись в массив RGB-цвета
					((byte*)psarr->pvData)[arrPos] = B; arrPos++;
					((byte*)psarr->pvData)[arrPos] = G; arrPos++;
					((byte*)psarr->pvData)[arrPos] = R; arrPos++;
				}
				if(rasHeader.Width % 2) dataPos++;
			}
		}
		else if (rasHeader.Depth==8)
		{
			// Цветовая палитра
			DWORD* colorMap;
			// x и y - "координаты"
			unsigned int i,x,y,yy;
			// текущая позиция чтения из буфера
			DWORD dataPos=0;
			// текущая позиция записи в safearray
			DWORD arrPos=0;

			// Вычисляем количество цветов в палитре
			unsigned int colorCount = rasHeader.ColorMapLength / 3;
			// Выделяем место под палитру
			colorMap = (DWORD*)HeapAlloc(GetProcessHeap(),0,colorCount * 4);
			if (!colorMap)
			{
				CloseHandle(rasFile); 
				rasFile = INVALID_HANDLE_VALUE;
				HeapFree(GetProcessHeap(),0,data);
				SafeArrayUnlock(psarr); 
				SafeArrayDestroy(psarr);		
				errCode=99; 
				return FALSE;
			}
			memset(colorMap,0,colorCount*4);

			// RGB-палитра
			if(rasHeader.ColorMapType==RAS_CMAPTYPE_RGB)
			{
				// Загружаем RED-составляющую
				for(i=0; i<colorCount; i++)
				{
					((byte*)&colorMap[i])[2] = ((byte*)data)[dataPos]; 
					dataPos++;
				}
				// Загружаем GREEN-составляющую
				for(i=0; i<colorCount; i++)
				{
					((byte*)&colorMap[i])[1] = ((byte*)data)[dataPos]; 
					dataPos++;
				}
				// Загружаем BLUE-составляющую
				for(i=0; i<colorCount; i++)
				{
					((byte*)&colorMap[i])[0] = ((byte*)data)[dataPos]; 
					dataPos++;
				}
			}

			x = 0;
			y = 0;
			int k;
			BYTE b;
			while ((dataPos<size) && (y<rasHeader.Height))
			{
				k=1;
				// Читаем текущий байт
				b=((byte*)data)[dataPos]; dataPos++;
				if (rasHeader.Type == RAS_TYPE_BYTEENCODED)
				{
					if (b == 0x80)
					{
						if (dataPos >= size) break;
						b=((byte*)data)[dataPos]; dataPos++;
						if (b == 0) b=0x80;
						else
						{
							k=b+1;
							if (dataPos >= size) break;
							b=((byte*)data)[dataPos]; dataPos++;
						}
					}
				}

				while (k)
				{
					yy = rasHeader.Height - y - 1;
					arrPos = (yy * rasHeader.Width + x)*4;
					
					((byte*)psarr->pvData)[arrPos] = ((byte*)&colorMap[b])[0]; arrPos++;
					((byte*)psarr->pvData)[arrPos] = ((byte*)&colorMap[b])[1]; arrPos++;
					((byte*)psarr->pvData)[arrPos] = ((byte*)&colorMap[b])[2]; arrPos++;
					x++;
					if (x == rasHeader.Width)
					{ 
						x=0; y++;
						if (rasHeader.Width %2) dataPos++;
					}

					k--;
				}
			}

			HeapFree(GetProcessHeap(),0,colorMap);
		}

		SafeArrayUnlock(psarr);
		HeapFree(GetProcessHeap(),0,data);
		CloseHandle(rasFile);
		rasFile = INVALID_HANDLE_VALUE;
		memset(&rasHeader,0,sizeof(rasHeader));
		errCode = 0;

		return TRUE;
	}

	BOOL Save(BSTR bstrPath, int width, int height, int channels, BYTE * data, BOOL encodingFlag)
	{
		USES_CONVERSION;

		// Заполняем заголовок
		SUNRASTER header;
		header.MagicNumber = RAS_MAGIC_NUMBER;
		header.Type = 0;
		header.Width = width;
		header.Height = height;
		header.Depth = 24;
		header.ColorMapType = RAS_TYPE_RGB;
		header.ColorMapLength = 0;
		header.Length = width * height * 3;

		// Преобразуем заголовок в big-endian формат
		header.MagicNumber = be2le(header.MagicNumber);
		header.Type = be2le(header.Type);
		header.Width = be2le(header.Width);
		header.Height = be2le(header.Height);
		header.Depth = be2le(header.Depth);
		header.ColorMapType = be2le(header.ColorMapType);
		header.ColorMapLength = be2le(header.ColorMapLength);
		header.Length = be2le(header.Length);

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
			errCode=16; 
			return FALSE;
		}

		int x,y; 
		unsigned int yy, arrPos;
		char BGR[3];
		DWORD bWriten;

		// Сохраняем заголовок...
		if ((!WriteFile(hOut,&header,sizeof(header),&bWriten,NULL)) ||
		(bWriten != sizeof(header)))
		{
			errCode=17; 
			return FALSE;
		}

		for (y=0; y<height; y++)
		{
			for (x=0; x<width; x++)
			{
				// Позиция пикселя в массиве
				yy = height - y - 1;
				arrPos = (yy * width + x)*4;
				// Сохраняем пиксель
				BGR[0] = data[arrPos+2];
				BGR[1] = data[arrPos+1];
				BGR[2] = data[arrPos];
				if ((!WriteFile(hOut,BGR,3,&bWriten,NULL)) ||
				(bWriten != 3))
				{
					CloseHandle(hOut);
					errCode=17; return FALSE;
				}		
			}
			if (width%2)
			{
				BGR[0]=0;
				if ((!WriteFile(hOut,BGR,1,&bWriten,NULL)) ||
				(bWriten != 1))
				{
					CloseHandle(hOut);
					errCode=17; return FALSE;
				}
			}

		}

		CloseHandle(hOut);

		return TRUE;
	}

	
	// В функции отдается сам массив, а не его копия.
	// В этом случае будет в памяти будет всего один массив.
	/* int GetImgFromBuffer(SAFEARRAY *& pArray)
	{
		if (SafeArrayCopy(psarr,&pArray) != S_OK)
			return 99;

		return 0;
	} */
	int GetImgFromBuffer(void** pData, BOOL (*CopyTo)(SAFEARRAY*, void**))
	{
		// reset the alpha channel
		ImageStudioUtils::FillAlphaChannel(&psarr, 255);

		if (CopyTo(psarr, pData))
			return 99;

		return 0;
	}
		
protected:
		
	// Функция:    be2le
	// Назначение: преобразует число из формата Big-Ended в формат 
	//             Low-Ended
	DWORD be2le(DWORD Value)
	{
		DWORD RESULT;

		((byte*)&RESULT)[0] = ((byte*)&Value)[3];
		((byte*)&RESULT)[1] = ((byte*)&Value)[2];
		((byte*)&RESULT)[2] = ((byte*)&Value)[1];
		((byte*)&RESULT)[3] = ((byte*)&Value)[0];
		return RESULT;
	}
	
protected:
	
	HANDLE			rasFile;
	SUNRASTER		rasHeader;
	SAFEARRAYBOUND	sabound[3];
	SAFEARRAY*		psarr;
	
	int errCode;
};

DWORD be2le(DWORD Value);

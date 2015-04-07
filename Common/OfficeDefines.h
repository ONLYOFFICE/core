#include <windows.h>

#ifndef _OFFICEDEFINES_H_
#define _OFFICEDEFINES_H_

//AVSOfficeImageFile: TIFFOptions in DocumentConverter.xml

namespace TiffCompression
{
	const int None = 0;
	const int Lzw = 1;
	const int Rle = 2;
	const int CCITT3 = 3;
	const int CCITT4 = 4;
}

//AVSOfficeTxtFile: TXTOptions in DocumentConverter.xml

namespace EncodingType
{
	const int Utf8 = 0;
	const int Unicode = 1;
	const int Ansi = 2;
	const int BigEndian = 3;
}

#define PERMISSION_PRINT  ( 1 << 2 ) // Ограничение печати документа
#define PERMISSION_CHANGE ( 1 << 3 ) // Ограничение на изменение содержимого документа (текст, графика, картинки)
#define PERMISSION_COPY   ( 1 << 4 ) // Ограничение на копирование документа и его содержимого

//Тип печати
const int c_nPrintTypeFit = 0; //вписывать по аспекту
const int c_nPrintTypeStrech = 1; //растягивать
const int c_nPrintType100 = 2; // печатать как есть
//пример xml для функции PrintPage <PrintSettings><Mode ZoomEnable=\"0\" RotateEnable=\"0\" PrintableAreaEnable=\"0\">%d</Mode></PrintSettings>
//enable - 0; diable - 1 ( если атрибут не задан, то он берется как 0 )
//ZoomEnable растягивать любой документ до размеров страницы
//RotateEnable поворачивать документ,если в повернутом состоянии он будет занимать большую площадь(например при c_nPrintType100) или если в обычном состоянии он не вписывает, а в повернутом вписывается
//PrintableAreaEnable печать только в области печати принтера

namespace TextFormatting
{
  const int c_nPerCharacter = 0;
  const int c_nLineByLine = 1;
  const int c_nWordProcessing = 2;
}

//additional param сохранения нескольких документов в один для IAVSOfficeFileConverter
const int c_nSaveModeNone = 0;
const int c_nSaveModeStart = 1;
const int c_nSaveModeContinue = 2;
const int c_nSaveModeCommit = 4;
#endif //_OFFICEDEFINES_H_

/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
	//from unicodeconverter_encodings.h
	const int Utf8		= 46;
	const int Unicode	= 50;
	const int Ansi		= 1000;
	const int BigEndian = 51;
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

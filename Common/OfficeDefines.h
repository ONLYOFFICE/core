/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

#define PERMISSION_PRINT  ( 1 << 2 ) // Restriction on document printing
#define PERMISSION_CHANGE ( 1 << 3 ) // Restriction on changing document content (text, graphics, images)
#define PERMISSION_COPY   ( 1 << 4 ) // Restriction on copying document and its content

//Print type
const int c_nPrintTypeFit = 0; //fit by aspect ratio
const int c_nPrintTypeStrech = 1; //stretch
const int c_nPrintType100 = 2; // print as is
//xml example for PrintPage function <PrintSettings><Mode ZoomEnable=\"0\" RotateEnable=\"0\" PrintableAreaEnable=\"0\">%d</Mode></PrintSettings>
//enable - 0; disable - 1 (if attribute is not set, it defaults to 0)
//ZoomEnable stretch any document to page size
//RotateEnable rotate document if rotated state takes more area (e.g. with c_nPrintType100) or if normal state doesn't fit but rotated does
//PrintableAreaEnable print only in printer's printable area

namespace TextFormatting
{
  const int c_nPerCharacter = 0;
  const int c_nLineByLine = 1;
  const int c_nWordProcessing = 2;
}

//additional param for saving multiple documents into one for IAVSOfficeFileConverter
const int c_nSaveModeNone = 0;
const int c_nSaveModeStart = 1;
const int c_nSaveModeContinue = 2;
const int c_nSaveModeCommit = 4;

#endif //_OFFICEDEFINES_H_

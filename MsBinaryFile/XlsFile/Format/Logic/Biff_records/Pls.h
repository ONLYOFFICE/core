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
#pragma once

#include "BiffRecordContinued.h" 

#if !defined(_WIN32) && !defined(_WIN64)

#ifndef customTagPoint
#define customTagPoint
    typedef struct tagPOINT
    {
        _INT32  x;
        _INT32  y;
    } POINT;
#endif

    typedef struct _devicemodeW
    {
        short  dmDeviceName[/*CCHDEVICENAME*/32];//utf16
        _UINT16 dmSpecVersion;
        _UINT16 dmDriverVersion;
        _UINT16 dmSize;
        _UINT16 dmDriverExtra;
        _UINT32 dmFields;
        union
        {
          /* printer only fields */
          struct
          {
            short dmOrientation;
            short dmPaperSize;
            short dmPaperLength;
            short dmPaperWidth;
            short dmScale;
            short dmCopies;
            short dmDefaultSource;
            short dmPrintQuality;
          };
          /* display only fields */
          struct
          {
            POINT dmPosition;
            _UINT32  dmDisplayOrientation;
            _UINT32  dmDisplayFixedOutput;
          };
        };
        short dmColor;
        short dmDuplex;
        short dmYResolution;
        short dmTTOption;
        short dmCollate;
        short  dmFormName[/*CCHFORMNAME*/32];//utf16
        _UINT16   dmLogPixels;
        _UINT32  dmBitsPerPel;
        _UINT32  dmPelsWidth;
        _UINT32  dmPelsHeight;
        union {
            _UINT32  dmDisplayFlags;
            _UINT32  dmNup;
        };
        _UINT32  dmDisplayFrequency;
    #if(WINVER >= 0x0400)
        _UINT32  dmICMMethod;
        _UINT32  dmICMIntent;
        _UINT32  dmMediaType;
        _UINT32  dmDitherType;
        _UINT32  dmReserved1;
        _UINT32  dmReserved2;
    #if (WINVER >= 0x0500) || (_WIN32_WINNT >= _WIN32_WINNT_NT4)
        _UINT32  dmPanningWidth;
        _UINT32  dmPanningHeight;
    #endif
    #endif /* WINVER >= 0x0400 */
    } DEVMODE;
#else
	#include <windows.h>
#endif

namespace XLS
{

	class Pls : public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pls)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pls)
public:
	Pls();
	~Pls();

	BaseObjectPtr clone();

	
	void readFields(CFRecord& record);

	static const ElementType	type = typePls;
//-----------------------------
	DEVMODE		rgb;	
	_INT32		bin_data_id;

};

} // namespace XLS


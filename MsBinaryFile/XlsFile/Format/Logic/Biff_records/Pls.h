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


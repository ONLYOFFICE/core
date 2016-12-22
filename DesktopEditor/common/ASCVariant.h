/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _ASCVARIANT_H_
#define _ASCVARIANT_H_

#include "Types.h"
#include "../../Common/DocxFormat/Source/Base/ASCString.h"

typedef short ASC_VARIANT_BOOL;

enum ASC_VARENUM
    {
        ASC_VT_EMPTY	= 0,
        ASC_VT_I2       = 2,
        ASC_VT_I4       = 3,
        ASC_VT_R4       = 4,
        ASC_VT_R8       = 5,
        ASC_VT_STR      = 8,
        ASC_VT_BOOL     = 11,
        ASC_VT_DECIMAL	= 14,
        ASC_VT_I1       = 16,
        ASC_VT_UI1      = 17,
        ASC_VT_UI2      = 18,
        ASC_VT_UI4      = 19,
        ASC_VT_I8       = 20,
        ASC_VT_UI8      = 21,
        ASC_VT_INT      = 22,
        ASC_VT_UINT     = 23,
    } ;


typedef unsigned short ASC_VARTYPE;

struct ASC_VARIANT
{
    ASC_VARTYPE     vt;

    LONG64          llVal;
    LONG            lVal;
    BYTE            bVal;
    SHORT           iVal;
    FLOAT           fltVal;
    std::wstring    strVal;
    bool            boolVal;
    double          dblVal;
};
/*
struct tagVARIANT
    {
    union
        {
        struct __tagVARIANT
            {
            VARTYPE vt;
            WORD wReserved1;
            WORD wReserved2;
            WORD wReserved3;
            union
                {
                LONGLONG llVal;
                LONG lVal;
                BYTE bVal;
                SHORT iVal;
                FLOAT fltVal;
                DOUBLE dblVal;
                VARIANT_BOOL boolVal;
                _VARIANT_BOOL bool;
                SCODE scode;
                CY cyVal;
                DATE date;
                BSTR bstrVal;
                IUnknown *punkVal;
                IDispatch *pdispVal;
                SAFEARRAY *parray;
                BYTE *pbVal;
                SHORT *piVal;
                LONG *plVal;
                LONGLONG *pllVal;
                FLOAT *pfltVal;
                DOUBLE *pdblVal;
                VARIANT_BOOL *pboolVal;
                _VARIANT_BOOL *pbool;
                SCODE *pscode;
                CY *pcyVal;
                DATE *pdate;
                BSTR *pbstrVal;
                IUnknown **ppunkVal;
                IDispatch **ppdispVal;
                SAFEARRAY **pparray;
                VARIANT *pvarVal;
                PVOID byref;
                CHAR cVal;
                USHORT uiVal;
                ULONG ulVal;
                ULONGLONG ullVal;
                INT intVal;
                UINT uintVal;
                DECIMAL *pdecVal;
                CHAR *pcVal;
                USHORT *puiVal;
                ULONG *pulVal;
                ULONGLONG *pullVal;
                INT *pintVal;
                UINT *puintVal;
                struct __tagBRECORD
                    {
                    PVOID pvRecord;
                    IRecordInfo *pRecInfo;
                    } 	__VARIANT_NAME_4;
                } 	__VARIANT_NAME_3;
            } 	__VARIANT_NAME_2;
        DECIMAL decVal;
        } 	__VARIANT_NAME_1;
    } ;
    */
typedef ASC_VARIANT *ASC_LPVARIANT;

typedef ASC_VARIANT ASC_VARIANTARG;

typedef ASC_VARIANT *ASC_LPVARIANTARG;


#endif //_ASCVARIANT_H_

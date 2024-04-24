/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "raster.h"

void* Raster_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void Raster_Free(void* p)
{
    if (p) ::free(p);
}
CBgraFrame* Raster_Create()
{
    return new CBgraFrame();
}
CBgraFrame* Raster_Load(unsigned char* buffer, int size)
{
    CBgraFrame* oRes = new CBgraFrame();
    oRes->put_IsRGBA(true);
    oRes->Decode(buffer, size);
    return oRes;
}
CBgraFrame* Raster_Init(double width_px, double height_px)
{
    int nRasterW = (int)width_px;
    int nRasterH = (int)height_px;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame* oRes = new CBgraFrame();
    oRes->put_IsRGBA(true);
    oRes->put_Data(pData);
    oRes->put_Width(nRasterW);
    oRes->put_Height(nRasterH);
    return oRes;
}
void Raster_Destroy(CBgraFrame* p)
{
    if (p) delete p;
}
int  Raster_GetHeight(CBgraFrame* p)
{
    if (p) return p->get_Height();
    return -1;
}
int  Raster_GetWidth (CBgraFrame* p)
{
    if (p) return p->get_Width();
    return -1;
}
bool Raster_Decode(CBgraFrame* p, unsigned char* buffer, int size)
{
    if (p)
    {
        p->put_IsRGBA(true);
        bool bRes = p->Decode(buffer, size);
        return bRes;
    }
    return false;
}
unsigned char* Raster_GetRGBA(CBgraFrame* p)
{
    unsigned char* buffer = NULL;
    if (p) buffer = p->get_Data();
    return buffer;
}

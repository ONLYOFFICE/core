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
#include "TimeVariant.h"

using namespace PPT;

CRecordTimeVariant* PPT::TimeVariantFactoryMethod(SRecordHeader & oHeader, POLE::Stream* pStream)
{
    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    CRecordTimeVariant* pTimeVariant(nullptr);
    CRecordTimeVariant tempTimeVariant;
    tempTimeVariant.ReadFromStream(oHeader, pStream);
    StreamUtils::StreamSeek(lPos, pStream);

    switch (tempTimeVariant.m_Type)
    {
    case TL_TVT_Bool:
    {
        pTimeVariant = new CRecordTimeVariantBool;
        pTimeVariant->ReadFromStream(oHeader, pStream);
        break;
    }
    case TL_TVT_Int:
    {
        pTimeVariant = new CRecordTimeVariantInt;
        pTimeVariant->ReadFromStream(oHeader, pStream);
        break;
    }
    case TL_TVT_Float:
    {
        pTimeVariant = new CRecordTimeVariantFloat;
        pTimeVariant->ReadFromStream(oHeader, pStream);
        break;
    }
    case TL_TVT_String:
    {
        pTimeVariant = new CRecordTimeVariantString;
        pTimeVariant->ReadFromStream(oHeader, pStream);
        break;
    }
    }
    StreamUtils::StreamSeek(lPos + oHeader.RecLen, pStream);

    return pTimeVariant;
}

void CRecordTimeVariant::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );
}

CRecordTimeVariant::~CRecordTimeVariant()
{

}

CRecordTimeVariant &CRecordTimeVariant::operator=(const CRecordTimeVariant &src)
{
    CUnknownRecord::operator=(src);
    m_Type = src.m_Type;

    return *this;
}

void CRecordTimeVariantBool::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordTimeVariant::ReadFromStream(oHeader, pStream);
    m_Value	= ( 0x1 == StreamUtils::ReadBYTE ( pStream ) );
}

CRecordTimeVariant &CRecordTimeVariantBool::operator=(const CRecordTimeVariant &src)
{
    CRecordTimeVariant::operator=(src);
    m_Value = dynamic_cast<const CRecordTimeVariantBool&>(src).m_Value;

    return *this;
}

void CRecordTimeVariantInt::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordTimeVariant::ReadFromStream(oHeader, pStream);
    m_Value	= StreamUtils::ReadDWORD ( pStream );
}

CRecordTimeVariant &CRecordTimeVariantInt::operator=(const CRecordTimeVariant &src)
{
    CRecordTimeVariant::operator=(src);
    m_Value = dynamic_cast<const CRecordTimeVariantInt&>(src).m_Value;

    return *this;
}

void CRecordTimeVariantFloat::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordTimeVariant::ReadFromStream(oHeader, pStream);
    m_Value	= StreamUtils::ReadFLOAT ( pStream );
}

CRecordTimeVariant &CRecordTimeVariantFloat::operator=(const CRecordTimeVariant &src)
{
    CRecordTimeVariant::operator=(src);
    m_Value = dynamic_cast<const CRecordTimeVariantFloat&>(src).m_Value;

    return *this;
}

void CRecordTimeVariantString::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    if (oHeader.RecLen == 0)
        return;

    LONG lPos; StreamUtils::StreamPosition(lPos, pStream);
    lPos += oHeader.RecLen;

    CRecordTimeVariant::ReadFromStream(oHeader, pStream);
    int strLen = m_oHeader.RecLen / 2 - 1;
    if (strLen > 0)
    {
        m_Value = StreamUtils::ReadStringW(pStream, strLen);
    }
    StreamUtils::StreamSeek(lPos, pStream);
}

CRecordTimeVariant &CRecordTimeVariantString::operator=(const CRecordTimeVariant &src)
{
    CRecordTimeVariant::operator=(src);
    m_Value = dynamic_cast<const CRecordTimeVariantString&>(src).m_Value;

    return *this;
}

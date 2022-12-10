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

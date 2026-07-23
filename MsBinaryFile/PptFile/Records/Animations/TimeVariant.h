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


#include "../../Reader/Records.h"
#include "../../Enums/_includer.h"


namespace PPT
{

class CRecordTimeVariant : public CUnknownRecord
{
public:

    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )override;

    virtual ~CRecordTimeVariant();

    virtual CRecordTimeVariant &operator=(const CRecordTimeVariant& src) ;

public:
    TimeVariantTypeEnum		m_Type;
};


class CRecordTimeVariantBool : public CRecordTimeVariant
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;

    virtual CRecordTimeVariant& operator=(const CRecordTimeVariant& src) override;

    virtual ~CRecordTimeVariantBool(){}

public:
    bool					m_Value;
};

class CRecordTimeVariantInt : public CRecordTimeVariant
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )override;

    virtual CRecordTimeVariant& operator=(const CRecordTimeVariant& src) override;

    virtual ~CRecordTimeVariantInt(){}

public:
    _UINT32                         m_Value;
};

class CRecordTimeVariantFloat : public CRecordTimeVariant
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;

    virtual CRecordTimeVariant& operator=(const CRecordTimeVariant& src) override;

public:
    FLOAT                           m_Value;
};

class CRecordTimeVariantString : public CRecordTimeVariant
{
public:
    virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream ) override;

    virtual CRecordTimeVariant& operator=(const CRecordTimeVariant& src) override;

public:
    std::wstring            m_Value;
};



class CRecordTimeDisplayType : public CRecordTimeVariantInt
{

};

class CRecordTimeMasterRelType : public CRecordTimeVariantInt
{

};

class CRecordTimeSubType : public CRecordTimeVariantInt
{

};

class CRecordTimeEffectID : public CRecordTimeVariantInt
{

};

class CRecordTimeEffectDir  : public CRecordTimeVariantInt
{

};

class CRecordTimeEffectType : public CRecordTimeVariantInt
{

};

class CRecordTimeAfterEffect : public CRecordTimeVariantBool
{

};

class CRecordTimeSlideCount : public CRecordTimeVariantInt
{

};

class CRecordTimeNodeTimeFilter : public CRecordTimeVariantString
{

};

class CRecordTimeEventFilter : public CRecordTimeVariantString
{

};

class CRecordTimeHideWhenStopped : public CRecordTimeVariantBool
{

};

class CRecordTimeGroupID : public CRecordTimeVariantInt
{

};

class CRecordTimeEffectNodeType  : public CRecordTimeVariantInt
{

};

class CRecordTimePlaceholderNode : public CRecordTimeVariantBool
{

};

class CRecordTimeMediaVolume : public CRecordTimeVariantFloat
{

};

class CRecordTimeMediaMute : public CRecordTimeVariantBool
{

};

class CRecordTimeZoomToFullScreen : public CRecordTimeVariantBool
{

};


class CRecordTimeRuntimeContext : public CRecordTimeVariantString
{

};

class CRecordColorModel : public CRecordTimeVariantInt
{

};

class CRecordTimeColorModel: public CRecordTimeVariantInt
{

};

class CRecordTimeColorDirection : public CRecordTimeVariantInt
{

};

class CRecordTimeOverride : public CRecordTimeVariantInt
{

};

class CRecordTimePointsTypes : public CRecordTimeVariantString
{

};

class CRecordTagNameAtom : public CRecordTimeVariantString
{

};

class CRecordTagValueAtom : public CRecordTimeVariantString
{

};

CRecordTimeVariant* TimeVariantFactoryMethod(SRecordHeader & oHeader, POLE::Stream* pStream);
}

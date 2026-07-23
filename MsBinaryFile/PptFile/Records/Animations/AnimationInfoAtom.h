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
#include "../../Structures/ColorIndexStruct.h"
#include <array>

namespace PPT
{
class CRecordAnimationInfoAtom : public CUnknownRecord
{
public:
    ColorIndexStruct m_sDimColor;

    USHORT      m_fReverse;
    USHORT      m_fAutomatic;
    USHORT      m_fSound;
    USHORT      m_fStopSound;
    USHORT      m_fPlay;
    USHORT      m_fSynchronous;
    USHORT      m_fHide;
    USHORT      m_fAnimateBg;

    USHORT      m_Reserved;

    _UINT32     m_SoundIdRef;
    _UINT32     m_OldSoundIdRef;
    _UINT32     m_DelayTime;

    USHORT      m_OrderID;
    USHORT      m_SlideCount;

    BYTE        m_AnimBuildType;
    BYTE        m_AnimEffect;
    BYTE        m_AnimEffectDirection;
    BYTE        m_AnimAfterEffect;
    BYTE        m_TextBuildSubEffect;
    BYTE        m_OleVerb;

    std::array<BYTE, 36> asByteArr{};

public:
    CRecordAnimationInfoAtom();
    ~CRecordAnimationInfoAtom();

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);
};
}

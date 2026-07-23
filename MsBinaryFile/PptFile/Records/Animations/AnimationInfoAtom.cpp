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
#include "AnimationInfoAtom.h"

using namespace PPT;

struct SFlagsAH {

    USHORT m_fA_H;

    USHORT getH()const {return (m_fA_H & 0xC000) >> 14;}
    USHORT getG()const {return (m_fA_H & 0x3000) >> 12;}
    USHORT getF()const {return (m_fA_H & 0xC00) >> 10;}
    USHORT getE()const {return (m_fA_H & 0x300) >> 8;}
    USHORT getD()const {return (m_fA_H & 0xC0) >> 6;}
    USHORT getC()const {return (m_fA_H & 0x30) >> 4;}
    USHORT getB()const {return (m_fA_H & 0xC) >> 2;}
    USHORT getA()const {return (m_fA_H & 0x3) >> 0;}



    void ReadFromStream(POLE::Stream* pStream) {
        m_fA_H = StreamUtils::ReadWORD(pStream);
    }
};


CRecordAnimationInfoAtom::CRecordAnimationInfoAtom()
{

}

CRecordAnimationInfoAtom::~CRecordAnimationInfoAtom()
{

}

void CRecordAnimationInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_sDimColor.ReadFromStream(pStream);

    SFlagsAH    fAH;
    fAH.ReadFromStream(pStream);

    m_fReverse      = fAH.getA();
    m_fAutomatic    = fAH.getB();
    m_fSound        = fAH.getC();
    m_fStopSound    = fAH.getD();
    m_fPlay         = fAH.getE();
    m_fSynchronous  = fAH.getF();
    m_fHide         = fAH.getG();
    m_fAnimateBg    = fAH.getH();

    m_Reserved                  = StreamUtils::ReadWORD(pStream);

    m_SoundIdRef                = StreamUtils::ReadDWORD(pStream);
    m_DelayTime                 = StreamUtils::ReadDWORD(pStream);

    m_OrderID                   = StreamUtils::ReadWORD(pStream);
    m_SlideCount                = StreamUtils::ReadWORD(pStream);

    m_AnimBuildType             = StreamUtils::ReadBYTE(pStream);
    m_AnimEffect                = StreamUtils::ReadBYTE(pStream);
    m_AnimEffectDirection       = StreamUtils::ReadBYTE(pStream);
    m_AnimAfterEffect           = StreamUtils::ReadBYTE(pStream);
    m_TextBuildSubEffect        = StreamUtils::ReadBYTE(pStream);
    m_OleVerb                   = StreamUtils::ReadBYTE(pStream);


    StreamUtils::StreamSkip(2, pStream);
    m_OldSoundIdRef = m_SoundIdRef;

    StreamUtils::StreamSkipBack(36, pStream);
    auto tempStr = StreamUtils::ReadStringA(pStream, 36);
    std::copy(tempStr.begin(), tempStr.end(), asByteArr.begin());
}

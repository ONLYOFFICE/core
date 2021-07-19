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


#include "../Reader/Records.h"
#include "../../Structures/ColorIndexStruct.h"

namespace PPT_FORMAT
{
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
public:

    CRecordAnimationInfoAtom()
    {
    }

    ~CRecordAnimationInfoAtom()
    {
    }

    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
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
    }
};
}

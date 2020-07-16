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
#include "Drawing/ColorIndex.h"

struct SFlagsAH {
    USHORT m_nfAH;
    // Is masks order ok?
    // Is shift right? May be 2, 8, 20, 80 ...
    unsigned getA()const {return m_nfAH & 0x4000;}
    unsigned getB()const {return m_nfAH & 0x1000;}
    unsigned getC()const {return m_nfAH & 0x400;}
    unsigned getD()const {return m_nfAH & 0x100;}
    unsigned getE()const {return m_nfAH & 0x40;}
    unsigned getF()const {return m_nfAH & 0x10;}
    unsigned getG()const {return m_nfAH & 0x4;}
    unsigned getH()const {return m_nfAH & 0x1;}



    void ReadFromStream(POLE::Stream* pStream) {
        m_nfAH = StreamUtils::ReadWORD(pStream);
    }
};

class CRecordAnimationInfoAtom : public CUnknownRecord
{
public:
    SColorIndex m_sDimColor;
    SFlagsAH    m_fA_H;

    USHORT      m_Reserved;

    _UINT32     m_SoundIdRef;
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
        m_fA_H.ReadFromStream(pStream);

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
	}
};




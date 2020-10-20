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
#include "TimeVariant.h"
#include "TimeAnimationValueAtom.h"


namespace PPT_FORMAT
{

class CRecordTimeAnimationEntry
{
public:

<<<<<<< HEAD
    CRecordTimeAnimationEntry()
=======
    CRecordTimeAnimationEntry() : m_pVarValue(nullptr)
>>>>>>> 883dda0f3f5c184ddedd4af6243a78aa6274526a
    {

    }

<<<<<<< HEAD
    virtual ~CRecordTimeAnimationEntry()
    {
=======
    CRecordTimeAnimationEntry& operator=(const CRecordTimeAnimationEntry& src)
    {
        RELEASEOBJECT(m_pVarValue)

        m_VarFormula = src.m_VarFormula;
        m_oTimeAnimationValueAtom = src.m_oTimeAnimationValueAtom;
        m_pVarValue = new CRecordTimeVariant;
        m_pVarValue = src.m_pVarValue;

        return *this;
    }

    CRecordTimeAnimationEntry(const CRecordTimeAnimationEntry& src) : m_pVarValue(nullptr)
    {
        operator=(src);
    }

    ~CRecordTimeAnimationEntry()
    {
        RELEASEOBJECT(m_pVarValue)
>>>>>>> 883dda0f3f5c184ddedd4af6243a78aa6274526a
    }

    virtual void ReadFromStream ( UINT& CurLen, SRecordHeader & oHeader, POLE::Stream* pStream )
    {
        m_oTimeAnimationValueAtom.ReadFromStream ( oHeader, pStream );

//        UINT res = 0;
        SRecordHeader ReadHeader;

        if ( ReadHeader.ReadFromStream(pStream) )
        {
            m_pVarValue = TimeVariantFactoryMethod( ReadHeader, pStream );
        }

        CurLen += 8 + ReadHeader.RecLen;

        if ( ReadHeader.ReadFromStream(pStream) )
        {
            m_VarFormula.ReadFromStream ( ReadHeader, pStream );
        }

        CurLen += 8 + ReadHeader.RecLen;
    }

    CRecordTimeAnimationValueAtom       m_oTimeAnimationValueAtom;
<<<<<<< HEAD
    nullable<CRecordTimeVariant>        m_pVarValue;
=======
    CRecordTimeVariant*                 m_pVarValue;
>>>>>>> 883dda0f3f5c184ddedd4af6243a78aa6274526a
    CRecordTimeVariantString            m_VarFormula;
};

}

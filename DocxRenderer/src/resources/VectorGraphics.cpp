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

#include <algorithm>
#include "VectorGraphics.h"
#include "../DesktopEditor/common/Types.h"
#include <string.h>

namespace NSDocxRenderer
{
    CVectorGraphics::CVectorGraphics()
    {
        m_pData = nullptr;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;

        End();
    }

    CVectorGraphics::~CVectorGraphics()
    {
        RELEASEMEM(m_pData);
    }

    void CVectorGraphics::AddSize(size_t nSize)
    {
        if (nullptr == m_pData)
        {
            m_lSize = std::max(nSize, (size_t)500);
            m_pData = (double *)malloc(m_lSize * sizeof(double));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
            {
                m_lSize *= 2;
            }

            double *pRealloc = (double *)realloc(m_pData, m_lSize * sizeof(double));
            if (nullptr != pRealloc)
            {
                // реаллок сработал
                m_pData = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                double *pMalloc = (double *)malloc(m_lSize * sizeof(double));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(double));

                free(m_pData);
                m_pData = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

    void CVectorGraphics::MoveTo(const double &x1, const double &y1)
    {
        AddSize(3);
        *m_pDataCur = vgtMove;
        ++m_pDataCur;

        *m_pDataCur = x1;
        ++m_pDataCur;
        *m_pDataCur = y1;
        ++m_pDataCur;

        m_lSizeCur += 3;

        CheckPoint(x1, y1);
    }

    void CVectorGraphics::LineTo(const double &x1, const double &y1)
    {
        AddSize(3);
        *m_pDataCur = vgtLine;
        ++m_pDataCur;

        *m_pDataCur = x1;
        ++m_pDataCur;
        *m_pDataCur = y1;
        ++m_pDataCur;

        m_lSizeCur += 3;

        CheckPoint(x1, y1);
    }

    void CVectorGraphics::CurveTo(const double &x1, const double &y1,
                                  const double &x2, const double &y2,
                                  const double &x3, const double &y3)
    {
        AddSize(7);
        *m_pDataCur = vgtCurve;
        ++m_pDataCur;

        *m_pDataCur = x1;
        ++m_pDataCur;
        *m_pDataCur = y1;
        ++m_pDataCur;
        *m_pDataCur = x2;
        ++m_pDataCur;
        *m_pDataCur = y2;
        ++m_pDataCur;
        *m_pDataCur = x3;
        ++m_pDataCur;
        *m_pDataCur = y3;
        ++m_pDataCur;

        m_lSizeCur += 7;

        CheckPoint(x1, y1);
        CheckPoint(x2, y2);
        CheckPoint(x3, y3);
    }

    void CVectorGraphics::Close()
    {
        AddSize(1);
        *m_pDataCur = vgtClose;
        ++m_pDataCur;

        m_lSizeCur += 1;
    }

    size_t CVectorGraphics::GetCurSize() const
    {
        return m_lSizeCur;
    }

    void CVectorGraphics::Clear()
    {
        RELEASEMEM(m_pData);

        m_pData = nullptr;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }

    void CVectorGraphics::ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }

    void CVectorGraphics::End()
    {
        ClearNoAttack();

        //todo
        m_dLeft = 0xFFFFFF;
        m_dTop = 0xFFFFFF;
        m_dRight = -0xFFFFFF;
        m_dBottom = -0xFFFFFF;
    }

    void CVectorGraphics::CheckPoint(const double &x, const double &y)
    {
        if (m_dLeft > x)
            m_dLeft = x;
        if (m_dRight < x)
            m_dRight = x;
        if (m_dTop > y)
            m_dTop = y;
        if (m_dBottom < y)
            m_dBottom = y;
    }
}

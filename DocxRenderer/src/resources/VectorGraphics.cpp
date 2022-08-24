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

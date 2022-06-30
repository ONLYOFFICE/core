#include "TextLine.h"
#include "../resources/Constants.h"
#include "../resources/SortElements.h"

namespace NSDocxRenderer
{
    CTextLine::CTextLine() : m_arConts()
    {
        m_dBaselinePos    = 0;
        m_dBaselineOffset = 0;

        m_dX		 	  = 0;
        m_dY			  = 0;
        m_dWidth		  = 0;
        m_dHeight		  = 0;

        m_eAlignmentType  = atatUnknown;

        m_pDominantShape  = nullptr;
    }

    void CTextLine::Clear()
    {
        for (size_t i = 0; i < m_arConts.size(); i++)
        {
            RELEASEOBJECT(m_arConts[i]);
        }
        m_arConts.clear();
    }

    CTextLine::~CTextLine()
    {
        Clear();
    }

    CTextLine::CTextLine(const CTextLine& oSrc)
    {
        *this = oSrc;
    }

    CTextLine& CTextLine::operator=(const CTextLine& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        Clear();

        for (size_t i = 0; i < oSrc.m_arConts.size(); i++)
        {
            m_arConts.push_back(new CContText(*oSrc.m_arConts[i]));
        }

        m_dBaselinePos	  = oSrc.m_dBaselinePos;
        m_dBaselineOffset = oSrc.m_dBaselineOffset;

        m_dX			  = oSrc.m_dX;
        m_dY			  = oSrc.m_dY;
        m_dWidth		  = oSrc.m_dWidth;
        m_dHeight		  = oSrc.m_dHeight;

        m_eAlignmentType  = oSrc.m_eAlignmentType;

        m_pDominantShape  = oSrc.m_pDominantShape;

        return *this;
    }

    void CTextLine::AddCont(CContText* pCont)
    {
        //if (0 == m_arConts.size())
        m_dBaselineOffset = fabs(m_dBaselineOffset) > fabs(pCont->m_dBaselineOffset) ? m_dBaselineOffset : pCont->m_dBaselineOffset;

        if ( ( pCont->m_dX > 0 ) && ( ( m_dX == 0 ) || ( pCont->m_dX < m_dX ) ) )
            m_dX = pCont->m_dX;

        if (m_dHeight < pCont->m_dHeight)
            m_dHeight = pCont->m_dHeight;

        m_arConts.push_back(pCont);
    }

    bool CTextLine::IsBigger(const CTextLine* oSrc)
    {
        return (m_dBaselinePos > oSrc->m_dBaselinePos) ? true : false;
    }

    bool CTextLine::IsBiggerOrEqual(const CTextLine* oSrc)
    {
        return (m_dBaselinePos >= oSrc->m_dBaselinePos) ? true : false;
    }

    void CTextLine::SortConts()
    {
        // сортировка непрерывных слов по m_dX
        SortElements(m_arConts);
    }

    void CTextLine::Merge(CTextLine* pTextLine)
    {
        size_t nCount = pTextLine->m_arConts.size();
        if (0 != nCount)
        {
            if (pTextLine->m_dX < m_dX)
            {
                m_dX = pTextLine->m_dX;
            }
            if (pTextLine->m_dBaselinePos < m_dBaselinePos)
            {
                m_dHeight = (m_dBaselinePos - pTextLine->m_dBaselinePos + pTextLine->m_dHeight);
            }
            else
            {
                m_dHeight = (pTextLine->m_dBaselinePos - m_dBaselinePos + m_dHeight);
            }

            for (size_t i = 0; i < nCount; ++i)
            {
                m_arConts.push_back(pTextLine->m_arConts[i]);
            }
        }
    }

    void CTextLine::Analyze()
    {
        size_t nCountConts = m_arConts.size();

        if (0 == nCountConts)
            return;

        CContText* pFirst = m_arConts[0];

        for (size_t i = 1; i < nCountConts; ++i)
        {
            CContText* pCurrent = m_arConts[i];

            double dFirstRight = pFirst->m_dX + pFirst->m_dWidth;
            double dCurrLeft = pCurrent->m_dX;
            double dDelta = dFirstRight - dCurrLeft;

            if (!pFirst->IsEqual(pCurrent) ||
                fabs(dDelta) > c_dTHE_STRING_X_PRECISION_MM)
            {
                if (i < nCountConts - 1)
                {
                    //переходим на
                    pFirst = m_arConts[i];
                }
                continue;
            }

            // продолжаем слово
            pFirst->m_oText += pCurrent->m_oText;
            pFirst->m_dWidth += pCurrent->m_dWidth + fabs(dDelta);

            m_arConts.erase(m_arConts.begin() + i);
            --i;
            --nCountConts;
        }
    }

    void CTextLine::CalculateWidth()
    {
        m_dWidth = m_arConts[0]->m_dWidth;

        for (size_t i = 1; i < m_arConts.size(); ++i)
        {
            m_dWidth += m_arConts[i]->m_dX - (m_arConts[i-1]->m_dX + m_arConts[i-1]->m_dWidth);
            m_dWidth += m_arConts[i]->m_dWidth;
        }
    }

    void CTextLine::AddSpaceToEnd()
    {
        if (m_arConts.empty())
        {
            return;
        }

        CContText* pCurrent = m_arConts.back();

        if (pCurrent->m_oText[pCurrent->m_oText.length()-1] != uint32_t(' '))
        {
            pCurrent->AddSpaceToEnd();
            m_dWidth += pCurrent->m_dSpaceWidthMM;
        }
    }

    void CTextLine::DetermineAssumedTextAlignmentType(double dWidthOfPage)
    {
        //рассматриваем строки, которые короче трети ширины страницы
        double maxTextLineWidth = dWidthOfPage/3; //нужна какая-нибудь отправная точка...
        double delta = 2 * c_dCENTER_POSITION_ERROR_MM; //координата m_dWidth/2 +- c_dCENTER_POSITION_ERROR_MM

        if (fabs(dWidthOfPage/2 - m_dX - m_dWidth/2) <= delta && //если середины линий по x одинаковы
                m_dWidth < maxTextLineWidth )
        {
            m_eAlignmentType = atatByCenter;
        }
        else if ((m_dX + m_dWidth/2) > (dWidthOfPage/2 + c_dCENTER_POSITION_ERROR_MM) && //середина строки правее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByRightEdge;
        }
        else if ((m_dX + m_dWidth/2) < (dWidthOfPage/2 - c_dCENTER_POSITION_ERROR_MM) && //середина строки левее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByLeftEdge;
        }
        else if (fabs(dWidthOfPage/2 - m_dX - m_dWidth/2) <= delta &&
                 m_dWidth > maxTextLineWidth + maxTextLineWidth/2 )
        {
            m_eAlignmentType = atatByWidth;
        }
        else
        {
            m_eAlignmentType = atatUnknown;
        }
    }

    bool CTextLine::AreAlignmentsAppropriate(const CTextLine* oSrc)
    {
        if ((m_eAlignmentType == oSrc->m_eAlignmentType && m_eAlignmentType!= atatByLeftEdge) ||
                (m_eAlignmentType == atatByWidth && oSrc->m_eAlignmentType == atatByLeftEdge) ||
                (m_eAlignmentType == atatByWidth && oSrc->m_eAlignmentType == atatUnknown) ||
                (m_eAlignmentType == atatUnknown && oSrc->m_eAlignmentType == atatByWidth))
        {
            return true;
        }
        return false;
    }

    bool CTextLine::AreLinesCrossing(const CTextLine* oSrc)
    {
        double dCurrentTop = m_dBaselinePos - m_dHeight - m_dBaselineOffset;
        double dNextTop = oSrc->m_dBaselinePos - oSrc->m_dHeight - oSrc->m_dBaselineOffset;

        if ((oSrc->m_dBaselinePos < m_dBaselinePos && dCurrentTop < oSrc->m_dBaselinePos) ||
                (oSrc->m_dBaselinePos > m_dBaselinePos && dNextTop < m_dBaselinePos))
        {
            return true;
        }
        return false;
    }

    double CTextLine::CalculateBeforeSpacing(const double* pPreviousStringOffset)
    {
        return m_dBaselinePos - *pPreviousStringOffset - m_dHeight - m_dBaselineOffset;
    }

    double CTextLine::CalculateStringOffset()
    {
        return m_dBaselinePos - m_dBaselineOffset;
    }

    double CTextLine::CalculateRightBorder(const double& dPageWidth)
    {
        return dPageWidth - (m_dX + m_dWidth);
    }

    bool CTextLine::IsForceBlock()
    {
        // линия отсортирована, так что сравниваем только соседние conts
        size_t nCount = m_arConts.size();
        if (nCount <= 1)
            return false;

        for (size_t i = 0; i < nCount; i++)
        {
            for (size_t j = i + 1; j < nCount; j++)
            {
                if (m_arConts[i]->GetIntersect(m_arConts[j]) > 10)
                    return true;
            }
        }
        return false;
    }

    void CTextLine::ToXml(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight)
    {
        size_t nCountConts = m_arConts.size();

        if (0 == nCountConts)
            return;

        CContText* pPrev = m_arConts[0];
        double dDelta = 0;

        for (size_t i = 1; i < nCountConts; ++i)
        {
            CContText* pCurrent = m_arConts[i];

            dDelta = pCurrent->m_dX - (pPrev->m_dX + pPrev->m_dWidth);

            if (dDelta < c_dTHE_STRING_X_PRECISION_MM)
            {
                // просто текст на тексте или сменились настройки (font/brush)
                pPrev->ToXml(oWriter, pManagerLight);
                pPrev  = pCurrent;
            }
            //else if (dDelta < 2 * pPrev->m_dSpaceWidthMM)
            //{
            //	// сменились настройки, но пробел все-таки вставить нужно
            //	pPrev->Write(oWriter, pManagerLight, true);
            //	pPrev = pCurrent;
            //}
            else
            {
                // расстояние слишком большое. нужно сделать большой пробел
                pPrev->ToXml(oWriter, pManagerLight);
                pPrev->AddWideSpaceToXml(dDelta, oWriter, pManagerLight);
                pPrev = pCurrent;
            }
        }

        pPrev->ToXml(oWriter, pManagerLight);
    }
}

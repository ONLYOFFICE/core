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
#include "Elements.h"

namespace PPT
{
    class CAudioPart
    {
    public:
        std::wstring            m_strFile;

        double			m_dStartTime;
        double			m_dEndTime;

        double			m_dClipStartTime;
        double			m_dClipEndTime;

        double			m_dAudioDuration;

        bool			m_bLoop;
        bool			m_bStop;

        bool			m_bIsTransition;

        double			m_dAmplify;

    public:
        CAudioPart()
        {
            m_dStartTime	= 0.0;
            m_dEndTime		= -1.0;

            m_dClipStartTime	= 0.0;
            m_dClipEndTime	= -1.0;

            m_dAudioDuration	= 0.0;

            m_bLoop		= false;
            m_bStop		= false;

            m_bIsTransition	= false;

            m_dAmplify		= 100.0;
        }

        ~CAudioPart()
        {
        }

        CAudioPart(const CAudioPart& oSrc)
        {
            *this = oSrc;
        }

        CAudioPart& operator=(const CAudioPart& oSrc)
        {
            m_strFile			= oSrc.m_strFile;

            m_dStartTime		= oSrc.m_dStartTime;
            m_dEndTime			= oSrc.m_dEndTime;

            m_dClipStartTime	= oSrc.m_dClipStartTime;
            m_dClipEndTime		= oSrc.m_dClipEndTime;

            m_dAudioDuration	= oSrc.m_dAudioDuration;

            m_bLoop				= oSrc.m_bLoop;
            m_bStop				= oSrc.m_bStop;

            m_bIsTransition		= oSrc.m_bIsTransition;

            m_dAmplify			= oSrc.m_dAmplify;

            return *this;
        }

        CAudioPart(CAudioElement* pAudioElem)
        {
            if (NULL == pAudioElem)
            {
                m_strFile			= _T("");

                m_dStartTime		= 0.0;
                m_dEndTime			= -1.0;

                m_dClipStartTime	= 0.0;
                m_dClipEndTime		= -1.0;

                m_dAudioDuration	= 0.0;

                m_bLoop				= false;
                m_bStop				= false;

                m_bIsTransition		= false;

                m_dAmplify			= 100.0;
            }
            else
            {
                m_strFile			= pAudioElem->m_strAudioFileName;

                m_dStartTime		= pAudioElem->m_dStartTime;
                m_dEndTime			= pAudioElem->m_dEndTime;

                m_dClipStartTime	= pAudioElem->m_dClipStartTime;
                m_dClipEndTime		= pAudioElem->m_dClipEndTime;

                m_dAudioDuration	= pAudioElem->m_dAudioDuration;

                m_bLoop				= ( pAudioElem->m_bLoop == TRUE) ? true : false;;
                m_bStop				= false;

                m_bIsTransition		= false;

                m_dAmplify			= 100.0;
            }
        }

    public:
        void CalculateDuration()
        {
            if (0.0 < m_dAudioDuration || _T("") == m_strFile)
                return;
        }
    };

    class CAudioOverlay
    {
    public:
        std::vector<CAudioPart> m_arParts;
        double m_dAllDuration;

    public:
        CAudioOverlay() : m_arParts(), m_dAllDuration(0.0)
        {
        }
        ~CAudioOverlay()
        {
        }
        CAudioOverlay(const CAudioOverlay& oSrc)
        {
            *this = oSrc;
        }
        CAudioOverlay& operator=(const CAudioOverlay& oSrc)
        {
            m_arParts.insert(m_arParts.end(), oSrc.m_arParts.begin(), oSrc.m_arParts.end());
            m_dAllDuration = oSrc.m_dAllDuration;
            return *this;
        }

    public:

        void Calculate()
        {
            size_t nCount = m_arParts.size();

            // нормализуем для начала
            for (size_t i = 0; i < nCount; ++i)
            {
                CAudioPart* pPart = &m_arParts[i];

                pPart->CalculateDuration();

                if (pPart->m_dStartTime < 0.0)
                    pPart->m_dStartTime = 0.0;

                pPart->m_dEndTime = pPart->m_dStartTime + pPart->m_dAudioDuration;
                if (pPart->m_dEndTime > m_dAllDuration)
                {
                    pPart->m_dEndTime = m_dAllDuration;
                }
                if (pPart->m_bLoop)
                {
                    pPart->m_dEndTime = m_dAllDuration;
                }
            }

            // пересчет
            for (size_t i = 0; i < nCount; ++i)
            {
                CAudioPart* pPart = &m_arParts[i];

                if (pPart->m_bIsTransition)
                {
                    if (pPart->m_bStop)
                    {
                        // нужно остановить всю музыку до этого
                        for (size_t j = 0; j < nCount; ++j)
                        {
                            if (j == i)
                                continue;

                            CAudioPart* pMem = &m_arParts[j];
                            if (pMem->m_dStartTime <= pPart->m_dStartTime && pMem->m_dEndTime > pPart->m_dStartTime)
                            {
                                pMem->m_dEndTime = pPart->m_dStartTime;
                            }
                        }
                    }
                    if (pPart->m_bLoop)
                    {
                        // зацикливаем до первого встречания аудио

                        double dMin = m_dAllDuration;
                        for (size_t j = 0; j < nCount; ++j)
                        {
                            if (j == i)
                                continue;

                            CAudioPart* pMem = &m_arParts[j];
                            if (pMem->m_dStartTime > pPart->m_dEndTime)
                            {
                                if (dMin > pMem->m_dStartTime)
                                    dMin = pMem->m_dStartTime;
                            }
                        }

                        pPart->m_dEndTime = dMin;
                    }
                }
                else
                {
                    if (pPart->m_bLoop)
                    {
                        pPart->m_dEndTime = m_dAllDuration;
                    }
                }
            }
        }

        std::wstring GetAudioOverlay()
        {
            std::wstring strRes = _T("");

            size_t nCount = m_arParts.size();

            for (size_t i = 0; i < nCount; ++i)
            {
                CAudioPart* pPart = &m_arParts[i];

                std::wstring strOverlay1 = L"<AudioSource StartTime='" + std::to_wstring(pPart->m_dStartTime)
                        + L"' Duration='" + std::to_wstring(pPart->m_dEndTime - pPart->m_dStartTime)
                        + L"' Amplify='" + std::to_wstring(pPart->m_dAmplify) + L"'>";

                int lIndex = pPart->m_strFile.find(L"file:///");
                if (0 == lIndex)
                {
                    pPart->m_strFile = pPart->m_strFile.substr(8);
                    //pPart->m_strFile.Replace('/', '\\');
                    //pPart->m_strFile.Replace(L"%20", L" ");
                }

                std::wstring strFile_ = pPart->m_strFile;
                CorrectXmlString(strFile_);

                std::wstring strOverlay2 = L"<Source StartTime='" + std::to_wstring(pPart->m_dClipStartTime)
                        + L"' EndTime='" + std::to_wstring(pPart->m_dClipEndTime)
                        + L"' FilePath='" + strFile_+ L"'/></AudioSource>";

                strRes += strOverlay1 + strOverlay2;
            }

            return strRes;
        }
    };
}

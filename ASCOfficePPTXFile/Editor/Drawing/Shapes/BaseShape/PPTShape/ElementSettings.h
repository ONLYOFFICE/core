/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../../../../../ASCOfficePPTFile/PPTFormatLib/Records/Drawing/ArtBlip.h"

#include "../../../../../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "Enums.h"

// это класс, использующийся для передачи свойств объектов,
// например - указатель на картинку... (по PID'у)

class CProperty
{
public:
    NSOfficeDrawing::PropertyId		m_ePID;
    bool							m_bIsBlip;
    bool							m_bComplex;
    DWORD							m_lValue;
    BYTE*							m_pOptions;
    // чтобы не плодить классы - это value, когда m_bComplex == true
    bool							m_bIsTruncated;

public:
    CProperty()
    {
        m_ePID = NSOfficeDrawing::left;
        m_bIsBlip = false;
        m_bComplex = false;
        m_lValue = 0;
        m_pOptions = NULL;

        m_bIsTruncated = false;
    }
    ~CProperty()
    {
        RELEASEARRAYOBJECTS(m_pOptions);
    }
    void FromStream(POLE::Stream* pStream)
    {
        // читаем из стрима...
        // только пока без учета bComplex
        // т.к. Complex - учитывается в контейнере, хранящем все проперти
        USHORT lMem = StreamUtils::ReadWORD(pStream);
        m_ePID = (NSOfficeDrawing::PropertyId)(lMem & 0x3FFF);

        m_bIsBlip = ((lMem & 0x4000) == 0x4000);
        m_bComplex = ((lMem & 0x8000) == 0x8000);

        m_lValue = StreamUtils::ReadDWORD(pStream);
    }

    void ComplexFromStream(POLE::Stream* pStream)
    {
        if (m_bComplex && m_lValue > 0)
        {
            if (NSOfficeDrawing::dgmConstrainBounds		== m_ePID ||
                    NSOfficeDrawing::fillShadeColors		== m_ePID ||
                    NSOfficeDrawing::lineDashStyle			== m_ePID ||
                    NSOfficeDrawing::pAdjustHandles			== m_ePID ||
                    NSOfficeDrawing::pConnectionSites		== m_ePID ||
                    NSOfficeDrawing::pConnectionSitesDir	== m_ePID ||
                    NSOfficeDrawing::pInscribe				== m_ePID ||
                    NSOfficeDrawing::pSegmentInfo			== m_ePID ||
                    NSOfficeDrawing::pVertices				== m_ePID ||
                    NSOfficeDrawing::pGuides				== m_ePID ||
                    NSOfficeDrawing::pWrapPolygonVertices	== m_ePID ||
                    NSOfficeDrawing::pRelationTbl			== m_ePID ||
                    NSOfficeDrawing::tableRowProperties		== m_ePID ||
                    NSOfficeDrawing::lineLeftDashStyle		== m_ePID ||
                    NSOfficeDrawing::lineTopDashStyle		== m_ePID ||
                    NSOfficeDrawing::lineRightDashStyle		== m_ePID ||
                    NSOfficeDrawing::lineBottomDashStyle	== m_ePID)
            {
                WORD nElems			= StreamUtils::ReadWORD(pStream);
                WORD nElemsAlloc	= StreamUtils::ReadWORD(pStream);
                WORD nElemSize		= StreamUtils::ReadWORD(pStream);

                if (0xFFF0 == nElemSize)
                {
                    nElemSize = 4;
                    m_bIsTruncated = TRUE;
                }

                LONG dwSize = nElems * nElemSize;

                if (m_lValue != (dwSize + 6))
                {
                    bool b = false;
                }

                m_lValue = dwSize;
            }

            switch( m_ePID )
            {
            case NSOfficeDrawing::fillBlip:
            {
                SRecordHeader oHeader;
                if (oHeader.ReadFromStream(pStream) == false )
                {
                    return;
                }
                switch (oHeader.RecType)
                {
                case RECORD_TYPE_ESCHER_BLIP_EMF:
                case RECORD_TYPE_ESCHER_BLIP_WMF:
                case RECORD_TYPE_ESCHER_BLIP_PICT:
                case RECORD_TYPE_ESCHER_BLIP_JPEG:
                case RECORD_TYPE_ESCHER_BLIP_PNG:
                case RECORD_TYPE_ESCHER_BLIP_DIB:
                case RECORD_TYPE_ESCHER_BLIP_TIFF:
                {
                    CRecordOfficeArtBlip art_blip;
                    art_blip.ReadFromStream(oHeader, pStream);
                }
                }

            }break;
            default:
            {
                if (0 == m_lValue)
                {
                    return;
                }

                RELEASEARRAYOBJECTS(m_pOptions);
                m_pOptions = new BYTE[m_lValue];

                ULONG lReadBytes = (ULONG)pStream->read(m_pOptions, m_lValue);
                if (lReadBytes != m_lValue)
                {
                    return;
                }
            }
            }
        }
    }
};

// А вот контейнер пропертей
class CProperties
{
public:
    std::vector<CProperty> m_arProperties;
    // по идее - это instance, но нам так удобнее,
    // тем более это класс - не связанный с RecordHeader
    long m_lCount;

public:
    CProperties() : m_arProperties()
    {
    }
    ~CProperties()
    {
        m_lCount = 0;
        m_arProperties.clear();
    }

    void FromStream(POLE::Stream* pStream, long lCount)
    {
        m_lCount = lCount;
        for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
        {
            CProperty elem;
            m_arProperties.push_back(elem);
            m_arProperties[lIndex].FromStream(pStream);
        }
        // теперь читаем дополнительную информацию
        // сортировано по pid'ам (но у нас пока просто по-порядку)
        for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
        {
            m_arProperties[lIndex].ComplexFromStream(pStream);
        }
    }

    std::wstring ToString()
    {
        std::wstring str;
//        for (size_t nIndex = 0; nIndex < m_arProperties.size(); ++nIndex)
//        {
//            str += m_arProperties[nIndex].ToString();
//        }
        return _T("<Properties>") + str + _T("</Properties>");
    }

    DWORD GetLen()
    {
        DWORD dwLen = 6 * m_lCount;
        for (long nIndex = 0; nIndex < m_lCount; ++nIndex)
        {
            if (m_arProperties[nIndex].m_bComplex)
            {
                dwLen += m_arProperties[nIndex].m_lValue;
            }
        }
        return dwLen;
    }
};

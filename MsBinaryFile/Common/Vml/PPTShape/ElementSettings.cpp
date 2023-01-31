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

#include "ElementSettings.h"
#include "../../../../OOXML/SystemUtility/File.h"

#include "../../../PptFile/Reader/Records.h"
#include "../../../PptFile/Reader/PPTFileDefines.h"

// это класс, использующийся для передачи свойств объектов,
// например - указатель на картинку... (по PID'у)

void CProperty::FromStream(POLE::Stream* pStream)
{
	// читаем из стрима...
	// только пока без учета bComplex
	// т.к. Complex - учитывается в контейнере, хранящем все проперти
	USHORT lMem = StreamUtils::ReadWORD(pStream);
	m_ePID = (ODRAW::ePropertyId)(lMem & 0x3FFF);

	m_bIsBlip = ((lMem & 0x4000) == 0x4000);
	m_bComplex = ((lMem & 0x8000) == 0x8000);

	m_lValue = StreamUtils::ReadDWORD(pStream);
}

void CProperty::ComplexFromStream(POLE::Stream* pStream)
{
	if (m_bComplex && m_lValue > 0)
	{
		if (ODRAW::dgmConstrainBounds == m_ePID ||
			ODRAW::fillShadeColors == m_ePID ||
			ODRAW::lineDashStyle == m_ePID ||
			ODRAW::pAdjustHandles == m_ePID ||
			ODRAW::pConnectionSites == m_ePID ||
			ODRAW::pConnectionSitesDir == m_ePID ||
			ODRAW::pInscribe == m_ePID ||
			ODRAW::pSegmentInfo == m_ePID ||
			ODRAW::pVertices == m_ePID ||
			ODRAW::pGuides == m_ePID ||
			ODRAW::pWrapPolygonVertices == m_ePID ||
			ODRAW::pRelationTbl == m_ePID ||
			ODRAW::tableRowProperties == m_ePID ||
			ODRAW::lineLeftDashStyle == m_ePID ||
			ODRAW::lineTopDashStyle == m_ePID ||
			ODRAW::lineRightDashStyle == m_ePID ||
			ODRAW::lineBottomDashStyle == m_ePID)
		{
			WORD nElems = StreamUtils::ReadWORD(pStream);
			WORD nElemsAlloc = StreamUtils::ReadWORD(pStream);
			WORD nElemSize = StreamUtils::ReadWORD(pStream);

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

		switch (m_ePID)
		{
		case ODRAW::fillBlip:
		{
            PPT::SRecordHeader oHeader;
			if (oHeader.ReadFromStream(pStream) == false)
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
                PPT::CRecordOfficeArtBlip art_blip;
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


void CProperties::FromStream(POLE::Stream* pStream, long lCount)
{
	m_lCount = lCount;
	for (size_t lIndex = 0; lIndex < m_lCount; ++lIndex)
	{
		CProperty elem;
		m_arProperties.push_back(elem);
		m_arProperties[lIndex].FromStream(pStream);
	}
	// теперь читаем дополнительную информацию
	// сортировано по pid'ам (но у нас пока просто по-порядку)
	for (size_t lIndex = 0; lIndex < m_lCount; ++lIndex)
	{
		m_arProperties[lIndex].ComplexFromStream(pStream);
	}
}


size_t CProperties::GetLen()
{
	size_t dwLen = 6 * m_lCount;
	for (size_t nIndex = 0; nIndex < m_lCount; ++nIndex)
	{
		if (m_arProperties[nIndex].m_bComplex)
		{
			dwLen += m_arProperties[nIndex].m_lValue;
		}
	}
	return dwLen;
}

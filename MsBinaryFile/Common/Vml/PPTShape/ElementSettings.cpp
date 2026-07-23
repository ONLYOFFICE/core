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
#pragma once

#include "ElementSettings.h"
#include "../../../../OOXML/SystemUtility/File.h"

#include "../../../PptFile/Reader/Records.h"
#include "../../../PptFile/Reader/PPTFileDefines.h"

#include "../../../PptFile/Records/Drawing/ArtBlip.h"

// this class is used for passing object properties,
// for example - pointer to an image... (by PID)

CProperty::CProperty()
{
	m_ePID = ODRAW::ePropertyId_left;
	m_bIsBlip = false;
	m_bComplex = false;
	m_lValue = 0;
	m_pOptions = NULL;

	m_bIsTruncated = false;
}
CProperty::~CProperty()
{
	if (m_pOptions)	delete []m_pOptions;
	m_pOptions = NULL;
}
void CProperty::FromStream(POLE::Stream* pStream)
{
	// reading from stream...
	// for now without considering bComplex
	// since Complex is handled in the container storing all properties
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

CProperties::CProperties() : m_arProperties()
{
}
CProperties::~CProperties()
{
	m_lCount = 0;
	m_arProperties.clear();
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
	// now reading additional information
	// sorted by pids (but for now we just go in order)
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

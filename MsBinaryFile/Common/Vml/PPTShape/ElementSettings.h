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

#include "../../../PptFile/Records/Drawing/ArtBlip.h"
#include "../../../../OOXML/Base/Types_32.h"
#include "../../../../Common/3dParty/pole/pole.h"
#include "Enums.h"

#include <vector>

// это класс, использующийся для передачи свойств объектов,
// например - указатель на картинку... (по PID'у)

class CProperty
{
public:
    ODRAW::ePropertyId				m_ePID;
    bool							m_bIsBlip;
    bool							m_bComplex;
    _UINT32							m_lValue;
    unsigned char*					m_pOptions;
    // чтобы не плодить классы - это value, когда m_bComplex == true
    bool							m_bIsTruncated;

    CProperty()
    {
        m_ePID = ODRAW::ePropertyId_left;
        m_bIsBlip = false;
        m_bComplex = false;
        m_lValue = 0;
        m_pOptions = NULL;

        m_bIsTruncated = false;
    }
    ~CProperty()
    {
        if (m_pOptions)	delete []m_pOptions;
		m_pOptions = NULL;
    }
	void FromStream(POLE::Stream* pStream);
	void ComplexFromStream(POLE::Stream* pStream);
};

// А вот контейнер пропертей
class CProperties
{
public:
    std::vector<CProperty> m_arProperties;
    // по идее - это instance, но нам так удобнее,
    // тем более это класс - не связанный с RecordHeader
    size_t m_lCount;

    CProperties() : m_arProperties()
    {
    }
    ~CProperties()
    {
        m_lCount = 0;
        m_arProperties.clear();
    }

	void FromStream(POLE::Stream* pStream, long lCount);
	size_t GetLen();
};

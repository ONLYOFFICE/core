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
#include "toVmlConvert.h"

void COOXToVMLGeometry::_CStringWriter::AddSize(size_t nSize)
{
	if (NULL == m_pData)
	{
        m_lSize = (std::max)((int)nSize, (int)1000);
		m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
		
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

		wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
		if (NULL != pRealloc)
		{
			// realloc succeeded
			m_pData		= pRealloc;
			m_pDataCur	= m_pData + m_lSizeCur;
		}
		else
		{
			wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
			memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

			free(m_pData);
			m_pData		= pMalloc;
			m_pDataCur	= m_pData + m_lSizeCur;
		}
	}
}
void COOXToVMLGeometry::_CStringWriter::AddIntNoCheck(int val)
{
	if (0 == val)
	{
		*m_pDataCur++ = (WCHAR)'0';
		++m_lSizeCur;
		return;
	}
	if (val < 0)
	{
		val = -val;
		*m_pDataCur++ = (WCHAR)'-';
		++m_lSizeCur;
	}

	int len = 0;
	int oval = val;
	while (oval > 0)
	{
		oval /= 10;
		++len;
	}

	oval = 1;
	while (val > 0)
	{
		m_pDataCur[len - oval] = (WCHAR)('0' + (val % 10));
		++oval;
		val /= 10;
	}

	m_pDataCur += len;
	m_lSizeCur += len;
}

COOXToVMLGeometry::COOXToVMLGeometry()
{
	m_bIsFillPart = false;
	m_bIsStrokePart = false;
	
	m_dScaleX = 1.0;
	m_dScaleY = 1.0;

	m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
	m_pSimpleGraphicsConverter->SetRenderer(this);

	m_lCountPathCommands = 0;
}
COOXToVMLGeometry::~COOXToVMLGeometry()
{
	RELEASEOBJECT(m_pSimpleGraphicsConverter);
}

HRESULT COOXToVMLGeometry::EndCommand(const DWORD& lType)
{ 
	m_lCurrentCommandType = -1;

	if (lType == c_nPathType)
	{
		m_oWriter.AddSize(20);
		if (!m_bIsFillPart)
		{
			m_oWriter.AddCharNoCheck((WCHAR)'n');
			m_oWriter.AddCharNoCheck((WCHAR)'f');
		}
		if (!m_bIsStrokePart)
		{
			m_oWriter.AddCharNoCheck((WCHAR)'n');
			m_oWriter.AddCharNoCheck((WCHAR)'s');
		}
		m_oWriter.AddCharNoCheck((WCHAR)'e');
		m_lCountPathCommands = 0;
	}

	return S_OK; 
}
HRESULT COOXToVMLGeometry::PathCommandMoveTo(const double& x, const double& y)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(x * m_dScaleX, y * m_dScaleY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(x, y);
	}
	return S_OK;		
}
HRESULT COOXToVMLGeometry::PathCommandLineTo(const double& x, const double& y)
{ 
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(x * m_dScaleX, y * m_dScaleY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(x, y);
	}

	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandLinesTo(double* points, const int& count)
{ 
	m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
{ 
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(x1 * m_dScaleX, y1 * m_dScaleY, x2 * m_dScaleX, y2 * m_dScaleY, x3 * m_dScaleX, y3 * m_dScaleY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
	}

	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandCurvesTo(double* points, const int& count)
{ 
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{ 
	m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandClose()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();
	}
	return S_OK;
}
HRESULT COOXToVMLGeometry::PathCommandGetCurrentPoint(double* x, double* y)
{
	if (NULL != m_pSimpleGraphicsConverter)
		return m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(x, y);
	return S_OK; 
}

HRESULT COOXToVMLGeometry::NewShape()
{
	m_pSimpleGraphicsConverter->PathCommandEnd();
	m_oWriter.ClearNoAttack();
	return S_OK;
}
HRESULT COOXToVMLGeometry:: ResultPath(std::wstring* bstrVal)
{
	*bstrVal = m_oWriter.GetData();
	return S_OK;
}



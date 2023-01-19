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
			// реаллок сработал
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



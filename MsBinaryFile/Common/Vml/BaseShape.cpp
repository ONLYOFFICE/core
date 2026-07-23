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

#include "BaseShape.h"

#include "PPTXShape/PptxShape.h"
#include "PPTShape/PptShape.h"

using namespace ODRAW;

CHandle_::CHandle_()
{
}
CHandle_& CHandle_::operator =(const CHandle_& oSrc)
{
	position		= oSrc.position;
	xrange			= oSrc.xrange;
	yrange			= oSrc.yrange;
	switchHandle	= oSrc.switchHandle;
	polar			= oSrc.polar;
	radiusrange		= oSrc.radiusrange;
	return (*this);
}

CBaseShape::CBaseShape()
{
	m_bCustomShape = false;
}
void CBaseShape::ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore)
{
	m_oPath.ToRenderer(pRenderer, pGeomInfo, dStartTime, dEndTime, pPen, pFore, GetClassType());
}
void CBaseShape::SetWidthHeightLogic(const double& dWidth, const double& dHeight)
{
	// none
}
void CBaseShape::GetWidthHeightLogic(double& dWidth, double& dHeight)
{
	// none
}
void CBaseShape::AddGuide(const std::wstring& strGuide)
{
}
CBaseShapePtr CBaseShape::CreateByType(NSBaseShape::ClassType ClassType, int ShapeType)
{
	if(ClassType == pptx)
	{
		return CPPTXShape::CreateByType((OOXMLShapes::ShapeType)ShapeType);
	}

	if (ClassType == ppt)
	{
		return CPPTShape::CreateByType((PPTShapes::ShapeType)ShapeType);
	}

	return CBaseShapePtr();
}
bool CBaseShape::SetType(NSBaseShape::ClassType ClassType, int ShapeType)
{
	if (ClassType != GetClassType())
		return false;

	if(ClassType == pptx)
	{
		return ((CPPTXShape*)this)->SetShapeType((OOXMLShapes::ShapeType)ShapeType);
	}
	if(ClassType == ppt)
	{
		return ((CPPTShape*)this)->SetShapeType((PPTShapes::ShapeType)ShapeType);
	}

	return false;
}
bool CBaseShape::SetProperties(CBaseShape* Shape)
{
	if( Shape == NULL)
		return false;

	m_oPath		= Shape->m_oPath;
	m_strPath	= Shape->m_strPath;
	m_strRect	= Shape->m_strRect;

	m_arAbsMaxAdjustments.clear();
	for(size_t i = 0; i < Shape->m_arAbsMaxAdjustments.size(); i++)
		m_arAbsMaxAdjustments.push_back(Shape->m_arAbsMaxAdjustments[i]);

	m_arAdjustments.clear();
	for(size_t i = 0; i < Shape->m_arAdjustments.size(); i++)
		m_arAdjustments.push_back(Shape->m_arAdjustments[i]);

	m_arGuides.clear();
	for(size_t i = 0; i < Shape->m_arGuides.size(); i++)
		m_arGuides.push_back(Shape->m_arGuides[i]);

	m_eJoin				= Shape->m_eJoin;
	m_bConcentricFill	= Shape->m_bConcentricFill;

	m_arConnectors.clear();
	for(size_t i = 0; i < Shape->m_arConnectors.size(); i++)
		m_arConnectors.push_back(Shape->m_arConnectors[i]);

	m_arConnectorAngles.clear();
	for(size_t i = 0; i < Shape->m_arConnectorAngles.size(); i++)
		m_arConnectorAngles.push_back(Shape->m_arConnectorAngles[i]);

	m_arTextRects.clear();
	for(size_t i = 0; i < Shape->m_arTextRects.size(); i++)
		m_arTextRects.push_back(Shape->m_arTextRects[i]);

	m_arHandles.clear();
	for(size_t i = 0; i < Shape->m_arHandles.size(); i++)
		m_arHandles.push_back(Shape->m_arHandles[i]);


	m_strRect = Shape->m_strRect;
	m_strTransformXml = Shape->m_strTransformXml;
	return true;
}
bool CBaseShape::SetToDublicate(CBaseShape* Shape)
{
	if( Shape == NULL)
		return false;

	Shape->m_oPath		= m_oPath;
	Shape->m_strPath	= m_strPath;
	Shape->m_strRect	= m_strRect;

	Shape->m_arAbsMaxAdjustments.clear();
	for(size_t i = 0; i < m_arAbsMaxAdjustments.size(); i++)
		Shape->m_arAbsMaxAdjustments.push_back(m_arAbsMaxAdjustments[i]);

	Shape->m_arAdjustments.clear();
	for(size_t i = 0; i < m_arAdjustments.size(); i++)
		Shape->m_arAdjustments.push_back(m_arAdjustments[i]);

	Shape->m_arGuides.clear();
	for(size_t i = 0; i < m_arGuides.size(); i++)
		Shape->m_arGuides.push_back(m_arGuides[i]);

	Shape->m_eJoin				= m_eJoin;
	Shape->m_bConcentricFill	= m_bConcentricFill;

	Shape->m_arConnectors.clear();
	for(size_t i = 0; i < m_arConnectors.size(); i++)
		Shape->m_arConnectors.push_back(m_arConnectors[i]);

	Shape->m_arConnectorAngles.clear();
	for(size_t i = 0; i < m_arConnectorAngles.size(); i++)
		Shape->m_arConnectorAngles.push_back(m_arConnectorAngles[i]);

	Shape->m_arTextRects.clear();
	for(size_t i = 0; i < m_arTextRects.size(); i++)
		Shape->m_arTextRects.push_back(m_arTextRects[i]);
	Shape->m_strRect = m_strRect;

	Shape->m_arHandles.clear();
	for(size_t i = 0; i < m_arHandles.size(); i++)
		Shape->m_arHandles.push_back(m_arHandles[i]);

	Shape->m_strTransformXml = m_strTransformXml;
	return true;
}

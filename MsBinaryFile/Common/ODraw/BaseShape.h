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
#include "PathShape.h"
#include "../../../OOXML/Common/SimpleTypes_Base.h"

namespace NSCustomShapesConvert
{

class CHandle_
{
public:
	std::wstring position;
	std::wstring xrange;
	std::wstring yrange;
	std::wstring switchHandle;
	std::wstring polar;
	std::wstring radiusrange; 

public:
	CHandle_()
	{
		position = _T("");
		xrange = _T("");
		yrange = _T("");
		switchHandle = _T("");
		polar = _T("");
		radiusrange = _T(""); 
	}
	CHandle_& operator =(const CHandle_& oSrc)
	{
		position		= oSrc.position;
		xrange			= oSrc.xrange;
		yrange			= oSrc.yrange;
		switchHandle	= oSrc.switchHandle;
		polar			= oSrc.polar;
		radiusrange		= oSrc.radiusrange;
		return (*this);
	}
};

class CBaseShape
{
public:
	std::vector<long>					m_arAdjustments;
	std::vector<double>					m_arGuides;

	LONG								m_eJoin;
	bool								m_bConcentricFill;

    std::vector<SimpleTypes::CPoint>	m_arConnectors;
	std::vector<LONG>					m_arConnectorAngles;

    std::vector<Aggplus::RECT>			m_arTextRects;
	
	std::vector<CHandle_>				m_arHandles;

	std::wstring						m_strTransformXml;

	std::wstring						m_strPath;
	std::wstring						m_strRect;

	LONG								m_lLimoX;
	LONG								m_lLimoY;
	
	CPath								m_oPath;
public:
	CBaseShape()
	{
	}

	virtual bool LoadTextRect(const std::wstring& xml)			= 0;
	virtual bool LoadPathList(const std::wstring& xml)			= 0;

	static CBaseShape* CreateByType(int ShapeType);
	
	bool SetType(int ShapeType);

	virtual bool SetProperties(CBaseShape* Shape)
	{
		if( Shape == NULL)
			return false;

		m_oPath		= Shape->m_oPath;
		m_strPath	= Shape->m_strPath;
		m_strRect	= Shape->m_strRect;
		
		m_arAdjustments.clear();
		for(size_t  i = 0; i < Shape->m_arAdjustments.size(); i++)
			m_arAdjustments.push_back(Shape->m_arAdjustments[i]);

		m_arGuides.clear();
		for(size_t  i = 0; i < Shape->m_arGuides.size(); i++)
			m_arGuides.push_back(Shape->m_arGuides[i]);

		m_eJoin				= Shape->m_eJoin;
		m_bConcentricFill	= Shape->m_bConcentricFill;

		m_arConnectors.clear();
		for(size_t  i = 0; i < Shape->m_arConnectors.size(); i++)
			m_arConnectors.push_back(Shape->m_arConnectors[i]);

		m_arConnectorAngles.clear();
		for(size_t  i = 0; i < Shape->m_arConnectorAngles.size(); i++)
			m_arConnectorAngles.push_back(Shape->m_arConnectorAngles[i]);

		m_arTextRects.clear();
		for(size_t  i = 0; i < Shape->m_arTextRects.size(); i++)
			m_arTextRects.push_back(Shape->m_arTextRects[i]);

		m_arHandles.clear();
		for(size_t  i = 0; i < Shape->m_arHandles.size(); i++)
			m_arHandles.push_back(Shape->m_arHandles[i]);


		m_strRect = Shape->m_strRect;
		m_strTransformXml = Shape->m_strTransformXml;
		return true;
	}

	//virtual bool SetToDublicate(CBaseShape* Shape)
	//{
	//	if( Shape == NULL)
	//		return false;

	//	Shape->m_oPath		= m_oPath;
	//	Shape->m_strPath	= m_strPath;
	//	Shape->m_strRect	= m_strRect;
	//	
	//	Shape->m_arAdjustments.clear();
	//	for(size_t  i = 0; i < m_arAdjustments.size(); i++)
	//		Shape->m_arAdjustments.push_back(m_arAdjustments[i]);

	//	Shape->Guides.clear();
	//	for(size_t  i = 0; i < Guides.size(); i++)
	//		Shape->Guides.push_back(Guides[i]);

	//	Shape->m_eJoin				= m_eJoin;
	//	Shape->m_bConcentricFill	= m_bConcentricFill;

	//	Shape->m_arConnectors.clear();
	//	for(size_t  i = 0; i < m_arConnectors.size(); i++)
	//		Shape->m_arConnectors.push_back(m_arConnectors[i]);

	//	Shape->m_arConnectorAngles.clear();
	//	for(size_t  i = 0; i < m_arConnectorAngles.size(); i++)
	//		Shape->m_arConnectorAngles.push_back(m_arConnectorAngles[i]);

	//	Shape->m_arTextRects.clear();
	//	for(size_t  i = 0; i < m_arTextRects.size(); i++)
	//		Shape->m_arTextRects.push_back(m_arTextRects[i]);
	//	Shape->m_strRect = m_strRect;

	//	Shape->m_arHandles.clear();
	//	for(int i = 0; i < m_arHandles.size(); i++)
	//		Shape->m_arHandles.push_back(m_arHandles[i]);

	//	Shape->m_strTransformXml = m_strTransformXml;
	//	return true;
	//}
};
}

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

#include "BaseShape.h"
#include "CustomGeomShape.h"
#include "../../XlsFile/Converter/ShapeType.h"
#include "../../Common/Vml/PPTShape/Enums.h"

namespace NSCustomShapesConvert
{

class CCustomShape : public NSCustomShapesConvert::CBaseShape
{
public:
	oox::MSOSPT								m_eType;
	NSCustomShapesConvert::CFormulasManager	m_oManager;

	NSCustomShapesConvert::CCustomVML		m_oCustomVML;
	
	std::wstring							m_strPathLimoX;
	std::wstring							m_strPathLimoY;

	std::vector<std::wstring> m_arStringTextRects;

	CCustomShape() : CBaseShape()
	{
		m_eType = oox::msosptNotPrimitive;
	}

	~CCustomShape()
	{
	}

	virtual void AddGuide(const std::wstring& strGuide)
	{
		m_oManager.AddFormula(strGuide);
	}

	virtual bool LoadAdjustHandlesList(const std::wstring& xml)
	{
		return true;
	}

	virtual bool LoadConnectorsList(const std::wstring& xml)
	{
		return true;
	}

	virtual bool LoadTextRect(const std::wstring& xml)
	{
		std::vector<std::wstring> oArray;
		NSStringUtils::ParseString(_T(";"), xml, oArray);

		LONG lCount = (LONG)oArray.size();

		if (lCount <= 0)
			return true;

		m_arStringTextRects.clear();
		for (LONG i = 0; i < lCount; ++i)
		{
			m_arStringTextRects.push_back(oArray[i]);
		}
		
		return true;
	}

	virtual bool LoadPathList(const std::wstring& xml)
	{
		m_strPath = xml;
	
		m_oPath.FromXML(xml, m_oManager);
		return true;
	}

	virtual bool SetAdjustment(long index, long value)
	{
		if (index < (long)m_arAdjustments.size() && index >= 0)
		{
			m_arAdjustments[index] =  value;
			return TRUE;
		}
		return FALSE;
	}

	virtual void ReCalculate()
	{
		m_oManager.Clear(&m_arAdjustments);
		
		m_oManager.CalculateResults();

		if (_T("") == m_strPath)
			return;

		LoadPathList(m_strPath);
	}

	static CCustomShape* CreateByType(oox::MSOSPT type);

	virtual bool SetProperties(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		m_oManager = ((CCustomShape*)Shape)->m_oManager;

		//m_strPathLimoX = ((CCustomShape*)Shape)->m_strPathLimoX;
		//m_strPathLimoY = ((CCustomShape*)Shape)->m_strPathLimoY;

		//m_arStringTextRects.clear();
		//m_arStringTextRects.insert(m_arStringTextRects.end(), ((CCustomShape*)Shape)->m_arStringTextRects.begin(), ((CCustomShape*)Shape)->m_arStringTextRects.end());
		
		return CBaseShape::SetProperties(Shape);
	}

	bool SetShapeType(oox::MSOSPT type)
	{
		CCustomShape* l_pShape = CreateByType(type);
		if(l_pShape != NULL)
		{
			m_eType = type;

			SetProperties(l_pShape);
			delete l_pShape;
			return true;
		}

		m_eType = oox::msosptNotPrimitive;
		return false;
	}
};
}

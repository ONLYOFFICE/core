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
#include "../BaseShape.h"
#include "CustomGeomShape.h"
#include "PptShapeEnum.h"
#include "../../../../OOXML/DocxFormat/Logic/VmlOfficeDrawing.h"

using namespace ODRAW;

static double FixedPointToDouble(unsigned int op)
{
	short			Integral	= op >> 16;
	unsigned short Fractional	= op - (Integral << 16);

	return Integral + (Fractional / 65536.0);
}

namespace DocFileFormat
{
	class ShapeType;
}

namespace ODRAW
{
	struct _textPath 
	{
		bool bEnabled = false;
		boost::optional<bool> bFitPath;
		boost::optional<bool> bFitShape;
		boost::optional<bool> bTrim;
		boost::optional<std::wstring> sStyle;
	};
	struct _3dOptions
	{
		_3dOptions();

		bool bEnabled;
		bool bMetallic;
		bool bExtrusionColor;
		bool bLightFace;

		bool bConstrainRotation;
		bool bRotationCenterAuto;
		bool bParallel;
		bool bKeyHarsh;
		bool bFillHarsh;

		boost::optional<double> dSpecularAmt;
		boost::optional<double> dDiffuseAmt;
		boost::optional<double> dShininess;
		_INT32 nEdgeThickness;
		_INT32 nExtrudeForward;
		_INT32 nExtrudeBackward;

		boost::optional<double> dBottomBevelWidth;
		boost::optional<double> dBottomBevelHeight;
		boost::optional<_INT32> nBottomBevelType;

		boost::optional<double> dTopBevelWidth;
		boost::optional<double> dTopBevelHeight;
		boost::optional<_INT32> nTopBevelType;

		boost::optional<double> dXRotationAngle;
		boost::optional<double> dYRotationAngle;
		boost::optional<double> dRotationAxisX;
		boost::optional<double> dRotationAxisY;
		boost::optional<double> dRotationAxisZ;
		boost::optional<double> dRotationAngle;
		boost::optional<double> dRotationCenterX;
		boost::optional<double> dRotationCenterY;
		boost::optional<double> dRotationCenterZ;

		boost::optional<double> dTolerance;
		double dXViewpoint;
		double dYViewpoint;
		boost::optional<double> dZViewpoint;
		
		boost::optional<CColor> oExtrusionColor;
		boost::optional<CColor> oCrMod;
		boost::optional<CColor> oExtrusionColorExt;
		boost::optional<_INT32> nTypeExtrusionColorExt;

		_INT32 nRenderMode;		

		double dOriginX;
		double dOriginY;
		double dSkewAngle;
		_INT32 nSkewAmount;

		boost::optional<double> dAmbientIntensity;
		boost::optional<double> dKeyIntensity;		
		double dKeyX;
		double dKeyY;
		double dFillIntensity;
		double dFillX;
		double dFillY;
		boost::optional<double> dKeyZ;
		boost::optional<double> dFillZ;
	};
}

class CPPTShape : public CBaseShape
{
public:
	PPTShapes::ShapeType			m_eType;
	NSGuidesVML::CFormulasManager	m_oManager;
	NSCustomVML::CCustomVML			m_oCustomVML;

	ODRAW::_3dOptions				m_3dOptions;
	ODRAW::_textPath				m_textPath;

	std::wstring m_strPathLimoX;
	std::wstring m_strPathLimoY;

	std::vector<std::wstring> m_arStringTextRects;
	bool m_bIsShapeType;

	bool m_bIsFilled;
	bool m_bIsStroked;

	nullable<OOX::VmlOffice::CSignatureLine> m_oSignatureLine;

	CPPTShape();
	virtual ~CPPTShape();

	virtual bool LoadFromXML(const std::wstring& xml);
	virtual bool LoadFromXML(XmlUtils::CXmlNode& oNodePict);
	virtual bool LoadFromXMLShapeType(XmlUtils::CXmlNode& oNodeShapeType);
	virtual bool LoadAdjustValuesList(const std::wstring& xml);

	virtual bool LoadGuidesList(const std::wstring& xml);
	virtual void AddGuide(const std::wstring& strGuide);
	virtual bool LoadAdjustHandlesList(const std::wstring& xml);
	void LoadAHList(XmlUtils::CXmlNode& oNode);

	virtual bool LoadConnectorsList(const std::wstring& xml);
	virtual bool LoadTextRect(const std::wstring& xml);
	virtual bool LoadPathList(const std::wstring& xml);
	virtual bool SetAdjustment(long index, long value);

	virtual std::wstring ToXML(CGeomShapeInfo& GeomInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen);
	virtual void ReCalculate();

	static CPPTShape* CreateByShapeType(DocFileFormat::ShapeType* pShapeType);
	static CBaseShapePtr CreateByType(PPTShapes::ShapeType type);
	virtual const ClassType GetClassType() const;

	virtual bool SetProperties(CBaseShape* Shape);
	virtual bool SetToDublicate(CBaseShape* Shape);

	bool SetShapeType(PPTShapes::ShapeType type);
	void CalcTextRectOffsets(double& dPercentLeft, double& dPercentTop, double& dPercentRight, double& dPercentBottom, LONG nIndex = 0);
	bool IsWordArt();

protected:
	bool GetPos(std::wstring str, LONG& lValue);
};

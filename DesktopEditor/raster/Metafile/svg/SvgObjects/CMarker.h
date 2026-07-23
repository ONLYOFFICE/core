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

#ifndef CMARKER_H
#define CMARKER_H

#include "CContainer.h"

namespace SVG
{
	enum class EMarkerUnits
	{
		StrokeWidth,
		UserSpaceOnUse
	};

	enum class EMarkerOrient
	{
		Auto,
		Auto_start_reverse,
		Angle
	};

	struct TPointData
	{
		Point  m_oPoint;
		double m_dAngle;

		TPointData()
		    : m_dAngle(0.)
		{}
	};

	struct TMarkerExternData
	{
		std::vector<TPointData>* m_pPoints;
		double m_dStroke;

		TMarkerExternData()
		    : m_pPoints(NULL), m_dStroke(0.)
		{}

		~TMarkerExternData()
		{
			RELEASEOBJECT(m_pPoints);
		}
	};

	class CMarker : public CObject, public CContainer<CRenderedObject>
	{
		friend class CObject;
		CMarker(CSvgReader& oReader);
	public:
		virtual ~CMarker() = default;

		ObjectType GetType() const override;

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		void Draw(IRenderer* pRenderer, const CSvgFile *pFile, const TMarkerExternData& oExternalData, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		bool NeedExternalAngle() const;
		EMarkerOrient GetOrientType() const;
	private:
		EMarkerUnits  m_enUnits;
		EMarkerOrient m_enOrient;

		double       m_dAngle;

		TRect        m_oWindow;
		TRect        m_oViewBox;

		TBounds      m_oBounds;

	};
}

#endif // CMARKER_H

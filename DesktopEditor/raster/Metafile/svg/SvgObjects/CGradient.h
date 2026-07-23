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

#ifndef CGRADIENT_H
#define CGRADIENT_H

#include "CContainer.h"
#include "CObjectBase.h"

namespace SVG
{
	typedef enum
	{
		GradU_ObjectBoundingBox,
		GradU_UserSpaceOnUse
	}GradientUnits;

	typedef enum
	{
		Pad,
		Reflect,
		Repeat
	}SpreadMethod;

	class CStopElement : public CObject
	{
	public:
		CStopElement(CSvgReader& oReader);

		ObjectType GetType() const override;

		SvgDigit GetOffset() const;
		SvgColor GetColor()  const;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;
	private:
		SvgDigit m_oOffset;
		SvgColor m_oColor;
	};

	class CGradient : public CContainer<CStopElement>, public CAppliedObject
	{
	protected:
		CGradient(CSvgReader& oReader);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		void ReadChildrens(CSvgReader& oReader, CSvgFile* pSvgFile) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
		void ApplyTransform(IRenderer *pRenderer, const TBounds& oBounds, double& dAngle) const;
	protected:
		CGradient* GetRefGradient(const CSvgFile *pFile) const;
		bool ApplyRefGradient(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) const;

		std::wstring  m_wsXlinkHref;
		GradientUnits m_enGradientUnits;
		SpreadMethod  m_enSpreadMethod;
		SvgTransform  m_oTransform;
	};

	class CLinearGradient : public CGradient
	{
		friend class CObject;
		CLinearGradient(CSvgReader& oReader);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oX1;
		SvgDigit m_oY1;
		SvgDigit m_oX2;
		SvgDigit m_oY2;
	};

	class CRadialGradient : public CGradient
	{
		friend class CObject;
		CRadialGradient(CSvgReader& oReader);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	private:
		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CGRADIENT_H

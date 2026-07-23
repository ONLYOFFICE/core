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

#ifndef CTEXT_H
#define CTEXT_H

#include "../../../graphics/pro/Fonts.h"

#include "CPath.h"
#include "CContainer.h"
#include "CObjectBase.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CTSpan : public CRenderedObject, public CContainer<CTSpan>
	{
		friend class CObject;
	protected:
		CTSpan(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const Point& oPosition = {});
		CTSpan(const CTSpan& oTSpan, double dX, const std::wstring& wsText);
		CTSpan(wchar_t wChar, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		virtual ~CTSpan();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		void ReadChildrens(CSvgReader& oReader, CSvgFile* pSvgFile) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		bool AddObject(CTSpan* pObject) override;

		void InheritStyles(const CTSpan* pTSpan);
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY, Aggplus::CMatrix& oOldMatrix) const;

		void UpdateFontSize();
		bool UseExternalFont(const CSvgFile* pFile, double dX, double dY, IRenderer* pRenderer, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;

		double GetWidth() const;
		void CorrectFontFamily(std::wstring& wsFontFamily) const;

		void CalculatePosition(double& dX, double& dY) const;

		void NormalizeFontSize(double& dFontHeight, double& dScaleX, double& dScaleY) const;
		void SetPosition(const Point& oPosition);

		void SetPositionFromParent(CRenderedObject* pParent);

		double GetFontHeight() const;

		std::vector<CTSpan> Split() const;

		NSFonts::IFontManager* m_pFontManager;

		SvgDigit     m_oX;
		SvgDigit     m_oY;

		std::wstring m_wsText;

		SvgFont m_oFont;
		SvgText m_oText;

		friend class CText;
		friend class CTextPath;
	};

	class CText : public CTSpan
	{
		friend class CObject;
	protected:
		CText(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	};

	class CTextPath : public CText
	{
		friend class CObject;
		CTextPath(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		void DrawGlyph(CTSpan* pTSpan, CMovingPath& oMovingPath, IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode, const CRenderedObject* pContexObject = NULL) const;

		std::wstring m_wsHref;
	};
}

#endif // CTEXT_H

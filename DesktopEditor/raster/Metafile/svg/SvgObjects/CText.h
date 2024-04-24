/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	public:
		CTSpan(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);
		CTSpan(const std::wstring& wsText, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);
		virtual ~CTSpan();

		static CTSpan* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
		static CTSpan* Create(const std::wstring& wsValue, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;

		bool AddObject(CTSpan* pObject) override;

		void InheritStyles(const CTSpan* pTSpan);
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const;

		TBounds GetBounds() const override;

		double GetWidth() const;
		void CorrectFontFamily(std::wstring& wsFontFamily) const;

		void Normalize(IRenderer* pRenderer, double& dX, double& dY, double& dFontHeight) const;
		void SetPosition(const Point& oPosition);

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
	public:
		CText(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);

		static CText* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;
	};

	class CTextPath : public CText
	{
	public:
		CTextPath(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const CSvgFile* pFile = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;

		static CTextPath* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const CSvgFile* pFile = NULL);
	private:
		void DrawGlyph(CTSpan* pTSpan, CMovingPath& oMovingPath, IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode) const;

		const CPath        *m_pPath;
	};
}

#endif // CTEXT_H

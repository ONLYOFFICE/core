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
#ifndef _GRAPHICS_EXPORTS_GRAPHICS_H_
#define _GRAPHICS_EXPORTS_GRAPHICS_H_

#include "../../graphics/GraphicsPath.h"

#include "../../raster/BgraFrame.h"
#include "../../raster/ImageFileFormatChecker.h"

#include "./Image.h"

#include "../../graphics/AlphaMask.h"

#include "../IRenderer.h"
#include "../structures.h"

namespace Aggplus
{
	class CDIB : public IGrObject
	{
	public:
		BYTE* m_pBits;
		LONG m_lWidth;
		LONG m_lHeight;

	public:
		CDIB() : IGrObject()
		{
			m_pBits = NULL;
			m_lWidth = 0;
			m_lHeight = 0;
		}
		virtual ~CDIB()
		{
			// delete all in system wrapper
		}

		virtual INT Create(LONG lWidth, LONG lHeight, double dDPIX, double dDPIY) = 0;
	};
}
namespace NSGraphics
{
	class GRAPHICS_DECL IGraphicsRenderer : public IRenderer
	{
	public:
		IGraphicsRenderer() {}
		virtual ~IGraphicsRenderer() {}

	public:
		virtual void SetImageCache(NSImages::IImageFilesCache* pCache) = 0;
		virtual void SetFontManager(NSFonts::IFontManager* pManager = NULL) = 0;
		virtual void CheckFontManager() = 0;

		virtual NSFonts::IFontManager* GetFontManager() = 0;

		virtual void CloseFont() = 0;
		virtual void ClearInstallFont() = 0;

		virtual void SetSwapRGB(bool bValue) = 0;
		virtual void SetTileImageDpi(const double& dDpi) = 0;
		virtual void Save() = 0;
		virtual void Restore() = 0;

	public:
		virtual void CreateFromBgraFrame(CBgraFrame* pFrame) = 0;
		virtual void SetCoordTransformOffset(double dOffsetX, double dOffsetY) = 0;


		virtual void SavePen(NSStructures::CPen& oPen) = 0;
		virtual void RestorePen(const NSStructures::CPen& oPen) = 0;

		virtual void SaveBrush(NSStructures::CBrush& oBrush) = 0;
		virtual void RestoreBrush(const NSStructures::CBrush& oBrush) = 0;
		virtual void put_GlobalAlphaEnabled(const bool& bEnabled, const double& dVal) = 0;
		virtual void put_IntegerGrid(const bool& bEnabled) = 0;
		virtual bool get_IntegerGrid() = 0;
		virtual void AddRect(const double& x, const double& y, const double& w, const double& h) = 0;
		virtual void SetFontAttack() = 0;

		virtual void Create(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib = NULL) = 0;
		virtual void CreateFlip(BYTE* pPixels, const Aggplus::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, Aggplus::CDIB* pDib = NULL) = 0;

		virtual Aggplus::CMatrix* GetFullTransform() = 0;
		virtual Aggplus::CMatrix* GetTransformMatrix() = 0;
		virtual void CalculateFullTransform() = 0;
		virtual void PathCommandRect(double x, double y, double w, double h) = 0;
		virtual Aggplus::CMatrix* GetCoordTransform() = 0;
		virtual void Fill() = 0;
		virtual void Stroke() = 0;
		virtual double GetPixW() = 0;
		virtual double GetPixH() = 0;

		//alpha mask methods
		virtual void SetAlphaMask(Aggplus::CAlphaMask* pAlphaMask) = 0;
		virtual Aggplus::CSoftMask* CreateSoftMask(bool bAlpha) = 0;
		virtual void SetSoftMask(Aggplus::CSoftMask* pSoftMask) = 0;

		// smart methods
		virtual void drawHorLine(BYTE align, double y, double x, double r, double penW) = 0;
		virtual void drawHorLine2(BYTE align, double y, double x, double r, double penW) = 0;

		virtual void drawVerLine(BYTE align, double x, double y, double b, double penW) = 0;
		virtual void drawHorLineExt(BYTE align, double y, double x, double r, double penW, double leftMW, double rightMW) = 0;
	};

	GRAPHICS_DECL IGraphicsRenderer* Create();

	GRAPHICS_DECL std::string GetHatchBase64(const std::wstring& name,
											 unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
											 unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2);
}



#endif // _GRAPHICS_EXPORTS_GRAPHICS_H_

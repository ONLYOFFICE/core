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
#ifndef _METAFILE_COMMON_IOUTPUTDEVICE_H
#define _METAFILE_COMMON_IOUTPUTDEVICE_H

#include "MetaFileTypes.h"

namespace MetaFile
{
	class CPath;

	class IOutputDevice
	{
	public:

		IOutputDevice() {}
		virtual ~IOutputDevice() {}

		// Device start and end operations
		virtual void Begin() = 0;
		virtual void End() = 0;

		// pBuffer - BGRA image with dimensions ulWidth, ulHeight that needs to be drawn in the specified rect
		virtual void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, unsigned int nBlendMode) = 0;

		virtual void DrawString(std::wstring& wsText, unsigned int unCharsCount, double dX, double dY, double* pDx,
		                        int iGraphicsMode = 1, double dXScale = 1, double dYScale = 1, bool bUseGID = false) = 0;

		virtual void DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(double dX, double dY) = 0;
		virtual void LineTo(double dX, double dY) = 0;
		virtual void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe) = 0;
		virtual void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath(int nType = 0) = 0;
		virtual void EndPath() = 0;

		virtual void ResetClip() = 0;
		virtual void IntersectClip(const TRectD& oClip) = 0;
		virtual void ExcludeClip(const TRectD& oClip, const TRectD& oBB) = 0;
		virtual void PathClip(const CPath& oPath, int nClipMode, TXForm *pTransform = NULL) = 0;
		virtual void StartClipPath(unsigned int unMode, int nFillMode = -1) = 0;
		virtual void EndClipPath(unsigned int unMode) = 0;

		virtual void UpdateDC() = 0;
		virtual void SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY) = 0;
		virtual void GetTransform(double* pdM11, double* pdM12, double* pdM21, double* pdM22, double* pdX, double* pdY) = 0;
	};

}

#endif //_METAFILE_COMMON_IOUTPUTDEVICE_H

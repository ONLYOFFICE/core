/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _METAFILE_COMMON_IOUTPUTDEVICE_H
#define _METAFILE_COMMON_IOUTPUTDEVICE_H

#include "MetaFileTypes.h"

namespace MetaFile
{
	class IOutputDevice
	{
	public:

		IOutputDevice() {}
		virtual ~IOutputDevice() {}

		// Начало и окончание работы устройства
		virtual void Begin() = 0;
		virtual void End() = 0;

		// pBuffer - BGRA картинка размерами ulWidth, ulHeight, которую надо нарисовать в заданном ректе
		virtual void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) = 0;

		virtual void DrawString(std::wstring& wsText, unsigned int unCharsCount, double dX, double dY, double* pDx) = 0;

		virtual void StartPath() = 0;
		virtual void MoveTo(double dX, double dY) = 0;
		virtual void LineTo(double dX, double dY) = 0;
		virtual void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe) = 0;
		virtual void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle) = 0;
		virtual void ClosePath() = 0;
		virtual void DrawPath(int nType = 0) = 0;
		virtual void EndPath() = 0;

		virtual void ResetClip() = 0;
		virtual void IntersectClip(double dLeft, double dTop, double dRight, double dBottom) = 0;
		virtual void StartClipPath(unsigned int unMode, int nFillMode = -1) = 0;
		virtual void EndClipPath(unsigned int unMode) = 0;

		virtual void UpdateDC() = 0;
	};

}

#endif //_METAFILE_COMMON_IOUTPUTDEVICE_H

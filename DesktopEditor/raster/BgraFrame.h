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
#ifndef _BUILD_BGRA_FRAME_
#define _BUILD_BGRA_FRAME_

#include <string>
#include "../common/Types.h"
#include "./../graphics/config.h"
#include "./../common/IGrObject.h"
#include "./../graphics/Image.h"

class GRAPHICS_DECL CBgraFrame
{
private:
	int m_nFileType;

	int m_lWidth;
	int m_lHeight;
	int m_lStride;

	BYTE* m_pData;

	BYTE* m_pPalette;
	int m_lPaletteColors;

	bool m_bIsGrayScale;
	double m_dJpegSaveQuality;

	bool m_bIsRGBA;

public:
	CBgraFrame();
	~CBgraFrame();

	void Destroy();
	void Clear();

	void ClearNoAttack();

	int get_Width();
	void put_Width(const int& lWidth);

	int get_Height();
	void put_Height(const int& lHeight);

	int get_Stride();
	void put_Stride(const int& lStride);

	BYTE* get_Data();
	void put_Data(BYTE* pData);

	void put_Palette(BYTE* pDataColors, const int& colors);

	bool IsGrayScale();

	void put_IsRGBA(const bool& bIsRGBA);
	bool get_IsRGBA();

	void SetJpegQuality(const double& value);

	bool OpenFile(const std::wstring& strFileName, unsigned int nFileType = 0, const bool& bIsOrientationRemove = false);

	bool SaveFile(const std::wstring& strFileName, unsigned int nFileType);
	bool Decode(BYTE*  pBuffer, int  nSize, unsigned int nFileType = 0);
	bool Encode(BYTE*& pBuffer, int& nSize, unsigned int nFileType);
	static void FreeEncodedMemory(void* pMemory);

	bool Resize(const long& nNewWidth, const long& nNewHeight, bool bDestroyData = true);

	bool ReColorPatternImage(const std::wstring& strFileName, unsigned int rgbColorBack, unsigned int rgbColorFore);

	void FromImage(IGrObject* pGraphics, bool bIsCopy = true);

	static bool RemoveOrientation(const std::wstring& sFile);
};

#endif

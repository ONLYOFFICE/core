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

#include "AlphaMask_p.h"

namespace Aggplus
{
	CAlphaMask::CAlphaMask()
		: m_pBuffer(NULL)
	{}

	CAlphaMask::CAlphaMask(BYTE *pBuffer, EMaskDataType eDataType, bool bExternalBuffer)
		: m_pBuffer(pBuffer), m_enDataType(eDataType), m_bExternalBuffer(bExternalBuffer)
	{}

	CAlphaMask::~CAlphaMask()
	{
		if (!m_bExternalBuffer)
			RELEASEARRAYOBJECTS(m_pBuffer);
	}

	BYTE *CAlphaMask::GetBuffer()
	{
		return m_pBuffer;
	}

	EMaskDataType CAlphaMask::GetDataType() const
	{
		return m_enDataType;
	}

	UINT CAlphaMask::GetStep() const
	{
		switch(m_enDataType)
		{
		case EMaskDataType::ImageBuffer: return 4;
		case EMaskDataType::AlphaBuffer: return 1;
		}
	}

	Status CAlphaMask::Create(UINT unWidth, UINT unHeight, EMaskDataType eDataType)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		m_enDataType      = eDataType;
		m_bExternalBuffer = false;

		UINT unSize = unWidth * unHeight * GetStep();
		
		m_pBuffer = new BYTE[unSize];

		if (NULL == m_pBuffer)
			return OutOfMemory;

		memset(m_pBuffer, 0x00, unSize);

		return Ok;
	}

	Status CAlphaMask::LoadFromBuffer(BYTE *pBuffer, EMaskDataType eDataType, bool bExternalBuffer)
	{
		m_pBuffer = pBuffer;
		m_enDataType = eDataType;
		m_bExternalBuffer = bExternalBuffer;
		return Ok;
	}

	CSoftMask::CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bFlip, bool bRGB, bool bAlpha)
	{
		if (bAlpha)
			m_pInternal = new CSoftMaskAlpha(pBuffer, unWidth, unHeight, false, bFlip);
		else
		{
			if (bRGB)
				m_pInternal = new CSoftMaskBGRAgray(pBuffer, unWidth, unHeight, false, bFlip);
			else
				m_pInternal = new CSoftMaskRGBAgray(pBuffer, unWidth, unHeight, false, bFlip);
		}
	}
	CSoftMask::~CSoftMask()
	{
		RELEASEOBJECT(m_pInternal);
	}

	unsigned int CSoftMask::GetStep() const { return m_pInternal->GetStep(); }
	unsigned int CSoftMask::GetWidth() const { return m_pInternal->GetWidth(); }
	unsigned int CSoftMask::GetHeight() const { return m_pInternal->GetHeight(); }
	BYTE* CSoftMask::GetBuffer() { return m_pInternal->GetBuffer(); }
	ESoftMaskType CSoftMask::GetDataType() { return m_pInternal->GetDataType(); }
}

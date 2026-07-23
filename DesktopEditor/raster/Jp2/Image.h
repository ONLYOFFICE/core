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

#include "Types.h"
#include "Utils.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Image
	//-------------------------------------------------------------------------------------------------------------------------------

	Image* Image_CreateEmpty()
	{
		Image *pImage = (Image*)Malloc(sizeof(Image));
		return pImage;
	}

	void   Image_Destroy(Image *pImage)
	{
		if (pImage)
		{
			if (pImage->pComponents)
			{
				for (int nIndex = 0; nIndex < pImage->nCsiz; nIndex++)
				{
					ImageComponent *pImageComp = &pImage->pComponents[nIndex];
					Free(pImageComp->pData);
				}
				Free(pImage->pComponents);
			}
			Free(pImage);
		}
	}

	Image* Image_Create(int nComponentsCount, ImageComponentParams *pCompParams, ColorSpace eColorSpace)
	{
		Image *pImage = NULL;
		pImage = (Image*)Malloc(sizeof(Image));

		if (pImage)
		{
			pImage->eColorSpace = eColorSpace;
			pImage->nCsiz       = nComponentsCount;

			// Allocate memory for components
			pImage->pComponents = (ImageComponent*)Malloc(pImage->nCsiz * sizeof(ImageComponent));

			if (!pImage->pComponents)
			{
				Image_Destroy(pImage);
				return NULL;
			}

			for (int nCurComponent = 0; nCurComponent < nComponentsCount; nCurComponent++)
			{
				ImageComponent *pComponent = &pImage->pComponents[nCurComponent];

				pComponent->nXRsiz     = pCompParams[nCurComponent].nXRsiz;
				pComponent->nYRsiz     = pCompParams[nCurComponent].nYRsiz;
				pComponent->nWidth     = pCompParams[nCurComponent].nWidth;
				pComponent->nHeight    = pCompParams[nCurComponent].nHeight;
				pComponent->nXOsiz     = pCompParams[nCurComponent].nXoffset;
				pComponent->nYOsiz     = pCompParams[nCurComponent].nYoffset;
				pComponent->nPrecision = pCompParams[nCurComponent].nPrecision;
				pComponent->nBPP       = pCompParams[nCurComponent].nBPP;
				pComponent->nSigned    = pCompParams[nCurComponent].nSigned;

				pComponent->pData = (int*)Malloc(pComponent->nWidth * pComponent->nHeight * sizeof(int));
				if (!pComponent->pData)
				{
					Image_Destroy(pImage);
					return NULL;
				}
			}
		}
		else
		{
			// TO DO: Display error message
		}

		return pImage;
	}
}
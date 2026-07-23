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

#include "Stream.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	void     TGT_Reset(TagTree *pTree)
	{
		if (NULL == pTree)
			return;

		for (int nIndex = 0; nIndex < pTree->nNodesCount; nIndex++)
		{
			pTree->pNodes[nIndex].nValue = 999;
			pTree->pNodes[nIndex].nLow   = 0;
			pTree->pNodes[nIndex].nKnown = 0;
		}
	}
	TagTree *TGT_Create(int nLeafsHorCount, int nLeafsVerCount)
	{
		TagTree *pTree = (TagTree *)Malloc(sizeof(TagTree));
		if (!pTree)
			return NULL;
		pTree->nLeafsHorCount = nLeafsHorCount;
		pTree->nLeafsVerCount = nLeafsVerCount;

		int nLevelsCount = 0;
		int anHor[32], anVer[32];
		anHor[0] = nLeafsHorCount;
		anVer[0] = nLeafsVerCount;

		pTree->nNodesCount = 0;
		int nCurLevelCount = 0;
		do
		{
			nCurLevelCount = anHor[nLevelsCount] * anVer[nLevelsCount];
			anHor[nLevelsCount + 1] = (anHor[nLevelsCount] + 1) / 2;
			anVer[nLevelsCount + 1] = (anVer[nLevelsCount] + 1) / 2;
			pTree->nNodesCount += nCurLevelCount;
			++nLevelsCount;
			// TO DO: Add additional check ( nLevelsCount + 1 < 32 )
		} while (nCurLevelCount > 1);

		// ADD
		if (pTree->nNodesCount == 0)
		{
			Free(pTree);
			return NULL;
		}

		pTree->pNodes = (TagNode *)Malloc(pTree->nNodesCount * sizeof(TagNode));
		if (!pTree->pNodes)
		{
			Free(pTree);
			return NULL;
		}

		TagNode *pNode = pTree->pNodes;
		TagNode *pParentNode = &pTree->pNodes[pTree->nLeafsHorCount * pTree->nLeafsVerCount];
		TagNode *pParentNode0 = pParentNode;

		for (int nI = 0; nI < nLevelsCount - 1; ++nI)
		{
			for (int nJ = 0; nJ < anVer[nI]; ++nJ)
			{
				int nK = anHor[nI];
				while (--nK >= 0)
				{
					pNode->pParent = pParentNode;
					++pNode;
					if (--nK >= 0)
					{
						pNode->pParent = pParentNode;
						++pNode;
					}
					++pParentNode;
				}
				if ((nJ & 1) || nJ == anVer[nI] - 1)
				{
					pParentNode0 = pParentNode;
				}
				else
				{
					pParentNode = pParentNode0;
					pParentNode0 += anHor[nI];
				}
			}
		}
		pNode->pParent = 0;

		TGT_Reset(pTree);

		return pTree;
	}

	void     TGT_Destroy(TagTree *pTree)
	{
		Free(pTree->pNodes);
		Free(pTree);
	}
	void     TGT_SetValue(TagTree *pTree, int nLeafIndex, int nValue)
	{
		TagNode *pNode = &pTree->pNodes[nLeafIndex];
		while (pNode && pNode->nValue > nValue)
		{
			pNode->nValue = nValue;
			pNode = pNode->pParent;
		}
	}

	void     TGT_Encode(BitIO *pBitStream, TagTree *pTree, int nLeafIndex, int nThreshold)
	{
		TagNode *apTreeNodes[31];
		TagNode **ppTreeNode;

		ppTreeNode = apTreeNodes;
		TagNode *pNode = &pTree->pNodes[nLeafIndex];
		while (pNode->pParent)
		{
			*ppTreeNode++ = pNode;
			pNode = pNode->pParent;
		}

		int nLow = 0;
		for (;;)
		{
			if (nLow > pNode->nLow)
			{
				pNode->nLow = nLow;
			}
			else
			{
				nLow = pNode->nLow;
			}

			while (nLow < nThreshold)
			{
				if (nLow >= pNode->nValue)
				{
					if (!pNode->nKnown)
					{
						BitIO_Write(pBitStream, 1, 1);
						pNode->nKnown = 1;
					}
					break;
				}
				BitIO_Write(pBitStream, 0, 1);
				++nLow;
			}

			pNode->nLow = nLow;
			if (ppTreeNode == apTreeNodes)
				break;
			pNode = *--ppTreeNode;
		}
	}

	int      TGT_Decode(BitIO *pBitStream, TagTree *pTree, int nLeafIndex, int nThreshold)
	{
		TagNode *apTreeNodes[31];
		TagNode **ppTreeNode;

		ppTreeNode = apTreeNodes;
		TagNode *pNode = &pTree->pNodes[nLeafIndex];
		while (pNode->pParent)
		{
			*ppTreeNode++ = pNode;
			pNode = pNode->pParent;
		}

		int nLow = 0;
		for (;;)
		{
			if (nLow > pNode->nLow)
			{
				pNode->nLow = nLow;
			}
			else
			{
				nLow = pNode->nLow;
			}
			while (nLow < nThreshold && nLow < pNode->nValue)
			{
				if (BitIO_Read(pBitStream, 1))
				{
					pNode->nValue = nLow;
				}
				else
				{
					++nLow;
				}
			}
			pNode->nLow = nLow;
			if (ppTreeNode == apTreeNodes)
			{
				break;
			}
			pNode = *--ppTreeNode;
		}

		return (pNode->nValue < nThreshold) ? 1 : 0;
	}
}
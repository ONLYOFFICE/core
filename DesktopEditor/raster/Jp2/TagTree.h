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
			// TO DO: Поставить дополнительную проверку  ( nLevelsCount + 1 < 32 )
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
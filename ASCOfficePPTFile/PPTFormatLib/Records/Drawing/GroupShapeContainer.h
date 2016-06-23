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
#pragma once

#include <vector>
#include "ShapeContainer.h"

class CRecordGroupShapeContainer : public CRecordsContainer
{
private:
	RECT m_rcGroupBounds;
	RECT m_rcGroupClientAnchor;

public:
	
	CRecordGroupShapeContainer()
	{
		m_rcGroupBounds.left = 0;
		m_rcGroupBounds.top = 0;
		m_rcGroupBounds.right = 0;
		m_rcGroupBounds.bottom = 0;

		m_rcGroupClientAnchor.left = 0;
		m_rcGroupClientAnchor.top = 0;
		m_rcGroupClientAnchor.right = 0;
		m_rcGroupClientAnchor.bottom = 0;
	}

	~CRecordGroupShapeContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);

		// вот... а теперь нужно взять и узнать перерасчет системы координат
		std::vector<CRecordShapeContainer*> oArrayShapes;
		GetRecordsByType(&oArrayShapes, false, false);

		if (!oArrayShapes.empty())
			oArrayShapes[0]->bGroupShape = true;//тут описание самой группы

		int nIndexBreak = -1;
		for (int nIndex = 0; nIndex < oArrayShapes.size(); ++nIndex)
		{
			std::vector<CRecordGroupShape*> oArrayGroupShapes;
			oArrayShapes[nIndex]->GetRecordsByType(&oArrayGroupShapes, false, true);

			if ( oArrayGroupShapes.size() > 0 )
			{
				m_rcGroupBounds.left	= oArrayGroupShapes[0]->m_oBounds.left;
				m_rcGroupBounds.top		= oArrayGroupShapes[0]->m_oBounds.top;
				m_rcGroupBounds.right	= oArrayGroupShapes[0]->m_oBounds.right;
				m_rcGroupBounds.bottom	= oArrayGroupShapes[0]->m_oBounds.bottom;

				std::vector<CRecordClientAnchor*> oArrayClients;
				oArrayShapes[nIndex]->GetRecordsByType(&oArrayClients, false, true);

				if ( oArrayClients.size() > 0)
				{
					m_rcGroupClientAnchor.left		= (LONG)oArrayClients[0]->m_oBounds.Left;
					m_rcGroupClientAnchor.top		= (LONG)oArrayClients[0]->m_oBounds.Top;
					m_rcGroupClientAnchor.right		= (LONG)oArrayClients[0]->m_oBounds.Right;
					m_rcGroupClientAnchor.bottom	= (LONG)oArrayClients[0]->m_oBounds.Bottom;
				}
				else
				{
					std::vector<CRecordChildAnchor*> oArrayChilds;
					oArrayShapes[nIndex]->GetRecordsByType(&oArrayChilds, false, true);
					
					if ( oArrayChilds.size() > 0)
					{
						m_rcGroupClientAnchor.left		= (LONG)oArrayChilds[0]->m_oBounds.left;
						m_rcGroupClientAnchor.top		= (LONG)oArrayChilds[0]->m_oBounds.top;
						m_rcGroupClientAnchor.right		= (LONG)oArrayChilds[0]->m_oBounds.right;
						m_rcGroupClientAnchor.bottom	= (LONG)oArrayChilds[0]->m_oBounds.bottom;
					}
				}

				nIndexBreak = nIndex;
				break;
			}
		}

		LONG lW1 = m_rcGroupBounds.right		- m_rcGroupBounds.left;
		LONG lH1 = m_rcGroupBounds.bottom		- m_rcGroupBounds.top;
		LONG lW2 = m_rcGroupClientAnchor.right	- m_rcGroupClientAnchor.left;
		LONG lH2 = m_rcGroupClientAnchor.bottom - m_rcGroupClientAnchor.top;

        bool bIsRecalc = ((lW1 > 0) && (lH1 > 0) && (lW2 > 0) && (lH2 > 0));
		if (bIsRecalc)
		{
			for (int nIndex = 0; nIndex < oArrayShapes.size(); ++nIndex)
			{
				if (nIndex != nIndexBreak)
				{
					oArrayShapes[nIndex]->m_pGroupBounds		= &m_rcGroupBounds;
					oArrayShapes[nIndex]->m_pGroupClientAnchor	= &m_rcGroupClientAnchor;
				}
			}
		}
	}

	void SetGroupRect()
	{
		std::vector<CRecordGroupShapeContainer*> oArrayGroupContainer;
		this->GetRecordsByType(&oArrayGroupContainer, false, false);

		int nCountGroups = oArrayGroupContainer.size();
		for (int i = 0; i < nCountGroups; ++i)
		{
			LONG lWidthGroup	= m_rcGroupBounds.right			- m_rcGroupBounds.left;
			LONG lHeightGroup	= m_rcGroupBounds.bottom		- m_rcGroupBounds.top;
			LONG lWidthClient	= m_rcGroupClientAnchor.right	- m_rcGroupClientAnchor.left;
			LONG lHeightClient	= m_rcGroupClientAnchor.bottom	- m_rcGroupClientAnchor.top;

            bool bIsRecalc = ((lWidthClient > 0) && (lHeightClient > 0) && (lWidthClient > 0) && (lHeightGroup > 0));

			if (bIsRecalc)
			{
				// здесь переводим координаты, чтобы они не зависили от группы
				double dScaleX = (double)(lWidthClient) / (lWidthGroup);
				double dScaleY = (double)(lHeightClient) / (lHeightGroup);
				
				RECT* prcChildAnchor = &oArrayGroupContainer[i]->m_rcGroupClientAnchor;

				prcChildAnchor->left	= m_rcGroupClientAnchor.left + (LONG)(dScaleX * (prcChildAnchor->left	- m_rcGroupBounds.left));
				prcChildAnchor->right	= m_rcGroupClientAnchor.left + (LONG)(dScaleX * (prcChildAnchor->right	- m_rcGroupBounds.left));

				prcChildAnchor->top		= m_rcGroupClientAnchor.top	 + (LONG)(dScaleY * (prcChildAnchor->top	- m_rcGroupBounds.top));
				prcChildAnchor->bottom	= m_rcGroupClientAnchor.top  + (LONG)(dScaleY * (prcChildAnchor->bottom	- m_rcGroupBounds.top));
			}

			oArrayGroupContainer[i]->SetGroupRect();
		}
	}
};

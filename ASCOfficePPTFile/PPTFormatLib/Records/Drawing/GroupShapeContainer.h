#pragma once
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
		this->GetRecordsByType(&oArrayShapes, false, false);

		int nIndexBreak = -1;
		for (int nIndex = 0; nIndex < oArrayShapes.size(); ++nIndex)
		{
			std::vector<CRecordGroupShape*> oArrayGroupShapes;
			oArrayShapes[nIndex]->GetRecordsByType(&oArrayGroupShapes, false, true);

			if (0 != oArrayGroupShapes.size())
			{
				m_rcGroupBounds.left	= oArrayGroupShapes[0]->m_oBounds.left;
				m_rcGroupBounds.top		= oArrayGroupShapes[0]->m_oBounds.top;
				m_rcGroupBounds.right	= oArrayGroupShapes[0]->m_oBounds.right;
				m_rcGroupBounds.bottom	= oArrayGroupShapes[0]->m_oBounds.bottom;

				std::vector<CRecordClientAnchor*> oArrayClients;
				oArrayShapes[nIndex]->GetRecordsByType(&oArrayClients, false, true);

				if (0 != oArrayClients.size())
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
					
					if (0 != oArrayChilds.size())
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

		LONG lW1 = m_rcGroupBounds.right - m_rcGroupBounds.left;
		LONG lH1 = m_rcGroupBounds.bottom - m_rcGroupBounds.top;
		LONG lW2 = m_rcGroupClientAnchor.right - m_rcGroupClientAnchor.left;
		LONG lH2 = m_rcGroupClientAnchor.bottom - m_rcGroupClientAnchor.top;

		BOOL bIsRecalc = ((lW1 > 0) && (lH1 > 0) && (lW2 > 0) && (lH2 > 0));
		if (bIsRecalc)
		{
			for (int nIndex = 0; nIndex < oArrayShapes.size(); ++nIndex)
			{
				if (nIndex != nIndexBreak)
				{
					oArrayShapes[nIndex]->m_pGroupBounds = &m_rcGroupBounds;
					oArrayShapes[nIndex]->m_pGroupClientAnchor = &m_rcGroupClientAnchor;
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

			BOOL bIsRecalc = ((lWidthClient > 0) && (lHeightClient > 0) && (lWidthClient > 0) && (lHeightGroup > 0));

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

	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};
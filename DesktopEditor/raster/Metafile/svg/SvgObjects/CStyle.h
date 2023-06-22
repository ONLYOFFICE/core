#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../SvgUtils.h"
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator_Private.h"
#include "CObjectBase.h"

#include <numeric>

namespace SVG
{
	class CSvgCalculator
	{
		NSCSS::CCssCalculator_Private *m_pInternal;
	public:
		CSvgCalculator();
		~CSvgCalculator();

		void AddStyles(const std::wstring& wsStyles);

		void Clear();

		void SetData(CObject* pSvgObject) const
		{
			if (NULL == pSvgObject)
				return;

			const std::map<std::wstring, NSCSS::CElement *> *pData = m_pInternal->GetData();
			const std::vector<NSCSS::CNode> arSelectors = pSvgObject->GetFullPath();

			if ((NULL == pData || pData->empty()) && arSelectors.empty())
				return;

			std::vector<std::wstring> arWords;
			arWords.reserve(arSelectors.size() * 2);

			std::vector<std::wstring> arNextNodes;
			arNextNodes.reserve(arSelectors.size() * 2);

			for (std::vector<NSCSS::CNode>::const_reverse_iterator oNode = arSelectors.rbegin(); oNode != arSelectors.rend(); ++oNode)
			{
				arWords.push_back(oNode->m_sName);

				if (!oNode->m_sClass.empty())
				{
					if (oNode->m_sClass.find(L' ') != std::wstring::npos)
					{
						std::vector<std::wstring> arClasses = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(oNode->m_sClass, L" ");

						if (arClasses.size() > 1)
							arClasses.resize(unique(arClasses.begin(),arClasses.end()) - arClasses.begin());
						switch (arClasses.size())
						{
							case 1:
							{
								arWords.push_back(L'.' + arClasses[0]);
								break;
							}
							case 2:
							{
								arWords.push_back(L'.' + arClasses[0] + L" ." + arClasses[1]);
								break;
							}
							case 3:
							{
								arWords.push_back(L'.' + arClasses[0] + L" ." + arClasses[1] + L" ." + arClasses[2]);
								break;
							}
							default:
							{
								arWords.push_back(std::accumulate(arClasses.begin(), arClasses.end(), std::wstring(),
								                                  [](std::wstring sRes, const std::wstring& sClass)
								                                    {return sRes += L'.' + sClass + L' ';}));
								break;
							}
						}
					}
					else
						arWords.push_back(L'.' + oNode->m_sClass);
				}
				if (!oNode->m_sId.empty())
					arWords.push_back(L'#' + oNode->m_sId);
			}

			std::vector<NSCSS::CElement*> arElements;

			for (size_t i = 0; i < arSelectors.size(); ++i)
			{
				std::wstring sName, sId;
				std::vector<std::wstring> arClasses;

				if (arWords.back()[0] == L'#')
				{
					sId = arWords.back();
					arWords.pop_back();
					arNextNodes.push_back(sId);
				}

				if (arWords.back()[0] == L'.')
				{
					arClasses = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(arWords.back(), L" ");
					arNextNodes.push_back(arWords.back());
					arWords.pop_back();
				}

				sName = arWords.back();
				arWords.pop_back();
				arNextNodes.push_back(sName);

				const std::map<std::wstring, NSCSS::CElement*>::const_iterator oFindName = pData->find(sName);
				std::map<std::wstring, NSCSS::CElement*>::const_iterator oFindId;
				std::vector<NSCSS::CElement*> arFindElements;

				if (!sId.empty())
				{
					oFindId = pData->find(sId);

					if (oFindId != std::end(*pData))
					{
						if (!oFindId->second->Empty())
							arFindElements.push_back(oFindId->second);

						const std::vector<NSCSS::CElement*> arTempPrev = oFindId->second->GetPrevElements(arNextNodes.rbegin() + ((arClasses.empty()) ? 1 : 2), arNextNodes.rend());

						if (!arTempPrev.empty())
							arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());
					}
				}

				if (!arClasses.empty())
				{
					for (std::vector<std::wstring>::const_reverse_iterator iClass = arClasses.rbegin(); iClass != arClasses.rend(); ++iClass)
					{
						const std::map<std::wstring, NSCSS::CElement*>::const_iterator oFindClass = pData->find(*iClass);
						if (oFindClass != std::end(*pData))
						{
							if (!oFindClass->second->Empty())
								arFindElements.push_back(oFindClass->second);

							const std::vector<NSCSS::CElement*> arTempPrev = oFindClass->second->GetPrevElements(arNextNodes.rbegin() + 2, arNextNodes.rend());
							const std::vector<NSCSS::CElement*> arTempKins = oFindClass->second->GetNextOfKin(sName);

							if (!arTempPrev.empty())
								arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());

							if (!arTempKins.empty())
								arFindElements.insert(arFindElements.end(), arTempKins.begin(), arTempKins.end());
						}
					}
				}

				if (oFindName != std::end(*pData))
				{
					if (!oFindName->second->Empty())
						arFindElements.push_back(oFindName->second);

					const std::vector<NSCSS::CElement*> arTempPrev = oFindName->second->GetPrevElements(arNextNodes.rbegin() + 1, arNextNodes.rend());
					const std::vector<NSCSS::CElement*> arTempKins = oFindName->second->GetNextOfKin(sName, arClasses);

					if (!arTempPrev.empty())
						arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());

					if (!arTempKins.empty())
						arFindElements.insert(arFindElements.end(), arTempKins.begin(), arTempKins.end());
				}


				if (arFindElements.size() > 1)
				{
					std::sort(arFindElements.rbegin(), arFindElements.rend(),
					          [](NSCSS::CElement* oFirstElement, NSCSS::CElement* oSecondElement)
					          {
						          return oFirstElement->GetWeight() > oSecondElement->GetWeight();
					          });
				}

				pSvgObject->SetData(arSelectors[i].m_mAttrs, i + 1);

				for (const NSCSS::CElement* oElement : arFindElements)
					pSvgObject->SetData(oElement->GetStyle(), i + 1);

				pSvgObject->SetData(arSelectors[i].m_sStyle, i + 1, true);
			}
		};
	};
}

#endif // CSTYLE_H

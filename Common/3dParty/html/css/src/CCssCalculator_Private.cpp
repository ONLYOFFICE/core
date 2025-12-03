#include "CCssCalculator_Private.h"

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

#include "../../katana-parser/src/selector.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "StaticFunctions.h"

#define MaxNumberRepetitions 6

inline static std::wstring StringifyValueList(const KatanaArray* oValues);
inline static std::wstring StringifyValue(const KatanaValue* oValue);
inline static bool         IsTableElement(const std::wstring& wsNameTag);

bool operator<(const std::vector<NSCSS::CNode> &arLeftSelectors, const std::vector<NSCSS::CNode> &arRightSelectors)
{
	const size_t& sizeLeftSelectors = arLeftSelectors.size();
	const size_t& sizeRightSelectors = arRightSelectors.size();

	if (sizeLeftSelectors < sizeRightSelectors)
		return true;
	else if (sizeLeftSelectors > sizeRightSelectors)
		return false;

	for (size_t i = 0; i < arLeftSelectors.size(); ++i)
	{
		if (arLeftSelectors[i] < arRightSelectors[i])
			return true;
	}

	return false;
}

namespace NSCSS
{
	CStyleStorage::CStyleStorage()
	{}

	CStyleStorage::~CStyleStorage()
	{
		Clear();
	}

	void CStyleStorage::Clear()
	{
		for (TStyleFileData* pStyleFileData : m_arStyleFiles)
		{
			if (nullptr == pStyleFileData)
				continue;

			for (std::map<std::wstring, CElement*>::iterator oIter = pStyleFileData->m_mStyleData.begin(); oIter != pStyleFileData->m_mStyleData.end(); ++oIter)
				if (oIter->second != nullptr)
					delete oIter->second;

			delete pStyleFileData;
		}

		m_arStyleFiles.clear();
		m_arEmptyStyleFiles.clear();

		ClearEmbeddedStyles();
		ClearAllowedStyleFiles();

		#ifdef CSS_CALCULATOR_WITH_XHTML
		ClearPageData();
		#endif
	}

	void CStyleStorage::AddStyles(const std::string& sStyle)
	{
		if (sStyle.empty())
			return;

		KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.length(), KatanaParserModeStylesheet);
		this->GetOutputData(output, m_mEmbeddedStyleData);
		katana_destroy_output(output);
	}

	void CStyleStorage::AddStyles(const std::wstring& wsStyle)
	{
		if (wsStyle.empty())
			return;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		std::wregex oRegex(L"@page\\s*([^{]*)(\\{[^}]*\\})");
		std::wsmatch oMatch;
		std::wstring::const_iterator oSearchStart(wsStyle.cbegin());

		while (std::regex_search(oSearchStart, wsStyle.cend(), oMatch, oRegex))
		{
			AddPageData(oMatch[1].str(), oMatch[2].str());
			oSearchStart = oMatch.suffix().first;
		}
		#endif

		AddStyles(U_TO_UTF8(wsStyle));
	}

	void CStyleStorage::AddStylesFromFile(const std::wstring& wsFileName)
	{
		std::set<std::wstring>::const_iterator itEmptyFileFound = m_arEmptyStyleFiles.find(wsFileName);

		if (m_arEmptyStyleFiles.cend() != itEmptyFileFound)
			return;

		std::vector<TStyleFileData*>::const_iterator itFound = std::find_if(m_arStyleFiles.cbegin(), m_arStyleFiles.cend(),
		                                                                    [wsFileName](const TStyleFileData* pStyleFileData)
		                                                                    { return wsFileName == pStyleFileData->m_wsStyleFilepath; });

		m_arAllowedStyleFiles.insert(wsFileName);

		if (m_arStyleFiles.cend() != itFound)
			return;

		TStyleFileData *pStyleFileData = new TStyleFileData();

		pStyleFileData->m_wsStyleFilepath = wsFileName;

		AddStyles(NS_STATIC_FUNCTIONS::GetContentAsUTF8(wsFileName), pStyleFileData->m_mStyleData);

		if (!pStyleFileData->m_mStyleData.empty())
			m_arStyleFiles.push_back(pStyleFileData);
		else
		{
			m_arEmptyStyleFiles.insert(wsFileName);
			delete pStyleFileData;
		}
	}

	void CStyleStorage::ClearStylesFromFile(const std::wstring& wsFileName)
	{
		std::vector<TStyleFileData*>::const_iterator itFound = std::find_if(m_arStyleFiles.cbegin(), m_arStyleFiles.cend(),
																			[wsFileName](const TStyleFileData* pStyleFileData)
																			{ return wsFileName == pStyleFileData->m_wsStyleFilepath; });

		if (m_arStyleFiles.cend() != itFound)
		{
			m_arStyleFiles.erase(itFound);
			delete *itFound;
		}
	}

	#ifdef CSS_CALCULATOR_WITH_XHTML
	void CStyleStorage::AddPageData(const std::wstring& wsPageName, const std::wstring& wsStyles)
	{
		m_arPageDatas.push_back({NS_STATIC_FUNCTIONS::GetWordsW(wsPageName), NS_STATIC_FUNCTIONS::GetRules(wsStyles)});
	}

	void CStyleStorage::SetPageData(NSProperties::CPage& oPage, const std::map<std::wstring, std::wstring>& mData, unsigned int unLevel, bool bHardMode)
	{
		for (const std::pair<std::wstring, std::wstring> &oData : mData)
		{
			if (L"margin" == oData.first)
				oPage.SetMargin(oData.second, unLevel, bHardMode);
			else if (L"size" == oData.first)
				oPage.SetSize(oData.second, unLevel, bHardMode);
			else if (L"mso-header-margin" == oData.first)
				oPage.SetHeader(oData.second, unLevel, bHardMode);
			else if (L"mso-footer-margin" == oData.first)
				oPage.SetFooter(oData.second, unLevel, bHardMode);
		}
	}

	std::map<std::wstring, std::wstring> CStyleStorage::GetPageData(const std::wstring& wsPageName)
	{
		if (m_arPageDatas.empty())
			return {};

		for (const TPageData& oPageData : m_arPageDatas)
		{
			if (std::find(oPageData.m_wsNames.begin(), oPageData.m_wsNames.end(), wsPageName) != oPageData.m_wsNames.end())
				return oPageData.m_mData;
		}

		return {};
	}

	void CStyleStorage::ClearPageData()
	{
		m_arPageDatas.clear();
	}
	#endif

	const CElement* CStyleStorage::FindElement(const std::wstring& wsSelector) const
	{
		if (wsSelector.empty())
			return nullptr;

		const CElement* pFoundElement = FindSelectorFromStyleData(wsSelector, m_mEmbeddedStyleData);

		if (nullptr != pFoundElement)
			return pFoundElement;

		for (std::vector<TStyleFileData*>::const_reverse_iterator itIter = m_arStyleFiles.crbegin(); itIter < m_arStyleFiles.crend(); ++itIter)
		{
			if (m_arAllowedStyleFiles.cend() == std::find(m_arAllowedStyleFiles.cbegin(), m_arAllowedStyleFiles.cend(), (*itIter)->m_wsStyleFilepath))
				continue;

			pFoundElement = FindSelectorFromStyleData(wsSelector, (*itIter)->m_mStyleData);

			if (nullptr != pFoundElement)
				return pFoundElement;
		}

		return nullptr;
	}

	void CStyleStorage::AddStyles(const std::string& sStyle, std::map<std::wstring, CElement*>& mStyleData)
	{
		if (sStyle.empty())
			return;

		KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.length(), KatanaParserModeStylesheet);
		this->GetOutputData(output, mStyleData);
		katana_destroy_output(output);
	}

	void CStyleStorage::ClearEmbeddedStyles()
	{
		for (std::map<std::wstring, CElement*>::iterator oIter = m_mEmbeddedStyleData.begin(); oIter != m_mEmbeddedStyleData.end(); ++oIter)
			if (oIter->second != nullptr)
				delete oIter->second;

		m_mEmbeddedStyleData.clear();
	}

	void CStyleStorage::ClearAllowedStyleFiles()
	{
		m_arAllowedStyleFiles.clear();
	}

	void CStyleStorage::GetStylesheet(const KatanaStylesheet* oStylesheet, std::map<std::wstring, CElement*>& mStyleData)
	{
		for (size_t i = 0; i < oStylesheet->imports.length; ++i)
			GetRule((KatanaRule*)oStylesheet->imports.data[i], mStyleData);

		for (size_t i = 0; i < oStylesheet->rules.length; ++i)
			GetRule((KatanaRule*)oStylesheet->rules.data[i], mStyleData);
	}

	void CStyleStorage::GetRule(const KatanaRule* oRule, std::map<std::wstring, CElement*>& mStyleData)
	{
		if ( NULL == oRule )
			return;

		switch (oRule->type) {
			case KatanaRuleStyle:
			{
				GetStyleRule((KatanaStyleRule*)oRule, mStyleData);
				break;
			}
			default:
				break;
		}
	}

	void CStyleStorage::GetStyleRule(const KatanaStyleRule* oRule, std::map<std::wstring, CElement*>& mStyleData)
	{
		if (oRule->declarations->length == 0)
			return;

		const std::map<std::wstring, std::wstring> mStyle = GetDeclarationList(oRule->declarations);
		for (const std::wstring &wsSelector : GetSelectorList(oRule->selectors))
		{
			std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(wsSelector, false, L" ");

			CElement* oLastElement = NULL;
			CElement* oFirstElement = NULL;
			bool bCreateFirst = true;

			for (std::vector<std::wstring>::reverse_iterator oWord = arWords.rbegin(); oWord != arWords.rend(); ++oWord)
			{
					const size_t posPoint = oWord->find(L'.');
					const size_t posLattice = oWord->find(L'#');

					const std::wstring sName = (posPoint != std::wstring::npos) ? oWord->substr(0, posPoint) : (posLattice != std::wstring::npos) ? oWord->substr(0, posLattice) : *oWord;
					const std::wstring sClass = (posPoint != std::wstring::npos) ? (posLattice == std::wstring::npos) ? oWord->substr(posPoint, oWord->length()) : oWord->substr(posPoint, posLattice - posPoint) : L"";
					const std::wstring sId = (posLattice != std::wstring::npos) ? oWord->substr(posLattice, oWord->length()) : L"";

					CElement* oNameElement = NULL;
					CElement* oClassElement = NULL;
					CElement* oIdElement = NULL;
					bool bIsNewElement = true;

					if (!sId.empty())
					{
						if (NULL == oFirstElement && bCreateFirst)
						{
							const std::map<std::wstring, CElement*>::const_iterator& oFindId = mStyleData.find(sId);
							if (oFindId != mStyleData.end())
							{
								oIdElement = oFindId->second;
								bCreateFirst = false;
							}
							else
							{
								oIdElement = new CElement;
								oIdElement->SetSelector(sId);
								if (bCreateFirst)
									oFirstElement = oIdElement;
							}
						}
						else
						{
							oIdElement = new CElement;
							oIdElement->SetSelector(sId);

							oLastElement->AddPrevElement(oIdElement);
						}
						bIsNewElement = false;
						oLastElement = oIdElement;
					}

					if (!sClass.empty())
					{
						if (NULL == oFirstElement && bCreateFirst)
						{
							const std::map<std::wstring, CElement*>::const_iterator& oFindClass = mStyleData.find(sClass);
							if (oFindClass != mStyleData.end())
							{
								oClassElement = oFindClass->second;
								bCreateFirst = false;
							}
							else
							{
								oClassElement = new CElement;
								oClassElement->SetSelector(sClass);
								if (bCreateFirst)
									oFirstElement = oClassElement;
							}
						}
						else
						{
							oClassElement = new CElement;
							oClassElement->SetSelector(sClass);

							if (bIsNewElement)
								oLastElement->AddPrevElement(oClassElement);
							else
								oLastElement->AddKinElement(oClassElement);
						}

						bIsNewElement = false;
						oLastElement = oClassElement;
					}

					if (!sName.empty())
					{
						if (NULL == oFirstElement && bCreateFirst)
						{
							const std::map<std::wstring, CElement*>::const_iterator& oFindName = mStyleData.find(sName);
							if (oFindName != mStyleData.end())
							{
								oNameElement = oFindName->second;
								bCreateFirst = false;
							}
							else
							{
								oNameElement = new CElement;
								oNameElement->SetSelector(sName);
								if (bCreateFirst)
									oFirstElement = oNameElement;
							}
						}
						else
						{
							oNameElement = new CElement;
							oNameElement->SetSelector(sName);

							if (bIsNewElement)
								oLastElement->AddPrevElement(oNameElement);
							else
								oLastElement->AddKinElement(oNameElement);

						}
						oLastElement = oNameElement;
					}
			}

			if (NULL != oLastElement)
				oLastElement->AddProperties(mStyle);

			if (NULL != oFirstElement)
				mStyleData[oFirstElement->GetSelector()] = oFirstElement;
		}
	}

	std::wstring CStyleStorage::GetValueList(const KatanaArray* oValues)
	{
		return StringifyValueList(oValues);
	}

	std::vector<std::wstring> CStyleStorage::GetSelectorList(const KatanaArray* oSelectors) const
	{
		if (oSelectors->length == 0)
			return std::vector<std::wstring>();

		std::vector<std::wstring> arSelectors;

		for (unsigned int i = 0; i < oSelectors->length; ++i)
			arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));

		return arSelectors;
	}

	std::wstring CStyleStorage::GetSelector(const KatanaSelector* oSelector) const
	{
		KatanaParser oParser;
		oParser.options = &kKatanaDefaultOptions;

		std::wstring wsText;
		const KatanaParserString* string = katana_selector_to_string(&oParser, const_cast<KatanaSelector*>(oSelector), NULL);
		const char* text = katana_string_to_characters(&oParser, string);

		katana_parser_deallocate(&oParser, (void*) string->data);
		katana_parser_deallocate(&oParser, (void*) string);

		wsText = UTF8_TO_U(std::string(text));

		katana_parser_deallocate(&oParser, (void*)text);

		return wsText;
	}

	std::map<std::wstring, std::wstring> CStyleStorage::GetDeclarationList(const KatanaArray* oDeclarations) const
	{
		if(oDeclarations->length == 0)
			return std::map<std::wstring, std::wstring>();

		std::map<std::wstring, std::wstring> arDeclarations;

		for (size_t i = 0; i < oDeclarations->length; ++i)
			arDeclarations.insert(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));

		return arDeclarations;
	}

	std::pair<std::wstring, std::wstring> CStyleStorage::GetDeclaration(const KatanaDeclaration* oDecl) const
	{
		std::wstring sValueList = StringifyValueList(oDecl->values);

		if (oDecl->important)
			sValueList += L" !important";

		return std::make_pair(UTF8_TO_U(std::string(oDecl->property)), sValueList);
	}

	void CStyleStorage::GetOutputData(KatanaOutput* oOutput, std::map<std::wstring, CElement*>& mStyleData)
	{
		if ( NULL == oOutput )
			return;

		switch (oOutput->mode) {
			case KatanaParserModeStylesheet:
				GetStylesheet(oOutput->stylesheet, mStyleData);
				break;
			case KatanaParserModeRule:
				GetRule(oOutput->rule, mStyleData);
				break;
			case KatanaParserModeKeyframeRule:
			case KatanaParserModeKeyframeKeyList:
			case KatanaParserModeMediaList:
			case KatanaParserModeValue:
			case KatanaParserModeSelector:
			case KatanaParserModeDeclarationList:
				break;
		}
	}

	const CElement* CStyleStorage::FindSelectorFromStyleData(const std::wstring& wsSelector, const std::map<std::wstring, CElement*>& mStyleData) const
	{
		std::map<std::wstring, CElement*>::const_iterator itFound = mStyleData.find(wsSelector);

		if (mStyleData.cend() != itFound)
			return itFound->second;

		return nullptr;
	}

	CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_nCountNodes(0), m_sEncoding(L"UTF-8"){}

	CCssCalculator_Private::~CCssCalculator_Private()
	{}

	#ifdef CSS_CALCULATOR_WITH_XHTML
	bool CCssCalculator_Private::CalculateCompiledStyle(std::vector<CNode>& arSelectors)
	{
		if (arSelectors.empty())
			return false;

		if (L"#text" == arSelectors.back().m_wsName)
		{
			if (arSelectors.size() > 1 && arSelectors.back().m_pCompiledStyle->Empty())
				*arSelectors.back().m_pCompiledStyle += *(arSelectors.end() - 2)->m_pCompiledStyle;

			if(arSelectors.crend() != std::find_if(arSelectors.crbegin(), arSelectors.crend(),
			                                       [](const CNode& oNode){ return IsTableElement(oNode.m_wsName); }))
			{
				arSelectors.back().m_pCompiledStyle->m_oBackground.Clear();
				arSelectors.back().m_pCompiledStyle->m_oBorder.Clear();
			}

			return true;
		}

		const std::map<std::vector<CNode>, CCompiledStyle>::iterator oItem = m_mUsedStyles.find(arSelectors);

		if (oItem != m_mUsedStyles.end())
		{
			arSelectors.back().SetCompiledStyle(new CCompiledStyle(oItem->second));
			return true;
		}

		if (!arSelectors.back().m_pCompiledStyle->Empty())
			return true;

		arSelectors.back().m_pCompiledStyle->SetDpi(m_nDpi);
		unsigned int unStart = 0;

		std::vector<CNode>::const_reverse_iterator itFound = std::find_if(arSelectors.crbegin(), arSelectors.crend(), [](const CNode& oNode){ return !oNode.m_pCompiledStyle->Empty(); });

		if (itFound != arSelectors.crend())
			unStart = itFound.base() - arSelectors.cbegin();

		std::vector<std::wstring> arNodes = CalculateAllNodes(arSelectors, unStart, arSelectors.size());
		std::vector<std::wstring> arPrevNodes = CalculateAllNodes(arSelectors, 0, unStart);
		bool bInTable = false;

		for (size_t i = 0; i < unStart; ++i)
		{
			if (!bInTable)
				bInTable = IsTableElement(arSelectors[i].m_wsName);
			else
				break;
		}

		for (size_t i = unStart; i < arSelectors.size(); ++i)
		{
			if (0 != i)
				*arSelectors[i].m_pCompiledStyle += *arSelectors[i - 1].m_pCompiledStyle;

			arSelectors[i].m_pCompiledStyle->AddParent(arSelectors[i].m_wsName);

			if (!bInTable)
				bInTable = IsTableElement(arSelectors[i].m_wsName);

			if (bInTable)
			{
				arSelectors[i].m_pCompiledStyle->m_oBackground.Clear();
				arSelectors[i].m_pCompiledStyle->m_oBorder.Clear();
				arSelectors[i].m_pCompiledStyle->m_oDisplay.Clear();
			}

			arSelectors[i].m_pCompiledStyle->AddStyle(arSelectors[i].m_mAttributes, i + 1);

			for (const CElement* oElement : FindElements(arNodes, arPrevNodes))
				arSelectors[i].m_pCompiledStyle->AddStyle(oElement->GetStyle(), i + 1);

			if (!arSelectors[i].m_wsStyle.empty())
				arSelectors[i].m_pCompiledStyle->AddStyle(arSelectors[i].m_wsStyle, i + 1, true);

			// Скидываем некоторые внешние стили, которые внутри таблицы переопределяются
			if (bInTable && i < arSelectors.size() - 1)
			{
				arSelectors[i].m_pCompiledStyle->m_oFont.GetLineHeight().Clear();
				arSelectors[i].m_pCompiledStyle->m_oPadding.Clear();
				arSelectors[i].m_pCompiledStyle->m_oMargin.Clear();
			}
		}

		arSelectors.back().m_pCompiledStyle->SetID(CalculateStyleId(arSelectors.back()));

		if (!arSelectors.back().m_pCompiledStyle->Empty())
			m_mUsedStyles[arSelectors] = *arSelectors.back().m_pCompiledStyle;

		return true;
	}

	void CCssCalculator_Private::SetPageData(NSProperties::CPage &oPage, const std::map<std::wstring, std::wstring> &mData, unsigned int unLevel, bool bHardMode)
	{
		//TODO:: пересмотреть данный метод
		m_oStyleStorage.SetPageData(oPage, mData, unLevel, bHardMode);
	}

	std::map<std::wstring, std::wstring> CCssCalculator_Private::GetPageData(const std::wstring &wsPageName)
	{
		return m_oStyleStorage.GetPageData(wsPageName);
	}

	void CCssCalculator_Private::ClearPageData()
	{
		m_oStyleStorage.ClearPageData();
	}
	#endif

	std::vector<std::wstring> CCssCalculator_Private::CalculateAllNodes(const std::vector<CNode> &arSelectors, unsigned int unStart, unsigned int unEnd)
	{
		if ((0 != unEnd && (unEnd < unStart || unEnd > arSelectors.size())) || (unStart == unEnd))
			return std::vector<std::wstring>();

		std::vector<std::wstring> arNodes;

		for (std::vector<CNode>::const_reverse_iterator oNode = arSelectors.rbegin() + ((0 != unEnd) ? (arSelectors.size() - unEnd) : 0); oNode != arSelectors.rend() - unStart; ++oNode)
		{
			if (!oNode->m_wsName.empty())
				arNodes.push_back(oNode->m_wsName);

			if (!oNode->m_wsClass.empty())
			{
				if (oNode->m_wsClass.find(L' ') != std::wstring::npos)
				{
					std::vector<std::wstring> arClasses = NS_STATIC_FUNCTIONS::GetWordsW(oNode->m_wsClass, false, L" ");

					arNodes.push_back(std::accumulate(arClasses.begin(), arClasses.end(), std::wstring(),
													  [](std::wstring sRes, const std::wstring& sClass)
														{return sRes += L'.' + sClass + L' ';}));
				}
				else
					arNodes.push_back(L'.' + oNode->m_wsClass);
			}

			if (!oNode->m_wsId.empty())
				arNodes.push_back(L'#' + oNode->m_wsId);
		}

		return arNodes;
	}

	void CCssCalculator_Private::FindPrevAndKindElements(const CElement *pElement, const std::vector<std::wstring> &arNextNodes, std::vector<const CElement*>& arFindedElements, const std::wstring &wsName, const std::vector<std::wstring> &arClasses)
	{
		if (arNextNodes.empty())
			return;

		const std::vector<CElement*> arTempPrev = pElement->GetPrevElements(arNextNodes.cbegin(), arNextNodes.cend());
		const std::vector<CElement*> arTempKins = pElement->GetNextOfKin(wsName, arClasses);

		if (!arTempPrev.empty())
			arFindedElements.insert(arFindedElements.end(), arTempPrev.begin(), arTempPrev.end());

		if (!arTempKins.empty())
			arFindedElements.insert(arFindedElements.end(), arTempKins.begin(), arTempKins.end());
	}

	std::vector<const CElement*> CCssCalculator_Private::FindElements(std::vector<std::wstring> &arNodes, std::vector<std::wstring> &arNextNodes)
	{
		if (arNodes.empty())
			return {};

		std::vector<const CElement*> arFindedElements;

		std::wstring wsName, wsClasses, wsId;
		std::vector<std::wstring> arClasses;

		if (!arNodes.empty() && arNodes.back()[0] == L'#')
		{
			wsId = arNodes.back();
			arNodes.pop_back();
		}

		if (!arNodes.empty() && arNodes.back()[0] == L'.')
		{
			wsClasses = arNodes.back();
			arClasses = NS_STATIC_FUNCTIONS::GetWordsW(wsClasses, false, L" ");
			arNodes.pop_back();
		}

		if (!arNodes.empty())
		{
			wsName = arNodes.back();
			arNodes.pop_back();
		}

		if (!wsId.empty())
		{
			const CElement* pFoundId = m_oStyleStorage.FindElement(wsId);

			if(nullptr != pFoundId)
			{
				if (!pFoundId->Empty())
					arFindedElements.push_back(pFoundId);

				FindPrevAndKindElements(pFoundId, arNextNodes, arFindedElements, wsName);
			}
		}

		if (!arClasses.empty())
		{
			for (std::vector<std::wstring>::const_reverse_iterator iClass = arClasses.rbegin(); iClass != arClasses.rend(); ++iClass)
			{
				const CElement* pFoundClass = m_oStyleStorage.FindElement(*iClass);

				if (nullptr != pFoundClass)
				{
					if (!pFoundClass->Empty())
						arFindedElements.push_back(pFoundClass);

					FindPrevAndKindElements(pFoundClass, arNextNodes, arFindedElements, wsName);
				}
			}
		}

		const CElement* pFoundName = m_oStyleStorage.FindElement(wsName);

		if (nullptr != pFoundName)
		{
			if (!pFoundName->Empty())
				arFindedElements.push_back(pFoundName);

			FindPrevAndKindElements(pFoundName, arNextNodes, arFindedElements, wsName, arClasses);
		}

		const CElement* pFoundAll = m_oStyleStorage.FindElement(L"*");

		if (nullptr != pFoundAll)
		{
			if (!pFoundAll->Empty())
				arFindedElements.push_back(pFoundAll);

			FindPrevAndKindElements(pFoundAll, arNextNodes, arFindedElements, wsName, arClasses);
		}

		if (arFindedElements.size() > 1)
		{
			std::sort(arFindedElements.rbegin(), arFindedElements.rend(),
			          [](const CElement* oFirstElement, const CElement* oSecondElement)
			          { return oFirstElement->GetWeight() > oSecondElement->GetWeight(); });
		}

		if (!wsId.empty())
			arNextNodes.push_back(wsId);

		if (!wsClasses.empty())
			arNextNodes.push_back(wsClasses);

		arNextNodes.push_back(wsName);

		return arFindedElements;
	}

	#ifdef CSS_CALCULATOR_WITH_XHTML
	std::wstring CCssCalculator_Private::CalculateStyleId(const CNode& oNode)
	{
		return oNode.m_wsName + ((!oNode.m_wsClass.empty()) ? L'.' + oNode.m_wsClass : L"") + ((oNode.m_wsId.empty()) ? L"" : L'#' + oNode.m_wsId) + L'-' + std::to_wstring(++m_nCountNodes);
	}

	bool CCssCalculator_Private::CalculatePageStyle(NSProperties::CPage &oPageData, const std::vector<CNode> &arSelectors)
	{
		if (arSelectors.empty())
			return false;

		std::vector<std::wstring> arNodes = CalculateAllNodes(arSelectors, 0, arSelectors.size());
		std::vector<std::wstring> arNextNodes;

		for (size_t i = 0; i < arSelectors.size(); ++i)
		{
			if (!arSelectors[i].m_wsStyle.empty() && std::wstring::npos != arSelectors[i].m_wsStyle.find(L"page"))
			{
				std::map<std::wstring, std::wstring> mRules = NS_STATIC_FUNCTIONS::GetRules(arSelectors[i].m_wsStyle);
				if (mRules.end() != mRules.find(L"page"))
					SetPageData(oPageData, GetPageData(mRules[L"page"]), i + 1, true);
			}

			for (const CElement* oElement : FindElements(arNodes, arNextNodes))
			{
				std::map<std::wstring, std::wstring> mRules = oElement->GetStyle();
				if (mRules.end() != mRules.find(L"page"))
					SetPageData(oPageData, GetPageData(mRules[L"page"]), i + 1, true);
			}

			if (arSelectors[i].m_mAttributes.end() != arSelectors[i].m_mAttributes.find(L"page"))
				SetPageData(oPageData, GetPageData(arSelectors[i].m_mAttributes.at(L"page")), i + 1, false);
		}
		
		return true;
	}
	#endif

	void CCssCalculator_Private::AddStyles(const std::string& sStyle)
	{
		m_oStyleStorage.AddStyles(sStyle);
	}

	void CCssCalculator_Private::AddStyles(const std::wstring& wsStyle)
	{
		m_oStyleStorage.AddStyles(wsStyle);
	}

	void CCssCalculator_Private::AddStylesFromFile(const std::wstring& wsFileName)
	{
		m_oStyleStorage.AddStylesFromFile(wsFileName);
	}

	void CCssCalculator_Private::SetDpi(unsigned short int nValue)
	{
		m_nDpi = nValue;
	}

	unsigned short int CCssCalculator_Private::GetDpi() const
	{
		return m_nDpi;
	}

	bool CCssCalculator_Private::HaveStylesById(const std::wstring& wsId) const
	{
		return nullptr != m_oStyleStorage.FindElement(L'#' + wsId);
	}

	void CCssCalculator_Private::ClearEmbeddedStyles()
	{
		m_oStyleStorage.ClearEmbeddedStyles();

		#ifdef CSS_CALCULATOR_WITH_XHTML
		m_mUsedStyles.clear();
		#endif
	}

	void CCssCalculator_Private::ClearAllowedStyleFiles()
	{
		m_oStyleStorage.ClearAllowedStyleFiles();
	}

	void CCssCalculator_Private::ClearStylesFromFile(const std::wstring& wsFilePath)
	{
		m_oStyleStorage.ClearStylesFromFile(wsFilePath);
	}

	std::wstring CCssCalculator_Private::GetEncoding() const
	{
		return m_sEncoding;
	}

	void CCssCalculator_Private::Clear()
	{
		m_sEncoding     = L"UTF-8";
		m_nDpi          = 96;

		m_oStyleStorage.Clear();

		#ifdef CSS_CALCULATOR_WITH_XHTML
		m_mUsedStyles.clear();
		#endif
	}

	bool IsTableElement(const std::wstring& wsNameTag)
	{
		return  L"td" == wsNameTag || L"tr" == wsNameTag || L"table" == wsNameTag ||
		        L"tbody" == wsNameTag || L"thead" == wsNameTag || L"tfoot" == wsNameTag ||
		        L"th" == wsNameTag;
	}
}

inline static std::wstring StringifyValueList(const KatanaArray* oValues)
{
	if (NULL == oValues)
		return std::wstring();

	std::wstring buffer;

	for (size_t i = 0; i < oValues->length; ++i)
	{
		KatanaValue* value = (KatanaValue*)oValues->data[i];
		buffer += StringifyValue(value);

		if ( i < oValues->length - 1 && value->unit != KATANA_VALUE_PARSER_OPERATOR )
		{
			if ( i < oValues->length - 2 )
			{
				value = (KatanaValue*)oValues->data[i + 1];
				if ( value->unit != KATANA_VALUE_PARSER_OPERATOR )
					buffer += L" ";
			}
			buffer += L" ";
		}
	}

	return buffer;
}

inline static std::wstring StringifyValue(const KatanaValue* oValue)
{
	std::wstring str;

	switch (oValue->unit) {
		case KATANA_VALUE_NUMBER:
		case KATANA_VALUE_PERCENTAGE:
		case KATANA_VALUE_EMS:
		case KATANA_VALUE_EXS:
		case KATANA_VALUE_REMS:
		case KATANA_VALUE_CHS:
		case KATANA_VALUE_PX:
		case KATANA_VALUE_CM:
		case KATANA_VALUE_DPPX:
		case KATANA_VALUE_DPI:
		case KATANA_VALUE_DPCM:
		case KATANA_VALUE_MM:
		case KATANA_VALUE_IN:
		case KATANA_VALUE_PT:
		case KATANA_VALUE_PC:
		case KATANA_VALUE_DEG:
		case KATANA_VALUE_RAD:
		case KATANA_VALUE_GRAD:
		case KATANA_VALUE_MS:
		case KATANA_VALUE_S:
		case KATANA_VALUE_HZ:
		case KATANA_VALUE_KHZ:
		case KATANA_VALUE_TURN:
			str = UTF8_TO_U(std::string(oValue->raw));
			break;
		case KATANA_VALUE_IDENT:
			str = UTF8_TO_U(std::string(oValue->string));
			break;
		case KATANA_VALUE_STRING:
		{
			str = L"\"" + UTF8_TO_U(std::string(oValue->string)) + L"\"";
			break;
		}
		case KATANA_VALUE_PARSER_FUNCTION:
		{
			const std::wstring& args_str = StringifyValueList(oValue->function->args);
			if (args_str.empty())
				break;

			str = UTF8_TO_U(std::string(oValue->function->name)) + args_str + L")";
			break;
		}
		case KATANA_VALUE_PARSER_OPERATOR:
			str = L" ";
			if (oValue->iValue != '=')
			{
				str += static_cast<wchar_t>(oValue->iValue);
				str += L" ";
			}
			else
				str += static_cast<wchar_t>(oValue->iValue);
			break;
		case KATANA_VALUE_PARSER_LIST:
			return StringifyValueList(oValue->list);
			break;
		case KATANA_VALUE_PARSER_HEXCOLOR:
			str = L"#" + UTF8_TO_U(std::string(oValue->string));
			break;
		case KATANA_VALUE_URI:
			str = L"url(" + UTF8_TO_U(std::string(oValue->string)) + L")";
			break;
		default:
			break;
	}

	return str;
}

inline static bool IsTableElement(const std::wstring& wsNameTag)
{
	return  L"td" == wsNameTag || L"tr" == wsNameTag || L"table" == wsNameTag ||
	        L"tbody" == wsNameTag || L"thead" == wsNameTag || L"tfoot" == wsNameTag ||
	        L"th" == wsNameTag;
}



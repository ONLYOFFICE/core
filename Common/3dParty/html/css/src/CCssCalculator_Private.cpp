#include "CCssCalculator_Private.h"

#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "../../katana-parser/src/selector.h"
#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "ConstValues.h"
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
	CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_nCountNodes(0), m_UnitMeasure(Point), m_mStatictics(NULL), m_sEncoding(L"UTF-8"){}

	CCssCalculator_Private::~CCssCalculator_Private()
	{
		m_arFiles.clear();

		for (std::map<std::wstring, CElement*>::iterator oIter = m_mData.begin(); oIter != m_mData.end(); ++oIter)
			if (oIter->second != NULL)
				delete oIter->second;

		m_mData.clear();

		if (NULL != m_mStatictics)
			delete m_mStatictics;
	}

	inline void CCssCalculator_Private::GetOutputData(KatanaOutput *oOutput)
	{
		if ( NULL == oOutput )
			return;

		switch (oOutput->mode) {
			case KatanaParserModeStylesheet:
				GetStylesheet(oOutput->stylesheet);
				break;
			case KatanaParserModeRule:
				GetRule(oOutput->rule);
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

	#ifdef CSS_CALCULATOR_WITH_XHTML
	std::map<std::wstring, std::wstring> CCssCalculator_Private::GetPageData(const std::wstring &wsPageName)
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
	
	void CCssCalculator_Private::SetPageData(NSProperties::CPage &oPage, const std::map<std::wstring, std::wstring> &mData, unsigned int unLevel, bool bHardMode)
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

	std::vector<std::wstring> CCssCalculator_Private::CalculateAllNodes(const std::vector<CNode> &arSelectors)
	{
		std::vector<std::wstring> arNodes;
		
		for (std::vector<CNode>::const_reverse_iterator oNode = arSelectors.rbegin(); oNode != arSelectors.rend(); ++oNode)
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

	void CCssCalculator_Private::FindPrevAndKindElements(const CElement *pElement, const std::vector<std::wstring> &arNextNodes, std::vector<CElement*>& arFindedElements, const std::wstring &wsName, const std::vector<std::wstring> &arClasses)
	{
		if (arNextNodes.empty())
			return;

		const std::vector<CElement*> arTempPrev = pElement->GetPrevElements(arNextNodes.crbegin() + 1, arNextNodes.crend());
		const std::vector<CElement*> arTempKins = pElement->GetNextOfKin(wsName, arClasses);

		if (!arTempPrev.empty())
			arFindedElements.insert(arFindedElements.end(), arTempPrev.begin(), arTempPrev.end());

		if (!arTempKins.empty())
			arFindedElements.insert(arFindedElements.end(), arTempKins.begin(), arTempKins.end());
	}

	std::vector<CElement*> CCssCalculator_Private::FindElements(std::vector<std::wstring> &arNodes, std::vector<std::wstring> &arNextNodes, bool bIsSettings)
	{
		if (arNodes.empty())
			return {};

		std::vector<CElement*> arFindedElements;

		std::wstring wsName, wsId;
		std::vector<std::wstring> arClasses;

		if (!arNodes.empty() && arNodes.back()[0] == L'#')
		{
			wsId = arNodes.back();
			arNodes.pop_back();
			arNextNodes.push_back(wsId);
		}

		if (!arNodes.empty() && arNodes.back()[0] == L'.')
		{
			arClasses = NS_STATIC_FUNCTIONS::GetWordsW(arNodes.back(), false, L" ");
			arNextNodes.push_back(arNodes.back());
			arNodes.pop_back();
		}

		if (!arNodes.empty())
		{
			wsName = arNodes.back();
			arNodes.pop_back();
			arNextNodes.push_back(wsName);
		}

		const std::map<std::wstring, CElement*>::const_iterator oFindName = m_mData.find(wsName);
		std::map<std::wstring, CElement*>::const_iterator oFindId;

		if (!wsId.empty())
		{
			oFindId = m_mData.find(wsId);

			if (oFindId != m_mData.end() && NULL != m_mStatictics)
			{
				std::map<StatistickElement, unsigned int>::const_iterator oFindCountId = m_mStatictics->find(StatistickElement{StatistickElement::IsId, wsId});

				if ((m_mStatictics->end() != oFindCountId) &&
				   (((bIsSettings && oFindCountId->second < MaxNumberRepetitions) ||
					 (!bIsSettings && oFindCountId->second >= MaxNumberRepetitions))))
				{
					if (!oFindId->second->Empty())
						arFindedElements.push_back(oFindId->second);
				}

				FindPrevAndKindElements(oFindId->second, arNextNodes, arFindedElements, wsName);
			}
		}

		if (!arClasses.empty())
		{
			if (!bIsSettings)
			{
				for (std::vector<std::wstring>::const_reverse_iterator iClass = arClasses.rbegin(); iClass != arClasses.rend(); ++iClass)
				{
					const std::map<std::wstring, CElement*>::const_iterator oFindClass = m_mData.find(*iClass);
					if (oFindClass != m_mData.end())
					{
						if (!oFindClass->second->Empty())
							arFindedElements.push_back(oFindClass->second);

						FindPrevAndKindElements(oFindClass->second, arNextNodes, arFindedElements, wsName);
					}
				}
			}
		}

		if (oFindName != m_mData.end())
		{
			if (!bIsSettings)
			{
				if (!oFindName->second->Empty())
					arFindedElements.push_back(oFindName->second);

				FindPrevAndKindElements(oFindName->second, arNextNodes, arFindedElements, wsName, arClasses);
			}
		}

		if (arFindedElements.size() > 1)
		{
			std::sort(arFindedElements.rbegin(), arFindedElements.rend(),
					  [](CElement* oFirstElement, CElement* oSecondElement)
					  {
						  return oFirstElement->GetWeight() > oSecondElement->GetWeight();
					  });
		}

		return arFindedElements;
	}
	#endif

	void CCssCalculator_Private::AddPageData(const std::wstring &wsPageNames, const std::wstring &wsStyles)
	{
		m_arPageDatas.push_back({NS_STATIC_FUNCTIONS::GetWordsW(wsPageNames), NS_STATIC_FUNCTIONS::GetRules(wsStyles)});
	}
	
	inline void CCssCalculator_Private::GetStylesheet(const KatanaStylesheet *oStylesheet)
	{
		for (size_t i = 0; i < oStylesheet->imports.length; ++i)
			GetRule((KatanaRule*)oStylesheet->imports.data[i]);

		for (size_t i = 0; i < oStylesheet->rules.length; ++i)
			GetRule((KatanaRule*)oStylesheet->rules.data[i]);
	}

	inline void CCssCalculator_Private::GetRule(const KatanaRule *oRule)
	{
		if ( NULL == oRule )
			return;

		switch (oRule->type) {
			case KatanaRuleStyle:
			{
				GetStyleRule((KatanaStyleRule*)oRule);
				break;
			}
			default:
				break;
		}
	}

	inline void CCssCalculator_Private::GetStyleRule(const KatanaStyleRule *oRule)
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
							const std::map<std::wstring, CElement*>::const_iterator& oFindId = m_mData.find(sId);
							if (oFindId != m_mData.end())
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
							const std::map<std::wstring, CElement*>::const_iterator& oFindClass = m_mData.find(sClass);
							if (oFindClass != m_mData.end())
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
							const std::map<std::wstring, CElement*>::const_iterator& oFindName = m_mData.find(sName);
							if (oFindName != m_mData.end())
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
				m_mData[oFirstElement->GetSelector()] = oFirstElement;
		}
	}

	inline std::vector<std::wstring> CCssCalculator_Private::GetSelectorList(const KatanaArray* oSelectors) const
	{
		if (oSelectors->length == 0)
			return std::vector<std::wstring>();

		std::vector<std::wstring> arSelectors;

		for (unsigned int i = 0; i < oSelectors->length; ++i)
			arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));

		return arSelectors;
	}

	inline std::wstring CCssCalculator_Private::GetSelector(const KatanaSelector *oSelector) const
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

	inline std::map<std::wstring, std::wstring> CCssCalculator_Private::GetDeclarationList(const KatanaArray* oDeclarations) const
	{
		if(oDeclarations->length == 0)
			return std::map<std::wstring, std::wstring>();

		std::map<std::wstring, std::wstring> arDeclarations;

		for (size_t i = 0; i < oDeclarations->length; ++i)
			arDeclarations.insert(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));

		return arDeclarations;
	}

	inline std::pair<std::wstring, std::wstring> CCssCalculator_Private::GetDeclaration(const KatanaDeclaration* oDecl) const
	{
		std::wstring sValueList = StringifyValueList(oDecl->values);

		if (oDecl->important)
			sValueList += L" !important";

		return std::make_pair(UTF8_TO_U(std::string(oDecl->property)), sValueList);
	}

	inline std::wstring CCssCalculator_Private::GetValueList(const KatanaArray *oValues)
	{
		return StringifyValueList(oValues);
	}

	#ifdef CSS_CALCULATOR_WITH_XHTML
	CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode>& arSelectors, const bool& bIsSettings, const UnitMeasure& unitMeasure)
	{
		if (arSelectors.empty())
			return CCompiledStyle();

		CCompiledStyle oStyle;

		GetCompiledStyle(oStyle, arSelectors, bIsSettings, unitMeasure);

		return oStyle;
	}

	bool CCssCalculator_Private::GetCompiledStyle(CCompiledStyle &oStyle, const std::vector<CNode> &arSelectors, const bool &bIsSettings, const UnitMeasure &unitMeasure)
	{
		if (arSelectors.empty())
			return false;

		SetUnitMeasure(unitMeasure);

		if (!bIsSettings)
		{
			const std::map<std::vector<CNode>, CCompiledStyle>::iterator oItem = m_mUsedStyles.find(arSelectors);

			if (oItem != m_mUsedStyles.end())
			{
				oStyle = oItem->second;
				return true;
			}
		}
		else if (NULL == m_mStatictics || m_mStatictics->empty())
		{
			oStyle.SetDpi(m_nDpi);
			oStyle.SetUnitMeasure(m_UnitMeasure);
			oStyle.SetID(arSelectors.back().m_wsName + ((!arSelectors.back().m_wsClass.empty()) ? L'.' + arSelectors.back().m_wsClass : L"") + ((arSelectors.back().m_wsId.empty()) ? L"" : L'#' + arSelectors.back().m_wsId) + L'-' + std::to_wstring(++m_nCountNodes));

			return false;
		}

		oStyle.SetDpi(m_nDpi);
		oStyle.SetUnitMeasure(m_UnitMeasure);

		std::vector<std::wstring> arNodes = CalculateAllNodes(arSelectors);
		std::vector<std::wstring> arPrevNodes;
		bool bInTable = false;
		
		for (size_t i = 0; i < arSelectors.size(); ++i)
		{
			oStyle.AddParent(arSelectors[i].m_wsName);

			if (!bInTable)
				bInTable = IsTableElement(arSelectors[i].m_wsName);

			if (bInTable)
			{
				oStyle.m_oBackground.Clear();
				oStyle.m_oBorder.Clear();
			}

			CCompiledStyle oTempStyle;

			oTempStyle.AddStyle(arSelectors[i].m_mAttributes, i + 1);

			for (const CElement* oElement : FindElements(arNodes, arPrevNodes, bIsSettings))
				oTempStyle.AddStyle(oElement->GetStyle(), i + 1);

			if (NULL != m_mStatictics)
			{
				std::map<StatistickElement, unsigned int>::const_iterator oFindCountStyle = m_mStatictics->find(StatistickElement{StatistickElement::IsStyle, arSelectors[i].m_wsStyle});

				if (oFindCountStyle != m_mStatictics->end())
				{
					if ((bIsSettings && oFindCountStyle->second <  MaxNumberRepetitions) ||
					   (!bIsSettings && oFindCountStyle->second >= MaxNumberRepetitions))
						oTempStyle.AddStyle(arSelectors[i].m_wsStyle, i + 1,  true);
					else if (!bIsSettings)
						oTempStyle.AddStyle(arSelectors[i].m_wsStyle, i + 1, true);
				}
				else /*if (bIsSettings)*/
					oTempStyle.AddStyle(arSelectors[i].m_wsStyle, i + 1, true);
			}
			else
				oTempStyle.AddStyle(arSelectors[i].m_wsStyle, i + 1, true);

			oStyle += oTempStyle;

			// Скидываем некоторые внешние стили, которые внутри таблицы переопределяются
			if (bInTable && i < arSelectors.size() - 1)
			{
				oStyle.m_oFont.GetLineHeight().Clear();
				oStyle.m_oPadding.Clear();
				oStyle.m_oMargin.Clear();
			}
		}

		oStyle.SetID(CalculateStyleId(arSelectors.back()));

		if (!bIsSettings && !oStyle.Empty())
			m_mUsedStyles[arSelectors] = oStyle;

		return true;
	}

	std::wstring CCssCalculator_Private::CalculateStyleId(const CNode& oNode)
	{
		return oNode.m_wsName + ((!oNode.m_wsClass.empty()) ? L'.' + oNode.m_wsClass : L"") + ((oNode.m_wsId.empty()) ? L"" : L'#' + oNode.m_wsId) + L'-' + std::to_wstring(++m_nCountNodes);
	}

	bool CCssCalculator_Private::CalculatePageStyle(NSProperties::CPage &oPageData, const std::vector<CNode> &arSelectors)
	{
		if (arSelectors.empty())
			return false;

		std::vector<std::wstring> arNodes = CalculateAllNodes(arSelectors);
		std::vector<std::wstring> arNextNodes;

		for (size_t i = 0; i < arSelectors.size(); ++i)
		{
			if (!arSelectors[i].m_wsStyle.empty() && std::wstring::npos != arSelectors[i].m_wsStyle.find(L"page"))
			{
				std::map<std::wstring, std::wstring> mRules = NS_STATIC_FUNCTIONS::GetRules(arSelectors[i].m_wsStyle);
				if (mRules.end() != mRules.find(L"page"))
					SetPageData(oPageData, GetPageData(mRules[L"page"]), i + 1, true);
			}

			for (const CElement* oElement : FindElements(arNodes, arNextNodes, false))
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
	void CCssCalculator_Private::AddStyles(const std::string &sStyle)
	{
		if (sStyle.empty())
			return;

		KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.length(), KatanaParserModeStylesheet);
		this->GetOutputData(output);
		katana_destroy_output(output);
	}

	void CCssCalculator_Private::AddStyles(const std::wstring &wsStyle)
	{
		if (wsStyle.empty())
			return;

		std::wregex oRegex(L"@page\\s*([^{]*)(\\{[^}]*\\})");
		std::wsmatch oMatch;
		std::wstring::const_iterator oSearchStart(wsStyle.cbegin());
		
		while (std::regex_search(oSearchStart, wsStyle.cend(), oMatch, oRegex))
		{
			AddPageData(oMatch[1].str(), oMatch[2].str());
			oSearchStart = oMatch.suffix().first;
		}

		AddStyles(U_TO_UTF8(wsStyle));
	}

	void CCssCalculator_Private::AddStylesFromFile(const std::wstring& wsFileName)
	{
		if (std::find(m_arFiles.begin(), m_arFiles.end(), wsFileName) != m_arFiles.end())
			return;

		m_arFiles.push_back(wsFileName);

		AddStyles(NS_STATIC_FUNCTIONS::GetContentAsUTF8(wsFileName));
	}

	void CCssCalculator_Private::SetDpi(unsigned short int nValue)
	{
		m_nDpi = nValue;
	}

	void CCssCalculator_Private::SetBodyTree(const CTree &oTree)
	{
		if (NULL == m_mStatictics)
			m_mStatictics = new std::map<StatistickElement, unsigned int>();

		CTree::CountingNumberRepetitions(oTree, *m_mStatictics);
	}

	void CCssCalculator_Private::SetUnitMeasure(const UnitMeasure& nType)
	{
		m_UnitMeasure = nType;
	}

	unsigned short int CCssCalculator_Private::GetDpi() const
	{
		return m_nDpi;
	}

	const std::map<std::wstring, CElement *> *CCssCalculator_Private::GetData() const
	{
		return &m_mData;
	}

	UnitMeasure CCssCalculator_Private::GetUnitMeasure() const
	{
		return m_UnitMeasure;
	}

	std::wstring CCssCalculator_Private::GetEncoding() const
	{
		return m_sEncoding;
	}

	void CCssCalculator_Private::Clear()
	{
		m_sEncoding     = L"UTF-8";
		m_nDpi          = 96;
		m_UnitMeasure   = Point;

		m_mData.clear();
		m_arFiles.clear();
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



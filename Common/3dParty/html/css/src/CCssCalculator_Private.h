#ifndef CCSSCALCULATOR_PRIVATE_H
#define CCSSCALCULATOR_PRIVATE_H

#include <vector>
#include <map>
#include <list>
#include <functional>
#include "CElement.h"
#include "ConstValues.h"
#include "CUnitMeasureConverter.h"
#include "../../katana-parser/src/katana.h"

#ifdef CSS_CALCULATOR_WITH_XHTML
#include "CCompiledStyle.h"
#endif

namespace NSCSS
{
	class CStyleStorage
	{
	public:
		CStyleStorage();
		~CStyleStorage();

		void Clear();

		void AddStyles(const std::string& sStyle);
		void AddStyles(const std::wstring& wsStyle);
		void AddStylesFromFile(const std::wstring& wsFileName);
		void AddPageData(const std::wstring& wsPageName, const std::wstring& wsStyles);

		void ClearPageData();
		void ClearEmbeddedStyles();
		void ClearAllowedStyleFiles();
		void ClearStylesFromFile(const std::wstring& wsFileName);

		void SetPageData(NSProperties::CPage& oPage, const std::map<std::wstring, std::wstring>& mData, unsigned int unLevel, bool bHardMode = false);

		std::map<std::wstring, std::wstring> GetPageData(const std::wstring& wsPageName);

		const CElement* FindElement(const std::wstring& wsSelector);
	private:
		typedef struct
		{
			std::wstring m_wsStyleFilepath;
			std::map<std::wstring, CElement*> m_mStyleData;
		} TStyleFileData;

		std::set<std::wstring> m_arEmptyStyleFiles;
		std::set<std::wstring> m_arAllowedStyleFiles;
		std::vector<TStyleFileData*> m_arStyleFiles;
		std::map<std::wstring, CElement*> m_mEmbeddedStyleData;

		typedef struct
		{
			std::vector<std::wstring>            m_wsNames;
			std::map<std::wstring, std::wstring> m_mData;
		} TPageData;

		std::vector<TPageData> m_arPageDatas;
	private:
		void AddStyles(const std::string& sStyle, std::map<std::wstring, CElement*>& mStyleData);

		void GetStylesheet(const KatanaStylesheet* oStylesheet, std::map<std::wstring, CElement*>& mStyleData);
		void GetRule(const KatanaRule* oRule, std::map<std::wstring, CElement*>& mStyleData);

		void GetStyleRule(const KatanaStyleRule* oRule, std::map<std::wstring, CElement*>& mStyleData);

		std::wstring GetValueList(const KatanaArray* oValues);

		std::vector<std::wstring> GetSelectorList(const KatanaArray* oSelectors) const;
		std::wstring GetSelector(const KatanaSelector* oSelector) const;

		std::map<std::wstring, std::wstring> GetDeclarationList(const KatanaArray* oDeclarations) const;
		std::pair<std::wstring, std::wstring> GetDeclaration(const KatanaDeclaration* oDecl) const;

		void GetOutputData(KatanaOutput* oOutput, std::map<std::wstring, CElement*>& mStyleData);

		const CElement* FindSelectorFromStyleData(const std::wstring& wsSelector, const std::map<std::wstring, CElement*>& mStyleData);
	};

	class CCssCalculator_Private
	{
		unsigned short int m_nDpi;
		unsigned short int m_nCountNodes;

		CStyleStorage m_oStyleStorage;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		std::map<std::vector<CNode>, CCompiledStyle> m_mUsedStyles;
		
		std::map<std::wstring, std::wstring> GetPageData(const std::wstring& wsPageName);
		void SetPageData(NSProperties::CPage& oPage, const std::map<std::wstring, std::wstring>& mData, unsigned int unLevel, bool bHardMode = false);

		std::vector<std::wstring> CalculateAllNodes(const std::vector<CNode>& arSelectors);

		void FindPrevAndKindElements(const CElement* pElement, const std::vector<std::wstring>& arNextNodes, std::vector<const CElement*>& arFindedElements, const std::wstring& wsName, const std::vector<std::wstring>& arClasses = {});
		std::vector<const CElement*> FindElements(std::vector<std::wstring>& arNodes, std::vector<std::wstring>& arNextNodes);
		#endif

		std::wstring m_sEncoding;

	public:
		CCssCalculator_Private();
		~CCssCalculator_Private();

		#ifdef CSS_CALCULATOR_WITH_XHTML
		CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors);
		bool GetCompiledStyle(CCompiledStyle& oStyle, const std::vector<CNode> &arSelectors);

		std::wstring CalculateStyleId(const CNode& oNode);
		bool CalculatePageStyle(NSProperties::CPage& oPageData, const std::vector<CNode> &arSelectors);
		#endif

		void AddStyles(const std::string& sStyle);
		void AddStyles(const std::wstring& wsStyle);
		void AddStylesFromFile(const std::wstring& wsFileName);

		void SetDpi(unsigned short int nValue);

		std::wstring GetEncoding() const;
		unsigned short int GetDpi() const;

		void ClearPageData();
		void ClearEmbeddedStyles();
		void ClearAllowedStyleFiles();
		void ClearStylesFromFile(const std::wstring& wsFilePath);
		void Clear();

	};
}
#endif // CCSSCALCULATOR_PRIVATE_H

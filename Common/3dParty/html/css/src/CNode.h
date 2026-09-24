#ifndef CNODE_H
#define CNODE_H

#include <string>
#include <vector>
#include <map>

namespace NSCSS
{
	#ifdef CSS_CALCULATOR_WITH_XHTML
	class CCompiledStyle;
	#endif
	class CNode
	{
	public:
		std::wstring m_wsName;  // Tag name
		std::wstring m_wsClass; // Tag class
		std::wstring m_wsId;    // Tag id
		std::wstring m_wsStyle; // Tag style
		std::map<std::wstring, std::wstring> m_mAttributes; // Other tag attributes
		//TODO:: perhaps using std::wstring is excessive

		#ifdef CSS_CALCULATOR_WITH_XHTML
		CCompiledStyle *m_pCompiledStyle;
		#endif
	public:
		CNode();
		CNode(const CNode& oNode);
		CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId);
		~CNode();

		bool Empty() const;

		bool GetAttributeValue(const std::wstring& wsAttributeName, std::wstring& wsAttributeValue) const;
		std::wstring GetAttributeValue(const std::wstring& wsAttributeName) const;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		void SetCompiledStyle(const CCompiledStyle* pCompiledStyle);
		#endif

		void Clear();

		std::vector<std::wstring> GetData() const;
		bool operator< (const CNode& oNode) const;
		bool operator== (const CNode& oNode) const;
	};
}

#endif // CNODE_H

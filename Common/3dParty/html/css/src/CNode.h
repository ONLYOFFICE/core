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
		std::wstring m_wsName;  // Имя тэга
		std::wstring m_wsClass; // Класс тэга
		std::wstring m_wsId;    // Id тэга
		std::wstring m_wsStyle; // Стиль тэга
		std::map<std::wstring, std::wstring> m_mAttributes; // Остальные аттрибуты тэга

		#ifdef CSS_CALCULATOR_WITH_XHTML
		CCompiledStyle *m_pCompiledStyle;
		#endif
	public:
		CNode();
		CNode(const CNode& oNode);
		CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId);
		~CNode();

		bool Empty() const;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		void SetCompiledStyle(CCompiledStyle* pCompiledStyle);
		#endif

		void Clear();

		std::vector<std::wstring> GetData() const;
		bool operator< (const CNode& oNode) const;
		bool operator== (const CNode& oNode) const;
	};
}

#endif // CNODE_H

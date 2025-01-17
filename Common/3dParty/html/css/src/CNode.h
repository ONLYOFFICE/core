#ifndef CNODE_H
#define CNODE_H

#include <string>
#include <vector>
#include <map>

namespace NSCSS
{
	class CCompiledStyle;
	class CNode
	{
	public:
		std::wstring m_wsName;  // Имя тэга
		std::wstring m_wsClass; // Класс тэга
		std::wstring m_wsId;    // Id тэга
		std::wstring m_wsStyle; // Стиль тэга
		std::map<std::wstring, std::wstring> m_mAttributes; // Остальные аттрибуты тэга

		CCompiledStyle *m_pCompiledStyle;
	public:
		CNode();
		CNode(const CNode& oNode);
		CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId);
		~CNode();

		bool Empty() const;

		void SetCompiledStyle(CCompiledStyle* pCompiledStyle);

		void Clear();

		std::vector<std::wstring> GetData() const;
		bool operator< (const CNode& oNode) const;
		bool operator== (const CNode& oNode) const;
	};
}

#endif // CNODE_H

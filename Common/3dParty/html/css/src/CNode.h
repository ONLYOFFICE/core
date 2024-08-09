#ifndef CNODE_H
#define CNODE_H

#include <string>
#include <vector>
#include <map>

namespace NSCSS
{
	class CNode
	{
	public:
		std::wstring m_wsName;  // Имя тэга
		std::wstring m_wsClass; // Класс тэга
		std::wstring m_wsId;    // Id тэга
		std::wstring m_wsStyle; // Стиль тэга
		std::map<std::wstring, std::wstring> m_mAttributes; // Остальные аттрибуты тэга

	public:
		CNode();
		CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId);

		bool Empty() const;

		std::vector<std::wstring> GetData() const;
		bool operator< (const CNode& oNode) const;
		bool operator== (const CNode& oNode) const;
	};
}

#endif // CNODE_H

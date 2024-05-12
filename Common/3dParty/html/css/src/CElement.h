#ifndef CELEMENT_H
#define CELEMENT_H

#include <map>
#include <vector>
#include <string>
#include "CNode.h"

namespace NSCSS
{
	class CElement
	{
		std::map<std::wstring, std::wstring> m_mStyle;

		std::vector<CElement*> m_arPrevElements;
		std::vector<CElement*> m_arKinElements;

		std::wstring m_sSelector;
		std::wstring m_sFullSelector;

		std::vector<unsigned short int> m_arWeight;

	public:
		CElement();
		~CElement();

		std::wstring GetSelector() const;
		std::wstring GetFullSelector() const;

		bool Empty() const;

		void SetSelector(const std::wstring& sSelector);
		void AddPropertie(const std::wstring& sName, const std::wstring& sValue);
		void AddProperties(const std::map<std::wstring, std::wstring>& mProperties);
		void AddPrevElement(CElement* oPrevElement);
		void AddKinElement(CElement* oKinElement);

		std::map<std::wstring, std::wstring> GetStyle() const;
		std::map<std::wstring, std::wstring> GetFullStyle(const std::vector<CNode>& arSelectors) const;
		std::map<std::wstring, std::wstring> GetFullStyle(const std::vector<std::wstring>& arNodes) const;
		std::vector<CElement *> GetNextOfKin(const std::wstring& sName, const std::vector<std::wstring>& arClasses = {}) const;
		std::vector<CElement *> GetPrevElements(const std::vector<std::wstring>::const_reverse_iterator& oNodesRBegin, const std::vector<std::wstring>::const_reverse_iterator& oNodesREnd) const;
		std::map<std::wstring, std::wstring> GetConvertStyle(const std::vector<CNode>& arNodes) const;

		CElement *FindPrevElement(const std::wstring& sSelector) const;

		std::vector<unsigned short int> GetWeight();
		void IncreasedWeight();
	};
}


#endif // CELEMENT_H

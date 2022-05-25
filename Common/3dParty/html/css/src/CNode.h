#ifndef CNODE_H
#define CNODE_H

#include <string>
#include <vector>
#include <map>

namespace NSCSS
{
    struct CNode
    {
        std::wstring m_sName;  // Имя тэга
        std::wstring m_sClass; // Класс тэга
        std::wstring m_sId;    // Id тэга
        std::wstring m_sStyle; // Стиль тэга
        std::map<std::wstring, std::wstring> m_mAttrs; // Остальные аттрибуты тэга

        CNode(){};
        CNode(std::wstring sName, std::wstring sClass, std::wstring sId) : m_sName(sName), m_sClass(sClass), m_sId(sId){};

        bool Empty() const
        {
            return m_sName.empty() && m_sClass.empty() && m_sId.empty() && m_sStyle.empty();
        }

        std::vector<std::wstring> GetData() const
        {
            std::vector<std::wstring> arValues;
            if (!m_sClass.empty())
                arValues.push_back(m_sClass);
            if (!m_sName.empty())
                arValues.push_back(m_sName);
            return arValues;
        }

        bool operator< (const CNode& oNode) const
        {
            if(m_sName != oNode.m_sName)
                return m_sName < oNode.m_sName;

            if(m_sClass != oNode.m_sClass)
                return m_sClass < oNode.m_sClass;

            if(m_sId != oNode.m_sId)
                return m_sId < oNode.m_sId;

            if(m_sStyle != oNode.m_sStyle)
                return m_sStyle < oNode.m_sStyle;

            return false;
        }

        bool operator== (const CNode& oNode) const
        {
            return((m_sId == oNode.m_sId)       &&
                   (m_sName == oNode.m_sName)   &&
                   (m_sClass == oNode.m_sClass) &&
                   (m_sStyle == oNode.m_sStyle));
        }
    };
}

#endif // CNODE_H

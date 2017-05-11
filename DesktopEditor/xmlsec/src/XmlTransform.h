#ifndef _XML_TRANSFORM_H_
#define _XML_TRANSFORM_H_

#include "./XmlRels.h"

class IXmlTransform
{
protected:
    std::string m_algorithm;

public:
    IXmlTransform()
    {
        m_algorithm = "";
    }
    virtual ~IXmlTransform()
    {
    }

public:
    virtual std::string Transform(const std::string& sXml) = 0;
    virtual void LoadFromXml(XmlUtils::CXmlNode& node) = 0;
};

class CXmlTransformRelationship : public IXmlTransform
{
protected:
    std::map<std::wstring, bool> m_arIds;

public:
    CXmlTransformRelationship() : IXmlTransform()
    {
        m_algorithm = "http://schemas.openxmlformats.org/package/2006/RelationshipTransform";
    }

    virtual std::string Transform(const std::wstring& sFile)
    {
        COOXMLRelationships _rels(sFile, &m_arIds);
        return U_TO_UTF8(_rels.GetXml());
    }

    virtual void LoadFromXml(XmlUtils::CXmlNode& node)
    {
        XmlUtils::CXmlNodes oNodesIds;
        node.GetChilds(oNodesIds);

        int nCount = oNodesIds.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode _node;
            oNodesIds.GetAt(i, _node);

            std::wstring sType = _node.GetAttribute("SourceId");
            if (!sType.empty())
                m_arIds.insert(std::pair<std::wstring, bool>(sType, true));
        }
    }
};

#endif //_XML_TRANSFORM_H_

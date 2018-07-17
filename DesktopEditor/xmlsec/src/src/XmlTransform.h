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

    static IXmlTransform* GetFromType(const std::string& alg);
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

    virtual std::string Transform(const std::string& xml)
    {
        COOXMLRelationships _rels(xml, &m_arIds);
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

class CXmlTransformC14N : public IXmlTransform
{
protected:
    int m_mode;
    bool m_comments;

public:
    CXmlTransformC14N() : IXmlTransform()
    {
        m_mode = -1;
        m_comments = false;
    }

    bool CheckC14NTransform(const std::string& alg)
    {
        m_mode = -1;
        if ("http://www.w3.org/TR/2001/REC-xml-c14n-20010315" == alg)
        {
            m_mode = XmlUtils::XML_C14N_1_0;
            m_comments = false;
        }
        else if ("http://www.w3.org/TR/2001/REC-xml-c14n-20010315#WithComments" == alg)
        {
            m_mode = XmlUtils::XML_C14N_1_0;
            m_comments = true;
        }
        else if ("http://www.w3.org/2006/12/xml-c14n11" == alg)
        {
            m_mode = XmlUtils::XML_C14N_1_1;
            m_comments = false;
        }
        else if ("http://www.w3.org/2006/12/xml-c14n11#WithComments" == alg)
        {
            m_mode = XmlUtils::XML_C14N_1_1;
            m_comments = true;
        }
        else if ("http://www.w3.org/2001/10/xml-exc-c14n#" == alg)
        {
            m_mode = XmlUtils::XML_C14N_EXCLUSIVE_1_0;
            m_comments = false;
        }
        else if ("http://www.w3.org/2001/10/xml-exc-c14n#WithComments" == alg)
        {
            m_mode = XmlUtils::XML_C14N_EXCLUSIVE_1_0;
            m_comments = true;
        }
        return (-1 != m_mode) ? true : false;
    }

    virtual std::string Transform(const std::string& xml)
    {
        if (-1 == m_mode)
            return xml;
        return XmlUtils::NSXmlCanonicalizator::Execute(xml, m_mode, m_comments);
    }

    virtual void LoadFromXml(XmlUtils::CXmlNode& node)
    {
        // none
        XML_UNUSED(node);
    }
};

class CXmlTransforms
{
protected:
    std::vector<IXmlTransform*> m_transforms;
    bool m_valid;

public:
    CXmlTransforms()
    {
        m_valid = true;
    }

    CXmlTransforms(XmlUtils::CXmlNode& node)
    {
        m_valid = true;

        XmlUtils::CXmlNodes oNodes = node.GetNodes(L"Transform");
        int nCount = oNodes.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode nodeTransform;
            oNodes.GetAt(i, nodeTransform);

            IXmlTransform* pTransform = IXmlTransform::GetFromType(nodeTransform.GetAttributeA("Algorithm"));
            if (NULL == pTransform)
            {
                m_valid = false;
                return;
            }

            pTransform->LoadFromXml(nodeTransform);
            m_transforms.push_back(pTransform);
        }
    }

    ~CXmlTransforms()
    {
        for (std::vector<IXmlTransform*>::iterator i = m_transforms.begin(); i != m_transforms.end(); i++)
        {
            IXmlTransform* t = *i;
            RELEASEOBJECT(t);
        }
        m_transforms.clear();
    }

    bool GetValid()
    {
        return m_valid;
    }

    std::string Transform(const std::string& xml)
    {
        std::string sResult = xml;
        for (std::vector<IXmlTransform*>::iterator i = m_transforms.begin(); i != m_transforms.end(); i++)
        {
            sResult = (*i)->Transform(sResult);
        }
        return sResult;
    }

    void AddTransform(IXmlTransform* transform)
    {
        m_transforms.push_back(transform);
    }
};

#endif //_XML_TRANSFORM_H_

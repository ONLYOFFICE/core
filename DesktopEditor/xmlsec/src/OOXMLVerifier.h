#ifndef _XML_OOXMLVERIFIER_H_
#define _XML_OOXMLVERIFIER_H_

#include "./XmlCanonicalizator.h"
#include "./XmlTransform.h"
#include "./XmlCertificate.h"

#define OOXML_SIGNATURE_VALID           0
#define OOXML_SIGNATURE_INVALID         1
#define OOXML_SIGNATURE_NOTSUPPORTED    2
#define OOXML_SIGNATURE_BAD             3

class COOXMLSignature
{
private:
    int             m_valid;
    std::string     m_guid;
    ICertificate*   m_cert;

    std::string     m_sImageValidBase64;
    std::string     m_sImageInvalidBase64;

private:
    XmlUtils::CXmlNode   m_node; // signature file

    class CXmlStackNamespaces
    {
    public:
        std::wstring            m_namespaces;
        XmlUtils::CXmlNode      m_node;

    public:
        CXmlStackNamespaces(const CXmlStackNamespaces& src)
        {
            m_namespaces = src.m_namespaces;
            m_node = src.m_node;
        }
        CXmlStackNamespaces()
        {
        }
        CXmlStackNamespaces(const XmlUtils::CXmlNode& node)
        {
            m_node = node;
        }

        CXmlStackNamespaces& operator=(const CXmlStackNamespaces& src)
        {
            m_namespaces = src.m_namespaces;
            m_node = src.m_node;
            return *this;
        }

        CXmlStackNamespaces GetById(const std::string& id)
        {
            return GetByIdRec(*this, id);
        }

        CXmlStackNamespaces GetByIdRec(CXmlStackNamespaces& stack, const std::string& id)
        {
            if (stack.m_node.GetAttributeA("Id") == id)
                return stack;

            CXmlStackNamespaces ret = stack;

            std::vector<std::wstring> _names;
            std::vector<std::wstring> _values;
            ret.m_node.ReadAllAttributes(_names, _values);

            NSStringUtils::CStringBuilder oBuilder;
            oBuilder.WriteString(L" ");
            for (std::vector<std::wstring>::iterator i = _names.begin(), j = _values.begin(); i != _names.end(); i++, j++)
            {
                if (i->find(L"xmlns") == 0)
                {
                    oBuilder.WriteString(*i);
                    oBuilder.WriteString(L"=\"");
                    oBuilder.WriteEncodeXmlString(*j);
                    oBuilder.WriteString(L"\"");
                }
            }

            if (oBuilder.GetCurSize() != 1)
                ret.m_namespaces += oBuilder.GetData();

            XmlUtils::CXmlNodes oNodes;
            if (stack.m_node.GetChilds(oNodes))
            {
                int nCount = oNodes.GetCount();

                for (int i = 0; i < nCount; i++)
                {
                    CXmlStackNamespaces _retRecursion = GetByIdRec(ret, id);
                    if (_retRecursion.m_node.IsValid())
                        return _retRecursion;
                }
            }

            return CXmlStackNamespaces();
        }

        std::string GetXml()
        {
            std::wstring sXml = m_node.GetXml();
            if (!m_namespaces.empty())
            {
                std::wstring sName = m_node.GetName();

                std::wstring sXmlFind = L"<" + sName + L" ";
                if (0 == sXml.find(sXmlFind))
                    sXml.replace(0, sXmlFind.length(), L"<" + sName + L" " + m_namespaces + L" ");
            }

            return U_TO_UTF8(sXml);
        }
    };

public:
    COOXMLSignature()
    {
        m_valid = OOXML_SIGNATURE_INVALID;
        m_guid = "";
        m_cert = NULL;
    }
    ~COOXMLSignature()
    {
        RELEASEOBJECT(m_cert);
    }

public:
    int GetValid()
    {
        return m_valid;
    }
    std::string GetGuid()
    {
        return m_guid;
    }
    ICertificate* GetCertificate()
    {
        return m_cert;
    }
    std::string GetImageValidBase64()
    {
        return m_sImageValidBase64;
    }
    std::string GetImageInvalidBase64()
    {
        return m_sImageInvalidBase64;
    }

public:
    void Check()
    {
        // 1) get cert
        XmlUtils::CXmlNode oNodeCert = m_node.ReadNode(L"KeyInfo").ReadNode(L"X509Data").ReadNode(L"X509Certificate");
        if (!oNodeCert.IsValid())
        {
            m_valid = OOXML_SIGNATURE_NOTSUPPORTED;
            return;
        }
        m_cert = new CCertificate();
        if (!m_cert->LoadFromBase64Data(U_TO_UTF8(oNodeCert.GetText())))
        {
            m_valid = OOXML_SIGNATURE_NOTSUPPORTED;
            return;
        }


    }

    friend class COOXMLVerifier;

private:
    XmlUtils::CXmlNode GetById(const std::string& id)
    {
        return GetByIdRec(m_node, id);
    }
    XmlUtils::CXmlNode GetByIdRec(XmlUtils::CXmlNode& node, const std::string& id)
    {
        XmlUtils::CXmlNode retNode;

        XmlUtils::CXmlNodes oNodes;
        if (node.GetChilds(oNodes))
        {
            int nCount = oNodes.GetCount();

            for (int i = 0; i < nCount; i++)
            {
                XmlUtils::CXmlNode _node;
                oNodes.GetAt(i, _node);

                if (_node.GetAttributeA("Id") == id)
                    return _node;

                retNode = GetByIdRec(_node, id);
                if (retNode.IsValid())
                    return retNode;
            }
        }

        return retNode;
    }
};

class COOXMLVerifier
{
public:
    std::wstring                            m_sFolder;
    std::vector<COOXMLSignature*>           m_arSignatures;

public:
    COOXMLVerifier(const std::wstring& sFolder)
    {
        m_sFolder = sFolder;

        if (!NSFile::CFileBinary::Exists(m_sFolder + L"/_xmlsignatures/origin.sigs"))
            return;

        XmlUtils::CXmlNode oContentTypes;
        if (!oContentTypes.FromXmlFile(m_sFolder + L"/[Content_Types].xml"))
            return;

        XmlUtils::CXmlNodes oOverrides = oContentTypes.GetNodes(L"Override");
        int nCount = oOverrides.GetCount();

        for (int i = 0; i < nCount; i++)
        {
            XmlUtils::CXmlNode node;
            oOverrides.GetAt(i, node);

            if (node.GetAttributeA("ContentType") != "application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml")
                continue;

            std::wstring sFile = m_sFolder + node.GetAttribute("PartName");
            XmlUtils::CXmlNode nodeSig;
            if (!nodeSig.FromXmlFile(sFile))
                continue;

            if (nodeSig.GetName() != L"Signature")
                continue;

            COOXMLSignature* pSignature = new COOXMLSignature();
            pSignature->m_node = nodeSig;
            pSignature->Check();

            m_arSignatures.push_back(pSignature);
        }
    }
    ~COOXMLVerifier()
    {
        for (std::vector<COOXMLSignature*>::iterator i = m_arSignatures.begin(); i != m_arSignatures.end(); i++)
        {
            COOXMLSignature* v = *i;
            RELEASEOBJECT(v);
        }
        m_arSignatures.clear();
    }
};

#endif //_XML_OOXMLVERIFIER_H_

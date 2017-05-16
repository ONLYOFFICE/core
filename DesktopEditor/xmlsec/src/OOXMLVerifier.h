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

    std::wstring    m_sFolder;

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

        CXmlStackNamespaces GetById(const std::string& id, const bool& isNameUse = false)
        {
            return GetByIdRec(*this, id, isNameUse);
        }

        CXmlStackNamespaces GetByIdRec(CXmlStackNamespaces& stack, const std::string& id, const bool& isNameUse = false)
        {
            if (stack.m_node.GetAttributeA("Id") == id)
                return stack;

            if (isNameUse)
            {
                std::string sName = U_TO_UTF8((stack.m_node.GetName()));
                if (sName == id)
                    return stack;
            }

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
                    oNodes.GetAt(i, ret.m_node);
                    CXmlStackNamespaces _retRecursion = ret.GetByIdRec(ret, id, isNameUse);
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
                {
                    sXml.replace(0, sXmlFind.length(), L"<" + sName + L" " + m_namespaces + L" ");
                }
                else
                {
                    sXmlFind = L"<" + sName + L">";
                    if (0 == sXml.find(sXmlFind))
                        sXml.replace(0, sXmlFind.length(), L"<" + sName + L" " + m_namespaces + L">");
                }
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
        // 1) Certificate
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

        // 2) Check files (Manifect)
        XmlUtils::CXmlNode nodeManifect = GetObjectById("idPackageObject");
        if (!nodeManifect.IsValid())
        {
            m_valid = OOXML_SIGNATURE_INVALID;
            return;
        }

        XmlUtils::CXmlNodes nodesManifestRefs = nodeManifect.ReadNode(L"Manifest").GetNodes(L"Reference");
        int nRefsCount = nodesManifestRefs.GetCount();
        for (int i = 0; i < nRefsCount; i++)
        {
            XmlUtils::CXmlNode tmp;
            nodesManifestRefs.GetAt(i, tmp);

            m_valid = CheckManifestReference(tmp);
            if (OOXML_SIGNATURE_VALID != m_valid)
                return;
        }

        // 3) Images
        XmlUtils::CXmlNode nodeImageValid = GetObjectById("idValidSigLnImg");
        if (nodeImageValid.IsValid())
            m_sImageValidBase64 = U_TO_UTF8(nodeImageValid.GetText());
        XmlUtils::CXmlNode nodeImageInvalid = GetObjectById("idInvalidSigLnImg");
        if (nodeImageInvalid.IsValid())
            m_sImageInvalidBase64 = U_TO_UTF8(nodeImageInvalid.GetText());

        // 4) Objects
        XmlUtils::CXmlNodes nodesReferences;
        m_node.ReadNode(L"SignedInfo").GetNodes(L"Reference", nodesReferences);
        nRefsCount = nodesReferences.GetCount();
        for (int i = 0; i < nRefsCount; i++)
        {
            XmlUtils::CXmlNode tmp;
            nodesReferences.GetAt(i, tmp);

            m_valid = CheckObjectReference(tmp);
            if (OOXML_SIGNATURE_VALID != m_valid)
                return;
        }

        // 5) Check signature
        CXmlStackNamespaces stack(m_node);
        CXmlStackNamespaces stackRes = stack.GetById("SignedInfo", true);
        std::string sXml = stackRes.GetXml();

        std::string sCanonicalizationMethod = m_node.ReadNode(L"SignedInfo").ReadNode(L"CanonicalizationMethod").GetAttributeA("Algorithm");
        std::string sSignatureMethod = m_node.ReadNode(L"SignedInfo").ReadNode(L"SignatureMethod").GetAttributeA("Algorithm");

        int nSignatureMethod = ICertificate::GetOOXMLHashAlg(sSignatureMethod);
        if (OOXML_HASH_ALG_INVALID == nSignatureMethod)
        {
            m_valid = OOXML_SIGNATURE_NOTSUPPORTED;
            return;
        }

        IXmlTransform* pCanonicalizationMethodTransform = IXmlTransform::GetFromType(sCanonicalizationMethod);
        if (NULL == pCanonicalizationMethodTransform)
        {
            m_valid = OOXML_SIGNATURE_NOTSUPPORTED;
            return;
        }

        std::string sSignatureCalcValue = pCanonicalizationMethodTransform->Transform(sXml);
        RELEASEOBJECT(pCanonicalizationMethodTransform);

        std::string sSignatureValue = U_TO_UTF8((m_node.ReadValueString(L"SignatureValue")));

        if (!m_cert->Verify(sSignatureCalcValue, sSignatureValue, nSignatureMethod))
            m_valid = OOXML_SIGNATURE_INVALID;
    }

    XmlUtils::CXmlNode GetObjectById(std::string sId)
    {
        XmlUtils::CXmlNodes oNodes = m_node.GetNodes(L"Object");
        int nCount = oNodes.GetCount();
        for (int i = 0; i < nCount; i++)
        {
            XmlUtils::CXmlNode tmp;
            oNodes.GetAt(i, tmp);
            if (sId == tmp.GetAttributeA("Id"))
                return tmp;
        }
        XmlUtils::CXmlNode ret;
        return ret;
    }

    friend class COOXMLVerifier;

private:

    int CheckManifestReference(XmlUtils::CXmlNode& node)
    {
        std::wstring sFile = node.GetAttribute("URI");
        std::wstring::size_type nPos = sFile.find(L"?");
        if (nPos == std::wstring::npos)
            return OOXML_SIGNATURE_INVALID;

        sFile = sFile.substr(0, nPos);
        sFile = m_sFolder + sFile;

        if (!NSFile::CFileBinary::Exists(sFile))
            return OOXML_SIGNATURE_INVALID;

        XmlUtils::CXmlNode nodeMethod = node.ReadNode(L"DigestMethod");
        if (!nodeMethod.IsValid())
            return OOXML_SIGNATURE_INVALID;

        int nAlg = ICertificate::GetOOXMLHashAlg(nodeMethod.GetAttributeA("Algorithm"));

        if (OOXML_HASH_ALG_INVALID == nAlg)
            return OOXML_SIGNATURE_NOTSUPPORTED;

        std::string sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        std::string sCalcValue = "";

        XmlUtils::CXmlNode nodeTransform = node.ReadNode(L"Transforms");
        if (!nodeTransform.IsValid())
        {
            // simple hash
            sCalcValue = m_cert->GetHash(sFile, nAlg);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        }
        else
        {
            // XML
            CXmlTransforms oTransforms(nodeTransform);
            if (!oTransforms.GetValid())
                return OOXML_SIGNATURE_NOTSUPPORTED;

            std::string sXml;
            NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sXml);

            sXml = oTransforms.Transform(sXml);

            sCalcValue = m_cert->GetHash(sXml, nAlg);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        }

        if (sCalcValue != sValue)
            return OOXML_SIGNATURE_INVALID;

        return OOXML_SIGNATURE_VALID;
    }

    int CheckObjectReference(XmlUtils::CXmlNode& node)
    {
        std::string sURI = node.GetAttributeA("URI");
        if ("" == sURI)
            return OOXML_SIGNATURE_INVALID;

        if (0 == sURI.find("#"))
            sURI = sURI.substr(1);

        std::string sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));

        CXmlTransforms oTransforms;

        CXmlTransformC14N* pTransform = new CXmlTransformC14N();
        pTransform->CheckC14NTransform("http://www.w3.org/TR/2001/REC-xml-c14n-20010315");
        oTransforms.AddTransform(pTransform);

#if 0
        XmlUtils::CXmlNode nodeTransform = node.ReadNode(L"Transforms");
        if (!nodeTransform.IsValid())
        {
            // simple hash
            sCalcValue = m_cert->GetHash(sFile, nAlg);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        }
        else
        {
            // XML
            CXmlTransforms oTransforms(nodeTransform);
            if (!oTransforms.GetValid())
                return OOXML_SIGNATURE_NOTSUPPORTED;

            std::string sXml;
            NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sXml);

            sXml = oTransforms.Transform(sXml);

            sCalcValue = m_cert->GetHash(sXml, nAlg);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        }
#endif

        CXmlStackNamespaces stack(m_node);
        CXmlStackNamespaces stackRes = stack.GetById(sURI);
        std::string sXml = stackRes.GetXml();

        sXml = oTransforms.Transform(sXml);

        XmlUtils::CXmlNode nodeMethod = node.ReadNode(L"DigestMethod");
        if (!nodeMethod.IsValid())
            return OOXML_SIGNATURE_INVALID;

        int nAlg = ICertificate::GetOOXMLHashAlg(nodeMethod.GetAttributeA("Algorithm"));
        std::string sCalcValue = m_cert->GetHash(sXml, nAlg);

        if (sCalcValue != sValue)
            return OOXML_SIGNATURE_INVALID;

        return OOXML_SIGNATURE_VALID;
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
            pSignature->m_sFolder = m_sFolder;
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

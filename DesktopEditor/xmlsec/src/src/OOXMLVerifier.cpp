#include "./XmlTransform.h"
#include "./../include/OOXMLVerifier.h"
#include "../../../../OfficeUtils/src/ZipFolder.h"
#include "./../include/CertificateCommon.h"

class COOXMLSignature_private
{
public:
    int             m_valid;
    std::string     m_guid;
    ICertificate*   m_cert;

    std::string     m_sImageValidBase64;
    std::string     m_sImageInvalidBase64;

    std::wstring    m_sFolder;
    IFolder*        m_pFolder;

    std::wstring    m_sFile;
    std::string     m_sDate;

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
    COOXMLSignature_private()
    {
        m_valid = OOXML_SIGNATURE_INVALID;
        m_guid = "";
        m_cert = NULL;
    }
    ~COOXMLSignature_private()
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
    std::string GetDate()
    {
        return m_sDate;
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

    std::wstring GetFile()
    {
        return m_sFile;
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
        m_cert = NSCertificate::CreateInstance();
        if (!m_cert->LoadFromBase64Data(U_TO_UTF8(oNodeCert.GetText())))
        {
            m_valid = OOXML_SIGNATURE_NOTSUPPORTED;
            return;
        }

        XmlUtils::CXmlNode firstChild = GetObjectById("idOfficeObject");
        std::wstring sSetupID = FindFirstChild(firstChild, L"SetupID").GetText();
        m_guid = U_TO_UTF8(sSetupID);

        if (m_guid.empty())
        {
            std::wstring sFile = m_sFile;
            NSStringUtils::string_replace(sFile, L"\\", L"/");

            std::wstring::size_type posSign = sFile.find(L"/_xmlsignatures");
            if (std::wstring::npos != posSign)
            {
                sFile = sFile.substr(posSign);
            }
            m_guid = U_TO_UTF8(sFile);
        }

        // 2) Images
        XmlUtils::CXmlNode nodeImageValid = GetObjectById("idValidSigLnImg");
        if (nodeImageValid.IsValid())
            m_sImageValidBase64 = GetBase64Image(nodeImageValid);
        XmlUtils::CXmlNode nodeImageInvalid = GetObjectById("idInvalidSigLnImg");
        if (nodeImageInvalid.IsValid())
            m_sImageInvalidBase64 = GetBase64Image(nodeImageInvalid);

        // 3) Check files (Manifect)
        XmlUtils::CXmlNode nodeManifect = GetObjectById("idPackageObject");
        if (!nodeManifect.IsValid())
        {
            m_valid = OOXML_SIGNATURE_INVALID;
            return;
        }

        XmlUtils::CXmlNode nodeSignProperties = GetObjectSignedProperties();
        XmlUtils::CXmlNode nodeSignedSignatureProperties = nodeSignProperties.ReadNodeNoNS(L"SignedSignatureProperties");
        XmlUtils::CXmlNode nodeST = nodeSignedSignatureProperties.ReadNodeNoNS(L"SigningTime");
        std::wstring sDateW = nodeST.GetText();
        if (!sDateW.empty())
            m_sDate = U_TO_UTF8(sDateW);

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

        // 5) Check sert digest! (TODO:)

        // 6) Check signature
        CXmlStackNamespaces stack(m_node);
        CXmlStackNamespaces stackRes = stack.GetById("SignedInfo", true);
        std::string sXml = stackRes.GetXml();

        std::string sCanonicalizationMethod = m_node.ReadNode(L"SignedInfo").ReadNode(L"CanonicalizationMethod").GetAttributeA("Algorithm");
        std::string sSignatureMethod = m_node.ReadNode(L"SignedInfo").ReadNode(L"SignatureMethod").GetAttributeA("Algorithm");

        int nSignatureMethod = NSCertificate::GetOOXMLHashAlg(sSignatureMethod);
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
        else
        {
            int nCertVerify = m_cert->VerifySelf();
            if (OPEN_SSL_WARNING_NOVERIFY == nCertVerify)
                m_valid = OOXML_SIGNATURE_INVALID;
        }
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

    XmlUtils::CXmlNode GetObjectSignedProperties()
    {
        XmlUtils::CXmlNodes oNodes = m_node.GetNodes(L"Object");
        int nCount = oNodes.GetCount();
        for (int i = 0; i < nCount; i++)
        {
            XmlUtils::CXmlNode tmp;
            oNodes.GetAt(i, tmp);

            XmlUtils::CXmlNode nodeQ = tmp.ReadNodeNoNS(L"QualifyingProperties");
            if (nodeQ.IsValid())
            {
                return nodeQ.ReadNodeNoNS(L"SignedProperties");
            }
        }
        XmlUtils::CXmlNode ret;
        return ret;
    }

    XmlUtils::CXmlNode FindFirstChild(XmlUtils::CXmlNode& node, const std::wstring& sName)
    {
        if (node.GetName() == sName)
            return node;

        XmlUtils::CXmlNodes childs;
        if (node.GetChilds(childs))
        {
            int nCount = childs.GetCount();
            for (int i = 0; i < nCount; i++)
            {
                XmlUtils::CXmlNode child;
                childs.GetAt(i, child);

                XmlUtils::CXmlNode ret = FindFirstChild(child, sName);
                if (ret.IsValid())
                    return ret;
            }
        }

        XmlUtils::CXmlNode ret;
        return ret;
    }

    std::string GetBase64Image(XmlUtils::CXmlNode& node)
    {
        std::wstring sW = node.GetText();
        std::string s = U_TO_UTF8(sW);

        int len = (int)s.length();
        int j = 0;

        for (int i = 0; i < len;)
        {
            char c = s.at(i);
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '+') || (c == '/'))
                s.at(j++) = s.at(i++);
            else
                i++;
        }
        s.resize(j);

        return s;
    }

    void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
    {
        size_t posn = 0;
        while (std::string::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }

    void MakeBase64_NOCRLF(std::string& value)
    {
        string_replaceA(value, "\r", "");
        string_replaceA(value, "\n", "");
    }

    friend class COOXMLVerifier;

public:

    int CheckManifestReference(XmlUtils::CXmlNode& node)
    {
        std::wstring sFile = node.GetAttribute("URI");
        std::wstring::size_type nPos = sFile.find(L"?");
        if (nPos == std::wstring::npos)
            return OOXML_SIGNATURE_INVALID;

        sFile = sFile.substr(0, nPos);
        sFile = m_sFolder + sFile;

        if (!m_pFolder->exists(sFile))
            return OOXML_SIGNATURE_INVALID;

        XmlUtils::CXmlNode nodeMethod = node.ReadNode(L"DigestMethod");
        if (!nodeMethod.IsValid())
            return OOXML_SIGNATURE_INVALID;

        int nAlg = NSCertificate::GetOOXMLHashAlg(nodeMethod.GetAttributeA("Algorithm"));

        if (OOXML_HASH_ALG_INVALID == nAlg)
            return OOXML_SIGNATURE_NOTSUPPORTED;

        std::string sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
        MakeBase64_NOCRLF(sValue);

        std::string sCalcValue = "";

        XmlUtils::CXmlNode nodeTransform = node.ReadNode(L"Transforms");
        if (!nodeTransform.IsValid())
        {
            // simple hash
            IFolder::CBuffer* buffer = NULL;
            if (m_pFolder->read(sFile, buffer))
                sCalcValue = m_cert->GetHash(buffer->Buffer, buffer->Size, (nAlg == -1) ? m_cert->GetHashAlg() : nAlg);
            RELEASEOBJECT(buffer);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
            MakeBase64_NOCRLF(sValue);
        }
        else
        {
            // XML
            CXmlTransforms oTransforms(nodeTransform);
            if (!oTransforms.GetValid())
                return OOXML_SIGNATURE_NOTSUPPORTED;

            std::string sXml = m_pFolder->readXml(sFile);
            sXml = oTransforms.Transform(sXml);

            sCalcValue = m_cert->GetHash(sXml, nAlg);
            sValue = U_TO_UTF8((node.ReadNodeText(L"DigestValue")));
            MakeBase64_NOCRLF(sValue);
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
        MakeBase64_NOCRLF(sValue);

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
            MakeBase64_NOCRLF(sValue);
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
            MakeBase64_NOCRLF(sValue);
        }
#endif

        CXmlStackNamespaces stack(m_node);
        CXmlStackNamespaces stackRes = stack.GetById(sURI);
        std::string sXml = stackRes.GetXml();

        sXml = oTransforms.Transform(sXml);

        XmlUtils::CXmlNode nodeMethod = node.ReadNode(L"DigestMethod");
        if (!nodeMethod.IsValid())
            return OOXML_SIGNATURE_INVALID;

        int nAlg = NSCertificate::GetOOXMLHashAlg(nodeMethod.GetAttributeA("Algorithm"));
        std::string sCalcValue = m_cert->GetHash(sXml, nAlg);

        if (sCalcValue != sValue)
            return OOXML_SIGNATURE_INVALID;

        return OOXML_SIGNATURE_VALID;
    }
};

COOXMLSignature::COOXMLSignature()
{
    m_internal = new COOXMLSignature_private();
}

COOXMLSignature::~COOXMLSignature()
{
    RELEASEOBJECT(m_internal);
}

int COOXMLSignature::GetValid()
{
    return m_internal->GetValid();
}

std::string COOXMLSignature::GetGuid()
{
    return m_internal->GetGuid();
}
std::string COOXMLSignature::GetDate()
{
    return m_internal->GetDate();
}

ICertificate* COOXMLSignature::GetCertificate()
{
    return m_internal->GetCertificate();
}

std::string COOXMLSignature::GetImageValidBase64()
{
    return m_internal->GetImageValidBase64();
}

std::string COOXMLSignature::GetImageInvalidBase64()
{
    return m_internal->GetImageInvalidBase64();
}

void COOXMLSignature::Check()
{
    m_internal->Check();
}

class COOXMLVerifier_private
{
public:
    IFolder*                                m_pFolder;
    std::vector<COOXMLSignature*>           m_arSignatures;
    std::vector<std::wstring>               m_arSignaturesFiles;

public:
    COOXMLVerifier_private(const std::wstring& sFolder)
    {
        m_pFolder = new CFolderSystem(sFolder);
        OpenFolder();
    }
    COOXMLVerifier_private(BYTE* data, DWORD length)
    {
        m_pFolder = new CZipFolderMemory(data, length);
        OpenFolder();
    }

    void OpenFolder()
    {
        // check .sig file
        std::vector<std::wstring> arFiles = m_pFolder->getFiles(L"_xmlsignatures", false);
        bool bIsFound = false;
        for (std::vector<std::wstring>::iterator i = arFiles.begin(); i != arFiles.end(); i++)
        {
            if (NSFile::GetFileExtention(*i) == L"sigs")
            {
                bIsFound = true;
                break;
            }
        }

        if (!bIsFound)
            return;

        XmlUtils::CXmlNode oContentTypes = m_pFolder->getNodeFromFile(L"[Content_Types].xml");
        if (!oContentTypes.IsValid())
            return;

        XmlUtils::CXmlNodes oOverrides = oContentTypes.GetNodes(L"Override");
        int nCount = oOverrides.GetCount();

        for (int i = 0; i < nCount; i++)
        {
            XmlUtils::CXmlNode node;
            oOverrides.GetAt(i, node);

            if (node.GetAttributeA("ContentType") != "application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml")
                continue;

            std::wstring sFile = node.GetAttribute("PartName");
            XmlUtils::CXmlNode nodeSig = m_pFolder->getNodeFromFile(sFile);
            if (!nodeSig.IsValid())
                continue;

            if (nodeSig.GetName() != L"Signature")
                continue;

            COOXMLSignature* pSignature = new COOXMLSignature();
            pSignature->m_internal->m_sFile = sFile;
            pSignature->m_internal->m_node = nodeSig;
            pSignature->m_internal->m_pFolder = m_pFolder;
            pSignature->Check();

            m_arSignatures.push_back(pSignature);
            m_arSignaturesFiles.push_back(sFile);
        }
    }

    ~COOXMLVerifier_private()
    {
        for (std::vector<COOXMLSignature*>::iterator i = m_arSignatures.begin(); i != m_arSignatures.end(); i++)
        {
            COOXMLSignature* v = *i;
            RELEASEOBJECT(v);
        }
        m_arSignatures.clear();
        RELEASEOBJECT(m_pFolder);
    }

    void RemoveSignature(const std::string& sGuid)
    {
        int nCountSignatures = (int)m_arSignatures.size();
        if (0 == nCountSignatures)
            return;

        bool bIsRemoveAll = sGuid.empty();
        std::wstring sFile;

        if (!bIsRemoveAll)
        {
            for (int i = 0; i < nCountSignatures; ++i)
            {
                COOXMLSignature* pSignature = m_arSignatures.at(i);
                if (pSignature->GetGuid() == sGuid)
                {
                    sFile = m_arSignaturesFiles.at(i);
                    m_arSignatures.erase(m_arSignatures.begin() + i);
                    delete pSignature;
                    --i;
                    --nCountSignatures;
                }                
            }

            bIsRemoveAll = m_arSignatures.empty();
        }

        if (!sFile.empty())
            m_pFolder->remove(sFile);

        if (!bIsRemoveAll && sFile.empty())
            return;

        XmlUtils::CXmlNode oContentTypes = m_pFolder->getNodeFromFile(L"[Content_Types].xml");
        std::wstring sXml = L"<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\n";
        XmlUtils::CXmlNodes oNodes;
        if (oContentTypes.GetNodes(L"*", oNodes))
        {
            int nCount = oNodes.GetCount();

            for (int i = 0; i < nCount; ++i)
            {
                XmlUtils::CXmlNode oNode;
                oNodes.GetAt(i, oNode);

                if (bIsRemoveAll)
                {
                    if (L"Default" == oNode.GetName() && L"sigs" == oNode.GetAttribute(L"Extension"))
                        continue;
                    if (L"Override" == oNode.GetName() && L"application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml" == oNode.GetAttribute(L"ContentType"))
                        continue;

                    sXml += oNode.GetXml();
                }
                else
                {
                    std::wstring sFileFound = m_pFolder->getLocalFilePath(sFile);
                    if (L"Override" == oNode.GetName() &&
                        L"application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml" == oNode.GetAttribute(L"ContentType") &&
                        sFileFound == oNode.GetAttribute(L"PartName"))
                        continue;

                    sXml += oNode.GetXml();
                }
            }
        }
        sXml += L"</Types>";

        m_pFolder->writeXml(L"[Content_Types].xml", sXml);

        if (bIsRemoveAll)
        {
            std::vector<std::wstring> arrDeleteFiles = m_pFolder->getFiles(L"_xmlsignatures", true);
            for (const std::wstring& sPath : arrDeleteFiles)
                m_pFolder->remove(sPath);

            XmlUtils::CXmlNode oRels = m_pFolder->getNodeFromFile(L"_rels/.rels");
            if (!oRels.IsValid())
                return;

            sXml = L"<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
            XmlUtils::CXmlNodes oNodes;
            if (oRels.GetNodes(L"*", oNodes))
            {
                int nCount = oNodes.GetCount();

                for (int i = 0; i < nCount; ++i)
                {
                    XmlUtils::CXmlNode oNode;
                    oNodes.GetAt(i, oNode);

                    if (L"Relationship" == oNode.GetName() &&
                        L"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/origin" == oNode.GetAttribute(L"Type"))
                        continue;

                    sXml += oNode.GetXml();
                }
            }
            sXml += L"</Relationships>";

            m_pFolder->writeXml(L"_rels/.rels", sXml);
        }
        else
        {
            std::wstring sFileFound = m_pFolder->getLocalFilePath(sFile);
            std::wstring::size_type posRemove = sFileFound.find(L"_xmlsignatures/");
            if (std::wstring::npos != posRemove)
                sFileFound = sFileFound.substr(posRemove + 15);

            std::wstring sOriginRels = L"_xmlsignatures/_rels/origin.sigs.rels";
            XmlUtils::CXmlNode oRels = m_pFolder->getNodeFromFile(sOriginRels);
            if (!oRels.IsValid())
                return;

            sXml = L"<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
            XmlUtils::CXmlNodes oNodes;
            if (oRels.GetNodes(L"*", oNodes))
            {
                int nCount = oNodes.GetCount();

                for (int i = 0; i < nCount; ++i)
                {
                    XmlUtils::CXmlNode oNode;
                    oNodes.GetAt(i, oNode);

                    if (L"Relationship" == oNode.GetName() &&
                        L"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/signature" == oNode.GetAttribute(L"Type") &&
                        sFileFound == oNode.GetAttribute(L"Target"))
                        continue;

                    sXml += oNode.GetXml();
                }
            }
            sXml += L"</Relationships>";

            m_pFolder->writeXml(sOriginRels, sXml);
        }
    }
};

COOXMLVerifier::COOXMLVerifier(const std::wstring& sFolder)
{
    m_internal = new COOXMLVerifier_private(sFolder);
}

COOXMLVerifier::COOXMLVerifier(BYTE* data, DWORD length)
{
    m_internal = new COOXMLVerifier_private(data, length);
}

COOXMLVerifier::~COOXMLVerifier()
{
    RELEASEOBJECT(m_internal);
}

int COOXMLVerifier::GetSignatureCount()
{
    return (int)m_internal->m_arSignatures.size();
}

COOXMLSignature* COOXMLVerifier::GetSignature(const int& index)
{
    if (index >= (int)m_internal->m_arSignatures.size())
        return NULL;
    return m_internal->m_arSignatures[index];
}

void COOXMLVerifier::RemoveSignature(const std::string& sGuid)
{
    m_internal->RemoveSignature(sGuid);

    IFolder::CBuffer* buffer = m_internal->m_pFolder->finalize();
    std::wstring folder = m_internal->m_pFolder->getFullFilePath(L"");
    RELEASEOBJECT(m_internal);

    if (buffer)
    {
        m_internal = new COOXMLVerifier_private(buffer->Buffer, buffer->Size);
        buffer->UnsetDestroy();
        delete buffer;
    }
    else
    {
        m_internal = new COOXMLVerifier_private(folder);
    }
}

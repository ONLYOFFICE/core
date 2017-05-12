#ifndef _XML_OOXMLVERIFIER_H_
#define _XML_OOXMLVERIFIER_H_

#include "./XmlCanonicalizator.h"
#include "./XmlSignerBase.h"
#include "./XmlTransform.h"

#define OOXML_SIGNATURE_VALID           0
#define OOXML_SIGNATURE_INVALID         1
#define OOXML_SIGNATURE_NOTSUPPORTED    2

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
    void Check();

    friend class COOXMLVerifier;
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

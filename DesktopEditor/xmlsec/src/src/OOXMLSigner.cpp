#include "./../include/OOXMLSigner.h"
#include "./../src/XmlTransform.h"
#include <cstdio>
#include <ctime>
#include <time.h>

class COOXMLSigner_private
{
public:
    ICertificate*                           m_certificate;
    std::wstring                            m_sFolder;

    std::wstring                            m_date;

    std::map<std::wstring, std::wstring>    m_content_types;
    std::vector<std::wstring>               m_rels;
    std::vector<std::wstring>               m_files;

    NSStringUtils::CStringBuilderA          m_signed_info;

    std::wstring                            m_image_valid;
    std::wstring                            m_image_invalid;

    std::wstring                            m_guid;

public:
    COOXMLSigner_private(const std::wstring& sFolder, ICertificate* pContext)
    {
        m_sFolder = sFolder;
        m_certificate = pContext;

        m_date = L"2017-04-21T08:30:21Z";

        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer1[100];
        char buffer2[100];

        std::time(&rawtime);
        timeinfo = std::gmtime(&rawtime);

        std::strftime(buffer1, 100, "%Y-%m-%d", timeinfo);
        std::strftime(buffer2, 100, "%H:%M:%S", timeinfo);

        std::string date = (std::string(buffer1) + "T" + std::string(buffer2) + "Z");
        m_date = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(date);

        m_signed_info.WriteString("<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/>");
        m_signed_info.WriteString("<SignatureMethod Algorithm=\"");
        m_signed_info.WriteString(ICertificate::GetSignatureMethodA(m_certificate->GetHashAlg()));
        m_signed_info.WriteString("\"/>");
    }
    ~COOXMLSigner_private()
    {
    }

    std::wstring GetReference(const std::wstring& file, const std::wstring& content_type)
    {
        std::wstring sXml = L"<Reference URI=\"" + file + L"?ContentType=" + content_type + L"\">";
        sXml += (L"<DigestMethod Algorithm=\"" + ICertificate::GetDigestMethod(m_certificate->GetHashAlg()) + L"\"/>");
        sXml += L"<DigestValue>";
        std::string sTmp = m_certificate->GetHash(m_sFolder + file, m_certificate->GetHashAlg());
        sXml += UTF8_TO_U(sTmp);
        sXml += L"</DigestValue>";
        sXml += L"</Reference>";
        return sXml;
    }

    std::string GetHashXml(const std::wstring& xml)
    {
        std::string sXmlSigned = U_TO_UTF8(xml);
        sXmlSigned = XmlUtils::NSXmlCanonicalizator::Execute(sXmlSigned, XmlUtils::XML_C14N_1_0);
        return m_certificate->GetHash(sXmlSigned, m_certificate->GetHashAlg());
    }

    std::string GetReferenceMain(const std::wstring& xml, const std::wstring& id, const bool& isCannon = true)
    {
        std::wstring sXml1 = L"<Object xmlns=\"http://www.w3.org/2000/09/xmldsig#\"";
        if (id.empty())
            sXml1 += L">";
        else
            sXml1 += (L" Id=\"" + id + L"\">");
        sXml1 += xml;
        sXml1 += L"</Object>";

        std::string sHash = GetHashXml(sXml1);

        std::string sRet;
        if (isCannon)
            sRet = "<Transforms><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>";

        sRet += ("<DigestMethod Algorithm=\"" + ICertificate::GetDigestMethodA(m_certificate->GetHashAlg()) + "\"/><DigestValue>" + sHash + "</DigestValue>");

        return sRet;
    }

    std::wstring GetImageBase64(const std::wstring& file)
    {
        if (0 == file.find(L"data:image/"))
        {
            return file.substr(file.find(L",") + 1);
        }

        BYTE* pData = NULL;
        DWORD dwLen = 0;
        if (!NSFile::CFileBinary::ReadAllBytes(file, &pData, dwLen))
            return L"";

        char* pDataC = NULL;
        int nLen = 0;
        NSFile::CBase64Converter::Encode(pData, (int)dwLen, pDataC, nLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

        std::wstring sReturn = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(pDataC, (LONG)nLen, FALSE);

        RELEASEARRAYOBJECTS(pData);
        RELEASEARRAYOBJECTS(pDataC);

        return sReturn;
    }

    std::wstring GetRelsReference(const std::wstring& file)
    {
        COOXMLRelationships oRels(m_sFolder + file, true);
        if (oRels.rels.size() == 0)
            return L"";

        if (L"/_rels/.rels" == file)
        {
            oRels.CheckOriginSigs(m_sFolder + file);

            // удалим все лишнее
            std::vector<COOXMLRelationship>::iterator i = oRels.rels.begin();
            while (i != oRels.rels.end())
            {
                if (0 == i->target.find(L"docProps/"))
                    i = oRels.rels.erase(i);
                else if (0 == i->target.find(L"_xmlsignatures/"))
                    i = oRels.rels.erase(i);
                else
                    i++;
            }
        }

        NSStringUtils::CStringBuilder builder;
        builder.WriteString(L"<Reference URI=\"");
        builder.WriteString(file);
        builder.WriteString(L"?ContentType=application/vnd.openxmlformats-package.relationships+xml\">");
        builder.WriteString(oRels.GetTransforms());
        builder.WriteString(L"<DigestMethod Algorithm=\"" + ICertificate::GetDigestMethod(m_certificate->GetHashAlg()) + L"\"/><DigestValue>");

        std::wstring sXml = oRels.GetXml();
        std::string sHash = GetHashXml(sXml);

        std::wstring sHashW = UTF8_TO_U(sHash);
        builder.WriteString(sHashW);

        builder.WriteString(L"</DigestValue></Reference>");

        return builder.GetData();
    }

    int GetCountSigns(const std::wstring& file)
    {
        if (!NSFile::CFileBinary::Exists(file))
        {
            std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/signature\" Target=\"sig1.xml\"/>\
</Relationships>";

            NSFile::CFileBinary::SaveToFile(file, sRels, false);
            return 1;
        }

        XmlUtils::CXmlNode oNode;
        oNode.FromXmlFile(file);

        XmlUtils::CXmlNodes oNodes;
        oNode.GetNodes(L"Relationship", oNodes);

        int rId = oNodes.GetCount() + 1;

        std::string sXmlA;
        NSFile::CFileBinary::ReadAllTextUtf8A(file, sXmlA);

        std::string::size_type pos = sXmlA.rfind("</Relationships>");
        if (pos == std::string::npos)
            return 1;

        std::string sRet = sXmlA.substr(0, pos);
        sRet += ("<Relationship Id=\"rId" + std::to_string(rId) + "\" \
Type=\"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/signature\" Target=\"sig" + std::to_string(rId) + ".xml\"/>\
</Relationships>");

        NSFile::CFileBinary::Remove(file);

        NSFile::CFileBinary oFile;
        oFile.CreateFileW(file);
        oFile.WriteFile((BYTE*)sRet.c_str(), (DWORD)sRet.length());
        oFile.CloseFile();

        return rId;
    }

    void ParseContentTypes()
    {
        std::wstring file = m_sFolder + L"/[Content_Types].xml";
        XmlUtils::CXmlNode oNode;
        oNode.FromXmlFile(file);

        XmlUtils::CXmlNodes nodesDefaults;
        oNode.GetNodes(L"Default", nodesDefaults);

        XmlUtils::CXmlNodes nodesOverrides;
        oNode.GetNodes(L"Override", nodesOverrides);

        int nCount = nodesDefaults.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode node;
            nodesDefaults.GetAt(i, node);

            m_content_types.insert(std::pair<std::wstring, std::wstring>(node.GetAttribute("Extension"), node.GetAttribute("ContentType")));
        }

        nCount = nodesOverrides.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode node;
            nodesOverrides.GetAt(i, node);

            m_content_types.insert(std::pair<std::wstring, std::wstring>(node.GetAttribute("PartName"), node.GetAttribute("ContentType")));
        }
    }

    void Parse()
    {
        // 1) Parse Content_Types.xml
        ParseContentTypes();

        // 2) Parse files in directory
        std::vector<std::wstring> files = NSDirectory::GetFiles(m_sFolder, true);

        // 3) Check each file
        std::wstring sFolder = m_sFolder;
        NSStringUtils::string_replace(sFolder, L"\\", L"/");
        for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
        {
            std::wstring sCheckFile = *i;
            NSStringUtils::string_replace(sCheckFile, L"\\", L"/");

            if (0 != sCheckFile.find(sFolder))
                continue;

            // make cool filename
            sCheckFile = sCheckFile.substr(sFolder.length());

            // check needed file
            if (0 == sCheckFile.find(L"/_xmlsignatures") ||
                0 == sCheckFile.find(L"/docProps") ||
                0 == sCheckFile.find(L"/[Content_Types].xml"))
                continue;

            // check rels and add to needed array
            std::wstring::size_type posExt = sCheckFile.rfind(L".");
            if (std::wstring::npos == posExt)
                continue;

            std::wstring sExt = sCheckFile.substr(posExt + 1);
            if (sExt == L"rels")
                m_rels.push_back(sCheckFile);
            else
                m_files.push_back(sCheckFile);
        }

        std::sort(m_rels.begin(), m_rels.end());
        std::sort(m_files.begin(), m_files.end());
    }

    void WriteRelsReferences(NSStringUtils::CStringBuilder& builder)
    {
        for (std::vector<std::wstring>::iterator i = m_rels.begin(); i != m_rels.end(); i++)
        {
            builder.WriteString(GetRelsReference(*i));
        }
    }

    void WriteFilesReferences(NSStringUtils::CStringBuilder& builder)
    {
        for (std::vector<std::wstring>::iterator i = m_files.begin(); i != m_files.end(); i++)
        {
            std::wstring sFile = *i;
            std::wstring sContentType = L"application/xml";

            std::map<std::wstring, std::wstring>::iterator _find = m_content_types.find(sFile);
            if (_find != m_content_types.end())
            {
                sContentType = _find->second;
            }
            else
            {
                std::wstring::size_type posExt = sFile.rfind(L".");
                if (std::wstring::npos != posExt)
                {
                    std::wstring sExt = sFile.substr(posExt + 1);

                    _find = m_content_types.find(sExt);
                    if (_find != m_content_types.end())
                        sContentType = _find->second;
                }
            }

            builder.WriteString(GetReference(sFile, sContentType));
        }
    }

    void WriteManifest(NSStringUtils::CStringBuilder& builder)
    {
        builder.WriteString(L"<Manifest>");
        WriteRelsReferences(builder);
        WriteFilesReferences(builder);
        builder.WriteString(L"</Manifest>");
    }

    void CorrectContentTypes(int nCountSigsNeeds)
    {
        std::wstring file = m_sFolder + L"/[Content_Types].xml";
        XmlUtils::CXmlNode oNode;
        oNode.FromXmlFile(file);

        XmlUtils::CXmlNodes nodesDefaults;
        oNode.GetNodes(L"Default", nodesDefaults);

        XmlUtils::CXmlNodes nodesOverrides;
        oNode.GetNodes(L"Override", nodesOverrides);

        std::string sAddition = "";

        bool bIsSigsExist = false;
        int nCount = nodesDefaults.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode node;
            nodesDefaults.GetAt(i, node);

            if ("sigs" == node.GetAttributeA("Extension") &&
                "application/vnd.openxmlformats-package.digital-signature-origin" == node.GetAttributeA("ContentType"))
            {
                bIsSigsExist = true;
                break;
            }
        }

        if (!bIsSigsExist)
            sAddition += "<Default Extension=\"sigs\" ContentType=\"application/vnd.openxmlformats-package.digital-signature-origin\"/>";

        int nCountSigs = 0;
        nCount = nodesOverrides.GetCount();
        for (int i = 0; i < nCount; ++i)
        {
            XmlUtils::CXmlNode node;
            nodesOverrides.GetAt(i, node);

            if ("application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml" == node.GetAttributeA("ContentType"))
            {
                ++nCountSigs;
            }
        }

        for (int i = nCountSigs; i < nCountSigsNeeds; ++i)
        {
            sAddition += "<Override PartName=\"/_xmlsignatures/sig";
            sAddition += std::to_string(i + 1);
            sAddition += ".xml\" ContentType=\"application/vnd.openxmlformats-package.digital-signature-xmlsignature+xml\"/>";
        }

        std::string sXmlA;
        NSFile::CFileBinary::ReadAllTextUtf8A(file, sXmlA);

        std::string::size_type pos = sXmlA.rfind("</Types>");
        if (pos == std::string::npos)
            return;

        std::string sRet = sXmlA.substr(0, pos);
        sRet += sAddition;
        sRet += "</Types>";

        NSFile::CFileBinary::Remove(file);

        NSFile::CFileBinary oFile;
        oFile.CreateFileW(file);
        oFile.WriteFile((BYTE*)sRet.c_str(), (DWORD)sRet.length());
        oFile.CloseFile();
    }

    void SetGuid(const std::wstring& guid)
    {
        m_guid = guid;
    }
    void SetImageValid(const std::wstring& file)
    {
        m_image_valid = GetImageBase64(file);
    }
    void SetImageInvalid(const std::wstring& file)
    {
        m_image_invalid = GetImageBase64(file);
    }

    std::wstring GeneratePackageObject()
    {
        NSStringUtils::CStringBuilder builder;
        WriteManifest(builder);

        builder.WriteString(L"<SignatureProperties><SignatureProperty Id=\"idSignatureTime\" Target=\"#idPackageSignature\">");
        builder.WriteString(L"<mdssi:SignatureTime xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\">");
        builder.WriteString(L"<mdssi:Format>YYYY-MM-DDThh:mm:ssTZD</mdssi:Format>");
        builder.WriteString(L"<mdssi:Value>");
        builder.WriteString(m_date);
        builder.WriteString(L"</mdssi:Value>");
        builder.WriteString(L"</mdssi:SignatureTime></SignatureProperty></SignatureProperties>");

        std::wstring sXml = builder.GetData();

        m_signed_info.WriteString("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idPackageObject\">");
        m_signed_info.WriteString(GetReferenceMain(sXml, L"idPackageObject", false));
        m_signed_info.WriteString("</Reference>");

        return (L"<Object Id=\"idPackageObject\">" + sXml + L"</Object>");
    }
    std::wstring GenerateOfficeObject()
    {
        NSStringUtils::CStringBuilder builder;

        builder.WriteString(L"<SignatureProperties><SignatureProperty Id=\"idOfficeV1Details\" Target=\"#idPackageSignature\">");
        builder.WriteString(L"<SignatureInfoV1 xmlns=\"http://schemas.microsoft.com/office/2006/digsig\">");
        builder.WriteString(L"<SetupID>");

        std::wstring sGUID = m_guid;
        if (0 == sGUID.find(L"/_xmlsignatures"))
            sGUID = L"";

        builder.WriteString(sGUID);
        builder.WriteString(L"</SetupID>");
        builder.WriteString(L"<SignatureText></SignatureText>");
        builder.WriteString(L"<SignatureImage>");
        builder.WriteString(m_image_valid);
        builder.WriteString(L"</SignatureImage>");

        if (!m_certificate->IsGOST())
        {
            builder.WriteString(L"<SignatureComments/>\
<WindowsVersion>10.0</WindowsVersion>\
<OfficeVersion>16.0</OfficeVersion>\
<ApplicationVersion>16.0</ApplicationVersion>\
<Monitors>2</Monitors>\
<HorizontalResolution>1680</HorizontalResolution>\
<VerticalResolution>1050</VerticalResolution>\
<ColorDepth>32</ColorDepth>\
<SignatureProviderId>{00000000-0000-0000-0000-000000000000}</SignatureProviderId>\
<SignatureProviderUrl/>\
<SignatureProviderDetails>9</SignatureProviderDetails>\
<SignatureType>2</SignatureType>\
</SignatureInfoV1>\
</SignatureProperty>\
</SignatureProperties>");
        }
        else
        {
            builder.WriteString(L"<SignatureComments/>\
<WindowsVersion>10.0</WindowsVersion>\
<OfficeVersion>16.0</OfficeVersion>\
<ApplicationVersion>16.0</ApplicationVersion>\
<Monitors>2</Monitors>\
<HorizontalResolution>1680</HorizontalResolution>\
<VerticalResolution>1050</VerticalResolution>\
<ColorDepth>32</ColorDepth>\
<SignatureProviderId>{F5AC7D23-DA04-45F5-ABCB-38CE7A982553}</SignatureProviderId>\
<SignatureProviderUrl>http://www.cryptopro.ru/products/office/signature</SignatureProviderUrl>\
<SignatureProviderDetails>8</SignatureProviderDetails>\
<SignatureType>1</SignatureType>\
</SignatureInfoV1>\
</SignatureProperty>\
</SignatureProperties>");
        }

        m_signed_info.WriteString("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idOfficeObject\">");
        m_signed_info.WriteString(GetReferenceMain(builder.GetData(), L"idOfficeObject", false));
        m_signed_info.WriteString("</Reference>");

        return (L"<Object Id=\"idOfficeObject\">" + builder.GetData() + L"</Object>");
    }

    std::wstring GenerateImageObject()
    {
        if (m_image_valid.empty())
            return L"";

        m_signed_info.WriteString("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idValidSigLnImg\">");
        m_signed_info.WriteString(GetReferenceMain(m_image_valid, L"idValidSigLnImg", false));
        m_signed_info.WriteString("</Reference>");

        m_signed_info.WriteString("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idInvalidSigLnImg\">");
        m_signed_info.WriteString(GetReferenceMain(m_image_invalid, L"idInvalidSigLnImg", false));
        m_signed_info.WriteString("</Reference>");

        return (L"<Object Id=\"idValidSigLnImg\">" + m_image_valid + L"</Object><Object Id=\"idInvalidSigLnImg\">" + m_image_invalid + L"</Object>");
    }

    std::wstring GenerateSignPropertiesObject()
    {
        std::wstring sName = m_certificate->GetIssuerName();

        std::string sKeyA = m_certificate->GetNumber();
        std::wstring sKey = UTF8_TO_U(sKeyA);

        std::string sCertHA = m_certificate->GetCertificateHash();
        std::wstring sCertHW = UTF8_TO_U(sCertHA);

        std::wstring sXml = (L"<xd:SignedSignatureProperties>\
<xd:SigningTime>" + m_date + L"</xd:SigningTime>\
<xd:SigningCertificate>\
<xd:Cert>\
<xd:CertDigest>\
<DigestMethod Algorithm=\"" + ICertificate::GetDigestMethod(m_certificate->GetHashAlg()) + L"\"/>\
<DigestValue>" + sCertHW + L"</DigestValue>\
</xd:CertDigest>\
<xd:IssuerSerial>\
<X509IssuerName>CN=" + sName + L"</X509IssuerName>\
<X509SerialNumber>" + sKey + L"</X509SerialNumber>\
</xd:IssuerSerial>\
</xd:Cert>\
</xd:SigningCertificate>\
<xd:SignaturePolicyIdentifier>\
<xd:SignaturePolicyImplied/>\
</xd:SignaturePolicyIdentifier>\
</xd:SignedSignatureProperties>");

        std::wstring sSignedXml = L"<xd:SignedProperties xmlns=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:xd=\"http://uri.etsi.org/01903/v1.3.2#\" Id=\"idSignedProperties\">";
        sSignedXml += sXml;
        sSignedXml += L"</xd:SignedProperties>";

        std::string sXmlTmp = XmlUtils::NSXmlCanonicalizator::Execute(U_TO_UTF8(sSignedXml), XmlUtils::XML_C14N_1_0);

        m_signed_info.WriteString("<Reference Type=\"http://uri.etsi.org/01903#SignedProperties\" URI=\"#idSignedProperties\">");
        m_signed_info.WriteString("<Transforms><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>");
        m_signed_info.WriteString("<DigestMethod Algorithm=\"" + ICertificate::GetDigestMethodA(m_certificate->GetHashAlg()) + "\"/><DigestValue>");
        m_signed_info.WriteString(m_certificate->GetHash(sXmlTmp, m_certificate->GetHashAlg()));
        m_signed_info.WriteString("</DigestValue></Reference>");

        return (L"<Object><xd:QualifyingProperties xmlns:xd=\"http://uri.etsi.org/01903/v1.3.2#\" Target=\"#idPackageSignature\">\
<xd:SignedProperties Id=\"idSignedProperties\">" + sXml + L"</xd:SignedProperties></xd:QualifyingProperties></Object>");
    }

    int AddSignatureReference()
    {
        std::wstring sDirectory = m_sFolder + L"/_xmlsignatures";

        if (!NSDirectory::Exists(sDirectory))
            NSDirectory::CreateDirectory(sDirectory);

        if (!NSFile::CFileBinary::Exists(sDirectory + L"/origin.sigs"))
        {
            NSFile::CFileBinary oFile;
            oFile.CreateFileW(sDirectory + L"/origin.sigs");
            oFile.CloseFile();
        }

        if (!NSDirectory::Exists(sDirectory + L"/_rels"))
            NSDirectory::CreateDirectory(sDirectory + L"/_rels");

        int nSignNum = GetCountSigns(sDirectory + L"/_rels/origin.sigs.rels");

        CorrectContentTypes(nSignNum);

        return nSignNum;
    }

    void Sign()
    {
        Parse();

        std::string sSignedData;

        NSStringUtils::CStringBuilder builderMain;

        builderMain.WriteString(GeneratePackageObject());
        builderMain.WriteString(GenerateOfficeObject());
        builderMain.WriteString(GenerateSignPropertiesObject());
        builderMain.WriteString(GenerateImageObject());

        std::string sSignedInfoData = m_signed_info.GetData();
        std::string sSignedXml = "<SignedInfo xmlns=\"http://www.w3.org/2000/09/xmldsig#\">" + sSignedInfoData + "</SignedInfo>";
        sSignedXml = XmlUtils::NSXmlCanonicalizator::Execute(sSignedXml, XmlUtils::XML_C14N_1_0);
        sSignedXml = m_certificate->Sign(sSignedXml);

        NSStringUtils::CStringBuilder builderResult;
        builderResult.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?><Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\" Id=\"idPackageSignature\"><SignedInfo>");
        builderResult.WriteString(UTF8_TO_U(sSignedInfoData));
        builderResult.WriteString(L"</SignedInfo>");
        builderResult.WriteString(L"<SignatureValue>");
        builderResult.WriteString(UTF8_TO_U(sSignedXml));
        builderResult.WriteString(L"</SignatureValue>");
        builderResult.WriteString(L"<KeyInfo><X509Data><X509Certificate>");
        builderResult.WriteString(UTF8_TO_U(m_certificate->GetCertificateBase64()));
        builderResult.WriteString(L"</X509Certificate></X509Data></KeyInfo>");

        builderResult.Write(builderMain);
        builderResult.WriteString(L"</Signature>");

        int nSignNum = AddSignatureReference();

        NSFile::CFileBinary::SaveToFile(m_sFolder + L"/_xmlsignatures/sig" + std::to_wstring(nSignNum) + L".xml", builderResult.GetData(), false);
    }
};

COOXMLSigner::COOXMLSigner(const std::wstring& sFolder, ICertificate* pContext)
{
    m_internal = new COOXMLSigner_private(sFolder, pContext);
}

COOXMLSigner::~COOXMLSigner()
{
    RELEASEOBJECT(m_internal);
}

void COOXMLSigner::SetGuid(const std::wstring& guid)
{
    m_internal->SetGuid(guid);
}

void COOXMLSigner::SetImageValid(const std::wstring& file)
{
    m_internal->SetImageValid(file);
}

void COOXMLSigner::SetImageInvalid(const std::wstring& file)
{
    m_internal->SetImageInvalid(file);
}

void COOXMLSigner::Sign()
{
    m_internal->Sign();
}

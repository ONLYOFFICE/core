#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>

#include <tchar.h>
#include <string>

#include "../../../common/File.h"
#include "../../../common/Directory.h"
#include "../../../common/BigInteger.h"

#include "../../../xml/include/xmlutils.h"
#include "../../../xml/libxml2/include/libxml/c14n.h"

#include <vector>

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "cryptui.lib")
#pragma comment (lib, "Advapi32.lib")

class CXmlCanonicalizator
{
private:
    class CXmlBuffer
    {
    public:
        NSStringUtils::CStringBuilderA builder;

    public:
        CXmlBuffer()
        {
        }
        ~CXmlBuffer()
        {
        }
    };

    static int buffer_xmlBufferIOWrite(CXmlBuffer* buf, const char* buffer, int len)
    {
        buf->builder.WriteString(buffer, (size_t)len);
        return len;
    }

    static int buffer_xmlBufferIOClose(CXmlBuffer* buf)
    {
        return 0;
    }

    static int buffer_xmlC14NIsVisibleCallback(void * user_data, xmlNodePtr node, xmlNodePtr parent)
    {
        if (node->type == XML_TEXT_NODE)
        {
            const char* cur = (char*)node->content;
            size_t size = strlen(cur);
            for (size_t i = 0; i < size; ++i, ++cur)
            {
                if (*cur != '\n' && *cur != '\r' && *cur != '\t')
                    return 1;
            }
            return 0;
        }
        return 1;
    }

public:
    static std::string Execute(const std::string& sXml, int mode)
    {
        xmlDocPtr xmlDoc = xmlParseMemory((char*)sXml.c_str(), (int)sXml.length());

        CXmlBuffer bufferC14N;
        xmlOutputBufferPtr _buffer = xmlOutputBufferCreateIO((xmlOutputWriteCallback)buffer_xmlBufferIOWrite,
                                                             (xmlOutputCloseCallback)buffer_xmlBufferIOClose,
                                                             &bufferC14N,
                                                             NULL);

        xmlC14NExecute(xmlDoc, buffer_xmlC14NIsVisibleCallback, NULL, mode, NULL, 0, _buffer);

        xmlOutputBufferClose(_buffer);

        return bufferC14N.builder.GetData();
    }

    static std::string Execute(const std::wstring& sXmlFile, int mode)
    {
        std::string sXml;
        NSFile::CFileBinary::ReadAllTextUtf8A(sXmlFile, sXml);

        xmlDocPtr xmlDoc = xmlParseMemory((char*)sXml.c_str(), (int)sXml.length());

        return Execute(sXml, mode);
    }
};

class CXmlSigner
{
private:
    PCCERT_CONTEXT m_context;

public:
    CXmlSigner(PCCERT_CONTEXT pCertContext)
    {
        m_context = pCertContext;        
    }
    ~CXmlSigner()
    {        
    }

public:

    std::string Sign(std::string sXml)
    {
        BOOL bResult = TRUE;
        DWORD dwKeySpec = 0;
        HCRYPTHASH  hHash = NULL;

        HCRYPTPROV hCryptProv = NULL;
        bResult = CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

        if (!bResult)
            return "";

        bResult = CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash);
        if (!bResult)
        {
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        bResult = CryptHashData(hHash, (BYTE*)sXml.c_str(), (DWORD)sXml.length(), 0);
        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        DWORD dwSigLen = 0;
        BYTE* pbSignature = NULL;
        bResult = CryptSignHash(hHash, dwKeySpec, NULL, 0, NULL, &dwSigLen);

        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        pbSignature = new BYTE[dwSigLen];
        bResult = CryptSignHash(hHash, dwKeySpec, NULL, 0, pbSignature, &dwSigLen);

        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        BYTE* pbSignatureMem = new BYTE[dwSigLen];
        ConvertEndian(pbSignature, pbSignatureMem, dwSigLen);

        char* pBase64 = NULL;
        int nBase64Len = 0;
        NSFile::CBase64Converter::Encode(pbSignatureMem, (int)dwSigLen, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NONE);

        delete[] pbSignature;
        delete[] pbSignatureMem;

        bResult = CryptDestroyHash(hHash);

        std::string sReturn(pBase64, nBase64Len);

        delete[] pBase64;

        CryptReleaseContext(hCryptProv, 0);

        return sReturn;
    }

    std::string GetHash(BYTE* pData, DWORD dwSize)
    {
        BOOL bResult = TRUE;
        DWORD dwKeySpec = 0;
        HCRYPTHASH  hHash = NULL;

        HCRYPTPROV hCryptProv = NULL;

        bResult = CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

        if (!bResult)
            return "";

        bResult = CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash);
        if (!bResult)
        {
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        bResult = CryptHashData(hHash, pData, dwSize, 0);
        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        DWORD cbHashSize = 0, dwCount = sizeof(DWORD);
        bResult = CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&cbHashSize, &dwCount, 0);

        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        BYTE* pDataHashRaw = new BYTE[dwCount];

        bResult = CryptGetHashParam(hHash, HP_HASHVAL, pDataHashRaw, &cbHashSize, 0);

        if (!bResult)
        {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hCryptProv, 0);
            return "";
        }

        char* pBase64_hash = NULL;
        int nBase64Len_hash = 0;
        NSFile::CBase64Converter::Encode(pDataHashRaw, (int)cbHashSize, pBase64_hash, nBase64Len_hash, NSBase64::B64_BASE64_FLAG_NOCRLF);

        std::string sReturn(pBase64_hash, nBase64Len_hash);
        delete [] pBase64_hash;

        //delete [] pDataHashRaw;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);

        return sReturn;
    }

    std::string GetHash(std::string& sXml)
    {
        return GetHash((BYTE*)sXml.c_str(), (DWORD)sXml.length());
    }

    std::string GetHash(std::wstring& sXmlFile)
    {
        BYTE* pFileData = NULL;
        DWORD dwFileDataLen = 0;
        NSFile::CFileBinary::ReadAllBytes(sXmlFile, &pFileData, dwFileDataLen);

        if (0 == dwFileDataLen)
            return "";

        std::string sReturn = GetHash(pFileData, dwFileDataLen);

        RELEASEARRAYOBJECTS(pFileData);
        return sReturn;
    }

    BOOL Verify(std::string& sXml, std::string& sXmlSignature)
    {
        DWORD dwKeySpec = 0;
        HCRYPTHASH hHash = NULL;
        HCRYPTKEY hPubKey = NULL;

        HCRYPTPROV hCryptProv = NULL;
        BOOL bResult = CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

        if (!bResult)
            return FALSE;

        bResult = CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash);

        if (!bResult)
        {
            CryptReleaseContext(hCryptProv, 0);
            return FALSE;
        }

        BYTE* pDataHash = NULL;
        DWORD dwHashLen = 0;
        int nTmp = 0;
        NSFile::CBase64Converter::Decode((char*)sXmlSignature.c_str(), (int)sXmlSignature.length(), pDataHash, nTmp);
        dwHashLen = (DWORD)nTmp;

        BYTE* pDataHashMem = new BYTE[dwHashLen];
        ConvertEndian(pDataHash, pDataHashMem, dwHashLen);

        RELEASEARRAYOBJECTS(pDataHash);

        bResult = CryptHashData(hHash, (BYTE*)sXml.c_str(), (DWORD)sXml.length(), 0);

        // Get the public key from the certificate
        CryptImportPublicKeyInfo(hCryptProv, m_context->dwCertEncodingType, &m_context->pCertInfo->SubjectPublicKeyInfo, &hPubKey);

        BOOL bResultRet = CryptVerifySignature(hHash, pDataHashMem, dwHashLen, hPubKey, NULL, 0);

        delete[] pDataHashMem;

        bResult = CryptDestroyHash(hHash);

        CryptDestroyKey(hPubKey);
        CryptReleaseContext(hCryptProv, 0);

        return bResultRet && bResult;
    }

    std::string GetCertificateBase64()
    {
        char* pData = NULL;
        int nDataLen = 0;
        NSFile::CBase64Converter::Encode(m_context->pbCertEncoded, (int)m_context->cbCertEncoded, pData, nDataLen, NSBase64::B64_BASE64_FLAG_NOCRLF);
        std::string sReturn(pData, nDataLen);
        RELEASEARRAYOBJECTS(pData);
        return sReturn;
    }

    std::string GetCertificateHash()
    {
        return GetHash(m_context->pbCertEncoded, (int)m_context->cbCertEncoded);
    }

public:
    static void ConvertEndian(const BYTE* src, BYTE* dst, DWORD size)
    {
        for(BYTE* p = dst + size - 1; p >= dst; ++src, --p)
            (*p) = (*src);
    }
};

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void MyHandleError(char *s);

bool Sign(PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile);
bool Verify(PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile);
bool SignDocument(std::wstring sFolderOOXML, PCCERT_CONTEXT pCertContext);

void main(void)
{
    if (false)
    {
        CBigInteger int1("345097");
        CBigInteger int2("87960324");

        CBigInteger val1 = int1 + int2;
        CBigInteger val2 = int1 - int2;
        CBigInteger val3 = int1 * int2;

        CBigInteger int3("66A1F302407647974D18D489855371B5", 16);

        unsigned char buffer[16] = { 0x66, 0xA1, 0xF3, 0x02, 0x40, 0x76, 0x47, 0x97, 0x4D, 0x18, 0xD4, 0x89, 0x85, 0x53, 0x71, 0xB5 };
        CBigInteger val4(buffer, 16);

        std::string sValue = int3.ToString();
    }

    //-------------------------------------------------------------------
    // Copyright (C) Microsoft.  All rights reserved.
    // This program lists all of the certificates in a system certificate
    // store and all of the property identifier numbers of those
    // certificates. It also demonstrates the use of two
    // UI functions. One, CryptUIDlgSelectCertificateFromStore,
    // displays the certificates in a store
    // and allows the user to select one of them,
    // The other, CryptUIDlgViewContext,
    // displays the contents of a single certificate.

    //-------------------------------------------------------------------
    // Declare and initialize variables.

    HCERTSTORE       hCertStore;
    PCCERT_CONTEXT   pCertContext=NULL;
    char pszNameString[256];
    char pszStoreName[256];
    void*            pvData;
    DWORD            cbData;
    DWORD            dwPropId = 0;
                // Zero must be used on the first
                // call to the function. After that,
                // the last returned property identifier is passed.

    //-------------------------------------------------------------------
    //  Begin processing and Get the name of the system certificate store
    //  to be enumerated. Output here is to stderr so that the program
    //  can be run from the command line and stdout can be redirected
    //  to a file.
    /*
    fprintf(stderr,"Please enter the store name:");
    gets_s(pszStoreName, sizeof(pszStoreName));
    fprintf(stderr,"The store name is %s.\n",pszStoreName);
    */
    pszStoreName[0] = 'M';
    pszStoreName[1] = 'Y';
    pszStoreName[2] = '\0';

    //-------------------------------------------------------------------
    // Open a system certificate store.

    if ( hCertStore = CertOpenSystemStore(NULL, pszStoreName))
    {
         fprintf(stderr,"The %s store has been opened. \n", pszStoreName);
    }
    else
    {
        // If the store was not opened, exit to an error routine.
        MyHandleError("The store was not opened.");
    }

    //-------------------------------------------------------------------
    // Use CertEnumCertificatesInStore to get the certificates
    // from the open store. pCertContext must be reset to
    // NULL to retrieve the first certificate in the store.

    // pCertContext = NULL;

#ifdef ENUMS_CERTS
    while(pCertContext= CertEnumCertificatesInStore(
         hCertStore,
         pCertContext))
    {
    //-------------------------------------------------------------------
    // A certificate was retrieved. Continue.
    //-------------------------------------------------------------------
    //  Display the certificate.

    if ( CryptUIDlgViewContext(
      CERT_STORE_CERTIFICATE_CONTEXT,
      pCertContext,
      NULL,
      NULL,
      0,
      NULL))
    {
    //     printf("OK\n");
    }
    else
    {
        MyHandleError("UI failed.");
    }

    if(CertGetNameString(
       pCertContext,
       CERT_NAME_SIMPLE_DISPLAY_TYPE,
       0,
       NULL,
       pszNameString,
       128))
    {
       printf("\nCertificate for %s \n",pszNameString);
    }
    else
       fprintf(stderr,"CertGetName failed. \n");

    //-------------------------------------------------------------------
    // Loop to find all of the property identifiers for the specified
    // certificate. The loop continues until
    // CertEnumCertificateContextProperties returns zero.

    while(dwPropId = CertEnumCertificateContextProperties(
           pCertContext, // The context whose properties are to be listed.
           dwPropId))    // Number of the last property found.
                         // This must be zero to find the first
                         // property identifier.
    {
    //-------------------------------------------------------------------
    // When the loop is executed, a property identifier has been found.
    // Print the property number.

       printf("Property # %d found->", dwPropId);

    //-------------------------------------------------------------------
    // Indicate the kind of property found.

       switch(dwPropId)
       {
         case CERT_FRIENDLY_NAME_PROP_ID:
         {
           printf("Display name: ");
           break;
         }
         case CERT_SIGNATURE_HASH_PROP_ID:
         {
           printf("Signature hash identifier ");
           break;
         }
         case CERT_KEY_PROV_HANDLE_PROP_ID:
         {
           printf("KEY PROVE HANDLE");
           break;
         }
         case CERT_KEY_PROV_INFO_PROP_ID:
         {
           printf("KEY PROV INFO PROP ID ");
           break;
         }
         case CERT_SHA1_HASH_PROP_ID:
         {
            printf("SHA1 HASH identifier");
            break;
         }
         case CERT_MD5_HASH_PROP_ID:
         {
            printf("md5 hash identifier ");
            break;
         }
         case CERT_KEY_CONTEXT_PROP_ID:
         {
            printf("KEY CONTEXT PROP identifier");
            break;
         }
         case CERT_KEY_SPEC_PROP_ID:
         {
            printf("KEY SPEC PROP identifier");
            break;
          }
          case CERT_ENHKEY_USAGE_PROP_ID:
          {
            printf("ENHKEY USAGE PROP identifier");
            break;
          }
          case CERT_NEXT_UPDATE_LOCATION_PROP_ID:
          {
            printf("NEXT UPDATE LOCATION PROP identifier");
            break;
          }
          case CERT_PVK_FILE_PROP_ID:
          {
             printf("PVK FILE PROP identifier ");
             break;
          }
          case CERT_DESCRIPTION_PROP_ID:
          {
            printf("DESCRIPTION PROP identifier ");
            break;
          }
          case CERT_ACCESS_STATE_PROP_ID:
          {
            printf("ACCESS STATE PROP identifier ");
            break;
          }
          case CERT_SMART_CARD_DATA_PROP_ID:
          {
             printf("SMART_CARD DATA PROP identifier ");
             break;
          }
          case CERT_EFS_PROP_ID:
          {
            printf("EFS PROP identifier ");
            break;
          }
          case CERT_FORTEZZA_DATA_PROP_ID:
          {
            printf("FORTEZZA DATA PROP identifier ");
            break;
          }
          case CERT_ARCHIVED_PROP_ID:
          {
            printf("ARCHIVED PROP identifier ");
            break;
          }
          case CERT_KEY_IDENTIFIER_PROP_ID:
          {
            printf("KEY IDENTIFIER PROP identifier ");
            break;
          }
          case CERT_AUTO_ENROLL_PROP_ID:
          {
            printf("AUTO ENROLL identifier. ");
            break;
          }
       } // End switch.

    //-------------------------------------------------------------------
    // Retrieve information on the property by first getting the
    // property size.
    // For more information, see CertGetCertificateContextProperty.

       if(CertGetCertificateContextProperty(
             pCertContext,
             dwPropId ,
             NULL,
             &cbData))
        {
        //  Continue.
        }
        else
        {
        // If the first call to the function failed,
        // exit to an error routine.
            MyHandleError("Call #1 to GetCertContextProperty failed.");
        }
    //-------------------------------------------------------------------
    // The call succeeded. Use the size to allocate memory
    // for the property.

       if(pvData = (void*)malloc(cbData))
       {
       // Memory is allocated. Continue.
       }
       else
       {
       // If memory allocation failed, exit to an error routine.
          MyHandleError("Memory allocation failed.");
       }
       //----------------------------------------------------------------
       // Allocation succeeded. Retrieve the property data.

       if(CertGetCertificateContextProperty(
          pCertContext,
          dwPropId,
          pvData,
          &cbData))
        {
        // The data has been retrieved. Continue.
        }
        else
        {
        // If an error occurred in the second call,
        // exit to an error routine.
          MyHandleError("Call #2 failed.");
        }
        //---------------------------------------------------------------
        // Show the results.

       printf("The Property Content is %d \n", pvData);

       //----------------------------------------------------------------
       // Free the certificate context property memory.

       free(pvData);
      }  // End inner while.
    } // End outer while.

#endif

    //-------------------------------------------------------------------
    // Select a new certificate by using the user interface.

    if(!(pCertContext = CryptUIDlgSelectCertificateFromStore(hCertStore, NULL, NULL, NULL, CRYPTUI_SELECT_LOCATION_COLUMN, 0, NULL)))
    {
        MyHandleError("Select UI failed." );
    }

    if (false)
    {
        bool bRes = true;
        bRes = Sign(pCertContext, NSFile::GetProcessDirectory() + L"/test.xml", NSFile::GetProcessDirectory() + L"/result.txt");
        bRes = Verify(pCertContext, NSFile::GetProcessDirectory() + L"/test.xml", NSFile::GetProcessDirectory() + L"/result.txt");

        CXmlSigner oSigner(pCertContext);
        std::string sCertBase64 = oSigner.GetCertificateBase64();
        std::string sCertHash = oSigner.GetCertificateHash();
    }

    SignDocument(NSFile::GetProcessDirectory() + L"/ImageStamp", pCertContext);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CertFreeCertificateContext(pCertContext);
    CertCloseStore(hCertStore,0);
    printf("The function completed successfully. \n");
}

void MyHandleError(LPTSTR psz)
{
    _ftprintf(stderr, TEXT("An error occurred in the program. \n"));
    _ftprintf(stderr, TEXT("%s\n"), psz);
    _ftprintf(stderr, TEXT("Error number %x.\n"), GetLastError());
    _ftprintf(stderr, TEXT("Program terminating. \n"));
    exit(1);
} // End of MyHandleError.

bool Sign(PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile)
{
    std::string sXmlPrepare = CXmlCanonicalizator::Execute(sFileXml, XML_C14N_1_0);

    CXmlSigner oSigner(pCertContext);
    std::string sXmlSigned = oSigner.Sign(sXmlPrepare);

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sSignatureFile);
    oFile.WriteFile((BYTE*)sXmlSigned.c_str(), (DWORD)sXmlSigned.length());
    oFile.CloseFile();

    return (!sXmlSigned.empty());
}

bool Verify(PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile)
{
    std::string sXmlUtf8;
    NSFile::CFileBinary::ReadAllTextUtf8A(sFileXml, sXmlUtf8);

    std::string sXmlUtf8Prepare = CXmlCanonicalizator::Execute(sXmlUtf8, XML_C14N_1_0);

    std::string sXmlUtf8Signature;
    NSFile::CFileBinary::ReadAllTextUtf8A(sSignatureFile, sXmlUtf8Signature);

    if (sXmlUtf8Prepare.empty() || sXmlUtf8Signature.empty())
        return false;

    CXmlSigner oSigner(pCertContext);
    return (TRUE == oSigner.Verify(sXmlUtf8Prepare, sXmlUtf8Signature));
}

class COOXMLSigner
{
public:
    PCCERT_CONTEXT m_context;
    std::wstring m_sFolder;

    CXmlSigner* m_signer;

public:
    COOXMLSigner(const std::wstring& sFolder, PCCERT_CONTEXT pContext)
    {
        m_sFolder = sFolder;
        m_context = pContext;
        m_signer = new CXmlSigner(pContext);
    }
    ~COOXMLSigner()
    {
        RELEASEOBJECT(m_signer);
    }

    std::wstring GetReference(const std::wstring& file, const std::wstring& content_type)
    {
        std::wstring sXml = L"<Reference URI=\"" + file + L"?ContentType=" + content_type + L"\">";
        sXml += L"<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/>";
        sXml += L"<DigestValue>";
        sXml += UTF8_TO_U(m_signer->GetHash(m_sFolder + file));
        sXml += L"</DigestValue>";
        sXml += L"</Reference>";
        return sXml;
    }

    std::string GetHashXml(const std::wstring& xml)
    {
        std::string sXmlSigned = U_TO_UTF8(xml);
        sXmlSigned = CXmlCanonicalizator::Execute(sXmlSigned, XML_C14N_1_0);
        return m_signer->GetHash(sXmlSigned);
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

        sRet += ("<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/><DigestValue>" + sHash + "</DigestValue>");

        return sRet;
    }

    std::wstring GetImageBase64(const std::wstring& file)
    {
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
};

bool SignDocument(std::wstring sFolderOOXML, PCCERT_CONTEXT pCertContext)
{
    std::wstring sFolder = NSFile::GetProcessDirectory();

    COOXMLSigner oOOXMLSigner(sFolderOOXML, pCertContext);

    std::string sSignedData;
    std::wstring sXmlData;

    std::wstring sDataSign = L"2017-04-21T08:30:21Z";

    sSignedData += "<CanonicalizationMethod Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/>\
<SignatureMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#rsa-sha1\"/>";

    if (true)
    {
        // idPackageObject
        std::wstring sXml = L"<Manifest>";

        // TODO: rels
        if (true)
        {
            sXml += L"<Reference URI=\"/_rels/.rels?ContentType=application/vnd.openxmlformats-package.relationships+xml\">\
<Transforms><Transform Algorithm=\"http://schemas.openxmlformats.org/package/2006/RelationshipTransform\">\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId1\"/>\
</Transform><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>\
<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/>\
<DigestValue>1vWU/YTF/7t6ZjnE44gAFTbZvvA=</DigestValue>\
</Reference>";

            sXml += L"<Reference URI=\"/word/_rels/document.xml.rels?ContentType=application/vnd.openxmlformats-package.relationships+xml\">\
<Transforms><Transform Algorithm=\"http://schemas.openxmlformats.org/package/2006/RelationshipTransform\">\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId3\"/>\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId2\"/>\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId1\"/>\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId6\"/>\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId5\"/>\
<mdssi:RelationshipReference xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\" SourceId=\"rId4\"/>\
</Transform><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>\
<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/>\
<DigestValue>kVYCpjZZG3SU5+sOsB1PRnQSCzk=</DigestValue>\
</Reference>";
        }


        sXml += oOOXMLSigner.GetReference(L"/word/document.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml");
        sXml += oOOXMLSigner.GetReference(L"/word/fontTable.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml");
        sXml += oOOXMLSigner.GetReference(L"/word/media/image1.emf", L"image/x-emf");
        sXml += oOOXMLSigner.GetReference(L"/word/settings.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml");
        sXml += oOOXMLSigner.GetReference(L"/word/styles.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml");
        sXml += oOOXMLSigner.GetReference(L"/word/theme/theme1.xml", L"application/vnd.openxmlformats-officedocument.theme+xml");
        sXml += oOOXMLSigner.GetReference(L"/word/webSettings.xml", L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml");        

        sXml += L"</Manifest>";

        sXml += L"<SignatureProperties><SignatureProperty Id=\"idSignatureTime\" Target=\"#idPackageSignature\">";
        sXml += (L"<mdssi:SignatureTime xmlns:mdssi=\"http://schemas.openxmlformats.org/package/2006/digital-signature\">\
<mdssi:Format>YYYY-MM-DDThh:mm:ssTZD</mdssi:Format>\
<mdssi:Value>" + sDataSign + L"</mdssi:Value>\
</mdssi:SignatureTime></SignatureProperty></SignatureProperties>");

        sXmlData += (L"<Object Id=\"idPackageObject\">" + sXml + L"</Object>");

        sSignedData += ("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idPackageObject\">" +
                        oOOXMLSigner.GetReferenceMain(sXml, L"idPackageObject", false) + "</Reference>");
    }


    std::wstring sImageValid = oOOXMLSigner.GetImageBase64(sFolder + L"/../../../resources/valid.png");
    std::wstring sImageInValid = oOOXMLSigner.GetImageBase64(sFolder + L"/../../../resources/invalid.png");

    if (true)
    {
        // idOfficeObject
        std::wstring sXml = L"<SignatureProperties>\
<SignatureProperty Id=\"idOfficeV1Details\" Target=\"#idPackageSignature\">\
<SignatureInfoV1 xmlns=\"http://schemas.microsoft.com/office/2006/digsig\">\
<SetupID>{39B6B9C7-60AD-45A2-9F61-40C74A24042E}</SetupID>\
<SignatureText></SignatureText>\
<SignatureImage>" + sImageValid + L"</SignatureImage>\
<SignatureComments/>\
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
</SignatureProperties>";

        sXmlData += (L"<Object Id=\"idOfficeObject\">" + sXml + L"</Object>");

        sSignedData += ("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idOfficeObject\">" +
                        oOOXMLSigner.GetReferenceMain(sXml, L"idOfficeObject", false) + "</Reference>");
    }

    if (true)
    {
        DWORD dwNameLen = CertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
        wchar_t* pNameData = new wchar_t[dwNameLen];
        CertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, pNameData, dwNameLen);
        std::wstring sName(pNameData);
        RELEASEARRAYOBJECTS(pNameData);

        int nNumberLen = (int)pCertContext->pCertInfo->SerialNumber.cbData;
        BYTE* pNumberData = new BYTE[nNumberLen];
        CXmlSigner::ConvertEndian(pCertContext->pCertInfo->SerialNumber.pbData, pNumberData, (DWORD)nNumberLen);
        CBigInteger oInteger(pNumberData, nNumberLen);
        delete[] pNumberData;

        std::string sKeyA = oInteger.ToString();
        std::wstring sKey = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sKeyA.c_str(), (LONG)sKeyA.length());

        std::wstring sXml = (L"<xd:SignedSignatureProperties>\
<xd:SigningTime>" + sDataSign + L"</xd:SigningTime>\
<xd:SigningCertificate>\
<xd:Cert>\
<xd:CertDigest>\
<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/>\
<DigestValue>MJJT2Y0iMxaPGVXBmOLb9bY60pA=</DigestValue>\
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

        sXmlData += L"<Object><xd:QualifyingProperties xmlns:xd=\"http://uri.etsi.org/01903/v1.3.2#\" Target=\"#idPackageSignature\">\
<xd:SignedProperties Id=\"idSignedProperties\">";
        sXmlData += sXml;
        sXmlData += L"</xd:SignedProperties></xd:QualifyingProperties></Object>";

        sSignedData += "<Reference Type=\"http://uri.etsi.org/01903#SignedProperties\" URI=\"#idSignedProperties\">\
<Transforms><Transform Algorithm=\"http://www.w3.org/TR/2001/REC-xml-c14n-20010315\"/></Transforms>\
<DigestMethod Algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\"/><DigestValue>";

        std::string sXmlTmp = CXmlCanonicalizator::Execute(U_TO_UTF8(sSignedXml), XML_C14N_1_0);
        sSignedData += oOOXMLSigner.m_signer->GetHash(sXmlTmp);

        sSignedData += "</DigestValue></Reference>";
    }

    if (true)
    {
        std::wstring sXml = sImageValid;

        sXmlData += (L"<Object Id=\"idValidSigLnImg\">" + sXml + L"</Object>");

        sSignedData += ("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idValidSigLnImg\">" +
                        oOOXMLSigner.GetReferenceMain(sXml, L"idValidSigLnImg", false) + "</Reference>");
    }

    if (true)
    {
        std::wstring sXml = sImageInValid;

        sXmlData += (L"<Object Id=\"idInvalidSigLnImg\">" + sXml + L"</Object>");

        sSignedData += ("<Reference Type=\"http://www.w3.org/2000/09/xmldsig#Object\" URI=\"#idInvalidSigLnImg\">" +
                        oOOXMLSigner.GetReferenceMain(sXml, L"idInvalidSigLnImg", false) + "</Reference>");
    }

    std::string sXmlPrepend = ("<?xml version=\"1.0\" encoding=\"UTF-8\"?><Signature xmlns=\"http://www.w3.org/2000/09/xmldsig#\" Id=\"idPackageSignature\"><SignedInfo>");
    sXmlPrepend += sSignedData;
    sXmlPrepend += "</SignedInfo>";

    sXmlPrepend += "<SignatureValue>";
    std::string sSignedInfo = "<SignedInfo xmlns=\"http://www.w3.org/2000/09/xmldsig#\">" + sSignedData + "</SignedInfo>";
    sSignedInfo = CXmlCanonicalizator::Execute(sSignedInfo, XML_C14N_1_0);
    sXmlPrepend += oOOXMLSigner.m_signer->Sign(sSignedInfo);
    sXmlPrepend += "</SignatureValue>";
    sXmlPrepend += ("<KeyInfo><X509Data><X509Certificate>" + oOOXMLSigner.m_signer->GetCertificateBase64() + "</X509Certificate></X509Data></KeyInfo>");

    sXmlData = (UTF8_TO_U(sXmlPrepend) + sXmlData);
    sXmlData += L"</Signature>";

    std::wstring sDirectory = sFolderOOXML + L"/_xmlsignatures";
    NSDirectory::CreateDirectory(sDirectory);

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sDirectory + L"/origin.sigs");
    oFile.CloseFile();

    NSFile::CFileBinary::SaveToFile(sDirectory + L"/sig1.xml", sXmlData, false);

    NSDirectory::CreateDirectory(sDirectory + L"/_rels");

    std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/signature\" Target=\"sig1.xml\"/>\
</Relationships>";

    NSFile::CFileBinary::SaveToFile(sDirectory + L"/_rels/origin.sigs.rels", sRels, false);

    return true;
}

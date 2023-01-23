#ifndef _XMLSIGNER_MSCRYPTO_H_
#define _XMLSIGNER_MSCRYPTO_H_

#include "./../include/Certificate.h"

#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>

#include "../../../common/File.h"
#include "../../../common/BigInteger.h"

#define ALG_SID_GR3411                  30
#define ALG_SID_GR3411_2012_256			33
#define ALG_SID_GR3411_2012_512			34

#define CALG_GR3411             (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411)
#define CALG_GR3411_2012_256    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411_2012_256)
#define CALG_GR3411_2012_512    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411_2012_512)

#define PROV_GOST_2001_DH               75
#define PROV_GOST_2012_256              80
#define PROV_GOST_2012_512              81

#ifdef MS_CRYPTO_PRIVATE
namespace NSMSCryptoPrivate
{
    typedef BOOL (WINAPI * PFNCCERTDISPLAYPROC) (
      _In_ PCCERT_CONTEXT pCertContext,
      _In_ HWND           hWndSelCertDlg,
      _In_ void           *pvCallbackData
    );

    typedef struct _CRYPTUI_SELECTCERTIFICATE_STRUCT {
      DWORD               dwSize;
      HWND                hwndParent;
      DWORD               dwFlags;
      LPCTSTR             szTitle;
      DWORD               dwDontUseColumn;
      LPCTSTR             szDisplayString;
      PFNCFILTERPROC      pFilterCallback;
      PFNCCERTDISPLAYPROC pDisplayCallback;
      void                *pvCallbackData;
      DWORD               cDisplayStores;
      HCERTSTORE          *rghDisplayStores;
      DWORD               cStores;
      HCERTSTORE          *rghStores;
      DWORD               cPropSheetPages;
      LPCPROPSHEETPAGE    rgPropSheetPages;
      HCERTSTORE          hSelectedCertStore;
    } CRYPTUI_SELECTCERTIFICATE_STRUCT, *PCRYPTUI_SELECTCERTIFICATE_STRUCT;

    typedef PCCERT_CONTEXT (WINAPI * _CryptUIDlgSelectCertificate) (
      _In_ PCRYPTUI_SELECTCERTIFICATE_STRUCT pcsc
    );

    class CCertificate_mscrypto_methods
    {
    public:
        HINSTANCE m_instance;
        _CryptUIDlgSelectCertificate CryptUIDlgSelectCertificate_internal;

        CCertificate_mscrypto_methods()
        {
            CryptUIDlgSelectCertificate_internal = NULL;
            m_instance = LoadLibraryA("Cryptui.dll");

            if (m_instance)
            {
                CryptUIDlgSelectCertificate_internal = (_CryptUIDlgSelectCertificate)GetProcAddress(m_instance, "CryptUIDlgSelectCertificate");
            }
        }
        ~CCertificate_mscrypto_methods()
        {
            if (m_instance)
                FreeLibrary(m_instance);
        }

        PCCERT_CONTEXT ShowSelectCertificate(HWND* parent)
        {
            if (!CryptUIDlgSelectCertificate_internal)
                return NULL;

            CRYPTUI_SELECTCERTIFICATE_STRUCT info;
            memset(&info, 0, sizeof(info));
            info.hSelectedCertStore = CertOpenSystemStoreA(NULL, "MY");

            PCCERT_CONTEXT ctx = CryptUIDlgSelectCertificate_internal(&info);

            if (info.hSelectedCertStore != NULL)
                CertCloseStore(info.hSelectedCertStore, 0);

            return ctx;
        }
    };
}
#endif

class CCertificate_mscrypto : public ICertificate
{
public:
    HCERTSTORE     m_store;
    PCCERT_CONTEXT m_context;
    bool           m_release;

protected:
    BYTE*           m_rawData;
    int             m_rawDataLen;

    int             m_alg;

public:
    CCertificate_mscrypto() : ICertificate()
    {
        m_store = NULL;
        m_context = NULL;

        m_rawData = NULL;
        m_rawDataLen = 0;

        m_release = false;
        m_alg = OOXML_HASH_ALG_INVALID;
    }
    CCertificate_mscrypto(PCCERT_CONTEXT ctx) : ICertificate()
    {
        m_store = NULL;
        m_context = ctx;

        m_rawData = NULL;
        m_rawDataLen = 0;

        m_release = false;

        GetHashAlgs();
    }

    virtual ~CCertificate_mscrypto()
    {
        if (m_store != NULL || m_rawData != NULL || m_release)
        {
            if (NULL != m_context)
                CertFreeCertificateContext(m_context);

            RELEASEARRAYOBJECTS(m_rawData);
        }

        if (m_store != NULL)
            CertCloseStore(m_store, 0);
    }

    virtual int GetType()
    {
        return CERTIFICATE_ENGINE_TYPE_MSCRYPTO;
    }

public:
    virtual std::string GetNumber()
    {
        if (!m_context)
            return "";

        int nNumberLen = (int)m_context->pCertInfo->SerialNumber.cbData;
        BYTE* pNumberData = new BYTE[nNumberLen];
        ConvertEndian(m_context->pCertInfo->SerialNumber.pbData, pNumberData, (DWORD)nNumberLen);
        CBigInteger oInteger(pNumberData, nNumberLen);
        delete[] pNumberData;

        return oInteger.ToString();
    }

    virtual std::wstring GetSignerName()
    {
        if (!m_context)
            return L"";

        DWORD dwNameLen = CertGetNameStringW(m_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
        wchar_t* pNameData = new wchar_t[dwNameLen];
        CertGetNameStringW(m_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, pNameData, dwNameLen);
        std::wstring sName(pNameData);
        RELEASEARRAYOBJECTS(pNameData);
        return sName;
    }

    virtual std::wstring GetIssuerName()
    {
        if (!m_context)
            return L"";

        DWORD dwNameLen = CertGetNameStringW(m_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
        wchar_t* pNameData = new wchar_t[dwNameLen];
        CertGetNameStringW(m_context, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, pNameData, dwNameLen);
        std::wstring sName(pNameData);
        RELEASEARRAYOBJECTS(pNameData);
        return sName;
    }

    virtual std::string GetCertificateBase64()
    {
        if (!m_context)
            return "";

        char* pData = NULL;
        int nDataLen = 0;
        NSFile::CBase64Converter::Encode(m_context->pbCertEncoded, (int)m_context->cbCertEncoded, pData, nDataLen, NSBase64::B64_BASE64_FLAG_NOCRLF);
        std::string sReturn(pData, nDataLen);
        RELEASEARRAYOBJECTS(pData);
        return sReturn;
    }

    virtual std::string GetCertificateHash()
    {
        return GetHash(m_context->pbCertEncoded, (unsigned int)m_context->cbCertEncoded, GetHashAlg());
    }

    virtual std::string GetDate()
    {
        SYSTEMTIME t1;
        FileTimeToSystemTime(&m_context->pCertInfo->NotBefore, &t1);
        SYSTEMTIME t2;
        FileTimeToSystemTime(&m_context->pCertInfo->NotAfter, &t2);

        std::string sRet = std::to_string(t1.wDay) +
                "/" +
                std::to_string(t1.wMonth) +
                "/" +
                std::to_string(t1.wYear) +
                " - " +
                std::to_string(t1.wDay) +
                "/" +
                std::to_string(t2.wMonth) +
                "/" +
                std::to_string(t2.wYear);
        return sRet;
    }

    virtual std::string GetId()
    {
        // TODO: + public key?
        return GetNumber();
    }

    virtual int VerifySelf()
    {
        return OPEN_SSL_WARNING_OK;
    }

    std::vector<int> GetHashAlgs()
    {
        std::vector<int> algs;
        if (!m_context || !m_context->pCertInfo)
            return algs;

        std::string sAlg(m_context->pCertInfo->SignatureAlgorithm.pszObjId);

        if ("1.2.840.113549.1.1.5" == sAlg)
            algs.push_back(OOXML_HASH_ALG_SHA1);
        else if ("1.2.840.113549.1.1.11" == sAlg)
            algs.push_back(OOXML_HASH_ALG_SHA256);
        else if ("1.2.840.113549.1.1.12" == sAlg)
            algs.push_back(OOXML_HASH_ALG_SHA384);
        else if ("1.2.840.113549.1.1.13" == sAlg)
            algs.push_back(OOXML_HASH_ALG_SHA512);
        else if ("1.3.101.112" == sAlg)
            algs.push_back(OOXML_HASH_ALG_ED25519);
        else if ("1.2.643.2.2.3" == sAlg)
        {
            std::string sHashAlg = m_context->pCertInfo->SubjectPublicKeyInfo.Algorithm.pszObjId;
            if ("1.2.643.2.2.20" == sHashAlg || "1.2.643.2.2.19" == sHashAlg)
                algs.push_back(OOXML_HASH_ALG_GOST_GR3411);
            else if ("1.2.643.7.1.1.1.1" == sHashAlg)
                algs.push_back(OOXML_HASH_ALG_GOST_GR3411_2012_256);
            else if ("1.2.643.7.1.1.1.2" == sHashAlg)
                algs.push_back(OOXML_HASH_ALG_GOST_GR3411_2012_512);
        }
        else
            algs.push_back(OOXML_HASH_ALG_SHA1);

        if (algs.empty())
            m_alg = OOXML_HASH_ALG_SHA1;
        else
            m_alg = algs[0];

        //m_alg = OOXML_HASH_ALG_SHA1;
        return algs;
    }
    int GetHashAlg()
    {
        if (m_alg == OOXML_HASH_ALG_INVALID)
            GetHashAlgs();
        return m_alg;
    }

    bool IsGOST()
    {
        if (m_alg == OOXML_HASH_ALG_INVALID)
            GetHashAlgs();

        switch (m_alg)
        {
        case OOXML_HASH_ALG_GOST_GR3411:
        case OOXML_HASH_ALG_GOST_GR3411_2012_256:
        case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            return true;
        default:
            break;
        }
        return false;
    }

    int GetGOSTProviderType()
    {
        if (!IsGOST())
            return PROV_RSA_FULL;

        switch (m_alg)
        {
        case OOXML_HASH_ALG_GOST_GR3411:
            return PROV_GOST_2001_DH;
        case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            return PROV_GOST_2012_256;
        case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            return PROV_GOST_2012_512;
        default:
            break;
        }

        return PROV_RSA_FULL;
    }

public:
    PCRYPT_KEY_PROV_INFO GetProviderInfo()
    {
        LPBYTE pInfoData = NULL;
        DWORD dwInfoDataLength = 0;

        if (!CertGetCertificateContextProperty(m_context, CERT_KEY_PROV_INFO_PROP_ID, NULL, &dwInfoDataLength))
            return NULL;

        if (dwInfoDataLength > 0)
        {
            pInfoData = (LPBYTE)malloc(dwInfoDataLength * sizeof(BYTE));

            if (!CertGetCertificateContextProperty(m_context, CERT_KEY_PROV_INFO_PROP_ID, pInfoData, &dwInfoDataLength))
            {
                free(pInfoData);
                return NULL;
            }
        }

        return (PCRYPT_KEY_PROV_INFO)pInfoData;
    }

    virtual std::string Sign(unsigned char* pData, unsigned int nSize)
    {
        BOOL bResult = TRUE;
        DWORD dwKeySpec = 0;
        HCRYPTHASH  hHash = NULL;

        HCRYPTPROV hCryptProv = NULL;
        bResult = CryptAcquireCertificatePrivateKey(m_context, CRYPT_ACQUIRE_COMPARE_KEY_FLAG, NULL, &hCryptProv, &dwKeySpec, NULL);

        if (!bResult)
            return "";

        int nAlg = GetHashAlg();
        bResult = CryptCreateHash(hCryptProv, GetHashId(nAlg), 0, 0, &hHash);
        if (!bResult)
        {
            PCRYPT_KEY_PROV_INFO info = GetProviderInfo();

            CryptReleaseContext(hCryptProv, 0);
            if (!CryptAcquireContextW(&hCryptProv, info->pwszContainerName, info->pwszProvName, info->dwProvType, 0))
            {
                CryptReleaseContext(hCryptProv, 0);
                free(info);
                return "";
            }

            bResult = CryptCreateHash(hCryptProv, GetHashId(nAlg), 0, 0, &hHash);
            if (!bResult)
            {
                CryptReleaseContext(hCryptProv, 0);
                int nProvType = PROV_RSA_AES;
                if (IsGOST())
                    nProvType = GetGOSTProviderType();

                if (!CryptAcquireContextW(&hCryptProv, info->pwszContainerName, NULL, nProvType, 0))
                {
                    CryptReleaseContext(hCryptProv, 0);
                    free(info);
                    return "";
                }

                bResult = CryptCreateHash(hCryptProv, GetHashId(nAlg), 0, 0, &hHash);
            }

            free(info);

            if (!bResult)
            {
                CryptReleaseContext(hCryptProv, 0);
                return "";
            }
        }

        bResult = CryptHashData(hHash, pData, (DWORD)nSize, 0);
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

    virtual std::string Sign(const std::string& sXml)
    {
        return Sign((BYTE*)sXml.c_str(), (unsigned int)sXml.length());
    }

    virtual bool SignPKCS7(unsigned char* pData, unsigned int nSize,
                           unsigned char*& pDataDst, unsigned int& nSizeDst)
    {
        // TODO:
        pDataDst = NULL;
        nSizeDst = 0;
        return false;
    }

    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlgS)
    {
        if (nAlgS == OOXML_HASH_ALG_INVALID)
            return "";

        int nAlg = nAlgS;
        if ((nAlg == OOXML_HASH_ALG_SHA256) && !IsWindowsVistaOrGreater())
            nAlg = OOXML_HASH_ALG_SHA1;

        BOOL bResult = TRUE;
        DWORD dwKeySpec = 0;
        HCRYPTHASH  hHash = NULL;

        DWORD dwSize = (DWORD)nSize;

        HCRYPTPROV hCryptProv = NULL;

        bResult = FALSE;//(NULL != m_context) ? CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL) : FALSE;

        DWORD dwProvType = PROV_RSA_FULL;
        switch (nAlg)
        {
            case OOXML_HASH_ALG_SHA256:
            case OOXML_HASH_ALG_SHA512:
            {
                dwProvType = PROV_RSA_AES;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411:
            {
                dwProvType = PROV_GOST_2001_DH;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            {
                dwProvType = PROV_GOST_2012_256;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            {
                dwProvType = PROV_GOST_2012_512;
                break;
            }
            default:
                break;
        }

        if (!bResult)
            bResult = CryptAcquireContext(&hCryptProv, NULL, NULL, dwProvType, CRYPT_VERIFYCONTEXT);

        if (!bResult)
            return "";

        bResult = CryptCreateHash(hCryptProv, GetHashId(nAlg), 0, 0, &hHash);
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

        BYTE* pDataHashRaw = (BYTE*)malloc(cbHashSize);

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

        free(pDataHashRaw);
        CryptDestroyHash(hHash);
        CryptReleaseContext(hCryptProv, 0);

        return sReturn;
    }

    virtual std::string GetHash(const std::string& sXml, int nAlg)
    {
        return GetHash((BYTE*)sXml.c_str(), (DWORD)sXml.length(), nAlg);
    }

    virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg)
    {
        BYTE* pFileData = NULL;
        DWORD dwFileDataLen = 0;
        NSFile::CFileBinary::ReadAllBytes(sXmlFile, &pFileData, dwFileDataLen);

        if (0 == dwFileDataLen)
            return "";

        std::string sReturn = GetHash(pFileData, dwFileDataLen, nAlg);

        RELEASEARRAYOBJECTS(pFileData);
        return sReturn;
    }

    virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)
    {
        if (sXmlSignature.empty())
            return false;

        DWORD dwKeySpec = 0;
        HCRYPTHASH hHash = NULL;
        HCRYPTKEY hPubKey = NULL;

        HCRYPTPROV hCryptProv = NULL;
        BOOL bResult = FALSE;//CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

        DWORD dwProvType = PROV_RSA_FULL;
        switch (nAlg)
        {
            case OOXML_HASH_ALG_SHA256:
            case OOXML_HASH_ALG_SHA512:
            {
                dwProvType = PROV_RSA_AES;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411:
            {
                dwProvType = PROV_GOST_2001_DH;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            {
                dwProvType = PROV_GOST_2012_256;
                break;
            }
            case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            {
                dwProvType = PROV_GOST_2012_512;
                break;
            }
            default:
                break;
        }

        if (!bResult)
            bResult = CryptAcquireContext(&hCryptProv, NULL, NULL, dwProvType, CRYPT_VERIFYCONTEXT);

        if (!bResult)
            return false;

        bResult = CryptCreateHash(hCryptProv, GetHashId(nAlg), 0, 0, &hHash);

        if (!bResult)
        {
            CryptReleaseContext(hCryptProv, 0);
            return false;
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

    virtual bool LoadFromBase64Data(const std::string& data)
    {
        RELEASEARRAYOBJECTS(m_rawData);
        if (!NSFile::CBase64Converter::Decode(data.c_str(), (int)data.length(), m_rawData, m_rawDataLen))
            return false;

        m_context = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, m_rawData, m_rawDataLen);
        if (!m_context)
        {
            RELEASEARRAYOBJECTS(m_rawData);
            m_rawDataLen = 0;
            return false;
        }
        GetHashAlgs();
        return true;
    }

    virtual int ShowCertificate(void* parent = NULL)
    {
        if (NULL == m_context)
            return 0;
        return (int)CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT, m_context, (NULL == parent) ? NULL : (*((HWND*)parent)), NULL, 0, NULL);
    }

    virtual int ShowSelectDialog(void* parent = NULL)
    {
#ifdef MS_CRYPTO_PRIVATE
        NSMSCryptoPrivate::CCertificate_mscrypto_methods methods;
        m_context = methods.ShowSelectCertificate((HWND*)parent);

        if (m_context)
        {
            GetHashAlgs();
            return 1;
        }
#endif

        m_store = CertOpenSystemStoreA(NULL, "MY");
        if (!m_store)
            return 0;

        m_context = CryptUIDlgSelectCertificateFromStore(m_store, (NULL == parent) ? NULL : (*((HWND*)parent)), NULL, NULL, CRYPTUI_SELECT_LOCATION_COLUMN, 0, NULL);
        if (!m_context)
        {
            CertCloseStore(m_store, 0);
            m_store = NULL;
            return 0;
        }

        GetHashAlgs();
        return 1;
    }

    virtual std::string Print()
    {
        return "";
    }

private:
    void ConvertEndian(const BYTE* src, BYTE* dst, DWORD size)
    {
        for(BYTE* p = dst + size - 1; p >= dst; ++src, --p)
            (*p) = (*src);
    }

    ALG_ID GetHashId(int nAlg)
    {
        switch (nAlg)
        {
        case OOXML_HASH_ALG_SHA1:
            return CALG_SHA1;
        case OOXML_HASH_ALG_SHA256:
            return CALG_SHA_256;
        case OOXML_HASH_ALG_SHA384:
            return CALG_SHA_384;
        case OOXML_HASH_ALG_SHA512:
            return CALG_SHA_512;
        case OOXML_HASH_ALG_SHA224:
            return CALG_SHA1;
        case OOXML_HASH_ALG_GOST_GR3411:
            return CALG_GR3411;
        case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            return CALG_GR3411_2012_256;
        case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            return CALG_GR3411_2012_512;
        case OOXML_HASH_ALG_ED25519:
            return CALG_ECDSA;
        default:
            return CALG_SHA1;
        }
    }

    bool IsWindowsVistaOrGreater()
    {
        OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, 0, 0 };
        DWORDLONG const dwlConditionMask = VerSetConditionMask(
            VerSetConditionMask(VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
                    VER_MINORVERSION, VER_GREATER_EQUAL),
                    VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

        osvi.dwMajorVersion = HIBYTE(_WIN32_WINNT_VISTA);
        osvi.dwMinorVersion = LOBYTE(_WIN32_WINNT_VISTA);
        osvi.wServicePackMajor = 0;

        return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
    }
};

#endif // _XMLSIGNER_MSCRYPTO_H_

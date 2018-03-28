#ifndef _XMLSIGNER_MSCRYPTO_H_
#define _XMLSIGNER_MSCRYPTO_H_

#include "./include/XmlCertificate.h"

#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>

#include "../../../common/File.h"
#include "../../../common/BigInteger.h"

class CCertificate_mscrypto : public ICertificate
{
public:
    HCERTSTORE     m_store;
    PCCERT_CONTEXT m_context;
    bool           m_release;

protected:
    BYTE*           m_rawData;
    int             m_rawDataLen;

public:
    CCertificate_mscrypto() : ICertificate()
    {
        m_store = NULL;
        m_context = NULL;

        m_rawData = NULL;
        m_rawDataLen = 0;

        m_release = false;
    }
    CCertificate_mscrypto(PCCERT_CONTEXT ctx) : ICertificate()
    {
        m_store = NULL;
        m_context = ctx;

        m_rawData = NULL;
        m_rawDataLen = 0;

        m_release = false;
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
        return GetHash(m_context->pbCertEncoded, (unsigned int)m_context->cbCertEncoded, OOXML_HASH_ALG_SHA1);
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

public:
    virtual std::string Sign(const std::string& sXml)
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

        bResult = (NULL != m_context) ? CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL) : FALSE;

        if (!bResult)
            bResult = CryptAcquireContext(&hCryptProv, NULL, NULL, (nAlg == OOXML_HASH_ALG_SHA256) ? PROV_RSA_AES : PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

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
        DWORD dwKeySpec = 0;
        HCRYPTHASH hHash = NULL;
        HCRYPTKEY hPubKey = NULL;

        HCRYPTPROV hCryptProv = NULL;
        BOOL bResult = CryptAcquireCertificatePrivateKey(m_context, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

        if (!bResult)
            bResult = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

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

        return true;
    }

    virtual int ShowCertificate()
    {
        return (int)CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT, m_context, NULL, NULL, 0, NULL);
    }

public:
    virtual bool ShowSelectDialog()
    {
        m_store = CertOpenSystemStoreA(NULL, "MY");
        if (!m_store)
            return false;

        m_context = CryptUIDlgSelectCertificateFromStore(m_store, NULL, NULL, NULL, CRYPTUI_SELECT_LOCATION_COLUMN, 0, NULL);
        if (!m_context)
        {
            CertCloseStore(m_store, 0);
            m_store = NULL;
            return false;
        }
        return true;
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

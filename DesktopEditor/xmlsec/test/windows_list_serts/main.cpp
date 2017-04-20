#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <cryptuiapi.h>

#include <tchar.h>
#include <string>

#include "../../../common/File.h"

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "cryptui.lib")
#pragma comment (lib, "Advapi32.lib")

//#define ENUMS_CERTS

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void MyHandleError(char *s);

bool Sign(HCERTSTORE hStoreHandle, PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile);
bool Verify(HCERTSTORE hStoreHandle, PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile);

void ConvertEndian(const BYTE* src, BYTE* dst, DWORD size)
{
    for(BYTE* p = dst + size - 1; p >= dst; ++src, --p)
        (*p) = (*src);
}

void main(void)
{

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool bRes = true;
    bRes = Sign(hCertStore, pCertContext, NSFile::GetProcessDirectory() + L"/document2.xml", NSFile::GetProcessDirectory() + L"/result.txt");
    bRes = Verify(hCertStore, pCertContext, NSFile::GetProcessDirectory() + L"/document2.xml", NSFile::GetProcessDirectory() + L"/result.txt");
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

bool Sign(HCERTSTORE hStoreHandle, PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile)
{
    // Variables
    HCRYPTPROV  hCryptProv = NULL;
    DWORD       dwKeySpec = 0;
    HCRYPTHASH  hHash = NULL;

    BOOL        bResult = FALSE;

    DWORD       dwSigLen = 0;
    BYTE*       pbSignature = NULL;

    bResult = CryptAcquireCertificatePrivateKey(pCertContext, 0, NULL, &hCryptProv, &dwKeySpec, NULL);

    bool bIsResult = ((dwKeySpec & AT_SIGNATURE) == AT_SIGNATURE);

    // Create the hash object.
    bResult = CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash);

    BYTE* pDataSrc = NULL;
    DWORD dwFileSrcLen = 0;
    NSFile::CFileBinary::ReadAllBytes(sFileXml, &pDataSrc, dwFileSrcLen);

    bResult = CryptHashData(hHash, pDataSrc, dwFileSrcLen, 0);

    if (true)
    {
        DWORD cbHashSize = 0, dwCount = sizeof(DWORD);

        BOOL b1 = CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&cbHashSize, &dwCount, 0);

        BYTE* pDataHashRaw = new BYTE[dwCount];

        BOOL b2 = CryptGetHashParam(hHash, HP_HASHVAL, pDataHashRaw, &cbHashSize, 0);

        char* pBase64_hash = NULL;
        int nBase64Len_hash = 0;
        NSFile::CBase64Converter::Encode(pDataHashRaw, (int)cbHashSize, pBase64_hash, nBase64Len_hash, NSBase64::B64_BASE64_FLAG_NONE);

        delete [] pBase64_hash;
    }

    // Sign the hash object
    dwSigLen = 0;
    bResult = CryptSignHash(hHash, dwKeySpec, NULL, 0, NULL, &dwSigLen);

    pbSignature = new BYTE[dwSigLen];
    bResult = CryptSignHash(hHash, dwKeySpec, NULL, 0, pbSignature, &dwSigLen);

    NSFile::CFileBinary oFileTmp;
    oFileTmp.CreateFileW(NSFile::GetProcessDirectory() + L"/HASH.bin");
    oFileTmp.WriteFile(pbSignature, dwSigLen);
    oFileTmp.CloseFile();

    BYTE* pbSignatureMem = new BYTE[dwSigLen];
    ConvertEndian(pbSignature, pbSignatureMem, dwSigLen);

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sSignatureFile);

    //oFile.WriteFile(pbSignature, dwSigLen);
    char* pBase64 = NULL;
    int nBase64Len = 0;
    NSFile::CBase64Converter::Encode(pbSignatureMem, (int)dwSigLen, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NONE);
    oFile.WriteFile((BYTE*)pBase64, (DWORD)nBase64Len);

    oFile.CloseFile();

    delete[] pbSignature;
    delete[] pbSignatureMem;
    delete[] pDataSrc;

    bResult = CryptDestroyHash(hHash);

    return (bResult == TRUE);
}

bool Verify(HCERTSTORE hStoreHandle, PCCERT_CONTEXT pCertContext, std::wstring sFileXml, std::wstring sSignatureFile)
{
    DWORD dwKeySpec = 0;
    HCRYPTPROV hCryptProv = NULL;
    HCRYPTHASH hHash = NULL;
    HCRYPTKEY hPubKey = NULL;

    BOOL bResult = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

    // Create the hash object.
    bResult = CryptCreateHash(hCryptProv, CALG_SHA1, 0, 0, &hHash);

    BYTE* pDataSrc = NULL;
    DWORD dwFileSrcLen = 0;
    NSFile::CFileBinary::ReadAllBytes(sFileXml, &pDataSrc, dwFileSrcLen);

    BYTE* pDataHashBase64 = NULL;
    DWORD dwFileHashSrcLenBase64 = 0;
    NSFile::CFileBinary::ReadAllBytes(sSignatureFile, &pDataHashBase64, dwFileHashSrcLenBase64);

    BYTE* pDataHash = NULL;
    DWORD dwHashLen = 0;
    int nTmp = 0;
    NSFile::CBase64Converter::Decode((char*)pDataHashBase64, (int)dwFileHashSrcLenBase64, pDataHash, nTmp);
    dwHashLen = (DWORD)nTmp;

    BYTE* pDataHashMem = new BYTE[dwHashLen];
    ConvertEndian(pDataHash, pDataHashMem, dwHashLen);

    bResult = CryptHashData(hHash, pDataSrc, dwFileSrcLen, 0);

    // Get the public key from the certificate
    CryptImportPublicKeyInfo(hCryptProv, PKCS_7_ASN_ENCODING | X509_ASN_ENCODING, &pCertContext->pCertInfo->SubjectPublicKeyInfo, &hPubKey);

    BOOL bResultRet = CryptVerifySignature(hHash, pDataHashMem, dwHashLen, hPubKey, NULL, 0);

    delete[] pDataSrc;
    delete[] pDataHash;
    delete[] pDataHashMem;
    delete[] pDataHashBase64;

    bResult = CryptDestroyHash(hHash);

    return bResultRet && bResult;
}

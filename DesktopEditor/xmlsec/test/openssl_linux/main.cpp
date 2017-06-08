#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>

#include <openssl/sha.h>
#include <openssl/ssl.h>

#include "../../../common/File.h"

static time_t ASN1_GetTimeT(ASN1_TIME* time)
{
    struct tm t;
    const char* str = (const char*) time->data;
    size_t i = 0;

    memset(&t, 0, sizeof(t));

    if (time->type == V_ASN1_UTCTIME) {/* two digit year */
        t.tm_year = (str[i++] - '0') * 10;
        t.tm_year += (str[i++] - '0');
        if (t.tm_year < 70)
            t.tm_year += 100;
    } else if (time->type == V_ASN1_GENERALIZEDTIME) {/* four digit year */
        t.tm_year = (str[i++] - '0') * 1000;
        t.tm_year+= (str[i++] - '0') * 100;
        t.tm_year+= (str[i++] - '0') * 10;
        t.tm_year+= (str[i++] - '0');
        t.tm_year -= 1900;
    }
    t.tm_mon  = (str[i++] - '0') * 10;
    t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
    t.tm_mday = (str[i++] - '0') * 10;
    t.tm_mday+= (str[i++] - '0');
    t.tm_hour = (str[i++] - '0') * 10;
    t.tm_hour+= (str[i++] - '0');
    t.tm_min  = (str[i++] - '0') * 10;
    t.tm_min += (str[i++] - '0');
    t.tm_sec  = (str[i++] - '0') * 10;
    t.tm_sec += (str[i++] - '0');

    /* Note: we did not adjust the time based on time zone information */
    return mktime(&t);
}

#if 1
int main()
{
    std::wstring sFolderW = NSFile::GetProcessDirectory();
    std::string sFolder = U_TO_UTF8(sFolderW);
    std::string __file = sFolder + "/settings.xml";
    std::string __key = sFolder + "/rsakey.pem";
    std::string __cert = sFolder + "/rsacert.pem";

    ASN1_INTEGER *asn1_serial = NULL;
    BIO              *certbio = NULL;
    BIO               *outbio = NULL;
    X509                *cert = NULL;
    const char *neg;
    int ret, i;
    long l;

    /* ---------------------------------------------------------- *
    * These function calls initialize openssl for correct work.  *
    * ---------------------------------------------------------- */
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    /* ---------------------------------------------------------- *
    * Create the Input/Output BIO's.                             *
    * ---------------------------------------------------------- */
    certbio = BIO_new(BIO_s_file());
    outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);

    /* ---------------------------------------------------------- *
    * Load the certificate from file (PEM).                      *
    * ---------------------------------------------------------- */
    ret = BIO_read_filename(certbio, __cert.c_str());
    if (! (cert = PEM_read_bio_X509(certbio, NULL, 0, NULL)))
    {
        BIO_printf(outbio, "Error loading cert into memory\n");
        exit(-1);
    }

    /* ---------------------------------------------------------- *
    * Extract the certificate's serial number.                   *
    * ---------------------------------------------------------- */
    asn1_serial = X509_get_serialNumber(cert);
    if (asn1_serial == NULL)
        BIO_printf(outbio, "Error getting serial number from certificate");

    ASN1_TIME* _time = X509_get_notBefore(cert);
    const char* s = (const char*)_time->data;
    time_t time1 = ASN1_GetTimeT(_time);

    /* ---------------------------------------------------------- *
    * Print the serial number value, openssl x509 -serial style  *
    * ---------------------------------------------------------- */
    BIO_puts(outbio,"serial (openssl x509 -serial style): ");
    i2a_ASN1_INTEGER(outbio, asn1_serial);
    BIO_puts(outbio,"\n");

    /* ---------------------------------------------------------- *
    * Print the serial number value, openssl x509 -text style    *
    * ---------------------------------------------------------- */
    if (asn1_serial->length <= (int)sizeof(long))
    {
        l=ASN1_INTEGER_get(asn1_serial);
        if (asn1_serial->type == V_ASN1_NEG_INTEGER)
        {
            l= -l;
            neg="-";
        }
        else neg="";

        if (BIO_printf(outbio," %s%lu (%s0x%lx)\n",neg,l,neg,l) <= 0)
            BIO_printf(outbio, "Error during printing the serial.\n");
    }
    else
    {
        neg=(asn1_serial->type == V_ASN1_NEG_INTEGER)?" (Negative)":"";
        //if (BIO_printf(outbio,"\n%12s%s","",neg) <= 0)
        if (BIO_printf(outbio,"serial (openssl x509 -text   style): %s","",neg) <= 0)
            BIO_printf(outbio, "Error during printing the serial.\n");

        for (i=0; i<asn1_serial->length; i++)
        {
            char c[2];
            c[0] = asn1_serial->data[i];
            c[1] = '0';
            BIO_printf(outbio, c);
        }
    }

    X509_free(cert);
    BIO_free_all(certbio);
    BIO_free_all(outbio);
    exit(0);
}
#endif

#if 0
int main(int argc, char **argv)
{
    std::wstring sFolderW = NSFile::GetProcessDirectory();
    std::string sFolder = U_TO_UTF8(sFolderW);
    std::string __file = sFolder + "/settings.xml";
    std::string __key = sFolder + "/rsakey.pem";
    std::string __cert = sFolder + "/www.pfx";

    std::string __export = sFolder + "/www.pem";

    FILE *fp;
    EVP_PKEY *pkey;
    X509 *cert;
    STACK_OF(X509) *ca = NULL;
    PKCS12 *p12;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    if (!(fp = fopen(__cert.c_str(), "rb")))
    {
        fprintf(stderr, "Error opening file %s\n", __cert.c_str());
        exit(1);
    }
    p12 = d2i_PKCS12_fp(fp, NULL);
    fclose (fp);

    if (!p12)
    {
        fprintf(stderr, "Error reading PKCS#12 file\n");
        ERR_print_errors_fp(stderr);
        exit (1);
    }

    if (!PKCS12_parse(p12, "111", &pkey, &cert, &ca))
    {
        fprintf(stderr, "Error parsing PKCS#12 file\n");
        ERR_print_errors_fp(stderr);
        exit (1);
    }
    PKCS12_free(p12);

    if (!(fp = fopen(__export.c_str(), "w")))
    {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }
    if (pkey)
    {
        fprintf(fp, "***Private Key***\n");
        PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
    }
    if (cert)
    {
        fprintf(fp, "***User Certificate***\n");
        PEM_write_X509_AUX(fp, cert);
    }
    if (ca && sk_X509_num(ca))
    {
        fprintf(fp, "***Other Certificates***\n");
        int count = sk_X509_num(ca);
        for (int i = 0; i < count; i++)
            PEM_write_X509_AUX(fp, sk_X509_value(ca, i));
    }

    sk_X509_pop_free(ca, X509_free);
    X509_free(cert);
    EVP_PKEY_free(pkey);

    fclose(fp);
    return 0;
}
#endif

#if 0
int main()
{    
    unsigned char ibuf[] = "compute sha1";
    unsigned char obuf[20];

    SHA1(ibuf, strlen((char*)ibuf), obuf);

    int i;
    for (i = 0; i < 20; i++) {
        printf("%02x ", obuf[i]);
    }
    printf("\n");

    return 0;
}
#endif

#if 0
int main()
{
    std::wstring sFolderW = NSFile::GetProcessDirectory();
    std::string sFolder = U_TO_UTF8(sFolderW);
    std::string __file = sFolder + "/settings.xml";

#if 1
    std::string __key = sFolder + "/www.pem";
    std::string __cert = sFolder + "/www.pem";
#else
    std::string __key = sFolder + "/rsakey.pem";
    std::string __cert = sFolder + "/rsacert.pem";
#endif

    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    FILE* file_key = NSFile::CFileBinary::OpenFileNative(UTF8_TO_U(__key), L"r+");
    FILE* file_cert = NSFile::CFileBinary::OpenFileNative(UTF8_TO_U(__key), L"r+");

    EVP_PKEY* pKey = NULL;
    PEM_read_PrivateKey(file_key, &pKey, NULL, NULL);

    X509* pCert = NULL;
    PEM_read_X509(file_cert, &pCert, NULL, NULL);

    EVP_MD_CTX* pCtx = EVP_MD_CTX_create();
    const EVP_MD* pDigest = EVP_sha1();

    int n1 = EVP_SignInit(pCtx, pDigest);

    BYTE* pDataSrc = NULL;
    DWORD nDataSrcLen = 0;
    NSFile::CFileBinary::ReadAllBytes(UTF8_TO_U(__file), &pDataSrc, nDataSrcLen);

    int n2 = EVP_SignUpdate(pCtx, pDataSrc, (size_t)nDataSrcLen);

    BYTE pSignature[4096];
    unsigned int nSignatureLen = 0;

    int n3 = EVP_SignFinal(pCtx, pSignature, &nSignatureLen, pKey);

    RELEASEARRAYOBJECTS(pDataSrc);

    EVP_PKEY_free(pKey);
    X509_free(pCert);

    EVP_MD_CTX_destroy(pCtx);

    fclose(file_key);
    fclose(file_cert);

    return 0;
}
#endif

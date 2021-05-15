#include "../../../common/File.h"
#include "../include/XmlCertificate.h"
#include "../include/OOXMLSigner.h"
#include "../include/OOXMLVerifier.h"

#ifdef _WIN32
//#define USE_MS_CRYPTO
#endif

int main()
{
#ifdef USE_MS_CRYPTO

    // TODO: Load sertificate from store

#else

    std::wstring sTestDir = NSFile::GetProcessDirectory() + L"/../../keys/";
    ICertificate* pCertificate = ICertificate::CreateInstance();
    pCertificate->FromFiles(sTestDir + L"key.key", "", sTestDir + L"cert.crt", "");



    delete pCertificate;

#endif
    return 0;
}

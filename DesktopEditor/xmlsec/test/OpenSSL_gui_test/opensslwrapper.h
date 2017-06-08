#ifndef OPENSSL_WRAPPER
#define OPENSSL_WRAPPER

#define OPEN_SSL_WARNING_OK         0
#define OPEN_SSL_WARNING_ERR        1
#define OPEN_SSL_WARNING_ALL_OK     2
#define OPEN_SSL_WARNING_PASS       4

class COpenssl_library_private;
class COpenssl_library
{
private:
    COpenssl_library_private* m_internal;

public:
    COpenssl_library();
    ~COpenssl_library();

    int LoadKey(std::wstring file, std::string password);
    int LoadCert(std::wstring file, std::string password);
};

#endif // OPENSSL_WRAPPER

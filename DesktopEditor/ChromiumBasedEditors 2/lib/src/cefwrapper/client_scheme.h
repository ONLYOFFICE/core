#ifndef CEF_ASC_CLIENT_SCHEME_H_
#define CEF_ASC_CLIENT_SCHEME_H_

#include <vector>
#include "include/cef_base.h"

class CefBrowser;
class CefSchemeRegistrar;

namespace asc_scheme
{
    // Register the scheme.
    void RegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar,
                           std::vector<CefString>& cookiable_schemes);

    // Create the scheme handler.
    bool InitScheme();
}

#endif

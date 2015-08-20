#ifndef CEF_ASC_CLIENT_RENDERER_H_
#define CEF_ASC_CLIENT_RENDERER_H_

#include "include/cef_base.h"
#include "cefclient/renderer/client_app_renderer.h"

namespace asc_client_renderer
{
    // Create the render delegate.
    void CreateRenderDelegates(client::ClientAppRenderer::DelegateSet& delegates);
}

#endif  // CEF_ASC_CLIENT_RENDERER_H_

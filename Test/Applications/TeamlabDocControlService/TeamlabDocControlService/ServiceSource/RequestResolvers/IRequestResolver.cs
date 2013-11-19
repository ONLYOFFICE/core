using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    interface IRequestResolver
    {
        string ProcessRequest(string request);
    }
}

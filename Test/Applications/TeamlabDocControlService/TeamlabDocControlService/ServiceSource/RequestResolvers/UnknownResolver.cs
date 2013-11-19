
using System;
using System.Collections.Generic;
using System.Text;
using TeamlabDocControlService.ServiceSource.Monitors;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    class UnknownResolver : IRequestResolver
    {
        TaskManager mManager;
        public UnknownResolver (TaskManager manager)
        {
            mManager = manager;
        }
        public string ProcessRequest(string request)
        {
            // there are no unknown tasks
            return "not implemented";
        }
    }
}

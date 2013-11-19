using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TeamlabDocControlService.ServiceSource
{
    public class ServiceSharedObject
    {
        private TeamlabDocService mService;

        public ServiceSharedObject(TeamlabDocService service)
        { 
            mService = service;
        }

        public void OnStart()
        { 
        }

        public void OnStop()
        { 
        }
    }
}

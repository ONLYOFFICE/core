using System;
using System.Collections.Generic;
using System.Text;
using TeamlabDocControlService.ServiceSource.Monitors;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    class MemoryMonitorResolver: IRequestResolver
    {
        public static string RESOLVER_NAME = "memory-monitor";

        TaskManager mManager;
        public MemoryMonitorResolver(TaskManager manager)
        {
            mManager = manager;
        }
        public string ProcessRequest (string request)
        {

            IServiceTask task = mManager.getTaskByName(RESOLVER_NAME);

            if (null == task)
            {
                task = mManager.AddTask(RESOLVER_NAME, ServiceHelpers.ExtractRequestParams(request));
            }

            string response = "<task name='" + RESOLVER_NAME + "'>" + task.GetResult() + "</task>";
            //mManager.RemoveTask ();
            return response;
        }
    }
}

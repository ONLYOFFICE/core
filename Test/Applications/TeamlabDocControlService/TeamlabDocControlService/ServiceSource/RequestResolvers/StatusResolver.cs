using System;
using System.Collections.Generic;
using System.Text;
using TeamlabDocControlService.ServiceSource.Monitors;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    class StatusResolver : IRequestResolver
    {
        public static string RESOLVER_NAME = "status";

        TaskManager mManager;
        public StatusResolver(TaskManager manager)
        {
            mManager = manager;
        }
        public string ProcessRequest(string request)
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

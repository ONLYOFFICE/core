using System;
using System.Collections.Generic;
using System.Text;
using TeamlabDocControlService.ServiceSource.Monitors;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    class ProcessorMonitorResolver: IRequestResolver
    {
        public static string RESOLVER_NAME = "processor-monitor";

        TaskManager mManager;
        public ProcessorMonitorResolver (TaskManager manager)
        {
            mManager = manager;
        }
        public string ProcessRequest (string request)
        {

            IServiceTask task = mManager.getTaskByName(RESOLVER_NAME);

            // check params
            string param = ServiceHelpers.ExtractRequestParams(request);


            if (null == task)
            {
                task =  mManager.AddTask(RESOLVER_NAME, param);
            }

            string response = "<task name='" + RESOLVER_NAME + "'>" + task.GetResult() + "</task>";
            //mManager.RemoveTask ();
            return response;
        }
    }
}

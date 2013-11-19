using System;
using System.Collections.Generic;
using System.Text;
using TeamlabDocControlService.ServiceSource.Monitors;

namespace TeamlabDocControlService.ServiceSource.RequestResolvers
{
    class RecycleAppPoolResolver : IRequestResolver
    {
        public static string RESOLVER_NAME = "recycleapppool";
        
        TaskManager mManager;
        public RecycleAppPoolResolver (TaskManager manager)
        {
            mManager = manager;
        }
        public string ProcessRequest (string request)
        {

            IServiceTask task = mManager.getTaskByName(RESOLVER_NAME);

            // check params
            string param = ServiceHelpers.ExtractRequestParams(request);
            
            // check action (start/stop/pause)
            string action = ServiceHelpers.ExtractRequestParams(request);

            // check 
            if (null == task)
            {
                task = mManager.AddTask(RESOLVER_NAME, param);
            }
            else
            {
                task.SetParams(param);
            }

            string response = "<task name='" + RESOLVER_NAME + "'>" + task.GetResult() + "</task>";
            //mManager.RemoveTask ();
            return response;
        }

    }
}

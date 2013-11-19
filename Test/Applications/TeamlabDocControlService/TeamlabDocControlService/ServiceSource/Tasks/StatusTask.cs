using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class StatusTask: ServiceTask
    {
        public StatusTask(Logger logger)
            : base (logger)
        {
        }
        public override TaskType GetTaskType()
        {
            return TaskType.Sync;   
        }
        protected override void DoWork()
        {
            SetResult("alive");
        }
    }
}

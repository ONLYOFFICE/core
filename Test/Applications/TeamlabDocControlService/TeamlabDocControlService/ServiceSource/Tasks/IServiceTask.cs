using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public enum Status { Stopped, Started, Paused, Error };
    public enum TaskType { Sync, Async };
    interface IServiceTask
    {
        void Start();
        void Stop(bool waitForEnd);
        void Pause();
        Status GetStatus();
        TaskType GetTaskType();
        string GetResult();
        void SetParams(String parameters);
    }
}

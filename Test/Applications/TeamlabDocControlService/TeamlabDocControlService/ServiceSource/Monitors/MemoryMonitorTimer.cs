using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public delegate void TeamlabMemoryTimerEventHandler (object sender, float value, DateTime time);

    class MemoryMonitorTimer: MonitorByTimer
    {
        private PerformanceCounter mCounter; 

        public event TeamlabMemoryTimerEventHandler OnMeasure;

        public MemoryMonitorTimer(double interval, Logger logger)
            : base (interval, logger)
        {
            mCounter = new PerformanceCounter("Memory", "Available MBytes");
        }
        protected override void OnTimer(object state)
        {
            float value = mCounter.NextValue();
            OnMeasure(this, value, DateTime.Now);
        }
    }
}

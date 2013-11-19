using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public delegate void TeamlabPerformanceTimerEventHandler (object sender, float value, DateTime time);

    class PerformanceMonitorTimer: MonitorByTimer
    {
        private PerformanceCounter mCpuCounter; 

        public event TeamlabPerformanceTimerEventHandler OnMeasure;

        public PerformanceMonitorTimer (double interval, Logger logger)
            : base (interval, logger)
        {
            mCpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
        }
        protected override void OnTimer(object state)
        {
            float value = mCpuCounter.NextValue();
            OnMeasure(this, value, DateTime.Now);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    abstract class MonitorByTimer 
    {
        private Timer mTimer;
        private double mInterval;
        protected Logger mLogger;

        public double Interval
        {
            get { return mInterval; }
            set { mInterval = value; }
        }

        public bool Start()
        {
            bool result = true;
            try
            {
                mTimer = new Timer(OnTimer);
                mTimer.Change(100, (long) mInterval);
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.Message);
                result = false;
            }
            return result;
        }
        public void Stop()
        {
            mTimer.Dispose();
            mTimer = null;
        }
        public MonitorByTimer (double interval, Logger logger)
        { 
            mInterval = interval;
            mLogger = logger;
        }
        protected abstract void OnTimer(object state);
    }
}

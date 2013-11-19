using System;
using System.Collections.Generic;
using System.Text;
using System.Timers;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public delegate void TeamlabFolderSizeTimerEventHandler(object sender, double size, DateTime time);

    class FolderSizeMonitorTimer: MonitorByTimer
    {
        private string mFolder;

        public event TeamlabFolderSizeTimerEventHandler OnMeasure;
      
        public FolderSizeMonitorTimer(string folder, double interval, Logger logger)
            : base (interval, logger)
        { 
            mFolder = folder;
        }
        protected override void OnTimer(object state)
        {
            double size = ServiceHelpers.CalculateFolderSize (mFolder, mLogger);
            OnMeasure (this, size, DateTime.Now);
        }
    }
}

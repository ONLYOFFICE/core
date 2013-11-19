using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Permissions;
using System.IO;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public delegate void TeamlabSizeExceedEventHandler (object sender, double size);

    class FolderMonitor
    {
        private string mPath;
        private double mLimit = 10.0*1024.0*1024.0;    // 10 M
        private FileSystemWatcher mWatcher = null;
        private Object mSync = new Object();
        private Logger mLogger = null;

        public event TeamlabSizeExceedEventHandler SizeExceeded;

        public FolderMonitor(string path, Logger log)
        {
            mLogger = log;
            mPath = path;
            mWatcher = new FileSystemWatcher();
            mWatcher.NotifyFilter = NotifyFilters.Size;
            mWatcher.Path = mPath;
            mWatcher.Changed += new FileSystemEventHandler(OnChange);
        }
        public void Start()
        {
            mWatcher.EnableRaisingEvents = true;
        }
        public void Stop()
        {
            mWatcher.EnableRaisingEvents = false;
        }
        public void SetSizeLimit(float limit)
        {
            lock (mSync)
            {
                mLimit = limit;
            }
        }
        protected void OnChange(object source, FileSystemEventArgs e)
        {
            double size = ServiceHelpers.CalculateFolderSize(mPath, mLogger);
            
            // log it
            if (size > mLimit)
            {
                // fire event
                SizeExceeded(this, size);
            }
        }
    }
}

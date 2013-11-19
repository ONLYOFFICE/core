using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TeamlabDocControlService.ServiceSource.Tasks
{
    public enum TTeamlabServiceTaskResult { STR_IDLE, STR_RUNNED, STR_FINISHED_OK, STR_FINISHED_FAILED, STR_CANCELLED, STR_UNKNOWN_ERROR };
    abstract class ThreadTask
    {
        protected Thread mThread = new Thread(ThreadRoutine);
        protected TTeamlabServiceTaskResult mResult = TTeamlabServiceTaskResult.STR_IDLE;
        protected Object mSync = new Object();
        protected Logger mLogger = null;

        public ThreadTask (Logger log)
        {
            mLogger = log;
        }
        public TTeamlabServiceTaskResult Result
        {
            get {lock (mSync) {return mResult;}}
            set {lock (mSync) { mResult = value;}}
        }
        public TTeamlabServiceTaskResult Start()
        {
            TTeamlabServiceTaskResult result = Stop();

            if ((TTeamlabServiceTaskResult.STR_FINISHED_OK != result)
                && (TTeamlabServiceTaskResult.STR_IDLE != result))
            {
                return result;
            }
            
            try
            {
                mThread.Start(this);
            }
            catch (Exception ex)
            {
                result = TTeamlabServiceTaskResult.STR_UNKNOWN_ERROR;
                if (null != mLogger)
                    mLogger.LogError(this.ToString() + ": " + ex.Message);
            }
            result = (mThread.ThreadState == ThreadState.Running) ? TTeamlabServiceTaskResult.STR_RUNNED : TTeamlabServiceTaskResult.STR_UNKNOWN_ERROR;
            Result = result;
            return result;
        }
        public TTeamlabServiceTaskResult Stop()
        {
            TTeamlabServiceTaskResult result = Result;
            if (ThreadState.Running == mThread.ThreadState)
            {
                mThread.Abort();
                Result = result = TTeamlabServiceTaskResult.STR_CANCELLED;
            }
            return result;
        }
        private static void ThreadRoutine(object obj)
        {
            (obj as ThreadTask).DoWork();
        }
        public abstract void DoWork();
    }
}

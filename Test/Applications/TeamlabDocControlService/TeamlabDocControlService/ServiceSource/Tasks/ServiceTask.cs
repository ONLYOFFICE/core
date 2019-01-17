/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    abstract class ServiceTask : IServiceTask
    {
        protected Object mSync = new Object();
        protected volatile float mProgress;
        protected volatile bool mNeedToStop;
        protected volatile bool mNeedPause;
        protected volatile Status mStatus;
        protected Thread mThread = new Thread (ThreadRoutine);
        protected Logger mLogger = null;
        protected string mResultXml;
        protected string mStartParams;

        public ServiceTask(Logger logger)
        {
            mProgress = 0.0f;
            mNeedToStop = false;
            mNeedPause = false;
            mStatus = Status.Stopped;
            mLogger = logger;
            SetResult ("task_created");
        }
        public void Start()
        {
            // task already started
            if (mThread.IsAlive)
            {
                if (Status.Paused == mStatus)
                {
                    // thread is paused. Resume.
                    mNeedPause = false;
                }
                return;
            }

            mNeedToStop = false;
            mNeedPause = false;
            mProgress = 0.0f;

            try
            {
                mThread.Start(this);
                SetResult("task_started_ok");
            }
            catch (Exception ex)
            {
                SetResult("task_started_fail");
                mStatus = Status.Error;
                if (null != mLogger)
                    mLogger.LogError(this.ToString() + ": " + ex.Message);
            }
        }
        public void Stop (bool waitForEnd)
        {
            // task already stopped
            if (!mThread.IsAlive)
            {
                SetResult("task_stopped");
                return;
            }

            mNeedPause = false;
            mNeedToStop = true;

            if (waitForEnd)
                mThread.Join();

            SetResult("task_stopped");
        }
        public void Pause()
        {
            mNeedPause = true;
        }
        virtual public string GetResult()
        {
            lock (mSync)
            {
                return mResultXml;
            }
        }
        protected void SetResult(string result)
        {
            lock (mSync)
            {
                mResultXml = result;
            }
        }
        public Status GetStatus ()
        {
            return mStatus;
        }
        public virtual TaskType GetTaskType()
        {
            return TaskType.Sync;
        }

        public void ForceTerminate ()
        {
            // task already stopped
            if (!mThread.IsAlive)
                return;

            try
            {
                mThread.Abort();
            }
            catch (Exception ex)
            {
                if (null != mLogger)
                    mLogger.LogError(this.ToString() + ": " + ex.Message);
            }
        }

        public virtual void SetParams (String parameters)
        {
            mStartParams = parameters;
        }

        private static void ThreadRoutine(object obj)
        {
            ServiceTask thisTask = (obj as ServiceTask);
            try
            {
                thisTask.DoWork();
            }
            catch (ThreadAbortException ex)
            {
                // manual aborting of hanged thread
                if (null != thisTask.mLogger)
                    thisTask.mLogger.LogError(thisTask.ToString() + ": " + ex.Message);
            }
            thisTask.mStatus = Status.Stopped;
        }
        protected bool NeedToPause()
        {
            return mNeedPause;
        }
        protected bool NeedToStop()
        {
            return mNeedToStop;
        }

        protected abstract void DoWork();
    }
}

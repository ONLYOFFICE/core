/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

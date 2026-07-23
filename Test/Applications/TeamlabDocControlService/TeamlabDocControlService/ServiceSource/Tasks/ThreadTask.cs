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

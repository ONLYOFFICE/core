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

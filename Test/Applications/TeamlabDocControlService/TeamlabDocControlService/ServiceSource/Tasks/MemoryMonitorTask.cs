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
using System.Diagnostics;
using TeamlabDocControlService.ServiceSource.ValueStorage;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class MemoryMonitorTask: ServiceTask
    {
        private PerformanceCounter mCounter;
        private FloatDataStorage mStorage;

        public MemoryMonitorTask(Logger logger)
            : base (logger)
        {
            mCounter = new PerformanceCounter("Memory", "Available MBytes");
            mStorage = new FloatDataStorage();
        }
        public override TaskType GetTaskType()
        {
            return TaskType.Sync;   
        }
        
        protected override void DoWork()
        {
            float nMidValue = -1.0F;
            while (true)
            {
                while (NeedToPause())
                {
                    Thread.Sleep(1000);
                }
                if (NeedToStop())
                    break;

                // get memory usage
                float value = mCounter.NextValue();
                if (nMidValue < 0)
                {
                    nMidValue = value;
                    // 
                    IDataItem<float> item = new DataItemBase<float>();
                    item.set(value);
                    mStorage.Add(item);
                }
                else
                {
                    // check 10% treshold
                    float delta = Math.Abs(value - nMidValue);
                    float proc = (nMidValue > 0) ? delta/nMidValue : 1.0F;   // procent of var
                    
                    if (proc > 0.1F && delta > 50.0F) // 10% relative and 50�� absolute value of memory usage delta
                    {
                        nMidValue = 0.5F * (nMidValue + value);  // tune mid value;
                        
                        // store to base
                        IDataItem<float> item = new DataItemBase<float>();
                        item.set(value);
                        mStorage.Add(item);
                    }
                }

                Thread.Sleep(1000);
            }
        }

        override public string GetResult()
        {
            string res = mStorage.ToXML();

            lock (mSync)
            {
                return res;
            }
        }
    }
}

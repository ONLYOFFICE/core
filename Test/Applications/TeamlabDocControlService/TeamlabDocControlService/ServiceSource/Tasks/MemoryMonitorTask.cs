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

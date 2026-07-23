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
using TeamlabDocControlService.ServiceSource.RequestResolvers;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class TaskManager
    {
        // task map
        Dictionary <String, IServiceTask> mEntities = new Dictionary<string,IServiceTask> ();
        Dictionary<String, IRequestResolver> mResolvers = new Dictionary<string,IRequestResolver> ();

        Logger mLogger = null;

        // returns request resolver by task name
        public IRequestResolver GetRequestResolverByName(string taskType)
        {
            IRequestResolver resolver = null;
            if (taskType == StatusResolver.RESOLVER_NAME)
            {
                resolver = new StatusResolver(this);
            }
            else if (taskType == MemoryMonitorResolver.RESOLVER_NAME)
            {
                resolver = new MemoryMonitorResolver(this);
            }
            else if (taskType == ProcessorMonitorResolver.RESOLVER_NAME)
            {
                resolver = new ProcessorMonitorResolver(this);
            }
            else if (taskType == AppPoolMonitorResolver.RESOLVER_NAME)
            {
                resolver = new AppPoolMonitorResolver(this);
            }
            else if (taskType == ClearFolderResolver.RESOLVER_NAME)
            {
                resolver = new ClearFolderResolver(this);
            }
            else if (taskType == RecycleAppPoolResolver.RESOLVER_NAME)
            {
                resolver = new RecycleAppPoolResolver (this);
            }

            if (null == resolver)
            { 
                // if there are no task type, return default resolver
                resolver = new UnknownResolver(this);
            }
            return resolver;
        }
        public IServiceTask getTaskByName(string name)
        {
            IServiceTask val;
            if (mEntities.TryGetValue(name, out val))
                return val;
            else
                return null;
        }
        public IServiceTask AddTask(string name, string parameters)
        {
            IServiceTask task = getTaskByName(name);

            if (null != task)
            { 
                // task already launched
                return task;
            }

            if (name == StatusResolver.RESOLVER_NAME)
            {
                task = new StatusTask (mLogger);
            }
            else if (name == MemoryMonitorResolver.RESOLVER_NAME)
            {
                task = new MemoryMonitorTask (mLogger);
            }
            else if (name == ProcessorMonitorResolver.RESOLVER_NAME)
            {
                task = new ProcessorMonitorTask (mLogger);
            }
            else if (name == AppPoolMonitorResolver.RESOLVER_NAME)
            {
                task = new AppPoolMonitorTask(mLogger);
            }
            else if (name == ClearFolderResolver.RESOLVER_NAME)
            {
                task = new ClearFolderTask(mLogger);
            }
            else if (name == RecycleAppPoolResolver.RESOLVER_NAME)
            {
                task = new RecycleAppPoolTask(mLogger);
            }

            mEntities.Add(name, task);
            task.SetParams (parameters);
            task.Start();

            return task;
        }
        public bool RemoveTask(string name)
        {
            IServiceTask task = getTaskByName(name);

            if (null == task)
            {
                // task already removed or has not been launched
                return false;
            }
            task.Stop (false);
            mEntities.Remove(name);
            return true;
        }
        public bool RemoveAllTasks()
        {
            bool res = true;
            foreach (KeyValuePair<string, IServiceTask> taskPair in mEntities)
            {                
                IServiceTask task = taskPair.Value;
                if (null == task)
                {
                    res = false;
                    continue;
                }

                task.Stop(false);
            }
            mEntities.Clear();
            return res;
        }


    }
}

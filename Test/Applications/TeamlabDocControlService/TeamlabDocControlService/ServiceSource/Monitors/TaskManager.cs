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

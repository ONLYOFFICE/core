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
                    
                    if (proc > 0.1F && delta > 50.0F) // 10% relative and 50ла absolute value of memory usage delta
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

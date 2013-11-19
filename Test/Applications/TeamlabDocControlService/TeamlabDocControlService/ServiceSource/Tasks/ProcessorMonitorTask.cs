using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;
using TeamlabDocControlService.ServiceSource.ValueStorage;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class ProcessorMonitorTask: ServiceTask
    {
        private PerformanceCounter mCounter;
        private FloatDataStorage mStorage;

        public ProcessorMonitorTask(Logger logger)
            : base (logger)
        {
            mCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
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

                // processor usage
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
                    float proc = (nMidValue > 0) ? delta / nMidValue : 1.0F;   // procent of difference
                    if (proc > 0.1F && delta > 5.0F) // 10% relative and 5% absolute value of processor usage delta
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
            return mStorage.ToXML();
        }
    }
}

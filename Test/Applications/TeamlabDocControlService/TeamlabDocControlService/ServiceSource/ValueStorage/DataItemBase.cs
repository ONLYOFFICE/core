using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.ValueStorage
{
    class DataItemBase<T>: IDataItem<T>
    {
        protected DateTime mDate;
        protected T mValue;

        public void set (T data)
        {
            mDate = DateTime.Now;
            mValue = data;
        }
        public T get()
        {
            return mValue;
        }
        public DateTime getDate()
        {
            return mDate;
        }
    }
}

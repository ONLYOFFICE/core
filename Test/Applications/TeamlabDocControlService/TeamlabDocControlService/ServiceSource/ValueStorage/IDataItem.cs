using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.ValueStorage
{
    public interface IDataItem<T>
    {
        void set (T data);
        T get();
        DateTime getDate();
    }
}

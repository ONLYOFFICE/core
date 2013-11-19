using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.ValueStorage
{
    // stores data (last N items)
    
    abstract public class DataStorage<T>
    {
        protected List<IDataItem<T>> mItems;
        protected int mMaxSize;
        protected const int MAX_SIZE_DEFAULT = 100 * 1024; // 100K array items by default

        public DataStorage ()
            : this (MAX_SIZE_DEFAULT)
        {
        }
        public DataStorage (int size)
        {
            mMaxSize = size;
            mItems = new List<IDataItem<T>>();
        }
        public void Add (IDataItem<T> item)
        {
            lock (this)
            {
                if (mItems.Count > MAX_SIZE_DEFAULT)
                    mItems.RemoveAt(0);

                mItems.Add(item);
            }
        }
        public void Clear()
        {
            lock (this)
            {
                mItems.Clear();
            }
        }
        public T GetItem(int index)
        {
            lock (this)
            {
                return mItems[index].get();
            }
        }
        public List<IDataItem<T>> GetItems()
        {
            lock (this)
            {
                return new List<IDataItem<T>> (mItems);
            }
        }
        abstract public string ToXML();
    }
}

#include "cfstorage.h"

using namespace CFCPP;

CFStorage::CFStorage()
{
}

RedBlackTree::RBTree CFStorage::getChildren() const
{
    if (children == nullptr)
    {
        //if (this.CompoundFile.HasSourceStream)
        //{
        children = LoadChildren(this.DirEntry.SID);
        //}
        //else
        if (children == null)
        {
            children = this.CompoundFile.CreateNewTree();
        }
    }
    return children;
}

RedBlackTree::RBTree CFStorage::LoadChildren(int SID)
{

}

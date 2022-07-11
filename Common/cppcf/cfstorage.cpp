#include "cfstorage.h"
#include "cfexception.h"
#include "RBTree/rbtreeexception.h"

using namespace CFCPP;
using RedBlackTree::RBTree;


CFStorage::CFStorage(std::shared_ptr<CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry) :
    CFItem(compFile)
{
    if (dirEntry == nullptr || dirEntry->getSid() < 0)
        throw new CFException("Attempting to create a CFStorage using an unitialized directory");

    this->dirEntry = dirEntry;
}

std::shared_ptr<RBTree> CFStorage::getChildren()
{
    if (children == nullptr)
    {
        //if (this.CompoundFile.HasSourceStream)
        //{
        children = LoadChildren(this->dirEntry->getSid());
        //}
        //else
        if (children == nullptr)
        {
            children = CompoundFile::CreateNewTree();
        }
    }
    return children;
}

std::shared_ptr<CFStream> CFStorage::AddStream(std::wstring streamName)
{
    CheckDisposed();

    if (streamName.empty())
        throw CFException("Stream name cannot be null or empty");



    std::shared_ptr<IDirectoryEntry> dirEntry = DirectoryEntry::TryNew(streamName, StgType::StgStream, compoundFile->GetDirectories().cast<IDirectoryEntry>());

    // Add new Stream directory entry
    //cfo = new CFStream(this.CompoundFile, streamName);

    try
    {
        // Add object to Siblings tree
        children->Insert(dirEntry);

        //... and set the root of the tree as new child of the current item directory entry
        dirEntry->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(children->getRoot())->getSid());
    }
    catch (RedBlackTree::RBTreeException &rbex)
    {
        compoundFile->ResetDirectoryEntry(dirEntry->getSid());

        throw new CFDuplicatedItemException(L"An entry with name '" + streamName + L"' is already present in storage '" + Name() + L"' ");
    }

    return std::shared_ptr<CFStream> (new CFStream(compoundFile, dirEntry));
}

std::shared_ptr<CFStream> CFStorage::GetStream(std::wstring streamName)
{
    CheckDisposed();

    std::shared_ptr<IDirectoryEntry> tmp = DirectoryEntry::Mock(streamName, StgType::StgStream);

    //if (children == null)
    //{
    //    children = compoundFile.GetChildrenTree(SID);
    //}

    IRBNode outDe = null;

    if (Children.TryLookup(tmp, out outDe) && (((IDirectoryEntry)outDe).StgType == StgType.StgStream))
    {
        return new CFStream(this.CompoundFile, (IDirectoryEntry)outDe);
    }
    else
    {
        throw new CFItemNotFound("Cannot find item [" + streamName + "] within the current storage");
    }
}

std::shared_ptr<RBTree> CFStorage::LoadChildren(int SID)
{
    std::shared_ptr<RBTree> childrenTree = compoundFile->GetChildrenTree(SID);

    if (childrenTree->getRoot() != nullptr)
        dirEntry->setChild((std::static_pointer_cast<IDirectoryEntry>(childrenTree->getRoot())->getSid()));
    else
        dirEntry->setChild(DirectoryEntry::NOSTREAM);

    return childrenTree;
}

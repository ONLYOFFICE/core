#include "cfstorage.h"
#include "cfexception.h"
#include "compoundfile.h"
#include "directoryentry.h"
#include "RBTree/rbtreeexception.h"
#include "RBTree/irbnode.h"

using namespace CFCPP;
using RedBlackTree::RBTree;


CFStorage::CFStorage(CompoundFile *compFile, const std::weak_ptr<IDirectoryEntry> &dirEntry) :
    CFItem(compFile)
{
    setDirEntry(dirEntry);
}

std::shared_ptr<RBTree> CFStorage::getChildren()
{
    if (children == nullptr)
    {
        children = LoadChildren(this->dirEntry.lock()->getSid());

        if (children == nullptr)
        {
            children = CompoundFile::CreateNewTree();
        }
    }
    return children;
}

std::shared_ptr<CFStream> CFStorage::AddStream(const std::wstring& streamName)
{
    CheckDisposed();

    if (streamName.empty())
        throw CFException("Stream name cannot be null or empty");



    std::shared_ptr<IDirectoryEntry> dirEntry = DirectoryEntry::TryNew(streamName, StgType::StgStream, compoundFile->GetDirectories());

    try
    {
        // Add object to Siblings tree
        getChildren()->Insert(dirEntry);

        //... and set the root of the tree as new child of the current item directory entry
        dirEntry->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
    }
    catch (RedBlackTree::RBTreeException &rbex)
    {
        compoundFile->ResetDirectoryEntry(dirEntry->getSid());

        throw CFDuplicatedItemException(L"An entry with name '" + streamName + L"' is already present in storage '" + Name() + L"' ");
    }

    return std::shared_ptr<CFStream>(new CFStream(compoundFile, dirEntry));
}

std::shared_ptr<CFStream> CFStorage::GetStream(const std::wstring& streamName)
{
    CheckDisposed();

    std::shared_ptr<IDirectoryEntry> tmp = DirectoryEntry::Mock(streamName, StgType::StgStream);

    RedBlackTree::PIRBNode outDe;

    if (getChildren()->TryLookup(tmp, outDe) &&
            ((std::static_pointer_cast<IDirectoryEntry>(outDe))->getStgType() == StgType::StgStream))
    {
        return std::shared_ptr<CFStream>(new CFStream(compoundFile, std::static_pointer_cast<IDirectoryEntry>(outDe)));
    }
    else
    {
        throw CFItemNotFound(L"Cannot find item [" + streamName + L"] within the current storage");
    }
}

bool CFStorage::TryGetStream(const std::wstring& streamName, std::shared_ptr<CFStream>& cfStream)
{
    bool result = false;
    cfStream.reset();

    try
    {
        CheckDisposed();

        std::shared_ptr<IDirectoryEntry> tmp = DirectoryEntry::Mock(streamName, StgType::StgStream);

        RedBlackTree::PIRBNode outDe;

        if (getChildren()->TryLookup(tmp, outDe) &&
                ((std::static_pointer_cast<IDirectoryEntry>(outDe))->getStgType() == StgType::StgStream))
        {
            cfStream = std::shared_ptr<CFStream>(new CFStream(compoundFile, std::static_pointer_cast<IDirectoryEntry>(outDe)));
            result = true;
        }
    }
    catch (CFDisposedException& ex)
    {
        result = false;
    }

    return result;
}

std::shared_ptr<CFStorage> CFStorage::GetStorage(const std::wstring &storageName)
{
    CheckDisposed();

    std::shared_ptr<IDirectoryEntry> templ = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
    RedBlackTree::PIRBNode outDe;

    if (getChildren()->TryLookup(templ, outDe) && std::static_pointer_cast<IDirectoryEntry>(outDe)->getStgType() == StgType::StgStorage)
    {
        return std::shared_ptr<CFStorage>(new CFStorage(compoundFile, std::dynamic_pointer_cast<IDirectoryEntry>(outDe)));
    }
    else
    {
        throw CFItemNotFound(L"Cannot find item [" + storageName + L"] within the current storage");
    }
}

std::shared_ptr<CFStorage> CFStorage::TryGetStorage(const std::wstring &storageName)
{
    CheckDisposed();

    std::shared_ptr<IDirectoryEntry> templ = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
    RedBlackTree::PIRBNode outDe;

    if (getChildren()->TryLookup(templ, outDe) && std::static_pointer_cast<IDirectoryEntry>(outDe)->getStgType() == StgType::StgStorage)
    {
        return std::shared_ptr<CFStorage>(new CFStorage(compoundFile, std::dynamic_pointer_cast<IDirectoryEntry>(outDe)));
    }
    else
    {
        return {};
    }
}

bool CFStorage::TryGetStorage(const std::wstring &storageName, std::shared_ptr<CFStorage>& cfStorage)
{
    bool result = false;
    cfStorage.reset();

    try
    {
        CheckDisposed();

        std::shared_ptr<IDirectoryEntry> templ = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
        RedBlackTree::PIRBNode outDe;

        if (getChildren()->TryLookup(templ, outDe) && std::static_pointer_cast<IDirectoryEntry>(outDe)->getStgType() == StgType::StgStorage)
        {
            cfStorage.reset(new CFStorage(compoundFile, std::dynamic_pointer_cast<IDirectoryEntry>(outDe)));
            result = true;
        }

    }
    catch (CFDisposedException& ex)
    {
        result = false;
    }

    return result;
}

std::shared_ptr<CFStorage> CFStorage::AddStorage(const std::wstring &storageName)
{
    CheckDisposed();

    if (storageName.empty())
        throw CFException("Stream name cannot be null or empty");

    // Add new Storage directory entry
    std::shared_ptr<IDirectoryEntry> cfo
            = DirectoryEntry::New(storageName, StgType::StgStorage, compoundFile->GetDirectories());

    //this.CompoundFile.InsertNewDirectoryEntry(cfo);

    try
    {
        // Add object to Siblings tree
        getChildren()->Insert(cfo);
    }
    catch (RedBlackTree::RBTreeDuplicatedItemException& ex)
    {
        compoundFile->ResetDirectoryEntry(cfo->getSid());
        cfo.reset();
        throw CFDuplicatedItemException(L"An entry with name '" + storageName + L"' is already present in storage '" + Name() + L"' ");
    }

    std::shared_ptr<IDirectoryEntry> childrenRoot = std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot());
    dirEntry.lock()->setChild(childrenRoot->getSid());

    return std::shared_ptr<CFStorage>(new CFStorage(compoundFile, cfo));
}

void CFStorage::VisitEntries(RedBlackTree::Action<std::shared_ptr<CFItem> > action, bool recursive)
{
    CheckDisposed();

    if (action != nullptr)
    {
        SVector<RedBlackTree::IRBNode> subStorages;

        RedBlackTree::Action<RedBlackTree::PIRBNode> internalAction =
                [&] (RedBlackTree::PIRBNode targetNode)
        {
            auto d = std::dynamic_pointer_cast<IDirectoryEntry>(targetNode);
            if (d->getStgType() == StgType::StgStream)
            {
                std::shared_ptr<CFStream> pstream (new CFStream(compoundFile, d));
                action(std::static_pointer_cast<CFItem>(pstream));
            }
            else
            {
                std::shared_ptr<CFStorage> pstorage(new CFStorage(compoundFile, d));
                action(std::static_pointer_cast<CFItem>(pstorage));
            }

            if (d->getChild() != DirectoryEntry::NOSTREAM)
                subStorages.push_back(targetNode);
        };

        getChildren()->VisitTreeNodes(internalAction);

        if (recursive && subStorages.size() > 0)
            for (const auto& n : *subStorages)
            {
                auto d = std::dynamic_pointer_cast<IDirectoryEntry>(n);
                CFStorage(compoundFile, d).VisitEntries(action, recursive);
            }
    }
}

void CFStorage::Delete(const std::wstring &entryName)
{
    CheckDisposed();

    // Find entry to delete
    auto tmp = DirectoryEntry::Mock(entryName, StgType::StgInvalid);

    RedBlackTree::PIRBNode foundObj;

    getChildren()->TryLookup(tmp, foundObj);

    if (foundObj == nullptr)
        throw CFItemNotFound(L"Entry named [" + entryName + L"] was not found");

    if (std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getStgType() == StgType::StgRoot)
        throw CFException("Root storage cannot be removed");


    RedBlackTree::PIRBNode altDel;
    switch (std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getStgType())
    {
    case StgType::StgStorage:
    {
        std::shared_ptr<CFStorage> temp(new CFStorage(compoundFile, std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)));

        // This is a storage. we have to remove children items first
        for (const auto& de : *(temp->getChildren()))
        {
            auto ded = std::dynamic_pointer_cast<IDirectoryEntry>(de);
            temp->Delete(ded->GetEntryName());
        }

        // ...then we need to rethread the root of siblings tree...
        if (getChildren()->getRoot() != nullptr)
            dirEntry.lock()->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
        else
            dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);

        // ...and finally Remove storage item from children tree...
        getChildren()->Delete(foundObj, altDel);

        // ...and remove directory (storage) entry

        if (altDel != nullptr)
        {
            foundObj = altDel;
        }

        compoundFile->InvalidateDirectoryEntry(std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getSid());

        break;
    }

    case StgType::StgStream:
    {
        // Free directory associated data stream.
        compoundFile->FreeAssociatedData(std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getSid());

        // Remove item from children tree
        getChildren()->Delete(foundObj, altDel);

        // Rethread the root of siblings tree...
        if (getChildren()->getRoot() != nullptr)
            dirEntry.lock()->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
        else
            dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);

        // Delete operation could possibly have cloned a directory, changing its SID.
        // Invalidate the ACTUALLY deleted directory.
        if (altDel != nullptr)
        {
            foundObj = altDel;
        }

        compoundFile->InvalidateDirectoryEntry(std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getSid());
    }
    default:
        break;
    }

}

void CFStorage::RenameItem(const std::wstring &oldItemName, const std::wstring &newItemName)
{
    auto templ = DirectoryEntry::Mock(oldItemName, StgType::StgInvalid);
    RedBlackTree::PIRBNode item;
    if (getChildren()->TryLookup(templ, item))
    {
        std::dynamic_pointer_cast<DirectoryEntry>(item)->SetEntryName(newItemName);
    }
    else throw CFItemNotFound(L"Item " + oldItemName + L" not found in Storage");

    children.reset();
    children = LoadChildren(dirEntry.lock()->getSid()); //Rethread

    if (children == nullptr)
    {
        children = compoundFile->CreateNewTree();
    }
}

std::shared_ptr<RBTree> CFStorage::LoadChildren(int SID)
{
    std::shared_ptr<RBTree> childrenTree = compoundFile->GetChildrenTree(SID);

    if (childrenTree->getRoot() != nullptr)
        dirEntry.lock()->setChild((std::static_pointer_cast<IDirectoryEntry>(childrenTree->getRoot())->getSid()));
    else
        dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);

    return childrenTree;
}

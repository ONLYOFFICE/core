/*
* (c) Copyright Ascensio System SIA 2010-2019
*
* This program is a free software product. You can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License (AGPL)
* version 3 as published by the Free Software Foundation. In accordance with
* Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
* that Ascensio System SIA expressly excludes the warranty of non-infringement
* of any third-party rights.
*
* This program is distributed WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
* details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
*
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
* street, Riga, Latvia, EU, LV-1050.
*
* The  interactive user interfaces in modified source and object code versions
* of the Program must display Appropriate Legal Notices, as required under
* Section 5 of the GNU AGPL version 3.
*
* Pursuant to Section 7(b) of the License you must retain the original Product
* logo when distributing the program. Pursuant to Section 7(e) we decline to
* grant you any rights under trademark law for use of our trademarks.
*
* All the Product's GUI elements, including illustrations and icon sets, as
* well as technical writing content are licensed under the terms of the
* Creative Commons Attribution-ShareAlike 4.0 International. See the License
* terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
*
*/
#include "cfstorage.h"
#include "cfexception.h"
#include "compoundfile_impl.h"
#include "directoryentry.h"
#include "RBTree/rbtreeexception.h"
#include "RBTree/irbnode.h"
#include "RBTree/rbtree.h"

using namespace CFCPP;
using RedBlackTree::RBTree;


CFStorage::CFStorage(CompoundFile_impl *compFile, const std::weak_ptr<IDirectoryEntry> &dirEntry) :
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
            children = CompoundFile_impl::CreateNewTree();
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
        getChildren()->Insert(dirEntry);

        this->dirEntry.lock()->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
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

    std::shared_ptr<IDirectoryEntry> temp = DirectoryEntry::Mock(streamName, StgType::StgStream);

    RedBlackTree::PIRBNode outDirEntry;

    if (getChildren()->TryLookup(temp, outDirEntry) &&
            ((std::static_pointer_cast<IDirectoryEntry>(outDirEntry))->getStgType() == StgType::StgStream))
    {
        return std::shared_ptr<CFStream>(new CFStream(compoundFile, std::static_pointer_cast<IDirectoryEntry>(outDirEntry)));
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

        RedBlackTree::PIRBNode outDirEntry;

        if (getChildren()->TryLookup(tmp, outDirEntry) &&
                ((std::static_pointer_cast<IDirectoryEntry>(outDirEntry))->getStgType() == StgType::StgStream))
        {
            cfStream = std::shared_ptr<CFStream>(new CFStream(compoundFile, std::static_pointer_cast<IDirectoryEntry>(outDirEntry)));
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

    std::shared_ptr<IDirectoryEntry> pattern = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
    RedBlackTree::PIRBNode outDe;

    if (getChildren()->TryLookup(pattern, outDe) && std::static_pointer_cast<IDirectoryEntry>(outDe)->getStgType() == StgType::StgStorage)
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

    std::shared_ptr<IDirectoryEntry> pattern = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
    RedBlackTree::PIRBNode outDe;

    if (getChildren()->TryLookup(pattern, outDe) && std::static_pointer_cast<IDirectoryEntry>(outDe)->getStgType() == StgType::StgStorage)
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

        std::shared_ptr<IDirectoryEntry> pattern = DirectoryEntry::Mock(storageName, StgType::StgInvalid);
        RedBlackTree::PIRBNode outDirEntry;

        if (getChildren()->TryLookup(pattern, outDirEntry) && std::static_pointer_cast<IDirectoryEntry>(outDirEntry)->getStgType() == StgType::StgStorage)
        {
            cfStorage.reset(new CFStorage(compoundFile, std::dynamic_pointer_cast<IDirectoryEntry>(outDirEntry)));
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
        throw CFException("Stream name cannot be empty");

    std::shared_ptr<IDirectoryEntry> cfo
            = DirectoryEntry::New(storageName, StgType::StgStorage, compoundFile->GetDirectories());

    try
    {
        getChildren()->Insert(cfo);
    }
    catch (RedBlackTree::RBTreeDuplicatedItemException& ex)
    {
        compoundFile->ResetDirectoryEntry(cfo->getSid());
        cfo.reset();
        throw CFDuplicatedItemException(L"An entry with name '" + storageName + L"' is already had in storage '" + Name() + L"' ");
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
        {
            for (const auto& node : *subStorages)
            {
                auto dirEntry = std::dynamic_pointer_cast<IDirectoryEntry>(node);
                CFStorage(compoundFile, dirEntry).VisitEntries(action, recursive);
            }
        }
    }
}

void CFStorage::Delete(const std::wstring &entryName)
{
    CheckDisposed();

    auto temp = DirectoryEntry::Mock(entryName, StgType::StgInvalid);

    RedBlackTree::PIRBNode foundObj;

    getChildren()->TryLookup(temp, foundObj);

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

        auto storageChild = temp->getChildren();
        for (auto iter = storageChild->begin(); iter != storageChild->end(); ++iter)
        {
            auto de = *iter;
            auto ded = std::dynamic_pointer_cast<IDirectoryEntry>(de);
            temp->Delete(ded->GetEntryName());
        }

        if (getChildren()->getRoot() != nullptr)
        {
            dirEntry.lock()->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
        }
        else
        {
            dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);
        }

        getChildren()->Delete(foundObj, altDel);

        if (altDel != nullptr)
        {
            foundObj = altDel;
        }

        compoundFile->InvalidateDirectoryEntry(std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getSid());

        break;
    }

    case StgType::StgStream:
    {
        compoundFile->FreeAssociatedData(std::dynamic_pointer_cast<IDirectoryEntry>(foundObj)->getSid());

        getChildren()->Delete(foundObj, altDel);

        if (getChildren()->getRoot() != nullptr)
        {
            dirEntry.lock()->setChild(std::dynamic_pointer_cast<IDirectoryEntry>(getChildren()->getRoot())->getSid());
        }
        else
        {
            dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);
        }

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
    auto pattern = DirectoryEntry::Mock(oldItemName, StgType::StgInvalid);
    RedBlackTree::PIRBNode item;
    if (getChildren()->TryLookup(pattern, item))
    {
        std::dynamic_pointer_cast<DirectoryEntry>(item)->SetEntryName(newItemName);
    }
    else
    {
        throw CFItemNotFound(L"Item " + oldItemName + L" not found in Storage");
    }

    children.reset();
    children = LoadChildren(dirEntry.lock()->getSid());

    if (children == nullptr)
    {
        children = compoundFile->CreateNewTree();
    }
}

std::shared_ptr<RBTree> CFStorage::LoadChildren(_INT32 SID)
{
    std::shared_ptr<RBTree> childrenTree = compoundFile->GetChildrenTree(SID);

    if (childrenTree->getRoot() != nullptr)
    {
        dirEntry.lock()->setChild((std::static_pointer_cast<IDirectoryEntry>(childrenTree->getRoot())->getSid()));
    }
    else
    {
        dirEntry.lock()->setChild(DirectoryEntry::NOSTREAM);
    }

    return childrenTree;
}

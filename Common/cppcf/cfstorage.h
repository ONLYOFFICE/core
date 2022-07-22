#pragma once

#include "RBTree/rbtree.h"
#include "idirectoryentry.h"
#include "cfstream.h"


namespace CFCPP
{
class CFStorage : public CFItem
{
public:
    CFStorage(CompoundFile* compFile, const std::weak_ptr<IDirectoryEntry> &dirEntry);


    std::shared_ptr<RedBlackTree::RBTree> getChildren();
    std::shared_ptr<CFStream> AddStream(const std::wstring& streamName);
    std::shared_ptr<CFStream> GetStream(const std::wstring& streamName);

    bool TryGetStream(const std::wstring& streamName, std::shared_ptr<CFStream> &cfStream);
    std::shared_ptr<CFStorage> GetStorage(const std::wstring& storageName);
    std::shared_ptr<CFStorage> TryGetStorage(const std::wstring& storageName);
    bool TryGetStorage(const std::wstring& storageName, std::shared_ptr<CFStorage> &cfStorage);
    std::shared_ptr<CFStorage> AddStorage(const std::wstring& storageName);
    void VisitEntries(RedBlackTree::Action<std::shared_ptr<CFItem>> action, bool recursive);
    void Delete(const std::wstring& entryName);
    void RenameItem(const std::wstring& oldItemName, const std::wstring& newItemName);
    std::streamsize size() const {return CFItem::size();}

private:
    std::shared_ptr<RedBlackTree::RBTree> LoadChildren(int SID);

private:
    std::shared_ptr<RedBlackTree::RBTree> children;
};
}

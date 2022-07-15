#pragma once

#include "cfitem.h"
#include "RBTree/rbtree.h"
#include "compoundfile.h"
#include "idirectoryentry.h"
#include "cfstream.h"


namespace CFCPP
{
class CFStorage : public CFItem
{
public:
    CFStorage(std::shared_ptr<const CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry);


    std::shared_ptr<RedBlackTree::RBTree> getChildren();
    std::shared_ptr<CFStream> AddStream(const std::wstring& streamName);
    std::shared_ptr<CFStream> GetStream(const std::wstring& streamName);

    bool TryGetStream(const std::wstring& streamName, std::shared_ptr<CFStream> &cfStream);
    std::shared_ptr<CFStorage> GetStorage(const std::wstring& storageName);
    std::shared_ptr<CFStorage> TryGetStorage(const std::wstring& storageName);
    bool TryGetStorage(const std::wstring& storageName, std::shared_ptr<CFStorage> &cfStorage);
    std::shared_ptr<CFStorage> AddStorage(const std::wstring& storageName);
    void VisitEntries(RedBlackTree::Action<PCFItem> action, bool recursive);
    void Delete(const std::wstring& entryName);
    void RenameItem(const std::wstring& oldItemName, const std::wstring& newItemName);
    std::streamsize size() const {return CFItem::size();}
    std::shared_ptr<IDirectoryEntry> getDirEntry() const {return dirEntry;}

private:
    std::shared_ptr<RedBlackTree::RBTree> LoadChildren(int SID);

private:
    std::shared_ptr<RedBlackTree::RBTree> children;
    std::shared_ptr<IDirectoryEntry> dirEntry;
};
}

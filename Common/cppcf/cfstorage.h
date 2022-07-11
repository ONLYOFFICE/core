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
    CFStorage(std::shared_ptr<CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry);


    std::shared_ptr<RedBlackTree::RBTree> getChildren();
    std::shared_ptr<CFStream> AddStream(std::wstring streamName);
    std::shared_ptr<CFStream> GetStream(std::wstring streamName);

private:
    std::shared_ptr<RedBlackTree::RBTree> LoadChildren(int SID);

private:
    std::shared_ptr<RedBlackTree::RBTree> children;
    std::shared_ptr<IDirectoryEntry> dirEntry;
};
}

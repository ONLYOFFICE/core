#pragma once

#include "cfitem.h"
#include "RBTree/rbtree.h"
#include "compoundfile.h"
#include "idirectoryentry.h"

namespace CFCPP
{
class CFStorage : public CFItem
{
public:
    CFStorage();


    RedBlackTree::RBTree getChildren() const;

private:
    RedBlackTree::RBTree LoadChildren(int SID);

private:
    std::shared_ptr<RedBlackTree::RBTree> children;
    std::shared_ptr<IDirectoryEntry> dirEntry;
};
}

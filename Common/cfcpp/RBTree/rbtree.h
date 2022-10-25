#pragma once

#include "irbnode.h"
#include <iterator>
#include <functional>
#include <list>


namespace RedBlackTree
{
template  <class T>
using Action = std::function<void(T)>;


class RBTree
{
public:
    RBTree(){}
    RBTree(PIRBNode root);

    const PIRBNode getRoot() const;
    void setRoot(const PIRBNode &newRoot);

    bool TryLookup(PIRBNode pattern, PIRBNode& val);
    void Insert(PIRBNode newNode);
    void Delete(PIRBNode pattern, PIRBNode& deletedAlt);

    void VisitTree(Action<PIRBNode> action);
    void VisitTreeNodes(Action<PIRBNode> action);

private:
    static Color NodeColor(PIRBNode n);
    static PIRBNode MaximumNode(PIRBNode node);

    PIRBNode LookupNode(PIRBNode pattern);
    void ReplaceNode(PIRBNode oldn, PIRBNode newn);
    void RotateLeft(PIRBNode node);
    void RotateRight(PIRBNode node);

    void InsertCase1(PIRBNode node);
    void InsertCase2(PIRBNode node);
    void InsertCase3(PIRBNode node);
    void InsertCase4(PIRBNode node);
    void InsertCase5(PIRBNode node);

    void DeleteCase1(PIRBNode node);
    void DeleteCase2(PIRBNode node);
    void DeleteCase3(PIRBNode node);
    void DeleteCase4(PIRBNode node);
    void DeleteCase5(PIRBNode node);
    void DeleteCase6(PIRBNode node);

    void DoVisitTree(Action<PIRBNode> action, PIRBNode walker);
    void DoVisitTreeNodes(Action<PIRBNode> action, PIRBNode walker);

public:

   class iterator : public std::iterator<std::output_iterator_tag, std::ptrdiff_t, IRBNode, IRBNode*, PIRBNode>
   {
       PIRBNode current;
       RBTree* tree;
   public:
       iterator(RBTree *tree);
       iterator& operator++();
       inline bool operator==(const iterator &other) const {return current == other.current;}
       inline bool operator!=(const iterator &other) const {return current != other.current;}
       inline PIRBNode operator*() {return current;}
   };

   iterator begin() {return iterator(this);}
   iterator end() {return iterator(nullptr);}

private:
    PIRBNode root;
};

}

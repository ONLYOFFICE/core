#include "rbtree.h"
#include "rbtreeexception.h"

using namespace RedBlackTree;

RBTree::RBTree(PIRBNode root) : root(root)
{}

const PIRBNode RBTree::getRoot() const
{
    return root;
}

void RBTree::setRoot(const PIRBNode &newRoot)
{
    root = newRoot;
}

bool RBTree::TryLookup(PIRBNode templ, PIRBNode &val)
{
    PIRBNode n = LookupNode(templ);

    if (n == nullptr)
    {
        val.reset();
        return false;
    }
    else
    {
        val = n;
        return true;
    }
}

void RBTree::Insert(PIRBNode newNode)
{
    newNode->setColor(Color::RED);
    PIRBNode insertedNode = newNode;

    if (getRoot() == nullptr)
    {
        setRoot(insertedNode);
    }
    else
    {
        PIRBNode n = getRoot();
        while (true)
        {
            int compResult = newNode->CompareTo(n);
            if (compResult == 0)
            {
                throw new RBTreeDuplicatedItemException(L"RBNode " + newNode->ToString() + L" already present in tree");
                //n->Value = value;
                //return;
            }
            else if (compResult < 0)
            {
                if (n->getLeft() == nullptr)
                {
                    n->setLeft(insertedNode);

                    break;
                }
                else
                {
                    n = n->getLeft();
                }
            }
            else
            {
                //assert compResult > 0;
                if (n->getRight() == nullptr)
                {
                    n->setRight(insertedNode);

                    break;
                }
                else
                {
                    n = n->getRight();
                }
            }
        }
        insertedNode->setParent(n);
    }

    InsertCase1(insertedNode);

//    if (NodeInserted != nullptr)
//    {
//        NodeInserted(insertedNode);
//    }

    //Trace.WriteLine(" ");
    //Print();
}

void RBTree::Delete(PIRBNode templ, PIRBNode &deletedAlt)
{
    deletedAlt.reset();
    PIRBNode n = LookupNode(templ);
    templ = n;
    if (n == nullptr)
        return;  // Key not found, do nothing
    if (n->getLeft() != nullptr && n->getRight() != nullptr)
    {
        // Copy key/value from predecessor and then delete it instead
        PIRBNode pred = MaximumNode(n->getLeft());
        pred->AssignValueTo(n);
        n = pred;
        deletedAlt = pred;
    }

    //assert n->left == null || n->right == null;
    PIRBNode child = (n->getRight() == nullptr) ? n->getLeft() : n->getRight();
    if (NodeColor(n) == Color::BLACK)
    {
        n->setColor(NodeColor(child));
        DeleteCase1(n);
    }

    ReplaceNode(n, child);

    if (NodeColor(getRoot()) == Color::RED)
    {
        getRoot()->setColor(Color::BLACK);
    }


    return;
}

void RBTree::VisitTree(Action<PIRBNode> action)
{
    PIRBNode walker = getRoot();

    if (walker != nullptr)
        DoVisitTree(action, walker);
}

Color RBTree::NodeColor(PIRBNode n)
{
    return n == nullptr ? Color::BLACK : n->getColor();
}

PIRBNode RBTree::MaximumNode(PIRBNode n)
{
    while (n->getRight() != nullptr)
    {
        n = n->getRight();
    }

    return n;
}

PIRBNode RBTree::LookupNode(PIRBNode templ)
{
    PIRBNode n = getRoot();

    while (n != nullptr)
    {
        int compResult = templ->CompareTo(n);

        if (compResult == 0)
        {
            return n;
        }
        else if (compResult < 0)
        {
            n = n->getLeft();
        }
        else
        {
            //assert compResult > 0;
            n = n->getRight();
        }
    }

    return n;
}

void RBTree::ReplaceNode(PIRBNode oldn, PIRBNode newn)
{
    if (oldn->getParent() == nullptr)
    {
        setRoot(newn);
    }
    else
    {
        if (oldn == oldn->getParent()->getLeft())
            oldn->getParent()->setLeft(newn);
        else
            oldn->getParent()->setRight(newn);
    }
    if (newn != nullptr)
    {
        newn->setParent(oldn->getParent());
    }
}

void RBTree::RotateLeft(PIRBNode n)
{
    PIRBNode r = n->getRight();
    ReplaceNode(n, r);
    n->setRight(r->getLeft());
    if (r->getLeft() != nullptr)
    {
        r->getLeft()->setParent(n);
    }
    r->setLeft(n);
    n->setParent(r);
}

void RBTree::RotateRight(PIRBNode n)
{
    PIRBNode l = n->getLeft();
    ReplaceNode(n, l);
    n->setLeft(l->getRight());

    if (l->getRight() != nullptr)
    {
        l->getRight()->setParent(n);
    }

    l->setRight(n);
    n->setParent(l);
}

void RBTree::InsertCase1(PIRBNode n)
{
    if (n->getParent() == nullptr)
        n->setColor(Color::BLACK);
    else
        InsertCase2(n);
}

void RBTree::InsertCase2(PIRBNode n)
{
    if (NodeColor(n->getParent()) == Color::BLACK)
        return; // Tree is still valid
    else
        InsertCase3(n);
}

void RBTree::InsertCase3(PIRBNode n)
{
    if (NodeColor(n->Uncle()) == Color::RED)
    {
        n->getParent()->setColor(Color::BLACK);
        n->Uncle()->setColor(Color::BLACK);
        n->Grandparent()->setColor(Color::RED);
        InsertCase1(n->Grandparent());
    }
    else
    {
        InsertCase4(n);
    }
}

void RBTree::InsertCase4(PIRBNode n)
{
    if (n == n->getParent()->getRight() && n->getParent() == n->Grandparent()->getLeft())
    {
        RotateLeft(n->getParent());
        n = n->getLeft();
    }
    else if (n == n->getParent()->getLeft() && n->getParent() == n->Grandparent()->getRight())
    {
        RotateRight(n->getParent());
        n = n->getRight();
    }

    InsertCase5(n);
}

void RBTree::InsertCase5(PIRBNode n)
{
    n->getParent()->setColor(Color::BLACK);
    n->Grandparent()->setColor(Color::RED);
    if (n == n->getParent()->getLeft() && n->getParent() == n->Grandparent()->getLeft())
    {
        RotateRight(n->Grandparent());
    }
    else
    {
        //assert n == n->getParent().right && n->getParent() == n->grandparent().right;
        RotateLeft(n->Grandparent());
    }
}

void RBTree::DeleteCase1(PIRBNode n)
{
    if (n->getParent() == nullptr)
        return;
    else
        DeleteCase2(n);
}

void RBTree::DeleteCase2(PIRBNode n)
{
    if (NodeColor(n->Sibling()) == Color::RED)
    {
        n->getParent()->setColor(Color::RED);
        n->Sibling()->setColor(Color::BLACK);
        if (n == n->getParent()->getLeft())
            RotateLeft(n->getParent());
        else
            RotateRight(n->getParent());
    }

    DeleteCase3(n);
}

void RBTree::DeleteCase3(PIRBNode n)
{
    if (NodeColor(n->getParent()) == Color::BLACK &&
            NodeColor(n->Sibling()) == Color::BLACK &&
            NodeColor(n->Sibling()->getLeft()) == Color::BLACK &&
            NodeColor(n->Sibling()->getRight()) == Color::BLACK)
    {
        n->Sibling()->setColor(Color::RED);
        DeleteCase1(n->getParent());
    }
    else
        DeleteCase4(n);
}

void RBTree::DeleteCase4(PIRBNode n)
{
    if (NodeColor(n->getParent()) == Color::RED &&
            NodeColor(n->Sibling()) == Color::BLACK &&
            NodeColor(n->Sibling()->getLeft()) == Color::BLACK &&
            NodeColor(n->Sibling()->getRight()) == Color::BLACK)
    {
        n->Sibling()->setColor(Color::RED);
        n->getParent()->setColor(Color::BLACK);
    }
    else
        DeleteCase5(n);
}

void RBTree::DeleteCase5(PIRBNode n)
{
    if (n == n->getParent()->getLeft() &&
            NodeColor(n->Sibling()) == Color::BLACK &&
            NodeColor(n->Sibling()->getLeft()) == Color::RED &&
            NodeColor(n->Sibling()->getRight()) == Color::BLACK)
    {
        n->Sibling()->setColor(Color::RED);
        n->Sibling()->getLeft()->setColor(Color::BLACK);
        RotateRight(n->Sibling());
    }
    else if (n == n->getParent()->getRight() &&
             NodeColor(n->Sibling()) == Color::BLACK &&
             NodeColor(n->Sibling()->getRight()) == Color::RED &&
             NodeColor(n->Sibling()->getLeft()) == Color::BLACK)
    {
        n->Sibling()->setColor(Color::RED);
        n->Sibling()->getRight()->setColor(Color::BLACK);
        RotateLeft(n->Sibling());
    }

    DeleteCase6(n);
}

void RBTree::DeleteCase6(PIRBNode n)
{
    n->Sibling()->setColor(NodeColor(n->getParent()));
    n->getParent()->setColor(Color::BLACK);
    if (n == n->getParent()->getLeft())
    {
        //assert nodeColor(n->sibling().right) == Color::RED;
        n->Sibling()->getRight()->setColor(Color::BLACK);
        RotateLeft(n->getParent());
    }
    else
    {
        //assert nodeColor(n->sibling().left) == Color::RED;
        n->Sibling()->getLeft()->setColor(Color::BLACK);
        RotateRight(n->getParent());
    }
}

void RBTree::DoVisitTree(Action<PIRBNode> action, PIRBNode walker)
{
    if (walker->getLeft() != nullptr)
    {
        DoVisitTree(action, walker->getLeft());
    }

    if (action != nullptr)
        action(walker);

    if (walker->getRight() != nullptr)
    {
        DoVisitTree(action, walker->getRight());
    }
}

void RBTree::VisitTreeNodes(Action<PIRBNode> action)
{
    //IN Order visit
    PIRBNode walker = getRoot();

    if (walker != nullptr)
        DoVisitTreeNodes(action, walker);
}

void RBTree::DoVisitTreeNodes(Action<PIRBNode> action, PIRBNode walker)
{
    if (walker->getLeft() != nullptr)
    {
        DoVisitTreeNodes(action, walker->getLeft());
    }

    if (action != nullptr)
        action(walker);

    if (walker->getRight() != nullptr)
    {

        DoVisitTreeNodes(action, walker->getRight());
    }
}

RBTree::iterator::iterator(RBTree &tree, bool end)
{
    Action<PIRBNode> inserter = [&] (PIRBNode pNode)
    {
        heap.push_back(pNode);
    };
    tree.VisitTreeNodes(inserter);
    current = end ? heap.end() : heap.begin();
}

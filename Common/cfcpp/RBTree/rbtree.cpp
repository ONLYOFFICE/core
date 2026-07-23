/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "rbtree.h"
#include "rbtreeexception.h"


using namespace RedBlackTree;

RBTree::RBTree(PIRBNode root) :
    root(root)
{}

const PIRBNode RBTree::getRoot() const
{
    return root;
}

void RBTree::setRoot(const PIRBNode &newRoot)
{
    root = newRoot;
}

bool RBTree::TryLookup(PIRBNode pattern, PIRBNode &val)
{
    PIRBNode node = LookupNode(pattern);

    if (node == nullptr)
    {
        val.reset();
        return false;
    }
    else
    {
        val = node;
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
        PIRBNode node = getRoot();
        while (true)
        {
            auto compResult = newNode->CompareTo(node);
            if (compResult == 0)
            {
                throw RBTreeDuplicatedItemException(L"RBNode " + newNode->ToString() + L" already exists in tree");
            }
            else if (compResult < 0)
            {
                if (node->getLeft() == nullptr)
                {
                    node->setLeft(insertedNode);

                    break;
                }
                else
                {
                    node = node->getLeft();
                }
            }
            else
            {
                if (node->getRight() == nullptr)
                {
                    node->setRight(insertedNode);

                    break;
                }
                else
                {
                    node = node->getRight();
                }
            }
        }
        insertedNode->setParent(node);
    }

    InsertCase1(insertedNode);
}

void RBTree::Delete(PIRBNode pattern, PIRBNode &deletedAlt)
{
    deletedAlt.reset();
    PIRBNode node = LookupNode(pattern);
    pattern = node;
    if (node == nullptr)
        return;  // Key not found
    if (node->getLeft() != nullptr && node->getRight() != nullptr)
    {
        PIRBNode pred = MaximumNode(node->getLeft());
        pred->AssignValueTo(node);
        node = pred;
        deletedAlt = pred;
    }

    PIRBNode child = (node->getRight() == nullptr) ? node->getLeft() : node->getRight();
    if (NodeColor(node) == Color::BLACK)
    {
        node->setColor(NodeColor(child));
        DeleteCase1(node);
    }

    ReplaceNode(node, child);

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

Color RBTree::NodeColor(PIRBNode node)
{
    return node == nullptr ? Color::BLACK : node->getColor();
}

PIRBNode RBTree::MaximumNode(PIRBNode node)
{
    while (node->getRight() != nullptr)
    {
        node = node->getRight();
    }

    return node;
}

PIRBNode RBTree::LookupNode(PIRBNode pattern)
{
    PIRBNode node = getRoot();

    while (node != nullptr)
    {
        auto compResult = pattern->CompareTo(node);

        if (compResult == 0)
        {
            return node;
        }
        else if (compResult < 0)
        {
            node = node->getLeft();
        }
        else // compResult > 0;
        {
            node = node->getRight();
        }
    }

    return node;
}

void RBTree::ReplaceNode(PIRBNode oldNode, PIRBNode newNode)
{
    if (oldNode->getParent() == nullptr)
    {
        setRoot(newNode);
    }
    else
    {
        if (oldNode == oldNode->getParent()->getLeft())
            oldNode->getParent()->setLeft(newNode);
        else
            oldNode->getParent()->setRight(newNode);
    }
    if (newNode != nullptr)
    {
        newNode->setParent(oldNode->getParent());
    }
}

void RBTree::RotateLeft(PIRBNode node)
{
    PIRBNode right = node->getRight();
    ReplaceNode(node, right);
    node->setRight(right->getLeft());
    if (right->getLeft() != nullptr)
    {
        right->getLeft()->setParent(node);
    }
    right->setLeft(node);
    node->setParent(right);
}

void RBTree::RotateRight(PIRBNode node)
{
    PIRBNode left = node->getLeft();
    ReplaceNode(node, left);
    node->setLeft(left->getRight());

    if (left->getRight() != nullptr)
    {
        left->getRight()->setParent(node);
    }

    left->setRight(node);
    node->setParent(left);
}

void RBTree::InsertCase1(PIRBNode node)
{
    if (node->getParent() == nullptr)
        node->setColor(Color::BLACK);
    else
        InsertCase2(node);
}

void RBTree::InsertCase2(PIRBNode node)
{
    if (NodeColor(node->getParent()) == Color::BLACK)
        return;
    else
        InsertCase3(node);
}

void RBTree::InsertCase3(PIRBNode node)
{
    if (NodeColor(node->Uncle()) == Color::RED)
    {
        node->getParent()->setColor(Color::BLACK);
        node->Uncle()->setColor(Color::BLACK);
        node->Grandparent()->setColor(Color::RED);
        InsertCase1(node->Grandparent());
    }
    else
    {
        InsertCase4(node);
    }
}

void RBTree::InsertCase4(PIRBNode node)
{
    if (node == node->getParent()->getRight() && node->getParent() == node->Grandparent()->getLeft())
    {
        RotateLeft(node->getParent());
        node = node->getLeft();
    }
    else if (node == node->getParent()->getLeft() && node->getParent() == node->Grandparent()->getRight())
    {
        RotateRight(node->getParent());
        node = node->getRight();
    }

    InsertCase5(node);
}

void RBTree::InsertCase5(PIRBNode node)
{
    node->getParent()->setColor(Color::BLACK);
    node->Grandparent()->setColor(Color::RED);
    if (node == node->getParent()->getLeft() && node->getParent() == node->Grandparent()->getLeft())
    {
        RotateRight(node->Grandparent());
    }
    else
    {
        RotateLeft(node->Grandparent());
    }
}

void RBTree::DeleteCase1(PIRBNode node)
{
    if (node->getParent() == nullptr)
        return;
    else
        DeleteCase2(node);
}

void RBTree::DeleteCase2(PIRBNode node)
{
    if (NodeColor(node->Sibling()) == Color::RED)
    {
        node->getParent()->setColor(Color::RED);
        node->Sibling()->setColor(Color::BLACK);
        if (node == node->getParent()->getLeft())
            RotateLeft(node->getParent());
        else
            RotateRight(node->getParent());
    }

    DeleteCase3(node);
}

void RBTree::DeleteCase3(PIRBNode node)
{
    if (NodeColor(node->getParent()) == Color::BLACK &&
            NodeColor(node->Sibling()) == Color::BLACK &&
            NodeColor(node->Sibling()->getLeft()) == Color::BLACK &&
            NodeColor(node->Sibling()->getRight()) == Color::BLACK)
    {
        node->Sibling()->setColor(Color::RED);
        DeleteCase1(node->getParent());
    }
    else
        DeleteCase4(node);
}

void RBTree::DeleteCase4(PIRBNode node)
{
    if (NodeColor(node->getParent()) == Color::RED &&
            NodeColor(node->Sibling()) == Color::BLACK &&
            NodeColor(node->Sibling()->getLeft()) == Color::BLACK &&
            NodeColor(node->Sibling()->getRight()) == Color::BLACK)
    {
        node->Sibling()->setColor(Color::RED);
        node->getParent()->setColor(Color::BLACK);
    }
    else
        DeleteCase5(node);
}

void RBTree::DeleteCase5(PIRBNode node)
{
    if (node == node->getParent()->getLeft() &&
            NodeColor(node->Sibling()) == Color::BLACK &&
            NodeColor(node->Sibling()->getLeft()) == Color::RED &&
            NodeColor(node->Sibling()->getRight()) == Color::BLACK)
    {
        node->Sibling()->setColor(Color::RED);
        node->Sibling()->getLeft()->setColor(Color::BLACK);
        RotateRight(node->Sibling());
    }
    else if (node == node->getParent()->getRight() &&
             NodeColor(node->Sibling()) == Color::BLACK &&
             NodeColor(node->Sibling()->getRight()) == Color::RED &&
             NodeColor(node->Sibling()->getLeft()) == Color::BLACK)
    {
        node->Sibling()->setColor(Color::RED);
        node->Sibling()->getRight()->setColor(Color::BLACK);
        RotateLeft(node->Sibling());
    }

    DeleteCase6(node);
}

void RBTree::DeleteCase6(PIRBNode node)
{
    node->Sibling()->setColor(NodeColor(node->getParent()));
    node->getParent()->setColor(Color::BLACK);
    if (node == node->getParent()->getLeft())
    {
        node->Sibling()->getRight()->setColor(Color::BLACK);
        RotateLeft(node->getParent());
    }
    else
    {
        node->Sibling()->getLeft()->setColor(Color::BLACK);
        RotateRight(node->getParent());
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

RBTree::iterator::iterator(RBTree *tree) : tree(tree)
{
    if (tree == nullptr)
        return;

    auto walker = tree->getRoot();
    while (walker != nullptr)
    {
        current = walker;
        walker = walker->getLeft();
    }
}

RBTree::iterator &RBTree::iterator::operator++()
{
    if (current->getRight())
    {
        current = current->getRight();
        auto walker = tree->getRoot();
        while (walker != nullptr)
        {
            current = walker;
            walker = walker->getLeft();
        }
    }
    else if (current->getParent())
    {
        current = current->getParent();
        return *this;
    }
    else
    {
        current.reset();
    }

    return *this;
}
